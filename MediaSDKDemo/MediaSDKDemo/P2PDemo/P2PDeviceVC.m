//
//  P2PDeviceVC.m
//  MediaSDKDemo
//
//  Created by Joe_Liu on 2018/8/6.
//  Copyright © 2018年 Joe_Liu. All rights reserved.
//

#import "P2PDeviceVC.h"
#import <TUTKP2P/TUTKP2P.h>
#import "CallSession.h"
#import "iToast.h"

#define VIDEO_BUF_SIZE    (1024 * 300)
#define AUDIO_BUF_SIZE    1024

@interface P2PDeviceVC ()<TK_p2p_Delegate>
@property (weak, nonatomic) IBOutlet UITextField *uidTF;

@property (nonatomic, copy)NSString  *myIDString;

@property (nonatomic, strong)NSMutableArray         *callSessionList;


@property (nonatomic, assign)BOOL isRunSendVideoThread;
@property (nonatomic, assign)BOOL isRunSendAudioThread;

@end

@implementation P2PDeviceVC

- (NSMutableArray *)callSessionList {
    if (!_callSessionList){
        _callSessionList = [NSMutableArray arrayWithCapacity:0];
    }
    return _callSessionList;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.myIDString = @"111111";
    [[TUTKP2P shareInstance] setP2pDelegate:self];
    
    self.uidTF.text = [[NSUserDefaults standardUserDefaults] objectForKey:@"UID"];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    [super touchesBegan:touches withEvent:event];
    
    [self.view endEditing:YES];
}

- (IBAction)BackClick:(id)sender {
    [self LogoutClick:nil];
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (IBAction)LoginClick:(id)sender {
    if (self.uidTF.text.length == 20) {
        [[NSUserDefaults standardUserDefaults] setObject:self.uidTF.text forKey:@"UID"];
        [[TUTKP2P shareInstance] TK_init];
        [[TUTKP2P shareInstance] TK_device_loginWithUID:self.uidTF.text Password:@"888888ii"];
    }
}

- (IBAction)LogoutClick:(id)sender {
    [self DisconnectClick:nil];
    [[TUTKP2P shareInstance] TK_device_logout];
    [[TUTKP2P shareInstance] TK_unInit];
}

- (IBAction)DisconnectClick:(id)sender {
    for (CallSession *callsession in self.callSessionList) {
        SMsgAVIoctrlCallEnd resp = {0};
        strncpy(&resp.myID, [self.myIDString UTF8String], sizeof(resp.myID));
        NSData *data = [NSData dataWithBytes:&resp length:sizeof(SMsgAVIoctrlCallEnd)];
        [[TUTKP2P shareInstance] TK_device_sendIOCtrl:IOTYPE_USER_IPCAM_CALL_END data:data session:callsession.session];
    }
    
    [self stopSendAudioData];
    [self stopSendVideoData];
    
    usleep(200*1000);
    [[TUTKP2P shareInstance] TK_device_disconnectAll];
    [self.callSessionList removeAllObjects];
}

- (IBAction)SendIOCtrlClick:(id)sender {
    
    for (CallSession *callSession in self.callSessionList) {
        if (callSession.session) {
            SMsgAVIoctrlIDInfo info = {0};
            info.count = 1;
            unsigned char *accID = (unsigned char *)[self.myIDString UTF8String];
            unsigned char *accUID = (unsigned char *)[self.uidTF.text UTF8String];
            strncpy(info.partiInfo->myID, accID, sizeof(char)*6);
            strncpy(info.partiInfo->myUID, accUID, sizeof(char)*6);
            NSData *data = [NSData dataWithBytes:&info length:sizeof(SMsgAVIoctrlIDInfo)];
            [[TUTKP2P shareInstance] TK_client_sendIOCtrl:IOTYPE_USER_IPCAM_IDINFO data:data session:callSession.session];
        }
    }
}

- (void)disconnectWithSession:(AVSession *)session {
    dispatch_async(dispatch_get_main_queue(), ^{
        
        CallSession *exitCallSession = nil;
        for (CallSession *callSession in self.callSessionList) {
            if ([callSession.session isEqual:session]) {
                exitCallSession = callSession;
                break;
            }
        }
        
        if (exitCallSession) {
            
            SMsgAVIoctrlCallEnd resp = {0};
            strncpy(&resp.myID, [self.myIDString UTF8String], sizeof(resp.myID));
            NSData *data = [NSData dataWithBytes:&resp length:sizeof(SMsgAVIoctrlCallEnd)];
            [[TUTKP2P shareInstance] TK_device_sendIOCtrl:IOTYPE_USER_IPCAM_CALL_END data:data session:exitCallSession.session];
            
            NSString *message = [NSString stringWithFormat:@"%@ 已断线",exitCallSession.idString];
            [self showTipsWithText:message];

            usleep(200*1000);

            [[TUTKP2P shareInstance] TK_device_disconnect:exitCallSession.session];

            [self.callSessionList removeObject:exitCallSession];

            if (self.callSessionList.count == 0) {

            }
        }
    });
}

- (void)showTipsWithText:(NSString *)text {
    dispatch_async(dispatch_get_main_queue(), ^{
        [[[[iToast makeText: text] setGravity:iToastGravityBottom] setDuration:iToastDurationShort] show];
    });
}

- (void)startSendVideoData
{
    if (!self.isRunSendVideoThread) {
        NSString *videoPath = [[NSBundle mainBundle] pathForResource:@"video" ofType:@"VGA"];
        NSThread *sendVideoThread = [[NSThread alloc]initWithTarget:self
                                                           selector:@selector(thread_sendVideoData:)
                                                             object:videoPath];
        [sendVideoThread setName:@"sendVideo"];
        [sendVideoThread start];
    }
}

- (void)stopSendVideoData
{
    self.isRunSendVideoThread = NO;
}

- (void)startSendAudioData
{
    if (!self.isRunSendAudioThread) {
        NSString *audioPath = [[NSBundle mainBundle] pathForResource:@"8k_16bit_mono" ofType:@"raw"];
        
        NSThread *sendAudioThread = [[NSThread alloc]initWithTarget:self
                                                           selector:@selector(thread_sendAudioData:)
                                                             object:audioPath];
        [sendAudioThread setName:@"sendAudio"];
        [sendAudioThread start];
    }
}

- (void)stopSendAudioData
{
    self.isRunSendAudioThread = NO;
}

- (void)thread_sendVideoData:(NSString *)videoPath
{
    char buf[VIDEO_BUF_SIZE];
    FILE *fp = NULL;
    
    int fps = 25;
    int sleepTick = 1000000/fps;
    
    fp = fopen([videoPath UTF8String], "rb");
    if(fp == NULL)
    {
        NSLog(@"Device sendVideoData: Video File \'%@\' open error!!\n", videoPath);
        return;
    }
    
    int size = fread(buf, 1, VIDEO_BUF_SIZE, fp);
    fclose(fp);
    if(size <= 0)
    {
        NSLog(@"Device sendVideoData: Video File \'%@\' read error!!\n", videoPath);
        return;
    }
    
    NSLog(@"Device sendVideoData start OK");
    
    NSData *videoData = [NSData dataWithBytes:buf length:size];
    
    self.isRunSendVideoThread = YES;
    
    while(self.isRunSendVideoThread)
    {
        for (AVSession *session in [[TUTKP2P shareInstance] TK_device_getAVSessions])
        {
            unsigned long long timestamp = [self getTimeStamp];
            [[TUTKP2P shareInstance] TK_device_onSendVideoData:videoData isIFrame:YES timeStamp:timestamp session:session];
        }
        NSLog(@"Device sendVideo size: %ld", videoData.length);
        usleep(sleepTick);
    }
    
    NSLog(@"Device sendVideoData stop");
}

- (void)thread_sendAudioData:(NSString *)audioPath
{
    FILE *fp=NULL;
    char buf[AUDIO_BUF_SIZE];
    int fps = 25;
    int sleepTick = 1000000/fps;
    
    fp = fopen([audioPath UTF8String], "rb");
    if(fp == NULL)
    {
        NSLog(@"Device sendAudioData: Audio File \'%@\' open error!!\n", audioPath);
        return;
    }
    
    int size = fread(buf, 1, AUDIO_BUF_SIZE, fp);
    fclose(fp);
    if(size <= 0)
    {
        NSLog(@"Device sendAudioData: Audio File \'%@\' read error!!\n", audioPath);
        return;
    }
    
    NSLog(@"Device sendAudioData start OK");
    
    NSData *audioData = [NSData dataWithBytes:buf length:size];
    
    self.isRunSendAudioThread = YES;
    
    while(self.isRunSendAudioThread)
    {
        for (AVSession *session in [[TUTKP2P shareInstance] TK_device_getAVSessions])
        {
            unsigned long long timestamp = [self getTimeStamp];
            [[TUTKP2P shareInstance] TK_device_onSendAudioData:audioData timeStamp:timestamp session:session];
        }
        NSLog(@"Device sendAudio size: %ld", audioData.length);
        usleep(sleepTick);
    }
    
    NSLog(@"Device sendAudioData stop");
}

#pragma mark - TUTKP2P Delegate
- (void)TK_device_outputIOCtrlData:(NSData *)data type:(NSInteger)type session:(AVSession *)session
{
    if (type == IOTYPE_USER_IPCAM_CALL_REQ)
    {
        SMsgAVIoctrlCallReq *req = (SMsgAVIoctrlCallReq *)data.bytes;
        
        unsigned char *myId = malloc(sizeof(req->myID)+1);
        memset(myId, 0, sizeof(req->myID)+1);
        memcpy(myId, req->myID, sizeof(req->myID));
        NSString *idString = [NSString stringWithUTF8String:myId];
        free(myId);
        
        unsigned char *myUID = malloc(sizeof(req->myUID)+1);
        memset(myUID, 0, sizeof(req->myUID)+1);
        memcpy(myUID, req->myUID, sizeof(req->myUID));
        NSString *uidString = [NSString stringWithUTF8String:myUID];
        free(myUID);
        
        dispatch_async(dispatch_get_main_queue(), ^{
            for (CallSession *callSession in self.callSessionList) {
                if ([callSession.idString isEqualToString:idString]){
                    return;
                }
            }
            CallSession *callSession = [[CallSession alloc] init];
            callSession.idString = idString;
            callSession.session = session;
            [self.callSessionList addObject:callSession];
            
            NSString *message = [NSString stringWithFormat:@"%@ 在呼叫你",idString];
            [self showTipsWithText:message];
            
            // 答应对方的呼叫
            SMsgAVIoctrlCallResp resp = {0};
            resp.answer = 1;
            strncpy(&resp.myID, [self.myIDString UTF8String], sizeof(resp.myID));
            NSData *data = [NSData dataWithBytes:&resp length:sizeof(SMsgAVIoctrlCallResp)];
            [[TUTKP2P shareInstance] TK_device_sendIOCtrl:IOTYPE_USER_IPCAM_CALL_RESP data:data session:session];
        });
        
    }else if (type == IOTYPE_USER_IPCAM_CALL_END)
    {
        [self disconnectWithSession:session];
        
    }else if (type == IOTYPE_USER_IPCAM_START)
    {
        [self startSendVideoData];
        
    }else if (type == IOTYPE_USER_IPCAM_AUDIOSTART)
    {
        [self startSendAudioData];
        
    }else if (type == IOTYPE_USER_IPCAM_SPEAKERSTART)
    {
        
    }else if (type == IOTYPE_USER_IPCAM_STOP)
    {
        [self stopSendVideoData];
        
    }else if (type == IOTYPE_USER_IPCAM_AUDIOSTOP)
    {
        [self stopSendAudioData];
        
    }else if (type == IOTYPE_USER_IPCAM_IDINFO) {
        
        SMsgAVIoctrlIDInfo *info = (SMsgAVIoctrlIDInfo *)data.bytes;
        
        unsigned char *myId = malloc(sizeof(char)*6+1);
        memset(myId, 0, sizeof(char)*6+1);
        memcpy(myId, info->partiInfo->myID, sizeof(char)*6);
        NSString *idString = [NSString stringWithUTF8String:myId];
        free(myId);
        
        unsigned char *myUID = malloc(sizeof(char)*20+1);
        memset(myUID, 0, sizeof(char)*20+1);
        memcpy(myUID, info->partiInfo->myUID, sizeof(char)*20);
        NSString *uidString = [NSString stringWithUTF8String:myUID];
        free(myUID);
        
        NSString *msg = [NSString stringWithFormat:@"%@ 发来指令",idString];
        [self showTipsWithText:msg];
    }
}

- (void)TK_device_sessionStatus:(int)state session:(AVSession *)session
{
    if (state == DEV_STATE_LOGINING) {
        
    }else if (state == DEV_STATE_LOGINED) {
        NSLog(@"连接成功!");
        [self showTipsWithText:NSLocalizedString(@"Login成功", @"")];
    }else if (state == DEV_STATE_EXIT_LISTEN) {
        NSLog(@"Listen失败!");
    }else if (state == DEV_STATE_EXCEED_MAX_SESSION) {
        NSLog(@"Session满了!");
    }else if (state == DEV_STATE_NEW_CLIENT) {
        NSLog(@"有client连接!");
        [self showTipsWithText:NSLocalizedString(@"有client连接", @"")];
    }else if (state == DEV_STATE_CONNECTING) {
        
    }else if (state == DEV_STATE_CONNECTED) {
        
    }else if (state == DEV_STATE_DISCONNECTED) {
        [self disconnectWithSession:session];
    }else if (state == DEV_STATE_CONNECT_FAILED) {
        [self disconnectWithSession:session];
    }
}

- (void)TK_device_outputVideoData:(NSData *) data
                       isKeyFrame:(BOOL)isKeyFrame
                        timeStamp:(unsigned long long)timeStamp
                          session:(AVSession *)session
{
    NSLog(@"Device recvVideoData size: %ld",data.length);
}

- (void)TK_device_outputAudioData:(NSData *)data
                        timeStamp:(unsigned long long)timeStamp
                          session:(AVSession *)session
{
    NSLog(@"Device recvAudioData size:%ld", data.length);
}


- (unsigned long long)getTimeStamp
{
    NSDate* date = [NSDate dateWithTimeIntervalSinceNow:0];
    NSTimeInterval timeInterval = [date timeIntervalSince1970]*1000;
    return (unsigned long long)timeInterval;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end

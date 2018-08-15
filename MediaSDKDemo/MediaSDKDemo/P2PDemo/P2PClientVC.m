//
//  P2PClientVC.m
//  MediaSDKDemo
//
//  Created by Joe_Liu on 2018/8/6.
//  Copyright © 2018年 Joe_Liu. All rights reserved.
//

#import "P2PClientVC.h"
#import <TUTKP2P/TUTKP2P.h>
#import "iToast.h"

#define VIDEO_BUF_SIZE    (1024 * 300)
#define AUDIO_BUF_SIZE    1024

@interface P2PClientVC ()<TK_p2p_Delegate>
@property (weak, nonatomic) IBOutlet UITextField *uidTF;

@property (nonatomic, copy)NSString  *myIDString;

@property (nonatomic, copy)NSString *idString;
@property (nonatomic, strong)AVSession *session;

@property (nonatomic, assign)BOOL isRunSendVideoThread;
@property (nonatomic, assign)BOOL isRunSendAudioThread;

@end

@implementation P2PClientVC

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.myIDString = @"222222";
    [[TUTKP2P shareInstance] setP2pDelegate:self];
    
    self.uidTF.text = [[NSUserDefaults standardUserDefaults] objectForKey:@"UID"];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    [super touchesBegan:touches withEvent:event];
    
    [self.view endEditing:YES];
}

- (IBAction)BackClick:(id)sender {
    [self DisconnectClick:nil];
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (IBAction)ConnectClick:(id)sender {
    if (self.uidTF.text.length == 20) {
        [[NSUserDefaults standardUserDefaults] setObject:self.uidTF.text forKey:@"UID"];
        [[TUTKP2P shareInstance] TK_init];
        [[TUTKP2P shareInstance] TK_client_connectWithUID:self.uidTF.text password:@"888888ii" channel:0];
    }
}

- (IBAction)DisconnectClick:(id)sender {
    [self StopSendVideoClick:nil];
    [self StopSpeakClick:nil];
    [self disconnectWithSession:self.session];
}

- (IBAction)StartSendVideoClick:(id)sender {
    if (self.session) {
        [[TUTKP2P shareInstance] TK_client_startSendVideo:self.session];
    }
    
    if (!self.isRunSendVideoThread) {
        NSString *videoPath = [[NSBundle mainBundle] pathForResource:@"video" ofType:@"VGA"];
        NSThread *sendVideoThread = [[NSThread alloc]initWithTarget:self
                                                           selector:@selector(thread_sendVideoData:)
                                                             object:videoPath];
        [sendVideoThread setName:@"sendVideo"];
        [sendVideoThread start];
    }
}

- (IBAction)StopSendVideoClick:(id)sender {
    if (self.session) {
        [[TUTKP2P shareInstance] TK_client_stopSendVideo:self.session];
    }
    
    self.isRunSendVideoThread = NO;
}

- (IBAction)StartReceiveVideoClick:(id)sender {
    if (self.session) {
        [[TUTKP2P shareInstance] TK_client_startRecvVideo:self.session];
    }
}

- (IBAction)StopReceiveVideoClick:(id)sender {
    if (self.session) {
        [[TUTKP2P shareInstance] TK_client_stopRecvVideo:self.session];
    }
}

- (IBAction)StartSpeakClick:(id)sender {
    if (self.session) {
        [[TUTKP2P shareInstance] TK_client_startSpeaking:self.session];
    }
    
    if (!self.isRunSendAudioThread) {
        NSString *audioPath = [[NSBundle mainBundle] pathForResource:@"8k_16bit_mono" ofType:@"raw"];
        
        NSThread *sendAudioThread = [[NSThread alloc]initWithTarget:self
                                                           selector:@selector(thread_sendAudioData:)
                                                             object:audioPath];
        [sendAudioThread setName:@"sendAudio"];
        [sendAudioThread start];
    }
}

- (IBAction)StopSpeakClick:(id)sender {
    if (self.session) {
        [[TUTKP2P shareInstance] TK_client_stopSpeaking:self.session];
    }
    
    self.isRunSendAudioThread = NO;
}

- (IBAction)StartListenerClick:(id)sender {
    if (self.session) {
        [[TUTKP2P shareInstance] TK_client_startListener:self.session];
    }
}

- (IBAction)StopListenerClick:(id)sender {
    if (self.session) {
        [[TUTKP2P shareInstance] TK_client_stopListener:self.session];
    }
}

- (IBAction)SendIOCtrlClick:(id)sender {
    if (self.session && self.uidTF.text.length == 20) {
        SMsgAVIoctrlIDInfo info = {0};
        info.count = 1;
        unsigned char *accID = (unsigned char *)[self.myIDString UTF8String];
        unsigned char *accUID = (unsigned char *)[self.uidTF.text UTF8String];
        strncpy(info.partiInfo->myID, accID, sizeof(char)*6);
        strncpy(info.partiInfo->myUID, accUID, sizeof(char)*6);
        NSData *data = [NSData dataWithBytes:&info length:sizeof(SMsgAVIoctrlIDInfo)];
        [[TUTKP2P shareInstance] TK_client_sendIOCtrl:IOTYPE_USER_IPCAM_IDINFO data:data session:self.session];
    }
}

// 断开连线
- (void)disconnectWithSession:(AVSession *)session {
    
    SMsgAVIoctrlCallEnd resp = {0};
    strncpy(&resp.myID, [self.myIDString UTF8String], sizeof(resp.myID));
    NSData *data = [NSData dataWithBytes:&resp length:sizeof(SMsgAVIoctrlCallEnd)];
    [[TUTKP2P shareInstance] TK_client_sendIOCtrl:IOTYPE_USER_IPCAM_CALL_END data:data session:session];
    
    NSString *message = [NSString stringWithFormat:@"%@ 已断线",self.idString];
    [self showTipsWithText:message];
    
    usleep(200*1000);
    
    self.session = nil;
    [[TUTKP2P shareInstance] TK_client_disconnectAll];
    [[TUTKP2P shareInstance] TK_unInit];
}

- (void)showTipsWithText:(NSString *)text {
    dispatch_async(dispatch_get_main_queue(), ^{
        [[[[iToast makeText: text] setGravity:iToastGravityBottom] setDuration:iToastDurationShort] show];
    });
}

- (void)thread_sendVideoData:(NSString *)videoPath
{
    char buf[VIDEO_BUF_SIZE];
    FILE *fp = NULL;
    
    int fps = 25;
    int sleepTick = 1000000/fps;
    
    fp = fopen([videoPath UTF8String], "rb");
    if(fp == NULL){
        NSLog(@"Client sendVideoData: Video File \'%@\' open error!!\n", videoPath);
        return;
    }
    
    int size = fread(buf, 1, VIDEO_BUF_SIZE, fp);
    fclose(fp);
    if(size <= 0)
    {
        NSLog(@"Client sendVideoData: Video File \'%@\' read error!!\n", videoPath);
        return;
    }
    
    NSLog(@"Client sendVideoData start OK");
    
    [[TUTKP2P shareInstance] TK_client_startSendVideo:self.session];
    NSData *videoData = [NSData dataWithBytes:buf length:size];
    
    self.isRunSendVideoThread = YES;
    
    while(self.isRunSendVideoThread)
    {
        unsigned long long timestamp = [self getTimeStamp];
        [[TUTKP2P shareInstance] TK_client_onSendVideoData:videoData isIFrame:YES timeStamp:timestamp session:self.session];
        NSLog(@"Client sendVideo size: %ld", videoData.length);
        usleep(sleepTick);
    }
    
    [[TUTKP2P shareInstance] TK_client_stopSendVideo:self.session];
    NSLog(@"Client sendVideoData stop");
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
        NSLog(@"Client sendAudioData: Audio File \'%@\' open error!!\n", audioPath);
        return;
    }
    
    int size = fread(buf, 1, AUDIO_BUF_SIZE, fp);
    fclose(fp);
    if(size <= 0)
    {
        NSLog(@"Client sendAudioData: Audio File \'%@\' read error!!\n", audioPath);
        return;
    }
    
    NSLog(@"Client sendAudioData start OK");
    
    [[TUTKP2P shareInstance] TK_client_startSpeaking:self.session];
    NSData *audioData = [NSData dataWithBytes:buf length:size];
    
    self.isRunSendAudioThread = YES;
    
    while(self.isRunSendAudioThread)
    {
        unsigned long long timestamp = [self getTimeStamp];
        [[TUTKP2P shareInstance] TK_client_onSendAudioData:audioData timeStamp:timestamp session:self.session];
        
        NSLog(@"Client sendAudio size: %ld", audioData.length);
        usleep(sleepTick);
    }
    
    [[TUTKP2P shareInstance] TK_client_stopSpeaking:self.session];
    NSLog(@"Client sendAudioData stop");
}

#pragma mark - TUTKP2P Delegate
// Client端连线状态回调
- (void)TK_client_sessionStatus:(int)state session:(AVSession *)session
{
    if (state == CLIENT_STATE_NEW_CONNECTED )
    {
        self.session = session;
        
        dispatch_async(dispatch_get_main_queue(), ^{
            // 呼叫对方
            SMsgAVIoctrlCallReq req = {0};
            req.callType = 2; //twoway
            unsigned char *accID = (unsigned char *)[self.myIDString UTF8String];
            unsigned char *accUID = (unsigned char *)[self.uidTF.text UTF8String];
            strncpy(&req.myID, accID, sizeof(req.myID));
            strncpy(&req.myUID, accUID, sizeof(req.myUID));
            NSData *data = [NSData dataWithBytes:&req length:sizeof(SMsgAVIoctrlCallReq)];
            [[TUTKP2P shareInstance] TK_client_sendIOCtrl:IOTYPE_USER_IPCAM_CALL_REQ data:data session:self.session];
        });
        
    }else if (state == CLIENT_STATE_DISCONNECTED)
    {
        [self disconnectWithSession:session];
        
    }else if (state == CLIENT_STATE_UNKNOWN_DEVICE)
    {
        [self disconnectWithSession:session];
        
    }else if (state == CLIENT_STATE_WRONG_PASSWORD)
    {
        NSLog(@"密码错误");
        [self disconnectWithSession:session];
        
    }else if (state == CLIENT_STATE_TIMEOUT)
    {
        [self disconnectWithSession:session];
        
    }else if (state == CLIENT_STATE_CONNECT_FAILED)
    {
        [self disconnectWithSession:session];
    }
}

- (void)TK_client_outputIOCtrlData:(NSData *)data type:(NSInteger)type session:(AVSession *)session
{
    if (type == IOTYPE_USER_IPCAM_CALL_RESP)
    {
        SMsgAVIoctrlCallResp *resp = (SMsgAVIoctrlCallResp *)data.bytes;
        if (resp->answer) {
            
            unsigned char *myId = malloc(sizeof(resp->myID)+1);
            memset(myId, 0, sizeof(resp->myID)+1);
            memcpy(myId, resp->myID, sizeof(resp->myID));
            self.idString = [NSString stringWithUTF8String:myId];
            free(myId);
            
            NSString *message = [NSString stringWithFormat:@"%@ 已接听",self.idString];
            [self showTipsWithText:message];
        }
    }else if (type == IOTYPE_USER_IPCAM_CALL_END)
    {
        [self disconnectWithSession:session];
        
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

- (void)TK_client_outputVideoData:(NSData *) data
                       isKeyFrame:(BOOL)isKeyFrame
                        timeStamp:(unsigned long long)timeStamp
                          session:(AVSession *)session
{
    NSLog(@"Client recvVideoData size: %ld",data.length);
}

- (void)TK_client_outputAudioData:(NSData *)data
                        timeStamp:(unsigned long long)timeStamp
                          session:(AVSession *)session
{
    NSLog(@"Client recvAudioData size:%ld", data.length);
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

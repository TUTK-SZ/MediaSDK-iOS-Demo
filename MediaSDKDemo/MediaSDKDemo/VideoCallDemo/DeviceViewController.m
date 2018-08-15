//
//  DeviceViewController.m
//  Demo
//
//  Created by Joe_Liu on 2018/7/27.
//  Copyright © 2018年 Joe_Liu. All rights reserved.
//

#import "DeviceViewController.h"
#import <TUTKP2P/TUTKP2P.h>
#import <TUTKMedia/TUTKMedia.h>
#import <AssetsLibrary/AssetsLibrary.h>
#import "iToast.h"
#import "CallSession.h"

#define weakself(object) __weak typeof(self) weakself = object

@interface DeviceViewController ()<TK_p2p_Delegate, TK_media_Delegate>
@property (weak, nonatomic) IBOutlet UITextField    *uidTF;
@property (weak, nonatomic) IBOutlet UIView *previewView;
@property (weak, nonatomic) IBOutlet UIImageView *displayView;

@property (nonatomic, copy)NSString  *myIDString;

@property (nonatomic, strong)NSMutableArray         *callSessionList;

@end

@implementation DeviceViewController

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
    [[TKMedia shareTKMedia] setMediaDelegate:self];
    
    self.uidTF.text = [[NSUserDefaults standardUserDefaults] objectForKey:@"UID"];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    [super touchesBegan:touches withEvent:event];
    
    [self.view endEditing:YES];
}

// 返回
- (IBAction)backClick:(id)sender {
    [self logoutClick:nil];
    [self dismissViewControllerAnimated:YES completion:nil];
}

// Login
- (IBAction)loginClick:(id)sender {
    
    if (self.uidTF.text.length == 20) {
        [[NSUserDefaults standardUserDefaults] setObject:self.uidTF.text forKey:@"UID"];
        [[TUTKP2P shareInstance] TK_init];
        [[TUTKP2P shareInstance] TK_device_loginWithUID:self.uidTF.text Password:@"888888ii"];
    }
}

// Logout
- (IBAction)logoutClick:(id)sender {
    [self disconnectClick:nil];
    [[TUTKP2P shareInstance] TK_device_logout];
    [[TUTKP2P shareInstance] TK_unInit];
}

// 断线
- (IBAction)disconnectClick:(id)sender {
    for (CallSession *callsession in self.callSessionList) {
        SMsgAVIoctrlCallEnd resp = {0};
        strncpy(&resp.myID, [self.myIDString UTF8String], sizeof(resp.myID));
        NSData *data = [NSData dataWithBytes:&resp length:sizeof(SMsgAVIoctrlCallEnd)];
        [[TUTKP2P shareInstance] TK_device_sendIOCtrl:IOTYPE_USER_IPCAM_CALL_END data:data session:callsession.session];
        
        [self stopMediaDecoderWithSession:callsession.session];
    }
    [self stopMediaCapture];
    
    usleep(200*1000);
    [[TUTKP2P shareInstance] TK_device_disconnectAll];
    [self.callSessionList removeAllObjects];
}

// 发送指令
- (IBAction)sendIOCtrlClick:(id)sender {
    
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

// 截图
- (IBAction)snapshotClick:(id)sender {
    //相册权限
    ALAuthorizationStatus authorAblum = [ALAssetsLibrary authorizationStatus];
    if (authorAblum == ALAuthorizationStatusRestricted || authorAblum == ALAuthorizationStatusDenied)
    {
        [self showTipsWithText:NSLocalizedString(@"请开启相册权限", @"")];
        return;
    }
    
    CallSession *callSession = self.callSessionList.firstObject;
    NSString *tag = [NSString stringWithFormat:@"%@", callSession.session];
    
    NSString *documentsPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,
                                                                   NSUserDomainMask,
                                                                   YES) lastObject];
    NSString *snapshotPath = [documentsPath stringByAppendingPathComponent:@"snapshot.jpg"];
    [[TKMedia shareTKMedia] TK_video_snapShotWithPath:snapshotPath tag:tag];
    [self showTipsWithText:@"The photo has been saved to the local phone."];
}

// 本地录像
- (IBAction)recordClick:(id)sender {
    //相册权限
    ALAuthorizationStatus authorAblum = [ALAssetsLibrary authorizationStatus];
    if (authorAblum == ALAuthorizationStatusRestricted || authorAblum == ALAuthorizationStatusDenied)
    {
        [self showTipsWithText:NSLocalizedString(@"请开启相册权限", @"")];
        return;
    }
    
    UIButton *btn = (UIButton *)sender;
    btn.selected = !btn.selected;
    if (btn.selected) {
        [btn setTitle:@"Stop Record" forState:UIControlStateSelected];
        [self startRecord:YES];
    }else {
        [btn setTitle:@"Start Record" forState:UIControlStateNormal];
        [self startRecord:NO];
    }
}

// 开始音视频的采集编码预览
- (void)startMediaCapture {
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [[TKMedia shareTKMedia] TK_preview_changeResolution:PreviewResolution_Low
                                                        fps:PreviewFPS_Low
                                                    bitrate:PreviewBitRate_High
                                                presetBlock:nil];
        [[TKMedia shareTKMedia] TK_preview_startCapture:self.previewView videoFormat:VideoFormat_H264 isSWEncode:NO];
        [[TKMedia shareTKMedia] TK_preview_setOrientation:AVCaptureVideoOrientationPortrait];
        
        [[TKMedia shareTKMedia] TK_audio_startAudioUnit:AudioCategory_playAndRecord sampleRate:SampleRate_8K];
        [[TKMedia shareTKMedia] TK_encode_initAudioCodeID:AudioFormat_AAC_ADTS
                                               sampleRate:SampleRate_8K
                                                  bitRate:AudioDataBits_16
                                                 channels:AudioChannels_MONO];
    });
}

// 停止音视频的采集编码预览
- (void)stopMediaCapture {
    [[TKMedia shareTKMedia] TK_preview_stopCapture];
    [[TKMedia shareTKMedia] TK_audio_stopAudioUnit];
    [[TKMedia shareTKMedia] TK_encode_deInitAudioEncode];
    
    dispatch_async(dispatch_get_main_queue(), ^{
        self.displayView.layer.sublayers = nil;
    });
}

// 开始音视频的解码
- (void)startMediaDecoderWithSession:(AVSession *)session {
    NSString *tag = [NSString stringWithFormat:@"%@",session];
    [[TKMedia shareTKMedia] TK_decode_startDecodeVideo:self.displayView tag:tag];
    
    [[TKMedia shareTKMedia] TK_decode_initAudioCodeID:AudioFormat_AAC_ADTS
                                           sampleRate:SampleRate_8K
                                              bitRate:AudioDataBits_16
                                             channels:AudioChannels_MONO
                                                  tag:tag];
    [[TKMedia shareTKMedia] TK_video_setScaleType:VideoScale_ResizeAspectFill tag:tag];
}

// 开始音视频的解码
- (void)stopMediaDecoderWithSession:(AVSession *)session {
    NSString *tag = [NSString stringWithFormat:@"%@",session];
    [[TKMedia shareTKMedia] TK_decode_stopDecodeVideo:tag];
    [[TKMedia shareTKMedia] TK_decode_deInitAudioDecode:tag];
}

// 断开连线
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
            
            [self stopMediaDecoderWithSession:exitCallSession.session];
            
            NSString *message = [NSString stringWithFormat:@"%@ 已断线",exitCallSession.idString];
            [self showTipsWithText:message];
            
            usleep(200*1000);
            
            [[TUTKP2P shareInstance] TK_device_disconnect:exitCallSession.session];
            
            [self.callSessionList removeObject:exitCallSession];
            
            if (self.callSessionList.count == 0) {
                [self stopMediaCapture];
            }
        }
    });
}

/**
 本地录像操作
 @param startRecord 开始/结束
 */
- (void)startRecord:(BOOL)startRecord
{
    NSString *documentsPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,
                                                                   NSUserDomainMask,
                                                                   YES) lastObject];
    NSString *videoPath = [documentsPath stringByAppendingPathComponent:@"video.mp4"];
    
    CallSession *callSession = self.callSessionList.firstObject;
    NSString *tag = [NSString stringWithFormat:@"%@", callSession.session];
    
    weakself(self);
    [[TKMedia shareTKMedia] TK_video_startRecording:startRecord
                                        videoFormat:VideoFormat_H264
                                         sampleRate:SampleRate_8K
                                            dataBit:AudioDataBits_16
                                           channels:AudioChannels_MONO
                                           withPath:videoPath
                                                tag:tag
                                         completion:^(NSError *error) {
                                             if (!error) {
                                                 NSLog(@"Save MP4 success !");
                                                 if (!startRecord) {
                                                     [weakself showTipsWithText:@"The video has been saved to the local phone."];
                                                 }
                                             }else {
                                                 NSLog(@"Save MP4 error:%@",error);
                                             }
                                         }];
}

#pragma mark - TUTKP2P Delegate
// Device端接收的指令回调
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
        });
        
        // 答应对方的呼叫
        SMsgAVIoctrlCallResp resp = {0};
        resp.answer = 1;
        strncpy(&resp.myID, [self.myIDString UTF8String], sizeof(resp.myID));
        NSData *data = [NSData dataWithBytes:&resp length:sizeof(SMsgAVIoctrlCallResp)];
        [[TUTKP2P shareInstance] TK_device_sendIOCtrl:IOTYPE_USER_IPCAM_CALL_RESP data:data session:session];
        
        [self startMediaCapture];
        [self startMediaDecoderWithSession:session];
        
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

// Device端连线状态回调
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

// Device端接收的视频回调
- (void)TK_device_outputVideoData:(NSData *) data
                       isKeyFrame:(BOOL)isKeyFrame
                        timeStamp:(unsigned long long)timeStamp
                          session:(AVSession *)session
{
    NSString *tag = [NSString stringWithFormat:@"%@",session];
    @autoreleasepool
    {
        [[TKMedia shareTKMedia]  TK_deocde_onReceiveVideoData:data timeStamp:timeStamp tag:tag];
    }
}

// Device端接收的音频回调
- (void)TK_device_outputAudioData:(NSData *)data
                        timeStamp:(unsigned long long)timeStamp
                          session:(AVSession *)session
{
    NSString *tag = [NSString stringWithFormat:@"%@",session];
    @autoreleasepool
    {
        [[TKMedia shareTKMedia] TK_deocde_onReceiveAudioData:data timeStamp:timeStamp tag:tag];
    }
}

#pragma mark - TUTKMedia Delegate
// 采集的音频回调
- (void)TK_audio_outputAudioData:(NSData *)data
                       timeStamp:(unsigned long long)timeStamp
{
    @autoreleasepool
    {
        [[TKMedia shareTKMedia] TK_encode_onReceiveAudioData:data timeStamp:timeStamp];
    }
}

// 编码的视频回调
- (void)TK_encode_outputVideoData:(NSData *) data
                       isKeyFrame:(BOOL)isKeyFrame
                        timeStamp:(unsigned long long)timeStamp
{
    @autoreleasepool
    {
        for (AVSession *session in [[TUTKP2P shareInstance] TK_device_getAVSessions])
        {
            [[TUTKP2P shareInstance] TK_device_onSendVideoData:data isIFrame:isKeyFrame timeStamp:timeStamp session:session];
        }
        
        for (AVSession *session in [[TUTKP2P shareInstance] TK_client_getAVSessions])
        {
            [[TUTKP2P shareInstance] TK_client_onSendVideoData:data isIFrame:isKeyFrame timeStamp:timeStamp session:session];
        }
    }
}

// 编码的音频回调
- (void)TK_encode_outputAudioData:(NSData *)data
                        timeStamp:(unsigned long long)timeStamp
{
    @autoreleasepool
    {
        for (AVSession *session in [[TUTKP2P shareInstance] TK_device_getAVSessions])
        {
            [[TUTKP2P shareInstance] TK_device_onSendAudioData:data  timeStamp:timeStamp session:session];
        }
        
        for (AVSession *session in [[TUTKP2P shareInstance] TK_client_getAVSessions])
        {
            [[TUTKP2P shareInstance] TK_client_onSendAudioData:data timeStamp:timeStamp session:session];
        }
    }
}

// 解码的音频回调
- (void)TK_decode_outputAudioData:(NSData *)data tag:(NSString *)tag
{
    @autoreleasepool
    {
        [[TKMedia shareTKMedia] TK_audio_onPlayAudioData:data tag:tag];
    }
}

// Tips显示
- (void)showTipsWithText:(NSString *)text {
    dispatch_async(dispatch_get_main_queue(), ^{
        [[[[iToast makeText: text] setGravity:iToastGravityBottom] setDuration:iToastDurationShort] show];
    });
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

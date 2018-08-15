//
//  ClientViewController.m
//  Demo
//
//  Created by Joe_Liu on 2018/7/27.
//  Copyright © 2018年 Joe_Liu. All rights reserved.
//

#import "ClientViewController.h"
#import <TUTKP2P/TUTKP2P.h>
#import <TUTKMedia/TUTKMedia.h>
#import <AssetsLibrary/AssetsLibrary.h>
#import "iToast.h"

#define weakself(object) __weak typeof(self) weakself = object

@interface ClientViewController ()<TK_p2p_Delegate, TK_media_Delegate>
@property (weak, nonatomic) IBOutlet UITextField *uidTF;
@property (weak, nonatomic) IBOutlet UIView *previewView;
@property (weak, nonatomic) IBOutlet UIImageView *displayView;

@property (nonatomic, copy)NSString  *myIDString;

@property (nonatomic, copy)NSString *idString;
@property (nonatomic, strong)AVSession *session;

@end

@implementation ClientViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.myIDString = @"222222";
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
    [self disconnectWithSession:self.session];
    [self dismissViewControllerAnimated:YES completion:nil];
}

// 连接
- (IBAction)connectClick:(id)sender {
    if (self.uidTF.text.length == 20) {
        [[NSUserDefaults standardUserDefaults] setObject:self.uidTF.text forKey:@"UID"];
        [[TUTKP2P shareInstance] TK_init];
        [[TUTKP2P shareInstance] TK_client_connectWithUID:self.uidTF.text password:@"888888ii" channel:0];
    }
}

// 断线
- (IBAction)disconnectClick:(id)sender {
    [self disconnectWithSession:self.session];
}

// 开始发送视频
- (IBAction)startSendVideoClick:(id)sender {
    if (self.session) {
        [[TUTKP2P shareInstance] TK_client_startSendVideo:self.session];
    }
}

// 停止发送视频
- (IBAction)stopSendVideoClick:(id)sender {
    if (self.session) {
        [[TUTKP2P shareInstance] TK_client_stopSendVideo:self.session];
    }
}

// 开始接收视频
- (IBAction)startRecvVideoClick:(id)sender {
    if (self.session) {
        [[TUTKP2P shareInstance] TK_client_startRecvVideo:self.session];
    }
}

// 停止接收视频
- (IBAction)stopRecvVideoClick:(id)sender {
    if (self.session) {
        [[TUTKP2P shareInstance] TK_client_stopRecvVideo:self.session];
    }
}

// 开始对讲
- (IBAction)startSpeakClick:(id)sender {
    if (self.session) {
        [[TUTKP2P shareInstance] TK_client_startSpeaking:self.session];
    }
}

// 停止对讲
- (IBAction)stopSpeakClick:(id)sender {
    if (self.session) {
        [[TUTKP2P shareInstance] TK_client_stopSpeaking:self.session];
    }
}

// 开始监听
- (IBAction)startListenerClick:(id)sender {
    if (self.session) {
        [[TUTKP2P shareInstance] TK_client_startListener:self.session];
    }
}

// 停止监听
- (IBAction)stopListenerClick:(id)sender {
    if (self.session) {
        [[TUTKP2P shareInstance] TK_client_stopListener:self.session];
    }
}

// 发送指令
- (IBAction)sendIOCtrlClick:(id)sender {
    
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

// 截图
- (IBAction)snapshotClick:(id)sender {
    //相册权限
    ALAuthorizationStatus authorAblum = [ALAssetsLibrary authorizationStatus];
    if (authorAblum == ALAuthorizationStatusRestricted || authorAblum == ALAuthorizationStatusDenied)
    {
        [self showTipsWithText:NSLocalizedString(@"请开启相册权限", @"")];
        return;
    }
    
    NSString *tag = [NSString stringWithFormat:@"%@", self.session];
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

// 停止音视频的解码
- (void)stopMediaDecoderWithSession:(AVSession *)session {
    NSString *tag = [NSString stringWithFormat:@"%@",session];
    [[TKMedia shareTKMedia] TK_decode_stopDecodeVideo:tag];
    [[TKMedia shareTKMedia] TK_decode_deInitAudioDecode:tag];
}

// 断开连线
- (void)disconnectWithSession:(AVSession *)session {
    
    SMsgAVIoctrlCallEnd resp = {0};
    strncpy(&resp.myID, [self.myIDString UTF8String], sizeof(resp.myID));
    NSData *data = [NSData dataWithBytes:&resp length:sizeof(SMsgAVIoctrlCallEnd)];
    [[TUTKP2P shareInstance] TK_client_sendIOCtrl:IOTYPE_USER_IPCAM_CALL_END data:data session:session];
    
    [self stopMediaDecoderWithSession:self.session];
    [self stopMediaCapture];
    
    if (self.idString.length == 6) {
        NSString *message = [NSString stringWithFormat:@"%@ 已断线",self.idString];
        [self showTipsWithText:message];
    }
    
    usleep(200*1000);
    
    self.session = nil;
    [[TUTKP2P shareInstance] TK_client_disconnectAll];
    [[TUTKP2P shareInstance] TK_unInit];
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
    
    NSString *tag = [NSString stringWithFormat:@"%@", self.session];
    
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
        
        [self startMediaCapture];
        [self startMediaDecoderWithSession:self.session];
        
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

// Client端接收的指令回调
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

// Client端接收的视频回调
- (void)TK_client_outputVideoData:(NSData *) data
                       isKeyFrame:(BOOL)isKeyFrame
                        timeStamp:(unsigned long long)timeStamp
                          session:(AVSession *)session
{
    NSString *tag = [NSString stringWithFormat:@"%@",session];
    @autoreleasepool
    {
        [[TKMedia shareTKMedia] TK_deocde_onReceiveVideoData:data timeStamp:timeStamp tag:tag];
    }
}

// Client端接收的音频回调
- (void)TK_client_outputAudioData:(NSData *)data
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

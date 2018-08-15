//
//  MediaViewController.m
//  MediaSDKDemo
//
//  Created by Joe_Liu on 2018/8/6.
//  Copyright © 2018年 Joe_Liu. All rights reserved.
//

#import "MediaViewController.h"
#import <TUTKMedia/TUTKMedia.h>
#import <AssetsLibrary/AssetsLibrary.h>
#import "iToast.h"

#define weakself(object) __weak typeof(self) weakself = object

@interface MediaViewController () <TK_media_Delegate> {
    int i;
}
@property (weak, nonatomic) IBOutlet UIView *previewView;
@property (weak, nonatomic) IBOutlet UIImageView *displayView;
@property (weak, nonatomic) IBOutlet UIButton *recordBtn;

@end

@implementation MediaViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    //相机权限
    [AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo completionHandler:^(BOOL granted)
     {
         if (granted)
         {
             NSLog(@"Authorized");
         }else
         {
             NSLog(@"Denied or Restricted");
         }
     }];
    
    i = 1;
     [[TKMedia shareTKMedia] setMediaDelegate:self];
}
- (IBAction)BackClick:(id)sender {
    [self StartRecordClick:self.recordBtn];
    [self StopClick:nil];
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (IBAction)StartClick:(id)sender {
    [[TKMedia shareTKMedia] TK_preview_startCapture:self.previewView videoFormat:VideoFormat_H264 isSWEncode:NO];
    [[TKMedia shareTKMedia] TK_preview_setSaleType:VideoScale_ResizeAspect];
    [[TKMedia shareTKMedia] TK_preview_setOrientation:AVCaptureVideoOrientationPortrait];
    
    [[TKMedia shareTKMedia] TK_decode_startDecodeVideo:self.displayView tag:@"1"];
    
    [[TKMedia shareTKMedia] TK_encode_initAudioCodeID:AudioFormat_AAC_ADTS
                                           sampleRate:SampleRate_8K
                                              bitRate:AudioDataBits_16
                                             channels:AudioChannels_MONO];
    [[TKMedia shareTKMedia] TK_decode_initAudioCodeID:AudioFormat_AAC_ADTS
                                           sampleRate:SampleRate_8K
                                              bitRate:AudioDataBits_16
                                             channels:AudioChannels_MONO
                                                  tag:@"1"];
    [[TKMedia shareTKMedia] TK_audio_startAudioUnit:AudioCategory_playAndRecord sampleRate:SampleRate_8K];
}

- (IBAction)StopClick:(id)sender {
    [[TKMedia shareTKMedia] TK_preview_stopCapture];
    [[TKMedia shareTKMedia] TK_decode_stopDecodeVideo:@"1"];
    
    [[TKMedia shareTKMedia] TK_audio_stopAudioUnit];
    [[TKMedia shareTKMedia] TK_decode_deInitAudioDecode:@"1"];
    [[TKMedia shareTKMedia] TK_encode_deInitAudioEncode];
    
    self.displayView.layer.sublayers = nil;
}

- (IBAction)ToggleClick:(id)sender {
    [[TKMedia shareTKMedia] TK_preview_switchCamera:^(BOOL isBackCamera)
     {
         NSLog(@"切换到%@置摄像头了",isBackCamera?@"后":@"前");
     }];
    
    [[TKMedia shareTKMedia] TK_preview_setOrientation:AVCaptureVideoOrientationPortrait];
}
- (IBAction)ResolutionClick:(id)sender {
    i++;
    if (i % 2 == 0)
    {
        [[TKMedia shareTKMedia] TK_preview_changeResolution:PreviewResolution_High
                                                        fps:PreviewFPS_High
                                                    bitrate:PreviewBitRate_Highest
                                                presetBlock:^(BOOL isSupport) {
                                                    NSLog(@"%@支持",isSupport ? @"":@"不");
                                                }];
        [[TKMedia shareTKMedia] TK_preview_setOrientation:AVCaptureVideoOrientationPortrait];
    }else
    {
        [[TKMedia shareTKMedia] TK_preview_changeResolution:PreviewResolution_Low
                                                        fps:PreviewFPS_Low
                                                    bitrate:PreviewBitRate_High
                                                presetBlock:^(BOOL isSupport) {
                                                    NSLog(@"%@支持",isSupport ? @"":@"不");
                                                }];
        [[TKMedia shareTKMedia] TK_preview_setOrientation:AVCaptureVideoOrientationPortrait];
    }
}

- (IBAction)SnapshotClick:(id)sender {
    //相册权限
    ALAuthorizationStatus authorAblum = [ALAssetsLibrary authorizationStatus];
    if (authorAblum == ALAuthorizationStatusRestricted || authorAblum == ALAuthorizationStatusDenied)
    {
        [self showTipsWithText:NSLocalizedString(@"请开启相册权限", @"")];
        return;
    }
    
    NSString *documentsPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,
                                                                   NSUserDomainMask,
                                                                   YES) lastObject];
    NSString *snapshotPath = [documentsPath stringByAppendingPathComponent:@"snapshot.jpg"];
    
    [[TKMedia shareTKMedia] TK_video_snapShotWithPath:snapshotPath tag:@"1"];
}

- (IBAction)StartRecordClick:(id)sender {
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
    
    weakself(self);
    [[TKMedia shareTKMedia] TK_video_startRecording:startRecord
                                        videoFormat:VideoFormat_H264
                                         sampleRate:SampleRate_8K
                                            dataBit:AudioDataBits_16
                                           channels:AudioChannels_MONO
                                           withPath:videoPath
                                                tag:@"1"
                                         completion:^(NSError *error) {
                                             if (!error) {
                                                 NSLog(@"Save MP4 success !");
                                             }else {
                                                 NSLog(@"Save MP4 error:%@",error);
                                             }
                                         }];
}

#pragma mark - TUTKMedia Delegate
- (void)TK_encode_outputVideoData:(NSData *)data isKeyFrame:(BOOL)isKeyFrame timeStamp:(unsigned long long)timeStamp
{
    @autoreleasepool
    {
        [[TKMedia shareTKMedia] TK_deocde_onReceiveVideoData:data timeStamp:timeStamp tag:@"1"];
    }
}


- (void)TK_audio_outputAudioData:(NSData *)data timeStamp:(unsigned long long)timeStamp
{
    [[TKMedia shareTKMedia] TK_encode_onReceiveAudioData:data timeStamp:timeStamp];
}

- (void)TK_encode_outputAudioData:(NSData *)data timeStamp:(unsigned long long)timeStamp
{
    [[TKMedia shareTKMedia] TK_deocde_onReceiveAudioData:data timeStamp:timeStamp tag:@"1"];
}

- (void)TK_decode_outputAudioData:(NSData *)data tag:(NSString *)tag
{
    if ([tag isEqualToString:@"1"]) {
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

//
//  TKMedia.h
//  TUTKMedia
//
//  Created by Joe_Liu on 2018/1/15.
//  Copyright © 2018年 Joe_Liu. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreMedia/CoreMedia.h>
#import <AVFoundation/AVFoundation.h>

typedef void (^SwitchCameraBlock) (BOOL isBackCamera);
typedef void (^PresetBlock) (BOOL isSupport);

typedef NS_ENUM(NSInteger, AudioCategory)
{
    AudioCategory_playAndRecord   = 0,
    AudioCategory_playback        = 1,
    AudioCategory_record          = 2,
};

typedef NS_ENUM(NSInteger, AudioFormat)
{
    AudioFormat_AAC_RAW = 0x86 ,
    AudioFormat_AAC_ADTS ,
    AudioFormat_AAC_LATM ,
    AudioFormat_G711U ,
    AudioFormat_G711A ,
    AudioFormat_ADPCM ,
    AudioFormat_PCM ,
    AudioFormat_SPEEX ,
    AudioFormat_MP3 ,
    AudioFormat_G726 //0x8F
};

typedef NS_ENUM(NSInteger, VideoFormat)
{
    VideoFormat_H264 = 0,
    VideoFormat_HEVC
};

typedef NS_ENUM(NSInteger, AudioChannels)
{
    AudioChannels_MONO = 0,
    AudioChannels_STEREO
};

typedef NS_ENUM(NSInteger, AudioDataBits)
{
    AudioDataBits_8 = 0,
    AudioDataBits_16
};

typedef NS_ENUM(NSInteger, SampleRate)
{
    SampleRate_8K = 0,
    SampleRate_16K
};

typedef NS_ENUM(NSInteger, PreviewResolution)
{
    PreviewResolution_Low     = 320*240 ,
    PreviewResolution_Medium  = 640*480 ,
    PreviewResolution_High    = 1280*720
};

typedef NS_ENUM(NSInteger, PreviewBitRate)
{
    PreviewBitRate_Lowest  = 1 ,
    PreviewBitRate_Low     = 2 ,
    PreviewBitRate_Medium  = 4 ,
    PreviewBitRate_High    = 8 ,
    PreviewBitRate_Highest = 16
};

typedef NS_ENUM(NSInteger, PreviewFPS)
{
    PreviewFPS_Lowest  = 10 ,
    PreviewFPS_Low     = 15 ,
    PreviewFPS_Medium  = 20 ,
    PreviewFPS_High    = 25 ,
    PreviewFPS_Highest = 30
};

typedef NS_ENUM(NSInteger, VideoScale)
{
    VideoScale_ResizeAspect = 0,
    VideoScale_ResizeAspectFill
};


@protocol TK_media_Delegate<NSObject>

@optional

/**
 输出编码后的视频数据
 
  @param data 编码后的视频数据
  @param isKeyFrame 是否是关键帧
  @param timeStamp 视频时间戳
 */

- (void)TK_encode_outputVideoData:(NSData *) data
                       isKeyFrame:(BOOL)isKeyFrame
                        timeStamp:(unsigned long long)timeStamp;

/**
 输出解码后的视频数据
 
 @param pixelBuffer 解码后的视频数据
 @param tag 视频解码器tag
 */
- (void)TK_decode_outputVideoPixelData:(CVPixelBufferRef)pixelBuffer
                                   tag:(NSString *)tag;

/**
 输出采集的pcm音频
 
  @param data pcm音频数据
  @param timeStamp 音频时间戳
 */
- (void)TK_audio_outputAudioData:(NSData *)data
                       timeStamp:(unsigned long long)timeStamp;

/**
 输出编码后的音频
 
  @param data 编码后的音频数据
  @param timeStamp 音频时间戳
 */
- (void)TK_encode_outputAudioData:(NSData *)data
                        timeStamp:(unsigned long long)timeStamp;

/**
 输出解码后的音频
 
 @param data pcm音频数据
 @param tag 音频解码器tag
 */
- (void)TK_decode_outputAudioData:(NSData *)data
                              tag:(NSString *)tag;

@end

@interface TKMedia : NSObject

/**
 获取音视频模块版本
 
 */
+ (NSString *)TK_getMediaVersion;

/**
 音视频模块单例
 
 */
+ (instancetype)shareTKMedia;

/**
 设置音视频模块的代理
 
 */
- (void)setMediaDelegate:(id<TK_media_Delegate>)mediaDelegate;

/**
 开始采集视频数据
 
 @param previewView 预览视图
 @param videoFormat 编码格式
 @param isSWEncode 是否是软编码
 */
- (void)TK_preview_startCapture:(UIView *)previewView
                    videoFormat:(VideoFormat)videoFormat
                     isSWEncode:(BOOL)isSWEncode;

/**
 停止采集视频数据
 
 */
- (void)TK_preview_stopCapture;

/**
 切换前后摄像头
 
 @param switchBlock 回调是否是后摄像头
 */
- (void)TK_preview_switchCamera:(SwitchCameraBlock)switchBlock;

/**
 切换前后摄像头
 
 @param videoScale 画面填充模式
 */
- (void)TK_preview_setSaleType:(VideoScale)videoScale;

/**
 设置采集的视频质量等级
 
 @param resolution 分辨率等级
 @param fps 刷新率等级
 @param bitrate 比特率等级
 @param presetBlock 回调是否支持
 */
- (void)TK_preview_changeResolution:(PreviewResolution)resolution
                                fps:(PreviewFPS)fps
                            bitrate:(PreviewBitRate)bitrate
                        presetBlock:(PresetBlock)presetBlock;

/**
 设置摄像头的采集方向
 
 @param videoOrientation 视频方向
 */
- (void)TK_preview_setOrientation:(AVCaptureVideoOrientation)videoOrientation;

/**
 设置视频解码器
 
 @param isHWDecode 是否硬解码 默认软解码
 @param videoFormat 视频格式 默认是VideoFormat_H264
 */
- (void)TK_decode_setVideoDecodeMode:(BOOL)isHWDecode
                         VideoFormat:(VideoFormat)videoFormat;

/**
 开始解码视频
 
 @param showView 播放视图
 @param tag 视频解码器tag
 */
- (void)TK_decode_startDecodeVideo:(UIView *)showView
                               tag:(NSString *)tag;

/**
 停止解码视频
 
 @param tag 视频解码器tag
 */
- (void)TK_decode_stopDecodeVideo:(NSString *)tag;

/**
 接收视频数据进行解码
 
 @param data 未解码的视频数据
 @param timeStamp 视频时间戳
 @param tag 视频解码器tag
 */
- (void)TK_deocde_onReceiveVideoData:(NSData *)data
                           timeStamp:(unsigned long long)timeStamp
                                 tag:(NSString *)tag;

/**
 开始采集或播放音频
 
 @param audioCategory AudioUnit的双工模式  默认全双工模式
 @param sampleRate 音频采样率  默认是8K
 */
- (void)TK_audio_startAudioUnit:(AudioCategory)audioCategory
                     sampleRate:(SampleRate)sampleRate;

/**
 停止采集或播放音频
 
 */
- (void)TK_audio_stopAudioUnit;

/**
 接收并播放音频
 
 @param data pcm音频数据
 */
- (void)TK_audio_onPlayAudioData:(NSData *)data
                             tag:(NSString *)tag;

/**
 初始化音频编码器
 
 @param codec_id 编码格式
 @param sampleRate 音频采样率
 @param bit_rate 音频位宽
 @param channels 音频声道
 */
- (void)TK_encode_initAudioCodeID:(AudioFormat)codec_id
                       sampleRate:(SampleRate)sampleRate
                          bitRate:(AudioDataBits)bit_rate
                         channels:(AudioChannels)channels;

/**
 销毁音频编码器
 
 */
- (void)TK_encode_deInitAudioEncode;

/**
 接收并编码音频
 
 @param data pcm音频数据
  @param timeStamp 音频时间戳
 */
- (void)TK_encode_onReceiveAudioData:(NSData *)data
                           timeStamp:(unsigned long long)timeStamp;

/**
 初始化音频解码器
 
 @param codec_id 解码格式
 @param sampleRate 音频采样率
 @param bit_rate 音频位宽
 @param channels 音频声道
 */
- (void)TK_decode_initAudioCodeID:(AudioFormat)codec_id
                       sampleRate:(SampleRate)sampleRate
                          bitRate:(AudioDataBits)bit_rate
                         channels:(AudioChannels)channels
                              tag:(NSString *)tag;

/**
 销毁音频解码器
 
 @param tag 音频解码器tag
 */
- (void)TK_decode_deInitAudioDecode:(NSString *)tag;

/**
 接收并解码音频
 
 @param data 未解码的音频数据
 @param timeStamp 音频时间戳
 @param tag 音频解码器tag
 */
- (void)TK_deocde_onReceiveAudioData:(NSData *)data
                           timeStamp:(unsigned long long)timeStamp
                                 tag:(NSString *)tag;


/**
 切换前后摄像头
 
 @param videoScale 画面填充模式
 */
- (void)TK_video_setScaleType:(VideoScale)videoScale tag:(NSString *)tag;

/**
 截图
 
 @param path 存储截图的沙盒路劲
 @param tag 视频解码器tag
 */
- (void)TK_video_snapShotWithPath:(NSString *)path
                              tag:(NSString *)tag;

/**
 录像
 
 @param startRecording 是否开始录像
 @param videoFormat 视频格式
 @param sampleRate 音频采样率
 @param dataBit 音频位宽
 @param channels 音频声道
 @param path 存储截图的沙盒路劲
 @param tag 视频解码器tag
 @param completion 回调成功与失败信息
 */
- (void)TK_video_startRecording:(BOOL)startRecording
                    videoFormat:(VideoFormat)videoFormat
                     sampleRate:(SampleRate)sampleRate
                        dataBit:(AudioDataBits)dataBit
                       channels:(AudioChannels)channels
                       withPath:(NSString *)path
                            tag:(NSString *)tag
                     completion:(void (^)(NSError *error))completion;


/**
 获取编码模式
 
 @return YES 是硬编/ NO 是软编
 */
- (BOOL)TK_video_getEncodeMode;

/**
 获取解码模式
 
 @return YES 是硬解/ NO 是软解
 */
- (BOOL)TK_video_getDecodeMode;

@end

#ifndef _Log_h
#define _Log_h
#ifdef ANDROID
    #include <jni.h>
    #include <android/log.h>
#endif //ANDROID


#define TAG "MediaDecEnc"

#ifdef _LOGD_
    #ifdef ANDROID
        #define LOGD(...) __android_log_print (ANDROID_LOG_DEBUG, TAG,__VA_ARGS__);
    #else
        #define LOGD(...) (printf(__VA_ARGS__) , printf("\r\n"))
    #endif //ANDROID
#else
    #define LOGD(...)
#endif /* _LOGD_ */

#ifdef _LOGE_
    #ifdef ANDROID
        #define LOGE(...) __android_log_print (ANDROID_LOG_ERROR, TAG,__VA_ARGS__);
    #else
        #define LOGE(...) (printf(__VA_ARGS__) , printf("\r\n"))
    #endif //ANDROID
#else
    #define LOGE(...)
#endif /* _LOGE_ */


#ifdef _LOGI_
    #ifdef ANDROID
        #define LOGI(...) __android_log_print (ANDROID_LOG_INFO, TAG,__VA_ARGS__);
    #else
        #define LOGI(...) (printf(__VA_ARGS__) , printf("\r\n"))
    #endif //ANDROID
#else
    #define LOGI(...)
#endif /* _LOGI_ */

#ifdef _LOGW_
    #ifdef ANDROID
        #define LOGW(...) __android_log_print (ANDROID_LOG_WARN, TAG,__VA_ARGS__);
    #else
        #define LOGW(...) (printf(__VA_ARGS__) , printf("\r\n"))
    #endif //ANDROID
#else
    #define LOGW(...)
#endif /* _LOGW_ */
#endif /* _Log_h */
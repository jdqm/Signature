//
// Created by Jdqm on 2023/5/4.
//

#include <android/log.h>

#ifndef SIGNATURE_LOGGER_H
#define SIGNATURE_LOGGER_H

#define LOG_I(TAG, ...)  __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOG_D(TAG, ...)  __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOG_W(TAG, ...)	__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define	YX_LOG_E(TAG, ...)	__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

#endif //SIGNATURE_LOGGER_H

#include <jni.h>
#include <string>
#include "log/logger.h"


const char HEX_CODE[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E',
                         'F'};

const char *SIGNATURE_SHA1 = "C57D8E89FACFF0EABE63FAFC7605EF0CE1105148";

void byteToHexStr(const char *source, char *result, jlong len) {
    for (int i = 0; i < len; ++i) {
        *(result + (2*i)) = HEX_CODE[((unsigned char) (*(source + i))) / 16];
        *(result + (2*i) + 1) = HEX_CODE[((unsigned char) (*(source + i))) % 16];
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_jdqm_securelib_SecureTool_checkSignature(JNIEnv *env, jobject thiz, jstring packageName, jobject context) {
    //context.getPackageManager()
    jclass contextClass = env->GetObjectClass(context);
    jmethodID methodPackageManager = env->GetMethodID(contextClass, "getPackageManager", "()Landroid/content/pm/PackageManager;");
    jobject packageManager = env->CallObjectMethod(context, methodPackageManager);
    env->DeleteLocalRef(contextClass);

    //PackageInfo info = packageManager.getPackageInfo(context.getPackageName(), GET_SIGNATURES);
    jclass packageManagerClass = env->GetObjectClass(packageManager);
    jmethodID methodGetPackageInfo = env->GetMethodID(packageManagerClass, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");

    // final int GET_SIGNATURES = 0x00000040;
    jobject packageInfo = env->CallObjectMethod(packageManager, methodGetPackageInfo, packageName, 0x40);
    env->DeleteLocalRef(packageManagerClass);

    //Signature[] signs = info.signatures;
    jclass packageInfoClass = env->GetObjectClass(packageInfo);
    jfieldID fieldSignatures = env->GetFieldID(packageInfoClass, "signatures", "[Landroid/content/pm/Signature;");
    jobjectArray signatures = static_cast<jobjectArray>(env->GetObjectField(packageInfo,fieldSignatures));
    jobject signature_object = env->GetObjectArrayElement(signatures, 0);
    env->DeleteLocalRef(packageInfoClass);

    //MessageDigest localMessageDigest = MessageDigest.getInstance("SHA1");
    //localMessageDigest.update(signs[0].toByteArray());
    jclass messageDigestClass = env->FindClass("java/security/MessageDigest");
    jmethodID jmethodGetInstance = env->GetStaticMethodID(messageDigestClass, "getInstance", "(Ljava/lang/String;)Ljava/security/MessageDigest;");
    jobject localMessageDigest = env->CallStaticObjectMethod(messageDigestClass, jmethodGetInstance, env->NewStringUTF("SHA1"));
    jmethodID methodUpdate = env->GetMethodID(messageDigestClass, "update", "([B)V");
    jclass signatureClass = env->FindClass("android/content/pm/Signature");
    jmethodID methodToByteArray = env->GetMethodID(signatureClass, "toByteArray", "()[B");
    jbyteArray signature = static_cast<jbyteArray>(env->CallObjectMethod(signature_object,methodToByteArray));
    env->CallVoidMethod(localMessageDigest, methodUpdate, signature);
    jmethodID methodDigest = env->GetMethodID(messageDigestClass, "digest", "()[B");
    jbyteArray sha1ByteArray = static_cast<jbyteArray>(env->CallObjectMethod(localMessageDigest,methodDigest));
    env->DeleteLocalRef(messageDigestClass);

    jlong len = env->GetArrayLength(sha1ByteArray);
    jbyte *byteArrayElements = env->GetByteArrayElements(sha1ByteArray, nullptr);
    size_t length = len * 2 + 1;
    char *charResult = (char*) malloc(length);
    memset(charResult, 0, length);
    byteToHexStr(reinterpret_cast<char *>(byteArrayElements), charResult, len);
    // 在末尾补\0
    *(charResult + length * 2) = '\0';
    if (strcmp(SIGNATURE_SHA1, charResult) == 0) {
        LOG_D("签名校验通过: ", "%s", charResult);
        return true;
    }
    LOG_D("签名校验失败: ", "%s", charResult);
    env->ReleaseByteArrayElements(sha1ByteArray, byteArrayElements, JNI_ABORT);
    free(charResult);
    return false;
}



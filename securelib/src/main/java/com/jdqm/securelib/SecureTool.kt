package com.jdqm.securelib

import android.content.Context

object SecureTool {
    init {
        System.loadLibrary("securelib")
    }

    /**
     * 检测aap签名是否合法
     */
    external fun checkSignature(packageName: String, context: Context): Boolean
}
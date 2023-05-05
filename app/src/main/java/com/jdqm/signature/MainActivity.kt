package com.jdqm.signature

import android.content.Context
import android.content.pm.PackageManager
import android.os.Bundle
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import com.jdqm.securelib.SecureTool
import java.security.MessageDigest

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        findViewById<TextView>(R.id.tv_text).text = getSignatureStr(this)

        if (!SecureTool.checkSignature(packageName, this)) {
            MaterialAlertDialogBuilder(this)
                .setTitle("警告")
                .setMessage("此App非官方签名，为了您的信息安全，建议卸载后到官方渠道重新下载安全！")
                .setPositiveButton("去逛网") { dialog, which ->
                    dialog.dismiss()
                }
                .setNegativeButton("我知道了") { dialog, which ->
                    dialog.dismiss()
                }
                .create()
                .show()
        }

    }

    private fun getSignatureStr(context: Context): String? {
        val packageManager = context.packageManager ?: return null
        try {
            val info = packageManager.getPackageInfo(context.packageName, PackageManager.GET_SIGNATURES)
            val signs = info.signatures
            // MessageDigest localMessageDigest = MessageDigest.getInstance("MD5");
            val localMessageDigest = MessageDigest.getInstance("SHA1")
            //  MessageDigest localMessageDigest = MessageDigest.getInstance("SHA-256");
            localMessageDigest.update(signs[0].toByteArray())
            return toHexString(localMessageDigest.digest())
        } catch (e: Exception) {
            e.printStackTrace()
        }
        return null
    }

    private fun toHexString(paramArrayOfByte: ByteArray): String {
        val localStringBuilder = StringBuilder(2 * paramArrayOfByte.size)
        paramArrayOfByte.forEach {
            var str = Integer.toString(0xFF and it.toInt(), 16)
            if (str.length == 1) {
                str = "0$str"
            }
            localStringBuilder.append(str)
        }
       return localStringBuilder.toString().uppercase()
    }
}
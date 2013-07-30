#include "JNIKelper.h"


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
void JNIKelper::setJNI(string _className)
{		
	className = _className;
	if(JniHelper::getStaticMethodInfo(minfo, (className + ".KSAWrap").c_str(), "getActivity", "()Ljava/lang/Object;"))
	{
		jobj =  minfo.env->NewGlobalRef(minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID));
		minfo.env->DeleteLocalRef(minfo.classID);
	}
}
void JNIKelper::releaseJNI()
{
	//CCLog("nonono");
	minfo.env->DeleteGlobalRef(jobj);
}


string JNIKelper::callJava_getUDID()
{
	JniMethodInfo __minfo;
	__minfo.classID = 0;
	__minfo.env = 0;
	__minfo.methodID = 0;
	string p;
	if(JniHelper::getMethodInfo(__minfo, (className + ".KSActivityBase").c_str(), "getUDID", "(Ljava/lang/String;)Ljava/lang/String;"))
	{
		jstring jstrTitle = (jstring)__minfo.env->CallObjectMethod(jobj, __minfo.methodID, __minfo.env->NewStringUTF("pier1/pier2/pier3/pier4/pier5/pier6"));
		//__minfo.env->CallO
		//__minfo.env->CallVoidMethod(jobj, __minfo.methodID);
		if(jstrTitle)
		{
			
			char* pszTitle = (char*)__minfo.env->GetStringUTFChars(jstrTitle, JNI_FALSE);
			
			p = pszTitle;
			
			__minfo.env->ReleaseStringUTFChars(jstrTitle, pszTitle);
			__minfo.env->DeleteLocalRef(jstrTitle);
			
		}
		
		__minfo.env->DeleteLocalRef(__minfo.classID);
	}
	if(p == "")
		return "0";
	else
		return p;
}

string JNIKelper::callJava_getDeviceInfo()
{
	JniMethodInfo __minfo;
	__minfo.classID = 0;
	__minfo.env = 0;
	__minfo.methodID = 0;
	string p;
	if(JniHelper::getMethodInfo(__minfo, (className + ".KSActivityBase").c_str(), "getUDID", "(Ljava/lang/String;)Ljava/lang/String;"))
	{
		jstring jstrTitle = (jstring)__minfo.env->CallObjectMethod(jobj, __minfo.methodID, __minfo.env->NewStringUTF("pier1/pier2/pier3/pier4/pier5/pier6"));
		//__minfo.env->CallO
		//__minfo.env->CallVoidMethod(jobj, __minfo.methodID);
		if(jstrTitle)
		{
			
			char* pszTitle = (char*)__minfo.env->GetStringUTFChars(jstrTitle, JNI_FALSE);
			
			p = pszTitle;
			
			__minfo.env->ReleaseStringUTFChars(jstrTitle, pszTitle);
			__minfo.env->DeleteLocalRef(jstrTitle);
			
		}
		
		__minfo.env->DeleteLocalRef(__minfo.classID);
	}
	if(p == "")
		return "0";
	else
		return p;
}




void JNIKelper::callJava_otherApp(string packName)
{
	JniMethodInfo __minfo;
	__minfo.classID = 0;
	__minfo.env = 0;
	__minfo.methodID = 0;
	string p;
	if(JniHelper::getMethodInfo(__minfo, (className + ".KSActivityBase").c_str(), "openApp", "(Ljava/lang/String;)V"))
	{
		__minfo.env->CallVoidMethod(jobj, __minfo.methodID, __minfo.env->NewStringUTF(packName.c_str()));
		__minfo.env->DeleteLocalRef(__minfo.classID);
	}
}
void JNIKelper::callJava_moreApp(string company)
{
	JniMethodInfo __minfo;
	__minfo.classID = 0;
	__minfo.env = 0;
	__minfo.methodID = 0;
	string p;
	// ()Ljava/lang/String;
	// (Ljava/lang/String;)V
//	if(JniHelper::getMethodInfo(__minfo, (className + ".KSActivityBase").c_str(), "showMoreApps", "()V"))
//	{
//		__minfo.env->CallVoidMethod(jobj, __minfo.methodID);
////		__minfo.env->CallVoidMethod(jobj, __minfo.methodID, __minfo.env->NewStringUTF(company.c_str())); 회사를 파라메터로 받음.
//		__minfo.env->DeleteLocalRef(__minfo.classID);
//	}
	if(JniHelper::getMethodInfo(__minfo, (className + ".KSActivityBase").c_str(), "showMoreApps", "(Ljava/lang/String;)V"))
	{
//		__minfo.env->CallVoidMethod(jobj, __minfo.methodID);
		__minfo.env->CallVoidMethod(jobj, __minfo.methodID, __minfo.env->NewStringUTF(company.c_str()));
		__minfo.env->DeleteLocalRef(__minfo.classID);
	}
}

void JNIKelper::callJava_simple(string functionName)
{
	JniMethodInfo __minfo;
	__minfo.classID = 0;
	__minfo.env = 0;
	__minfo.methodID = 0;
	if(JniHelper::getMethodInfo(__minfo, (className + ".KSActivityBase").c_str(), functionName.c_str(), "()V"))
	{
		__minfo.env->CallVoidMethod(jobj, __minfo.methodID);
		__minfo.env->DeleteLocalRef(__minfo.classID);
	}
}
string JNIKelper::callJava_getUTF8String(string _strkey)
{
	JniMethodInfo __minfo;
	
	if(JniHelper::getMethodInfo(__minfo, (className + ".KSActivityBase").c_str(), "getUTF8String", "(Ljava/lang/String;)Ljava/lang/String;"))
	{
		jstring returnedStr = (jstring)__minfo.env->CallObjectMethod(jobj, __minfo.methodID, __minfo.env->NewStringUTF(_strkey.c_str()));
		
		const char* ret = __minfo.env->GetStringUTFChars(returnedStr, JNI_FALSE);
		string realRet = ret;
		__minfo.env->ReleaseStringUTFChars(returnedStr, ret);
		__minfo.env->DeleteLocalRef(__minfo.classID);
		return realRet;
	}
	return _strkey;
	
	
}

#endif
#include "InAppHelper.h"


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
void InAppHelper::setJNI(string _className)
{		
	className = _className;
	CCLog("cls %s", className.c_str());
	if(JniHelper::getStaticMethodInfo(minfo, (className + "").c_str(), "getActivity", "()Ljava/lang/Object;"))
	{
		CCLog("cls %d", __LINE__);
		jobj =  minfo.env->NewGlobalRef(minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID));
		minfo.env->DeleteLocalRef(minfo.classID);
	}
	CCLog("cls %d", __LINE__);
}
void InAppHelper::releaseJNI()
{
	//CCLog("nonono");
	minfo.env->DeleteGlobalRef(jobj);
}

void InAppHelper::callJava_buyItem(string itemID, CCObject* target, RecvInAppSelType selector)
{
	JniMethodInfo __minfo;
	__minfo.classID = 0;
	__minfo.env = 0;
	__minfo.methodID = 0;
	if(JniHelper::getMethodInfo(__minfo, (className + "").c_str(), "buyItem", "(Ljava/lang/String;I)V")) // itemID, delekey
	{
		int getKey = InAppDelegator::getInstance()->addDeleSel(target, selector);
		__minfo.env->CallVoidMethod(jobj, __minfo.methodID, __minfo.env->NewStringUTF(itemID.c_str()), getKey);
		__minfo.env->DeleteLocalRef(__minfo.classID);
	}
}



string InAppHelper::callJava_getPrice(string itemID)
{
	JniMethodInfo __minfo;
	__minfo.classID = 0;
	__minfo.env = 0;
	__minfo.methodID = 0;
	string p;
	if(JniHelper::getMethodInfo(__minfo, (className + "").c_str(), "getPrice", "(Ljava/lang/String;)Ljava/lang/String;"))
	{
		jstring jstrTitle = (jstring)__minfo.env->CallObjectMethod(jobj, __minfo.methodID, __minfo.env->NewStringUTF(itemID.c_str()));
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



#endif
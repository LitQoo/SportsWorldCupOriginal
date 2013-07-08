//
//  InAppDelegator.cpp
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 12. 11..
//
//

#include "InAppDelegator.h"
#include "InAppHelper.h"

#ifdef __cplusplus
extern "C" {
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	void Java_com_litqoo_SportsWorldCup_SportsWorldCup_sendInAppData(JNIEnv *env, jobject thiz, jstring datas, int resultcode, int delekey) // ¿⁄πŸø°º≠ µ•¿Ã≈Õ πﬁ¿Ω!!
	{
		jboolean isCopy = JNI_FALSE;
		const char* revStr = env->GetStringUTFChars(datas, &isCopy); // UTF-8 πÆ¿⁄∏¶ æÚæÓø».
		string throwData = revStr;
		InAppDelegator::DeleSel delesel = InAppDelegator::getInstance()->getDeleSel(delekey);
		InAppDelegator::getInstance()->removeDeleSel(delekey);

		if(delesel.target != 0 && delesel.selector != 0)
		{
			((delesel.target)->*(delesel.selector))(throwData, resultcode);
		}
		
		env->ReleaseStringUTFChars(datas, revStr);
	}
#endif
#ifdef __cplusplus
}
#endif
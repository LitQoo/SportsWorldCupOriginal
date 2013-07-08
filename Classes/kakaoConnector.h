#ifndef __kakaoConnector_H__
#define __kakaoConnector_H__

#include "cocos2d.h"
#include "kakaoConnectorDelegator.h"


using namespace cocos2d;
enum KCERROR {
    SUCCESS = 0,
    NETWORKERROR = 6,
    NOT_SUPPORTED_IN_GUEST_MODE=8,
    SUCCESS_NOT_VERIFIED=10,
    UNDER_MAINTENANCE=-9798,
    KAServerErrorNotAuthorized=-1000,
    ERROR=-500,
    LOWER_AGE_LIMIT=-451,
    KAServerErrorInvaildGrant=-400,
    INVALID_PUSH_TOKEN=-200,
    INSUFFICIENT_PERMISSION=-100,
    MESSAGE_OVER_QUOTA=-32,
    EXCEED_INVITE_CHAT_LIMIT=-31,
    EXCEED_CHAT_LIMIT=-30,
    CURRENT_HEART_MISMATCH=-18,
    INVITE_MESSAGE_BLOCKED=-17,
    MESSAGE_BLOCK_USER=-16,
    CHAT_NOT_FOUND=-15,
    UNSUPPORTED_DEVICE=-14,
    UNREGISTERD_USER=-13,
    INVALID_REQUEST=-12,
    DEACTIVATED_USER=-11,
    NOT_FOUND=-10
    };

class kakaoConnector
{
public:
    
    static kakaoConnector* get()
	{
		static kakaoConnector* _ins = 0;
		if(_ins == 0)
			_ins = new kakaoConnector();
		return _ins;
	}
    kakaoConnector(){
        
    }
    bool isLogin();
    void clearRegistration();
    void login(CCObject *target,kakaoSelType selector);
    void ageVerification(JsonBox::Object param,CCObject *target,kakaoSelType);
    void loadLocalUser(CCObject *target,kakaoSelType selector);
    void loadFriends(CCObject *target,kakaoSelType selector);
    void sendMessage(JsonBox::Object param,CCObject *target,kakaoSelType selector);
    void logout(CCObject *target,kakaoSelType selector);
    void unregister(CCObject *target,kakaoSelType selector);
    void showStoryView(JsonBox::Object param,CCObject *target,kakaoSelType selector);
    void sendPaymentInfo(JsonBox::Object param,CCObject *target,kakaoSelType selector);
    void loadGameInfo(CCObject *target,kakaoSelType selector);
    void loadGameMe(CCObject *target,kakaoSelType selector);
    void loadGameFriends(CCObject *target,kakaoSelType selector);
    void loadLeaderBoard(JsonBox::Object param,CCObject *target,kakaoSelType selector);
    void updateResult(JsonBox::Object param,CCObject *target,kakaoSelType selector);
    void updateMe(JsonBox::Object param,CCObject *target,kakaoSelType selector);
    void sendGameMessage(JsonBox::Object param,CCObject *target,kakaoSelType selector);
    void loadGameMessages(CCObject *target,kakaoSelType selector);
    void acceptMessage(JsonBox::Object param,CCObject *target,kakaoSelType selector);
    void acceptAllMessages(CCObject *target,kakaoSelType selector);
    void useHeart(JsonBox::Object param,CCObject *target,kakaoSelType selector);
    void messageBlock(JsonBox::Object param,CCObject *target,kakaoSelType selector);
    void sendInviteMessage(JsonBox::Object param,CCObject *target,kakaoSelType selector);
    void deleteMe(CCObject *target,kakaoSelType selector);

};


#endif 

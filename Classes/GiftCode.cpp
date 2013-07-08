//
//  GiftCode.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 22..
//
//

#include "GiftCode.h"
#include "Bridge.h"
#include "KS_Util.h"
#include "GraphDog.h"
#include <sstream>
#include "DisableTouch.h"
#include "KSModalAlert.h"
#include "NSDefault.h"
#include "KS_Util.h"
#include "KSoundEngine.h"
#include "GDTranslate.h"
using namespace GDTranslate;
bool GiftCode::init()
{
	CCLayerColor::initWithColor(ccc4(0,0, 0, 0));
	this->setTouchEnabled(true);
	
	thiz = CCSprite::create("giftcode_back.png");
	addChild(thiz);
	
	CCSize FieldSize = CCSize( 200, 0 );
	giftCodeTTF = CCTextFieldTTF::textFieldWithPlaceHolder( "<input here>", FieldSize, kCCTextAlignmentLeft, defaultFont, 18 );
	giftCodeTTF->setAnchorPoint(ccp(0, 0.5));
	giftCodeTTF->setDelegate(this);
	thiz->addChild( giftCodeTTF, 999 );
	giftCodeTTF->setPosition( ccp(33, 231) );
	giftCodeTTF->setColor(ccc3(255, 255, 255));
	giftCodeTTF->setString("");
	
	CCSize descSize = CCSize(200, 0);

	CCLabelTTF* desc = CCLabelTTF::create(gt("publishcode").c_str(), defaultFont, 14.f,
								descSize, kCCTextAlignmentCenter );
	desc->setColor(ccc3(0, 0, 0));
	desc->setPosition(ccp(132, 93));
	thiz->addChild(desc, 2);
	CCMenu* _menu = CCMenu::create(0);
	_menu->setPosition(CCPointZero);
	thiz->addChild(_menu);
	
	CCMenuItem* cancel = CCMenuItemImage::create("giftcode_cancel.png", "giftcode_cancel.png",
															  this, menu_selector(ThisClassType::CANCEL));
	cancel->setPosition(ccp(76, 188));
	_menu->addChild(cancel, 2);
	
	CCMenuItem* ok = CCMenuItemImage::create("giftcode_ok.png", "giftcode_ok.png",
															  this, menu_selector(ThisClassType::OK));
	ok->setPosition(ccp(188, 188));
	_menu->addChild(ok, 2);
	return true;
}
void GiftCode::CANCEL(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	giftCodeTTF->detachWithIME();
	mediator->showRankWithMenu();
}
void GiftCode::OK(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	giftCodeTTF->detachWithIME();
	string code = giftCodeTTF->getString();
	JsonBox::Object param;
	DisableTouch* cl = DisableTouch::create();
	addChild(cl);
	
	ostringstream oss;
	oss << (int)cl;
	param["callback"] = oss.str();
	
	param["giftcode"] = code;
	graphdog->command("usegiftcode", &param, this, gd_selector(ThisClassType::CONFIRM_CODE));
}

void GiftCode::CONFIRM_CODE(JsonBox::Object js)
{
	JsonBox::Object obj = js["param"].getObject();
	string _cl = obj["callback"].getString();
	
	CCNode* cl = (CCNode*)atoi(_cl.c_str());
	cl->removeFromParentAndCleanup(true);
	
//	0 : complete
//	870 : a gift code a hour
//	880 : invalid code
//	890 : used code

	KS::KSLog("%", js["state"]);
	if(js["state"].getString() == "ok")
	{
		int returnCode = js["resultcode"].getInt();
		string category = js["category"].getString();
		int value = js["value"].getInt();
		switch(returnCode)
		{
			case 100:
				if(category == "gold")
				{
					NSDefault::setGold(NSDefault::getGold() + value);
					CCNotificationCenter::sharedNotificationCenter()->postNotification("refreshGold");
					giftCodeTTF->setString("");
				}
				break;
			case 870:
				KSModalAlert("", gt("giftperhour").c_str(), 1,
							 "OK", NOPARAM);
				giftCodeTTF->setString("");
				break;
			case 880:
				KSModalAlert("", gt("invalidgiftcode").c_str(), 1,
							 "OK", NOPARAM);
				break;
			case 890:
				KSModalAlert("", gt("alreadyusegift").c_str(), 1,
							 "OK", NOPARAM);
				giftCodeTTF->setString("");
				break;
		}
	}
	else
	{
		//network error
		KSModalAlert("", "Network error, try again", 1,
					 "OK", NOPARAM);
	}
	
	
}
void GiftCode::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}
bool GiftCode::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = pTouch;
	
	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	location = thiz->convertToNodeSpace(location);
	giftCodeTTF->detachWithIME();
	if(giftCodeTTF->boundingBox().containsPoint( location))
	{
		//CCLog("attach IME");
		giftCodeTTF->attachWithIME();
	}

	return true;
}
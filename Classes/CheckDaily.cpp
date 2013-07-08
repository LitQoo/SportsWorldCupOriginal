//
//  CheckDaily.cpp
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 6. 26..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "CheckDaily.h"
#include "NSDefault.h"
#include "KSBlock.h"

CheckDaily::CheckDaily()
{
	touchEnable = false;
}
CheckDaily::~CheckDaily()
{
//	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}
void CheckDaily::registerWithTouchDispatcher()
{
	cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate
	(this, INT_MIN, true);
}
bool CheckDaily::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = pTouch;

	CCPoint location(ccp(0, 0));
	
	if(touchEnable)
	{
		int reward[] = {0, 0, 500, 500, 2000, 500, 500, 4000};
		int seq = NSDefault::getAttendanceSeq();
		if(seq >= 2 && seq <= 7)
		{
			NSDefault::setGold(NSDefault::getGold() + reward[seq]);
			CCNotificationCenter::sharedNotificationCenter()->postNotification("refreshGold");
		}
//		seq = 2;
		
		removeFromParentAndCleanup(true);
	}
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
	
	return true;
}
bool CheckDaily::init()
{
	CCLayerColor::initWithColor(ccc4(0, 0, 0, 0));
	setTouchEnabled(true);
	CCSprite* b = CCSprite::create();
	b->setTextureRect(CCRectMake(0, 0, 600, 320));
	b->setColor(ccc3(0, 0, 0));
	b->setOpacity(0);
	
	b->runAction(CCFadeTo::create(0.5f, 180));
//	b->setOpacity(150);
	//	b->setContentSize(CCSizeMake(600, 320));
	b->setPosition(ccp(240, 160));
	b->setAnchorPoint(ccp(0.5f, 0.5f));
	addChild(b,1);

	
	CCSprite* r = CCSprite::create("attendance.png");
	
	r->setPosition(ccp(240, 160));
	addChild(r, 3);
	
	CCPoint pos[] = {{0,0}, {0,0}, {183, 203}, {258, 203}, {335, 203}, {310, 147}, {236, 147}, {158, 145}};
	
	int seq = NSDefault::getAttendanceSeq();
//	seq = 2; // 2일차로 강제로 세팅 .
	CCPoint curPos = pos[seq];
	
	CCSprite* itemCheck = CCSprite::create("item_check.png");
	
	itemCheck->setPosition(curPos);
	
	itemCheck->runAction(CCBlink::create(2 * 1000, 5 * 1000));
	addChild(itemCheck, 4);
	
	runAction(CCSequence::create(CCDelayTime::create(1.5f),
								 KSFunc(
										{
											touchEnable = true;
										}
								 ), 0));
	
//	this->setIsTouchEnabled(true);
	
	return true;
}
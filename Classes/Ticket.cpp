//
//  SendTicket.cpp
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 6. 27..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "Ticket.h"


#include "KS_Util.h"
#include "FBConnector.h"
#include "GDTranslate.h"
#include "KSModalAlert.h"
#include "Bridge.h"
#include "cocos-ext.h"
#include "NSDefault.h"
using namespace GDTranslate;
Ticket::Ticket()
{
	ticketDatas = 0;
}
Ticket::~Ticket()
{
	if(ticketDatas != 0)
		ticketDatas->release();
}
void Ticket::registerWithTouchDispatcher()
{
	cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate
	(this, INT_MIN + 1, true);
//	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
}
bool Ticket::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = pTouch;

	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
	
	return true;
}
bool Ticket::init()
{
	CCLayer::init();
	setTouchEnabled(true);
	//	this->setIsTouchEnabled(true);
	
	
	
	return true;
}

void Ticket::createRecvTicket(CCArray* _tds)
{
	_tds->retain();
	ticketDatas = _tds;
	
	string sendList;
	for(int i=0; i<ticketDatas->count(); i++)
	{
		TicketData* td = (TicketData*)ticketDatas->objectAtIndex(i);
		
		sendList += td->from.fbname + ",";
	}
	
	sendList.assign(sendList.begin(), sendList.end() - 1);
	
	CCSprite* b = CCSprite::create();
	b->setTextureRect(CCRectMake(0, 0, 600, 320));
	b->setColor(ccc3(0, 0, 0));
	b->setOpacity(0);
	
	b->runAction(CCFadeTo::create(0.5f, 180));
	b->setPosition(ccp(240, 160));
	b->setAnchorPoint(ccp(0.5f, 0.5f));
	addChild(b);
	CCSprite* back = CCSprite::create("unlock_back2.png");
	back->setPosition(ccp(240, 160));
	addChild(back);
	
	CCLabelTTF* desc = CCLabelTTF::create(sendList.c_str(), defaultFont, 14.f, CCSizeMake(247, 37), kCCTextAlignmentCenter);
	
	desc->setPosition(ccp(244, 203));
	
	addChild(desc);
	
	CCSprite* sprite = CCSprite::create("unlock_ticketon.png");
	sprite->setPosition(ccp(240, 145));
	addChild(sprite,2);
	CCMenu* _menu = CCMenu::create();
	
	_menu->setPosition(ccp(0, 0));
	
	addChild(_menu, 3);
	_menu->setTouchPriority(INT_MIN);
	CCMenuItem* ok = CCMenuItemImage::create("unlock_ok.png", "unlock_ok.png", this, menu_selector(Ticket::T_OK));
	_menu->addChild(ok);
	ok->setPosition(ccp(240, 74));
}
void Ticket::createRecvTicketReq(CCArray* _tds)
{
	_tds->retain();
	ticketDatas = _tds;
	
	
	string sendList;
	for(int i=0; i<ticketDatas->count(); i++)
	{
		TicketData* td = (TicketData*)ticketDatas->objectAtIndex(i);
		sendList += td->from.fbname + ",";
	}
	
	sendList.assign(sendList.begin(), sendList.end() - 1);

	CCSprite* b = CCSprite::create();
	b->setTextureRect(CCRectMake(0, 0, 600, 320));
	b->setColor(ccc3(0, 0, 0));
	b->setOpacity(0);
	
	b->runAction(CCSequence::create(CCFadeTo::create(0.5f, 180), CCDelayTime::create(1.5f),0));
	b->setPosition(ccp(240, 160));
	b->setAnchorPoint(ccp(0.5f, 0.5f));
	addChild(b);
	CCSprite* back = CCSprite::create("unlock_back.png");
	back->setPosition(ccp(240, 160));
	addChild(back);
	
	CCLabelTTF* desc = CCLabelTTF::create(sendList.c_str(), defaultFont, 14.f, CCSizeMake(247, 37), kCCTextAlignmentCenter);
	
	desc->setPosition(ccp(244, 203));
	
	addChild(desc);
	
	CCSprite* sprite = CCSprite::create("unlock_ticketon.png");
	sprite->setPosition(ccp(240, 145));
	addChild(sprite,2);
	CCMenu* _menu = CCMenu::create();
	
	_menu->setPosition(ccp(0, 0));
	
	addChild(_menu, 3);
	_menu->setTouchPriority(INT_MIN);
	CCMenuItem* exit = CCMenuItemImage::create("unlock_no.png", "unlock_no.png", this, menu_selector(Ticket::T_NO));
	_menu->addChild(exit);
	exit->setPosition(ccp(165, 74));
	CCMenuItem* ask = CCMenuItemImage::create("unlock_yes.png", "unlock_yes.png", this, menu_selector(Ticket::T_YES));
	ask->setPosition(ccp(314, 74));
	_menu->addChild(ask);

}

void Ticket::T_YES(CCObject*)
{
	std::string sendList;
	for(int i=0; i<ticketDatas->count(); i++)
	{
		TicketData* td = (TicketData*)ticketDatas->objectAtIndex(i);
		sendList += td->from.fbid + ",";
	}	
	sendList.assign(sendList.begin(), sendList.end() - 1);
	FBConnector::get()->sendRequest(sendList, "msg", "gameticketrecv", this, fb_selector(ThisClassType::T_YES_OK));
}

void Ticket::T_YES_OK(JsonBox::Value v2)
{
	KS::KSLog("%", v2);
	
	if(v2["error"].getInt() == 0)
	{
		CCLog("T_YES_OK suc");
		for(int i=0; i<ticketDatas->count(); i++)
		{
			TicketData* td = (TicketData*)ticketDatas->objectAtIndex(i);
			FBConnector::get()->removeRequests(td->ticketid, 0, 0);
//			sendList += td->from.fbname + ",";
		}
	}
	else // 실패
	{
		CCLog("T_YES_OK fail");
		KSModalAlert("", gt("sendfail").c_str(), 1,
					 "OK", NOPARAM);
	}
	removeFromParentAndCleanup(true);
}

void Ticket::T_OK(CCObject*)
{
	// 티켓이 두개라는 가정하에 특수화된 처리임. 세명이상 늘릴 경우 다른식으로 프로그램 하여야 할 것임.
	bool isGameOpen = false;
	for(int i=0; i<ticketDatas->count(); i++)
	{
		TicketData* td = (TicketData*)ticketDatas->objectAtIndex(i);
		if(NSDefault::getArrivedTicketId() != "" && NSDefault::getArrivedTicketId() != td->from.fbid) // 이미 받은게 있고 이전 사람과 다른 티켓이라면
		{
			isGameOpen = true;
		}
		
		FBConnector::get()->removeRequests(td->ticketid, 0, 0);
		NSDefault::setArrivedTicketId(td->from.fbid);
	}
	

	if(isGameOpen)
	{
		NSDefault::setArrivedTicketId("");
		NSDefault::setSoonOpenGame("");
		NSDefault::setTicketIsStarted(0);
		
		CCNotificationCenter::sharedNotificationCenter()->postNotification("onGameOpenWithFriends", 0);
	}
	removeFromParentAndCleanup(true);
}
void Ticket::T_NO(CCObject*)
{
	for(int i=0; i<ticketDatas->count(); i++)
	{
		TicketData* td = (TicketData*)ticketDatas->objectAtIndex(i);
		FBConnector::get()->removeRequests(td->ticketid, 0, 0);
	}
	removeFromParentAndCleanup(true);
}
void Ticket::createTicketSend()
{
	CCSprite* b = CCSprite::create();
	b->setTextureRect(CCRectMake(0, 0, 600, 320));
	b->setColor(ccc3(0, 0, 0));
	b->setOpacity(0);
	
	b->runAction(CCSequence::create(CCFadeTo::create(0.5f, 180), CCDelayTime::create(1.5f),0));
	b->setPosition(ccp(240, 160));
	b->setAnchorPoint(ccp(0.5f, 0.5f));
	addChild(b);
	CCSprite* back = CCSprite::create("unlock_back.png");
	back->setPosition(ccp(240, 160));
	addChild(back);
	
	// 하나가 와 있는 경우
	if(NSDefault::getArrivedTicketId() != "")
	{
		CCSprite* a = CCSprite::create("unlock_ticketon.png");
		a->setPosition(ccp(184, 145));
		addChild(a);
		
		CCSprite* b = CCSprite::create("unlock_ticketoff.png");
		b->setPosition(ccp(297, 145));
		addChild(b);
		
		
	}
	else
	{
		CCSprite* a = CCSprite::create("unlock_ticketoff.png");
		a->setPosition(ccp(184, 145));
		addChild(a);
		
		CCSprite* b = CCSprite::create("unlock_ticketoff.png");
		b->setPosition(ccp(297, 145));
		addChild(b);
	}
	
	CCLabelTTF* desc = CCLabelTTF::create(gt("youneedticket").c_str(), defaultFont, 14.f, CCSizeMake(247, 37), kCCTextAlignmentCenter);
	
	desc->setPosition(ccp(244, 203));
	
	addChild(desc);
	
	CCMenu* _menu = CCMenu::create();
	
	_menu->setPosition(ccp(0, 0));
	
	addChild(_menu);
	_menu->setTouchPriority(INT_MIN);
	CCMenuItem* exit = CCMenuItemImage::create("unlock_exit.png", "unlock_exit.png", this, menu_selector(Ticket::EXIT));
	_menu->addChild(exit);
	exit->setPosition(ccp(165, 74));
	CCMenuItem* ask = CCMenuItemImage::create("unlock_askfriends.png", "unlock_askfriends.png", this, menu_selector(Ticket::ASK));
	ask->setPosition(ccp(314, 74));
	_menu->addChild(ask);

}
void Ticket::EXIT(CCObject*)
{
	removeFromParentAndCleanup(true);
}

void Ticket::ASK(CCObject*)
{
	
	if(FBConnector::get()->isLogin())
		FBConnector::get()->sendRequest("", gt("ticketreq").c_str(), "gameticketreq", this, fb_selector(Ticket::SENDTICKET));
	else
	{
		KSModalAlert("", gt("needlogin").c_str(), 1,
					 "OK", NOPARAM);
		removeFromParentAndCleanup(true);
	}
	//FBConnector::get()->sendInvite("", "SportsWorldCup", "www", "http://zsoo.net", "", 0, 0);
}

void Ticket::SENDTICKET(JsonBox::Value v2)
{
	removeFromParentAndCleanup(true);
	if(v2["error"].getInt() == 0)
	{
		NSDefault::setTicketIsStarted(1);
		KSModalAlert("", gt("sended").c_str(), 1, "OK", NOPARAM);
	}
	else
		KSModalAlert("", gt("fail").c_str(), 1, "OK", NOPARAM);
}


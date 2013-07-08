//
//  GoToHome.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 19..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "GoToHome.h"
#include "GraphDog.h"
#include "SWIntro.h"
#include "GameSystem.h"
#include "LoadingScene.h"
#include "KS_Util.h"
#include "GDTranslate.h"
using namespace GDTranslate;
GoToHome::GoToHome()
{
	
}
GoToHome::~GoToHome()
{
//	
}



void GoToHome::registerWithTouchDispatcher()
{
//	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority - 1, true);
}
bool GoToHome::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = pTouch;

	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	
	return true;
}
//
//void GoToHome::ccTouchesMoved(CCSet* pTouches, CCEvent* pEvent)
//{
//	
//}
//void GoToHome::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent)
//{
//	
//}
bool GoToHome::init()
{
	CCLayer::init();
	CCSprite* mainBack = CCSprite::create("main_back.png");
	mainBack->setPosition(ccp(240, 160));
	addChild(mainBack);
	CCLayer* myLayer = CCLayer::create();
	addChild(myLayer);
	CCSprite* titleBack = CCSprite::create("main_titleback.png");
	titleBack->setPosition(ccp(240, 160));
	titleBack->runAction(CCRepeatForever::create(CCRotateBy::create(2.f*1.f/0.33f, 360.f)));
	myLayer->addChild(titleBack, 0);
	CCSprite* title = CCSprite::create("main_title.png");
	title->setPosition(ccp(240, 160));
	myLayer->addChild(title, 0);
	
	//this->setTouchEnabled(true);
	graphdog->command("getweek", 0, this, gd_selector(ThisClassType::finishGetWeek));
	return true;
}
void GoToHome::finishGetWeek(JsonBox::Object js)
{
	try
	{
		if(js["state"].getString() != "ok")
			throw string(gt("netfail").c_str());
		playInfo->currentWeek = js["week"].getInt();
		if(playInfo->currentGame != "WORLDCUP")
		{
			ReplaceSceneParam(SWIntro, INIT_MODE::GAMEOVER);
		}
		else
			ReplaceSceneParam(SWIntro, INIT_MODE::WORLDCUP_GAMEOVER);
	}
	catch(const string& msg)
	{
		KSModalAlert("", msg.c_str(), 1,
					 gt("retrying").c_str(), this, callfunc_selector(ThisClassType::RETRY));
	}
}

void GoToHome::RETRY()
{
	graphdog->command("getweek", 0, this, gd_selector(ThisClassType::finishGetWeek));
}
void GoToHome::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}
void GoToHome::onExit()
{
	
	
	CCLayer::onExit();
}

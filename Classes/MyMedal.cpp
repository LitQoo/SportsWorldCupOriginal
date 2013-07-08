//
//  MyMedal.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 7..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "MyMedal.h"
#include "KS_Util.h"
#include "NSDefault.h"
#include "Bridge.h"
#include "AKSaveData.h"
#include "ACSaveData.h"
#include "BS2SaveData.h"
#include "HWSaveData.h"
#include "WCSaveData.h"
MyMedal::MyMedal()
{
	
}
MyMedal::~MyMedal()
{
	
}


bool MyMedal::init()
{
	CCLayer::init();
		
	this->setTouchEnabled(true);
	
	thiz = CCSprite::create("start_medal.png");
	addChild(thiz);
	
	g = CCLabelBMFont::create(KS_Util::stringWithFormat("%d", NSDefault::getGoldMedal()).c_str(), "medal.fnt");
	g->setPosition(ccp(35, 75));
	thiz->addChild(g, 2);
	
	s = CCLabelBMFont::create(KS_Util::stringWithFormat("%d", NSDefault::getSilverMedal()).c_str(), "medal.fnt");
	s->setPosition(ccp(78, 75));
	thiz->addChild(s, 2);
	
	b = CCLabelBMFont::create(KS_Util::stringWithFormat("%d", NSDefault::getCopperMedal()).c_str(), "medal.fnt");
	b->setPosition(ccp(121, 75));
	thiz->addChild(b, 2);
	
	t = CCLabelBMFont::create(KS_Util::stringWithFormat("%d", NSDefault::getTop10Medal()).c_str(), "medal.fnt");
	t->setPosition(ccp(164, 75));
	thiz->addChild(t, 2);
	
	
	w = CCLabelBMFont::create(
							  KS_Util::stringWithFormat("%d", playInfo->wcweekly).c_str(), "totalscore.fnt");
//			  AKSaveData::getWeeklyMaxScore().second + ACSaveData::getWeeklyMaxScore().second +
//																				BS2SaveData::getWeeklyMaxScore().second + HWSaveData::getWeeklyMaxScore().second).c_str(), "totalscore.fnt");
	w->setPosition(ccp(100, 29));
	thiz->addChild(w, 2);
	return true;
}
void MyMedal::syncInfo()
{
	g->setString(KS_Util::stringWithFormat("%d", NSDefault::getGoldMedal()).c_str());
	s->setString(KS_Util::stringWithFormat("%d", NSDefault::getSilverMedal()).c_str());
	b->setString(KS_Util::stringWithFormat("%d", NSDefault::getCopperMedal()).c_str());
	t->setString(KS_Util::stringWithFormat("%d", NSDefault::getTop10Medal()).c_str());
	w->setString(KS_Util::stringWithFormat("%d",
										   playInfo->wcweekly).c_str());
}
void MyMedal::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}
void MyMedal::onExit()
{
	
	
	CCLayer::onExit();
}

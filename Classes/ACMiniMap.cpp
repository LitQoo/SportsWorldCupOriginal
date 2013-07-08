//
//  ACMiniMap.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 25..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "ACMiniMap.h"
#include "KSBlock.h"
#include "KSBaseScene.h"
#include "KS_Util.h"
#include "SceneUtil.h"
const float ACMiniMap::RADIUS = 25.f;
ACMiniMap::ACMiniMap()
{
	pastPoint = 0;
	nowPoint = 0;
}
ACMiniMap::~ACMiniMap()
{
}
void ACMiniMap::finish1to2()
{
	
	
	auto mator = SceneUtil::playAnimation("aw_arrowscore_2to1.png", 0.07, 5, 5, 90, 110, 1);
	addChild(mator.second, 2);
	mator.second->runAction(CCSequence::create(
												CCCallFunc::create(this, callfunc_selector(ThisClassType::finish2to1_1)),
												mator.first,												
												CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite)),
												CCCallFunc::create(this, callfunc_selector(ThisClassType::finish2to1)),
												 0));
	
	
	
//	runAction(CCSequence::create(0,0,0));
}

void ACMiniMap::finish2to1_1()
{
	
	back2->setVisible(false);
}
void ACMiniMap::finish2to1()
{
	back->setVisible(true);
}
void ACMiniMap::pushHistroy(CCPoint direction, int point)
{
	auto mator = SceneUtil::playAnimation("aw_arrowscore_1to2.png", 0.07, 5, 5, 90, 110, false);
	back->setVisible(false);
//	mator.second->runAction(CCSequence::create(mator.first,
//												KSFunc(back2->setVisible(true);),
//												CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite)),
//												 0));
	mator.second->runAction(CCSequence::create(mator.first,
												KSFunc(back2->setVisible(true);),
												CCDelayTime::create(0.5f),
												CCCallFunc::create(this, callfunc_selector(ThisClassType::finish1to2)),
												CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite)),
												0));
	
	addChild(mator.second, 1);
	if(nowPoint)
		nowPoint->setVisible(false);
	
	if(point >= 0 && point < 10)
		score->setString(KS_Util::stringWithFormat("%d", point).c_str());
	else
		score->setString("");
	if(point > 0)
	{
		CCPoint dotPosition = direction;

		CCPoint adder = ccp(back->getContentSize().width / 2.f, back->getContentSize().height / 2.f);
		dotPosition = ccpAdd(dotPosition, adder);
		CCSprite* past = CCSprite::create("aw_arrowscore_past.png");
		past->setPosition(dotPosition);
		back->addChild(past, 1);
		CCSprite* now = CCSprite::create("aw_arrowscore_now.png");
		now->setPosition(dotPosition);
		back->addChild(now, 1);
		
		pastPoint = past;
		nowPoint = now;
	}
	
	
}
bool ACMiniMap::init()
{
	CCSprite::init();
	ignoreAnchorPointForPosition(true);
	back = CCSprite::create("aw_arrowscore_back.png");
	back2 = CCSprite::create("aw_arrowscore_back2.png");
	//back->setPosition(ccp(78, 81));
	addChild(back, 1);
	back2->setVisible(false);
	addChild(back2, 2);
	score = CCLabelAtlas::create("0", "aw_arrowscore_font.png", 42, 39, '0');
//	score->setPosition(ccp(50, 95));
	score->setPosition(ccp(24, 32));
	back2->addChild(score, 2);
	return true;
}
void ACMiniMap::onEnterTransitionDidFinish()
{
	CCSprite::onEnterTransitionDidFinish();
}
void ACMiniMap::onExit()
{
	
	
	CCSprite::onExit();
}

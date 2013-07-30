//
//  GameScene.cpp
//  ArcherWorldCupX
//
//  Created by apple on 12. 7. 6..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.lua_tinker::get<float>(L, "flagXMinDistance")
//

#include "SKGameScene.h"



#include "PauseLayer.h"
#include "SimpleAudioEngine.h"
#include "SKPlay.h"
#include "SKSaveData.h"
#include "KSBaseScene.h"
#include "GameOver.h"
#include "LoadingScene.h"
#include "SWIntro.h"
#include "GameSystem.h"
#include "NSDefault.h"
#include "PauseLayer.h"
#include "ACSaveData.h"
#include "AKSaveData.h"
#include "BS2SaveData.h"
#include "HWSaveData.h"
#include "SKSaveData.h"
#include "WorldCupMiddle.h"
#include "WCPlay.h"
#include "KSoundEngine.h"
#include "KS_Util.h"
#include "SceneUtil.h"
#include "GDTranslate.h"
using namespace GDTranslate;
using namespace cocos2d;
using namespace CocosDenshion;
float SKGameScene::GND_CREATION_TIME = 0.25f;
float SKGameScene::CRASH_ACTION_TIME = 1.f;

//float SKGameScene::AHEAD_INIT_TIME = 1.2f;

//float SKGameScene::FLAG_CREATION_DISANCE = 0.5f;

void SKGameScene::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void SKGameScene::finishTimeOver()
{
	
	graphdog->command("getweek", 0, this, gd_selector(ThisClassType::finishGetWeek));
}

bool SKGameScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	setKeypadEnabled(true);
	KSoundEngine::sharedEngine()->playSound("hotshots.mp3");
	//	KSoundEngine::sharedEngine()->playSound("beat area.mp3");
//		KSoundEngine::sharedEngine()->playSound("Block Buster.mp3");
	//	KSoundEngine::sharedEngine()->playSound("Monster Truck.mp3");

	playInfo->__ateCoin = 0;
	
	
	this->setTouchEnabled(true);

	
//	CCSprite* _back = CCSprite::create("aw_back1.png");
	int remainder;
	if(NSDefault::isReview()) //
		remainder = 5;
	else
		remainder = 1;
	CCSprite* _back = CCSprite::create(KS_Util::stringWithFormat("aw_back%d.png",
																	  rand() % remainder + 1).c_str());
	_back->setPosition(ccp(240, 160));
	addChild(_back);

	
	
	
	
	KSoundEngine::sharedEngine()->playSound("readygo.mp3");
	auto mator = SceneUtil::playAnimation("ready.png", 0.1, 2, 10, 256, 65, false);
	auto action = CCSequence::create(mator.first, CCCallFuncN::create(this, callfuncN_selector(ThisClassType::finishReady)), 0);
	mator.second->runAction(action);
	addChild(mator.second, 1000);
	mator.second->setPosition(ccp(240, 160));
	

	{
		map<string, string> item2img;
		item2img["shield"] = "item_shield1_1.png"; item2img["exp"] = "item_exp1_1.png";
		item2img["item_skwide1.png"] = "item_skwide1_1.png"; item2img["item_curvespeed1.png"] = "item_curvespeed1_1.png";
		
		CCPoint itemPositions1[] = {ccp(240, 120)};
		CCPoint itemPositions2[] = {ccp(240 - 30, 120), ccp(240 + 30, 120)};
		CCPoint itemPositions3[] = {ccp(180, 120), ccp(240, 120), ccp(300, 120)};
		CCPoint itemPositions4[] = {ccp(240 - 90, 120), ccp(240 - 30, 120), ccp(240 + 30, 120), ccp(240 + 90, 120)};
		CCPoint itemPositions5[] = {ccp(120, 120), ccp(180, 120), ccp(240, 120), ccp(300, 120), ccp(360, 120)};
		int itemPositionsIndex = 0;
		int items = 0;
		for(auto iter = playInfo->selectedItems.begin(); iter != playInfo->selectedItems.end(); ++iter)
		{
			if(iter->second && item2img[iter->first] != "")
				items++;
		}
		for(auto selectedItemIter = playInfo->selectedItems.begin(); selectedItemIter != playInfo->selectedItems.end(); ++selectedItemIter)
		{
			if(selectedItemIter->second && item2img[selectedItemIter->first] != "")
			{
				
				CCSprite* it = CCSprite::create( item2img[selectedItemIter->first].c_str() );
				if(items == 1)
					it->setPosition(itemPositions1[itemPositionsIndex]);
				else if(items == 2)
					it->setPosition(itemPositions2[itemPositionsIndex]);
				else if(items == 3)
					it->setPosition(itemPositions3[itemPositionsIndex]);
				else if(items == 4)
					it->setPosition(itemPositions4[itemPositionsIndex]);
				else if(items == 5)
					it->setPosition(itemPositions5[itemPositionsIndex]);
				
				addChild(it, 999);
				it->runAction(CCSequence::create(CCDelayTime::create(1.f), CCFadeOut::create(0.3f),
												  CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite)), 0));
				itemPositionsIndex++;
			}
		}
	}
	
	obstacleLayer = SKObstacleLayer::create();
	addChild(obstacleLayer, 2);
	
	
	
	skChar = new SKCharacter;
	skChar->init();
	skChar->autorelease();
	obstacleLayer->addChild(skChar, 1003);
	obstacleLayer->setSkier(skChar);
	
	skChar->createSprite();
	skChar->setDistance(0);
	skChar->setX(0);
	skChar->setPosition(ccp(240, 0));
	skChar->setAnchorPoint(ccp(0.5, 0.f));
	
	CCSprite* fever_back = CCSprite::create("ui_zerobar_back.png");
	fever_back->setPosition(ccp(240, 20));
	addChild(fever_back, 3);
	
	graphics.feverGauge = CCProgressTimer::create(CCSprite::create("ui_zerobar_front.png"));
	graphics.feverGauge->setBarChangeRate(ccp(1, 0));
	graphics.feverGauge->setMidpoint(ccp(0, 0.5f));
	graphics.feverGauge->setPosition(ccp(240, 20));
	graphics.feverGauge->setType(CCProgressTimerType::kCCProgressTimerTypeBar);
	
	addChild(graphics.feverGauge, 3);
	
	graphics.watch1 = CCSprite::create("ui_time1.png");
	graphics.watch1->setPosition(ccp(431, 277));
	addChild(graphics.watch1, 2);
	auto watchMator = SceneUtil::playAnimation("ui_time2.png", 0.05, 2, 2, 85, 75, true);
	watchMator.second->runAction(watchMator.first);
	graphics.watch2 = watchMator.second;
	graphics.watch2->setPosition(ccp(431, 277));
	addChild(graphics.watch2, 2);
	graphics.watch2->setVisible(false);
	
	
	graphics.timeFnt = CCLabelBMFont::create(KS_Util::stringWithFormat("%.0f", (info.remainTime) * BASETIME / info.INIT_GAME_TIME).c_str(), "time1.fnt");
	addChild(graphics.timeFnt, 3);
	graphics.timeFnt->setPosition(ccp(431,274));
	
	graphics.zeroSprite = CCProgressTimer::create(CCSprite::create("back0.png"));
	graphics.zeroSprite->setBarChangeRate(ccp(1, 0));
	graphics.zeroSprite->setMidpoint(ccp(0, 0.5f));
	graphics.zeroSprite->setScale(2.f);
	graphics.zeroSprite->setAnchorPoint(ccp(0, 0));
	graphics.zeroSprite->setPosition(ccp(0, 0));
	graphics.zeroSprite->setType(CCProgressTimerType::kCCProgressTimerTypeBar);
	graphics.zeroSprite->setPercentage(0);
	addChild(graphics.zeroSprite, 0);
	graphics.scoreFnt = CCLabelBMFont::create("0", "uiscore.fnt");
	graphics.scoreFnt->setAnchorPoint(ccp(0.5, 1.f));
	graphics.scoreFnt->setPosition(ccp(240, 320.f));
	graphics.scoreFnt->setScale(0.6f);
	addChild(graphics.scoreFnt, 2);
	
	CCSprite* leftFire = CCSprite::create("sk_left.png");
	leftFire->setPosition(ccp(45, 45));
	addChild(leftFire, 2);
	
	CCSprite* rightFire = CCSprite::create("sk_right.png");
	rightFire->setPosition(ccp(435, 45));
	addChild(rightFire, 2);
	
	CCSprite* ui_gold = CCSprite::create("ui_gold.png");
	ui_gold->setPosition(ccp(432, 221));
	addChild(ui_gold, 10);
	
	graphics.ateGoldFnt = CCLabelBMFont::create("0", "main1.fnt");
	graphics.ateGoldFnt->setPosition(ccp(440, 221));
	addChild(graphics.ateGoldFnt, 11);

	CCMenu* _menu = CCMenu::create(0);
	addChild(_menu, 3);
	_menu->setPosition(CCPointZero);
	
	graphics.pause = CCMenuItemImage::create("ui_stop.png", "ui_stop.png",
														  this, menu_selector(ThisClassType::PAUSE));
	
	_menu->addChild(graphics.pause, 2);
	graphics.pause->setPosition(ccp(24 + 10, 295 - 10));
	
	CCSprite* left = CCSprite::create("left.png");
	left->setAnchorPoint(ccp(1, 0.5f));
	left->setPosition(ccp(0, 160));
	addChild(left, INT_MAX);
	
	CCSprite* right = CCSprite::create("right.png");
	right->setAnchorPoint(ccp(0.f, 0.5f));
	right->setPosition(ccp(480, 160));
	addChild(right, INT_MAX);

	graphics.zeroSprite = CCProgressTimer::create(CCSprite::create("sk_rainbow.png"));
	graphics.zeroSprite->setBarChangeRate(ccp(1, 0));
	graphics.zeroSprite->setMidpoint(ccp(0, 0.5f));
//	graphics.zeroSprite->setScale(2.f);
	graphics.zeroSprite->setAnchorPoint(ccp(0, 1));
	graphics.zeroSprite->setPosition(ccp(0, 320.f));
	graphics.zeroSprite->setType(CCProgressTimerType::kCCProgressTimerTypeBar);
	graphics.zeroSprite->setPercentage(0);
	obstacleLayer->addChild(graphics.zeroSprite, 1);
//	SKObstacle* ee = new SKPense;
//	ee->init();
//	ee->autorelease();
//	obstacleLayer->createObstacle(skChar->getX(), skChar->getDistance(), ee, skChar->MOVESTATE);
	
	
	return true;
}
void SKGameScene::createDecorationProcess()
{
	if(info.nextDecoCreationTime < skChar->getDistance())
	{
		SKObstacle* ee = new SKGrass();
		ee->init();
		ee->autorelease();
		obstacleLayer->createObstacle(skChar->getX()+well512.GetFloatValue(-300.f, 300.f), skChar->getDistance(), ee, skChar->MOVESTATE); //@
		info.nextDecoCreationTime = skChar->getDistance() + (lua_tinker::get<float>(L, "MIN_CREATION_DISTANCE") + (lua_tinker::get<float>(L, "MAX_CREATION_DISTANCE") - lua_tinker::get<float>(L, "MIN_CREATION_DISTANCE")) * KS_Util::rnd()) / 2.f;
	}
}

void SKGameScene::createFlagProcess()
{
	if(info.nextFlagCreationDistance < skChar->getDistance())
	{
		obstacleLayer->createFlag(skChar->getX(), skChar->getDistance());
		info.nextFlagCreationDistance = skChar->getDistance() + (lua_tinker::get<float>(L, "FLAG_MIN_CREATION_DISTANCE") + (lua_tinker::get<float>(L, "FLAG_MAX_CREATION_DISTANCE") - lua_tinker::get<float>(L, "FLAG_MIN_CREATION_DISTANCE")) * KS_Util::rnd() );
	
		nextCreationDistance = skChar->getDistance() + lua_tinker::get<float>(L, "MIN_CREATION_DISTANCE"); //@ 깃발뒤 100만큼 떨어진후 장애물생성
	}
	
}
void SKGameScene::createObstacleProcess()
{
	
	if(nextCreationDistance+lua_tinker::get<float>(L, "MIN_CREATION_DISTANCE")>info.nextFlagCreationDistance)return; //@깃발 100앞에는 생성금지
	
	if(nextCreationDistance < skChar->getDistance())
	{
		
		
		int ofs = KS_Util::rnd()*300-600-300; //@ offset
		int xlo=ofs;
		int obstacle_between_distance = 1400/lua_tinker::get<float>(L, "OBSTACLES_OF_LINE");
		int obtcnt = lua_tinker::get<float>(L, "OBSTACLES_OF_LINE")+KS_Util::rnd()*2-1;
		for(int i=0;i<=obtcnt-1;i++){ //@ 한줄에 5개씩 장애물생성
		//SKTree, SKStone, SKBigStone, SKPense, SKCuttingTree1, SKCuttingTree2
		SKObstacle* ee;
		int r;
		if(info.gameMode == SKGameInfo::ZERO)
			r = 6; //@ 7 , 일단 펜스는 집어쳐
		else
			r = 15;
		int t = rand() % r;
		switch(t)
		{
			case 0:
				ee = new SKTree;
				break;
			case 1:
				ee = new SKStone;
				break;
			case 2:
				ee = new SKStone;
				break;
			case 3:
				ee = new SKBigStone;
				break;
			case 4:
				ee = new SKCuttingTree1;
				break;
			case 5:
				ee = new SKCuttingTree2;
				break;
//			case 6:
//				ee = new SKHumanObs;
//				break;
			default:
				ee = new SKTree;
				//ee = new SKPense;
		}
		ee->init();
		ee->autorelease();
		obstacleLayer->createObstacle(skChar->getX()+xlo, skChar->getDistance(), ee, skChar->MOVESTATE);
		xlo += obstacle_between_distance+ee->SIZE; //@
		}
		//////////////////////
		nextCreationDistance = skChar->getDistance() + lua_tinker::get<float>(L, "MIN_CREATION_DISTANCE") + (lua_tinker::get<float>(L, "MAX_CREATION_DISTANCE") - lua_tinker::get<float>(L, "MIN_CREATION_DISTANCE")) * KS_Util::rnd();
	}
}
void SKGameScene::createGroundProcess()
{
	if(info.nextGroundCreationTime < gameTimer)
	{
		
		obstacleLayer->createGround(skChar->getDistance());
		info.nextGroundCreationTime = gameTimer + GND_CREATION_TIME;
	}

}
void SKGameScene::moveSkier(float dt)
{
	if(skChar->ACTION_STATE == SKCharacter::LEFT_ACTION || skChar->ACTION_STATE == SKCharacter::RIGHT_ACTION)
		info.skierSpeed -= 0.03f; // 0.01 //# 0.03f
	else if(skChar->ACTION_STATE == SKCharacter::STRAIGHT_ACTION){
		info.skierSpeed += 0.03f; // 0.015f; //#0.03f
	}
	
//	if(info.gameMode == SKGameInfo::ZERO) //#
//	{
//		info.skierSpeed *= 1.05f;
//	}
	
	info.skierSpeed = MIN(info.skierSpeed, lua_tinker::get<float>(L, "MAX_SPEED"));
	info.skierSpeed = MAX(info.skierSpeed, lua_tinker::get<float>(L, "MIN_SPEED"));

	skChar->setDistance(skChar->getDistance() + info.skierSpeed);
	
	bool curveSpeed = playInfo->selectedItems["item_curvespeed1.png"];
	float multi = curveSpeed ? 1.3f : 1.f;
	
	if(skChar->MOVESTATE == SKCharacter::LEFT)
	{
		if(skChar->PLAYERSTATE == SKCharacter::STUN)
			skChar->setX(skChar->getX() - lua_tinker::get<float>(L, "STUN_HORIZ_SPEED") * multi);
		else
			skChar->setX(skChar->getX() - lua_tinker::get<float>(L, "HORIZ_SPEED") * multi);
	}
	else if(skChar->MOVESTATE == SKCharacter::RIGHT )
	{
		if(skChar->PLAYERSTATE == SKCharacter::STUN)
			skChar->setX(skChar->getX() + lua_tinker::get<float>(L, "STUN_HORIZ_SPEED") * multi);
		else
			skChar->setX(skChar->getX() + lua_tinker::get<float>(L, "HORIZ_SPEED") * multi);
	}
	else
	{
		skChar->setX(skChar->getX());
	}
	skChar->updateState(dt);
	
	bool finish = !skChar->boostEffect.step(dt);
	
	if(finish)
	{
		if(skChar->BOOSTSTATE == SKCharacter::BOOSTING)
		{
//			aheadRemainTime = AHEAD_INIT_TIME; // 앞으로 가 있는 시간.
//			skChar->BOOSTSTATE = SKCharacter::MAINTAIN;
		}
		else if(skChar->BOOSTSTATE == SKCharacter::DEBOOSTING)
		{
			skChar->BOOSTSTATE = SKCharacter::GROUND;
		}
		else if(skChar->BOOSTSTATE == SKCharacter::GROUND)
		{ // 감속이 끝났다면
			
		}
	}
//	aheadRemainTime -= dt;
//	if(aheadRemainTime <= 0.f && skChar->BOOSTSTATE == SKCharacter::MAINTAIN)
//	{
//		skChar->BOOSTSTATE = SKCharacter::DEBOOSTING; // 뒤로 가는 중...
//		skChar->boostEffect.init(skChar->boostEffect.getValue(), 0, 1.0f);
//	}
	skChar->setPosition(ccp(skChar->getPosition().x, skChar->boostEffect.getValue()));
//	if(notFinish == false) // finished
//	{
//		boostEffect.init(boostEffect.getValue(), 0, 1.f);
//	}
}

void SKGameScene::crashProcess()
{
	SKCharacter::CRASHTYPE crashResult = obstacleLayer->checkCrash();
	if(crashResult == SKCharacter::CRASHTYPE::NO_CRASH)
	{
		crashResult = obstacleLayer->checkCrashFlag();
	}
	
	if(crashResult == SKCharacter::CRASHTYPE::LEFT_CRASH)
	{
		skChar->toStun(crashResult);
		info.skierSpeed /= 2.f;
		CCLog("left crash");
		onCrash();
	}
	else if(crashResult == SKCharacter::CRASHTYPE::RIGHT_CRASH)
	{
		skChar->toStun(crashResult);
		info.skierSpeed /= 2.f;
		CCLog("right crash");
		onCrash();
	}
}


void SKGameScene::onCrash()
{
	bool shieldMode = false;
	
	{
		// 실드 발동!!
		if(playInfo->selectedItems["shield"] != GameShop::NONE && info.usedShield == false)
		{
			info.usedShield = true;
			shieldMode = true;
			KSoundEngine::sharedEngine()->playSound("shield.mp3");
			CCSprite* shield;
			auto shieldMator = SceneUtil::playAnimation("shield.png", 0.07, 4, 8, 160, 160, 2);
			shield = shieldMator.second;
			shield->runAction(CCSequence::createWithTwoActions(shieldMator.first, CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite))));
			shield->setPosition(ccp(240, 200));
			addChild(shield, 10000);
			
		}
	}
	if(shieldMode == false)
	{
		switch(info.gameMode)
		{
			case SKGameInfo::ZERO:
				skplay->combo = 0;
				
				if(info.isGameOvering == false)
				{
					KSoundEngine::sharedEngine()->playSound("uhuh.mp3");
					KSoundEngine::sharedEngine()->playSound("crashonhurdle.mp3");
				}
				graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + lua_tinker::get<float>(L, "CRASH_DEC_VALUE_IN_ZERO"));
				break;
			case SKGameInfo::NORMAL:
				skplay->combo = 0;
				if(info.isGameOvering == false)
				{
					KSoundEngine::sharedEngine()->playSound("uhuh.mp3");
					KSoundEngine::sharedEngine()->playSound("crashonhurdle.mp3");
				}
				graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + lua_tinker::get<float>(L, "CRASH_DEC_VALUE_IN_NORMAL"));
				break;
			default:
				skplay->combo = 0;
				if(info.isGameOvering == false)
				{
					KSoundEngine::sharedEngine()->playSound("uhuh.mp3");
					KSoundEngine::sharedEngine()->playSound("crashonhurdle.mp3");
				}
				graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + lua_tinker::get<float>(L, "CRASH_DEC_VALUE_IN_NORMAL"));
				break;
		}
	}
	
	
	
	if(shieldMode == false)
	{
		runAction(shakeAction(getPosition()));
		KSoundEngine::sharedEngine()->playSound("crashonhurdle.mp3");
	}
	
}
CCFiniteTimeAction* SKGameScene::shakeAction(CCPoint originalPosition)
{
	CCMoveBy* move1 = CCMoveBy::create(0.05f, CCPointMake(-4,3));
	CCMoveBy* move2 = CCMoveBy::create(0.05f, CCPointMake(3,-2));
	CCMoveBy* move3 = CCMoveBy::create(0.05f, CCPointMake(-3,2));
	CCMoveBy* move4 = CCMoveBy::create(0.05f, CCPointMake(3,-4));
	CCMoveBy* move5 = CCMoveBy::create(0.05f, CCPointMake(-4,2));
	
	CCMoveBy* move6 = CCMoveBy::create(0.05f, CCPointMake(3,2));
	CCMoveBy* move7 = CCMoveBy::create(0.05f, CCPointMake(-4,-2));
	CCMoveBy* move8 = CCMoveBy::create(0.05f, CCPointMake(5,2));
	CCMoveBy* move9 = CCMoveBy::create(0.05f, CCPointMake(-2,-4));
	CCMoveBy* move10 = CCMoveBy::create(0.05f, CCPointMake(5,3));
	CCMoveTo* moveBack = CCMoveTo::create(0.05f, originalPosition);
	
	auto shakeWorldSeq = CCSequence::create(move1,move2,move3,move4,move5,move6,move7,move8,move9,move10,moveBack, 0);
	return shakeWorldSeq;
}


// 장애물 촤 이면 true
void SKGameScene::onCha(bool obs)
{
	if(info.isGameOvering == true)
		return;
	// 장애물이면 점수만 올려줌, 장애물이 아니면(깃발이면) 전부 표시.
	if(obs)
	{
		skplay->gameScore = skplay->gameScore.getVar() + scoreFromWhat(obs);
//		wcplay->gameScore = skplay->gameScore.getVar();
//		wcplay->scoreEachGame["SK"] = wcplay->gameScore.getVar();
		CCLabelBMFont* bonusFnt = CCLabelBMFont::create(KS_Util::stringWithFormat("+%d", scoreFromWhat(obs)).c_str(), "bonusscore.fnt");
		bonusFnt->setPosition(ccp(graphics.scoreFnt->getPosition().x + graphics.scoreFnt->getContentSize().width / 3.f,
								  graphics.scoreFnt->getPosition().y - graphics.scoreFnt->getContentSize().height / 3.f));
		addChild(bonusFnt, 2);
		
		float virtualMaxScore = MAX(100000, playInfo->skscore);
		float goalScale = MIN(1.3f, 0.6f + (float)skplay->gameScore.getVar() * 2.f / (float)virtualMaxScore);
		bonusFnt->setScale(0.3f);
		auto action0 = CCEaseBackOut::create(CCScaleTo::create(1.f, goalScale));
		auto action1 = CCDelayTime::create(0.2f);
		auto action2 = CCFadeOut::create(0.2f);
		auto action3 = CCCallFuncN::create(this, callfuncN_selector(SKGameScene::deleteSprite));
		auto totalAction = CCSequence::create(action0, action1, action2, action3, 0);
		bonusFnt->runAction(totalAction);
	
		
		graphics.scoreFnt->runAction(CCEaseBackOut::create(CCScaleTo::create(1.f, goalScale)));

	}
	else
	{
		skplay->combo++;
		
		//	if(BOOSTSTATE != BOOSTING)
		{
			
			
		}
		info.lastChaTime = gameTimer;
		KSoundEngine::sharedEngine()->playSound("skigihap.wav");
		bool bShout = false;
		if(info.gameMode == SKGameInfo::ZERO)
		{
			if(skplay->combo.getVar() % 10 == 0)
			{
				KSoundEngine::sharedEngine()->playSound("shout.mp3");
				bShout = true;
			}
		}
		
		if(bShout == false)
		{
			if(skplay->combo.getVar() % 5 == 0)
			{
				int r = rand() % 8;
				KSoundEngine::sharedEngine()->playSound(KS_Util::stringWithFormat("p%d.mp3", r).c_str());
			}
		}
		
		if(info.gameMode == SKGameInfo::ZERO)
		{
			graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + lua_tinker::get<float>(L, "INC_VALUE_IN_ZERO"));
			eatCoin(4);
		}
		else
		{
			graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + lua_tinker::get<float>(L, "INC_VALUE_IN_NORMAL"));
		}
		
		
		
		if(info.gameMode == SKGameInfo::ZERO)
		{ // 콤보시 제로모드 상태면 주는 패널티 같은 것들.
			
		}
		else
		{
		}
		
		if(info.gameMode != SKGameInfo::ZERO && graphics.feverGauge->getPercentage() >= 100.f && info.isGameOvering == false)
		{
			onEnterZero();
		}
		
		KS::KSLog("%", skplay->combo.getVar());
		skplay->gameScore = skplay->gameScore.getVar() + scoreFromWhat(obs);
//		wcplay->gameScore = skplay->gameScore.getVar();
//		wcplay->scoreEachGame["SK"] = wcplay->gameScore.getVar();
		CCLabelBMFont* bonusFnt = CCLabelBMFont::create(KS_Util::stringWithFormat("+%d", scoreFromWhat(obs)).c_str(), "bonusscore.fnt");
		bonusFnt->setPosition(ccp(graphics.scoreFnt->getPosition().x + graphics.scoreFnt->getContentSize().width / 3.f,
								  graphics.scoreFnt->getPosition().y - graphics.scoreFnt->getContentSize().height / 3.f));
		addChild(bonusFnt, 2);
		
		float virtualMaxScore = MAX(100000, playInfo->skscore);
		float goalScale = MIN(1.3f, 0.6f + (float)skplay->gameScore.getVar() * 2.f / (float)virtualMaxScore);
		bonusFnt->setScale(0.3f);
		auto action0 = CCEaseBackOut::create(CCScaleTo::create(1.f, goalScale));
		auto action1 = CCDelayTime::create(0.2f);
		auto action2 = CCFadeOut::create(0.2f);
		auto action3 = CCCallFuncN::create(this, callfuncN_selector(SKGameScene::deleteSprite));
		auto totalAction = CCSequence::create(action0, action1, action2, action3, 0);
		bonusFnt->runAction(totalAction);
		

		graphics.scoreFnt->runAction(CCEaseBackOut::create(CCScaleTo::create(1.f, goalScale)));
		
		//110, 250
		auto mator = SceneUtil::playAnimation("game_combo.png", 0.1, COMBO_COLS, COMBO_FRAME, COMBO_WIDTH, COMBO_HEIGHT, false);
		mator.second->runAction(CCSequence::create(mator.first,
													//CCDelayTime::create(0.5f),
													CCCallFuncN::create(this, callfuncN_selector(SKGameScene::deleteSprite)), 0));
		mator.second->setPosition(ccp(85, 250));
		addChild(mator.second, 2);
		
		CCLabelBMFont* comboFnt = CCLabelBMFont::create(KS_Util::stringWithFormat("%d", skplay->combo.getVar()).c_str(), "gamecombo.fnt");
		comboFnt->setPosition(ccp(85, 270));
		comboFnt->setScale(0.6f);
		comboFnt->runAction(CCSequence::create(CCEaseBackOut::create(CCScaleTo::create(0.3, 1.f)),
												CCDelayTime::create(0.5f), CCFadeOut::create(0.5f),
												CCCallFuncN::create(this, callfuncN_selector(SKGameScene::deleteSprite)), 0));
		addChild(comboFnt, 2);

	}
		
	
	
}
void SKGameScene::updateScoreFnt(float dt)
{
	int goalScore = skplay->gameScore.getVar();
	int displayedScore = atoi(graphics.scoreFnt->getString());
	if(displayedScore < goalScore )
	{
		int adderValue = (goalScore - displayedScore) / 30 + 21;
		adderValue = MIN(adderValue, goalScore - displayedScore);
		graphics.scoreFnt->setString(KS_Util::stringWithFormat("%d", displayedScore + adderValue).c_str());
	}
}
void SKGameScene::judgeChaProcess()
{
	if(true) // 장애물 촤~
	{
		int chaCount = 0;
		SKObstacleLayer::OBSPASSTYPE result = obstacleLayer->checkObsCha(chaCount);
		if(result == SKObstacleLayer::OBSPASSTYPE::OBS_CHA)
		{ //  촤촤촤
			onCha(true);
			
		}
		else if(result == SKObstacleLayer::OBSPASSTYPE::OBS_NOCHA)
		{ // 노챠
			
		}
	}
	
	int chaCount = 0;
	SKObstacleLayer::FLAGPASSTYPE result = obstacleLayer->checkFlagCha(chaCount);
	if(result == SKObstacleLayer::FLAGPASSTYPE::FLAG_CHA)
	{ //  촤촤촤
		onCha(false);
		
	}
	else if(result == SKObstacleLayer::FLAGPASSTYPE::FLAG_NOCHA)
	{ // 노챠
		
	}
	
}

void SKGameScene::timeProcess(float dt)
{
	if(info.gameMode == SKGameInfo::NORMAL)
	{
		info.remainTime -= dt;
	}
	
	if(info.remainTime <= 10 && !info.in10s)
	{
		KSoundEngine::sharedEngine()->playSound("hurryup.mp3");
		info.in10s = true;
		graphics.watch1->setVisible(false);
		graphics.watch2->setVisible(true);
	}
	if(info.remainTime >= 0)
		graphics.timeFnt->setString(KS_Util::stringWithFormat("%.0f", (info.remainTime) * BASETIME / (float)info.INIT_GAME_TIME).c_str());
	else
	{
		
		{
#if 0
			if(playInfo->selectedItems["item_random"] != GameShop::NONE&& !graphics.watch3->isVisible())
			{
				KSoundEngine::sharedEngine()->playSound("bonustime.mp3");
				graphics.watch2->setVisible(false);
				graphics.watch3->setVisible(true);
				graphics.timeFnt->setVisible(false);
				auto mator = SceneUtil::playAnimation("bonustime.png", 0.08, 6, 6, 135, 135, 1);
				mator.second->runAction(CCSequence::create(mator.first, CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite)),0));
				mator.second->setPosition(ccp(431, 277));
				addChild(mator.second, 10);
			}
#endif
		}
		CCLog("z %d", __LINE__);
		if(info.remainTime <= -info.bonusTime && info.isGameOvering == false)
		{
			setTouchEnabled(false);
			skChar->MOVESTATE = SKCharacter::NONE;
			info.isGameOvering = true;
			graphics.pause->setEnabled(false);
			auto mator = SceneUtil::playAnimation("timeover.png", 0.1, 2, 4, TIMEOVERWIDTH, TIMEOVERHEIGHT, 8);
			KSoundEngine::sharedEngine()->playSound("timeover.mp3");
			CCLog("z %d", __LINE__);
			mator.second->setPosition(ccp(240, 160));
			mator.second->setScale(1.2f);
			CCLog("z %d", __LINE__);
			auto action = CCSequence::create(mator.first, CCCallFunc::create(this, callfunc_selector(ThisClassType::finishTimeOver)), 0);
			CCLog("z %d", __LINE__);
			mator.second->runAction(action);
			CCLog("z %d", __LINE__);
			addChild(mator.second, INT_MAX);
			CCLog("z %d", __LINE__);
		}
	}
	
	if(info.gameMode == SKGameInfo::ZERO)
	{
		info.timeAfterZero += dt;
		
		
		//		CCLog("-- %d", info.zeroStep);
		graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + lua_tinker::get<float>(L, "ZERO_DEC_VALUE"));
		if(graphics.feverGauge->getPercentage() <= 0.f)
		{
			onExitZero();
		}
	}
//	bool bZero = info.gameMode == SKGameInfo::ZERO ? true : false;

	if(info.lastChaTime + lua_tinker::get<float>(L, "COMBO_RESET_TIME") < gameTimer)
	{
		skplay->combo = 0;
		
		info.lastChaTime = gameTimer;
	}
}

void SKGameScene::update(float dt)
{
	dt = 1.f/60.f;
	gameTimer += dt;
	createGroundProcess();
	createObstacleProcess();
	createDecorationProcess();
	createFlagProcess();
	moveSkier(dt);
	crashProcess();
	judgeChaProcess();
	timeProcess(dt);
	
	obstacleLayer->destroyObstacle(skChar->getDistance());
	obstacleLayer->destroyGround(skChar->getDistance());
	obstacleLayer->destroyFlag(skChar->getDistance());
	obstacleLayer->render(skChar->getX(), skChar->getDistance());
	obstacleLayer->focusFlag();
	updateScoreFnt(dt);
	skChar->render();
	updateBalance();
}
void SKGameScene::PAUSE(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	SKPauseLayer* pauseLayer = SKPauseLayer::create();
	//pauseLayer->setPosition(ccp(0, -20));
	addChild(pauseLayer, 1000);
	
	KS::KSLog("pause");
}

void SKGameScene::finishGetWeek(JsonBox::Object js)
{
	try
	{
		if(js["state"].getString() != "ok")
			throw string(gt("netfail").c_str());
		KS::KSLog("%", js);
		playInfo->currentWeek = js["week"].getInt();
		if(playInfo->currentGame != "WORLDCUP")
			ReplaceSceneParam(SWIntro, INIT_MODE::GAMEOVER);
		else
		{
			if(playInfo->getWorldcupGame() != "")
				ReplaceSceneParam(WorldCupMiddle, skplay->gameScore.getVar());
			else
				ReplaceSceneParam(SWIntro, INIT_MODE::WORLDCUP_GAMEOVER);
		}
	}
	catch(const string& msg)
	{
		KSModalAlert("", msg.c_str(), 1,
							  gt("retrying").c_str(), this, callfunc_selector(ThisClassType::RETRY));
	}
}
void SKGameScene::RETRY()
{
	graphdog->command("getweek", 0, this, gd_selector(ThisClassType::finishGetWeek));
}



void SKGameScene::finishGo(CCNode* n)
{
	n->removeFromParentAndCleanup(true);
	
	schedule(schedule_selector(ThisClassType::update));
	//this->schedule(schedule_selector(ThisClassType::gameTimer));
}
void SKGameScene::finishReady(CCNode* n)
{
	n->removeFromParentAndCleanup(true);
	auto mator = SceneUtil::playAnimation("go.png", 0.075, 2, 4, 256, 65, false);
	auto action0 = CCSpawn::create(CCSequence::createWithTwoActions(CCDelayTime::create(0.1f), CCScaleTo::create(0.4f, 4.f)), CCSequence::createWithTwoActions(CCDelayTime::create(0.3f), CCFadeOut::create(0.2f)), mator.first, 0);
	auto action = CCSequence::create(action0, CCCallFuncN::create(this, callfuncN_selector(ThisClassType::finishGo)), 0);
	mator.second->runAction(action);
	addChild(mator.second, 1000);
	mator.second->setPosition(ccp(240, 160));
}

void SKGameScene::onExitZero()
{
	graphics.zeroSprite->runAction(CCSequence::create(CCProgressFromTo::create(0.4f, 100.f, 0.f),
													   0));
	info.gameMode = SKGameInfo::NORMAL;
	KSoundEngine::sharedEngine()->playSound("hotshots.mp3");
	KSoundEngine::sharedEngine()->playSound("exitzero.mp3");
	skChar->runAction(CCMoveBy::create(0.5f, ccp(0, -30)));
	graphics.watch2->resumeSchedulerAndActions();
}
void SKGameScene::onEnterZero()
{
	graphics.zeroSprite->runAction(CCSequence::create(CCProgressFromTo::create(0.4f, 0, 100.f),
													   0));
	info.gameMode = SKGameInfo::ZERO;
	KSoundEngine::sharedEngine()->playSound("RetroSpace.mp3");
	
	KSoundEngine::sharedEngine()->playSound("fever.mp3");
	graphics.watch2->pauseSchedulerAndActions();
//	skChar->runAction(CCMoveBy::create(0.5f, ccp(0, 30)));
}
// Touch

bool SKGameScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	KS::KSLog("%", pTouch->getLocationInView().x);
	float clickX = pTouch->getLocationInView().x;
	currentTouchCount++;
	if(clickX <= 240)
	{
		skChar->MOVESTATE = SKCharacter::LEFT;
//		if(skChar->PLAYERSTATE != SKCharacter::STUN)
//			skChar->leftAction();
	}
	else
	{
		skChar->MOVESTATE = SKCharacter::RIGHT;
//		if(skChar->PLAYERSTATE != SKCharacter::STUN)
//			skChar->rightAction();
	}
	
//	tkplay->gameScore += rand()%10000;
//	wcplay->gameScore = tkplay->gameScore.getVar();
//	wcplay->scoreEachGame["SK"] = wcplay->gameScore.getVar();
	
//	finishTimeOver();
	return true;
}

void SKGameScene::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	currentTouchCount--;
	if(currentTouchCount < 0)
	{
		currentTouchCount = 0;
	}
	if(currentTouchCount == 0)
	{
		skChar->MOVESTATE = SKCharacter::NONE;
//		skChar->goStraight();
	}
}
void  SKGameScene::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	
	CCPoint location = pTouch->getLocationInView();
	location=CCDirector::sharedDirector()->convertToGL(location);
//	CCPoint location2 = convertToNodeSpace(location);

}
void  SKGameScene::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
//	if(gameStep==GSHandlingBow)gameStep=GSShootArrow;
	currentTouchCount--;
	if(currentTouchCount < 0)
	{
		currentTouchCount = 0;
	}
	
	if(currentTouchCount == 0)
	{
		skChar->MOVESTATE = SKCharacter::NONE;
//		skChar->goStraight();
	}
}

SKGameScene::~SKGameScene(){
	//delete glayer;
//	delete sResult;
	playInfo->gamePtr = 0;
	
	LuaManager::shared()->closeLua("sk.lua");
//	lua_close(lua);
}

void SKGameScene::eatCoin(int n)
{
	NSDefault::setGold(NSDefault::getGold() + n);
	info.ateGoldCount+=n;
	playInfo->__ateCoin = info.ateGoldCount;
	graphics.ateGoldFnt->setString(KS_Util::stringWithFormat("%d", info.ateGoldCount).c_str());CCLog("%d", __LINE__);
	for(int i=0; i<n; i++)
	{
		auto retainAnimation = SceneUtil::playAnimation("coin.png", 0.07f, 6, 6,
														SKGraphics::COIN_WIDTH, SKGraphics::COIN_HEIGHT, false);
		addChild(retainAnimation.second, 2);
		retainAnimation.second->setPosition(ccp(240, 20));
		retainAnimation.second->setScale(0.3f);

		ccBezierConfig bc;
		bc.controlPoint_1 = retainAnimation.second->getPosition();
		bc.controlPoint_2 = ccp(450 + rand()%50, 50);
		bc.endPosition = ccp(441, 221);

		auto moveAction = CCSequence::create(CCDelayTime::create(i * 0.2f), KSFunc(KSoundEngine::sharedEngine()->playSound("se_takegold01.mp3");),
											  CCSpawn::create(CCBezierTo::create(1.2f, bc), CCScaleTo::create(1.2f, 1.f), 0), 0 );

		auto action2 = CCSpawn::createWithTwoActions(retainAnimation.first, moveAction); // 가면서 사라짐.

		auto action3 = CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite));

		auto totalAction = CCSequence::create(action2, action3, 0);
		retainAnimation.second->runAction(totalAction);
	}
	CCLog("e2");
}







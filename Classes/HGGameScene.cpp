//
//  GameScene.cpp
//  ArcherWorldCupX
//
//  Created by apple on 12. 7. 6..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "HGGameScene.h"


#include "HGGameLayer.h"
#include "PauseLayer.h"
#include "SimpleAudioEngine.h"
#include "HGPlay.h"
#include "HGSaveData.h"
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
#include "HGSaveData.h"
#include "WorldCupMiddle.h"
#include "WCPlay.h"
#include "KSoundEngine.h"
#include "KS_Util.h"
#include "HGPlay.h"
#include "SceneUtil.h"
#include "HGPlayer.h"
#include "GDTranslate.h"
using namespace GDTranslate;
using namespace cocos2d;
using namespace CocosDenshion;


//float SKGameScene::AHEAD_INIT_TIME = 1.2f;

//float SKGameScene::FLAG_CREATION_DISANCE = 0.5f;

void HGGameScene::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void HGGameScene::finishTimeOver()
{
	graphdog->command("getweek", 0, this, gd_selector(ThisClassType::finishGetWeek));
//	graphdog->command("getweek", 0, this, gd_selector(ThisClassType::finishGetWeek));
}

bool HGGameScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	setKeypadEnabled(true);
	CCSprite* back = CCSprite::create("hg_back.png");
	addChild(back);
	back->setPosition(ccp(240, 160));
	gameLayer = HGGameLayer::create();
	gameLayer->setL(lua);
	addChild(gameLayer, 1);
	KSoundEngine::sharedEngine()->playSound("hotshots.mp3");
	//	KSoundEngine::sharedEngine()->playSound("beat area.mp3");
//		KSoundEngine::sharedEngine()->playSound("Block Buster.mp3");
	//	KSoundEngine::sharedEngine()->playSound("Monster Truck.mp3");

	playInfo->__ateCoin = 0;
	
	
	this->setTouchEnabled(true);

	
//	CCSprite* _back = CCSprite::spriteWithFile("aw_back1.png");
	
//	CCSprite* _back = CCSprite::create(KS_Util::stringWithFormat("aw_back1.png").c_str());
//	_back->setPosition(ccp(240, 160));
//	addChild(_back);

//	gameLayer->runAction(CCRepeat::create(CCRotateBy::create(6.f, 360.f), -1));
	if(playInfo->selectedItems["item_hanggliding1.png"] != GameShop::NONE)
		gameLayer->getHgPlayer()->runAction(CCScaleTo::create(0.5f, 0.7f));
	
	if(playInfo->selectedItems["item_hanggliding2.png"] != GameShop::NONE)
	{
		gameLayer->runAction(CCScaleTo::create(0.5f, 0.8f));
	}
	else
	{
//		gameLayer->setScale(0.6f);
		gameLayer->setScale(1.2f);
	}
	KSoundEngine::sharedEngine()->playSound("readygo.mp3");
	auto mator = SceneUtil::playAnimation("ready.png", 0.1, 2, 10, 256, 65, false);
	auto action = CCSequence::create(mator.first, CCCallFuncN::create(this, callfuncN_selector(ThisClassType::finishReady)), 0);
	mator.second->runAction(action);
	addChild(mator.second, 1000);
	mator.second->setPosition(ccp(240, 160));
	

	{
		map<string, string> item2img;
		item2img["shield"] = "item_shield1_1.png"; item2img["exp"] = "item_exp1_1.png";
		item2img["item_hanggliding1.png"] = "item_hanggliding1_1.png"; item2img["item_hanggliding2.png"] = "item_hanggliding2_1.png";
		
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
	
	
	
	CCSprite* fever_back = CCSprite::create("ui_zerobar_back.png");
	fever_back->setPosition(ccp(240, 20));
	addChild(fever_back, 3);
	
	graphics.feverGauge = CCProgressTimer::create(CCSprite::create("ui_zerobar_front.png"));
	graphics.feverGauge->setPosition(ccp(240, 20));
	graphics.feverGauge->setMidpoint(ccp(0, 0.5f));
	graphics.feverGauge->setBarChangeRate(ccp(1, 0));
	graphics.feverGauge->setPercentage(0);
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
//	graphics.zeroSprite->runAction(CCRepeat::create(CCRotateBy::create(6.f, 360.f), -1));
	graphics.zeroSprite->setScale(2.f);
	graphics.zeroSprite->setAnchorPoint(ccp(0, 0));
	graphics.zeroSprite->setPosition(ccp(0, 0));
	graphics.zeroSprite->setMidpoint(ccp(0, 0.5f));
	graphics.zeroSprite->setBarChangeRate(ccp(1, 0));
	graphics.zeroSprite->setType(CCProgressTimerType::kCCProgressTimerTypeBar);
	
	graphics.zeroSprite->setPercentage(0);
	addChild(graphics.zeroSprite);
	graphics.scoreFnt = CCLabelBMFont::create("0", "uiscore.fnt");
	graphics.scoreFnt->setAnchorPoint(ccp(0.5, 1.f));
	graphics.scoreFnt->setPosition(ccp(240, 320.f));
	graphics.scoreFnt->setScale(0.6f);
	addChild(graphics.scoreFnt, 2);
	
	CCSprite* left = CCSprite::create("left.png");
	left->setAnchorPoint(ccp(1, 0.5f));
	left->setPosition(ccp(0, 160));
	addChild(left, INT_MAX);
	
	CCSprite* right = CCSprite::create("right.png");
	right->setAnchorPoint(ccp(0.f, 0.5f));
	right->setPosition(ccp(480, 160));
	addChild(right, INT_MAX);

	CCSprite* ui_gold = CCSprite::create("ui_gold.png");
	ui_gold->setPosition(ccp(432, 221));
	addChild(ui_gold, 10);
	
	graphics.ateGoldFnt = CCLabelBMFont::create("0", "main1.fnt");
	graphics.ateGoldFnt->setPosition(ccp(440, 221));
	addChild(graphics.ateGoldFnt, 11);

	CCMenu* _menu = CCMenu::create();
	addChild(_menu, 3);
	_menu->setPosition(CCPointZero);
	
	graphics.pause = CCMenuItemImage::create("ui_stop.png", "ui_stop.png",
														  this, menu_selector(ThisClassType::PAUSE));
	
	_menu->addChild(graphics.pause, 2);
	graphics.pause->setPosition(ccp(24 + 10, 295 - 10));
	
//	graphics.zeroSprite = CCProgressTimer::create(CCSprite::create("sk_rainbow.png"));
////	graphics.zeroSprite->setScale(2.f);
//	graphics.zeroSprite->setAnchorPoint(ccp(0, 1));
//	graphics.zeroSprite->setPosition(ccp(0, 320.f));
//	graphics.zeroSprite->setMidpoint(ccp(0, 0.5f));
//	graphics.zeroSprite->setBarChangeRate(ccp(1, 0));
//	graphics.zeroSprite->setPercentage(0);
	
	//obstacleLayer->addChild(graphics.zeroSprite, 1);
	
	
	
	
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HGGameScene::onScore), "onHGScore", 0);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HGGameScene::onBuildingScore), "onHGBuildingScore", 0);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HGGameScene::onCrash), "onHGCrash", 0);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HGGameScene::onEatCoin), "onHGEatCoin", 0);
	return true;
}



void HGGameScene::onCrash()
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
			case HGGameInfo::ZERO:
				hgplay->combo = 0;
				
				if(info.isGameOvering == false)
				{
					KSoundEngine::sharedEngine()->playSound("uhuh.mp3");
					KSoundEngine::sharedEngine()->playSound("crashonhurdle.mp3");
				}
				graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + lua_tinker::get<float>(lua, "CRASH_DEC_VALUE_IN_ZERO"));
				break;
			case HGGameInfo::NORMAL:
				hgplay->combo = 0;
				if(info.isGameOvering == false)
				{
					KSoundEngine::sharedEngine()->playSound("uhuh.mp3");
					KSoundEngine::sharedEngine()->playSound("crashonhurdle.mp3");
				}
				graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + lua_tinker::get<float>(lua, "CRASH_DEC_VALUE_IN_NORMAL"));
				break;
			default:
				CCLog("%x %d", graphics.feverGauge, info.gameMode);
				hgplay->combo = 0;
				if(info.isGameOvering == false)
				{
					KSoundEngine::sharedEngine()->playSound("uhuh.mp3");
					KSoundEngine::sharedEngine()->playSound("crashonhurdle.mp3");
				}
				graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + lua_tinker::get<float>(lua, "CRASH_DEC_VALUE_IN_NORMAL"));
				break;
		}
	}
	
	
	
	if(shieldMode == false)
	{
		CCLog("쿵!!!");
		runAction(shakeAction(getPosition()));
		KSoundEngine::sharedEngine()->playSound("crashonhurdle.mp3");
	}

}

void HGGameScene::onBuildingScore()
{
	if(info.isGameOvering == true)
		return;
	
	//KSoundEngine::sharedEngine()->playSound("skigihap.wav");
	bool bShout = false;
	
	int adder = lua_tinker::get<int>(lua, "SCORE_PER_BUILDING");
	hgplay->gameScore = hgplay->gameScore.getVar() + adder;
	
	float virtualMaxScore = MAX(100000, playInfo->hgscore);
	float goalScale = MIN(1.3f, 0.6f + (float)hgplay->gameScore.getVar() * 2.f / (float)virtualMaxScore);
	graphics.scoreFnt->runAction(CCEaseBackOut::create(CCScaleTo::create(1.f, goalScale)));
}
void HGGameScene::onScore()
{
	if(info.isGameOvering == true)
		return;
	
	hgplay->combo++;
	
	KSoundEngine::sharedEngine()->playSound("skigihap.wav");
	bool bShout = false;
	if(info.gameMode == HGGameInfo::ZERO)
	{
		// 10 번에 한번 함성.
//		if(skplay->combo.getVar() % 10 == 0)
//		{
//			KSoundEngine::sharedEngine()->playSound("shout.mp3");
//			bShout = true;
//		}
	}

	// 5번에 한번 함성
//	if(bShout == false)
//	{
//		if(skplay->combo.getVar() % 5 == 0)
//		{
//			int r = rand() % 8;
//			KSoundEngine::sharedEngine()->playSound(KS_Util::stringWithFormat("p%d.mp3", r).c_str());
//		}
//	}
	
	if(info.gameMode == HGGameInfo::ZERO)
	{
		graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + lua_tinker::get<float>(lua, "INC_VALUE_IN_ZERO"));
//		eatCoin(4);
	}
	else
	{
		graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + lua_tinker::get<float>(lua, "INC_VALUE_IN_NORMAL"));
	}
	
	
	if(info.gameMode != HGGameInfo::ZERO && graphics.feverGauge->getPercentage() >= 100.f && info.isGameOvering == false)
	{
		CCLog("zero1!");
		onEnterZero();
	}
	int adderScore = lua_tinker::call<int>(lua, "score", hgplay->combo.getVar());
	hgplay->gameScore = hgplay->gameScore.getVar() + adderScore;
//	wcplay->gameScore = hgplay->gameScore.getVar();
//	wcplay->scoreEachGame["HG"] = wcplay->gameScore.getVar();
	CCLabelBMFont* bonusFnt = CCLabelBMFont::create(KS_Util::stringWithFormat("+%d", adderScore).c_str(), "bonusscore.fnt");
	bonusFnt->setPosition(ccp(graphics.scoreFnt->getPosition().x + graphics.scoreFnt->getContentSize().width / 3.f,
							  graphics.scoreFnt->getPosition().y - graphics.scoreFnt->getContentSize().height / 3.f));
	addChild(bonusFnt, 2);
	
	float virtualMaxScore = MAX(100000, playInfo->hgscore);
	float goalScale = MIN(1.3f, 0.6f + (float)hgplay->gameScore.getVar() * 2.f / (float)virtualMaxScore);
	bonusFnt->setScale(0.3f);
	auto action0 = CCEaseBackOut::create(CCScaleTo::create(1.f, goalScale));
	auto action1 = CCDelayTime::create(0.2f);
	auto action2 = CCFadeOut::create(0.2f);
	auto action3 = CCCallFuncN::create(this, callfuncN_selector(HGGameScene::deleteSprite));
	auto totalAction = CCSequence::create(action0, action1, action2, action3, 0);
	bonusFnt->runAction(totalAction);
	
	
	graphics.scoreFnt->runAction(CCEaseBackOut::create(CCScaleTo::create(1.f, goalScale)));
	
	//110, 250
	auto mator = SceneUtil::playAnimation("game_combo.png", 0.1, COMBO_COLS, COMBO_FRAME, COMBO_WIDTH, COMBO_HEIGHT, false);
	mator.second->runAction(CCSequence::create(mator.first,
											   //CCDelayTime::create(0.5f),
											   CCCallFuncN::create(this, callfuncN_selector(HGGameScene::deleteSprite)), 0));
	mator.second->setPosition(ccp(85, 250));
	addChild(mator.second, 2);
	
	CCLabelBMFont* comboFnt = CCLabelBMFont::create(KS_Util::stringWithFormat("%d", hgplay->combo.getVar()).c_str(), "gamecombo.fnt");
	comboFnt->setPosition(ccp(85, 270));
	comboFnt->setScale(0.6f);
	comboFnt->runAction(CCSequence::create(CCEaseBackOut::create(CCScaleTo::create(0.3, 1.f)),
										   CCDelayTime::create(0.5f), CCFadeOut::create(0.5f),
										   CCCallFuncN::create(this, callfuncN_selector(HGGameScene::deleteSprite)), 0));
	addChild(comboFnt, 2);
	
	

}
CCFiniteTimeAction* HGGameScene::shakeAction(CCPoint originalPosition)
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

void HGGameScene::updateScoreFnt(float dt)
{
	int goalScore = hgplay->gameScore.getVar();
	int displayedScore = atoi(graphics.scoreFnt->getString());
	if(displayedScore < goalScore )
	{
		int adderValue = (goalScore - displayedScore) / 30 + 21;
		adderValue = MIN(adderValue, goalScore - displayedScore);
		graphics.scoreFnt->setString(KS_Util::stringWithFormat("%d", displayedScore + adderValue).c_str());
	}
}

void HGGameScene::timeProcess(float dt)
{
	if(info.gameMode == HGGameInfo::NORMAL)
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
			if(playInfo->selectedItems["item_random"] != GameShop::NONE && !graphics.watch3->isVisible())
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
		if(info.remainTime <= -info.bonusTime && info.isGameOvering == false)
		{
			gameLayer->setIsStarted(false);
			gameLayer->getVoidNode()->stopAllActions();
			setTouchEnabled(false);
			info.isGameOvering = true;
			graphics.pause->setEnabled(false);
			auto mator = SceneUtil::playAnimation("timeover.png", 0.1, 2, 4, TIMEOVERWIDTH, TIMEOVERHEIGHT, 8);
			KSoundEngine::sharedEngine()->playSound("timeover.mp3");
			mator.second->setPosition(ccp(240, 160));
			mator.second->setScale(1.2f);
			auto action = CCSequence::createWithTwoActions(mator.first, CCCallFunc::create(this, callfunc_selector(ThisClassType::finishTimeOver)));
			mator.second->runAction(action);
			addChild(mator.second, INT_MAX);
		}
	}
	
	if(info.gameMode == HGGameInfo::ZERO)
	{
		info.timeAfterZero += dt;
		
		
		//		CCLog("-- %d", info.zeroStep);
		graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + lua_tinker::get<float>(lua, "ZERO_DEC_VALUE"));
		if(graphics.feverGauge->getPercentage() <= 0.f)
		{
			onExitZero();
		}
	}
//	bool bZero = info.gameMode == HGGameInfo::ZERO ? true : false;
	
}

void HGGameScene::onEatCoin()
{
	CCLog("%s %d", __FUNCTION__, __LINE__);
	NSDefault::setGold(NSDefault::getGold() + 1);
	info.ateGoldCount += 1;
	playInfo->__ateCoin = info.ateGoldCount;
	graphics.ateGoldFnt->setString(KS_Util::stringWithFormat("%d", info.ateGoldCount).c_str());
}


void HGGameScene::update(float dt)
{
	dt = 1.f/60.f;
	gameTimer += dt;
	
	timeProcess(dt);
	
	
	updateScoreFnt(dt);
//	lua_State* L = LuaManager::shared()->getLua("hg.lua");
	lua_tinker::call<void>(lua, "updateBalance", hgplay->combo.getVar(), info.gameMode, info.timeAfterZero);
}
void HGGameScene::PAUSE(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	HGPauseLayer* pauseLayer = HGPauseLayer::create();
	//pauseLayer->setPosition(ccp(0, -20));
	addChild(pauseLayer, 1000);
	KS::KSLog("pause");
}

void HGGameScene::finishGetWeek(JsonBox::Object js)
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
				ReplaceSceneParam(WorldCupMiddle, hgplay->gameScore.getVar());
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
void HGGameScene::RETRY()
{
	graphdog->command("getweek", 0, this, gd_selector(ThisClassType::finishGetWeek));
}



void HGGameScene::finishGo(CCNode* n)
{
	n->removeFromParentAndCleanup(true);
	
	schedule(schedule_selector(ThisClassType::update));
	gameLayer->setIsStarted(true);
	gameLayer->getHgPlayer()->startScheduler();
	auto a1 = CCMoveBy::create(4, ccp(-480, 0));
	auto a2 = KSFuncN(node, {node->setPosition(ccp(0, 0));});
	auto a3 = CCRepeatForever::create(CCSequence::createWithTwoActions(a1, a2));
	gameLayer->getVoidNode()->runAction(a3);
	//this->schedule(schedule_selector(ThisClassType::gameTimer));
}
void HGGameScene::finishReady(CCNode* n)
{
	
	n->removeFromParentAndCleanup(true);
	auto mator = SceneUtil::playAnimation("go.png", 0.075, 2, 4, 256, 65, false);
	auto action0 = CCSpawn::create(CCSequence::createWithTwoActions(CCDelayTime::create(0.1f), CCScaleTo::create(0.4f, 4.f)), CCSequence::create(CCDelayTime::create(0.3f), CCFadeOut::create(0.2f), 0), mator.first, 0);
	auto action = CCSequence::create(action0, CCCallFuncN::create(this, callfuncN_selector(ThisClassType::finishGo)), 0);
	mator.second->runAction(action);
	addChild(mator.second, 1000);
	mator.second->setPosition(ccp(240, 160));
}

void HGGameScene::onExitZero()
{
	graphics.zeroSprite->runAction(CCSequence::create(CCProgressFromTo::create(0.4f, 100.f, 0.f),
													   0));
	graphics.watch2->resumeSchedulerAndActions();
	info.gameMode = HGGameInfo::NORMAL;
	KSoundEngine::sharedEngine()->playSound("hotshots.mp3");
	KSoundEngine::sharedEngine()->playSound("exitzero.mp3");
}
void HGGameScene::onEnterZero()
{
	graphics.zeroSprite->runAction(CCSequence::create(CCProgressFromTo::create(0.4f, 0, 100.f),
													   0));
	graphics.watch2->pauseSchedulerAndActions();
	info.gameMode = HGGameInfo::ZERO;
	KSoundEngine::sharedEngine()->playSound("RetroSpace.mp3");
	
	KSoundEngine::sharedEngine()->playSound("fever.mp3");
	
//	skChar->runAction(CCMoveBy::actionWithDuration(0.5f, ccp(0, 30)));
}
// Touch

bool HGGameScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	KS::KSLog("%", pTouch->getLocationInView().x);
	float clickX = pTouch->getLocationInView().x;
//	CCLog("Beg");
	gameLayer->setIsUp(true);
	
	return true;
}

void HGGameScene::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	gameLayer->setIsUp(false);
}
void  HGGameScene::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	
	CCPoint location = pTouch->getLocationInView();
	location=CCDirector::sharedDirector()->convertToGL(location);
	CCPoint location2 = convertToNodeSpace(location);

}
void  HGGameScene::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	gameLayer->setIsUp(false);
//	if(gameStep==GSHandlingBow)gameStep=GSShootArrow;
}

HGGameScene::~HGGameScene(){
	//delete glayer;
//	delete sResult;
	playInfo->gamePtr = 0;
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
	LuaManager::shared()->closeLua("hg.lua");
//	lua_close(lua);
}

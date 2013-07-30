//
//  GameScene.cpp
//  ArcherWorldCupX
//
//  Created by apple on 12. 7. 6..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "ACGameScene.h"
#include "SimpleAudioEngine.h"
#include "ACPlay.h"
#include "ACSaveData.h"
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
#include "WorldCupMiddle.h"
#include "WCPlay.h"
#include "KSoundEngine.h"
#include "SceneUtil.h"
#include "GDTranslate.h"
using namespace GDTranslate;
using namespace cocos2d;
using namespace CocosDenshion;

JUDGE ACGameScene::HITS = {2.5f, 5.f, 7.5f, 10.f, 12.5f, 15.f, 17.5f, 20.f, 22.5f, 25.f};
JUDGE ACGameScene::HITS100 = {2.5f, 5.f, 7.5f, 10.f, 12.5f, 15.f, 17.5f, 20.f, 22.5f, 25.f};
JUDGE ACGameScene::HITS80 = {2.0f, 4.f, 6.f, 8.f, 10.f, 12.f, 14.f, 16.f, 18.f, 20.f};
JUDGE ACGameScene::HITS60 = {1.5f, 3.f, 4.5f, 6.f, 7.5f, 9.f, 10.5f, 12.f, 13.5f, 15.f};
JUDGE ACGameScene::HITS40 = {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f};
void ACGameScene::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}

void ACGameScene::showTargetNumber(int s = 100)
{
	switch(s)
	{
		case 100:
			target100->setVisible(true);
			target80->setVisible(false);
			target60->setVisible(false);
			target40->setVisible(false);
			target = TARGET::_100;
			break;
		case 80:
			target100->setVisible(false);
			target80->setVisible(true);
			target60->setVisible(false);
			target40->setVisible(false);
			target = TARGET::_80;
			break;
		case 60:
			target100->setVisible(false);
			target80->setVisible(false);
			target60->setVisible(true);
			target40->setVisible(false);
			target = TARGET::_60;
			break;
		case 40:
			target100->setVisible(false);
			target80->setVisible(false);
			target60->setVisible(false);
			target40->setVisible(true);
			target = TARGET::_40;
			break;
		default:
			target100->setVisible(true);
			target80->setVisible(false);
			target60->setVisible(false);
			target40->setVisible(false);
			target = TARGET::_100;
			break;
	}
}
bool ACGameScene::init()
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
	setScale(1.0f);
	auto t1 = SceneUtil::playAnimation("game_combo.png", 0.1, COMBO_COLS, COMBO_FRAME, COMBO_WIDTH, COMBO_HEIGHT, false); // texture 로딩. 바로 해제될 녀석.
	auto t2 = CCLabelBMFont::create("00", "gamecombo.fnt"); // texture 로딩. 바로 해제될 녀석.
	t1.second->setPosition(ccp(-200, -200));
	t1.second->runAction(CCSequence::createWithTwoActions(t1.first, CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite))));
	addChild(t1.second);
	
	addChild(t2); removeChild(t2, true);
	
	this->setTouchEnabled(true);
	setAccelerometerEnabled(true);

	
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

	
	
	target100=CCSprite::create("aw_target100.png");
	target100->setAnchorPoint(ccp(0.5,0.5));
	this->addChild(target100,1);
	
	target80=CCSprite::create("aw_target80.png");
	target80->setAnchorPoint(ccp(0.5,0.5));
	this->addChild(target80,1);
	
	target60=CCSprite::create("aw_target60.png");
	target60->setAnchorPoint(ccp(0.5,0.5));
	this->addChild(target60,1);
	
	target40=CCSprite::create("aw_target40.png");
	target40->setAnchorPoint(ccp(0.5,0.5));
	this->addChild(target40,1);
	showTargetNumber(100);
	setTargetPosition(graphics.INIT_TARGET_POS);
	bowSet = CCLayer::create();
	bowSet->setAnchorPoint(ccp(0.5,0.5));
	bowSet->setContentSize(CCSizeMake(480,320));
	bowSet->setPosition(ccp(0, 100));
	this->addChild(bowSet,2);
	
	CCSize bowSetPoint = bowSet->getContentSize();
	    
    bowH = CCSprite::create("aw_hand1_1.png");
    bowH->setAnchorPoint(ccp(0.5,0));
    bowH->setPosition(ccp(bowSetPoint.width/2,bowSetPoint.height/2));
    bowSet->addChild(bowH,6);
    
    CCSprite *_bowR = CCSprite::create("aw_hand1_2.png");
    _bowR->setAnchorPoint(ccp(0.5,1));
    _bowR->setPosition(ccp(bowSetPoint.width/2,bowSetPoint.height/2));
    bowSet->addChild(_bowR,6);
    
	
	arrow = CCSprite::create("aw_arrow1_2.png");
	arrow->setAnchorPoint(ccp(0.5, 1));
	arrow->setPosition(ccp(bowSetPoint.width/2,bowSetPoint.height/2+10));
	bowSet->addChild(arrow,6);
	//75 95
	auto mator1 = SceneUtil::playAnimation("aw_pull.png", 0.15, 4, 4, 75, 95, true);
	pullImg = mator1.second;
	pullImg->runAction(mator1.first);
	pullImg->setAnchorPoint(ccp(0.5,0.5));
	pullImg->setPosition(ccp(240,100));
	this->addChild(pullImg,5);
	
	controller = CCSprite::create("aw_control.png");
	controller->setAnchorPoint(ccp(0.5,0.5));
	controller->setPosition(ccp(240,100));
	controller->setVisible(false);
	this->addChild(controller,5);
	
	arrowPoint=CCSprite::create("aw_arrowPoint.png");
	arrowPoint->setAnchorPoint(ccp(0.5,0.5));
	//arrowPoint->setPosition(ccpAdd(bow->getPosition(), bowSet->getPosition()));
    arrowPoint->setPosition(ccpAdd(bowH->getPosition(), bowSet->getPosition()));
    
	this->addChild(arrowPoint,6);
	arrowPoint->setVisible(false);
	arrowPoint->setTag(0);
	gameStep=GSPullingCheck;

	
//	sResult->initWithDelegate(this, menu_selector(ACGameScene::nextArrow), menu_selector(ACGameScene::touchedExit));
//	sResult->setContentSize(CCSizeMake(320, 480));
//	sResult->setAnchorPoint(ccp(0,0));
//	sResult->setPosition(ccp(0,0));
//	this->addChild(sResult,7);
//	sResult->setVisible(false);
	
	
	
//	sResult->setContentSize(CCSizeMake(320, 480));
	
	

//	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("SE_heart.wav");

    CCSprite* fever_back = CCSprite::create("ui_zerobar_back.png");
	fever_back->setRotation(-90);
	fever_back->setPosition(ccp(439, 156));
	addChild(fever_back, 5);
	
	feverGauge = CCProgressTimer::create(CCSprite::create("ui_zerobar_front.png"));
	feverGauge->setMidpoint(ccp(0,0));
	feverGauge->setBarChangeRate(ccp(1,0));
	feverGauge->setRotation(-90);
	feverGauge->setPosition(ccp(439, 156));
	feverGauge->setType(CCProgressTimerType::kCCProgressTimerTypeBar);
	addChild(feverGauge, 5);
	
	timeFnt = CCLabelBMFont::create(KS_Util::stringWithFormat("%.0f", info.remainTime * BASETIME / info.INIT_GAME_TIME ).c_str(), "time1.fnt");
	addChild(timeFnt, 3);
	timeFnt->setPosition(ccp(431,274));
	

	graphics.watch1 = CCSprite::create("ui_time1.png");
	graphics.watch1->setPosition(ccp(431, 277));
	addChild(graphics.watch1, 1);
	auto watchMator = SceneUtil::playAnimation("ui_time2.png", 0.05, 2, 2, 85, 75, true);
	watchMator.second->runAction(watchMator.first);
	graphics.watch2 = watchMator.second;
	graphics.watch2->setPosition(ccp(431, 277));
	addChild(graphics.watch2, 1);
	graphics.watch2->setVisible(false);
		
	graphics.windLayer = CCLayer::create();
	addChild(graphics.windLayer, 2);
	CCSprite* windGaugeBack = CCSprite::create("aw_windback.png");
	windGaugeBack->setPosition(ccp(240, 291));
	graphics.windLayer->addChild(windGaugeBack);
	
	graphics.leftWind = CCProgressTimer::create(CCSprite::create("aw_windleftbody.png"));
	graphics.leftWind->setBarChangeRate(ccp(1,0));
	graphics.leftWind->setMidpoint(ccp(1,0.5f));
	graphics.leftWind->setType(CCProgressTimerType::kCCProgressTimerTypeBar);
	graphics.leftWind->setPosition(graphics.LEFTWINDPOS);
	graphics.windLayer->addChild(graphics.leftWind, 1);
	graphics.rightWind = CCProgressTimer::create(CCSprite::create("aw_windrightbody.png"));
	graphics.rightWind->setBarChangeRate(ccp(1,0));
	graphics.rightWind->setMidpoint(ccp(0, 0.5));
	graphics.rightWind->setType(CCProgressTimerType::kCCProgressTimerTypeBar);
	graphics.rightWind->setPosition(graphics.RIGHTWINDPOS);
	graphics.windLayer->addChild(graphics.rightWind, 1);
	
	graphics.leftHead = CCSprite::create("aw_windlefthead.png");
	graphics.leftHead->setAnchorPoint(ccp(1, 0.5f));
	graphics.windLayer->addChild(graphics.leftHead, 2);
	graphics.leftHead->setVisible(false);
	
	graphics.rightHead = CCSprite::create("aw_windrighthead.png");
	graphics.rightHead->setAnchorPoint(ccp(0.f, 0.5f));
	graphics.windLayer->addChild(graphics.rightHead, 2);
	graphics.rightHead->setVisible(false);
	
	graphics.leftWind->setPercentage(0.f);
	graphics.rightWind->setPercentage(0.f);
	
	graphics.windValue = CCLabelBMFont::create(KS_Util::stringWithFormat("%d", info.wind).c_str(), "windcount.fnt");
	graphics.windValue->setPosition(graphics.WINDVALUEPOS);
	graphics.windLayer->addChild(graphics.windValue, 2);

	if(playInfo->selectedItems["item_wind1.png"] == GameShop::NONE)
			graphics.windValue->setVisible(false);
	graphics.zeroSprite = CCProgressTimer::create(CCSprite::create("back0.png"));
	graphics.zeroSprite->setBarChangeRate(ccp(1,0));
	graphics.zeroSprite->setMidpoint(ccp(0, 0.5f));
	graphics.zeroSprite->setScale(2.f);
	graphics.zeroSprite->setAnchorPoint(ccp(0, 0));
	graphics.zeroSprite->setPosition(ccp(0, 0));
	graphics.zeroSprite->setType(CCProgressTimerType::kCCProgressTimerTypeBar);
	graphics.zeroSprite->setPercentage(0);
	addChild(graphics.zeroSprite, 0);
	
	miniMap = ACMiniMap::create();
	miniMap->setPosition(ccp(330, 160));
//	miniMap->setPosition(ccp(100, 100));
	addChild(miniMap, 1);
	//setScale(0.5f);
	
	graphics.scoreFnt = CCLabelBMFont::create("0", "uiscore.fnt");
	graphics.scoreFnt->setAnchorPoint(ccp(0.0, 0.f));
	graphics.scoreFnt->setPosition(ccp(20, 9));
	graphics.scoreFnt->setScale(0.3f);
	addChild(graphics.scoreFnt, 2);
	
	KSoundEngine::sharedEngine()->playSound("readygo.mp3");
	auto mator = SceneUtil::playAnimation("ready.png", 0.1, 2, 10, 256, 65, false);
	auto action = CCSequence::create(mator.first, CCCallFuncN::create(this, callfuncN_selector(ThisClassType::finishReady)), 0);
	mator.second->runAction(action);
	addChild(mator.second, 1000);
	mator.second->setPosition(ccp(240, 160));
	

	{
		map<string, string> item2img;
		item2img["shield"] = "item_shield1_1.png"; item2img["exp"] = "item_exp1_1.png";
		item2img["item_wind1.png"] = "item_wind1_1.png"; item2img["item_vibration1.png"] = "item_vibration1_1.png";
		
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
	
	
	CCSprite* left = CCSprite::create("left.png");
	left->setAnchorPoint(ccp(1, 0.5f));
	left->setPosition(ccp(0, 160));
	addChild(left, INT_MAX);
	
	CCSprite* right = CCSprite::create("right.png");
	right->setAnchorPoint(ccp(0.f, 0.5f));
	right->setPosition(ccp(480, 160));
	addChild(right, INT_MAX);
	
	CCSprite* ui_gold = CCSprite::create("ui_gold.png");
	ui_gold->setPosition(ccp(435, 15));
	addChild(ui_gold, 10);
	
	graphics.ateGoldFnt = CCLabelBMFont::create("0", "main1.fnt");
	graphics.ateGoldFnt->setPosition(ccp(443, 15));
	addChild(graphics.ateGoldFnt, 11);
	
	CCMenu* _menu = CCMenu::create(0);
	addChild(_menu, 3);
	_menu->setPosition(CCPointZero);
	
	graphics.pause = CCMenuItemImage::create("ui_stop.png", "ui_stop.png",
															 this, menu_selector(ThisClassType::PAUSE));
	
	_menu->addChild(graphics.pause, 2);
	graphics.pause->setPosition(ccp(24 + 10, 295 - 10));
	

	return true;
}

void ACGameScene::PAUSE(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	ACPauseLayer* pauseLayer = ACPauseLayer::create();
	//pauseLayer->setPosition(ccp(0, -20));
	addChild(pauseLayer, 1000);
	
	KS::KSLog("pause");
}

void ACGameScene::finishGetWeek(JsonBox::Object js)
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
				ReplaceSceneParam(WorldCupMiddle, acplay->gameScore.getVar());
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
void ACGameScene::RETRY()
{
	graphdog->command("getweek", 0, this, gd_selector(ThisClassType::finishGetWeek));
}
void ACGameScene::finishGo(CCNode* n)
{
	n->removeFromParentAndCleanup(true);
	this->schedule(schedule_selector(ThisClassType::gameTimer));
}
void ACGameScene::finishReady(CCNode* n)
{
	n->removeFromParentAndCleanup(true);
	auto mator = SceneUtil::playAnimation("go.png", 0.075, 2, 4, 256, 65, false);
	auto action0 = CCSpawn::create(CCSequence::createWithTwoActions(CCDelayTime::create(0.1f), CCScaleTo::create(0.4f, 4.f)), CCSequence::createWithTwoActions(CCDelayTime::create(0.3f), CCFadeOut::create(0.2f)), mator.first, 0);
	auto action = CCSequence::create(action0, CCCallFuncN::create(this, callfuncN_selector(ThisClassType::finishGo)), 0);
	mator.second->runAction(action);
	addChild(mator.second, 1000);
	mator.second->setPosition(ccp(240, 160));
}


void ACGameScene::setHeadSprite()
{
	if(graphics.leftWind->getPercentage() > 0)
	{
		KS::KSLog(",, %", 240 - graphics.leftWind->getPercentage() * 0.01f * graphics.leftWind->getContentSize().width);
		graphics.leftHead->setPosition(ccp(240 - graphics.leftWind->getPercentage() * 0.01f * graphics.leftWind->getContentSize().width,
										   284));
		graphics.leftHead->setVisible(true);
	}
	else
		graphics.leftHead->setVisible(false);
	if(graphics.rightWind->getPercentage() > 0)
	{
		KS::KSLog(",,, %", 240 + graphics.rightWind->getPercentage() * 0.01f * graphics.rightWind->getContentSize().width);
		graphics.rightHead->setPosition(ccp(240 + graphics.rightWind->getPercentage() * 0.01f * graphics.rightWind->getContentSize().width,
											284));
		graphics.rightHead->setVisible(true);
	}
	else
		graphics.rightHead->setVisible(false);
	
}

void ACGameScene::pullBow(){
	if(gameStep==GSPullingCheck){
		
		setHeadSprite();
		oax = currentAcX;
		oay = currentAcY;
		oaz = currentAcZ;
		CCSize bowSetPoint = bowSet->getContentSize();
		CCMoveTo *action = CCMoveTo::create(0.2, ccp(bowSetPoint.width/2,bowSetPoint.height/2));

		arrow->runAction(action);
		KSoundEngine::sharedEngine()->playSound("SE_pull.wav");
	}
}

void ACGameScene::checkPulling(CCPoint pTouch){
	if(pTouch.y<pullImg->getPosition().y){
		gameStep=GSHandlingBow;
		controller->setVisible(true);
		pullImg->setVisible(false);
		pullingStart = true;
		if(info.gameMode == ACGameInfo::ZERO)
		{
			if(feverGauge->getPercentage() <= 0.f)
			{
				onExitZeroArea(false);
			}
		}
		if(info.gameMode == ACGameInfo::ZERO)
		{
			KS::KSLog("shotc %", info.shotCounterAfterZero);
			
			int w = rand()%5;
			if(info.shotCounterAfterZero >=5)
			{
				w = MIN(w + (int)(rand()%(info.shotCounterAfterZero - 4) * 2.f), info.MAX_WIND);
			}
			
			info.wind = w * (rand()%2 ? -1 : 1);
			graphics.windValue->setString(KS_Util::stringWithFormat("%d", abs(info.wind)).c_str());
			switch(info.shotCounterAfterZero)
			{
				case 1:
				case 2:
				case 3:
					showTargetNumber(80);
					break;
				case 4:
				case 5:
					showTargetNumber(60);
					break;
				case 6:
					showTargetNumber(40);
					break;
			}
		}
		else
		{
			showTargetNumber(100);
			int w = MIN(feverGauge->getPercentage() / 25 + rand()%5, info.MAX_WIND);
			info.wind = w * (rand()%2 ? -1 : 1);
			graphics.windValue->setString(KS_Util::stringWithFormat("%d", abs(info.wind)).c_str());
		}
		
		if(info.wind >= 0)
		{
			graphics.leftWind->setPercentage(0);
			graphics.rightWind->setPercentage(fabsf(info.wind) * (100.f / (float)info.MAX_WIND));
		}
		else
		{
			graphics.rightWind->setPercentage(0);
			graphics.leftWind->setPercentage(fabsf(info.wind) * (100.f / (float)info.MAX_WIND));
		}
	}
}

void ACGameScene::clacHandling(CCPoint pTouch){
	CCPoint controllerPoint = controller->getPosition();
	
	float ctrl = 25.f;
	userCont=ccp((pTouch.x-controllerPoint.x)/ctrl,(pTouch.y-controllerPoint.y)/ctrl);
	
	int ctrlMax=2;
	
	if(userCont.x>ctrlMax)userCont.x=ctrlMax;
	if(userCont.y>ctrlMax)userCont.y=ctrlMax;
	if(userCont.x<-ctrlMax)userCont.x=-ctrlMax;
	if(userCont.y<-ctrlMax)userCont.y=-ctrlMax;
}


void ACGameScene::didAccelerate(CCAcceleration* pAccelerationValue)
{
	float X = pAccelerationValue->x;
	float Y = pAccelerationValue->y;
	float Z = pAccelerationValue->z;
	currentAcX = X;
	currentAcY = Y;// + 0.95;
	currentAcZ = Z;// + 0.35;
	//CCLog("%.2f %.2f %.2f", X, Y, Z);
}

void ACGameScene::applyHandling(){ // timer
	float heatX = 0;
	float heatY = 0;

	// 0.1 초에 한번씩 심장 뛰기
	if(timeCnt%6==0){
//		heatX=(2-rand()%5)/2.f;
		heatY=(2-rand()%5)/2.f;
	}

	if(playInfo->selectedItems["item_vibration1.png"])
		heatY *= 0.9f;
	// 1초에 한번씩 바람 변화
	if(timeCnt==0){
		int _w = (rand()%2 ? -1 : +1) * rand()%2;
		info.wind += _w;
		if(info.wind > info.MAX_WIND)
			info.wind = info.MAX_WIND;
		if(info.wind < -info.MAX_WIND)
			info.wind = -info.MAX_WIND;
		graphics.windValue->setString(KS_Util::stringWithFormat("%d", abs(info.wind)).c_str());
		if(info.wind >= 0)
		{
			graphics.leftWind->setPercentage(0);
			graphics.rightWind->setPercentage(fabsf(info.wind) * (100.f / (float)info.MAX_WIND));
		}
		else
		{
			graphics.rightWind->setPercentage(0);
			graphics.leftWind->setPercentage(fabsf(info.wind) * (100.f / (float)info.MAX_WIND));
		}
		CCLog("wind = %d", info.wind);
		setHeadSprite();
	}
	
	// 0.1 초에 한번씩 심장 소리.
	if(timeCnt%6==0){
		
		// 잡았을 때 점점 심장 소리가 줄어듬.
		// 변수 이름은 left'Sec' 이지만 실제 단위는 1/10초.
		
	}
	
	// 컨트롤 적용. 너무 부드러우니까 보정.
	if(timeCnt%3==0){
		CCPoint nowPoint=bowSet->getPosition();
		CCPoint newPoint=ccp(nowPoint.x+userCont.x+heatX,nowPoint.y+userCont.y+heatY);
		bowSet->setPosition(newPoint);
	}
#if 0
	float X = (currentAcX - oax) / 0.1f;
	float Y = (currentAcZ - oaz) / -0.2f;
	//CCLog("Y = %f", Y);
	if(timeCnt%3==0){
		CCPoint nowPoint=bowSet->getPosition();
		CCPoint newPoint=ccp(nowPoint.x+X + heatX,nowPoint.y+Y + heatY) ;
		bowSet->setPosition(newPoint);
	}
#endif
	
	

}

void ACGameScene::hidePoint()
{
	int point = pointFromTargetArrow(getTargetPosition(), arrowPoint->getPosition());
	if(point <= 0)
	{
		arrowPoint->setVisible(false);
	}
}

void ACGameScene::shootArrow(){
	
	gameStep=GSMoveArrow;
	info.shotCounter++;
	if(info.gameMode == ACGameInfo::ZERO)
		info.shotCounterAfterZero++;
	CCPoint bowPoint = ccpAdd(bowH->getPosition(), bowSet->getPosition());
	arrowPoint->setPosition(bowPoint);
	arrowPoint->setScale(1);
	arrowPoint->setVisible(true);
	controller->setVisible(false);
	arrow->setVisible(false);
	
//	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	
    
    
	//glayer->wind *= 2;
    {
		CCMoveBy *action1_1 = CCMoveBy::create(0.35, ccp(info.wind,20));
		CCMoveBy *action1_2 = CCMoveBy::create(0.25, ccp(info.wind,-20));
		CCCallFunc *action1_3 = CCCallFunc::create(this, callfunc_selector(ACGameScene::hidePoint));
		CCDelayTime *action1_4 = CCDelayTime::create(0.5);
		CCCallFunc *action1_5 = CCCallFunc::create(this, callfunc_selector(ACGameScene::showResult));
		CCFiniteTimeAction *action1=CCSequence::create(action1_1,action1_2,action1_3, action1_4,action1_5, NULL);
		float pointScale = 0.5f;;
		switch (target) {
			case TARGET::_100:
				pointScale = 0.5f;
				break;
			case TARGET::_80:
				pointScale = 0.4f;
				break;
			case TARGET::_60:
				pointScale = 0.35f;
				break;
			case TARGET::_40:
				pointScale = 0.3f;
				break;
			default:
				break;
		}

			
		CCScaleBy *action2 = CCScaleBy::create(0.6, pointScale);
		CCSpawn *action = CCSpawn::create(action1, action2, 0);
		CCEaseInOut * actions = CCEaseInOut::create(action, 0.6);
		arrowPoint->runAction(action);
	}	
	CCMoveBy *action3_1 = CCMoveBy::create(0.1, ccp(0,-180));
	CCRotateTo *action3_2 = CCRotateTo::create(0.1, 70);
	CCSpawn *action4 = CCSpawn::create(action3_1,action3_2, 0);
	bowSet->runAction(action4);
	KSoundEngine::sharedEngine()->playSound("SE_arrow.wav");
}


int ACGameScene::gaugeFromPoint(int point)
{
	if(point >= 7)
	{
		if(info.gameMode == ACGameInfo::ZERO)
			return (point - 7) * 10;
		else
			return (point - 7) * 8;
	}
	else
	{
		if(info.gameMode == ACGameInfo::ZERO)
			return -25;
		else
			return 0;
	}
}

int ACGameScene::comboScore(int point)
{
	int m = (info.gameMode == ACGameInfo::ZERO ? 2 : 1) * point * 100;
	//info.shotCounter
	return ((acplay->combo.getVar()+1) * 2) * m * 2.f;
	//return ((acplay->combo.getVar() * 100 + info.shotCounter * 50) * (info.gameMode == ACGameInfo::ZERO ? 2 : 1) * 2)*2;
}
void ACGameScene::showResult(){
	info.wind = 0;
	gameStep=GSShowResult;
	
	int point = pointFromTargetArrow(getTargetPosition(), arrowPoint->getPosition());

	//CCLog("target %f %f, arrow %f %f", target->getPosition().x, target->getPosition().y, arrowPoint->getPosition().x, arrowPoint->getPosition().y);
	float mult = ACMiniMap::RADIUS/getHITS()[9];
	KS::KSLog("mult = %", mult);
	miniMap->pushHistroy(ccpMult(ccpSub(arrowPoint->getPosition(), getTargetPosition()), mult), point);
//	miniMap->setScale(0.7f);
//	miniMap->runAction(CCSequence::createWithTwoActions(CCEaseBackOut::create(CCScaleTo::create(0.5, 1.5f)),
//												CCScaleTo::create(0.3f, 1.0f)));
	
	if(info.gameMode == ACGameInfo::ZERO)
	{

		if(point == 8)
			eatCoin(4);
		else if(point == 9)
			eatCoin(7);
		else if(point == 10)
			eatCoin(10);
	}
	KS::KSLog("point = %", point);
	if(point >= COMBO_GT_PT)
	{
		acplay->combo++;
		bool bShout = false;
		if(info.gameMode == ACGameInfo::ZERO)
		{
			if(acplay->combo.getVar() % 5 == 0)
			{
				KSoundEngine::sharedEngine()->playSound("shout.mp3");
				bShout = true;
			}
		}
		
		if(bShout == false)
		{
			if(point == 8)
			{
				if(rand()%2)
				{
					KSoundEngine::sharedEngine()->playSound(KS_Util::stringWithFormat("p%d.mp3", 3).c_str());
				}
				else
				{
					KSoundEngine::sharedEngine()->playSound(KS_Util::stringWithFormat("p%d.mp3", 2).c_str());
				}
			}
			else if(point == 9)
			{
				if(rand()%2)
				{
					KSoundEngine::sharedEngine()->playSound(KS_Util::stringWithFormat("p%d.mp3", 4).c_str());
				}
				else
				{
					KSoundEngine::sharedEngine()->playSound(KS_Util::stringWithFormat("p%d.mp3", 5).c_str());
				}
			}
			else if(point == 10)
			{
				if(rand()%2)
				{
					KSoundEngine::sharedEngine()->playSound(KS_Util::stringWithFormat("p%d.mp3", 6).c_str());
				}
				else
				{
					KSoundEngine::sharedEngine()->playSound(KS_Util::stringWithFormat("p%d.mp3", 7).c_str());
				}
			}
		}
		
		
//		KSoundEngine::sharedEngine()->playSound("combo.wav");
		auto mator = SceneUtil::playAnimation("game_combo.png", 0.1, COMBO_COLS, COMBO_FRAME, COMBO_WIDTH, COMBO_HEIGHT, false);
		mator.second->runAction(CCSequence::create(mator.first,
													//CCDelayTime::create(0.5f),
													CCCallFuncN::create(this, callfuncN_selector(ACGameScene::deleteSprite)), 0));
		mator.second->setPosition(ccp(85, 100));
		addChild(mator.second, 2);
		
		CCLabelBMFont* comboFnt = CCLabelBMFont::create(KS_Util::stringWithFormat("%d", acplay->combo.getVar()).c_str(), "gamecombo.fnt");
		comboFnt->setPosition(ccp(85, 120));
		comboFnt->setScale(0.6f);
		comboFnt->runAction(CCSequence::create(CCEaseBackOut::create(CCScaleTo::create(0.3, 1.f)),
												CCDelayTime::create(0.5f), CCFadeOut::create(0.5f),
												CCCallFuncN::create(this, callfuncN_selector(ACGameScene::deleteSprite)), 0));
		addChild(comboFnt, 2);
		
		
		
	}
	else
	{
		bool shieldMode = false;

		{
			if(playInfo->selectedItems["shield"] == true && info.usedShield == false)
			{
				info.usedShield = true;
				shieldMode = true;
				CCSprite* shield;
				KSoundEngine::sharedEngine()->playSound("shield.mp3");
				auto shieldMator = SceneUtil::playAnimation("shield.png", 0.07, 4, 8, 160, 160, 2);
				shield = shieldMator.second;
				shield->runAction(CCSequence::createWithTwoActions(shieldMator.first, CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite))));
				shield->setPosition(ccp(240, 200));
				addChild(shield, 10000);
				//			shield->runAction(CCSequence::create(CCBlink::create(0.8, 3.f), CCDelayTime::create(2.f), CCFadeOut::create(0.5f),
				//												  CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite)), 0));
			}
		}
		if(shieldMode == false)
		{
			acplay->combo = 0;
			KSoundEngine::sharedEngine()->playSound("uhuh.mp3");
		}
	}
	acplay->gameScore += this->comboScore(point);
//	wcplay->gameScore = acplay->gameScore.getVar();
//	wcplay->scoreEachGame["AC"] = wcplay->gameScore.getVar();
	CCLabelBMFont* bonusFnt = CCLabelBMFont::create(KS_Util::stringWithFormat("+%d", comboScore(point)).c_str(), "bonusscore.fnt");
	bonusFnt->setPosition(ccp(graphics.scoreFnt->getPosition().x + graphics.scoreFnt->getContentSize().width / 3.f,
							  graphics.scoreFnt->getPosition().y + graphics.scoreFnt->getContentSize().height / 3.f));
	addChild(bonusFnt, 2);
	float virtualMaxScore = MAX(100000, playInfo->acscore);
	float goalScale = MIN(1.3f, 0.6f + (float)acplay->gameScore.getVar() * 2.f / (float)virtualMaxScore);
	bonusFnt->setScale(0.3f);
	auto action0 = CCEaseBackOut::create(CCScaleTo::create(1.f, goalScale));
	auto action1 = CCDelayTime::create(0.2f);
	auto action2 = CCFadeOut::create(0.2f);
	auto action3 = CCCallFuncN::create(this, callfuncN_selector(ACGameScene::deleteSprite));
	auto totalAction = CCSequence::create(action0, action1, action2, action3, 0);
	bonusFnt->runAction(totalAction);
	
	
	graphics.scoreFnt->runAction(CCEaseBackOut::create(CCScaleTo::create(1.f, goalScale)));
	
	//graphics.scoreFnt->runAction(CCEaseBackOut::create(CCScaleTo::create(1.f, 1.0f + ACSaveData::getMaxScore() / acplay->gameScore.getVar())));
	
	
	//graphics.scoreFnt->setString(KS_Util::stringWithFormat("%d", acplay->gameScore.getVar()).c_str());
	feverGauge->setPercentage(feverGauge->getPercentage() + gaugeFromPoint(point));
	
	if(info.gameMode != ACGameInfo::ZERO && feverGauge->getPercentage() >= 100.f && isGameOvering == false)
	{
		onEnterZero();
	}

	nextArrow();
}
void ACGameScene::eatCoin(int n)
{
	NSDefault::setGold(NSDefault::getGold() + n);
	info.ateGoldCount+=n;
	playInfo->__ateCoin = info.ateGoldCount;
	graphics.ateGoldFnt->setString(KS_Util::stringWithFormat("%d", info.ateGoldCount).c_str());
	for(int i=0; i<n; i++)
	{
		auto retainAnimation = SceneUtil::playAnimation("coin.png", 0.07f, 6, 6, ACGraphics::COIN_WIDTH, ACGraphics::COIN_HEIGHT, false);
		addChild(retainAnimation.second, 0);
		retainAnimation.second->setPosition(ACGraphics::INIT_TARGET_POS);
		retainAnimation.second->setScale(0.3f);
		ccBezierConfig bc;
		bc.controlPoint_1 = ACGraphics::INIT_TARGET_POS;//retainAnimation.second->getPosition();
		bc.controlPoint_2 = ccp(450 + rand()%50, 500);
		bc.endPosition = ccp(460, 20);
		auto moveAction = CCSequence::create(CCDelayTime::create(i * 0.2f), KSFunc(KSoundEngine::sharedEngine()->playSound("se_takegold01.mp3");),
								CCSpawn::create(CCBezierTo::create(1.2f, bc), CCScaleTo::create(1.2f, 1.f), 0), 0 );
		auto action2 = CCSpawn::create(retainAnimation.first, moveAction/*, CCFadeOut::create(1.2f)*/, 0); // 가면서 사라짐.
		auto action3 = CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite));
		auto totalAction = CCSequence::create(action2, action3, 0);
		retainAnimation.second->runAction(totalAction);
	}	
}
void ACGameScene::nextArrow(){
	gameStep=GSPullingCheck;
	
	
//	sResult->setVisible(false);
	
	controller->setVisible(false);
	pullImg->setVisible(true);
	arrow->setVisible(true);
	CCSize bowSetPoint = bowSet->getContentSize();
	arrow->setPosition(ccp(bowSetPoint.width/2,bowSetPoint.height/2+10));
	arrowPoint->setVisible(false);
	
	setTargetPosition(graphics.INIT_TARGET_POS);
	
	CCRotateTo *action1=CCRotateTo::create(0.2, 0);
	float bowPosition = (rand()%50) * (rand()%2 ? -1 : 1);
	CCMoveTo *action2 = CCMoveTo::create(0.2, ccp(bowPosition,100));
	CCFiniteTimeAction *action = CCSpawn::create(action1,action2,NULL);
	bowSet->runAction(action);
}
void ACGameScene::setTargetPosition(CCPoint p)
{
	target100->setPosition(p);
	target80->setPosition(p);
	target60->setPosition(p);
	target40->setPosition(p);
}
CCPoint ACGameScene::getTargetPosition()
{
	return target100->getPosition();
}

void ACGameScene::touchedExit(){
	
}
//util
int ACGameScene::calcPoint(CCPoint _target,CCPoint _arrow,float offset){
	float _x=-(getTargetPosition().x-arrowPoint->getPosition().x)*offset;
    float _y=-(getTargetPosition().y-arrowPoint->getPosition().y)*offset;
    int point = 10-(int)sqrtf(_y*_y+_x*_x)/5;
	if(point<0)point=0;
	
	return point;
}
string ACGameScene::stringWithFormat(const char* _Format, ...)
{
	va_list argptr;
	va_start(argptr, _Format);
	char buf[256];
	vsprintf(buf, _Format, argptr);
	string str = buf;
	
	va_end(argptr);
	return str;
}

// Timer

void ACGameScene::gameTimer(float dt){
	dt = 1.f/60.f;
	controlProc(dt);
	timeCheck(dt);
	updateScoreFnt(dt);
}
void ACGameScene::controlProc(float dt)
{
	timeCnt++;
	if(timeCnt>=60 || pullingStart)
	{
		timeCnt=0;
		pullingStart = false;
	}
	if(gameStep==GSHandlingBow)
		this->applyHandling();
	else if(gameStep==GSShootArrow)
		this->shootArrow();
}
void ACGameScene::timeCheck(float dt)
{
	
	if(info.gameMode == ACGameInfo::ZERO)
	{
		feverGauge->setPercentage(feverGauge->getPercentage() + ACGameInfo::ZERO_DEC_VALUE);
		if(feverGauge->getPercentage() <= 0)
		{
			onExitZeroArea(true);
		}
	}
	else
	{
		info.remainTime -= dt;
		if(info.remainTime <= 10 && !info.in10s)
		{
			info.in10s = true;
			graphics.watch1->setVisible(false);
			graphics.watch2->setVisible(true);
			KSoundEngine::sharedEngine()->playSound("hurryup.mp3");
		}
		if(info.remainTime >= 0)
			timeFnt->setString(KS_Util::stringWithFormat("%.0f", info.remainTime * BASETIME / (float)info.INIT_GAME_TIME).c_str());
		else
		{

			{
#if 0
				if(playInfo->selectedItems["item_random"] && !graphics.watch3->isVisible())
				{
					graphics.watch2->setVisible(false);
					graphics.watch3->setVisible(true);
					timeFnt->setVisible(false);
					KSoundEngine::sharedEngine()->playSound("bonustime.mp3");
					auto mator = SceneUtil::playAnimation("bonustime.png", 0.08, 6, 6, 135, 135, 1);
					mator.second->runAction(CCSequence::create(mator.first, CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite)),0));
					mator.second->setPosition(ccp(431, 277));
					addChild(mator.second, 10);
				}
#endif
			}
			
			CCLog("bt : %f", -info.bonusTime);
			if(info.remainTime <= -info.bonusTime && isGameOvering == false)
			{
				if(gameStep==GSHandlingBow)
					gameStep=GSShootArrow;
				setTouchEnabled(false);
				isGameOvering = true;
				graphics.pause->setEnabled(false);
				auto mator = SceneUtil::playAnimation("timeover.png", 0.1, 2, 4, TIMEOVERWIDTH, TIMEOVERHEIGHT, 8);
				KSoundEngine::sharedEngine()->playSound("timeover.mp3");
				mator.second->setPosition(ccp(240, 160));
				mator.second->setScale(1.2f);
				auto action = CCSequence::create(mator.first, CCCallFunc::create(this, callfunc_selector(ThisClassType::finishTimeOver)), 0);
				mator.second->runAction(action);
				addChild(mator.second, INT_MAX);
			}
		}
	}
}
void ACGameScene::onExitZeroArea(bool isCatchBow)
{
	KSoundEngine::sharedEngine()->playSound("hotshots.mp3");
	KSoundEngine::sharedEngine()->playSound("exitzero.mp3");
	info.gameMode = ACGameInfo::NORMAL;
	info.shotCounterAfterZero = 0;
	
	graphics.zeroSprite->runAction(CCSequence::create(CCProgressFromTo::create(0.4f, 100.f, 0.f),
													   
													   0));
	graphics.watch2->resumeSchedulerAndActions();
	if(!isCatchBow)
		showTargetNumber(100);
}
void ACGameScene::onEnterZero()
{
	KSoundEngine::sharedEngine()->playSound("RetroSpace.mp3");
	info.gameMode = ACGameInfo::ZERO;
	info.shotCounterAfterZero = 0;
	showTargetNumber(100);
	graphics.zeroSprite->runAction(CCSequence::create(CCProgressFromTo::create(0.4f, 0.f, 100.f),
													   0));
	graphics.watch2->pauseSchedulerAndActions();
	KSoundEngine::sharedEngine()->playSound("fever.mp3");
}
// Touch

bool ACGameScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	KS::KSLog("%", pTouch->getLocationInView().x);
	this->pullBow();
	return true;
}

void ACGameScene::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	
}
void  ACGameScene::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	
	CCPoint location = pTouch->getLocationInView();
	location=CCDirector::sharedDirector()->convertToGL(location);
	CCPoint location2 = convertToNodeSpace(location);
	if(gameStep==GSPullingCheck)			this->checkPulling(location2);
	else if(gameStep==GSHandlingBow)		this->clacHandling(location2);
}
void  ACGameScene::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	if(gameStep==GSHandlingBow)gameStep=GSShootArrow;
}            

ACGameScene::~ACGameScene(){
	//delete glayer;
//	delete sResult;
	playInfo->gamePtr = 0;
}

int ACGameScene::pointFromTargetArrow(CCPoint target, CCPoint arrow)
{
	float distance = sqrtf( (target.x - arrow.x)*(target.x - arrow.x) + (target.y - arrow.y)*(target.y - arrow.y) );
	JUDGE& HITS = getHITS();
	const int targetTableSize = sizeof(HITS) / sizeof(float);
	int res = targetTableSize;
	for(int i=0; i<targetTableSize; i++)
	{
		if(distance < HITS[i])
		{
			res = i;
			break;
		}
	}
	int scoreTable[targetTableSize] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	if(targetTableSize == res)
		return 0;
	else
		return scoreTable[res];
}

void ACGameScene::updateScoreFnt(float dt)
{
	
	int goalScore = acplay->gameScore.getVar();
	int displayedScore = atoi(graphics.scoreFnt->getString());
	if(displayedScore < goalScore )
	{
		int adderValue = (goalScore - displayedScore) / 30 + 21;
		adderValue = MIN(adderValue, goalScore - displayedScore);
		graphics.scoreFnt->setString(KS_Util::stringWithFormat("%d", displayedScore + adderValue).c_str());
	}
}
//void GameScene::menuCloseCallback(CCObject* pSender)
//{
//    CCDirector::sharedDirector()->end();
//	
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    exit(0);
//#endif
//}

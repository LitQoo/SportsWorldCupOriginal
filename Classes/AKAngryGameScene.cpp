//
//  AngryGameScene.cpp
//  AngryKeeper
//
//  Created by ksoo k on 12. 9. 10..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "AKAngryGameScene.h"
#include "LoadingScene.h"

#include "KS_Util.h"
#include "KSoundEngine.h"
#include <algorithm>
#include "Bridge.h"
#include "NSDefault.h"
#include "KSModalAlert.h"
#include "AKTextureFactory.h"
#include "AKPlay.h"
#include "AKSaveData.h"
#include "Bridge.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "AudioToolBox/AudioServices.h"
#endif
#include "KSBaseScene.h"
#include "SWIntro.h"
#include "GameSystem.h"
#include "PauseLayer.h"
#include "WCPlay.h"
#include "KSoundEngine.h"

#include "SceneUtil.h"
const CCPoint AngryGameScene::GUNPOSITION = ccp(240, 50);
const float AngryGameScene::DISABLE_TIME = 1.5f; // 1.5f
const float AngryGameScene::COMBO_RESET_TIME = 5.f;
const float AngryGameScene::INIT_SCALE = 1.0f; //0.6
const float AngryGameScene::MAX_SCALE = 1.0f; //0.85
const float AngryGameScene::MIN_SCALE = 1.0f; // 0.5
//void AngryGameScene::decZeroGauge(bool bird_arrive_goal)
//{
//	if(info.gameMode == AKGameInfo::ZERO)
//	{
//		if(bird_arrive_goal)
//			graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + AKGameInfo::CRASH_DEC_VALUE_WHEN * 2);
//		else
//			graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + AKGameInfo::CRASH_DEC_VALUE_IN_ZERO);
//	}
//	else
//	{
//		if(bird_arrive_goal)
//			graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + AKGameInfo::CRASH_DEC_VALUE_IN_NORMAL * 2);
//		else
//			graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + AKGameInfo::CRASH_DEC_VALUE_IN_NORMAL);
//	}
//	
//	if(info.gameMode == AKGameInfo::ZERO && graphics.feverGauge->getPercentage() <= 0.f)
//	{
//		onExitZeroMode();
//	}
//}

AngryGameScene::AngryGameScene()
{
	playInfo->gamePtr = this;
	
}
AngryGameScene::~AngryGameScene()
{
	playInfo->gamePtr = 0;
}

void AngryGameScene::onEnterZeroMode()
{
	KSoundEngine::sharedEngine()->playSound("RetroSpace.mp3");
	graphics.zeroSprite->runAction(CCSequence::create(CCProgressFromTo::create(0.4f, 0, 100.f),
											   0));
	info.gameMode = AKGameInfo::ZERO;
//	info.comboInZeroMode = 0;
	graphics.watch2->pauseSchedulerAndActions();
	info.lastLeftShotTime = info.gameTimer; // 왼쪽 안싸서 패널티 입는거 방지. (제로모드 부터 패널티 적용됨)
	//setScale(0.8f);
	KSoundEngine::sharedEngine()->playSound("fever.mp3");
}


void AngryGameScene::onExitZeroMode()
{
	KSoundEngine::sharedEngine()->playSound("hotshots.mp3");
	KSoundEngine::sharedEngine()->playSound("exitzero.mp3");
	graphics.zeroSprite->runAction(CCSequence::create(CCProgressFromTo::create(0.4f, 100.f, 0.f),
											  0));
	info.gameMode = AKGameInfo::NORMAL;
	info.zeroStep = 1;
	info.beforeRunStep = 1;
	info.timeAfterZero = 0.f;
	graphics.leftAim->setScale(INIT_SCALE * info.bonusScale);
	graphics.rightAim->setScale(INIT_SCALE * info.bonusScale);
	info.minTerm = AKGameInfo::INIT_MINTERM;
	info.maxTerm = AKGameInfo::INIT_MAXTERM;
	graphics.watch2->resumeSchedulerAndActions();
	//setScale(1.f);
}


void AngryGameScene::shakeScreenOnFire()
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
	CCMoveTo* moveBack = CCMoveTo::create(0.05f, CCPointMake(0,0));
	
	auto shakeWorldSeq = CCSequence::create(move1,move2,move3,move4,move5,move6,move7,move8,move9,move10,moveBack,nil);
	runAction(shakeWorldSeq);
	
}



void AngryGameScene::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent)
{
	KS::KSLog("began");
	for(auto iter = pTouches->begin(); iter != pTouches->end(); ++iter)
	{
		CCTouch* touch = (CCTouch*)*iter;
		
		/*if(touch->view()->isKindOfClass(EAGLView::class()) == false) // #
		 continue;*/
		CCPoint _t = touch->getLocationInView();
		
		CCPoint location(ccp(0, 0));
		location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
		location = convertToNodeSpace(location);
		KS::KSLog("% %", location.x, location.y);
		
		if(info.isDisable == false)
		{
			KSoundEngine::sharedEngine()->playSound("bbang.mp3");
			if(location.x <= 240)
			{
				if(!onLeftFire())
				{
					onFailShot(false);
				}
			}
			else
			{
				if(!onRightFire())
				{
					onFailShot(true);
				}
			}
		}
	}
}

void AngryGameScene::eatCoin(int n, CCPoint from)
{
	NSDefault::setGold(NSDefault::getGold() + n);
	info.ateGoldCount+=n;
	playInfo->__ateCoin = info.ateGoldCount;
	graphics.ateGoldFnt->setString(KS_Util::stringWithFormat("%d", info.ateGoldCount).c_str());
	for(int i=0; i<n; i++)
	{
		auto retainAnimation = SceneUtil::playAnimation("coin.png", 0.07f, 6, 6, AKGraphics::COIN_WIDTH, AKGraphics::COIN_HEIGHT, 3);
		addChild(retainAnimation.second, 2);
//		ccBezierConfig bc;
//		bc.controlPoint_1 = retainAnimation.second->getPosition();
//		bc.controlPoint_2 = ccp(400, 70);
//		bc.endPosition = ccp(240, 40);
		auto moveAction = CCSequence::create(CCDelayTime::create(i * 0.2f), KSFunc(KSoundEngine::sharedEngine()->playSound("se_takegold01.mp3");),
												   CCEaseOut::create(CCMoveTo::create(0.5f, ccp(240, 40)), 0.8f), 0);
		auto action2 = CCSpawn::create(retainAnimation.first, moveAction, CCScaleTo::create(0.8f, 0.5f)/*, CCFadeOut::create(1.2f)*/, 0); // 가면서 사라짐.
		auto action3 = CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite));
		auto totalAction = CCSequence::create(action2, action3, 0);
		retainAnimation.second->setPosition(from);
		retainAnimation.second->runAction(totalAction);
	}
}
void AngryGameScene::onShot(CCSprite* dish)
{
	akplay->combo++;
	
	bool bShout = false;
	if(info.gameMode == AKGameInfo::ZERO)
	{
		if(akplay->combo.getVar() % 10 == 0)
		{
			KSoundEngine::sharedEngine()->playSound("shout.mp3");
			bShout = true;
		}
	}
	
	if(bShout == false)
	{
		if(akplay->combo.getVar() % 5 == 0)
		{
			int r = rand() % 8;
			KSoundEngine::sharedEngine()->playSound(KS_Util::stringWithFormat("p%d.mp3", r).c_str());
		}
	}
	

//	KSoundEngine::sharedEngine()->playSound("combo.wav");
	Flying* sp = dynamic_cast<Flying*>(dish);
	CCAssert(sp, "");
	sp->damaged();
	bool isDish = dynamic_cast<Dish*>(sp) ? true : false;
	CCParticleSystem* frag = CCParticleSystem::create("cw_dishbreak.plist");
	if( isDish )
	{
		Dish* sp1 = dynamic_cast<Dish*>(sp);
		frag->setStartColor(graphics.dishColors[sp1->getDishNumber()]);
		frag->setEndColor(graphics.dishColors[sp1->getDishNumber()]);
		frag->setPosition(sp1->getPosition());
		addChild(frag);
		
		if(info.gameMode == AKGameInfo::ZERO)
			eatCoin(2, sp1->getPosition());
	}

	auto animator = SceneUtil::playAnimation("ak_dish_bomb.png", 0.09, DISH_BROKEN_FRAME, DISH_BROKEN_FRAME,
											 DISH_BROKEN_WIDTH, DISH_BROKEN_HEIGHT, false);
	animator.second->setPosition(sp->getPosition());
	animator.second->runAction(CCSequence::createWithTwoActions(animator.first, CCCallFuncN::create(this, callfuncN_selector(AngryGameScene::deleteSprite))));
	addChild(animator.second, 2);
	sp->removeFromParentAndCleanup(true);
	
	
	
	if(isDish)
	{
		if(info.gameMode == AKGameInfo::ZERO)
		{
			graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + AKGameInfo::INC_VALUE_IN_ZERO);
		}
		else
		{
			graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + AKGameInfo::INC_VALUE_IN_NORMAL);
		}
		
		
		
		if(info.gameMode == AKGameInfo::ZERO)
		{
//			info.comboInZeroMode++;
			int zs = info.zeroStep - 5;
			if(zs < 0) zs = 0;
			graphics.leftAim->setScale( MIN(MAX_SCALE, INIT_SCALE - MIN(20, zs) * 0.02f) * info.bonusScale);
			graphics.rightAim->setScale( MIN(MAX_SCALE, INIT_SCALE - MIN(20, zs) * 0.02f) * info.bonusScale );
		}
		else
		{
		}
		
		if(info.gameMode != AKGameInfo::ZERO && graphics.feverGauge->getPercentage() >= 100.f && info.isGameOvering == false)
		{
			onEnterZeroMode();
		}
		
		KS::KSLog("%", akplay->combo.getVar());
		akplay->gameScore = akplay->gameScore.getVar() + scoreFromCombo();
//		wcplay->gameScore = akplay->gameScore.getVar();
//		wcplay->scoreEachGame["AK"] = wcplay->gameScore.getVar();
		CCLabelBMFont* bonusFnt = CCLabelBMFont::create(KS_Util::stringWithFormat("+%d", scoreFromCombo()).c_str(), "bonusscore.fnt");
		bonusFnt->setPosition(ccp(graphics.scoreFnt->getPosition().x + graphics.scoreFnt->getContentSize().width / 3.f,
								  graphics.scoreFnt->getPosition().y - graphics.scoreFnt->getContentSize().height / 3.f));
		addChild(bonusFnt, 2);
		
		float virtualMaxScore = MAX(100000, AKSaveData::getMaxScore());
		float goalScale = MIN(1.3f, 0.6f + (float)akplay->gameScore.getVar() * 2.f / (float)virtualMaxScore);
		bonusFnt->setScale(0.3f);
		auto action0 = CCEaseBackOut::create(CCScaleTo::create(1.f, goalScale));
		auto action1 = CCDelayTime::create(0.2f);
		auto action2 = CCFadeOut::create(0.2f);
		auto action3 = CCCallFuncN::create(this, callfuncN_selector(AngryGameScene::deleteSprite));
		auto totalAction = CCSequence::create(action0, action1, action2, action3, 0);
		bonusFnt->runAction(totalAction);
		
		
		graphics.scoreFnt->runAction(CCEaseBackOut::create(CCScaleTo::create(1.f, goalScale)));
		
		//110, 250
		auto mator = SceneUtil::playAnimation("game_combo.png", 0.1, COMBO_COLS, COMBO_FRAME, COMBO_WIDTH, COMBO_HEIGHT, false);
		mator.second->runAction(CCSequence::create(mator.first,
						//CCDelayTime::create(0.5f),
						CCCallFuncN::create(this, callfuncN_selector(AngryGameScene::deleteSprite)), 0));
		mator.second->setPosition(ccp(85, 250));
		addChild(mator.second, 2);
		
		CCLabelBMFont* comboFnt = CCLabelBMFont::create(KS_Util::stringWithFormat("%d", akplay->combo.getVar()).c_str(), "gamecombo.fnt");
		comboFnt->setPosition(ccp(85, 270));
		comboFnt->setScale(0.6f);
		comboFnt->runAction(CCSequence::create(CCEaseBackOut::create(CCScaleTo::create(0.3, 1.f)),
												CCDelayTime::create(0.5f), CCFadeOut::create(0.5f),
													 CCCallFuncN::create(this, callfuncN_selector(AngryGameScene::deleteSprite)), 0));
		addChild(comboFnt, 2);
		
		for(int i=0; i<=2; i++)
		{
			graphics.guns[i]->runAction(shortShakeAction(GUNPOSITION));
		}
		
	}
	
}

void AngryGameScene::updateScoreFnt(float dt)
{
	int goalScore = akplay->gameScore.getVar();
	int displayedScore = atoi(graphics.scoreFnt->getString());
	if(displayedScore < goalScore )
	{
		int adderValue = (goalScore - displayedScore) / 30 + 21;
		adderValue = MIN(adderValue, goalScore - displayedScore);
		graphics.scoreFnt->setString(KS_Util::stringWithFormat("%d", displayedScore + adderValue).c_str());
	}	
}

int AngryGameScene::scoreFromCombo()
{
	int vCombo = MIN(30, akplay->combo.getVar());
	int r = vCombo * 100;
	
	return info.gameMode == AKGameInfo::ZERO ? 2 * r * 2 : r * 2;
}
void AngryGameScene::onFailShot(bool right)
{
	info.isDisable = true;
	info.disableTime = AngryGameScene::DISABLE_TIME;
	graphics.disableLeft->setVisible(true);
	graphics.disableRight->setVisible(true);
	graphics.leftAim->setOpacity(0);
	graphics.rightAim->setOpacity(0);
	
	bool shieldMode = false;

	{
		if(playInfo->selectedItems["shield"] == true && info.usedShield == false)
		{
			info.usedShield = true;
			shieldMode = true;
			info.lastLeftShotTime += 3.f;
			info.lastRightShotTime += 3.f;
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
		info.lastLeftShotTime = info.gameTimer;
		info.lastRightShotTime = info.gameTimer;
		switch(info.gameMode)
		{
			case AKGameInfo::ZERO:
				akplay->combo = 0;
				KSoundEngine::sharedEngine()->playSound("uhuh.mp3");
//				info.comboInZeroMode = 0;
				int zs;
				zs = info.zeroStep - 5;
				if(zs < 0) zs = 0;
				graphics.leftAim->setScale( MIN(MAX_SCALE, INIT_SCALE - MIN(20, zs) * 0.02f) * info.bonusScale);
				graphics.rightAim->setScale( MIN(MAX_SCALE, INIT_SCALE - MIN(20, zs) * 0.02f) * info.bonusScale);
				float c;
				c = 1.f;
				if(info.zeroStep >= 15)
					c = 1.5f;
				graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + AKGameInfo::CRASH_DEC_VALUE_WHEN_NOTARGET_IN_ZERO * c);
				break;
			case AKGameInfo::NORMAL:
				akplay->combo = 0;
				KSoundEngine::sharedEngine()->playSound("uhuh.mp3");
				graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + AKGameInfo::CRASH_DEC_VALUE_WHEN_NOTARGET_IN_NORMAL);
				break;
			default:
				graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + AKGameInfo::CRASH_DEC_VALUE_WHEN_NOTARGET_IN_NORMAL);
				break;
		}
	}
	
	
	auto animator = SceneUtil::playAnimation("ak_firefail.png", 0.08, FAIL_FRAME, FAIL_FRAME, FAIL_WIDTH, FAIL_HEIGHT, false);
	if(right)
		animator.second->setPosition(graphics.rightAim->getPosition());
	else
		animator.second->setPosition(graphics.leftAim->getPosition());
	animator.second->runAction(CCSequence::createWithTwoActions(animator.first, CCCallFuncN::create(this, callfuncN_selector(AngryGameScene::deleteSprite))));
	addChild(animator.second);
	
	if(shieldMode == false)
		runAction(shakeAction(getPosition()));
}
bool AngryGameScene::onLeftFire()
{
	info.lastLeftShotTime = info.gameTimer;
	graphics.showGun(AKGraphics::LEFT_GUN);
	return killBird(graphics.leftAim);
}
bool AngryGameScene::onRightFire()
{
	info.lastRightShotTime = info.gameTimer;
	graphics.showGun(AKGraphics::RIGHT_GUN);
	
	return killBird(graphics.rightAim);
	
}
bool AngryGameScene::killBird(CCSprite* gunMark)
{
	vector<list<Bird*>::iterator> dishIters;
	vector<list<Bird*>::iterator> notDishIters;
	for(auto iter = graphics.birds.begin(); iter != graphics.birds.end(); ++iter)
	{
		if(gunMark->boundingBox().containsPoint( (*iter)->getPosition() ))
		{
			if(dynamic_cast<Dish*>(*iter))
				dishIters.push_back(iter);
			else if(dynamic_cast<NotDish*>(*iter))
				notDishIters.push_back(iter);
		}
	}
	
	if(dishIters.empty() && notDishIters.empty())
		return false; // 아무것도 못쐈으면 실패처리함
	else
	{
		bool existDish = !dishIters.empty();
		if(existDish)
		{
			for(auto iter = dishIters.begin(); iter != dishIters.end(); ++iter)
			{
				onShot(**iter);
				graphics.birds.erase(*iter);
			}
			return true;
		}
		else
		{
			for(auto iter = notDishIters.begin(); iter != notDishIters.end(); ++iter)
			{
				onShot(**iter);
				graphics.birds.erase(*iter);
			}
			return true;
		}
	}
}
void AngryGameScene::onShotBird(bool right)
{
	info.isDisable = true;
	info.disableTime = AngryGameScene::DISABLE_TIME;
	graphics.disableLeft->setVisible(true);
	graphics.disableRight->setVisible(true);
	graphics.leftAim->setOpacity(0);
	graphics.rightAim->setOpacity(0);
	
	bool shieldMode = false;
	
	{
		if(playInfo->selectedItems["shield"] == true && info.usedShield == false)
		{
			info.usedShield = true;
			shieldMode = true;
			info.lastLeftShotTime += 1.f;
			info.lastRightShotTime += 1.f;
			KSoundEngine::sharedEngine()->playSound("shield.mp3");
			CCSprite* shield = CCSprite::create("item_shield1_1.png");
			shield->setPosition(ccp(240, 200));
			addChild(shield, 10000);
			shield->runAction(CCSequence::create(CCBlink::create(0.8, 3.f), CCDelayTime::create(2.f), CCFadeOut::create(0.5f),
												  CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite)), 0));
		}

	}
	if(shieldMode == false)
	{
		info.lastLeftShotTime = info.gameTimer;
		info.lastRightShotTime = info.gameTimer;
		switch(info.gameMode)
		{
			case AKGameInfo::ZERO:
//				info.comboInZeroMode = 0;
				akplay->combo = 0;
				KSoundEngine::sharedEngine()->playSound("uhuh.mp3");
				float c;
				c = 1.f;
				if(info.zeroStep >= 15)
					c = 1.5f;
				int zs;
				zs = info.zeroStep - 5;
				if(zs < 0) zs = 0;
				graphics.leftAim->setScale( MIN(MAX_SCALE, INIT_SCALE - MIN(20, zs) * 0.02f)* info.bonusScale );
				graphics.rightAim->setScale( MIN(MAX_SCALE, INIT_SCALE - MIN(20, zs) * 0.02f) * info.bonusScale);
				graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + AKGameInfo::CRASH_DEC_VALUE_WHEN_NOTARGET_IN_ZERO * c);
				break;
			case AKGameInfo::NORMAL:
				akplay->combo = 0;
				KSoundEngine::sharedEngine()->playSound("uhuh.mp3");
				graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + AKGameInfo::CRASH_DEC_VALUE_WHEN_NOTARGET_IN_NORMAL);
				break;
			default:
				akplay->combo = 0;
				KSoundEngine::sharedEngine()->playSound("uhuh.mp3");
				graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + AKGameInfo::CRASH_DEC_VALUE_WHEN_NOTARGET_IN_NORMAL);
				break;
		}
		runAction(shakeAction(getPosition()));
	}
		
	
	
	//onFailShot(right);
}
bool AngryGameScene::init()
{
	CCLayer::init();
	
	this->setKeypadEnabled(true);
	KSoundEngine::sharedEngine()->playSound("hotshots.mp3");
//	KSoundEngine::sharedEngine()->playSound("beat area.mp3");
//	KSoundEngine::sharedEngine()->playSound("Block Buster.mp3");
//	KSoundEngine::sharedEngine()->playSound("Monster Truck.mp3");
	playInfo->__ateCoin = 0;
	setTouchEnabled(true);
	
	auto t1 = SceneUtil::playAnimation("game_combo.png", 0.1, COMBO_COLS, COMBO_FRAME, COMBO_WIDTH, COMBO_HEIGHT, false); // texture 로딩. 바로 해제될 녀석.
	auto t2 = CCLabelBMFont::create("00", "gamecombo.fnt"); // texture 로딩. 바로 해제될 녀석.
	t1.second->setPosition(ccp(-200, -200));
	t1.second->runAction(CCSequence::createWithTwoActions(t1.first, CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite))));
	addChild(t1.second);

	addChild(t2); removeChild(t2, true);
	int remainder;
	if(NSDefault::isReview()) //
		remainder = 5;
	else
		remainder = 1;
	CCSprite* t = CCSprite::create(KS_Util::stringWithFormat("ak_back%d.png", (rand()) % remainder + 1).c_str());
	
	
	t->getTexture()->setAliasTexParameters();
	t->setAnchorPoint(ccp(0, 0));
	
	addChild(t, 0);
	
		
	
	
	
	CCSprite* fever_back = CCSprite::create("ui_zerobar_back.png");
	fever_back->setPosition(ccp(240, 12));
	addChild(fever_back, 3);
	
	graphics.feverGauge = CCProgressTimer::create(CCSprite::create("ui_zerobar_front.png"));
	graphics.feverGauge->setBarChangeRate(ccp(1, 0));
	graphics.feverGauge->setMidpoint(ccp(0, 0.5f));
	graphics.feverGauge->setPosition(ccp(240, 12));
	graphics.feverGauge->setType(CCProgressTimerType::kCCProgressTimerTypeBar);
	
	addChild(graphics.feverGauge, 3);
	
	
	
	graphics.watch1 = CCSprite::create("ui_time1.png");
	graphics.watch1->setPosition(ccp(431, 277));
	addChild(graphics.watch1, 1);
	auto watchMator = SceneUtil::playAnimation("ui_time2.png", 0.05, 2, 2, 85, 75, true);
	watchMator.second->runAction(watchMator.first);
	graphics.watch2 = watchMator.second;
	graphics.watch2->setPosition(ccp(431, 277));
	addChild(graphics.watch2, 1);
	graphics.watch2->setVisible(false);
	
	
	graphics.timeFnt = CCLabelBMFont::create(KS_Util::stringWithFormat("%.0f", (info.remainTime) * BASETIME / info.INIT_GAME_TIME).c_str(), "time1.fnt");
	addChild(graphics.timeFnt, 2);
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
	
	auto aimator1 = SceneUtil::playAnimation("ak_leftaim.png", 0.2, AKGraphics::AIM_FRAME, AKGraphics::AIM_FRAME, AKGraphics::AIM_SIZE, AKGraphics::AIM_SIZE, true);
	graphics.leftAim = aimator1.second;
	graphics.leftAim->setPosition(ccp(AngryGameScene::LEFT_AIM_X, AngryGameScene::LEFT_AIM_Y));
	graphics.leftAim->setScale(INIT_SCALE * info.bonusScale);
	graphics.leftAim->runAction(aimator1.first);
	addChild(graphics.leftAim, 3);
	 
	graphics.disableLeft = CCSprite::create("ak_aimdisable.png");
	graphics.disableLeft->setAnchorPoint(ccp(0, 0));
	graphics.disableLeft->setVisible(false);
	graphics.leftAim->addChild(graphics.disableLeft);
	
	auto aimator2 = SceneUtil::playAnimation("ak_rightaim.png", 0.2, AKGraphics::AIM_FRAME, AKGraphics::AIM_FRAME, AKGraphics::AIM_SIZE, AKGraphics::AIM_SIZE, true);
	graphics.rightAim = aimator2.second;
	graphics.rightAim->setPosition(ccp(AngryGameScene::RIGHT_AIM_X, AngryGameScene::RIGHT_AIM_Y));
	graphics.rightAim->setScale(INIT_SCALE * info.bonusScale);
	graphics.rightAim->runAction(aimator2.first);
	
	addChild(graphics.rightAim, 3);
	
	graphics.disableRight = CCSprite::create("ak_aimdisable.png");
	graphics.disableRight->setAnchorPoint(ccp(0, 0));
	graphics.disableRight->setVisible(false);
	graphics.rightAim->addChild(graphics.disableRight);
	
	
	graphics.scoreFnt = CCLabelBMFont::create("0", "uiscore.fnt");
	graphics.scoreFnt->setAnchorPoint(ccp(0.5, 1.f));
	graphics.scoreFnt->setPosition(ccp(240, 315.f));
	graphics.scoreFnt->setScale(0.6f);
	addChild(graphics.scoreFnt, 2);
	
	CCSprite* leftGun = CCSprite::create("ak_leftgun.png");
	leftGun->setPosition(GUNPOSITION);
	addChild(leftGun, 1);
	graphics.guns.push_back(leftGun);
	
	CCSprite* cGun = CCSprite::create("ak_centergun.png");
	cGun->setPosition(GUNPOSITION);
	addChild(cGun, 1);
	graphics.guns.push_back(cGun);
	
	CCSprite* rGun = CCSprite::create("ak_rightgun.png");
	rGun->setPosition(GUNPOSITION);
	addChild(rGun, 1);
	graphics.guns.push_back(rGun);
	graphics.showGun(AKGraphics::CENTER_GUN);
	
	CCSprite* leftFire = CCSprite::create("ak_leftfire.png");
	leftFire->setPosition(ccp(45, 60));
	addChild(leftFire, 2);
	
	CCSprite* rightFire = CCSprite::create("ak_rightfire.png");
	rightFire->setPosition(ccp(435, 60));
	addChild(rightFire, 2);
	KSoundEngine::sharedEngine()->playSound("readygo.mp3");
	auto mator = SceneUtil::playAnimation("ready.png", 0.1, 2, 10, 256, 65, false);
	auto action = CCSequence::create(mator.first, CCCallFuncN::create(this, callfuncN_selector(ThisClassType::finishReady)), 0);
	mator.second->runAction(action);
	addChild(mator.second, 1000);
	mator.second->setPosition(ccp(240, 160));
	

	{
		map<string, string> item2img;
		item2img["shield"] = "item_shield1_1.png"; item2img["exp"] = "item_exp1_1.png";
		item2img["item_bird1.png"] = "item_bird1_1.png"; item2img["item_large1.png"] = "item_large1_1.png";
		
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
	ui_gold->setPosition(ccp(240, 34));
	addChild(ui_gold, 10);
	
	graphics.ateGoldFnt = CCLabelBMFont::create("0", "main1.fnt");
	graphics.ateGoldFnt->setPosition(ccp(248, 34));
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
void AngryGameScene::PAUSE(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	AKPauseLayer* pauseLayer = AKPauseLayer::create();
	//pauseLayer->setPosition(ccp(0, -20));
	addChild(pauseLayer, 1000);
	
	KS::KSLog("pause");
}
void AngryGameScene::finishGo(CCNode* n)
{
	n->removeFromParentAndCleanup(true);
	schedule(schedule_selector(ThisClassType::update));
}
void AngryGameScene::finishReady(CCNode* n)
{
	n->removeFromParentAndCleanup(true);
	auto mator = SceneUtil::playAnimation("go.png", 0.075, 2, 4, 256, 65, false);
	auto action0 = CCSpawn::create(CCSequence::createWithTwoActions(CCDelayTime::create(0.1f), CCScaleTo::create(0.4f, 4.f)), CCSequence::createWithTwoActions(CCDelayTime::create(0.3f), CCFadeOut::create(0.2f)), mator.first, 0);
	auto action = CCSequence::create(action0, CCCallFuncN::create(this, callfuncN_selector(ThisClassType::finishGo)), 0);
	mator.second->runAction(action);
	addChild(mator.second, 1000);
	mator.second->setPosition(ccp(240, 160));
}
void AngryGameScene::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}
void AngryGameScene::onExit()
{
	CCLayer::onExit();
}
void AngryGameScene::createFlyingProc(float dt)
{
	//CCLog("%f   %f", info.nextCreateTime, info.gameTimer);
	bool bZero = info.gameMode == AKGameInfo::ZERO ? true : false;
	
	if(info.nextCreateTime < info.gameTimer)
	{
		info.lastCreateTime = info.nextCreateTime;
		info.nextCreateTime += info.minTerm + AngryGameScene::rnd() * (info.maxTerm - info.minTerm);
		
		///////////////////////////////
		if(!info.isDisable)
		{
			bool x = bZero && info.zeroStep >= CROSS_BEAT_STEP;
			if(!x) // 엇박 버전이 아니면
			{
				if(rand()%5 == 0) // 엇박 없을 때의 확률
					createFlying(true);
				else
					createFlying(false);
			}
			else
			{
				createFlying(false);
			}
		}
	}
	
	if(info.nextCreateTime2 < info.gameTimer)
	{
		info.lastCreateTime2 = info.nextCreateTime2;
		float leftUp;
		if(info.zeroStep <= 7)
		{
			leftUp = 4.f;
		}
		else if(info.zeroStep <= 10)
		{
			leftUp = 2.f;
		}
		else
		{
			leftUp = 1.f;
		}
		info.nextCreateTime2 += (info.minTerm*leftUp) + AngryGameScene::rnd() * (info.maxTerm*leftUp - info.minTerm*leftUp) + 0.45f;
		
		///////////////////////////////
		if(!info.isDisable && bZero && info.zeroStep >= CROSS_BEAT_STEP) // 제로모드부터 엇박 출연함.
		{
			createFlying(true);
		}
	}
}
void AngryGameScene::createFlying(bool toR)
{
	Flying* bb; //= Dish::sprite(this, getVelocity());
	bool bZero = info.gameMode == AKGameInfo::ZERO ? true : false;
	bool isFewBird = false;
	
	
	isFewBird = playInfo->selectedItems["item_bird1.png"];
	
	int r = isFewBird ? 10 : 8;
	if(rand() % r == 0 && bZero)
	{
		bb = NotDish::sprite(this, getVelocity());
		info.lastLeftShotTime += 1.5f; // 새 인경우 왼쪽 리셋 타임을 올려줌... 안쏘다보면 콤보초기화 되는거 방지.
	}
	else
	{
		bb = Dish::sprite(this, getVelocity());
	}
	
	addChild(bb);
	bb->initFlying(toR, bZero);
	//bb->initFlying(rand()%2 ? false : true);
	graphics.birds.push_back(bb);
}
float AngryGameScene::getVelocity()
{
	// base value + ...
	KS::KSLog("zerostep = %", info.zeroStep);
	int limitStep = MIN(10, info.zeroStep);
	return 140 + limitStep * 18;//200.f * graphics.feverGauge->getPercentage() / 100.f;
}


void AngryGameScene::timeChecker(float dt)
{
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

#if 0
		{
			if(playInfo->selectedItems["item_random"] && !graphics.watch3->isVisible())
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
		}
#endif
		if(info.remainTime <= -info.bonusTime && info.isGameOvering == false)
		{
			setTouchEnabled(false);
			info.isGameOvering = true;
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

	if(info.gameMode == AKGameInfo::ZERO)
	{
		info.timeAfterZero += dt;
		
		int nextStep = MIN((int)(info.timeAfterZero / 2.25f + 1.f), 25); // 10 초에 한번 ?
		if(nextStep != info.beforeRunStep)
		{
			info.zeroStep = nextStep;
			if(info.zeroStep == CROSS_BEAT_STEP)
			{
				info.lastLeftShotTime = info.gameTimer;
			}
			info.beforeRunStep = info.zeroStep;
		}
		if(info.zeroStep >= 10)
		{
			int z = MIN(15, info.zeroStep);
			z -= 9;
			info.minTerm = AKGameInfo::INIT_MINTERM - z * 0.05f;
			info.maxTerm = AKGameInfo::INIT_MAXTERM - z * 0.05f;
		}
//		CCLog("-- %d", info.zeroStep);
		graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + AKGameInfo::ZERO_DEC_VALUE);
		if(graphics.feverGauge->getPercentage() <= 0.f)
		{
			onExitZeroMode();
		}
	}
	bool bZero = info.gameMode == AKGameInfo::ZERO ? true : false;
	if(info.lastLeftShotTime + COMBO_RESET_TIME < info.gameTimer && bZero && info.zeroStep >= CROSS_BEAT_STEP)
	{
		akplay->combo = 0;
		KSoundEngine::sharedEngine()->playSound("uhuh.mp3");
		info.lastLeftShotTime = info.gameTimer;
		
		switch(info.gameMode)
		{
			case AKGameInfo::ZERO:
				graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + AKGameInfo::CRASH_DEC_VALUE_WHEN_NOTARGET_IN_ZERO);
				break;
			case AKGameInfo::NORMAL:
				graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + AKGameInfo::CRASH_DEC_VALUE_WHEN_NOTARGET_IN_NORMAL);
				break;
			default:
				graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + AKGameInfo::CRASH_DEC_VALUE_WHEN_NOTARGET_IN_NORMAL);
				break;
		}
	}
	if(info.lastRightShotTime + COMBO_RESET_TIME < info.gameTimer)
	{
		akplay->combo = 0;
		KSoundEngine::sharedEngine()->playSound("uhuh.mp3");
		info.lastRightShotTime = info.gameTimer;
		
		switch(info.gameMode)
		{
			case AKGameInfo::ZERO:
				graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + AKGameInfo::CRASH_DEC_VALUE_WHEN_NOTARGET_IN_ZERO);
				break;
			case AKGameInfo::NORMAL:
				graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + AKGameInfo::CRASH_DEC_VALUE_WHEN_NOTARGET_IN_NORMAL);
				break;
			default:
				graphics.feverGauge->setPercentage(graphics.feverGauge->getPercentage() + AKGameInfo::CRASH_DEC_VALUE_WHEN_NOTARGET_IN_NORMAL);
				break;
		}
	}
	if(info.isDisable)
	{
		info.disableTime -= dt;
		if(info.disableTime <= 0.2f)
		{
			info.isDisable = false;
			graphics.disableLeft->setVisible(false);
			graphics.disableRight->setVisible(false);
			graphics.leftAim->setOpacity(255);
			graphics.rightAim->setOpacity(255);
		}
	}
}

void AngryGameScene::moveFlying(float dt)
{
	for(auto iter = graphics.birds.begin(); iter != graphics.birds.end();)
	{
		(*iter)->setNextPath(dt);
		
		if( (*iter)->getIsGoal() )
		{
			CCSprite* sp = (*iter);
			
			iter = graphics.birds.erase(iter);
			sp->removeFromParentAndCleanup(true);
		}
		else
		{
			++iter;
		}
	}
}
void AngryGameScene::update(float dt)
{
	dt = 1.f/60.f;
	info.gameTimer += dt;
	
	if(info.gameMode == AKGameInfo::NORMAL)
	{
		info.remainTime -= dt;
	}
	
	
	createFlyingProc(dt);
	moveFlying(dt);
	traceTarget(dt);
	updateScoreFnt(dt);
	timeChecker(dt);
	
	
}


void AngryGameScene::traceTarget(float dt)
{
	if(graphics.birds.empty())
		return;
	auto leftTarget = graphics.birds.begin();
	auto rightTarget = graphics.birds.begin();
	float leftMinDistance, rightMinDistance;
	leftMinDistance = rightMinDistance = 9999999.f;
	float aimSize = graphics.leftAim->boundingBox().size.width;
	for(auto iter = graphics.birds.begin(); iter != graphics.birds.end(); ++iter)
	{
		CCSprite* sp = (*iter);
		float targetPos = sp->getPosition().x;
		bool isDish = dynamic_cast<Dish*>(sp) ? true : false;
		if((*iter)->getRight())
		{
			if(0 < targetPos && targetPos <= AngryGameScene::LEFT_AIM_X + aimSize / 2.f)
			{
				if(fabsf(AngryGameScene::LEFT_AIM_X - targetPos) < leftMinDistance)
				{
					leftMinDistance = fabsf(AngryGameScene::LEFT_AIM_X - targetPos);
					leftTarget = iter;
				}
			}
			else if(AngryGameScene::LEFT_AIM_X + aimSize / 2.f < targetPos && targetPos <= AngryGameScene::RIGHT_AIM_X + aimSize / 2.f
					&& isDish)
			{
				if( fabsf(AngryGameScene::RIGHT_AIM_X - targetPos) < rightMinDistance)
				{
					rightMinDistance = fabsf(AngryGameScene::RIGHT_AIM_X - targetPos);
					rightTarget = iter;
				}
			}
		}
		else
		{
			if(AngryGameScene::RIGHT_AIM_X - aimSize / 2.f <= targetPos && targetPos <= 480)
			{
				if( fabsf(AngryGameScene::RIGHT_AIM_X - targetPos) < rightMinDistance)
				{
					rightMinDistance = fabsf(AngryGameScene::RIGHT_AIM_X - targetPos);
					rightTarget = iter;
				}
			}
			else if(AngryGameScene::LEFT_AIM_X - aimSize / 2.f <= targetPos && targetPos < AngryGameScene::RIGHT_AIM_X - aimSize / 2.f
					&& isDish)
			{
				if(fabsf(AngryGameScene::LEFT_AIM_X - targetPos) < leftMinDistance)
				{
					leftMinDistance = fabsf(AngryGameScene::LEFT_AIM_X - targetPos);
					leftTarget = iter;
				}
			}
		}
	}
	graphics.leftAim->setPosition(ccp(graphics.leftAim->getPosition().x, (*leftTarget)->getPosition().y));
	graphics.rightAim->setPosition(ccp(graphics.rightAim->getPosition().x, (*rightTarget)->getPosition().y));
	traceLeftTarget(dt);
	traceRightTarget(dt);
}
void AngryGameScene::traceLeftTarget(float dt)
{
	
}
void AngryGameScene::traceRightTarget(float dt)
{
	
}



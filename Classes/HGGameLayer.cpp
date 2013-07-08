//
//  HGGameLayer.cpp
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 6. 5..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "HGGameLayer.h"
#include "HGPlayer.h"
#include "HGBuilding.h"
#include "KSBlock.h"
#include "HGMiddleObs.h"
#include "HGTopObs.h"
#include "SceneUtil.h"
#include "HGCoin.h"
#include "HGGraphics.h"
#include "KSoundEngine.h"
#include "GameSystem.h"
#include "HGPlay.h"

#include "HGGameScene.h"
#include "LuaManager.h"
//float HGGameLayer::PLAYER_SPEED = 5.f; // 게임 진행 속도.
//float HGGameLayer::CHAR_POSITION = 100; // 왼쪽으로부터 떨어짐 // 100
//float HGGameLayer::MIN_TURNING_SPACING = 150; // 꼭지점 사이의 x 거리
//float HGGameLayer::MAX_TURNING_SPACING = 500; // 꼭지점 사이의 x 거리
//float HGGameLayer::MIN_BUILDING_Y = 30; // 빌딩 높이
//float HGGameLayer::MAX_BUILDING_Y = 170; // 빌딩 높이
//float HGGameLayer::MIN_PASS_SPACING = 100; // 통과 간격
//float HGGameLayer::MAX_PASS_SPACING = 200; // 통과 간격
//const float HGGameLayer::ONCE_CREATING_SIZE = 480.f;
//float HGGameLayer::MIN_MIDDLE_DISTANCE = 300; // 중간 장애물 거리
//float HGGameLayer::MAX_MIDDLE_DISTANCE = 500; // 중간 장애물 거리
//float HGGameLayer::MIN_PASS_MARGIN = 60;

template <typename RVal>
RVal lu_call(const char* n)
{
	lua_State* L = LuaManager::shared()->getLua("hg.lua");
//	HGGameScene* hs = (HGGameScene*)HGL->getParent();
	return lua_tinker::get<RVal>(L, n);
//	return lua_tinker::call<RVal>(HGL->getL(), n, hgplay->combo.getVar(), hs->getInfo().gameMode, hs->getInfo().timeAfterZero);
}

HGGameLayer::HGGameLayer()
{
	lastCreationDistance = -400;
	lastCreationY = 0;
	nextBuildingPosition = -20;
	nextObsPosition = 300;
	nextTopObsPosition = -20;
	vertVelocity = 0;
	isUp = false;
	isStarted = false;
	
	
	}
HGGameLayer::~HGGameLayer()
{
	
}
void HGGameLayer::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}
bool HGGameLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = pTouch;
	
	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	
	return true;
}

void HGGameLayer::createMiddleObs()
{
	if(tuningPoints.empty() == false)
	{
		while(nextObsPosition <= nextBuildingPosition)
		{
			std::pair<CCPoint, CCPoint> pairPoint =  getPairPointWithBottom(nextObsPosition);
			std::pair<CCPoint, CCPoint> pairTopPoint =  getPairPointWithTop(nextObsPosition);
			CCPoint p1 = pairPoint.first;
			CCPoint p2 = pairPoint.second;
			CCPoint p3 = pairTopPoint.first;
			CCPoint p4 = pairTopPoint.second;
			
			if(p1.equals(ccp(-100.f, -100.f)) && p2.equals(ccp(-100.f, -100.f)))
			{
				break;
			}
			
			float buildingHeight = (p2.y - p1.y) / (p2.x - p1.x) * (nextObsPosition - p1.x) + p1.y;
			float topHeight = (p4.y - p3.y) / (p4.x - p3.x) * (nextObsPosition - p3.x) + p3.y;
			vector<CCSprite*> middleList;
			for(int i=1; i<=3; i++)
			{
				middleList.push_back(CCSprite::create(CCString::createWithFormat("hg_stone%d.png", i)->getCString()));
			}
			
			bool onceCreate = false;
			int stoneIndex = middleList.size();
			for(auto iter = middleList.rbegin(); iter != middleList.rend(); ++iter, --stoneIndex)
			{
				float spaceSize = topHeight - buildingHeight;
				float obsSize = (*iter)->getContentSize().height;
//				CCLog("obsSize = %f", obsSize);
				if(spaceSize - obsSize <= lu_call<float>("MIN_PASS_MARGIN")) // 생성할 여건이 안되면 패스.
				{
					CCLog("여견 안됨.");
					continue;
				}
				int tryCount = 10;
				bool exitLoop = false;
				for(int i=0; i<tryCount; i++)
				{
					float mo = well512.GetFloatValue(buildingHeight, topHeight);
					
					float maxValue = MAX(topHeight - mo - obsSize / 2.f, mo - buildingHeight - obsSize / 2.f);
					if(maxValue > lu_call<float>("MIN_PASS_MARGIN"))
					{
						HGMiddleObs* middleObs = HGMiddleObs::createWithStoneIndex(stoneIndex);
						middleObsSprites.push_back(middleObs);
						
						addChild(middleObs, 2);
						middleObs->setPosX(nextObsPosition);
						middleObs->setPosY(mo);
						nextObsPosition += well512.GetFloatValue(lu_call<float>("MIN_MIDDLE_DISTANCE") , lu_call<float>("MAX_MIDDLE_DISTANCE"));
						
						onceCreate = true;
						exitLoop = true;
						break;
					}
				}
				if(exitLoop)
					break;
			}
			if(onceCreate == false)
			{
				nextObsPosition += 100; // 다음 기회에 생성.
			}
		}
	}
}

void HGGameLayer::createTopObs()
{
	if(tuningPoints.empty() == false)
	{
		while(nextTopObsPosition <= (tuningPoints.rbegin())->top.x)
		{
			std::pair<CCPoint, CCPoint> pairPoint =  getPairPointWithTop(nextTopObsPosition);
			CCPoint p1 = pairPoint.first;
			CCPoint p2 = pairPoint.second;
			if(p1.equals(ccp(-100.f, -100.f)) && p2.equals(ccp(-100.f, -100.f)))
			{
				break;
			}
			
			HGTopObs* bd = HGTopObs::create();
			bd->setAnchorPoint(ccp(0.0f, 0.f));
			topObsSprites.push_back(bd);
			addChild(bd, 2);
			bd->setPosX(nextTopObsPosition);
			
			float yPosition = (p2.y - p1.y) / (p2.x - p1.x) * (nextTopObsPosition - p1.x) + p1.y;
			float hSize = 320 - yPosition;
			//			bd->setScale(hSize / bd->getContentSize().height);
			bd->setPosY(yPosition);
			nextTopObsPosition += bd->getCloudWidth();
		}
	}
	
}
void HGGameLayer::createBuilding()
{
	if(tuningPoints.empty() == false)
	{
		while(nextBuildingPosition <= (tuningPoints.rbegin())->bottom.x)
		{
			//			CCLog("in!!");
			std::pair<CCPoint, CCPoint> pairPoint =  getPairPointWithBottom(nextBuildingPosition);
			CCPoint p1 = pairPoint.first;
			CCPoint p2 = pairPoint.second;
			if(p1.equals(ccp(-100.f, -100.f)) && p2.equals(ccp(-100.f, -100.f)))
			{
				break;
			}
			
			HGBuilding* bd = HGBuilding::create();
			bd->setAnchorPoint(ccp(0.5f, 1.f));
			buildingSprites.push_back(bd);
			addChild(bd, 2);
			bd->setPosX(nextBuildingPosition);
			bd->setPosY((p2.y - p1.y) / (p2.x - p1.x) * (nextBuildingPosition - p1.x) + p1.y);
			createCoin(bd);
			nextBuildingPosition += bd->getBuildingWidth();
		}
	}
	
}

void HGGameLayer::createCoin(HGBuilding* bd)
{
	//	for(HGBuilding* bd : buildingSprites)
	HGGameScene* hs = (HGGameScene*)getParent();
	if(hs->getInfo().gameMode == HGGameInfo::ZERO)
	{
		
		HGCoin* coin = HGCoin::create();
		coin->setPosX(bd->getPosX());
		coin->setPosY(bd->getPosY() + 40);
		coinSprites.push_back(coin);
		addChild(coin, 2);
		
	}
	
	//	auto coinAnimation = SceneUtil::playAnimation("coin.png", 0.07f, 6, 6, Graphics::COIN_WIDTH, Graphics::COIN_HEIGHT, true);
	//	coinAnimation.second->runAction(coinAnimation.first);
	//	addChild(coinAnimation.second, zorder::COIN);
}

void HGGameLayer::initTurningPoint()
{
	{
		
		
		TurningPoint tp;
		tp.bottom = ccp(-200, 50);
		tp.top = ccp(-200, 280);
		tuningPoints.push_back(tp);
	}
	{
		TurningPoint tp;
		tp.bottom = ccp(500, 50);
		tp.top = ccp(500, 280);
		tuningPoints.push_back(tp);
	}
	lastCreationDistance = 500;
}
void HGGameLayer::createTuningPoint()
{
	float limitDistance = lastCreationDistance + lu_call<float>("ONCE_CREATING_SIZE");
	float oldDistance = lastCreationDistance;
	float oldY = lastCreationY;
	float createCondition = 480.f;
	if(lastCreationDistance - createCondition <= hgPlayer->getPosX())
	{
		while(limitDistance > oldDistance)
		{
			oldDistance += well512.GetFloatValue(lu_call<float>("MIN_TURNING_SPACING"), lu_call<float>("MAX_TURNING_SPACING"));
			//			CCLog("d : %f", oldDistance);
			oldY = well512.GetFloatValue(lu_call<float>("MIN_BUILDING_Y") , lu_call<float>("MAX_BUILDING_Y"));// * well512.GetPlusMinus();
			
			TurningPoint tp;
			tp.bottom = ccp(oldDistance, oldY);
			tp.top = ccp(oldDistance, oldY + well512.GetFloatValue(lu_call<float>("MIN_PASS_SPACING"), lu_call<float>("MAX_PASS_SPACING")));
			tuningPoints.push_back(tp);
			//			HGBuilding* bd = HGBuilding::create();
			//			addChild(bd, 2);
			//			bd->setPosX(oldDistance);
			//			bd->setPosY(oldY);
			//			buildingSprites.push_back(bd);
		}
		lastCreationDistance = oldDistance;
	}
}
std::pair<CCPoint, CCPoint> HGGameLayer::getPairPointWithTop(float xp)
{
	if(tuningPoints.size() >= 2)
	{
		for(int i=0; i<tuningPoints.size() - 1; i++)
		{
			if(tuningPoints[i].top.x <= xp && xp <= tuningPoints[i+1].top.x)
			{
				return std::make_pair(tuningPoints[i].top, tuningPoints[i+1].top);
			}
		}
	}
	return std::make_pair(ccp(-100.f, -100.f), ccp(-100.f, -100.f));
}
std::pair<CCPoint, CCPoint> HGGameLayer::getPairPointWithBottom(float xp)
{
	if(tuningPoints.size() >= 2)
	{
		for(int i=0; i<tuningPoints.size() - 1; i++)
		{
			if(tuningPoints[i].bottom.x <= xp && xp <= tuningPoints[i+1].bottom.x)
			{
				return std::make_pair(tuningPoints[i].bottom, tuningPoints[i+1].bottom);
			}
		}
	}
	return std::make_pair(ccp(-100.f, -100.f), ccp(-100.f, -100.f));
}
void HGGameLayer::movePlayer()
{
	HGGameScene* hs = (HGGameScene*)getParent();
	
	if(isStarted)
	{
//		hgPlayer->setPosX(hgPlayer->getPosX() + lua_tinker::call<float>(L, "abc"));
		float retValue = lu_call<float>("PLAYER_SPEED");
//		CCLog("retValue %f", retValue);
		hgPlayer->setPosX(hgPlayer->getPosX() + retValue);
	}
	

	//	CCLog("player pos %f", hgPlayer->getPosX());
}
void HGGameLayer::update(float dt)
{
	createTuningPoint();
	createBuilding();
	createTopObs();
	createMiddleObs();
	render();
	movePlayer();
	upDownProcess();
	checkEatCoin();
	crashProcess();
	checkPassObs();
	gabageCollector();
}

void HGGameLayer::crashProcess()
{
	// 플레이어와 빌딩, 하늘, 중간 장애물과 충돌처리함.
	float oldScale = hgPlayer->getScale();
	hgPlayer->setScale(oldScale * 0.4f);
	CCRect b = hgPlayer->boundingBox(); // 충돌 판정
	hgPlayer->setScale(oldScale);
	
	bool crashed = false;
	for_each(buildingSprites.begin(), buildingSprites.end(), [&](HGBuilding* build)
			 {
				 CCRect a = build->boundingBox();
				 if(this->AABB(a, b) && build->getCollapse() == false)
				 {
					 if(hgPlayer->STATE == HGPlayer::NORMAL)
					 {
						 hgPlayer->damaged();
						 build->setCollapse(true); // 붕괴로 설정.
						 float angle = 5.f;
						 build->runAction(CCSequence::create(
															 CCRotateBy::create(0.1f, -angle),
															 CCRepeat::create(CCSequence::create(CCRotateBy::create(0.2, angle * 2.f),
																								 CCRotateBy::create(0.2, -angle * 2.f), 0), 5), 0));
						 crashed = true;
						 return;
						 //					 hgPlayer->setColor(ccc3(255, 0, 0));
					 }else
					 {
						 build->setCollapse(true); // 붕괴로 설정.
						 float angle = 5.f;
						 build->runAction(CCSequence::create(
															 CCRotateBy::create(0.1f, -angle),
															 CCRepeat::create(CCSequence::create(CCRotateBy::create(0.2, angle * 2.f),
																								 CCRotateBy::create(0.2, -angle * 2.f), 0), 5), 0));
					 }
				 }
			 });
	if(crashed)
	{
		CCLog("onHGCrash");
		CCNotificationCenter::sharedNotificationCenter()->postNotification("onHGCrash");
		return;
	}
	
	for_each(topObsSprites.begin(), topObsSprites.end(), [&](CCSprite* build)
			 {
				 CCRect a = build->boundingBox();
				 if(this->AABB(a, b) && hgPlayer->STATE == HGPlayer::NORMAL)
				 {
					 hgPlayer->damaged();
					 
					 crashed = true;
					 return;
					 //					 hgPlayer->setColor(ccc3(255, 0, 0));
				 }
				 
			 });
	if(crashed)
	{
		CCLog("onHGCrash");
		CCNotificationCenter::sharedNotificationCenter()->postNotification("onHGCrash");
		return;
	}
	for_each(middleObsSprites.begin(), middleObsSprites.end(), [&](HGMiddleObs*& build)
			 {
				 CCRect a = build->boundingBox();
				 if(this->AABB(a, b) && hgPlayer->STATE == HGPlayer::NORMAL)
				 {
					 
					 hgPlayer->damaged();
					 build->setIsAttack(true);
					 crashed = true;
					 return;
					 //					 hgPlayer->setColor(ccc3(255, 0, 0));
				 }
				 
			 });
	if(crashed)
	{
		CCLog("onHGCrash");
		CCSprite* g;
		CCNotificationCenter::sharedNotificationCenter()->postNotification("onHGCrash");
		return;
	}
	
}

void HGGameLayer::onScore()
{
	CCLog("Score!!");
	CCNotificationCenter::sharedNotificationCenter()->postNotification("onHGScore");
	
	
}
void HGGameLayer::checkPassObs()
{
	for_each(middleObsSprites.begin(), middleObsSprites.end(), [=](HGMiddleObs*& build)
			 {
				 
				 // 득점 조건 검사
				 bool passed = false;
				 if(hgPlayer->getPosX() > build->getPosX() + build->getWidth() / 2.f
					&& build->getIsChecked() == false)
				 {
					 build->setIsChecked(true);
					 passed = true;
				 }
				 if(passed && hgPlayer->STATE == HGPlayer::NORMAL) // 방금 막 지나감
				 {
					 this->onScore();
				 }
			 });
	
}
bool HGGameLayer::AABB(CCRect a, CCRect b)
{
	//접하면 충돌로 봄.
	//x축에대하여
	
	if( a.getMaxX() < b.getMinX() ||
	   a.getMinX() > b.getMaxX() )
		return false;
	
	//y축에대하여
	
	if(a.getMaxY() < b.getMinY() ||
	   a.getMinY() > b.getMaxY() )
		return false;
	return true;
}
void HGGameLayer::gabageCollector()
{
	for(auto iter = buildingSprites.begin(); iter != buildingSprites.end();)
	{
		if(hgPlayer->getPosX() - 200 > (*iter)->getPosX())
		{
			(*iter)->removeFromParentAndCleanup(true);
			iter = buildingSprites.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	for(auto iter = tuningPoints.begin(); iter != tuningPoints.end();)
	{
		if(hgPlayer->getPosX() - 200 > iter->bottom.x) // 아래쪽 기준.
		{
			iter = tuningPoints.erase(iter);
		}
		else
			++iter;
	}
	
	for(auto iter = middleObsSprites.begin(); iter != middleObsSprites.end();)
	{
		if(hgPlayer->getPosX() - 200 > (*iter)->getPosX())
		{
			(*iter)->removeFromParentAndCleanup(true);
			iter = middleObsSprites.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	
	for(auto iter = topObsSprites.begin(); iter != topObsSprites.end();)
	{
		if(hgPlayer->getPosX() - 200 > (*iter)->getPosX())
		{
			(*iter)->removeFromParentAndCleanup(true);
			iter = topObsSprites.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	
	for(auto iter = coinSprites.begin(); iter != coinSprites.end(); )
	{
		if(hgPlayer->getPosX() - 200 > (*iter)->getPosX())
		{
			(*iter)->removeFromParentAndCleanup(true);
			iter = coinSprites.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void HGGameLayer::checkEatCoin()
{
	// 플레이어와 코인과 충돌 처리함.
	//hgPlayer->setScale(0.1f);
	CCRect b = hgPlayer->boundingBox();
	//hgPlayer->setScale(1.f);
	bool crashed = false;
	HGCoin* coinSprite = 0;
	for_each(coinSprites.begin(), coinSprites.end(), [&](HGCoin* coin)
			 {
				 CCRect a = coin->boundingBox();
				 if(this->AABB(a, b) /*&& hgPlayer->STATE == HGPlayer::NORMAL*/ && coin->getIsEat() == false)
				 {
					 coin->setIsEat(true);
					 //					 hgPlayer->damaged();
					 crashed = true;
					 coinSprite = coin;
					 return;
					 //					 hgPlayer->setColor(ccc3(255, 0, 0));
				 }
				 
			 });
	if(crashed)
	{
		//coinSprite
		CCNotificationCenter::sharedNotificationCenter()->postNotification("onHGEatCoin");
		ccBezierConfig bc;
		bc.controlPoint_1 = coinSprite->getPosition();
		bc.controlPoint_2 = ccp(200 + rand()%50, 50);
		bc.endPosition = ccp(470, 221);
		float i = 0.f;
		auto moveAction = CCSequence::create(CCDelayTime::create(i * 0.2f),
											 KSFunc(KSoundEngine::sharedEngine()->playSound("se_takegold01.mp3");),
											 CCMoveTo::create(1.2, bc.endPosition), 0);
		auto action2 = CCSpawn::create(moveAction, CCScaleTo::create(0.8f, 0.5f)/*, CCFadeOut::create(1.2f)*/, 0); // 가면서 사라짐.
		auto action3 = CCCallFuncN::create(this, callfuncN_selector(HGGameLayer::deleteSprite));
		auto totalAction = CCSequence::create(action2, action3, 0);
		coinSprite->runAction(totalAction);
		coinSprites.erase(std::remove(coinSprites.begin(), coinSprites.end(), coinSprite), coinSprites.end());
		//		CCNotificationCenter::sharedNotificationCenter()->postNotification("onHGCrash");
		return;
	}
}
void HGGameLayer::render()
{
	float diff = lu_call<float>("CHAR_POSITION") - hgPlayer->getPosX();
	hgPlayer->setPosition(ccp(lu_call<float>("CHAR_POSITION"), hgPlayer->getPosY()));
	this->setPositionY(-hgPlayer->getPosY() + 160);
	for(auto iter = buildingSprites.begin(); iter != buildingSprites.end(); ++iter)
	{
		HGBuilding* bd = (*iter);
		if(bd->getCollapse()) // 붕괴 되고 있으면 ...
			bd->setPosY( bd->getPosY() - 1.f );
		bd->setPosition(ccp(bd->getPosX() + diff, bd->getPosY()));
	}
	
	for(auto iter = middleObsSprites.begin(); iter != middleObsSprites.end(); ++iter)
	{
		HGMiddleObs* bd = (*iter);
		bd->setPosition(ccp(bd->getPosX() + diff, bd->getPosY()));
	}
	
	for(auto iter = topObsSprites.begin(); iter != topObsSprites.end(); ++iter)
	{
		HGTopObs* bd = (*iter);
		bd->setPosition(ccp(bd->getPosX() + diff, bd->getPosY()));
	}
	
	for(auto i : coinSprites)
	{
		if(i->getIsEat() == false)
			i->setPosition(ccp(i->getPosX() + diff, i->getPosY()));
	}
}

void HGGameLayer::upDownProcess()
{
	if(isStarted)
	{
		if(isUp)
		{
			vertVelocity += 8.f * 1.f / 60.f; // 8
			vertVelocity = MIN(vertVelocity, 10.f);
//			hgPlayer->setRotation(-5.f);
			//		CCLog("up %f", vertVelocity);
		}
		else
		{
			
			vertVelocity -= 8.f * 1.f / 60.f; //
			
			vertVelocity = MAX(vertVelocity, -10.f);
//			hgPlayer->setRotation(5.f);
			//		CCLog("down %f", vertVelocity);
		}
		
		hgPlayer->setRotation(-vertVelocity * 2.f * 2.f);
		//setRotation(-vertVelocity * 2.f);// * 2.f);
		float pos = hgPlayer->getPosY() + vertVelocity;
		float upLimit = 300.f;
		float downLimit = 20.f;
		float realPos = MIN(upLimit, pos);
		realPos = MAX(downLimit, realPos);
		
		if(realPos >= upLimit || realPos <= downLimit)
		{
			vertVelocity = 0;
		}
		hgPlayer->setPosY(realPos);
		
	}
	//hgPlayer->setPosY(hgPlayer->getPosY() + vertVelocity);
}
bool HGGameLayer::init()
{
	CCLayer::init();
	
	initTurningPoint();
	//	setScale(0.3f);
	this->setTouchEnabled(true);
	
	hgPlayer = HGPlayer::create();
	addChild(hgPlayer, 4);
	scheduleUpdate();
	
	
	{
		voidNode = CCParallaxNode::create();
		addChild(voidNode);
		
		CCSprite* b1 = CCSprite::create("hg_backtower.png");
		b1->setAnchorPoint(ccp(0,0 ));
		b1->setPosition(ccp(0,0));
		
		CCSprite* b2 = CCSprite::create("hg_backtower.png");
		b2->setAnchorPoint(ccp(0,0 ));
		b2->setPosition(ccp(0,0));
		voidNode->addChild(b1, 1,
						   ccp(1,0), ccp(0,0));
		voidNode->addChild(b2, 1,
						   ccp(1,0), ccp(480,0));
		
	}
	//	{
	//		CCParallaxNode* voidNode = CCParallaxNode::create();
	//		addChild(voidNode, 3);
	//
	//		CCSprite* b1 = CCSprite::create("hg_topcloud.png");
	//		b1->setAnchorPoint(ccp(0,0 ));
	//		b1->setPosition(ccp(0,0));
	//
	//		CCSprite* b2 = CCSprite::create("hg_topcloud.png");
	//		b2->setAnchorPoint(ccp(0,0 ));
	//		b2->setPosition(ccp(0,0));
	//
	//		voidNode->addChild(b1, 1, ccp(0.3f, 0), ccp(0, 290));
	//		voidNode->addChild(b2, 1, ccp(0.3f, 0), ccp(480, 290));
	//		auto a1 = CCMoveBy::create(8, ccp(-480 * (1.f/0.3f), 0));
	//		auto a2 = KSFuncN(node, {
	//			node->setPosition(ccp(0, 0));
	//		});
	//		auto a3 = CCRepeatForever::create(CCSequence::createWithTwoActions(a1, a2));
	//		voidNode->runAction(a3);
	//
	//	}
	
	
	
	return true;
}
void HGGameLayer::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}
void HGGameLayer::onExit()
{
	
	
	CCLayer::onExit();
}

//
//  SKObstacleLayer.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 5. 10..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "SKObstacleLayer.h"
#include "SKCharacter.h"
#include "SKGameScene.h"
#include "KS_Util.h"
#include "BS2Geometry.h"
#include "AKAngryGameScene.h"
#include "SceneUtil.h"
#include "LuaManager.h"
#include "lua_tinker.h"

int SKObstacleLayer::BASE_POSITION = 240;
int SKObstacleLayer::FAR_H = 218; //220
int SKObstacleLayer::CLOSE_SIZE = 480;
int SKObstacleLayer::FAR_SIZE = 300;
int SKObstacleLayer::VIEW_VALUE = 600;
float SKObstacleLayer::EYE_HEIGHT = 60.f;

static lua_State* L;
// 장애물 삭제시, obstacles 에서 원하는 객체 빼내고, removeFromParent 함.
SKObstacleLayer::SKObstacleLayer()
{
//	D.resize(600);
	L = LuaManager::shared()->getLua("sk.lua");
	D[0] = 0;
	const float horiz = 35.f;
	for(int i=1; i<=1000; i++)
	{
		D[i] = (D[i-1] + 1 - atan(i/horiz) * 2.f / M_PI);
	}
	
	for(int i=-1; i>= -1000; i--)
	{
		D[i] = D[i + 1] - 1 + atan(i/horiz) * 2.f / M_PI;
	}
	
	backZOrder = 1000;

}
SKObstacleLayer::~SKObstacleLayer()
{
	//	
}
void SKObstacleLayer::registerWithTouchDispatcher()
{
	//	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}
bool SKObstacleLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = pTouch;
	
	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	
	return true;
}
bool SKObstacleLayer::init()
{
	CCLayer::init();
	CCSprite* backtop = CCSprite::create("sk_backtop.png");
	backtop->setAnchorPoint(ccp(0.5, 1.f));
	backtop->setPosition(ccp(240, 320));
	addChild(backtop, 0);
	
	CCSprite* backbottom = CCSprite::create("sk_backbottom.png");
	backbottom->setAnchorPoint(ccp(0.5, 0.f));
	backbottom->setPosition(ccp(240, 0));
	addChild(backbottom, 1001);
	//	this->setTouchEnabled(true);
	
	CCParticleSystem* backSnow = CCParticleSystem::create("snow.plist");
	addChild(backSnow, INT_MAX);
	backSnow->setPosition(ccp(240, 320));
	
	{
		CCParticleSystem* backSnow = CCParticleSystem::create("snow.plist");
		backSnow->setStartSize(5.f);
		backSnow->setEndSize(3.f);
		addChild(backSnow, 0);
		backSnow->setPosition(ccp(240, 320));
	}
	
	focusFlagSprite = CCSprite::create("sk_arrow.png");
	focusFlagSprite->runAction(
							   CCRepeatForever::create( CCSequence::createWithTwoActions(CCEaseOut::create(CCMoveBy::create(0.8, ccp(0, 5)), 1.3),
																						   CCEaseOut::create(CCMoveBy::create(0.8, ccp(0, -5)), 1.3))) );
	addChild(focusFlagSprite, 2);
	return true;
}

SKCharacter::CRASHTYPE SKObstacleLayer::crashLine(CCPoint A, CCPoint B, CCPoint C, CCPoint D, SKFlag* flag)
{
	bool interBool = ccpSegmentIntersect(A, B, C, D);
	if(interBool)
	{
		CCPoint interPoint = ccpIntersectPoint(A, B, C, D);
		float obsx = flag->getObsX();
		flag->setColor(ccc3(255, 0, 0));
		if(obsx >= interPoint.x) // 오른발 부딪힘
		{
			return SKCharacter::CRASHTYPE::RIGHT_CRASH;
		}
		else // 왼발 부딪힘
		{
			return SKCharacter::CRASHTYPE::LEFT_CRASH;
		}
		
	}
	
	return SKCharacter::CRASHTYPE::NO_CRASH;
}
SKCharacter::CRASHTYPE SKObstacleLayer::checkCrashFlag()
{
	return SKCharacter::CRASHTYPE::NO_CRASH; // 깃발 충돌 제거
	if(skier->PLAYERSTATE != SKCharacter::NORMAL)
	{
		return SKCharacter::CRASHTYPE::NO_CRASH;
	}
	
	for(auto iter = flags.begin(); iter != flags.end(); ++iter)
	{
		// A : 장애물 왼쪽 위치
		// B : 장애물 오른쪽 위치
		// C-D : 플레이어의 이동하는 자취
		
		auto firstObject = (*iter).first;
		auto secondObject = (*iter).second;
		SKCharacter::CRASHTYPE crashType = SKCharacter::CRASHTYPE::NO_CRASH;
		{
			CCPoint A = ccp(firstObject->getObsX() - firstObject->getObsSize() / 2.f - (lua_tinker::get<float>(L, "PLAYERWIDTH") / 2.f), firstObject->getDistance());
			CCPoint B = ccp(firstObject->getObsX() + firstObject->getObsSize() / 2.f + (lua_tinker::get<float>(L, "PLAYERWIDTH") / 2.f), firstObject->getDistance());
			CCPoint C = ccp(skier->getPrevX(), skier->getPrevDistance()); // 캐릭터의 이전 위치
			CCPoint D = ccp(skier->getX(), skier->getDistance()); // 캐릭터의 현재 위치.
			crashType = crashLine(A, B, C, D, firstObject);
		}
		if(crashType == SKCharacter::CRASHTYPE::NO_CRASH)
		{
			CCPoint A = ccp(secondObject->getObsX() - secondObject->getObsSize() / 2.f - (lua_tinker::get<float>(L, "PLAYERWIDTH") / 2.f), secondObject->getDistance());
			CCPoint B = ccp(secondObject->getObsX() + secondObject->getObsSize() / 2.f + (lua_tinker::get<float>(L, "PLAYERWIDTH") / 2.f), secondObject->getDistance());
			CCPoint C = ccp(skier->getPrevX(), skier->getPrevDistance()); // 캐릭터의 이전 위치
			CCPoint D = ccp(skier->getX(), skier->getDistance()); // 캐릭터의 현재 위치.
			crashType = crashLine(A, B, C, D, secondObject);
		}
		if(crashType != SKCharacter::CRASHTYPE::NO_CRASH)
			return crashType;
		
	}
	return SKCharacter::CRASHTYPE::NO_CRASH;
}
SKCharacter::CRASHTYPE SKObstacleLayer::checkCrash()
{
//	float currentPosition = skier->getX();
//	float currentDistance = skier->getDistance();
//	bool isCrash = false;
	if(skier->PLAYERSTATE != SKCharacter::NORMAL)
	{
		return SKCharacter::CRASHTYPE::NO_CRASH;
	}
	for(auto iter = obstacles.begin(); iter != obstacles.end(); ++iter)
	{
		SKObstacle* obs = *iter;
		if(obs->deco == true)
			continue;
		// A : 장애물 왼쪽 위치
		// B : 장애물 오른쪽 위치
		// C-D : 플레이어의 이동하는 자취
		CCPoint A = ccp(obs->getObsX() - obs->getObsSize() / 2.f - (lua_tinker::get<float>(L, "PLAYERWIDTH") / 2.f), obs->getDistance());
		CCPoint B = ccp(obs->getObsX() + obs->getObsSize() / 2.f + (lua_tinker::get<float>(L, "PLAYERWIDTH") / 2.f), obs->getDistance());
		CCPoint C = ccp(skier->getPrevX(), skier->getPrevDistance()); // 캐릭터의 이전 위치
		CCPoint D = ccp(skier->getX(), skier->getDistance()); // 캐릭터의 현재 위치.
		
		bool interBool = ccpSegmentIntersect(A, B, C, D);
		if(interBool)
		{
			CCPoint interPoint = ccpIntersectPoint(A, B, C, D);
			float obsx = (*iter)->getObsX();
			obs->setColor(ccc3(255, 0, 0));
			
			if(dynamic_cast<SKTree*>(obs))
			{
				float skierHeight = 100.f;
				CCPoint skierPos = skier->getPosition();
				CCParticleSystem* headSnow = CCParticleSystem::create("treebreak.plist");
				addChild(headSnow, skier->getZOrder());
				headSnow->setPosition(ccp(skierPos.x, skierPos.y + skierHeight));
				
				auto animator = SceneUtil::playAnimation("ak_dish_bomb.png", 0.09, AngryGameScene::DISH_BROKEN_FRAME, AngryGameScene::DISH_BROKEN_FRAME,
														 AngryGameScene::DISH_BROKEN_WIDTH, AngryGameScene::DISH_BROKEN_HEIGHT, false);
				animator.second->runAction(CCSequence::createWithTwoActions(animator.first, CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite))));
				
				
				animator.second->setPosition(ccp(skierPos.x, skierPos.y + skierHeight));
				//addChild(animator.second, skier->getZOrder()-1);
				addChild(animator.second, 1002);
				if(obsx >= interPoint.x) // 오른발 부딪힘
				{
					return SKCharacter::CRASHTYPE::RIGHT_CRASH;
				}
				else // 왼발 부딪힘
				{
					return SKCharacter::CRASHTYPE::LEFT_CRASH;
				}
			}
			else
			{
				float skierHeight = 100.f;
				CCPoint skierPos = skier->getPosition();
				auto animator = SceneUtil::playAnimation("ak_dish_bomb.png", 0.09, AngryGameScene::DISH_BROKEN_FRAME, AngryGameScene::DISH_BROKEN_FRAME,
														 AngryGameScene::DISH_BROKEN_WIDTH, AngryGameScene::DISH_BROKEN_HEIGHT, false);
				animator.second->runAction(CCSequence::createWithTwoActions(animator.first, CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite))));
				
				
				
				addChild(animator.second, 1002);
				
				float legLength = 30.f;
				if(obsx >= interPoint.x) // 오른발 부딪힘
				{
					animator.second->setPosition(ccp(skierPos.x + legLength, skierPos.y + 30.f));
					return SKCharacter::CRASHTYPE::RIGHT_CRASH;
				}
				else // 왼발 부딪힘
				{
					
					animator.second->setPosition(ccp(skierPos.x - legLength, skierPos.y + 30.f));
					return SKCharacter::CRASHTYPE::LEFT_CRASH;
				}
			}
		}
	}
	return SKCharacter::CRASHTYPE::NO_CRASH;
}

void SKObstacleLayer::createFlag(float currentPosition, float currentDistance)
{
	static bool plusSign = true;
	plusSign = !plusSign;
	int sign;
	if(plusSign)
		sign = 1;
	else sign = -1;//= KS_Util::plus_or_minus();
	float X = currentPosition +  sign * KS_Util::beetween(lua_tinker::get<float>(L, "flagXMinDistance"),
														  lua_tinker::get<float>(L, "flagXMaxDistance"));
	float Y = currentDistance + 1000.f; //#currentDistance + 600.f
	bool useNaiveItem = playInfo->selectedItems["item_skwide1.png"];
	int multi = useNaiveItem ? 1.2f : 1.f;
	float X1 = X - lua_tinker::get<float>(L, "flagGap") * multi;
	float X2 = X + lua_tinker::get<float>(L, "flagGap") * multi;
	
	int z = getBackzOrder();
	pair<SKFlag*, SKFlag*> pairFlag;
	{
		SKFlag* skflag = new SKFlag;
		skflag->right = false;
		skflag->init();
		skflag->autorelease();
		skflag->createSprite();
		skflag->setDistance(Y);
		skflag->setObsX(X1);
		pairFlag.first = skflag;
//		int order = SKObstacle::myOrder - skflag->getDistance();
//		if(order < 0)
//		{
//			order = SKObstacle::myOrder - (-order) % SKObstacle::myOrder; // -1 같은 경우에 50000 으로 만듬.
//		}
		addChild(skflag, z);
	}
	{
		SKFlag* skflag = new SKFlag;
		skflag->right = true;
		skflag->init();
		skflag->autorelease();
		skflag->createSprite();
		skflag->setDistance(Y);
		skflag->setObsX(X2);
		pairFlag.second = skflag;
//		int order = SKObstacle::myOrder - skflag->getDistance();
//		if(order < 0)
//		{
//			order = SKObstacle::myOrder - (-order) % SKObstacle::myOrder; // -1 같은 경우에 50000 으로 만듬.
//		}
		addChild(skflag, z);
	}
	flags.push_back(pairFlag);
}

void SKObstacleLayer::focusFlag()
{
	if(flags.empty() == false)
	{
		pair<SKFlag*, SKFlag*> flagPair = flags.back();
		
		float x = (flagPair.first->getPosition().x + flagPair.second->getPosition().x) / 2.f;
		
		if(skier->getDistance() < flagPair.first->getDistance())
		{
			focusFlagSprite->setVisible(true);
			focusFlagSprite->setPosition(ccp(x, 255));
		}
		else
		{
			focusFlagSprite->setVisible(false);
		}
	}
	else
	{
		focusFlagSprite->setVisible(false);
	}
	
	
}
void SKObstacleLayer::createObstacle(float currentPosition, float currentDistance, SKObstacle* obs, int currentMove)
{
	obs->createSprite();
	float adder = 0;
	if(currentMove == SKCharacter::RIGHT)
		adder += 800; //#300
	else if(currentMove == SKCharacter::LEFT)
	{
		adder -= 800;//#300
	}	
	
	obs->setDistance(currentDistance + 1000.f); // + 270 //# 600.f
	float _01 = KS_Util::rnd();
	int sign = KS_Util::plus_or_minus();
	
	// 철조망이 가운데 오지 않도록 함.
	if(dynamic_cast<SKPense*>(obs))
	{
		obs->setObsX(currentPosition + (170 + 50 * _01) * sign ) ;
	}
	else
	{
		obs->setObsX(currentPosition + adder); //# currentPosition + 350.f * _01 * sign + adder
	}
	
	obstacles.push_back(obs);
//	int order = SKObstacle::myOrder - obs->getDistance();
//	if(order < 0)
//	{
//		order = SKObstacle::myOrder - (-order) % SKObstacle::myOrder; // -1 같은 경우에 50000 으로 만듬.
//	}
	addChild(obs, getBackzOrder());
}
void SKObstacleLayer::createGround(float currentDistance)
{
	SKGround* obs = new SKGround;
	obs->init();
	obs->autorelease();
	
	obs->createSprite();
	obs->obsDistance = currentDistance + 500;
	obs->setAnchorPoint(ccp(0.5, 0.f));
	grounds.push_back(obs);
	addChild(obs, 1002);
}
void SKObstacleLayer::destroyObstacle(float currentDistance)
{
	for(auto iter = obstacles.begin(); iter != obstacles.end();)
	{
		
		if(currentDistance > (*iter)->getDistance() + 50.f)
		{
			(*iter)->removeFromParentAndCleanup(true);
			iter = obstacles.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void SKObstacleLayer::destroyGround(float currentDistance)
{
	for(auto iter = grounds.begin(); iter != grounds.end();)
	{
		if(currentDistance > (*iter)->obsDistance + 150.f)
		{
			(*iter)->removeFromParentAndCleanup(true);
			iter = grounds.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void SKObstacleLayer::destroyFlag(float currentDistance)
{
	for(auto iter = flags.begin(); iter != flags.end();)
	{
		auto firstObject = (*iter).first;
		auto secondObject = (*iter).second;
		if(currentDistance > (*iter).first->getDistance() + 150.f)
		{
			firstObject->removeFromParentAndCleanup(true);
			secondObject->removeFromParentAndCleanup(true);
			iter = flags.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
void SKObstacleLayer::render(float currentPosition, float currentDistance)
{
	renderObs(currentPosition, currentDistance);
	renderGround(currentPosition, currentDistance);
	renderPlayer(currentPosition, currentDistance);
	renderFlag(currentPosition, currentDistance);
	
	
}
void SKObstacleLayer::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}
void SKObstacleLayer::onExit()
{
	
	
	CCLayer::onExit();
}
SKObstacleLayer::FLAGPASSTYPE SKObstacleLayer::checkFlagCha(int& chaCount)
{
	int cnt = 0;
	for(auto iter = flags.begin(); iter != flags.end(); ++iter)
	{
		auto firstObject = (*iter).first;
		auto secondObject = (*iter).second;

		//엄격한 판정.
//		CCPoint A = ccp(firstObject->getObsX() + firstObject->getObsSize() / 2.f + (SKGameScene::PLAYERWIDTH / 2.f), firstObject->getDistance());
//		CCPoint B = ccp(secondObject->getObsX() - secondObject->getObsSize() / 2.f - (SKGameScene::PLAYERWIDTH / 2.f), secondObject->getDistance());
		
		// 쉬운 판정. 바깥 판정.
		CCPoint A = ccp(firstObject->getObsX() - firstObject->getObsSize() / 2.f + (lua_tinker::get<float>(L, "PLAYERWIDTH") / 2.f), firstObject->getDistance());
		CCPoint B = ccp(secondObject->getObsX() + secondObject->getObsSize() / 2.f - (lua_tinker::get<float>(L, "PLAYERWIDTH") / 2.f), secondObject->getDistance());
		
		// 판정할 대상이라면
		if(skier->getDistance() > firstObject->getDistance() && firstObject->getIsJudged() == false)
		{
			firstObject->setJudged(true); // 판정 됨.
			
			if(skier->PLAYERSTATE == SKCharacter::NORMAL)
			{
//				float minDistance = MIN(fabsf(A.x - skier->getX()), fabsf(B.x - skier->getX()) );
//				// minDistance 여부로 판정함.
//				if(minDistance <= CHA_CRITERION)
//				{
//					cnt++;
//				}
				
				if(A.x <= skier->getX() && skier->getX() <= B.x)
				{
					cnt++;
				}
			}
//			CCLog("judge value %f", minDistance);
		}
	}
	if(cnt > 0)
	{
		chaCount = cnt;
		return FLAGPASSTYPE::FLAG_CHA;
	}
	else
	{
		chaCount = 0;
		return FLAGPASSTYPE::FLAG_NOCHA;
	}
}
SKObstacleLayer::OBSPASSTYPE SKObstacleLayer::checkObsCha(int& chaCount)
{
	int cnt = 0;
	for(auto iter = obstacles.begin(); iter != obstacles.end(); ++iter)
	{
		if((*iter)->deco == true)
			continue;
		CCPoint A = ccp((*iter)->getObsX() - (*iter)->getObsSize() / 2.f - (lua_tinker::get<float>(L, "PLAYERWIDTH") / 2.f), (*iter)->getDistance());
		CCPoint B = ccp((*iter)->getObsX() + (*iter)->getObsSize() / 2.f + (lua_tinker::get<float>(L, "PLAYERWIDTH") / 2.f), (*iter)->getDistance());
		
		
		// 판정할 대상이라면
		if(skier->getDistance() > (*iter)->getDistance() && (*iter)->getIsJudged() == false)
		{
			(*iter)->setJudged(true); // 판정 됨.
			float minDistance = MIN(fabsf(A.x - skier->getX()), fabsf(B.x - skier->getX()) );
			if(skier->PLAYERSTATE == SKCharacter::NORMAL)
			{
				// minDistance 여부로 판정함.
				//if(minDistance <= CHA_CRITERION)
				{
					cnt++;
				}
			}
//			CCLog("judge value %f", minDistance);
		}
	}
	if(cnt > 0)
	{
		chaCount = cnt;
		return OBSPASSTYPE::OBS_CHA;
	}
	else
	{
		chaCount = 0;
		return OBSPASSTYPE::OBS_NOCHA;
	}
	
}

void SKObstacleLayer::renderGround(float currentPosition, float currentDistance)
{
	if(grounds.empty() == false)
	{
		for(auto iter = grounds.begin(); iter != grounds.end() - 1; ++iter)
		{
			float d = (*iter)->obsDistance - currentDistance;
			float fd = projection(d) + SKCharacter::CHAR_BASEY;
			
			auto nextIter = iter;
			++nextIter;
			float nextD = (*nextIter)->obsDistance - currentDistance;
			float nextFD = projection(nextD) + SKCharacter::CHAR_BASEY;
			
			if(fd > FAR_H)
			{
				(*iter)->setVisible(false);
			}
			else
			{
				(*iter)->setVisible(true);
				(*iter)->setScaleY((nextFD - fd));
				
				float decoGroundY = (*iter)->getPosition().y + (*iter)->getScaleY();
				float horizonal = 220; // 지평선 y 위치.
				if(decoGroundY > horizonal)
				{
					(*iter)->setScaleY(horizonal - (*iter)->getPosition().y);
				}
			}
			
			(*iter)->setPosition(ccp(240, fd));
			
			
		}
	}
}
void SKObstacleLayer::renderObs(float currentPosition, float currentDistance)
{
	float Y = CLOSE_SIZE * FAR_H / (CLOSE_SIZE - FAR_H);
	
	// 현재가로위치와 현재 어디까지 진행 위치를 고려하여 장애물들의 위치를 세팅함.
	for(auto iter = obstacles.begin(); iter != obstacles.end(); ++iter)
	{
		float d = (*iter)->getDistance() - currentDistance;
		float fd = projection(d) + SKCharacter::CHAR_BASEY;
		float obsX = (*iter)->getObsX() - currentPosition;
		
		bool topPosition = false;
		if(fd > FAR_H)
		{
			(*iter)->setVisible(true);
			topPosition = true;
		}
		//		else
		{
			if(currentDistance > (*iter)->getDistance() )
			{
				(*iter)->setOpacity(150);
			}
			
			
			(*iter)->setVisible(true);
			float maxDiff = FAR_H;
			//			CCLog("scale  %f", powf(1.3f - (fd / maxDiff), 0.4f));
			
			//CCLog("scale : %f",d);
			//(*iter)->setScale(1.f-(d/600.f));
			
			(*iter)->setScale(MAX(0.f, powf(1.3f - (fd / maxDiff), 0.4f) ) * 0.7f);
			//##(*iter)->setScale(MAX(0.f, powf(1.3f - (fd / maxDiff), 0.6f) ));
			if(d <= 0)
			{
				//				(*iter)->setColor(ccc3(255, 0, 0));
				reorderChild(skier, (*iter)->getZOrder()-1);
			}
			//			(*iter)->setScale(MAX(0.15f, pow(1.0f - (fd / maxDiff), 0.85f) ));
		}
		float x = (Y - fd) * CLOSE_SIZE / (Y); // 장애물의 밑변.
		float resultX = obsX * x / CLOSE_SIZE;// / CLOSE_SIZE;//(FAR_H - resultY)/(FAR_H);
		if(topPosition)
		{
			(*iter)->setPosition(ccp(resultX + BASE_POSITION, (FAR_H-(fd-FAR_H)))); //@ Y : FAR_H - SKObstacle::JUDGE_HEIGHT
		}
		else
		{
			(*iter)->setPosition(ccp(resultX + BASE_POSITION, fd - SKObstacle::JUDGE_HEIGHT));
            if((*iter)->getReordered() == false)
            {
                (*iter)->setReordered(true);
                int order = SKObstacle::myOrder - (*iter)->getDistance();//@ ADD
                if(order < 0)
                {
                    order = SKObstacle::myOrder - (-order) % SKObstacle::myOrder; // -1 같은 경우에 50000 으로 만듬.
                }
                reorderChild(*iter, order);//@ADD
            }
			
		}
		
	}
}
void SKObstacleLayer::renderFlag(float currentPosition, float currentDistance)
{
	float Y = CLOSE_SIZE * FAR_H / (CLOSE_SIZE - FAR_H);
	
	// 현재가로위치와 현재 어디까지 진행 위치를 고려하여 장애물들의 위치를 세팅함.
	for(auto iter = flags.begin(); iter != flags.end(); ++iter)
	{
		auto firstObject = (*iter).first;
		auto secondObject = (*iter).second;
		
		// firstObject 기준으로 배치함. 어차피 일직선상에 있을테니까.
		float d = firstObject->getDistance() - currentDistance;
		float fd = projection(d) + SKCharacter::CHAR_BASEY;
		float obsX1 = firstObject->getObsX() - currentPosition;
		float obsX2 = secondObject->getObsX() - currentPosition;
		bool topPosition = false;
		if(fd > FAR_H)
		{
			firstObject->setVisible(true);
			secondObject->setVisible(true);
			topPosition = true;
		}
		//		else
		{
			if(currentDistance > firstObject->getDistance() )
			{
				firstObject->setOpacity(150);
				secondObject->setOpacity(150);
			}
			
			
			firstObject->setVisible(true);
			secondObject->setVisible(true);
			float maxDiff = FAR_H;
			//			CCLog("scale  %f", powf(1.3f - (fd / maxDiff), 0.4f));
			//#firstObject->setScale(MAX(0.f, powf(1.2f - (fd / maxDiff), 0.6f) ));
			//#secondObject->setScale(MAX(0.f, powf(1.2f - (fd / maxDiff), 0.6f) ));
			firstObject->setScale(MAX(0.f, powf(1.3f - (fd / maxDiff), 0.4f) ) * 0.7f);
			secondObject->setScale(MAX(0.f, powf(1.3f - (fd / maxDiff), 0.4f) ) * 0.7f);
			
			//CCLog("scale  %f, fd = %f", (*iter)->getScale(), fd);
			if(d <= 0)
			{
				//				(*iter)->setColor(ccc3(255, 0, 0));
				reorderChild(skier, firstObject->getZOrder()-1);
				reorderChild(skier, secondObject->getZOrder()-1);
			}
			//			(*iter)->setScale(MAX(0.15f, pow(1.0f - (fd / maxDiff), 0.85f) ));
		}
		float x = (Y - fd) * CLOSE_SIZE / (Y); // 장애물의 밑변.
		float resultX1 = obsX1 * x / CLOSE_SIZE;// / CLOSE_SIZE;//(FAR_H - resultY)/(FAR_H);
		float resultX2 = obsX2 * x / CLOSE_SIZE;// / CLOSE_SIZE;//(FAR_H - resultY)/(FAR_H);
		if(topPosition)
		{
			firstObject->setPosition(ccp(resultX1 + BASE_POSITION,(FAR_H-(fd-FAR_H)) )); //@ Y:FAR_H - SKObstacle::JUDGE_HEIGHT
			secondObject->setPosition(ccp(resultX2 + BASE_POSITION, (FAR_H-(fd-FAR_H))));//@ Y:FAR_H - SKObstacle::JUDGE_HEIGHT
		}
		else
		{
			firstObject->setPosition(ccp(resultX1 + BASE_POSITION, fd - SKObstacle::JUDGE_HEIGHT));
			secondObject->setPosition(ccp(resultX2 + BASE_POSITION, fd - SKObstacle::JUDGE_HEIGHT));
            
            if(firstObject->getReordered() == false)
            {
                firstObject->setReordered(true);
                int order = SKObstacle::myOrder - firstObject->getDistance();//@ ADD
                if(order < 0)
                {
                    order = SKObstacle::myOrder - (-order) % SKObstacle::myOrder; // -1 같은 경우에 50000 으로 만듬.
                }
                reorderChild(firstObject, order);//@ADD
                reorderChild(secondObject, order);//@ADD
            }
		}
	}
}

void SKObstacleLayer::renderPlayer(float currentPosition, float currentDistance)
{
	// Player 의 위치에 따른 스케일 조정.
	float d = skier->getPosition().y;
	float maxDiff = FAR_H;
	float fd = projection(d) + SKCharacter::CHAR_BASEY;
	skier->setScale(MAX(0.f, powf(1.2 - (fd / maxDiff), 0.6f) ));
}
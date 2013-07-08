//
//  JoinMembers.cpp
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 5. 31..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#include "JoinMembers.h"

#include "NSDefault.h"
#include "KSBlock.h"
#include "Bridge.h"
#include "KS_Util.h"
#include <cmath>
JoinMembers::JoinMembers()
{
	timer = 0;
	theta = wellRand.GetFloatValue(2 * M_PI);
	progressUsers = 0;
	allUsers = 100;
	allUserFnt = 0;
	isStarted = false;
}
JoinMembers::~JoinMembers()
{
	
}
void JoinMembers::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}
bool JoinMembers::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = pTouch;

	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	
	return true;
}
bool JoinMembers::init()
{
	CCLayer::init();
		
	this->setTouchEnabled(true);
	schedule(schedule_selector(JoinMembers::update));
	goalPoint = ccp(240, 160);
	
	Member* findingUser = new Member();
	findingUser->initWithFile("search_finding.png");
	findingUser->autorelease();
	findingUser->setPosition(ccp(240, 225 + 20.f));
	findingUser->goalPoint = findingUser->getPosition();
	findingUser->startPoint = findingUser->getPosition();
	findingUser->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(CCFadeOut::create(1.f),
																					  CCFadeIn
																					  ::create(1.f))));
//	findingUser->isGoal = true;
	addChild(findingUser);
	members.push_back(findingUser);
	
	allUser = new Member();
	allUser->initWithFile("search_user.png");
	allUser->setPosition(ccp(240, 176  + 20.f));
	allUser->goalPoint = allUser->getPosition();
	allUser->startPoint = allUser->getPosition();
	addChild(allUser);
//	allUser->isGoal = true;
	members.push_back(allUser);
	
	Member* me = new Member();
	me->initWithFile("search_me.png");
	
	me->autorelease();
//	me->setScale(2.f);
//	me->isGoal = true;
	me->setPosition(ccp(240, 145  + 20.f));
	me->goalPoint = me->getPosition();
	me->startPoint = me->getPosition();
	addChild(me);
	members.push_back(me);
	
	CCSprite* flag = CCSprite::createWithSpriteFrameName((NSDefault::getCountry() + ".png").c_str());
	flag->setPosition(ccp(28, 22));
	me->addChild(flag);
	
	CCLabelTTF* userName = CCLabelTTF::create(NSDefault::getUserName().c_str(), defaultFont, 13.f);
	userName->setColor(ccc3(0,0,0));
	me->addChild(userName);
	userName->setAnchorPoint(ccp(0.f, 0.5f));
	userName->setPosition(ccp(55, 21));
	
	Member* progressBack = new Member();
	progressBack->initWithFile("search_back.png");
	progressBack->autorelease();
	progressBack->setPosition(ccp(240, 202  + 20.f));
	progressBack->startPoint = progressBack->goalPoint = progressBack->getPosition();
	addChild(progressBack);
	
	members.push_back(progressBack);
	
	progress = CCProgressTimer::create(CCSprite::create("search_front.png"));
	progress->setBarChangeRate(ccp(1, 0));
	progress->setMidpoint(ccp(0, 0.5f));
	progress->setType(CCProgressTimerType::kCCProgressTimerTypeBar);
	progress->setPosition(ccp(240, 202  + 20.f));
	progress->setPercentage(0.f);
	addChild(progress);
	
	return true;
}
void JoinMembers::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}
void JoinMembers::onExit()
{
	
	
	CCLayer::onExit();
}
void JoinMembers::update(float dt)
{
	timer += 1.f/60.f;
	
	moveMembersProcess();
}

bool JoinMembers::AABB(CCRect a, CCRect b)
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
void JoinMembers::moveMembersProcess()
{
	// 빠르게 넘어가기 위해 강제 시간 세팅.
//	if(timer > 3.f && isStarted == false)
//	{
//		isStarted = true;
//		CCNotificationCenter::sharedNotificationCenter()->postNotification("finishStartScores2");
//		return;
//	}
	
	for(auto iter = members.begin(); iter != members.end(); ++iter)
	{
		Member* t = *iter;
//		CCLog("%f %f", t->movingTime, timer);
		if(t->movingTime < timer)
		{
			if(onceMove[t] == false)
			{
				onceMove[t] = true;
				progressUsers++;
				if(allUserFnt)
					allUserFnt->setString(KS_Util::stringWithFormat("%d", progressUsers).c_str());
				progress->setPercentage((float)progressUsers / (float)allUsers * 100.f);
				
				if(progressUsers == allUsers && isStarted == false)
				{
					isStarted = true;
					runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.2f),
													   KSFunc({
						CCNotificationCenter::sharedNotificationCenter()->postNotification("finishStartScores2");
					})));
					
				}
			}
			float dx = t->goalPoint.x - t->getPosition().x;
			float dy = t->goalPoint.y - t->getPosition().y;
			float dis = ccpDistance(t->goalPoint, t->startPoint);
			dx = dx / dis * 40.f;
			dy = dy / dis * 40.f;
			if(fabsf(dis) < 5.f)
			{
				t->setPosition(t->goalPoint);
			}
			else
			{
				t->setPosition(ccp(t->getPosition().x + dx, t->getPosition().y + dy));
			}
			
		}		
	}
}

bool JoinMembers::crashCheck(Member* m)
{
	bool crash = false;
	for(auto iter = members.begin(); iter != members.end(); ++iter)
	{
		Member* t = *iter;
		if(t == m)
			continue;
		
		if(AABB(t->boundingBox(), m->boundingBox()))
		{
			crash = true;
			break;
		}
	}
	return crash;
}
void JoinMembers::joiningMembers(JsonBox::Array js, int allUserNumber)
{
	allUsers = js.size();
	KS::KSLog("%", js);
	allUserFnt = CCLabelTTF::create("0", defaultFont, 13.f);
	allUser->addChild(allUserFnt);
	allUserFnt->setPosition(ccp(62, 7));
	for(auto iter = js.begin(); iter != js.end(); ++iter)
	{
		string flag = (*iter)["flag"].getString();
		string userName = (*iter)["nick"].getString();
		theta = wellRand.GetFloatValue(30 * M_PI / 180.f, (-210.f) * M_PI / 180.f);
		CCPoint startPoint = ccp(600 * cosf(theta) + 240.f, 600 * sinf(theta) + 160.f);
		Member* t = new Member();
		t->initWithFile("search_other.png");
		t->autorelease();
		t->setPosition(startPoint);
		
		CCSprite* flagSprite = CCSprite::createWithSpriteFrameName((flag + ".png").c_str());
		if(flagSprite)
		{
			flagSprite->setPosition(ccp(16, 13));
			t->addChild(flagSprite);
		}
		CCLabelTTF* userNameFnt = CCLabelTTF::create(userName.c_str(), defaultFont, 12.f);
		userNameFnt->setPosition(ccp(34, 12));
		userNameFnt->setAnchorPoint(ccp(0.0f, 0.5f));
		userNameFnt->setColor(ccc3(0, 0, 0));
		t->addChild(userNameFnt);
		members.push_back(t);
		addChild(t);
		t->setScale(wellRand.GetFloatValue(0.5f, 1.5f));
		//t 가 부딪힐 때 까지 움직이다 부딪히면 빠져 나옴.
		CCPoint prevPosition = startPoint;
		while(!crashCheck(t))
		{
			float dx = goalPoint.x - t->getPosition().x;
			float dy = goalPoint.y - t->getPosition().y;
			float dis = ccpDistance(goalPoint, startPoint);
			dx = dx / dis * 1.f;
			dy = dy / dis * 1.f;
			prevPosition = t->getPosition();
			t->setPosition(ccp(t->getPosition().x + dx, t->getPosition().y + dy));
		}
		t->setPosition(prevPosition);
		t->goalPoint = prevPosition;
		t->startPoint = startPoint;
	}
	
	for(auto iter = members.begin(); iter != members.end(); ++iter)
	{
		(*iter)->setPosition((*iter)->startPoint);
	}
	
	sort(members.begin(), members.end(), PointerCompare());
	float prevTime = timer;
	for(auto iter = members.begin(); iter != members.end(); ++iter)
	{
		Member* t = *iter;
		t->movingTime = prevTime + wellRand.GetFloatValue(0.05f / 2.f, 0.4f / 2.f);
		prevTime = t->movingTime;
		(*iter)->setPosition((*iter)->startPoint);
	}
}


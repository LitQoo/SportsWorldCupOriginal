//
//  JoinMembers.h
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 5. 31..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCupOriginal_JoinMembers_cpp
#define SportsWorldCupOriginal_JoinMembers_cpp
#include "Well512.h"
#include "cocos2d.h"
USING_NS_CC;
#include "JsonBox.h"
#include <vector>


class Member : public CCSprite
{
public:
	
//	bool isGoal;
	string flag;
	string userName;
	float movingTime;
	CCPoint goalPoint;
	CCPoint startPoint;
	Member()
	{
//		isGoal = false;
	}
};

struct PointerCompare
{
	bool operator()(Member* l, Member* r)
	{
		float d1 = ccpDistance(ccp(240, 145), l->goalPoint);
		float d2 = ccpDistance(ccp(240, 145), r->goalPoint);
		return d1 < d2;
		return l->movingTime < r->movingTime;
	}
};
class JoinMembers : public CCLayer, public CCTextFieldDelegate
{
public:
	JoinMembers();
	virtual ~JoinMembers();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	CREATE_FUNC(JoinMembers);
	virtual void registerWithTouchDispatcher();
	
	void joiningMembers(JsonBox::Array js, int allUserNumber);
	void update(float dt);
private:
	bool isStarted;
	CCPoint goalPoint;
	float timer;
	Well512 wellRand;
	float theta;
	std::vector<Member*> members;
	CCProgressTimer* progress;
	CCLabelTTF* allUserFnt;
	void moveMembersProcess();
	bool AABB(CCRect a, CCRect b);
	bool crashCheck(Member* m);
	int allUsers;
	int progressUsers;
	Member* allUser;
	std::map<Member*, bool> onceMove; // 한번이라도 움직였으면 true 됨.
};

#endif

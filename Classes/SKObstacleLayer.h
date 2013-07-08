//
//  SKObstacle.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 5. 10..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCup_SKObstacle_cpp
#define SportsWorldCup_SKObstacle_cpp

#include "SKFlag.h"
#include "SKCharacter.h"
#include "SKObstacle.h"
#include "cocos2d.h"
USING_NS_CC;

#include <vector>
#include <map>
using namespace std;

class SKObstacleLayer : public CCLayer
{
public:
	SKObstacleLayer();
	virtual ~SKObstacleLayer();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	CREATE_FUNC(SKObstacleLayer);
	virtual void registerWithTouchDispatcher();
	SKCharacter::CRASHTYPE checkCrash();
	SKCharacter::CRASHTYPE checkCrashFlag();
	
	void createObstacle(float currentPosition, float currentDistance, SKObstacle* obs, int currentMove);
	void createGround(float currentDistance);
	void createFlag(float currentPosition, float currentDistance);
	void destroyObstacle(float currentDistance);
	void destroyGround(float currentDistance);
	void destroyFlag(float currentDistance);
	void render(float currentPosition, float currentDistance);
	void focusFlag();
	static int FAR_H;
	static int CLOSE_SIZE;
	static int FAR_SIZE;
	static int VIEW_VALUE;

	
	void setSkier(SKCharacter* c){skier = c;}
	enum OBSPASSTYPE {OBS_CHA=1, OBS_NOCHA=2};
	enum FLAGPASSTYPE {FLAG_CHA=1, FLAG_NOCHA=2};
	OBSPASSTYPE checkObsCha(int& chaCount);
	FLAGPASSTYPE checkFlagCha(int& chaCount);
private:
	int backZOrder;
	int getBackzOrder()
	{
		int b = backZOrder;
		
		if(backZOrder <= 3)
		{
			backZOrder = 1000;
		}
		else
		{
			backZOrder--;
		}
		return b;
	}
	static int BASE_POSITION;
	
	static float EYE_HEIGHT;
	SKCharacter* skier;
	CCSprite* focusFlagSprite;
	vector<SKObstacle*> obstacles;
	vector<SKGround*> grounds;
	vector<pair<SKFlag*, SKFlag*>> flags;
	map<int, float> D;
	float projection(float d){return (d > 1000 ? D[1000] : D[d]) * 2.5f;}
	void renderGround(float currentPosition, float currentDistance);
	void renderObs(float currentPosition, float currentDistance);
	void renderFlag(float currentPosition, float currentDistance);
	void renderPlayer(float currentPosition, float currentDistance);
	
	SKCharacter::CRASHTYPE crashLine(CCPoint A, CCPoint B, CCPoint C, CCPoint D, SKFlag* flag);
	
	void deleteSprite(CCNode* _obj)
	{
		_obj->removeFromParentAndCleanup(true);
		
	}
};

#endif

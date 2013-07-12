//
//  HGGameLayer.h
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 6. 5..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCupOriginal_HGGameLayer_cpp
#define SportsWorldCupOriginal_HGGameLayer_cpp

#include "cocos2d.h"
USING_NS_CC;
#include "Well512.h"




class HGTopObs;
class HGBuilding;
class HGPlayer;
class HGMiddleObs;
class HGCoin;

struct TurningPoint
{
	CCPoint top;
	CCPoint bottom;
};

class HGGameLayer : public CCLayer, public CCTextFieldDelegate
{
public:
	HGPlayer* getHgPlayer(){return hgPlayer;}
	void deleteSprite(CCNode* _n)
	{
		_n->getParent()->removeChild(_n, true);
	}
	HGGameLayer();
	virtual ~HGGameLayer();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	CREATE_FUNC(HGGameLayer);
	virtual void registerWithTouchDispatcher();
	void initTurningPoint();
	void update(float dt);
	void createTuningPoint();
	
	void createBuilding();
	void createCoin(HGBuilding*);
	void createTopObs();
	void createMiddleObs();
	void checkPassObs();
	void checkEatCoin();
//	static float CHAR_POSITION;
//	static float MIN_TURNING_SPACING;
//	static float MAX_TURNING_SPACING;
//	static float MIN_BUILDING_Y;
//	static float MAX_BUILDING_Y;
//	static float MIN_PASS_SPACING;
//	static float MAX_PASS_SPACING;
//	static float MIN_MIDDLE_DISTANCE;
//	static float MAX_MIDDLE_DISTANCE;
//	static const float ONCE_CREATING_SIZE;
//	static float PLAYER_SPEED;
//	static float MIN_PASS_MARGIN;
private:
	
	float nextBuildingPosition;
	float nextObsPosition;
	float nextTopObsPosition;
	void upDownProcess();
	void onScore();
	void onBuildingScore();
	float vertVelocity;
	Well512 well512;
	void render();

	std::vector<TurningPoint> tuningPoints;
	std::vector<HGBuilding*> buildingSprites;
	std::vector<HGMiddleObs*> middleObsSprites;
	std::vector<HGTopObs*> topObsSprites;
	std::vector<HGCoin*> coinSprites;

	float lastCreationDistance;
	float lastCreationY;
	void movePlayer();
	void gabageCollector();
	bool AABB(CCRect a, CCRect b);
	void crashProcess();
	std::pair<CCPoint, CCPoint> getPairPointWithBottom(float xp);
	std::pair<CCPoint, CCPoint> getPairPointWithTop(float xp);
	CC_SYNTHESIZE(bool, isUp, IsUp);
//	HGPlayer* hgPlayer;
	
	CC_SYNTHESIZE(HGPlayer*, hgPlayer, HgPlayer);
	CC_SYNTHESIZE(bool, isStarted, IsStarted);
	CC_SYNTHESIZE(CCParallaxNode*, voidNode, VoidNode);
	CC_SYNTHESIZE(lua_State*, lua, L);
//	CC_SYNTHESIZE_PASS_BY_REF(HGPlayer*, hgPlayer, HgPlayer)
	
};

#endif

//
//  GameScene.h
//  ArcherWorldCupX
//
//  Created by apple on 12. 7. 6..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#ifndef SKGameScene_GameScene_h
#define SKGameScene_GameScene_h

#include "HWFromTo.h"
#include "SKGameInfo.h"
#include "SKObstacleLayer.h"
#include "cocos2d.h"
#include "CommonGame.h"
#include "KSModalAlert.h"
#include "JNIKelper.h"
#include <string.h>
#include "GraphDog.h"
#include "KS_Util.h"
#include "LoadingScene.h"
#include "GameSystem.h"
#include "SWIntro.h"
#include "lua_tinker.h"
#include "SKGraphics.h"
#include "LuaManager.h"
using namespace std;
using namespace cocos2d;

#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::create(); _A * layer = _A::create(); scene->addChild(layer); return scene; }
class SKGameScene : public cocos2d::CCLayer, public CommonGame
{
public:
	SKGameScene()
	{
		L = LuaManager::shared()->getLua("sk.lua");
		currentTouchCount = 0;
		playInfo->gamePtr = this;
		gameTimer = 0;
		lastCreationDistance = 0.f;
		
		nextCreationDistance = lua_tinker::get<float>(L, "MIN_CREATION_DISTANCE") + (lua_tinker::get<float>(L, "MAX_CREATION_DISTANCE") - lua_tinker::get<float>(L, "MIN_CREATION_DISTANCE")) * KS_Util::rnd();
		
//		LuaManager::shared()->getLua("sk.lua");
		
//		aheadRemainTime = 0.f;
	}
	~SKGameScene();
	virtual bool init();  
	CREATE_FUNC(SKGameScene);
    SCENE_FUNC(SKGameScene);

private:
	void PAUSE(CCObject*);
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();
    
	void finishTimeOver();
	void RETRY();
	void finishGetWeek(JsonBox::Object js);
	void onExitZero();
	void onEnterZero();
	
	void deleteSprite(CCNode* _obj)
	{
		_obj->removeFromParentAndCleanup(true);
		
	}
	void finishGo(CCNode* n);
	void finishReady(CCNode* n);
	void eatCoin(int n);
	void update(float dt);
	void timeProcess(float dt);
	void createObstacleProcess();
	void createGroundProcess();
	void createDecorationProcess();
	void createFlagProcess();
	void moveSkier(float dt);
	void crashProcess();
	void judgeChaProcess();
	void onCha(bool obs); 
	void onCrash();
	int scoreFromWhat(bool obs);
	void updateScoreFnt(float dt);
	void updateBalance();
	CCFiniteTimeAction* shakeAction(CCPoint originalPosition);
	
	
private:
	lua_State* L;
	enum {COMBO_COLS = 3, COMBO_FRAME=6, COMBO_WIDTH = 120, COMBO_HEIGHT = 120};
	SKGraphics graphics;
	SKGameInfo info;
	SKObstacleLayer* obstacleLayer;
	SKCharacter* skChar;
		float lastCrashTime;
	
	float gameTimer;
	float nextCreationDistance;
	float lastCreationDistance;
	
	
	
	
	
	
	int currentTouchCount;
	Well512 well512;
//	float aheadRemainTime; // 앞으로 쭉 나갔다가 다시 들어오기까지의 시간
	//static float AHEAD_INIT_TIME;
public:
	static float GND_CREATION_TIME;

	

	
	static float CRASH_ACTION_TIME;
};


#endif

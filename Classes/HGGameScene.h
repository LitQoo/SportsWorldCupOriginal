//
//  GameScene.h
//  ArcherWorldCupX
//
//  Created by apple on 12. 7. 6..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#ifndef HGGameScene_GameScene_h
#define HGGameScene_GameScene_h

#include "HGGameInfo.h"
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
#include "HGGraphics.h"
#include "lua_tinker.h"
#include "LuaManager.h"
using namespace std;
using namespace cocos2d;

class HGGameLayer;
#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::create(); _A * layer = _A::create(); scene->addChild(layer); return scene; }
class HGGameScene : public cocos2d::CCLayer, public CommonGame
{
public:
	HGGameScene()
	{
		playInfo->gamePtr = this;
		gameTimer = 0;
	
		
		HGGameScene* hs = (HGGameScene*)getParent();
		
		
		
		lua = LuaManager::shared()->getLua("hg.lua");
//		aheadRemainTime = 0.f;
	}
	~HGGameScene();
	virtual bool init();  
	CREATE_FUNC(HGGameScene);
    SCENE_FUNC(HGGameScene);
	void keyBackClicked()
	{
		//PAUSE(0);
	}
private:
	void onScore();
	void onBuildingScore();
	void onCrash();
	void onEatCoin();

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
	int scoreFromWhat(bool obs);
	void updateScoreFnt(float dt);
	CCFiniteTimeAction* shakeAction(CCPoint originalPosition);
	
	
private:
	enum {COMBO_COLS = 3, COMBO_FRAME=6, COMBO_WIDTH = 120, COMBO_HEIGHT = 120};
	HGGraphics graphics;
	float gameTimer;
	Well512 well512;
	HGGameLayer* gameLayer;
	
	
	CC_SYNTHESIZE(HGGameInfo, info, Info);
	CC_SYNTHESIZE(lua_State*, lua, L);
};


#endif

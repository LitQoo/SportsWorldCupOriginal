#pragma once

#include "HWGameScene.h"
#include "HWSharingClass.h"
#include "NSDefault.h"
#include "GraphDog.h"
#include "KS_Util.h"
#include "LoadingScene.h"
#include "GameSystem.h"
#include "SWIntro.h"
#include "WorldCupMiddle.h"
#include "HWPlay.h"
#include "GDTranslate.h"
using namespace GDTranslate;
//#include "RoundScene.h"
class FirstGameScene : public GameScene
{
public:
	static int NORMALBACK_SIZE;
	static int NORMALBACK_HEIGHT_SIZE;
	static int FARBACK_SIZE;
	static int FARBACK_HEIGHT_SIZE;
	
private:
	CCSprite* farBack1;CCSprite* farBack2;
	float farBackPos;
	CCSprite* normalBack1;CCSprite* normalBack2;
	float normalBackPos;
	CCSprite* ad_back1; CCSprite* ad_back2;
	float adBackPos;
	float closeBackPos;
	
	CCArray* closeBacks;
	float lastBackPos;
	int screenCount;
	bool _threerepeat;
	int randCount;
	CCSpriteBatchNode* trackBatch;
public:

	virtual ~FirstGameScene()
	{
		closeBacks->release();
	}
	static CCScene* scene()
	{
		CCScene * scene = CCScene::create();
		FirstGameScene* layer = FirstGameScene::create();
		scene->addChild(layer);
		return scene;
	}

	// implement the "static node()" method manually
	CREATE_FUNC(FirstGameScene);
private:
	
	virtual void onEnterTransitionDidFinish();
	virtual void setBackground();
	virtual void initHurdleMapAndPlayer();
	virtual bool init();
	virtual void randomSound(float dt);
	virtual void cachingPList();
	virtual void unjiCheck();
	virtual void timeCheck(float dt);
	virtual void moveBackground(float dt);
	virtual void moveMap(float dt);
	virtual void gameTest(float dt);
	virtual void draw();
	void finishTimeOver()
	{
		graphdog->command("getweek", 0, this, gd_selector(ThisClassType::finishGetWeek));
		
	}
	void finishGetWeek(JsonBox::Object js)
	{
		try
		{
			if(js["state"].getString() != "ok")
				throw string(gt("netfail").c_str());
			playInfo->currentWeek = js["week"].getInt();
			
			if(playInfo->currentGame != "WORLDCUP")
				ReplaceSceneParam(SWIntro, INIT_MODE::GAMEOVER);
			else
			{
				if(playInfo->getWorldcupGame() != "")
					ReplaceSceneParam(WorldCupMiddle, hwplay->gameScore.getVar());
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
	void RETRY()
	{
		graphdog->command("getweek", 0, this, gd_selector(ThisClassType::finishGetWeek));
	}
	
};
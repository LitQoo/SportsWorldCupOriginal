//
//  GameSelect.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 6..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCup_GameSelect_cpp
#define SportsWorldCup_GameSelect_cpp
#include "UIMediator.h"
#include "cocos2d.h"
#include <vector>
#include "Well512.h"
using namespace std;
USING_NS_CC;

struct ContainerElement
{
	string gameCode;
	string titleFile;
	int maxScore;
	int weeklyMaxScore;
	SEL_MenuHandler recordHandler;
	enum CATEGORY_T{OPENED = 1, WORLDCUP = 2,LOCKED = 3 } CATEGORY;
	
	bool operator<(const ContainerElement& e) const
	{
		return this->CATEGORY < e.CATEGORY;
	}
	
};
		
struct Ordering
{
	int seq;
	ContainerElement::CATEGORY_T ct;
	bool operator<(const Ordering& e) const
	{
		return this->ct < e.ct;
	}
};

class GameTitle : public CCSprite
{
	
public:
	enum {menu_record_tag = 0x1000, menu_with_gold, menu_invite};
	CCMenu* menu;
	CCMenuItem* record;
	CCSprite* lockScreen;
};

class GameSelect : public CCLayer, public UIColleague

{
private:
	Well512 well512;
	UIMediator* mediator;
	CCSprite* thiz;
	vector<CCSprite*> gameSprites;
	int currentOrder;
	CCPoint beginPoint;
	CCPoint endPoint;

	void RECORD_HURDLE(CCObject*);
	void RECORD_ARCHER(CCObject*);
	void RECORD_BASKETBALL(CCObject*);
	void RECORD_CLAY(CCObject*);
	void RECORD_WORLDCUP(CCObject*);
	void RECORD_SKI(CCObject*);
	void RECORD_HANG(CCObject*);
//	enum {NONE, UPWARD, DOWNWARD}preserveCmd;
	bool isMoving;
	enum {GAME_COUNT = 7};
	Ordering gameOrder[GAME_COUNT];
	int slidedWithMove;
	
	
	std::vector<std::string> opendGames;
	std::vector<std::string> lockedGames;
	std::string soonOpenGame;
	ContainerElement::CATEGORY_T game2Category(const string& name);
	void sendTicket(CCObject*);
	void unlockWithGold(CCObject*);
	void UNLOCKSTAGE();
	void onUnlockStageWithFriends(CCObject*);
	std::vector<GameTitle*> gameTitles;
public:
	/*
	 spriteFile.push_back(ContainerElement{"SK", "start_ski.png", SKSaveData::getMaxScore(), SKSaveData::getWeeklyMaxScore().second, menu_selector(ThisClassType::RECORD_SKI), game2Category("SK")});
	 spriteFile.push_back(ContainerElement{"AK", "start_clay.png", AKSaveData::getMaxScore(), AKSaveData::getWeeklyMaxScore().second, menu_selector(ThisClassType::RECORD_CLAY), game2Category("AK")});
	 spriteFile.push_back(ContainerElement{"AC", "start_archery.png",ACSaveData::getMaxScore(), ACSaveData::getWeeklyMaxScore().second, menu_selector(ThisClassType::RECORD_ARCHER), game2Category("AC")});
	 spriteFile.push_back(ContainerElement{"HG", "start_hanggliding.png", HGSaveData::getMaxScore(), HGSaveData::getWeeklyMaxScore().second,
	 menu_selector(ThisClassType::RECORD_HANG), game2Category("HG")});
	 spriteFile.push_back(ContainerElement{"BS", "start_basketball.png", BS2SaveData::getMaxScore(), BS2SaveData::getWeeklyMaxScore().second, menu_selector(ThisClassType::RECORD_BASKETBALL), game2Category("BS")});
	 spriteFile.push_back(ContainerElement{"HW", "start_hurdle.png", HWSaveData::getMaxScore(), HWSaveData::getWeeklyMaxScore().second, menu_selector(ThisClassType::RECORD_HURDLE), game2Category("HW")});
	 spriteFile.push_back(ContainerElement{"WORLDCUP", "start_coming
	 */
	static const int SKI_INDEX = 0;
	static const int CLAY_INDEX = 1;
	static const int ARCHERY_INDEX = 2;
	static const int HG_INDEX = 3;
	static const int BASKETBALL_INDEX = 4;
	static const int HURDLE_INDEX = 5;
	static const int COMINGSOON = 6;
	GameSelect();
	virtual ~GameSelect();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	void hideGames();
	void showGames();
	int getCurrentIndex()
	{
		CCAssert(0 <= currentOrder && currentOrder < sizeof(gameOrder) / sizeof(gameOrder[0]), "index error");
		return gameOrder[currentOrder].seq;
	}
	static GameSelect* create(UIMediator *m)
	{
		GameSelect *pRet = new GameSelect();
		pRet->setMediator(m);
		if (pRet && pRet->init())
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}
	virtual void registerWithTouchDispatcher();
	void setMediator(UIMediator* m)
	{
		mediator = m;
	}
	void finishMoving();
	void syncInfo();
};

#endif

#pragma once

//#include "AppDelegate.h"
#include "cocos2d.h"
#include "CommonGame.h"

#include "KSProtectVar.h"
#include "IntroHead.h"

#include <map>
#include <string>
#include <deque>
#include "GameShop.h"

#include "Well512.h"
USING_NS_CC;

extern int TIMEOVERWIDTH;
extern int TIMEOVERHEIGHT;
extern float BASETIME;
class GameSystem
{
public:
	int __ateCoin;
	string currentGame;
	map<string, GameShop::ITEM_SELTYPE> selectedItems;
	int currentWeek;
	int whenRecvLeftTime;
	CommonGame* gamePtr;
	IntroHead* pIntroHead;
	bool justOpen;
	int acscore;
	int acweekly;
	int akscore;
	int akweekly;
	int bsscore;
	int bsweekly;
	int hgscore;
	int hgweekly;
	int hwscore;
	int hwweekly;
	int skscore;
	int skweekly;
	int wcscore;
	int wcweekly;
public:
	static GameSystem* sharedObject()
	{
		static GameSystem* _ins = 0;
		if(_ins == 0)
		{
			_ins = new GameSystem();
		}
		return _ins;
	}
	void initLvTable();
	pair<int, int> getLV_EXP(int exp);
	int expFromLv(int lv);
	static int getCurrentDay();
	static int getCurrentTime_s();
	enum {HEART_CHARGE_TIME = 15 * 60, DEFAULT_MAX_HEART = 5}; // X minutes
	enum {WORLDCUP_ENTRANCE_FEE = 2000};
	//
	
	void createWorldcupOrder();
	void initGameAndStart(string game);
	string getWorldcupGame();
	void popGame();
	const std::deque<std::string>& getWorldcupOrder() const {return worldcupOrder;};
	double randomItemProb(int i);
	std::deque<std::string> originalWorldcupOrder; // 게임 순서 저장하는 용도.
private:
	
	enum {MAX_LEVEL = 100};
	std::deque<std::string> worldcupOrder;
	
	int lvTable[MAX_LEVEL];

	GameSystem()
	{
		__ateCoin = 0;
		gamePtr = 0;
		pIntroHead = 0;
		justOpen = false;
		
		wcscore = acscore = akscore = bsscore = hgscore = hwscore = skscore = 0;
		
		wcweekly = acweekly = akweekly = bsweekly = hgweekly = hwweekly = skweekly = 0;

	}
};

extern GameSystem* playInfo;
// +(BOOL)isRetinaDisplay; // #




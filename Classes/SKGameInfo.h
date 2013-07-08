#ifndef SK_GameInfo_h
#define SK_GameInfo_h

#include "GameSystem.h"
#include "cocos2d.h"
#include "lua_tinker.h"
#include "LuaManager.h"

using namespace cocos2d;



struct SKGameInfo
{
	static CCPoint CHARACTER_POSITION;
	
	bool in10s; // 10초 안에 진입했는지.
	bool usedShield;
	int ateGoldCount;
	
	//	int comboInZeroMode;
	float remainTime;

	enum GAMEMODETYPE {NORMAL = 0, ZERO=2} gameMode;
	static int INIT_GAME_TIME;
	enum {NON_WORLDCUP_TIME = 40/2, WORLDCUP_TIME = 27}; // 40, 27
	float lastChaTime;
	float timeAfterZero;
	float bonusTime;
	SKGameInfo();
	bool isGameOvering; // 타임오버 후 한번만 실행되게 하게끔 하는 변수
	void initBalanceElement();
	
	float lastGroundCreationTime;
	float nextGroundCreationTime;
	float skierSpeed;
	
	float nextDecoCreationTime;
	float lastDecoCreationTime;
	
	float nextFlagCreationDistance;
	float lastFlagCreationDistance;
	lua_State* L;
};



#endif



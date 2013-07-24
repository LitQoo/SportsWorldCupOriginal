//
//  HWGameInformation.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 1. 31..
//
//

#ifndef __SportsWorldCup__HWGameInformation__
#define __SportsWorldCup__HWGameInformation__


#include "HWHurdleMapLayer.h"
#include "HWFromTo.h"
#include "GameSystem.h"
#include "ProbSelector.h"
#include "NSDefault.h"
// 게임에서만 쓰이는 변수.  다른 씬에서는 안씀
struct HWGameInformation
{
	bool in10s; // 10초 안에 진입했는지.
	bool isGameOvering; // 타임오버 후 한번만 실행되게 하게끔 하는 변수
	float remainTime;
	float bonusTime;
	//int nowDistance;
	int humanPosition;
	FromToWithDuration runSpeed2;
	float runSpeed;
	enum GAMEMODETYPE {NORMAL = 0, ZEROING=1, ZERO=2, OUTZEROING=3} gameMode;
	int runStep;
	float bonusScale;
	int ateGoldCount;
	static int INIT_GAME_TIME;
	enum {NON_WORLDCUP_TIME = 42/2, WORLDCUP_TIME = 26};
	bool usedShield;
	HWGameInformation()
	{
		in10s = false;
		usedShield = false;
		if(playInfo->currentGame == "WORLDCUP")
		{
			HWGameInformation::INIT_GAME_TIME = HWGameInformation::WORLDCUP_TIME;
		}
		else
		{
			HWGameInformation::INIT_GAME_TIME = HWGameInformation::NON_WORLDCUP_TIME;
		}
		
		if(playInfo->selectedItems["item_random"] != GameShop::NONE)
		{
			int i = ProbSelector::sel(playInfo->randomItemProb(0), playInfo->randomItemProb(1),
									  playInfo->randomItemProb(2), playInfo->randomItemProb(3), 0.0);
			NSDefault::incRandomItemCount();
			std::string a("item_highhurdle1.png");
			std::string b("item_wideview1.png");
			switch(i)
			{
				case 0:
					if(playInfo->selectedItems[a] == GameShop::NONE)
						playInfo->selectedItems[a] = GameShop::RANDOMSEL;
					if(playInfo->selectedItems[b] == GameShop::NONE)
						playInfo->selectedItems[b] = GameShop::RANDOMSEL;
					break;
				case 1:
					if(playInfo->selectedItems[b] == GameShop::NONE)
						playInfo->selectedItems[b] = GameShop::RANDOMSEL;
					break;
				case 2:
					if(playInfo->selectedItems[a] == GameShop::NONE)
						playInfo->selectedItems[a] = GameShop::RANDOMSEL;
					break;
				case 3:
					break;
			}
		}
		bonusTime = 0.f;
//		bonusTime = playInfo->selectedItems["item_random"] ? (float)INIT_GAME_TIME * 0.1f : 0.f;
		isGameOvering = false;
		ateGoldCount = 0;
		bonusScale = playInfo->selectedItems["item_wideview1.png"] ? 0.9f : 1.f;
	}
};

struct HWInnerVar
{
	
	int zeroStep;
	float timeAfterZero;
	int beforeRunStep;
	FromToWithDuration scaleFromTo;
	FromToWithDuration positionFromTo;
	float resJumpTime;
	bool noTouchBeforeResJump;
	bool touching;
	bool isTimeover;
	bool isReady;
	float minJumpInPixel;
	float maxJumpInPixel;
	//bool isTracingHill;
	//double tracingTime;
	//float prevHillHeight;
	double startTime;
	bool isScoring;
	enum {NONE, RESJUMP} reservedJump;
	
	float yVelocity;
	static const float leapVelocity;
	static const float hurdleG;
	bool beginDrop;
	Obstacle* prevGround;
	
	static const float ZERO_DEC_VALUE;
	static const float INC_VALUE_IN_ZERO;
	static const float INC_VALUE_IN_NORMAL;
	static const float CRASH_DEC_VALUE_IN_NORMAL;
	static const float CRASH_DEC_VALUE_IN_ZERO;
};
#endif /* defined(__SportsWorldCup__HWGameInformation__) */

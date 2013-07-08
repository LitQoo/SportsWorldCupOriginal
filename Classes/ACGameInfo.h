//
//  GameInfo.h
//  ArcherWorldCupX
//
//  Created by apple on 12. 7. 7..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#ifndef ArcherWorldCupX_GameInfo_h
#define ArcherWorldCupX_GameInfo_h

#include "GameSystem.h"
#include "ProbSelector.h"
#include "NSDefault.h"
struct ACGameInfo
{
	bool in10s;
	bool usedShield;
	static const float ZERO_DEC_VALUE;
	static const float INC_VALUE_IN_ZERO;
	static const float INC_VALUE_IN_NORMAL;
	static const float CRASH_DEC_VALUE_IN_NORMAL;
	static const float CRASH_DEC_VALUE_IN_ZERO;
	float remainTime;
	float bonusTime;
	int shotCounterAfterZero;
	int shotCounter;
	enum {MAX_WIND = 20, MIN_WIND=0};
	enum {NON_WORLDCUP_TIME = 48/2, WORLDCUP_TIME = 25};
	static int INIT_GAME_TIME;
	int wind;
	int ateGoldCount;
	enum GAMEMODETYPE {NORMAL = 0, ZEROING=1, ZERO=2, OUTZEROING=3} gameMode;
	ACGameInfo()
	{
		in10s = false;
		usedShield = false;
		ateGoldCount = 0;
		if(playInfo->currentGame == "WORLDCUP")
		{
			ACGameInfo::INIT_GAME_TIME = ACGameInfo::WORLDCUP_TIME;
		}
		else
		{
			ACGameInfo::INIT_GAME_TIME = ACGameInfo::NON_WORLDCUP_TIME;
		}
		remainTime = INIT_GAME_TIME;
		shotCounterAfterZero = 0;
		shotCounter = 0;
		wind = 0.f;
		
		if(playInfo->selectedItems["item_random"] != GameShop::NONE)
		{
			int i = ProbSelector::sel(playInfo->randomItemProb(0), playInfo->randomItemProb(1),
									  playInfo->randomItemProb(2), playInfo->randomItemProb(3), 0.0);
			NSDefault::incRandomItemCount();
			std::string a("item_wind1.png");
			std::string b("item_vibration1.png");
			switch(i)
			{
				case 0:
					playInfo->selectedItems[a] = GameShop::RANDOMSEL;
					playInfo->selectedItems[b] = GameShop::RANDOMSEL;
					break;
				case 1:
					playInfo->selectedItems[b] = GameShop::RANDOMSEL;
					break;
				case 2:
					playInfo->selectedItems[a] = GameShop::RANDOMSEL;
					break;
				case 3:
					break;
			}
		}
//		bonusTime = playInfo->selectedItems["item_random"] ? (float)INIT_GAME_TIME * 0.1f : 0.f;
	}
};


#endif



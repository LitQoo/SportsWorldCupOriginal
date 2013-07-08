//
//  AKGameInfo.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 19..
//
//

#include "SKGameInfo.h"
#include "SKGameScene.h"
#include "KS_Util.h"
#include "ProbSelector.h"
#include "NSDefault.h"


int SKGameInfo::INIT_GAME_TIME = 40; // 40.f
#include "GameSystem.h"
SKGameInfo::SKGameInfo()
{
	L = LuaManager::shared()->getLua("sk.lua");
	in10s = false;
	usedShield = false;
	if(playInfo->currentGame == "WORLDCUP")
	{
		SKGameInfo::INIT_GAME_TIME = SKGameInfo::WORLDCUP_TIME;
	}
	else
	{
		SKGameInfo::INIT_GAME_TIME = SKGameInfo::NON_WORLDCUP_TIME;
	}
	{
//		if(playInfo->selectedItems["item_wideview1.png"])
		{
			// 아이템 처리.
		}
	}
	
	gameMode = SKGameInfo::NORMAL;
	timeAfterZero = 0.f;
	remainTime = INIT_GAME_TIME;
	bonusTime = 0.f;
	
	{
		if(playInfo->selectedItems["item_random"] != GameShop::NONE)
		{
			int i = ProbSelector::sel(playInfo->randomItemProb(0), playInfo->randomItemProb(1),
									  playInfo->randomItemProb(2), playInfo->randomItemProb(3), 0.0);
			NSDefault::incRandomItemCount();
			std::string a("item_skwide1.png");
			std::string b("item_curvespeed1.png");
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
	}
	

	ateGoldCount = 0;
	isGameOvering = false;
	
	lastGroundCreationTime = 0.f;
	nextGroundCreationTime = SKGameScene::GND_CREATION_TIME;
	
	lastDecoCreationTime = 0.f;
	nextDecoCreationTime = (lua_tinker::get<float>(L, "MIN_CREATION_DISTANCE") + (lua_tinker::get<float>(L, "MAX_CREATION_DISTANCE") - lua_tinker::get<float>(L, "MIN_CREATION_DISTANCE")) * KS_Util::rnd() ) / 2.f;
	
	lastFlagCreationDistance = 0.f;
	nextFlagCreationDistance = (lua_tinker::get<float>(L, "FLAG_MIN_CREATION_DISTANCE") + (lua_tinker::get<float>(L, "FLAG_MAX_CREATION_DISTANCE") - lua_tinker::get<float>(L, "FLAG_MIN_CREATION_DISTANCE")) * KS_Util::rnd() );
	skierSpeed = lua_tinker::get<float>(L, "DEFAULT_SPEED");
}

void SKGameInfo::initBalanceElement()
{
	
}
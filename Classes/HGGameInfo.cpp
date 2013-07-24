//
//  AKGameInfo.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 19..
//
//

#include "HGGameInfo.h"
#include "HGGameScene.h"
#include "KS_Util.h"
#include "ProbSelector.h"
#include "NSDefault.h"



int HGGameInfo::INIT_GAME_TIME = 40; // 40.f
#include "GameSystem.h"
HGGameInfo::HGGameInfo()
{
	in10s = false;
	usedShield = false;
	if(playInfo->currentGame == "WORLDCUP")
	{
		HGGameInfo::INIT_GAME_TIME = HGGameInfo::WORLDCUP_TIME;
	}
	else
	{
		HGGameInfo::INIT_GAME_TIME = HGGameInfo::NON_WORLDCUP_TIME;
	}
	{
		if(playInfo->selectedItems["item_hanggliding2.png"] != GameShop::NONE)
		{
			// 아이템 처리.
		}
	}
	
	gameMode = HGGameInfo::NORMAL;
	timeAfterZero = 0.f;
	remainTime = INIT_GAME_TIME;
	bonusTime = 0.f;
	
	{
		if(playInfo->selectedItems["item_random"] != GameShop::NONE)
		{
			int i = ProbSelector::sel(playInfo->randomItemProb(0), playInfo->randomItemProb(1),
									  playInfo->randomItemProb(2), playInfo->randomItemProb(3), 0.0);
			NSDefault::incRandomItemCount();
			std::string a("item_hanggliding1.png");
			std::string b("item_hanggliding2.png");
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
	}
	

	ateGoldCount = 0;
	isGameOvering = false;
}

void HGGameInfo::initBalanceElement()
{
	
}
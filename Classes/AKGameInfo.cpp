//
//  AKGameInfo.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 19..
//
//

#include "AKGameInfo.h"
#include "AKAngryGameScene.h"
#include "NSDefault.h"
const float AKGameInfo::ZERO_DEC_VALUE = -0.17f;
const float AKGameInfo::INC_VALUE_IN_ZERO = 8.f; // 13.f
const float AKGameInfo::INC_VALUE_IN_NORMAL = 5.f; // 12.f
const float AKGameInfo::CRASH_DEC_VALUE_WHEN_NOTARGET_IN_NORMAL = 0.f; // -40.f
const float AKGameInfo::CRASH_DEC_VALUE_WHEN_NOTARGET_IN_ZERO = -40.f;
const float AKGameInfo::CRASH_DEC_VALUE_WHEN_WRONGTARGET_IN_NORMAL = 0.f; // -40.f
const float AKGameInfo::CRASH_DEC_VALUE_WHEN_WRONGTARGET_IN_ZERO = -30.f;

const float AKGameInfo::INIT_MINTERM = 0.6f;
const float AKGameInfo::INIT_MAXTERM = 0.8f;
int AKGameInfo::INIT_GAME_TIME = 40;
#include "GameSystem.h"
#include "ProbSelector.h"
AKGameInfo::AKGameInfo()
{
	in10s = false;
	usedShield = false;
	bonusScale = 1.f;
	if(playInfo->currentGame == "WORLDCUP")
	{
		AKGameInfo::INIT_GAME_TIME = AKGameInfo::WORLDCUP_TIME;
	}
	else
	{
		AKGameInfo::INIT_GAME_TIME = AKGameInfo::NON_WORLDCUP_TIME;
	}
	{
		if(playInfo->selectedItems["item_large1.png"] != GameShop::NONE)
			bonusScale = 1.1f;
	}

	gameMode = AKGameInfo::NORMAL;
	zeroStep = 1;
	beforeRunStep = 1;
	timeAfterZero = 0.f;
	remainTime = INIT_GAME_TIME;
	bonusTime = 0.f;

	{
		
		
		if(playInfo->selectedItems["item_random"] != GameShop::NONE)
		{
			int i = ProbSelector::sel(playInfo->randomItemProb(0), playInfo->randomItemProb(1),
									  playInfo->randomItemProb(2), playInfo->randomItemProb(3), 0.0);
			NSDefault::incRandomItemCount();
			std::string a("item_bird1.png");
			std::string b("item_large1.png");
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
//			bonusTime = (float)INIT_GAME_TIME * 0.1f;
	}
	
	gameTimer = 0;
	
	lastCreateTime = 0.f;
	lastCreateTime2 = 0.f;
	minTerm = INIT_MINTERM;
	maxTerm = INIT_MAXTERM;
	nextCreateTime = lastCreateTime + minTerm + AngryGameScene::rnd() * (maxTerm - minTerm);
	nextCreateTime2 = lastCreateTime2 + minTerm + AngryGameScene::rnd() * (maxTerm - minTerm);
	disableTime = 0.f;
	isDisable = false;
//	comboInZeroMode = 0;
	ateGoldCount = 0;
	isGameOvering = false;
}

void AKGameInfoinitBalanceElement()
{
	
}
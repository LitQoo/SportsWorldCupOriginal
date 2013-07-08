//
//  skconfig.cpp
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 5. 30..
//
//

#include "skconfig.h"
#include "SKPlay.h"
#include "SKGameScene.h"
#include "SKObstacleLayer.h"
#include "SKCharacter.h"
#include "SKGameInfo.h"



//float SKGameScene::HORIZ_SPEED = 5.2f;						// 좌우 이동속도


void SKGameScene::updateBalance()
{
	const float timeAfterZero = info.timeAfterZero;									// 제로모드후 시간
	const int currentCombo = skplay->combo.getVar();								// 현재 콤보 상태
//	const float currentGauge = graphics.feverGauge->getPercentage();				// 게이지 0~100 사이.
	const bool bZero = info.gameMode == SKGameInfo::GAMEMODETYPE::ZERO ? true : false;	// 제로인지 여부.
//	const float remainTime = info.remainTime;											// 남은 시간.
//  경수야 안녕
	
	lua_tinker::call<void>(L, "updateBalance", currentCombo, bZero, timeAfterZero);
	
   
	// 여기서 밸런스 조작하면 됨.
}



int SKGameScene::scoreFromWhat(bool obs)
{
	// 장애물 촤~ 이면 obs = truessdsdzzzzzxchzxkchzxkchjxzkczzzzxcxc
	// 곶침
	// 리턴값이 더할 점수임.
	if(obs)
	{
		return 700.f * 2.f;
	}
	else
	{
		float mul = 1.f;
		if(info.gameMode == SKGameInfo::ZERO)
			mul = 2.f;
		return skplay->combo.getVar() * 1000.f * mul * 2.f;
	}
	
}

//zzz

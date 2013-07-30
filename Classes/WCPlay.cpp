//
//  WCPlay.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 4. 4..
//
//

#include "WCPlay.h"
#include "GameSystem.h"
#include "ACPlay.h"
#include "AKPlay.h"
#include "SKPlay.h"
#include "HGPlay.h"
#include "BS2Play.h"
#include "HWPlay.h"


int WCPlay::totalGameScore()
{
	return acplay->gameScore.getVar() + akplay->gameScore.getVar() + skplay->gameScore.getVar() + hgplay->gameScore.getVar() + bs2play->gameScore.getVar() +
		hwplay->gameScore.getVar();
}

WCPlay* wcplay = WCPlay::sharedObject();
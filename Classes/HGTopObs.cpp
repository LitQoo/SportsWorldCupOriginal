//
//  HGBuilding.cpp
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 6. 17..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "HGTopObs.h"
#include "KS_Util.h"
HGTopObs::HGTopObs()
{
	
}
HGTopObs::~HGTopObs()
{

}

bool HGTopObs::init()
{
	AdvanceSprite::init();
	
	selfAnimation = new CCAnimation;//::animation();
	selfAnimation->init();
	int r = rand() % 3 + 1;
	cloudWidth = createAnimation(selfAnimation,  KS_Util::stringWithFormat("hg_cloud%d.png", r).c_str()).size.width;
//					, 4, 4, 100, 60);
	addFrames(selfAnimation->getFrames());
	this->startAnimation(1, 1, -1, 0, 0, 20, false, false);
	
	
	
	return true;
}

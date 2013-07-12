//
//  HGBuilding.cpp
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 6. 17..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "HGBuilding.h"
#include "KS_Util.h"
HGBuilding::HGBuilding()
{
	collapse = false;
	isChecked = false;
	width = 70;
}
HGBuilding::~HGBuilding()
{
	
}

bool HGBuilding::init()
{
	AdvanceSprite::init();
	
	selfAnimation = new CCAnimation;//::animation();
	selfAnimation->init();
	buildingWidth = createAnimation(selfAnimation,
			KS_Util::stringWithFormat("hg_tower%d.png", rand() % 10 + 1).c_str()).size.width;
	addFrames(selfAnimation->getFrames());
	this->startAnimation(1, 1, -1, 0, 0, 20, false, false);
	
	
	
	return true;
}

//
//  HGCoin.cpp
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 6. 19..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "HGCoin.h"

#include "KS_Util.h"
#include "HGGraphics.h"

HGCoin::HGCoin()
{
	isEat = false;
	
}
HGCoin::~HGCoin()
{
	
}

bool HGCoin::init()
{
	AdvanceSprite::init();
	
	selfAnimation = new CCAnimation;//::animation();
	selfAnimation->init();
	createAnimation(selfAnimation, "coin.png", 6,6, HGGraphics::COIN_WIDTH, HGGraphics::COIN_HEIGHT);
	addFrames(selfAnimation->getFrames());
	this->startAnimation(1, 6, -1, 0, 0, 20, false, false);
	
	
	
	return true;
}

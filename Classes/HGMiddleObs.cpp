//
//  MiddleObs.cpp
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 6. 18..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "HGMiddleObs.h"

HGMiddleObs::HGMiddleObs()
{
	isAttack = false;
	isChecked = false;
}
HGMiddleObs::~HGMiddleObs()
{

}

bool HGMiddleObs::initWithStoneIndex(int i)
{
	AdvanceSprite::init();
	
	selfAnimation = new CCAnimation;//::animation();
	selfAnimation->init();
	createAnimation(selfAnimation, CCString::createWithFormat("hg_stone%d.png", i)->getCString());//, 8, 8, 70, 25);
	width = 70;
	addFrames(selfAnimation->getFrames());
	this->startAnimation(1, 1, -1, 0, 0, 20, false, false);
	
	return true;

}
bool HGMiddleObs::init()
{
	AdvanceSprite::init();
	
	selfAnimation = new CCAnimation;//::animation();
	selfAnimation->init();
	createAnimation(selfAnimation, "hg_stone1.png");//, 8, 8, 70, 25);
	width = 70;
	addFrames(selfAnimation->getFrames());
	this->startAnimation(1, 1, -1, 0, 0, 20, false, false);
	
	return true;
}


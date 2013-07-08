//
//  HGPlayer.cpp
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 6. 17..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "HGPlayer.h"

float HGPlayer::UNBEATABLE_TIME = 2.f;

HGPlayer::HGPlayer()
{
	posX = 50;
	posY = 160;
	STATE = NORMAL;
}
HGPlayer::~HGPlayer()
{

}
bool HGPlayer::init()
{
	AdvanceSprite::init();
	
	goAnimation = new CCAnimation;//::animation();
	goAnimation->init();
	createAnimation(goAnimation, "hg_ch.png", 8, 8, 70, 25);
	addFrames(goAnimation->getFrames());
	this->startAnimation(1, 8, -1, 0, 0, 10, false, false);

	
	
	return true;
}

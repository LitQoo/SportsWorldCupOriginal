//
//  ArcheryShop.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 11..
//
//

#include "SkiShop.h"
bool SkiShop::init()
{
	CCSprite::init();
	GameShop::initMenu("SK", "item_skwide1.png", "item_curvespeed1.png");
	
	
	return true;
}

SkiShop::~SkiShop()
{
	
}
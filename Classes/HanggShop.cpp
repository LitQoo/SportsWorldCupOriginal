//
//  ArcheryShop.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 11..
//
//

#include "HanggShop.h"
bool HanggShop::init()
{
	CCSprite::init();
	GameShop::initMenu("HG", "item_hanggliding1.png", "item_hanggliding2.png");
	
	
	return true;
}

HanggShop::~HanggShop()
{
	
}
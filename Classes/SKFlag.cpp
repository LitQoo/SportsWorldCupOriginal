//
//  SKFlag.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 5. 21..
//
//

#include "SKFlag.h"

#include "KS_Util.h"

int SKFlag::cycle = 0;
void SKFlag::createSprite()
{
	cycle++;
	obsAnimation = new CCAnimation;//::animation();
	obsAnimation->init();
	string fileName;
	if(cycle % 4 == 1 || cycle % 4 == 2)
	{
		if(right)
		{
			fileName = "sk_flag1_right.png";
		}
		else
		{
			fileName = "sk_flag1_left.png";
		}
	}
	else
	{
		if(right)
		{
			fileName = "sk_flag2_right.png";
		}
		else
		{
			fileName = "sk_flag2_left.png";
		}
	}
	createAnimation(obsAnimation, fileName.c_str(), 1, 1, 55, 120);
	addFrames(obsAnimation->getFrames());
	this->startAnimation(1, 1, -1, 0, 0, 15, false, false);
	this->setAnchorPoint(ccp(0.5, 0.05f));
	obsSize = SIZE;
}
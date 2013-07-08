//
//  ACGraphics.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 22..
//
//

#ifndef __SportsWorldCup__HGGraphics__
#define __SportsWorldCup__HGGraphics__
#include "cocos2d.h"
USING_NS_CC;
struct HGGraphics
{
	CCMenuItem* pause;
	CCProgressTimer* zeroSprite;
	CCProgressTimer* feverGauge;
	CCLabelBMFont* scoreFnt;
	CCLabelBMFont* timeFnt;
	CCSprite* watch1, *watch2, *watch3;
	CCSprite* leftAim;
	CCSprite* rightAim;
	CCLabelBMFont* ateGoldFnt;
	CCSprite* disableLeft,*disableRight;
	enum {AIM_FRAME = 4, AIM_SIZE = 55};
	enum {COIN_WIDTH = 31, COIN_HEIGHT = 31};
	HGGraphics()
	{
	}
};

#endif /* defined(__SportsWorldCup__ACGraphics__) */

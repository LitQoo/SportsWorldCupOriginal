//
//  CheckDaily.h
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 6. 26..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCupOriginal_CheckDaily_cpp
#define SportsWorldCupOriginal_CheckDaily_cpp

#include "cocos2d.h"
USING_NS_CC;


class CheckDaily : public CCLayerColor
{
private:
	bool touchEnable;
public:
	CheckDaily();
	virtual ~CheckDaily();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	CREATE_FUNC(CheckDaily);
	virtual void registerWithTouchDispatcher();
};

#endif

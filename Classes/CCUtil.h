//
//  CCUtil.h
//  HurdleWorldCup
//
//  Created by ksoo k on 12. 5. 15..
//  Copyright (c) 2012³â __MyCompanyName__. All rights reserved.
//
#pragma once

#include "cocos2d.h"

USING_NS_CC;
class CCUtil : public CCObject
{
public:
	static int getNumberOfChild(CCNode* node);
	static bool isLeft(CCPoint a,CCPoint b,CCPoint c);
	static bool isIntersect(CCPoint a,CCPoint b,CCPoint c,CCPoint d);
	static bool isOnSegment(CCPoint a,CCPoint b,CCPoint c);
};

class TopMenu : public CCMenu
{
public:
	virtual ~TopMenu()
	{
		
		
	}
	virtual void registerWithTouchDispatcher()
	{
		
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority - 2, true);
	}
};



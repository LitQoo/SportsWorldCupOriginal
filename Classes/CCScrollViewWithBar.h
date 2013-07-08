//
//  CCScrollViewWithBar.h
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 6. 24..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCupOriginal_CCScrollViewWithBar_cpp
#define SportsWorldCupOriginal_CCScrollViewWithBar_cpp

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;


class CCScrollViewWithBar : public cocos2d::extension::CCScrollView
{
public:
	CCScrollViewWithBar();
	virtual ~CCScrollViewWithBar();

	virtual bool init();
	CREATE_FUNC(CCScrollViewWithBar);
	virtual void registerWithTouchDispatcher();
};

#endif

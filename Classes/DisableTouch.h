//  Copyright (c) 2012년 LitQoo Inc. All rights reserved.
//  hsnks100@gmail.com LitQoo.한경수
//  DisableTouch.h

#ifndef __CoverLayer_cpp
#define __CoverLayer_cpp

#include "cocos2d.h"
USING_NS_CC;


class DisableTouch : public CCLayerColor
{
private:
	virtual void registerWithTouchDispatcher()
	{
		cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate
		(this, INT_MIN, true);
	}
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		return true; // swallow;
	}
	
public:
	DisableTouch() : CCLayerColor(){}
	virtual ~DisableTouch() {
	}
	
	bool init()
	{
		if(!this->CCLayerColor::initWithColor(ccc4(0, 0, 0, 0)))
			return false;
		
		setTouchEnabled(true);
//		ignoreAnchorPointForPosition(true);
		return true;
	}
	

	virtual bool init(ccColor4B color)
	{
		CCLayerColor::initWithColor(color);

		setTouchEnabled(true);
//		ignoreAnchorPointForPosition(true);
		return true;
	}
	CREATE_FUNC(DisableTouch);
	static DisableTouch* create(ccColor4B color)
	{
		DisableTouch *pRet = new DisableTouch();
		if (pRet && pRet->init(color))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}; 

};

#endif

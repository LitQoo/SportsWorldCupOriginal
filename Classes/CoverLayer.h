//
//  CoverLayer.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 4. 12..
//
//

#ifndef __SportsWorldCup__CoverLayer__
#define __SportsWorldCup__CoverLayer__

#include "cocos2d.h"

USING_NS_CC;

class CoverLayer : public CCLayerColor
{
private:
	virtual void registerWithTouchDispatcher()
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority - 1, true);
	}
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		CCPoint location = pTouch->getLocationInView();
		location = CCDirector::sharedDirector()->convertToGL(location);
		
		return true; // swallow;
	}
	
public:
	virtual bool init()
	{
		if ( !this->CCLayerColor::initWithColor(ccc4(0,0,0,150), 600, 480) )
			return false;
		
		setTouchEnabled(true);
		return true;
	}
	
	CoverLayer() : CCLayerColor(){}
	virtual ~CoverLayer() {
		
	}
	CREATE_FUNC(CoverLayer);
};

#endif /* defined(__SportsWorldCup__CoverLayer__) */

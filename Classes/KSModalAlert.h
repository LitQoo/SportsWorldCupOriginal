#pragma once

#include "cocos2d.h"

#include <stdarg.h>
#include <string>

using namespace std;


#define NOPARAM (CCObject*)0, (SEL_CallFunc)0

class KSCoverLayer : public cocos2d::CCLayerColor
{
public:
	static int touchPriority;
private:
	virtual void registerWithTouchDispatcher()
	{
		cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate
		(this, touchPriority, true);
	}
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
public:
	KSCoverLayer()
	{
		touchPriority--;
	}
	CREATE_FUNC(KSCoverLayer);
	virtual ~KSCoverLayer() {
		touchPriority++;
	}

	virtual bool init();
};

class KSCallBack : public cocos2d::CCNode
{
public:
	KSCallBack(CCObject * _selectorTarget,
		cocos2d::SEL_CallFunc _selector )
	{
		selectorTarget = _selectorTarget;
		selector = _selector;
	}

	virtual ~KSCallBack()
	{

	}
	static KSCallBack* callBackWithOption(
		cocos2d::CCLayer* coverLayer,
		CCObject *selectorTarget,
		cocos2d::SEL_CallFunc selector)
	{
		KSCallBack *cncb = new KSCallBack(selectorTarget, selector);
		coverLayer->addChild(cncb);
		cncb->release();
		return cncb;
	}
	void Execute(CCObject *menu_item);
private:
	CC_SYNTHESIZE(CCObject*, selectorTarget, selectorTarget);
	CC_SYNTHESIZE(cocos2d::SEL_CallFunc, selector, selector);
};

void KSModalAlert(char const* title,	char const *message, int nargs, ...);
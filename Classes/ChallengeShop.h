//
//  ChallengeShop.h
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 6. 21..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCupOriginal_ChallengeShop_cpp
#define SportsWorldCupOriginal_ChallengeShop_cpp
#include "cocos-ext.h"

USING_NS_CC_EXT;
#include "cocos2d.h"
USING_NS_CC;

#include "ScrollBar.h"
#include "GameShop.h"

#define THIS ChallengeShop

class ChallengeItemLayer;
class ChallengeShop  : public CCSprite, public CCScrollViewDelegate
{
public:
	ChallengeShop();
	virtual ~ChallengeShop();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	CREATE_FUNC(ChallengeShop);
	virtual void registerWithTouchDispatcher();
	static const int LIMITNUMBER;
private:
	
	CCLabelBMFont* price;
	std::vector<CCSprite*> checks;
	std::map<std::string, GameShop::ITEM_SELTYPE> selectedItems;
	std::map<std::string, int> levelLimits;
	struct __POSITION{
		CCPoint time, shield, exp;
	}position;
	CCSprite* thiz;
	CCLabelTTF* desc;
	void RANDOM(CCObject*);
	void SHIELD(CCObject*);
	void EXP(CCObject*);
	void onDesc(CCObject*);
	CCMenuItemToggle* times, *shields, *exps;
	void LEVELLIMIT(CCObject* m);
	ScrollBar* bar;
	
public:
	void scrollViewDidScroll(CCScrollView* view){
		 if(bar != NULL) bar->setBarRefresh();
	}
    void scrollViewDidZoom(CCScrollView* view){}
	int getTotalPrice();
	int getRealPrice();
	int getTrueCount();
	void refreshPrice();
	std::map<std::string, GameShop::ITEM_SELTYPE> getSelectedItems();
	
	CC_SYNTHESIZE(ChallengeItemLayer*,  itemLayer, ItemLayer);
};

#endif

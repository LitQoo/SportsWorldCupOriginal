//
//  FacebookPost.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 5..
//
//

#ifndef __SportsWorldCup__FacebookPost__
#define __SportsWorldCup__FacebookPost__

#include "cocos2d.h"
#include "LQTableView.h"
#include "UIMediator.h"

USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;
class IntroHead;
class IntroMain;
class FacebookPost : public CCLayer, public UIColleague, public CCTableViewDataSource, public CCTableViewDelegate
{
private:
	JsonBox::Array fbMailList;
	CCTableView* postBoxView;
//	LQTableView* postBoxView;
	
	CCSprite* fbLoading;
	CCSprite* thiz;
	UIMediator* mediator;
private:
	void GETHEART(CCObject* item);
	void removeRequestFinish(JsonBox::Value v);
	
public:
	virtual ~FacebookPost();
	virtual bool init();
	static FacebookPost* create(UIMediator *m)
	{
		FacebookPost *pRet = new FacebookPost();
		pRet->setMediator(m);
		if (pRet && pRet->init())
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
	}
	void loadRequest();
	void getRequestFinish(JsonBox::Value v2);
	void setMediator(UIMediator* m)
	{
		mediator = m;
	}
	void BACK(CCObject*);
	
	
	//////////////////////////////////////////
	virtual void tableCellHighlight(CCTableView* table, CCTableViewCell* cell);
    virtual void tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view) {};
    virtual void scrollViewDidZoom(CCScrollView* view) {}
    
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
};

class PostTableViewCell : public cocos2d::extension::CCTableViewCell
{
private:
	
public:
	PostTableViewCell()
	{
		profileSprite = 0;
		timeTTF = 0;
		_menu = 0;
		_back = 0;
		nameTTF = 0;
	}
	CCSprite* profileSprite;
	CCSprite* _back;
	CCLabelTTF* timeTTF;
	CCLabelTTF* nameTTF;
	CCMenu* _menu;
};

#endif /* defined(__SportsWorldCup__FacebookPost__) */

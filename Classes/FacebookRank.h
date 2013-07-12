//
//  FacebookRank.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 28..
//
//

#ifndef __SportsWorldCup__FacebookRank__
#define __SportsWorldCup__FacebookRank__

#include "cocos2d.h"
#include "LQTableView.h"
#include "IntroHead.h"
#include "IntroMain.h"

USING_NS_CC;
#include "cocos-ext.h"
USING_NS_CC_EXT;
class FacebookRank : public CCLayer, public UIColleague, public CCTableViewDataSource, public CCTableViewDelegate
{
private:
	CCTableView* ccView;
	
	JsonBox::Array fblist;
	CCMenuItem* postbox;
	CCMenuItem* facebookLogin;
	CCSprite* postboxNewSprite;
	
	CCSprite* thiz;
	CCLabelBMFont* reqCount;
	UIMediator* mediator;
	CCLabelBMFont* leftTimeFnt;
private:

		void INVITE(CCObject* item);
	void SENDHEART(CCObject* item);
	void sendHeartFinish(JsonBox::Value v);
	void invitationFinish(JsonBox::Value v);
	
	void facebookLoginFinish(JsonBox::Value v2);
	void POSTBOX(CCObject*);
	void FACEBOOKLOGIN();
	virtual void update(float dt);

public:
	void getRequestFakeFinish(JsonBox::Value v2);
	CCSprite* fbLoading;
	float leftTime_s;
	virtual bool init();
	static FacebookRank* create(UIMediator *m)
	{ 
		FacebookRank *pRet = new FacebookRank();
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
	void facebookGetScoresFinish(JsonBox::Value v2);
	void COUNTRYRANK(CCObject*);
//	void finishGetWeek(JsonBox::Object js);
	void getRequestCountAndTicket(JsonBox::Value v2);
	void setMediator(UIMediator* m)
	{
		mediator = m;
	}
	virtual ~FacebookRank();
	
	//tabview
    virtual void tableCellHighlight(CCTableView* table, CCTableViewCell* cell);
    virtual void tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view) {};
    virtual void scrollViewDidZoom(CCScrollView* view) {}
    
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
};


class CustomTableViewCell : public cocos2d::extension::CCTableViewCell
{
private:
	
public:
	CustomTableViewCell()
	{
		profileSprite = 0;
		nameTTF = 0;
		rankTTF = 0;
		_checked = 0;
		rankSprite = 0;
		_menu = 0;
		inviteMsg = 0;
		scoreBM = 0;
		_back = 0;
	}
	CCSprite* profileSprite;
	CCLabelTTF* nameTTF;
	CCLabelBMFont* rankTTF;
	CCSprite* _checked;
	CCSprite *rankSprite;
	CCMenu* _menu;
	CCLabelTTF* inviteMsg;
	CCLabelBMFont* scoreBM;
	CCSprite* _back;
	
	
};









#endif /* defined(__SportsWorldCup__FacebookRank__) */

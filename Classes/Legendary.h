//
//  Legendary.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 4. 5..
//
//

#ifndef __SportsWorldCup__Legendary__
#define __SportsWorldCup__Legendary__

#include "cocos2d.h"
#include "JsonBox.h"
#include "UIMediator.h"
#include "HWFromTo.h"
USING_NS_CC;

#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::create(); _A * layer = _A::create(); scene->addChild(layer); return scene; }
class Legendary : public CCSprite, public UIColleague
{
private:
	struct __POSITION{
		CCPoint rank[10];
		CCPoint flag[10];
		CCPoint user[10];
		CCPoint score[10];
		CCPoint time[10];
		CCPoint beginRankPosition;
		CCPoint endRankPosition;
	}position;
	CCSprite* thiz;
	UIMediator* mediator;

	void update(float dt);
	void CLOSE(CCObject*);
	CCSprite* loading;
public:
	Legendary();
	virtual ~Legendary();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	static Legendary* create(UIMediator *m)
	{
		Legendary *pRet = new Legendary();
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
	void setMediator(UIMediator* m)
	{
		mediator = m;
	}
	void appearMe();
	
	void loadRecord(std::string gameType);
public:
	void finishGetScores(JsonBox::Object js);
};

#endif /* defined(__SportsWorldCup__Legendary__) */

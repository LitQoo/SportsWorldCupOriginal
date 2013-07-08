//
//  CountryRank.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 28..
//
//

#include "CountryRank.h"
#include <sstream>
#include "KS_Util.h"
#include "Bridge.h"

#include "IntroMain.h"
#include "GraphDog.h"
#include "GameSystem.h"
#include "KSoundEngine.h"
using namespace std;


bool CountryRank::init()
{
	CCSprite::init();
	CCSprite* back = CCSprite::create("main_countryrank_back.png");
	
	addChild(back);
	thiz = back;
	
	CCMenu* _menu = CCMenu::create(0);
	_menu->setPosition(CCPointZero);
	thiz->addChild(_menu);

	{
		
		CCMenuItem* item = CCMenuItemImage::create("main_mini_facebookrank.png", "main_mini_facebookrank.png",
																this, menu_selector(CountryRank::FACEBOOKRANK));
		item->setPosition(ccp(194, 25));
		_menu->addChild(item, 2);
		item->runAction(
						CCRepeatForever::create( CCSequence::createWithTwoActions(CCEaseOut::create(CCMoveBy::create(0.8, ccp(0, 5)), 1.3),
																					CCEaseOut::create(CCMoveBy::create(0.8, ccp(0, -5)), 1.3))) );
	}
	
	userGraphs.resize(10);
	scoreGraphs.resize(10);
	
	for(int i=0; i<10; i++)
	{
		userGraphs[i] = CCProgressTimer::create(CCSprite::create("main_countryrank_user.png"));
//		userGraphs[i]->setMidpoint(CCPointZero);
		userGraphs[i]->setType(CCProgressTimerType::kCCProgressTimerTypeBar);
		userGraphs[i]->setMidpoint(ccp(1.f, 0.5f));
		userGraphs[i]->setBarChangeRate(ccp(1.f, 0));
//		userGraphs[i]->setReverseDirection(false);
		userGraphs[i]->setPosition(ccp(72, position.user[i].y));
		thiz->addChild(userGraphs[i], 2);
		
		scoreGraphs[i] = CCProgressTimer::create(CCSprite::create("main_countryrank_score.png"));
		scoreGraphs[i]->setBarChangeRate(ccp(1.f, 0.f));
		scoreGraphs[i]->setMidpoint(ccp(0, 0.5f));
//		scoreGraphs[i]->setReverseDirection(false);
		scoreGraphs[i]->setType(CCProgressTimerType::kCCProgressTimerTypeBar);
		scoreGraphs[i]->setPosition(ccp(192, position.user[i].y));
		thiz->addChild(scoreGraphs[i], 2);
	}
	return true;
}

void CountryRank::FACEBOOKRANK(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	mediator->showFacebookRank();
}
CountryRank::~CountryRank()
{
	//graphdog->removeCommand(this);
}
void CountryRank::finishGetFlagRank(JsonBox::Object js)
{
	ostringstream oss;
	oss << js << endl;
	KS::KSLog("%", oss.str().c_str());
	
	mediator->incLoadCount();
	if(js["state"].getString() != "ok")
		return;
	
	int maxUser = -1;
	int maxScore = -1;
	JsonBox::Array ranks = js["list"].getArray();
	for(int i=0; i<ranks.size() && i < 10; i++)
	{
		JsonBox::Object e = ranks[i].getObject();
		int score = e["score"].getInt();
		int users = e["user"].getInt();
		maxScore = MAX(maxScore, score);
		maxUser = MAX(maxUser, users);
	}
	for(int i=0; i<ranks.size() && i < 10; i++)
	{
		JsonBox::Object e = ranks[i].getObject();
		string flag = e["flag"].getString();
		int score;
		int users;
		score = e["score"].getInt();
		users = e["user"].getInt();
		userGraphs[i]->setPercentage((float)users / (float)maxUser * 100.f);
		scoreGraphs[i]->setPercentage((float)score / (float)maxScore * 100.f);
		CCLabelBMFont* u = CCLabelBMFont::create(KS_Util::stringWithFormat("%d", users).c_str(), "countryrank.fnt");
		u->setAnchorPoint(ccp(0, 0.5f));
		CCLabelBMFont* s = CCLabelBMFont::create(KS_Util::stringWithFormat("%d", score).c_str(), "countryrank.fnt");
		s->setAnchorPoint(ccp(1, 0.5f));
		flag = KS_Util::stringWithFormat("%s.png", flag.c_str());
		if(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(flag.c_str()))
		{
			CCSprite* f = CCSprite::createWithSpriteFrameName(flag.c_str());
			if(f)
			{
				f->setScale(0.8f);
				f->setPosition(position.flag[i]);
				thiz->addChild(f, 2);
			}
			
		}
		
		u->setPosition(position.user[i]);
		s->setPosition(position.score[i]);
		
		thiz->addChild(u, 2);
		thiz->addChild(s, 2);
		
		//CCSprite*
	}
	
	
}




//
//  GameOver.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 27..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "GameOver.h"
#include "GraphDog.h"
#include "GameSystem.h"
#include "BS2Play.h"
#include "HWPlay.h"
#include "ACPlay.h"
#include "AKPlay.h"
#include "WCPlay.h"
#include "SKPlay.h"
#include "KS_Util.h"

#include <sstream>
#include "Bridge.h"
#include "NSDefault.h"
#include "WCSaveData.h"
#include "AKSaveData.h"
#include "ACSaveData.h"
#include "BS2SaveData.h"
#include "HWSaveData.h"
#include "SKSaveData.h"
#include "KSModalAlert.h"
#include "FBConnector.h"
#include "SWIntro.h"

#include "HGPlay.h"
#include "HGGameScene.h"

#include "HGSaveData.h"
#include "GDTranslate.h"
using namespace GDTranslate;

void putScore(int& bScore, int obScore, int& bWeekly, int rank, UIMediator* mediator)
{
	string res;
//	CCLog("beforeScore : %d, obScore : %d", bScore, obScore);
	if(bScore < obScore)
	{
		bScore = obScore;
		bWeekly = obScore;
		res = "max";
		
	}
	else
	{
		if(bWeekly < obScore)
		{
			bWeekly = obScore;
			res = "weekly";
		}
		else
		{
			res = "myscore";
		}
	}
	mediator->showJudgeOnGameOver(res, rank, obScore, bScore, bWeekly);
}

GameOver::GameOver()
{
	// 테이블 좌표 잡아주기.
	int arrY[] = {171, 154, 137, 120, 103, 86, 69, 52, 35, 18};

	for(int i=0; i<10; i++)
	{
		position.rank[i].x = 15;
		position.flag[i].x = 40;
		position.user[i].x = 52;
		position.score[i].x = 220;
		position.time[i].x = 244;
	}
	for(int i=0; i<10; i++)
	{
		int Y = arrY[i];
		position.rank[i].y = Y;
		position.flag[i].y = Y;
		position.user[i].y = Y;
		position.score[i].y = Y;
		position.time[i].y = Y;
	}
	
	position.beginRankPosition = ccp(246, 215);
	position.endRankPosition = ccp(27, 215);
}
GameOver::~GameOver()
{
	GraphDog::get()->removeCommand(this); 
}
bool GameOver::init()
{
	CCSprite::init();
		
//	this->setTouchEnabled(true);
	
	thiz = CCSprite::create("gameover_back.png");
	addChild(thiz);
	
	me = CCSprite::create("gameover_me.png");
	me->setPosition(ccp(246, 215));
	thiz->addChild(me, 2);
	
	meFnt = CCLabelBMFont::create("0", "reachvalue.fnt");
	meFnt->setColor(ccc3(27, 69, 137));
	meFnt->setPosition(ccp(me->getContentSize().width / 2.f, 38));
	me->addChild(meFnt, 3);
	
	rankX.init(me->getPosition().x, me->getPosition().x, 1.f);
	rankPercent.init(100.f, 100.f, 1.f);
	

	

	
	
//	mediator->refreshEXP();
	schedule(schedule_selector(GameOver::update));
	return true;
}
void GameOver::update(float dt)
{
	rankX.step(dt);
	rankPercent.step(dt);
	me->setPosition(ccp(rankX.getValue(), me->getPosition().y));
	meFnt->setString(KS_Util::stringWithFormat("%.0f%%", (float)(rankPercent.getValue() * 100.f)).c_str());
}

void GameOver::appearMe()
{
	
	
	int score = 0;
	if(playInfo->currentGame == "AK")
	{
		score = akplay->gameScore.getVar();
	}
	else if(playInfo->currentGame == "BS")
	{
		score = bs2play->gameScore.getVar();
	}
	else if(playInfo->currentGame == "AC")
	{
		score = acplay->gameScore.getVar();
	}
	else if(playInfo->currentGame == "HW")
	{
		score = hwplay->gameScore.getVar();
	}
	else if(playInfo->currentGame == "SK")
	{
		score = skplay->gameScore.getVar();
	}
	else if(playInfo->currentGame == "HG")
	{
		score = hgplay->gameScore.getVar();
	}
	else if(playInfo->currentGame == "WORLDCUP")
	{
		score = wcplay->totalGameScore();
	}
	
	beforeEXP = NSDefault::getExp();
	int m = 1;
	if(playInfo->selectedItems["exp"] != GameShop::NONE)
	{
		m = 2;
		mediator->showExp2();
	}
	
	pair<int, int> lv_exp = playInfo->getLV_EXP(NSDefault::getExp());
	 // 경험치 얻을 수 있는 맥스치를 정해줌, 그리고 얻은 경험치를 어느 정도 절삭함.
	int getExp = MIN(playInfo->expFromLv(lv_exp.first), powf(score, 0.70f) * 10.f / 10000.f);
	NSDefault::setExp(NSDefault::getExp() + (getExp * m));
	afterEXP = NSDefault::getExp();
	
	mediator->refreshEXP();
	if(playInfo->getLV_EXP(beforeEXP).first < playInfo->getLV_EXP(afterEXP).first)
	{
		// level up! //##
		
		if(mediator->getHeartSize() < 5 && playInfo->getLV_EXP(afterEXP).first <= 10)
		{
			mediator->addHeartSprite();
			NSDefault::setHeartNumber(NSDefault::getHeartNumber() + 1);
		}
		
		mediator->showLevelUp();
	}
}
void GameOver::finishGetScores(JsonBox::Object js)
{
//	ostringstream oss;
//	oss << js << endl;
//	CCLog("**********************************************************************\n%s", oss.str().c_str());
	
	try {
//		KS::KSLog("%", js);
		KS::KSLog("%", js["state"].getString().c_str());
		if(js["state"].getString() != "ok")
			throw string(gt("retrying").c_str());
		mediator->incLoadCount();
		CCLabelBMFont* allUsers = CCLabelBMFont::create(KS_Util::stringWithFormat("%d", js["alluser"].getInt()).c_str(), "main1.fnt");
//		allUsers->setColor(ccc3(0,0,0));
		allUsers->setPosition(ccp(226, 264));
		thiz->addChild(allUsers, 2);
		
		
		
		JsonBox::Array jList = js["list"].getArray();
		int st = js["timestamp"].getInt();
		int z = jList.size();

		float myrank = js["myrank"].getInt();
		int myscore = 0;
		int week = playInfo->currentWeek;
		for(int i=0; i<jList.size() && i < 10; i++)
		{
			JsonBox::Object entry = jList[i].getObject();
			string nick = entry["nick"].getString();
			int score = entry["score"].getInt();
			int duration;
			int rank = entry["rank"].getInt();
			
			duration = entry["playtime"].getInt();
//			if(entry["etime"].getInt() == 0) // 안끝났다면
//			{
//				duration = st - entry["stime"].getInt();
//			}
//			else
//			{
//				duration = entry["etime"].getInt() - entry["stime"].getInt();
//			}

			string mmss;
			if(duration < 3600)
			{
				mmss = KS_Util::stringWithFormat("%2d'%2d\"", (int)(duration / 60), duration % 60);
			}
			else
			{
				int hours = duration / 3600;
				if(hours > 100)
					mmss = KS_Util::stringWithFormat("inf", hours);
				else if(hours > 1)
					mmss = KS_Util::stringWithFormat("%d hrs", hours);
				else
					mmss = KS_Util::stringWithFormat("%d hr", hours);
			}
			
			CCLabelTTF* rankFnt;
			if(!(1 <= rank && rank <= 3))
			{
				rankFnt = CCLabelTTF::create(KS_Util::stringWithFormat("%d", entry["rank"].getInt()).c_str(), defaultFont, 12.f);
				rankFnt->setColor(ccc3(0, 0, 0));
				rankFnt->setPosition(position.rank[i]);
				thiz->addChild(rankFnt, 2);
			}
			CCSprite* flag = CCSprite::createWithSpriteFrameName(KS_Util::stringWithFormat("%s.png", entry["flag"].getString().c_str()).c_str());
			if(flag)
			{
				flag->setPosition(position.flag[i]);
				thiz->addChild(flag, 2);
				flag->setScale(0.9f);
			}
			CCLabelTTF* nickTTF = CCLabelTTF::create(nick.c_str(), defaultFont, 12.f);
			nickTTF->setColor(ccc3(0, 0, 0));
			nickTTF->setAnchorPoint(ccp(0.f, 0.5f));
			nickTTF->setPosition(position.user[i]);
			
			
			thiz->addChild(nickTTF, 2);
			
			CCLabelBMFont* scoreFnt = CCLabelBMFont::create(KS_Util::stringWithFormat("%d", score).c_str(), "rankscore.fnt");
			scoreFnt->setPosition(position.score[i]);
			scoreFnt->setAnchorPoint(ccp(1.f, 0.5f));
			
			
			thiz->addChild(scoreFnt, 2);
			
			CCLabelTTF* timeFnt = CCLabelTTF::create(mmss.c_str(), defaultFont, 12.f);
			timeFnt->setColor(ccc3(0, 0, 0));
			timeFnt->setPosition(position.time[i]);
			if(entry["isme"].getBoolean())
			{
				scoreFnt->setColor(ccc3(255, 0, 0));
				nickTTF->setColor(ccc3(255, 0, 0));
				timeFnt->setColor(ccc3(255, 0, 0));
				if(!(1 <= rank && rank <= 3))
					rankFnt->setColor(ccc3(255, 0, 0));
				myscore = score;
			}
			thiz->addChild(timeFnt, 2);
		}

		if(playInfo->currentGame == "AC")
		{
			putScore(playInfo->acscore, myscore, playInfo->acweekly, myrank, mediator);
		}
		else if(playInfo->currentGame == "AK")
		{
			putScore(playInfo->akscore, myscore, playInfo->akweekly, myrank, mediator);
		}
		else if(playInfo->currentGame == "BS")
		{
			putScore(playInfo->bsscore, myscore, playInfo->bsweekly, myrank, mediator);
		}
		else if(playInfo->currentGame == "HW")
		{
			putScore(playInfo->hwscore, myscore, playInfo->hwweekly, myrank, mediator);
		}
		else if(playInfo->currentGame == "SK")
		{
			putScore(playInfo->skscore, myscore, playInfo->skweekly, myrank, mediator);
		}
		else if(playInfo->currentGame == "HG")
		{
			putScore(playInfo->hgscore, myscore, playInfo->hgweekly, myrank, mediator);
		}
		else  if(playInfo->currentGame == "WORLDCUP")
		{
			putScore(playInfo->wcscore, myscore, playInfo->wcweekly, myrank, mediator);
		}
		CCLog("%d", playInfo->skscore);
		mediator->sendFacebookScore();
		rankPercent.init(1.f, (myrank / (float)js["alluser"].getInt()), 2.f);
		KS::KSLog("% % % ", position.endRankPosition.x, (position.beginRankPosition.x - position.endRankPosition.x), (myrank / (float)js["alluser"].getInt()));
		rankX.init(me->getPosition().x, position.endRankPosition.x + (position.beginRankPosition.x - position.endRankPosition.x) * (myrank / (float)js["alluser"].getInt()),
				   2.f);
		

		


		
	} catch (const string& msg) {
		mediator->showFailedGameOver(msg);
		
	}
	
	
	
}


void GameOver::onEnterTransitionDidFinish()
{
	CCSprite::onEnterTransitionDidFinish();
}
void GameOver::onExit()
{
	CCSprite::onExit();
}



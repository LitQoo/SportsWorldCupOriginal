//
//  SWIntro.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 1. 23..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCup_SWIntro_cpp
#define SportsWorldCup_SWIntro_cpp
#include "MoreApps.h"
#include "JoinMembers.h"
#include "GiftCode.h"
#include "HWPlayer.h"
#include "HWFromTo.h"
#include "IntroHead.h"
#include "IntroMain.h"
#include "JsonBox.h"
#include "UIMediator.h"
#include "FacebookRank.h"
#include "CountryRank.h"
#include "FacebookPost.h"
#include "Option.h"
#include "DisableTouch.h"
#include "MyMedal.h"
#include "GameSelect.h"
#include "CCMenuItemImageDown.h"
#include "GameOver.h"

#include "KSModalAlert.h"
#include "ACSaveData.h"
#include "AKSaveData.h"
#include "BS2SaveData.h"
#include "HWSaveData.h"

#include "cocos2d.h"

#include <sstream>

class Legendary; class SkiShop; class HurdleShop; class BasketShop; class ClayShop; class ArcheryShop;
class HanggShop; class HeartShop; class GoldShop; class RubyShop; class MyScore;
class ChallengeShop;


USING_NS_CC;
class SWIntro : public CCLayer, public CCTextFieldDelegate, public UIMediator
{
public:
	enum INIT_MODE initMode;
	SWIntro();
	virtual ~SWIntro();
	virtual bool init();

	void afterInit();
	virtual void onEnterTransitionDidFinish();
	virtual void onEnter();
	virtual void onExit();
	static CCScene* scene(enum INIT_MODE e) {
		CCScene* scene = CCScene::create();
		SWIntro * layer = SWIntro::create(e);
		scene->addChild(layer);
		return scene;
	}
	static SWIntro* create(enum INIT_MODE e)
	{
		SWIntro *pRet = new SWIntro();
		pRet->initMode = e;
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
	void finishGetWeek(JsonBox::Object js);
	virtual void showFacebookRank();
	virtual void showBackToFacebook();
	virtual void showCountryRank();
	virtual void showFacebookPost();
	virtual void showOption();
	virtual void showGameSelect();
	virtual void showLegendaryRecord(string gameType);
	virtual void showGameOver();
	virtual void showFailedGameOver(const string& msg);
	virtual void showAlert(const string& msg);
	void showAlert(CCObject* str);
	virtual void showRubyShop();
	virtual void showGoldShop();
	virtual void showHeartShop();
	virtual void showArcheryShop();
	virtual void showHurdleShop();
	virtual void showBasketShop();
	virtual void showClayShop();
	virtual void showSkiShop();
	virtual void showHgShop();
	virtual void showChallengeShop();
	virtual void showGiftCode();
	virtual void showMoreApps();
	virtual void sendFacebookScore();
	virtual void showJudgeOnGameOver(string result, int rank, int score, int max, int weekly);
	virtual void showLevelUp();
	virtual void showExp2();
	virtual void refreshEXP();
	virtual void showWorldcupMark(bool);
	virtual void incLoadCount();
	virtual int getHeartSize(){
		return introHead->getHeartSize();
	}
	virtual void addHeartSprite(){
		introHead->addHeartSprite();
	}
	void refreshGold(CCObject*);
	void onRequestTicket(CCObject*);
	void onRecvRequestTicket(CCObject*);
	void onRecvTicket(CCObject*);
	void openNewGameEffect(CCObject*);
	void onGameOpenWithFriends(CCObject*);
//	virtual void setGold(int n)
//	{
//		introHead->setGold(n);
//	}
	virtual void setRuby(int n);
	virtual void setFacebookLeftTime(int s);
	virtual enum INIT_MODE getInitMode(){return initMode;}
	virtual int getCurrentTime_s();
	void finishFacebookSendScore(JsonBox::Value v);
private:
	bool checkBeforeMoving();
	void goReview();
	void deleteSprite(CCNode* _n)
	{
		_n->getParent()->removeChild(_n, true);
	}
		CCMenuItemImageDown* startGameBtn, *moreapps, *option, *home, *restartGame;
	CCSprite* gameOn;
	CCSprite* loading;
	CCSprite* titleBack;
	CCSprite* title;
	CCSprite* worldcupMark;
	int loadCount;
	bool bLoaded;
	CCNode* catchingColleague;
	IntroHead* introHead;
	FacebookRank* facebookRank;
	CountryRank* countryRank;
	FacebookPost* postBox;
	Option* optionWindow;
	MyMedal* medalWindow;
	GameSelect* gameSelect;
	GameOver* gameOverWindow;
	GiftCode* giftCodeWindow;
	MoreApps* moreAppsWindow;
	MyScore* myScoreWindow;
	RubyShop* rubyWindow;
	GoldShop* goldWindow;
	HeartShop* heartWindow;
	ArcheryShop* archeryWindow;
	BasketShop* basketWindow;
	HurdleShop* hurdleWindow;
	ClayShop* clayWindow;
	SkiShop* skiWindow;
	ChallengeShop* challengeWindow;
	HanggShop* hanggWindow;
	Legendary* legendaryWindow;
	JoinMembers* joinMembers;
	string houseAdUrl;
	string storeLocation;
	string aid;
	string gameType; // 게임 시작시에 임시 저장할 변수.
	bool isComplete;
	std::queue<pair<MemoryStruct, string>> chunks;
	void START(CCObject*);
	void UPDATE_OK();
	void startGame(CCNode* colleague, JsonBox::Object param);
	void RESTART(CCObject*);
	void HOME(CCObject*);
	void MOREAPPS(CCObject*);
	void OPTION(CCObject*);
	
	
	void RETRY();
	void GAMEOVERRETRY();
	void finishInfo(JsonBox::Object js);
	void finishStartScores(JsonBox::Object js);
	void finishStartScores2(CCObject*);
	void finishGetHouseAd(JsonBox::Object js);
	void finishGetUpdate(JsonBox::Object js);
	void finishGetLastestScores(JsonBox::Object js);
	void getScores(JsonBox::Object js);
	void showRank();
	void showRankWithMenu();
	static void* t_function(void* _caller);
	static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
	void imageDown(float dt);
	void foldingMenus();
	void disfoldingMenus();
	
	struct _PositionTag
	{
		CCPoint COUNTRYRANK_FRAME;
		CCPoint FACEBOOK_FRAME;
		CCPoint MEDAL_FRAME;
		CCPoint GOLD_NUMBER;
		CCPoint SILVER_NUMBER;
		CCPoint BRONZE_NUMBER ;
		CCPoint T10_NUMBER ;
		CCPoint MOREAPPS ;
		CCPoint OPTION ;
		CCPoint BASKETGAME;
		CCPoint ARCHERYGAME;
		CCPoint HURDLEGAME;
		CCPoint CLAYGAME;
		CCPoint FIRSTFLAG ;
		CCPoint SECONDFLAG;
		CCPoint THIRDFLAG ;
		
		CCPoint FIRSTPT ;
		CCPoint FIRSTUSER ;
		
		CCPoint SECONDPT ;
		CCPoint SECONDUSER;
		
		CCPoint THIRDPT;
		CCPoint THIRDUSER ;
		
		CCPoint MAXSCORE;
		CCPoint WEEKLYMAX;
		CCPoint START ;
		CCPoint TOLEFT ;
		
		CCPoint HOME;
		CCPoint GAMEOVER_HOME;
		CCPoint RESTART;
		CCPoint CUP;
		CCPoint MYSCORE;
		CCPoint EVENT;
		CCPoint CONTENT;
		CCPoint INTROHEAD;
		_PositionTag()
		{
			INTROHEAD = ccp(0, 288);
			CONTENT = ccp(143, 144);
			EVENT = ccp(380, 144);
			COUNTRYRANK_FRAME = ccp(587, 145);
			FACEBOOK_FRAME = ccp(241, 145);
			MEDAL_FRAME = ccp(378, 213);
			GOLD_NUMBER = ccp(52, 202);
			SILVER_NUMBER = ccp(52, 145);
			BRONZE_NUMBER = ccp(52, 87);
			T10_NUMBER = ccp(52, 29);
			MOREAPPS = ccp(330, 107);
			OPTION = ccp(428, 107);
			BASKETGAME = ccp(436, 245);
			ARCHERYGAME = ccp(436, 179);
			HURDLEGAME = ccp(436, 113);
			CLAYGAME = ccp(436, 47);
			FIRSTFLAG = ccp(529, 229);
			SECONDFLAG = ccp(529, 186);
			THIRDFLAG = ccp(529, 143);
			
			FIRSTPT = ccp(610, 233);
			FIRSTUSER = ccp(595, 218);
			
			SECONDPT = ccp(610, 191);
			SECONDUSER = ccp(595, 175);
			
			THIRDPT = ccp(610, 148);
			THIRDUSER = ccp(595, 133);
			
			MAXSCORE = ccp(552, 80);
			WEEKLYMAX = ccp(552, 33);
			START = ccp(378, 46);
			TOLEFT = ccp(372, 153);
			
			HOME = ccp(378, 112);
			CUP = ccp(379, 186);
			MYSCORE = ccp(379, 194);
			
			GAMEOVER_HOME = ccp(378, 88);
			RESTART = ccp(378, 32);
		}
	}position;

};

#endif





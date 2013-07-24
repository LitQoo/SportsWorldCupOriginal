//
//  SWIntro.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 1. 23..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "SWIntro.h"
#include "JoinMembers.h"
#include "SKGameScene.h"
#include "CCImage2.h"
#include "Tutorial.h"
#include "GameSystem.h"
#include "LoadingScene.h"
#include "WCPlay.h"
#include "WCSaveData.h"
#include "BS2Play.h"
#include "AKPlay.h"
#include "HGPlay.h"
#include "HWPlay.h"
#include "SKPlay.h"
#include "BS2BasketGame.h"
#include "AKAngryGameScene.h"
#include "HWFirstGameScene.h"
#include "HGGameScene.h"
#include "ACGameScene.h"
#include "HWGameDirector.h"
#include "ACPlay.h"
#include "AKSaveData.h"
#include "ACSaveData.h"
#include "BS2SaveData.h"
#include "HWSaveData.h"
#include "KSModalAlert.h"
#include "Bridge.h"
#include "KSBlock.h"
#include "HWFromTo.h"
#include "GraphDog.h"
#include "KSBlock.h"
#include "GameSystem.h"
#include "Bridge.h"
#include "JNIKelper.h"
#include "HouseAd.h"
#include "KSoundEngine.h"
#include "SceneUtil.h"
#include "ChallengeShop.h"
#include "CheckDaily.h"
#include "Ticket.h"
#include "MyScore.h"
#include "RubyShop.h"
#include "GoldShop.h"
#include "HeartShop.h"
#include "HanggShop.h"
#include "ArcheryShop.h"
#include "ClayShop.h"
#include "BasketShop.h"
#include "HurdleShop.h"
#include "SkiShop.h"
#include "Legendary.h"

using namespace std;
#include "GDTranslate.h"
using namespace GDTranslate;
SWIntro::SWIntro()
{
	loadCount = 0;
	bLoaded = false;
	catchingColleague = 0;
	
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ThisClassType::finishStartScores2), "finishStartScores2", 0);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SWIntro::showAlert), "showAlert", 0);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SWIntro::refreshGold), "refreshGold", 0);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SWIntro::openNewGameEffect), "openNewGameEffect", 0);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SWIntro::onRequestTicket), "onRequestTicket", 0);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SWIntro::onRecvRequestTicket), "onRecvRequestTicket", 0);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SWIntro::onRecvTicket), "onRecvTicket", 0);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SWIntro::onGameOpenWithFriends), "onGameOpenWithFriends", 0);
}
SWIntro::~SWIntro()
{
	FBDelegator::getInstance()->removeTarget(this);
	
	CCArray* children = this->getChildren();
	for(int i=0; i<children->count(); i++)
	{
		CCObject* tempNode = children->objectAtIndex(i);
		if(tempNode != 0 && 1<tempNode->retainCount())
		{
			while(1<tempNode->retainCount())
				tempNode->release();
		}
	}
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	CCTextureCache::sharedTextureCache()->removeAllTextures();
	
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

void SWIntro::finishGetLastestScores(JsonBox::Object js)
{
	incLoadCount();
	/*
	 "action" : "getscoreinfo",
	 "data" : {
	 "week" : 26,
	 "weekly" : {
	 "HG" : 2800
	 }
	 },
	 "param" : {},
	 "state" : "ok"
	 */
	try {
		//
		
		if(js["state"].getString() != "ok")
		{
			throw string(gt("netfail").c_str());
		}
//		js["data"].getObject()
		
		KS::KSLog("%", js);
		JsonBox::Object data = js["data"].getObject();
		JsonBox::Object weekly = data["weekly"].getObject();
		JsonBox::Object _max = data["max"].getObject();
		int wHG = weekly["HG"].getInt();
		int wSK = weekly["SK"].getInt();
		int wAK = weekly["AK"].getInt();
		int wAC = weekly["AC"].getInt();
		int wBS = weekly["BS"].getInt();
		int wHW = weekly["HW"].getInt();
		int wWC = weekly["WORLDCUP"].getInt();
		
		int HG = _max["HG"].getInt();
		int SK = _max["SK"].getInt();
		int AK = _max["AK"].getInt();
		int AC = _max["AC"].getInt();
		int BS = _max["BS"].getInt();
		int HW = _max["HW"].getInt();
		int WC = _max["WORLDCUP"].getInt();
		playInfo->hgscore = HG;
		playInfo->hgweekly = wHG;
		
		playInfo->skscore = SK;
		playInfo->skweekly = wSK;
		
		playInfo->akscore = AK;
		playInfo->akweekly = wAK;
		
		playInfo->acscore = AC;
		playInfo->acweekly = wAC;
		
		playInfo->bsscore = BS;
		playInfo->bsweekly = wBS;
		
		playInfo->hwscore = HW;
		playInfo->hwweekly = wHW;
		
		playInfo->wcscore = WC;
		playInfo->wcweekly = wWC;
	}
	catch (const string& msg) {
		
	}
}
void SWIntro::finishGetUpdate(JsonBox::Object js)
{
	incLoadCount();
	try {
		KS::KSLog("%", js);
		if(js["state"].getString() != "ok")
		{
			throw string(gt("netfail").c_str());
		}
		JsonBox::Array myList = js["list"].getArray();
		if(myList.size() <= 0)
		{
			return;
		}
		JsonBox::Object list0 = myList[0].getObject();
		JsonBox::Object userdata = list0["userdata"].getObject();
		int need = userdata["need"].getInt();
		int lastestVersion = userdata["version"].getInt();
		int currentVersion = graphdog->getAppVersion();
		
		if(lastestVersion > currentVersion) // 업데이트 해야한다
		{
			if(need > currentVersion) // 필수 인가
			{
				CCLog("must");
				KSModalAlert("", gt("mustupdate").c_str(), 1,
							 "OK", this, callfunc_selector(ThisClassType::UPDATE_OK));
			}
			else
			{
				CCLog("not must");
				KSModalAlert("", gt("update").c_str(), 2,
							 "NO", NOPARAM,
							 "YES", this, callfunc_selector(ThisClassType::UPDATE_OK));
			}
		}
		else // 업데이트 필요없다
		{
			JsonBox::Object param;
			param["rid"] = list0["rid"].getInt();
			graphdog->command("removerequest", &param, 0, 0);
		}
	}
	catch (const string& msg) {
		
	}
}
void SWIntro::UPDATE_OK()
{
	KSModalAlert("", gt("mustupdate").c_str(), 1,
				 "OK", this, callfunc_selector(ThisClassType::UPDATE_OK));
//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//	write_review();
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//	::otherApp("595879505");
//#endif
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	GraphDogLib::openAppStore("595879505");
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	::otherApp("595879505");
#endif
}
void SWIntro::finishGetHouseAd(JsonBox::Object js)
{
	
	try {
		KS::KSLog("%", js);
		
		if(js["state"].getString() != "ok")
		{
			incLoadCount();
			throw string("");
		}
		
		JsonBox::Array myList = js["list"].getArray();
		if(myList.size() <= 0)
		{
			incLoadCount();
			return;
		}
		JsonBox::Object list0 = myList[0].getObject();
		JsonBox::Object userdata = list0["userdata"].getObject();
		CCLog("%s", userdata["image"].getString().c_str());
		houseAdUrl = userdata["image"].getString();
		storeLocation = userdata["storeid"].getString();
		aid = userdata["aid"].getString();
		pthread_t p_thread;
		if(pthread_create(&p_thread, NULL, t_function, this) == 0)
		{
			incLoadCount();
			throw string("It can not create thread");
		}
	} catch (const string& msg) {
		
//		KSModalAlert("", msg.c_str(), 1,
//					 "OK", NOPARAM);
	}

}



size_t SWIntro::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
    
    mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
    
    if (mem->memory == NULL) {
        /* out of memory! */
        KS::KSLog("not enough memory (realloc returned NULL)");
        exit(EXIT_FAILURE);
    }
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

void* SWIntro::t_function(void* _caller)
{
	SWIntro* caller = (SWIntro*)_caller;
	
	CURLcode retCode = curl_global_init(CURL_GLOBAL_ALL);
	if(retCode != CURLcode::CURLE_OK)
		return NULL;
	CURL *curl_handle = curl_easy_init();
	
	//while (1)
//	int i = 0;
	do{
		string pURL = caller->houseAdUrl;
		MemoryStruct chunk = {(char*)malloc(1), 0};
		
		curl_easy_setopt(curl_handle, CURLOPT_URL, pURL.c_str());
		curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, true);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
		if(curl_easy_perform(curl_handle) != CURLcode::CURLE_OK)
			return NULL;
		caller->chunks.push(pair<MemoryStruct, string>(chunk, caller->storeLocation));
		
		//CCScheduler::sharedScheduler()->scheduleSelector(schedule_selector(MoreApps::imageDown), caller, 0,false);
		//CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(FBConnector::imageDown), FBConnector::get(), 0, false, 0, 0);
	}while(0); // 반복문이 필요없어서 이렇게 대체... 나중에 필요하면 바꿈.
	caller->isComplete = true;
	caller->incLoadCount();
	curl_easy_cleanup(curl_handle);
	curl_global_cleanup();
	return NULL;
	
}

void SWIntro::imageDown(float dt)
{
	while(!chunks.empty())
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		CCImage* img = new CCImage;
		CCImage* img2 = new CCImage;
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		CCImage2* img = new CCImage2; //in cocos2d-x 1.x
		CCImage2* img2 = new CCImage2; //in cocos2d-x 1.x
#endif
		CCTexture2D* texture = new CCTexture2D();
		CCTexture2D* texture2 = new CCTexture2D();
		auto chunk_index = chunks.front();
		try {			
			auto chunk_index = chunks.front();
			
			if(img->initWithImageData(chunk_index.first.memory, (long)chunk_index.first.size, CCImage::kFmtUnKnown) == false)
				throw "";
			if(img2->initWithImageData(chunk_index.first.memory, (long)chunk_index.first.size, CCImage::kFmtUnKnown) == false)
				throw "";
			
			if(texture->initWithImage(img) == false)
				throw "";
			if(texture2->initWithImage(img2) == false)
				throw "";
			CCSprite* sprite = CCSprite::createWithTexture(texture);
			CCSprite* sprite2 = CCSprite::createWithTexture(texture2);
//			sprite->setScale(2.f);
//			sprite2->setScale(2.f);
			
//			sprite->setPosition(ccp(240, 160));
//			addChild(sprite, 3);
//			CCSprite* xButton = CCSprite::create("cancel_down.png");
//			xButton->setPosition(ccp(sprite->getContentSize().width, sprite->getContentSize().height));
//			sprite->addChild(xButton);
			HouseAd* had = HouseAd::create();
			had->setAd(sprite, sprite2, storeLocation, aid);
			addChild(had, INT_MAX);
			
			texture->release();
			delete img;// in cocos2d-x 1.x
			if(chunk_index.first.memory)
				free(chunk_index.first.memory);
			chunks.pop();
		} catch (...) {
			texture->release();
			delete img;// in cocos2d-x 1.x
			if(chunk_index.first.memory)
				free(chunk_index.first.memory);
			chunks.pop();
		}
	}
}
void SWIntro::afterInit()
{
	CCLog("afterInit");
	
	
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
	/*
	 Home Trip.mp3");
	 Retro Space.mp3");
	 Let me Fly.mp3");
	 Jacky.mp3");
	 */
	//	KSoundEngine::sharedEngine()->playSound("Home Trip.mp3");
	KSoundEngine::sharedEngine()->playSound("RetroSpace.mp3");
	CCLog("AfterInit Exit %d", __LINE__);
	//	KSoundEngine::sharedEngine()->playSound("Let me Fly.mp3");
	//	KSoundEngine::sharedEngine()->playSound("Jacky.mp3");
	
	if(initMode == INIT_MODE::STARTUP)
	{
		CCLog("AfterInit Exit %d", __LINE__);
		schedule(schedule_selector(SWIntro::imageDown));
		CCLog("%s %d", __FILE__, __LINE__);
		GraphDog::get()->setFlag(NSDefault::getCountry());
		CCLog("%s %d", __FILE__, __LINE__);
		GraphDog::get()->setNick(NSDefault::getUserName());
		CCLog("%s %d", __FILE__, __LINE__);
		GraphDog::get()->setup("sportsworldcup", "GDSK3388", "com.litqoo.lib", 16);
		CCLog("AfterInit Exit %d", __LINE__);
		CCLog("end setup %s", KS::getLocalCode().c_str());
		//		graphdog->setLanguage(KS::getLocalCode());
		//////////////
		vector<CommandParam> vcp;
		{
			JsonBox::Object param;
			//			JsonBox::Array category;
			param["category"] = "housead";
			param["limit"] = 1;
			param["autoremove"] = true;
			vcp.push_back(CommandParam("getrequests", param, this, gd_selector(ThisClassType::finishGetHouseAd)));
		}
		{
			JsonBox::Object param;
			//			JsonBox::Array category;
			param["category"] = "update";
			param["autoremove"] = false;
			vcp.push_back(CommandParam("getrequests", param, this, gd_selector(ThisClassType::finishGetUpdate)));
		}
		{
			JsonBox::Object empty;
			vcp.push_back(CommandParam("getscoreinfo", empty, this, gd_selector(ThisClassType::finishGetLastestScores)));
		}
		graphdog->command(vcp);
		
		//		param["limit"] = 1;
	}
	//this->setTouchEnabled(true);
	CCSprite* mainBack = CCSprite::create("main_back.png");
	mainBack->setPosition(ccp(240, 160));
	addChild(mainBack);
	//CCSize aa = CCEGLView::sharedOpenGLView().getSize();
	CCLog("AfterInit Exit %d", __LINE__);
	gameOverWindow = GameOver::create(this);
	gameOverWindow->setPosition(ccp(-400, 100));
	addChild(gameOverWindow, 3);
	
	giftCodeWindow = 0;
	
	
	moreAppsWindow = 0;
	
	
	introHead = IntroHead::create(this);
	playInfo->pIntroHead = introHead;
	introHead->syncTimeWithInc();
	addChild(introHead, 200);
	
	
	
	
	countryRank = CountryRank::create(this);
	countryRank->setPosition(ccp(143, 500));
	addChild(countryRank, 3);
	
	postBox = FacebookPost::create(this);
	postBox->setPosition(ccp(200, 500));
	addChild(postBox, 3);
	
	optionWindow = Option::create(this);
	optionWindow->setPosition(ccp(200, 500));
	addChild(optionWindow, 3);
	
	medalWindow = 0;
	
	
	gameSelect = 0;
	legendaryWindow = 0;
	
	
	
	myScoreWindow = MyScore::create(this);
	myScoreWindow->setPosition(ccp(position.MYSCORE.x + 250, position.MYSCORE.y));
	addChild(myScoreWindow, 3);
	CCLog("AfterInit Exit %d", __LINE__);
	rubyWindow = 0;
	goldWindow = 0;
	heartWindow = 0;
	archeryWindow = 0;
	basketWindow = 0;
	hurdleWindow = 0;
	clayWindow = 0;
	skiWindow = 0;
	hanggWindow = 0;
	challengeWindow = 0;
	CCMenu* _menu = CCMenu::create(0);
	_menu->setPosition(CCPointZero);
	addChild(_menu, 3);
	{
		startGameBtn = CCMenuItemImageDown::itemFromNormalImage("main_start.png", "main_start.png",
																this, menu_selector(ThisClassType::START));

		startGameBtn->setPosition(ccp(position.START.x, position.START.y - 200));
		_menu->addChild(startGameBtn, 3);
		
		worldcupMark = CCSprite::create("worldcup_startgold.png");
		worldcupMark->setPosition(ccp(146, 18));
		startGameBtn->addChild(worldcupMark, 2);
		worldcupMark->setVisible(false);
		
	}
	
	{
		moreapps = CCMenuItemImageDown::itemFromNormalImage("main_moreapps.png", "main_moreapps.png",
															this, menu_selector(ThisClassType::MOREAPPS));
		moreapps->setPosition(ccp(position.MOREAPPS.x + 300, position.MOREAPPS.y));
		
		
		_menu->addChild(moreapps, 3);
	}
	{
		option = CCMenuItemImageDown::itemFromNormalImage("main_option.png", "main_option.png",
														  this, menu_selector(ThisClassType::OPTION));
		option->setPosition(ccp(position.OPTION.x + 300, position.OPTION.y));
		_menu->addChild(option, 3);
	}
	{
		home = CCMenuItemImageDown::itemFromNormalImage("gameover_home.png", "gameover_home.png",
														this, menu_selector(ThisClassType::HOME));
		home->setPosition(ccp(position.HOME.x + 300, position.HOME.y));
		_menu->addChild(home, 3);
	}
	titleBack = CCSprite::create("main_titleback.png");
	titleBack->setPosition(ccp(240, 160));
	titleBack->runAction(CCRepeatForever::create(CCRotateBy::create(2.f*1.f/0.33f, 360.f)));
	addChild(titleBack, 2);
	title = CCSprite::create("main_title.png");
	title->setPosition(ccp(240, 160));
	addChild(title, 2);
	
	facebookRank = FacebookRank::create(this);
	facebookRank->setPosition(ccp(143, 500));
	addChild(facebookRank, 3);
	CCLog("AfterInit Exit %d", __LINE__);
	string newGameFileName;
	if(initMode == INIT_MODE::STARTUP)
	{
		
		vector<CommandParam> vcp;
		{
			JsonBox::Object param;
			param["type"] = "WORLDCUP";
			vcp.push_back(CommandParam("getflagranks", param, countryRank, gd_selector(CountryRank::finishGetFlagRank)));
		}
		{
			vcp.push_back(CommandParam("getweek", 0, this, gd_selector(ThisClassType::finishGetWeek)));
		}
		graphdog->command(vcp);
		
		if(FBConnector::get()->isUsed() && FBConnector::get()->isLogin())
		{
			facebookRank->fbLoading->setVisible(true);
			FBConnector::get()->getRequests(facebookRank, fb_selector(FacebookRank::getRequestCountAndTicket));
			FBConnector::get()->getRequests(facebookRank, fb_selector(FacebookRank::getRequestFakeFinish));
			FBConnector::get()->getScores(facebookRank, fb_selector(FacebookRank::facebookGetScoresFinish));
		}
		
		newGameFileName = "gameover_newgame.png";
	}
	else if(initMode == INIT_MODE::GAMEOVER)
	{
		int gainScore;
		if(playInfo->currentGame == "BS")
		{
			gainScore = bs2play->gameScore.getVar();
		}
		else if(playInfo->currentGame == "AK")
		{
			gainScore = akplay->gameScore.getVar();
		}
		else if(playInfo->currentGame == "AC")
		{
			gainScore = acplay->gameScore.getVar();
		}
		else if(playInfo->currentGame == "HW")
		{
			gainScore = hwplay->gameScore.getVar();
		}
		else if(playInfo->currentGame == "SK")
		{
			gainScore = skplay->gameScore.getVar();
		}
		else if(playInfo->currentGame == "HG")
		{
			gainScore = hgplay->gameScore.getVar();
		}
		JsonBox::Object param;
		param["score"] = gainScore;
		param["isover"] = true;
		
		JsonBox::Object userdata;
		
		userdata["hw"] = hwplay->gameScore.getVar();
		userdata["ac"] = acplay->gameScore.getVar();
		userdata["ak"] = akplay->gameScore.getVar();
		userdata["bs"] = bs2play->gameScore.getVar();
		userdata["sk"] = skplay->gameScore.getVar();
		userdata["hg"] = hgplay->gameScore.getVar();
		
		JsonBox::Object gParam;
		gParam["category"] = "gameover_info";
		ostringstream oss;
		oss << playInfo->currentGame << ", " << "score : " << gainScore << ", 먹은 코인 " << playInfo->__ateCoin <<
			"레벨 : " << playInfo->getLV_EXP(NSDefault::getExp()).first << "경험치" << NSDefault::getExp() <<
			"가지고 있는 돈 : " << NSDefault::getGold() << "현재 하트" <<
			NSDefault::getHeartNumber() << "기기 정보" << graphdog->getDeviceInfo();
		gParam["log"] = oss.str();
		graphdog->command("writelog", &gParam, 0, 0);
//		userdata["gold"] = playInfo->__ateCoin;
//		userdata["level"] = playInfo->getLV_EXP(NSDefault::getExp()).first;
//		userdata["exp"] = NSDefault::getExp();
//		userdata["havingmoney"] = NSDefault::getGold();
//		userdata["heart"] = NSDefault::getHeartNumber();
//		userdata["device"] = graphdog->getDeviceInfo();
		string items;
		for(auto iter = playInfo->selectedItems.begin(); iter != playInfo->selectedItems.end(); ++iter)
		{
			if((*iter).second)
				items += (*iter).first + "/";
		}
//		userdata["items"] = items;
		
		
		
//		param["userdata"] = userdata;
		JsonBox::Object param2;
		param2["type"] = "WORLDCUP";
		
		vector<CommandParam> vcp;
		vcp.push_back(CommandParam("getscores", param, gameOverWindow, gd_selector(GameOver::finishGetScores)));
		vcp.push_back(CommandParam("getflagranks", param2, countryRank, gd_selector(CountryRank::finishGetFlagRank)));
		
		JsonBox::Object empty;
		vcp.push_back(CommandParam("getscoreinfo", empty, this, gd_selector(ThisClassType::finishGetLastestScores)));
		graphdog->command(vcp);
		newGameFileName = "gameover_newgame.png";
	}
	
	else if(initMode == INIT_MODE::WORLDCUP_GAMEOVER)
	{
		JsonBox::Object param;
		param["score"] = wcplay->totalGameScore();
		param["isover"] = true;
		JsonBox::Object userdata;
		userdata["hw"] = hwplay->gameScore.getVar();
		userdata["ac"] = acplay->gameScore.getVar();
		userdata["ak"] = akplay->gameScore.getVar();
		userdata["bs"] = bs2play->gameScore.getVar();
		userdata["sk"] = skplay->gameScore.getVar();
		userdata["hg"] = hgplay->gameScore.getVar();
		
//		for(auto i : wcplay->scoreEachGame)
//		{
//			CCLog("%s %d", i.first.c_str(), i.second);
//		}
//		
//		param["userdata"] = userdata;
		JsonBox::Object param2;
		param2["type"] = "WORLDCUP";
		
		vector<CommandParam> vcp;
		vcp.push_back(CommandParam("getscores", param, gameOverWindow, gd_selector(GameOver::finishGetScores))); //## 중요.
		vcp.push_back(CommandParam("getflagranks", param2, countryRank, gd_selector(CountryRank::finishGetFlagRank)));
		JsonBox::Object empty;
		vcp.push_back(CommandParam("getscoreinfo", empty, this, gd_selector(ThisClassType::finishGetLastestScores)));
		graphdog->command(vcp);
		
		
		
		newGameFileName = "worldcup_gameover_restart.png";
	}
	
	{
		restartGame = CCMenuItemImageDown::itemFromNormalImage(newGameFileName.c_str(), newGameFileName.c_str(),
															   this, menu_selector(ThisClassType::RESTART));
		restartGame->setPosition(ccp(position.RESTART.x, position.RESTART.y - 200));
		_menu->addChild(restartGame, 3);
	}
	//	CCLog("GraphDogLib::getDeviceInfo() ------- %s", GraphDogLib::getDeviceInfo().c_str());
	CCLog("AfterInit Exit %d", __LINE__);
}
bool SWIntro::init()
{
	CCLayer::init();
	runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.1f),
									   CCCallFunc::create(this, callfunc_selector(SWIntro::afterInit))));
	
	
	
		
	return true;
}

int SWIntro::getCurrentTime_s()
{
	return introHead->timestamp + (playInfo->getCurrentTime_s() - introHead->oldLocalTime);
}
void SWIntro::setRuby(int n)
{
	introHead->setRuby(n);
}
void SWIntro::refreshGold(CCObject*)
{
	introHead->setGold(NSDefault::getGold());
}
void SWIntro::setFacebookLeftTime(int s)
{
	facebookRank->leftTime_s = s;
}
bool SWIntro::checkBeforeMoving()
{
	if(optionWindow->isValidName() == false)
	{
		optionWindow->alertInvalidName();
		return false;
	}
	if(catchingColleague)
	{
		
		if(catchingColleague->numberOfRunningActions() == 0)
		{
			DisableTouch* cl = DisableTouch::create();
			addChild(cl);
			
			catchingColleague->runAction(CCSequence::create(CCMoveBy::create(0.5f, ccp(0, 287)), CCMoveBy::create(0.1f, ccp(0, 100)),
															KSFunc(cl->removeFromParentAndCleanup(true);),0 ));
		}
	}
	
	return true;
}
void SWIntro::goReview()
{
	write_review();
	NSDefault::setReview(1);
}
void SWIntro::finishFacebookSendScore(JsonBox::Value v)
{
	//if(FBConnector::get()->isUsed() && FBConnector::get()->isLogin())
	
	KS::KSLog("%", v);
	{
		facebookRank->fbLoading->setVisible(true);
		FBConnector::get()->getRequests(facebookRank, fb_selector(FacebookRank::getRequestCountAndTicket));
		FBConnector::get()->getRequests(facebookRank, fb_selector(FacebookRank::getRequestFakeFinish));
		FBConnector::get()->getScores(facebookRank, fb_selector(FacebookRank::facebookGetScoresFinish));
	}
}
void SWIntro::GAMEOVERRETRY()
{	
	int gainScore;
	if(playInfo->currentGame == "BS")
	{
		gainScore = bs2play->gameScore.getVar();
	}
	else if(playInfo->currentGame == "AK")
	{
		gainScore = akplay->gameScore.getVar();
	}
	else if(playInfo->currentGame == "AC")
	{
		gainScore = acplay->gameScore.getVar();
	}
	else if(playInfo->currentGame == "HW")
	{
		gainScore = hwplay->gameScore.getVar();
	}
	else if(playInfo->currentGame == "SK")
	{
		gainScore = skplay->gameScore.getVar();
	}
	else if(playInfo->currentGame == "WORLDCUP")
	{
		gainScore = wcplay->totalGameScore();
	}
	
	JsonBox::Object param;
	param["score"] = gainScore;
	param["isover"] = true;
	
	JsonBox::Object userdata;
	userdata["hw"] = hwplay->gameScore.getVar();
	userdata["ac"] = acplay->gameScore.getVar();
	userdata["ak"] = akplay->gameScore.getVar();
	userdata["bs"] = bs2play->gameScore.getVar();
	userdata["sk"] = skplay->gameScore.getVar();
	userdata["hg"] = hgplay->gameScore.getVar();
	JsonBox::Object gParam;
	gParam["category"] = "gameover_info";
	ostringstream oss;
	oss << playInfo->currentGame << ", " << "score : " << gainScore << ", 먹은 코인 " << playInfo->__ateCoin <<
	"레벨 : " << playInfo->getLV_EXP(NSDefault::getExp()).first << "경험치" << NSDefault::getExp() <<
	"가지고 있는 돈 : " << NSDefault::getGold() << "현재 하트" <<
	NSDefault::getHeartNumber() << "기기 정보" << graphdog->getDeviceInfo();
	gParam["log"] = oss.str();
	graphdog->command("writelog", &gParam, 0, 0);
	
//	userdata["gold"] = playInfo->__ateCoin;
//	userdata["level"] = playInfo->getLV_EXP(NSDefault::getExp()).first;
//	userdata["exp"] = NSDefault::getExp();
//	userdata["havingmoney"] = NSDefault::getGold();
//	userdata["heart"] = NSDefault::getHeartNumber();
//	userdata["device"] = graphdog->getDeviceInfo();
	string items;
	for(auto iter = playInfo->selectedItems.begin(); iter != playInfo->selectedItems.end(); ++iter)
	{
		if((*iter).second)
			items += (*iter).first + "/";
	}
	userdata["items"] = items;
	param["userdata"] = userdata;
	graphdog->command("getscores", &param, gameOverWindow, gd_selector(GameOver::finishGetScores));
}

void SWIntro::finishGetWeek(JsonBox::Object js)
{
	incLoadCount();
//	mediator->incLoadCount();
	if(js["state"].getString() != "ok")
		return;
	
	KS::KSLog("%", js);
	int dayofyear = js["dayofyear"].getInt();
	
	bool check = false;
	if(NSDefault::getLastAttendance() + 1 == dayofyear) // 마지막 출석일이 어제라면
	{
		NSDefault::setLastAttendance(dayofyear);
		NSDefault::setAttendanceSeq(NSDefault::getAttendanceSeq() + 1);
		check = true;
		if(NSDefault::getAttendanceSeq() == 8)
		{
			NSDefault::setAttendanceSeq(2); // 2 일차로 세팅.
		}
	}
	else
	{
		if(NSDefault::getLastAttendance() != dayofyear)
		{
			NSDefault::setAttendanceSeq(1); // 1일차
			NSDefault::setLastAttendance(dayofyear);
			check = false;
		}
	}
	
	if(check)
	{ // 출석체크
		CheckDaily* cd = CheckDaily::create();
		addChild(cd, 405);
	}
	
	playInfo->currentWeek = js["week"].getInt();
	playInfo->whenRecvLeftTime = GameSystem::getCurrentTime_s();
	facebookRank->leftTime_s = js["lefttime"].getInt();
//	setFacebookLeftTime(js["lefttime"].getInt());
	
}


void SWIntro::MOREAPPS(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	showMoreApps();
}
void SWIntro::OPTION(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	
	if(catchingColleague != optionWindow)
		showOption();	
}

void SWIntro::showRankWithMenu()
{
	if(medalWindow == 0)
	{
		medalWindow = MyMedal::create(this);
		medalWindow->setPosition(ccp(position.MEDAL_FRAME.x + 250, position.MEDAL_FRAME.y));
		addChild(medalWindow, 3);
	}
	medalWindow->runAction(CCMoveBy::create(0.3f, ccp(300, 0)));
	home->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.1f), CCMoveTo::create(0.3f, ccp(position.HOME.x + 300.f, position.HOME.y))));
	restartGame->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.01f), CCMoveBy::create(0.3f, ccp(0, -200))));
	startGameBtn->runAction(CCMoveTo::create(0.5f, position.START));
	title->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.2f), CCMoveTo::create(0.3f, position.CUP)));
	titleBack->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.2f), CCMoveTo::create(0.3f, position.CUP)));
	
	moreapps->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.3f), CCMoveTo::create(0.3f, position.MOREAPPS)));
	option->runAction(CCSequence::create(CCDelayTime::create(0.3f), CCMoveTo::create(0.3f, position.OPTION),
										  KSFunc(
												 {
													 startGameBtn->setEnabled(true);
												 }
												 ) ,0));
	myScoreWindow->runAction(CCMoveTo::create(0.3f, ccp(position.MYSCORE.x + 250, position.MYSCORE.y)));
	startGameBtn->setEnabled(false);
//	titleBack->runAction(CCFadeIn::create(0.5f));
//	title->runAction(CCFadeIn::create(0.5f));
	if(gameSelect)
		gameSelect->hideGames();
	showRank();
}
void SWIntro::showRank()
{
	if(FBConnector::get()->isUsed())
	{
		showFacebookRank();
	}
	else
	{
		int r = rand()%5;
		if(r == 0)
		{
			showFacebookRank();
		}
		else
		{
			showCountryRank();
		}
	}
	
}

void SWIntro::incLoadCount()
{
	loadCount++;
	if(bLoaded) // 이미 로드 되어있으면 별다른 조치를 안취함
		return;
	int loadCnt = 0;
	// 처음엔 country, getweek, syncwithinc
	// 게임오버 시 country, gameover 로드, syncTime 로드 그래서 3
	if(initMode == INIT_MODE::STARTUP)
	{
		loadCnt++; // country
		loadCnt++; // getweek
		loadCnt++; // syncwithinc
		loadCnt++; // 공지.
		loadCnt++; // 업데이트 확인.
		loadCnt++; // 최신의 나의 점수를 받아옴.
	}else
	{
		loadCnt++; // country
		loadCnt++; // gameover
		loadCnt++; // syncwithinc
	}
	
	bool loadComplete = false;
	if(FBConnector::get()->isUsed() && FBConnector::get()->isLogin())
	{
		if(loadCount >= loadCnt + 1) // facebook 때문에 + 1
		{
			loadComplete = true;
		}
	}
	else
	{
		if(loadCount >= loadCnt)
			loadComplete = true;
	}
	
	if(loadComplete)
	{
		if(initMode == INIT_MODE::STARTUP)
		{
			bLoaded = true;
			showRank();
			moreapps->setPosition(ccp(position.MOREAPPS.x + 300, position.MOREAPPS.y));
			option->setPosition(ccp(position.OPTION.x + 300, position.OPTION.y));
			startGameBtn->setPosition(ccp(position.START.x, position.START.y - 200));
			
			moreapps->runAction(CCMoveTo::create(0.5f, position.MOREAPPS));
			option->runAction(CCMoveTo::create(0.5f, position.OPTION));
			startGameBtn->runAction(CCMoveTo::create(0.5f, position.START));
			titleBack->runAction(CCMoveTo::create(0.5f, position.CUP));
			title->runAction(CCMoveTo::create(0.5f, position.CUP));
			
			introHead->runAction(CCMoveTo::create(0.5f, ccp(0, 288)));
		}
		else if(initMode == INIT_MODE::GAMEOVER || initMode == INIT_MODE::WORLDCUP_GAMEOVER)
		{
			bLoaded = true;
			showGameOver();
			
		}
	}
}
void SWIntro::HOME(CCObject*)
{
//	medalWindow->runAction(CCMoveBy::create(0.3f, ccp(300, 0)));
//	home->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.1f), CCMoveBy::create(0.3f, ccp(300, 0))));
//	restartGame->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.01f), CCMoveBy::create(0.3f, ccp(0, -200))));
//	startGame->runAction(CCMoveTo::create(0.5f, position.START));
//	title->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.2f), CCMoveTo::create(0.3f, position.CUP)));
//	titleBack->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.2f), CCMoveTo::create(0.3f, position.CUP)));
//	
//	moreapps->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.3f), CCMoveTo::create(0.3f, position.MOREAPPS)));
//	option->runAction(CCSequence::create(CCDelayTime::create(0.3f), CCMoveTo::create(0.3f, position.OPTION),
//					  KSFunc(
//							{
//								startGame->setEnabled(true);
//							}
//					) ,0));
//	myScoreWindow->runAction(CCMoveTo::create(0.3f, ccp(position.MYSCORE.x + 250, position.MYSCORE.y)));
//	startGame->setEnabled(false);
//	titleBack->runAction(CCFadeIn::create(0.5f));
//	title->runAction(CCFadeIn::create(0.5f));
//	gameSelect->hideGames();
//	showRank();
	if(checkBeforeMoving() == false)
		return;
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	showWorldcupMark(false);
	showRankWithMenu();
}

void SWIntro::startGame(CCNode* colleague, JsonBox::Object param)
{
	if(param["type"].getString() == "WORLDCUP")
	{
		ChallengeShop* gs = dynamic_cast<ChallengeShop*>(colleague);
		if(gs)
		{
			if(NSDefault::getGold() >= GameSystem::WORLDCUP_ENTRANCE_FEE + gs->getRealPrice()) // ##
			{
				BASETIME = 30.f; // 30초 기준.
				playInfo->selectedItems.clear();
				playInfo->selectedItems = gs->getSelectedItems();
				
				for(auto i : playInfo->selectedItems)
				{
					CCLog("A : %s %d", i.first.c_str(), i.second);
				}
				DisableTouch* cl = DisableTouch::create();
				addChild(cl);
				ostringstream oss;
				oss << (int)cl;
				param["callback"] = oss.str();
				
				param["getlist"] = true;
				param["limit"] = 30;
				graphdog->command("startscores", &param, this, gd_selector(SWIntro::finishStartScores));
				joinMembers = JoinMembers::create();
				this->addChild(joinMembers, INT_MAX - 1);
				foldingMenus();
			}
			else
			{
				KSModalAlert("", gt("require_coin").c_str(), 1, "OK",
							 NOPARAM);
			}
		}
		
	}
	else if(NSDefault::getHeartNumber() > 0 || NSDefault::isTodayFree())
	{
		GameShop* gs = dynamic_cast<GameShop*>(colleague);
		if(gs)
		{
			if(NSDefault::getGold() >= gs->getRealPrice())
			{
				BASETIME = 30.f; // 60초 기준
				playInfo->selectedItems.clear();
				playInfo->selectedItems = gs->selectedItems;
				for(auto i : gs->selectedItems)
				{
					CCLog("%s : %d", i.first.c_str(), (int)i.second);
				}
				DisableTouch* cl = DisableTouch::create();
				addChild(cl);
				
				ostringstream oss;
				oss << (int)cl;
				param["callback"] = oss.str();
				param["getlist"] = true;
				param["limit"] = 30;
				graphdog->command("startscores", &param, this, gd_selector(SWIntro::finishStartScores));
				joinMembers = JoinMembers::create();
				this->addChild(joinMembers, INT_MAX - 1);
				foldingMenus();
			}
			else
			{
				KSModalAlert("", gt("require_coin").c_str(), 1, "OK",
							 NOPARAM);
			}
		}
	}
	else
	{
		KSModalAlert("", gt("require_heart").c_str(), 1, "OK",
					 NOPARAM);
		showHeartShop();
	}
}
void SWIntro::START(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	// 이미 게임 샵 화면이면 선택된 게임을 시작함.
	if(catchingColleague == hurdleWindow || catchingColleague == archeryWindow || catchingColleague == basketWindow ||
		catchingColleague == clayWindow || catchingColleague == skiWindow || catchingColleague == hanggWindow ||
	   catchingColleague == challengeWindow)
	{
		JsonBox::Object param;
		if(catchingColleague == hurdleWindow)
		{
			param["type"] = "HW";
		}
		else if(catchingColleague == archeryWindow)
		{
			param["type"] = "AC";
		}
		else if(catchingColleague == basketWindow)
		{
			param["type"] = "BS";
		}
		else if(catchingColleague == clayWindow)
		{
			param["type"] = "AK";
		}
		else if(catchingColleague == skiWindow)
		{
			param["type"] = "SK";
		}
		else if(catchingColleague == hanggWindow)
		{
			param["type"] = "HG";
		}
		else if(catchingColleague == challengeWindow)
		{
			param["type"] = "WORLDCUP";
		}
		startGame(catchingColleague, param);
	}
	else if(catchingColleague == gameSelect)
	{		
		switch(gameSelect->getCurrentIndex())
		{
			case GameSelect::BASKETBALL_INDEX:
				if(NSDefault::getIsOpenedGame("BS"))
				{
					gameSelect->hideGames();
					showBasketShop();
				}
				else
				{
					KSModalAlert("", "NO EXECUTE", 1, "OK", NOPARAM);
				}
				break;
			case GameSelect::CLAY_INDEX:
				if(NSDefault::getIsOpenedGame("AK"))
				{
					gameSelect->hideGames();
					showClayShop();
				}
				else
				{
					KSModalAlert("", "NO EXECUTE", 1, "OK", NOPARAM);
				}
				
				break;
			case GameSelect::HURDLE_INDEX:
				if(NSDefault::getIsOpenedGame("HW"))
				{
					gameSelect->hideGames();
					showHurdleShop();
				}
				else
				{
					KSModalAlert("", "NO EXECUTE", 1, "OK", NOPARAM);
				}
				
				break;
			case GameSelect::ARCHERY_INDEX:
				if(NSDefault::getIsOpenedGame("AC"))
				{
					gameSelect->hideGames();
					showArcheryShop();
				}
				else
				{
					KSModalAlert("", "NO EXECUTE", 1, "OK", NOPARAM);
				}
				break;
			case GameSelect::SKI_INDEX:
				if(NSDefault::getIsOpenedGame("SK"))
				{
					gameSelect->hideGames();
					showSkiShop();
				}
				else
				{
					KSModalAlert("", "NO EXECUTE", 1, "OK", NOPARAM);
				}
				break;
			case GameSelect::HG_INDEX:
				if(NSDefault::getIsOpenedGame("HG"))
				{
					gameSelect->hideGames();
					showHgShop();
				}
				else
				{
					KSModalAlert("", "NO EXECUTE", 1, "OK", NOPARAM);
				}
				break;
			case GameSelect::COMINGSOON:
				if(NSDefault::getIsOpenedGame("WORLDCUP"))
				{
					gameSelect->hideGames();
					showChallengeShop();
				}
				else
				{
					KSModalAlert("", "NO EXECUTE", 1, "OK", NOPARAM);
				}
				break;
		}
		
		
	}
	// 아닌 경우엔 게임 셀렉을 띄움.
	else
	{
		if(checkBeforeMoving() == false)
			return;
		title->runAction(CCMoveTo::create(0.3f, ccp(position.CUP.x + 300.f, position.CUP.y)));
		titleBack->runAction(CCMoveTo::create(0.3f, ccp(position.CUP.x + 300.f, position.CUP.y)));
		
		moreapps->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.1f), CCMoveTo::create(0.3f, ccp(position.MOREAPPS.x + 300.f, position.MOREAPPS.y) )));
		option->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.1f), CCMoveTo::create(0.3f, ccp(position.OPTION.x + 300.f, position.OPTION.y))));
		
		if(medalWindow == 0)
		{
			medalWindow = MyMedal::create(this);
			medalWindow->setPosition(ccp(position.MEDAL_FRAME.x + 250, position.MEDAL_FRAME.y));
			addChild(medalWindow, 3);
		}
		
		medalWindow->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.2f), CCMoveTo::create(0.3f, position.MEDAL_FRAME)));
		home->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.3f), CCMoveTo::create(0.3f, position.HOME)));
		
		myScoreWindow->runAction(CCMoveTo::create(0.3f, ccp(position.MYSCORE.x + 250, position.MYSCORE.y)));
		showGameSelect();
	}
}

void SWIntro::RESTART(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
//	if(initMode == INIT_MODE::WORLDCUP_GAMEOVER)
//	{ // 바로
//		playInfo->createWorldcupOrder();
//		wcplay->initVars();
//		JsonBox::Object param;
//		param["type"] = "WORLDCUP";
//		startGame(catchingColleague, param);
//	}
//	else
	{
		if(catchingColleague == hurdleWindow || catchingColleague == archeryWindow || catchingColleague == basketWindow ||
		   catchingColleague == clayWindow || catchingColleague == skiWindow || catchingColleague == hanggWindow ||
		   catchingColleague == challengeWindow)
		{
			START(0);
		}
		else// if(catchingColleague == gameOverWindow)
		{
			if(playInfo->currentGame == "BS")
			{
				showBasketShop();
			}
			else if(playInfo->currentGame == "HW")
			{
				showHurdleShop();
			}
			else if(playInfo->currentGame == "AC")
			{
				showArcheryShop();
			}
			else if(playInfo->currentGame == "AK")
			{
				showClayShop();
			}
			else if(playInfo->currentGame == "SK")
			{
				showSkiShop();
			}
			else if(playInfo->currentGame == "HG")
			{
				showHgShop();
			}
			else if(playInfo->currentGame == "WORLDCUP")
			{
				showChallengeShop();
			}
		}
	}
	
}
void SWIntro::finishStartScores(JsonBox::Object js)
{
	JsonBox::Object obj = js["param"].getObject();
	string _cl = obj["callback"].getString();
	CCNode* cl = (CCNode*)atoi(_cl.c_str());
	try
	{
		ostringstream oss;
		oss << js << endl;
		KS::KSLog("%", oss.str().c_str());
		
		if(js["state"].getString() != "ok")
		{
			throw string(gt("netfail").c_str());
		}
		GameShop* gs = dynamic_cast<GameShop*>(catchingColleague);
		if(gs)
		{
			NSDefault::setGold(NSDefault::getGold() - gs->getRealPrice());
			if(gs->getRealPrice() != 0)
			{
				KSoundEngine::sharedEngine()->playSound("usemoney.mp3");
				JsonBox::Object gParam;
				gParam["category"] = "useitem";
				gParam["log"] = gs->getSelectedItems();
				graphdog->command("writelog", &gParam, 0, 0);
			}
			introHead->setGold(NSDefault::getGold());
		}
		JsonBox::Object paramObj = js["param"].getObject();
		gameType = paramObj["type"].getString();
		
		if(gameType != "WORLDCUP")
		{
			if(NSDefault::isTodayFree() == false) // 24H 무료 모드가 아니면 깎음.
			{
				introHead->removeHeartAnimation();
				if(NSDefault::getHeartNumber() == 5)
					NSDefault::setHeartBaseTime(GameSystem::getCurrentTime_s());
				NSDefault::setHeartNumber(NSDefault::getHeartNumber() - 1);
			}
			
		}
		else
		{
			ChallengeShop* cs = dynamic_cast<ChallengeShop*>(catchingColleague);
			NSDefault::setGold(NSDefault::getGold() - GameSystem::WORLDCUP_ENTRANCE_FEE - cs->getRealPrice());
			KSoundEngine::sharedEngine()->playSound("usemoney.mp3");
			JsonBox::Object gParam;
			gParam["category"] = "startchallenge";
			gParam["log"] = "챌린지 시작함 ㅋㅋㅋ";
			graphdog->command("writelog", &gParam, 0, 0);
			introHead->setGold(NSDefault::getGold());
		}
		// 보이는 것들 다 치우고 참가자 모집 에니메이션 띄움.
					
		runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.0f),
										   KSFunc(
												  {
													  JsonBox::Object js2 = js;
													  joinMembers->joiningMembers(js2["list"].getArray(), js2["alluser"].getInt());
												  })));
		
	
		} catch (const string& msg)
		{
			cl->removeFromParentAndCleanup(true);
			disfoldingMenus();
			KSModalAlert("", msg.c_str(), 1,
						 "OK", NOPARAM);
	}
}

void SWIntro::finishStartScores2(CCObject*)
{
	playInfo->currentGame = gameType;
	playInfo->wcscore = 0;
	acplay->initVars();
	akplay->initVars();
	bs2play->initVars();
	hwplay->initVars();
	skplay->initVars();
	hgplay->initVars();
	if(playInfo->currentGame == "WORLDCUP")
	{
//		playInfo->selectedItems.clear();
		string ret = playInfo->getWorldcupGame();
		
		//														  std::pair<std::string, KSProtectVar<int>> tt = std::make_pair(ret, KSProtectVar<int>(0));
		//														  wcplay->scoreEachGame.push_back(tt);
		playInfo->popGame();
		playInfo->initGameAndStart(ret);
	}
	else
	{
		if(playInfo->currentGame == "BS")
		{
			if(NSDefault::getBasketCount() <= 5)
			{
				NSDefault::incBasketCount();
				ReplaceScene(Tutorial);
			}
			else
			{
				bs2play->initVars();
				ReplaceScene(BasketGame);
			}
			
		}
		else if(playInfo->currentGame == "HW")
		{
			if(NSDefault::getHurdleCount() <= 5)
			{
				NSDefault::incHurdleCount();
				ReplaceScene(Tutorial);
			}
			else
			{
				hwplay->initVars();
				ReplaceScene(HWGameDirector);
			}
		}
		else if(playInfo->currentGame == "AC")
		{
			if(NSDefault::getArcherCount() <= 5)
			{
				NSDefault::incArcherCount();
				ReplaceScene(Tutorial);
			}
			else
			{
				acplay->initVars();
				ReplaceScene(ACGameScene);
			}
			
		}
		else if(playInfo->currentGame == "AK")
		{
			if(NSDefault::getClayCount() <= 5)
			{
				NSDefault::incClayCount();
				ReplaceScene(Tutorial);
			}
			else
			{
				akplay->initVars();
				ReplaceScene(AngryGameScene);
			}
			
		}
		else if(playInfo->currentGame == "SK")
		{
			if(NSDefault::getSkiCount() <= 5)
			{
				NSDefault::incSkiCount();
				ReplaceScene(Tutorial);
			}
			else
			{
				skplay->initVars();
				ReplaceScene(SKGameScene);
			}
		}
		else if(playInfo->currentGame == "HG")
		{
			if(NSDefault::getHanggCount() <= 5)
			{
				NSDefault::incHanggCount();
				ReplaceScene(Tutorial);
			}
			else
			{
				hgplay->initVars();
				ReplaceScene(HGGameScene);
			}
		}
		//playInfo->initGameAndStart(playInfo->currentGame); // 이거 왜 있는거지?
	}

}
void SWIntro::finishInfo(JsonBox::Object js)
{
}

void SWIntro::getScores(JsonBox::Object js)
{
}

void SWIntro::onEnter()
{
	CCLayer::onEnter();
	

}
void SWIntro::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
	
		
	
}
void SWIntro::onExit()
{
	playInfo->pIntroHead = 0;
	CCLayer::onExit();
}

void SWIntro::showFacebookRank(){
	if(checkBeforeMoving() == false)
		return;
	
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	facebookRank->setPosition(ccp(143, -144));
	facebookRank->runAction(CCMoveTo::create(0.5f, ccp(143, 144)));
	if(FBConnector::get()->isLogin())
	{
		FBConnector::get()->getRequests(facebookRank, fb_selector(FacebookRank::getRequestCountAndTicket));
	}
	catchingColleague = facebookRank;
}
void SWIntro::showBackToFacebook(){
	if(checkBeforeMoving() == false)
		return;
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	facebookRank->setPosition(ccp(143, -144));
	facebookRank->runAction(CCMoveTo::create(0.5f, ccp(143, 144)));
	catchingColleague = facebookRank;
	if(FBConnector::get()->isLogin())
	{
		FBConnector::get()->getRequests(facebookRank, fb_selector(FacebookRank::getRequestCountAndTicket));
	}
}
void SWIntro::showCountryRank(){
	if(checkBeforeMoving() == false)
		return;
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	countryRank->setPosition(ccp(143, -144));
	countryRank->runAction(CCMoveTo::create(0.5f, ccp(143, 144)));
	catchingColleague = countryRank;
}
void SWIntro::showFacebookPost(){
	if(checkBeforeMoving() == false)
		return;
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	postBox->setPosition(ccp(143, -144));
	postBox->runAction(CCMoveTo::create(0.5f, ccp(143, 144)));
	catchingColleague = postBox;
	postBox->loadRequest();
}
void SWIntro::showOption()
{
	if(checkBeforeMoving() == false)
		return;
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	optionWindow->setPosition(ccp(143, -144));
	optionWindow->runAction(CCMoveTo::create(0.5f, ccp(143, 144)));
	catchingColleague = optionWindow;
}
void SWIntro::showGameSelect()
{
	if(checkBeforeMoving() == false)
		return;
	
	if(gameSelect == 0 || playInfo->justOpen == true)
	{
		if(gameSelect != 0)
			gameSelect->removeFromParentAndCleanup(true); // 제거후 다시 로드.
		
		
		gameSelect = GameSelect::create(this);
		gameSelect->setPosition(ccp(-400, 500));
		addChild(gameSelect, 3);
		playInfo->justOpen = false; // 막 연게 아니도록 세팅함.
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	gameSelect->showGames();
	gameSelect->setPosition(ccp(143, -144));
	gameSelect->runAction(CCMoveTo::create(0.5f, ccp(143, 144)));
	catchingColleague = gameSelect;
	
}
void SWIntro::showLegendaryRecord(string gameType)
{
	if(checkBeforeMoving() == false)
		return;
	if(legendaryWindow == 0)
	{
		legendaryWindow = Legendary::create(this);
		legendaryWindow->setPosition(ccp(-400, 500));
		addChild(legendaryWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	legendaryWindow->loadRecord(gameType);
	legendaryWindow->setPosition(ccp(143, -144));
	legendaryWindow->runAction(CCMoveTo::create(0.5f, ccp(143, 144)));
	catchingColleague = legendaryWindow;
	if(gameSelect)
		gameSelect->hideGames();
}

void SWIntro::showGameOver()
{
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	introHead->runAction(CCMoveTo::create(0.5f, ccp(0, 288)));
	gameOverWindow->setPosition(ccp(143, -144));
	introHead->expBar->setPercentage(0);
	gameOverWindow->runAction(CCSequence::createWithTwoActions(CCMoveTo::create(0.5f, ccp(143, 144)), CCCallFunc::create(gameOverWindow, callfunc_selector(GameOver::appearMe)) ));
	
	myScoreWindow->runAction(CCMoveTo::create(0.3f, position.MYSCORE));
	home->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.2f), CCMoveTo::create(0.3f, position.GAMEOVER_HOME)));
	restartGame->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.3f), CCMoveTo::create(0.3f, position.RESTART)));
	
	titleBack->runAction(CCSequence::create(CCFadeOut::create(0.5f), CCMoveTo::create(0.1f, ccp(-400, -400)),
											 CCFadeIn::create(0.1f), 0));
	title->runAction(CCSequence::create(CCFadeOut::create(0.5f), CCMoveTo::create(0.1f, ccp(-400, -400)),
										 CCFadeIn::create(0.1f), 0));
	
	
	
	catchingColleague = gameOverWindow;
}
void SWIntro::showFailedGameOver(const string& msg)
{
	KSModalAlert("", msg.c_str(), 1,
				 "RETRY", this, callfunc_selector(ThisClassType::GAMEOVERRETRY));
}
void SWIntro::showAlert(const string& msg)
{
	KSModalAlert("", msg.c_str(), 1,
				 "OK", NOPARAM);
}

void SWIntro::showAlert(CCObject* str)
{
	CCString* pStr = (CCString*)str;
	
	KSModalAlert("", pStr->getCString(), 1,
				 "OK", NOPARAM);
}
void SWIntro::showRubyShop()
{
	if(rubyWindow == catchingColleague)
		return;
	if(checkBeforeMoving() == false)
		return;
	if(gameSelect == catchingColleague)
		gameSelect->hideGames();
	if(rubyWindow == 0)
	{
		rubyWindow = RubyShop::create(this);
		rubyWindow->setPosition(ccp(200, 500));
		addChild(rubyWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	rubyWindow->setPosition(ccp(143, -144));
	rubyWindow->runAction(CCMoveTo::create(0.5f, ccp(143, 144)));
	catchingColleague = rubyWindow;
	
}
void SWIntro::showGoldShop()
{
	if(goldWindow == catchingColleague)
		return;
	if(checkBeforeMoving() == false)
		return;
	if(gameSelect == catchingColleague)
		gameSelect->hideGames();
	if(goldWindow == 0)
	{
		goldWindow = GoldShop::create(this);
		goldWindow->setPosition(ccp(200, 500));
		addChild(goldWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	goldWindow->setPosition(ccp(143, -144));
	goldWindow->runAction(CCMoveTo::create(0.5f, ccp(143, 144)));
	catchingColleague = goldWindow;
}
void SWIntro::showHeartShop()
{
	if(heartWindow == catchingColleague)
		return;
	if(checkBeforeMoving() == false)
		return;
	if(gameSelect == catchingColleague)
		gameSelect->hideGames();
	if(heartWindow == 0)
	{
		heartWindow = HeartShop::create(this);
		heartWindow->setPosition(ccp(200, 500));
		addChild(heartWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	heartWindow->setPosition(ccp(143, -144));
	heartWindow->runAction(CCMoveTo::create(0.5f, ccp(143, 144)));
	catchingColleague = heartWindow;
}
void SWIntro::showArcheryShop()
{
	if(checkBeforeMoving() == false)
		return;
	
	if(archeryWindow == 0)
	{
		archeryWindow = ArcheryShop::create(this);
		archeryWindow->setPosition(ccp(200, 500));
		addChild(archeryWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	archeryWindow->setPosition(ccp(143, -144));
	archeryWindow->runAction(CCMoveTo::create(0.5f, ccp(143, 144)));
	catchingColleague = archeryWindow;
}
void SWIntro::showHgShop()
{
	if(checkBeforeMoving() == false)
		return;
	
	if(hanggWindow == 0)
	{
		hanggWindow = HanggShop::create(this);
		hanggWindow->setPosition(ccp(200, 500));
		addChild(hanggWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	hanggWindow->setPosition(ccp(143, -144));
	hanggWindow->runAction(CCMoveTo::create(0.5f, ccp(143, 144)));
	catchingColleague = hanggWindow;
}
void SWIntro::showSkiShop()
{
	if(checkBeforeMoving() == false)
		return;
	
	if(skiWindow == 0)
	{
		skiWindow = SkiShop::create(this);
		skiWindow->setPosition(ccp(200, 500));
		addChild(skiWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	skiWindow->setPosition(ccp(143, -144));
	skiWindow->runAction(CCMoveTo::create(0.5f, ccp(143, 144)));
	catchingColleague = skiWindow;
}

void SWIntro::showHurdleShop()
{
	if(checkBeforeMoving() == false)
		return;
	if(hurdleWindow == 0)
	{
		hurdleWindow = HurdleShop::create(this);
		hurdleWindow->setPosition(ccp(200, 500));
		addChild(hurdleWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	hurdleWindow->setPosition(ccp(143, -144));
	hurdleWindow->runAction(CCMoveTo::create(0.5f, ccp(143, 144)));
	catchingColleague = hurdleWindow;
}
void SWIntro::showBasketShop()
{
	if(checkBeforeMoving() == false)
		return;
	if(basketWindow == 0)
	{
		basketWindow = BasketShop::create(this);
		basketWindow->setPosition(ccp(200, 500));
		addChild(basketWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	basketWindow->setPosition(ccp(143, -144));
	basketWindow->runAction(CCMoveTo::create(0.5f, ccp(143, 144)));
	catchingColleague = basketWindow;
}
void SWIntro::showClayShop()
{
	if(checkBeforeMoving() == false)
		return;
	if(clayWindow == 0)
	{
		clayWindow = ClayShop::create(this);
		clayWindow->setPosition(ccp(200, 500));
		addChild(clayWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	clayWindow->setPosition(ccp(143, -144));
	clayWindow->runAction(CCMoveTo::create(0.5f, ccp(143, 144)));
	catchingColleague = clayWindow;
}

void SWIntro::showChallengeShop()
{
	if(checkBeforeMoving() == false)
		return;
	if(challengeWindow == 0)
	{
		challengeWindow = ChallengeShop::create();
		challengeWindow->setPosition(ccp(200, 500));
		addChild(challengeWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	challengeWindow->setPosition(ccp(143, -144));
	challengeWindow->runAction(CCMoveTo::create(0.5f, ccp(143, 144)));
	catchingColleague = challengeWindow;
}
void SWIntro::showGiftCode()
{
	if(checkBeforeMoving() == false)
		return;
	if(giftCodeWindow == 0)
	{
		giftCodeWindow = GiftCode::create(this);
		giftCodeWindow->setPosition(ccp(200, 500));
		addChild(clayWindow, 3);
	}
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	giftCodeWindow->setPosition(ccp(143, -144));
	giftCodeWindow->runAction(CCMoveTo::create(0.5f, ccp(143, 144)));
	catchingColleague = giftCodeWindow;
}
void SWIntro::showMoreApps()
{
	if(moreAppsWindow == catchingColleague)
		return;
	if(checkBeforeMoving() == false)
		return;
	
	if(moreAppsWindow != 0)
		moreAppsWindow->removeFromParentAndCleanup(true);
	
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	moreAppsWindow = MoreApps::create(this);
	moreAppsWindow->setPosition(ccp(-400, 100));
	addChild(moreAppsWindow, 3);

		
	moreAppsWindow->setPosition(ccp(143, -144));
	moreAppsWindow->runAction(CCMoveTo::create(0.5f, ccp(143, 144)));
	catchingColleague = moreAppsWindow;
}
void SWIntro::sendFacebookScore()
{
//	int ac = playInfo->currentWeek == ACSaveData::getWeeklyMaxScore().first ? ACSaveData::getWeeklyMaxScore().second : 0;
//	int ak = playInfo->currentWeek == AKSaveData::getWeeklyMaxScore().first ? AKSaveData::getWeeklyMaxScore().second : 0;
//	int bs = playInfo->currentWeek == BS2SaveData::getWeeklyMaxScore().first ? BS2SaveData::getWeeklyMaxScore().second : 0;
//	int hw = playInfo->currentWeek == HWSaveData::getWeeklyMaxScore().first ? HWSaveData::getWeeklyMaxScore().second : 0;
//	
	if(FBConnector::get()->isUsed() && FBConnector::get()->isLogin())
	{
		FBConnector::get()->sendScore(wcplay->totalGameScore(), this, fb_selector(SWIntro::finishFacebookSendScore));
		//			FBConnector::get()->getScores(facebookRank, fb_selector(FacebookRank::facebookGetScoresFinish));
	}
}
void SWIntro::showJudgeOnGameOver(string result, int rank, int score, int max, int weekly)
{
	myScoreWindow->showJudgeOnGameOver(result, rank, score, max, weekly);
	KSoundEngine::sharedEngine()->playSound("slot.mp3");
	if(score >= 200000 && rand()%15 == 0 && NSDefault::isReview() == 0)
	{
		NSDefault::setReview(1);
		KSModalAlert("", gt("opennewmap").c_str(), 2,
					 "NO", NOPARAM,
					 "YES", this, callfunc_selector(ThisClassType::goReview));
	}
}
void SWIntro::showLevelUp()
{
	KSoundEngine::sharedEngine()->playSound("levelup.mp3");
	auto mator = SceneUtil::playAnimation("levelup.png", 0.2, 3, 6, 170, 170, 1);
	auto ac = CCSequence::create(mator.first,CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite)), 0);
	mator.second->runAction(ac);
	mator.second->setPosition(ccp(380, 180));
	addChild(mator.second, 3);
}
void SWIntro::showExp2()
{
	auto mator = SceneUtil::playAnimation("expx2.png", 0.2, 3, 3, 52, 32, 6);
	auto ac = CCSequence::create(mator.first,CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite)), 0);
	mator.second->setScale(1.5f);
	mator.second->runAction(ac);
	mator.second->setPosition(ccp(420, 270));
	addChild(mator.second, 3);
}
void SWIntro::refreshEXP()
{
	introHead->refreshEXP();
}
void SWIntro::showWorldcupMark(bool b)
{
	worldcupMark->setVisible(b);
}


void SWIntro::foldingMenus()
{
	catchingColleague->runAction(CCMoveTo::create(0.3f, ccp(position.CONTENT.x - 400, position.CONTENT.y)));
	//introHead->runAction(CCMoveTo::create(0.3f, ccp(position.INTROHEAD.x, position.INTROHEAD.y + 100)));
	if(medalWindow)
		medalWindow->runAction(CCMoveTo::create(0.3f, ccp(position.MEDAL_FRAME.x + 300, position.MEDAL_FRAME.y )));
	if(myScoreWindow)
		myScoreWindow->runAction(CCMoveTo::create(0.3f, ccp(position.MYSCORE.x + 300, position.MYSCORE.y)));
	
	if(catchingColleague != gameOverWindow)
	{
		
		if(title->getPosition().equals(position.CUP)) // 컵이 제자리에 있다면
		{
			title->runAction(CCMoveTo::create(0.3f, ccp(position.CUP.x + 300.f, position.CUP.y) ));
			titleBack->runAction(CCMoveTo::create(0.3f, ccp(position.CUP.x + 300.f, position.CUP.y) ));
		}
	}
	
	home->runAction(CCMoveTo::create(0.3f, ccp(position.HOME.x + 300.f, position.HOME.y) ));
	startGameBtn->runAction(CCMoveTo::create(0.3f, ccp(position.START.x + 300.f, position.START.y) ));
	restartGame->runAction(CCMoveTo::create(0.3f, ccp(position.START.x + 300.f, position.START.y) ));
}
void SWIntro::disfoldingMenus()
{
	catchingColleague->runAction(CCMoveTo::create(0.3f, position.CONTENT));
	
	if(dynamic_cast<GameShop*>(catchingColleague))
	{
		if(medalWindow == 0)
		{
			medalWindow = MyMedal::create(this);
			medalWindow->setPosition(ccp(position.MEDAL_FRAME.x + 250, position.MEDAL_FRAME.y));
			addChild(medalWindow, 3);
		}
		medalWindow->runAction(CCMoveTo::create(0.3f, position.MEDAL_FRAME));
	}
	//introHead->runAction(CCMoveTo::create(0.3f, ccp(position.INTROHEAD.x, position.INTROHEAD.y + 100)));
	
	
	
	home->runAction(CCMoveTo::create(0.3f, position.HOME ));
	startGameBtn->runAction(CCMoveTo::create(0.3f, position.START ));
	joinMembers->removeFromParentAndCleanup(true);
//	restartGame->runAction(CCMoveTo::create(0.3f, position.START));
	
	
}

void SWIntro::openNewGameEffect(CCObject*)
{
	CCSprite* b = CCSprite::create();
	b->setTextureRect(CCRectMake(0, 0, 600, 320));
	b->setColor(ccc3(0, 0, 0));
	b->setOpacity(0);
//	b->setOpacity(0);
	
	b->runAction(CCFadeTo::create(0.5f, 180));
	b->runAction(CCSequence::create(CCFadeTo::create(0.5f, 180), CCDelayTime::create(1.5f), CCFadeTo::create(0.5f, 0),
									CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite)),0));
	b->setPosition(ccp(240, 160));
	b->setAnchorPoint(ccp(0.5f, 0.5f));
	addChild(b, 400);
	CCSprite* newGameBack = CCSprite::create("open_newgameback.png");
	newGameBack->setOpacity(0);
	newGameBack->setPosition(ccp(240, 160));
	addChild(newGameBack, 401);
	newGameBack->runAction(CCSequence::create(CCDelayTime::create(0.25f),
															CCSpawn::create(CCSequence::create(CCScaleTo::create(0.5f, 2.f), CCDelayTime::create(1.75f), CCScaleTo::create(0.5f, 0.6f), 0),
																			CCRotateBy::create(2.25f + 0.5f, 360.f),
																			CCSequence::create(CCFadeTo::create(0.5f, 255), CCDelayTime::create(1.75f), CCFadeTo::create(0.5f, 0), 0),
																			
																			
																			0),
											  CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite)) ,0));
	
	CCSprite* comment = CCSprite::create("open_newgame.png");
	comment->setOpacity(0);
	comment->setScale(0.6f);
	comment->setPosition(ccp(240, 160));
	
	addChild(comment, 402);
	
	comment->runAction(CCSequence::create(CCDelayTime::create(0.25f),
														CCSpawn::create(CCSequence::create(CCScaleTo::create(0.5f, 1.f), CCDelayTime::create(1.75f), CCScaleTo::create(0.5f, 0.6f), 0),
																		CCSequence::create(CCFadeTo::create(0.5f, 255), CCDelayTime::create(1.75f), CCFadeTo::create(0.5f, 0), 0),
																		
																		
																		0),
														CCCallFuncN::create(this, callfuncN_selector(ThisClassType::deleteSprite)),
														0));
}
void SWIntro::onRecvRequestTicket(CCObject* _tds)
{
	Ticket* t = Ticket::create();
	t->createRecvTicketReq((CCArray*)_tds);
	addChild(t, 400);
}
void SWIntro::onGameOpenWithFriends(CCObject*)
{
	if(gameSelect != 0)
	{
		CCNotificationCenter::sharedNotificationCenter()->postNotification("onUnlockStageWithFriends");
	}
	else
	{
		NSDefault::setOpenGame(NSDefault::getSoonOpenGame());
	}
}
void SWIntro::onRecvTicket(CCObject* _tds)
{
	// 티켓 받을 때 요청을 보내지 않았으면 다 지움.
	if(NSDefault::getTicketIsStarted() == false)
	{
		NSDefault::setArrivedTicketId("");
		CCArray* ticketDatas = (CCArray*)_tds;
		for(int i=0; i<ticketDatas->count(); i++)
		{
			TicketData* td = (TicketData*)ticketDatas->objectAtIndex(i);
			FBConnector::get()->removeRequests(td->ticketid, 0, 0);
		}
	}
	else
	{
		Ticket* t = Ticket::create();
		t->createRecvTicket((CCArray*)_tds);
		addChild(t, 400);
	}
}
void SWIntro::onRequestTicket(CCObject*)
{
	Ticket* st = Ticket::create();
	st->createTicketSend();
	
	addChild(st, 400);
	
	
}


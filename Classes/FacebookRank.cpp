//
//  FacebookRank.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 2. 28..
//
//

#include "FacebookRank.h"
#include "Bridge.h"
#include "KS_Util.h"
#include "NSDefault.h"
#include "KSoundEngine.h"
#include "KSModalAlert.h"
#include "SWIntro.h"
#include "GraphDog.h"
#include "KSoundEngine.h"

#include "KSLabelTTF.h"
#include "SceneUtil.h"
#include "GDTranslate.h"

#include "Ticket.h"
using namespace GDTranslate;
static const int FB_X = 12;
static const int FB_Y = 53;
static const int FB_W = 245;
static const int FB_H = 194;

static const int FB_PROFILE_W = 33;
static const int FB_PROFILE_H = 33;
static const int FB_CELL_H = 35;
static const int INVITE_TAG = 0x34545;
static const int HEART_TAG = 0x84353;
static const int CELL_MENUS = 0xcf487;
static const int GETHEART_TAG = 0xfeabf3;
void FacebookRank::FACEBOOKLOGIN()
{
	KS::KSLog("FACEBOOK LOGIN");
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	facebookLogin->setVisible(false);
	FBConnector::get()->login(this, fb_selector(FacebookRank::facebookLoginFinish));
}

FacebookRank::~FacebookRank()
{
	FBDelegator::getInstance()->removeTarget(this);
}
bool FacebookRank::init()
{
//	CCNode::init();
//	CCNode::create()
	CCLayer::init();
	leftTime_s = -1;
	
	setTouchEnabled(true);
	CCSprite* back = CCSprite::create("main_facebook_back.png");
	
	addChild(back);
	thiz = back;
	leftTimeFnt = CCLabelBMFont::create("", "main2.fnt");
	leftTimeFnt->setPosition(ccp(218, 263));
	thiz->addChild(leftTimeFnt, 2);
	
	float loadingX = FB_X + FB_W / 2.f;
	float loadingY = FB_Y + FB_H / 2.f;
	auto animator = SceneUtil::playAnimation("loading.png", 0.1, 9, 9, 56, 56, true);
	animator.second->runAction(animator.first);
	thiz->addChild(animator.second, INT_MAX);
	animator.second->setPosition(ccp(loadingX, loadingY));
	fbLoading = animator.second;
	fbLoading->setVisible(false);
	
	ccView = CCTableView::create(this, CCSizeMake(FB_W, FB_H));
	ccView->setDirection(kCCScrollViewDirectionVertical);
	ccView->setVerticalFillOrder(kCCTableViewFillTopDown);
	ccView->setPosition(ccp(12, 52));
	ccView->setDelegate(this);
	thiz->addChild(ccView, 11);
//	ccView->ignoreAnchorPointForPosition(false);
//	ccView->setAnchorPoint(ccp(0.5, 0.5));
	
		
	
	
	CCMenu* _menu = CCMenu::create(0);
	_menu->setPosition(CCPointZero);
	thiz->addChild(_menu);
	{
		CCMenuItem* fblogin = CCMenuItemImage::create("facebook_login.png", "facebook_login.png",
																   this, menu_selector(FacebookRank::FACEBOOKLOGIN));
		facebookLogin = fblogin;
		fblogin->setPosition(ccp(130, 150));
		_menu->addChild(fblogin, 2);
		fblogin->setVisible(false);
		
	}
	{
		CCMenuItem* item = CCMenuItemImage::create("main_postbox.png", "main_postbox.png",
																this, menu_selector(FacebookRank::POSTBOX));
		postbox = item;
		item->setPosition(ccp(38, 26));
		_menu->addChild(item, 2);
	}
	if(FBConnector::get()->isUsed())
	{
		if(!FBConnector::get()->isLogin())
		{
			KS::KSLog("is not login");
			fbLoading->setVisible(true);
			FBConnector::get()->login(this, fb_selector(FacebookRank::facebookLoginFinish));
		}
		else
		{
			KS::KSLog("islogin");
		}
	}
	else
	{
		facebookLogin->setVisible(true);
		
	}
	
	postboxNewSprite = CCSprite::create("facebook_new.png");
	//facebookPostBox->addChild(postboxNewSprite, 1);
	postboxNewSprite->setPosition(ccp(58, 34));
	postboxNewSprite->setVisible(false);
	thiz->addChild(postboxNewSprite, 2);
	
	reqCount = CCLabelBMFont::create("", "main2.fnt");
	reqCount->setPosition(ccp(58, 36));
	thiz->addChild(reqCount, 3);
	{
		CCMenuItem* countryRank = CCMenuItemImage::create("main_mini_countryrank.png", "main_mini_countryrank.png",
																   this, menu_selector(FacebookRank::COUNTRYRANK));
		countryRank->setPosition(ccp(196, 25));
		_menu->addChild(countryRank, 2);
		
		countryRank->runAction(
						CCRepeatForever::create( CCSequence::createWithTwoActions(CCEaseOut::create(CCMoveBy::create(0.8, ccp(0, 5)), 1.3),
																					CCEaseOut::create(CCMoveBy::create(0.8, ccp(0, -5)), 1.3))) );
	}
//	this->runAction(CCRepeat::create(CCSequence::createWithTwoActions(CCMoveBy::create(1.f, ccp(0, 100)),
//																		  CCMoveBy::create(1.f, ccp(0, -100))), 100));
	
	schedule(schedule_selector(ThisClassType::update));
	return true;
}
void FacebookRank::finishGetWeek(JsonBox::Object js)
{
	if(js["state"].getString() != "ok")
		return;
	playInfo->currentWeek = js["week"].getInt();
	playInfo->whenRecvLeftTime = GameSystem::getCurrentTime_s();
	mediator->setFacebookLeftTime(js["lefttime"].getInt());
}
void FacebookRank::update(float dt)
{
	int remainS = (leftTime_s - (GameSystem::getCurrentTime_s() - playInfo->whenRecvLeftTime));
	static int lastReqTime = -1;
	if(remainS < 0 && playInfo->whenRecvLeftTime >= lastReqTime)
	{
		lastReqTime = GameSystem::getCurrentTime_s();
		graphdog->command("getweek", 0, this, gd_selector(ThisClassType::finishGetWeek));
	}
	else if(remainS >= 0)
	{
		int h = remainS / 3600;
		remainS %= 3600;
		int m = (remainS) / 60;
		remainS %= 60;
		int s = remainS;
		leftTimeFnt->setString(KS_Util::stringWithFormat("%02d:%02d:%02d", h, m, s).c_str());
	}
	
}
void FacebookRank::COUNTRYRANK(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	mediator->showCountryRank();
}
void FacebookRank::getRequestCountAndTicket(JsonBox::Value v2)
{
	/*
	 [{
	 "created_time" : "2013-06-28T02:37:10+0000",
	 "data" : "gameticketreq",
	 "from" : {
	 "id" : "100001181176876",
	 "name" : "한경수"
	 },
	 "id" : "467959046625773_100002141448085",
	 "message" : "ticketreq"
	 }]
	 이런식으로 들어옴.
	 
	 */
	
	CCArray* tds = CCArray::create();
	CCArray* recvTickets = CCArray::create();
	
	tds->init();
	recvTickets->init();
	
//	std::vector<TicketData> tds;
	JsonBox::Array arr = v2.getArray();
	// ticket 정보 추려냄.
	for(int i=0; i<arr.size(); i++)
	{
		JsonBox::Object e = arr[i].getObject();
		if(e["data"].getString() == "gameticketreq")
		{
			TicketData* td = new TicketData;
			td->autorelease();
			
			td->data = e["data"].getString();
			JsonBox::Object from  = e["from"].getObject();
			td->from.fbid = from["id"].getString();
			td->from.fbname = from["name"].getString();
			td->ticketid = e["id"].getString();
			tds->addObject(td);
		}
		if(e["data"].getString() == "gameticketrecv")
		{
			TicketData* td = new TicketData;
			td->autorelease();
			
			td->data = e["data"].getString();
			JsonBox::Object from  = e["from"].getObject();
			td->from.fbid = from["id"].getString();
			td->from.fbname = from["name"].getString();
			td->ticketid = e["id"].getString();
			recvTickets->addObject(td);
		}
	}
	
	if(recvTickets->count() != 0)
	{
		CCLog("no ticket");
		
		{
			CCNotificationCenter::sharedNotificationCenter()->postNotification("onRecvTicket", recvTickets);
		}
		
	}
	else
	{
		if(tds->count() != 0)
			CCNotificationCenter::sharedNotificationCenter()->postNotification("onRecvRequestTicket", tds);
	}
	
	
//	CCLog("%s", sendList.c_str());
	
	int c = v2.getArray().size() - tds->count();
	if(c > 0)
	{
		postboxNewSprite->setVisible(true);
		reqCount->setString(KS_Util::stringWithFormat("%d", c).c_str());
	}
	else
	{
		postboxNewSprite->setVisible(false);
		reqCount->setString("");
	}
	
	
	//postBoxView->setVisible(true);
	//facebookPostBox->setVisible(false);
	//facebookBack->setVisible(true);
}
void FacebookRank::POSTBOX(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	mediator->showFacebookPost();
	KS::KSLog("%", __FUNCTION__);
}

void FacebookRank::facebookLoginFinish(JsonBox::Value v2)
{
	
	
//	FBConnector::get()->getRequests(this, fb_selector(FacebookRank::getRequestCountAndTicket));
	fbLoading->setVisible(false);
	KS::KSLog("FACEBOOK LOGIN FINISH");
	JsonBox::Object o2 = v2.getObject();
	//ostringstream oss;
	KS::KSLog("%\n", v2);
	if(o2["error"].getInt() == FBConnectorResult::FBSuccess)
	{
		//facebookPostBox->setVisible(true);
		facebookLogin->setVisible(false);
		FBConnector::get()->getRequests(this, fb_selector(FacebookRank::getRequestFakeFinish));
		FBConnector::get()->getScores(this, fb_selector(FacebookRank::facebookGetScoresFinish));
		JsonBox::Object gParam;
		gParam["category"] = "facebook";
		gParam["log"] = "로그인성공";
		graphdog->command("writelog", &gParam, 0, 0);
	}
	else
	{
		mediator->incLoadCount();
		facebookLogin->setVisible(true);
		KSModalAlert("", gt("fail_facebook").c_str(), 1,
							  "OK", NOPARAM);
		JsonBox::Object gParam;
		gParam["category"] = "facebook";
		gParam["log"] = "로그인실패";
		graphdog->command("writelog", &gParam, 0, 0);
	}
}

void FacebookRank::getRequestFakeFinish(JsonBox::Value v2)
{
//	if(v2.getArray().empty() == false)
//	{
//		// new image
//		postboxNewSprite->setVisible(true);
//	}
}
void FacebookRank::facebookGetScoresFinish(JsonBox::Value v2)
{
	mediator->incLoadCount();
	fbLoading->setVisible(false);
	fblist = v2.getArray();
	
	
	ccView->reloadData();
	KS::KSLog("%", v2);
	//FBConnector::get()->sendScore(44444, this, fb_selector(IntroMain::facebookTest3));
}




void FacebookRank::INVITE(CCObject* item)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	KS::KSLog("invite start");
	CCMenuItem* realItem = (CCMenuItem*)item;
	int row = (int)realItem->getUserData();
	JsonBox::Object _info=fblist[row].getObject();
    JsonBox::Object _user=_info["user"].getObject();
	string fbid = _user["id"].getString();
	
	// 24시간 안에 초대를 보낸적이 있으면 앱 리케스트로 돌린다.
	if(NSDefault::getInviteIn24H())
	{
		// request
		// 보내는건 센드 리퀘스트지만 받는건 invitationFinish.
		FBConnector::get()->sendRequest(fbid, gt("together").c_str(), "INVITE", this, fb_selector(FacebookRank::invitationFinish));
	}
	else
	{
		// invite
		FBConnector::get()->sendInvite(fbid, "SportsWorldCup", gt("together").c_str(), "http://litqoo.com/sportsworldcup", "http://litqoo.com/sportsworldcup/icon.jpg",
									   this, fb_selector(FacebookRank::invitationFinish));
	}
	
}
void FacebookRank::invitationFinish(JsonBox::Value v)
{
	KS::KSLog("invitation finish");
	JsonBox::Object vo = v.getObject();
	JsonBox::Object gParam;
	gParam["category"] = "facebook";
	gParam["log"] = vo["id"].getString() + "를 초대했음.";
	graphdog->command("writelog", &gParam, 0, 0);
	
	if(vo["error"].getInt() == FBConnectorResult::FBSuccess) // succesx
	{
		NSDefault::setInviteIn24H();
		string returnedID = vo["id"].getString();
		NSDefault::setInvitable(returnedID);
		int searchedRow;
		for(int i = 0; i<fblist.size(); i++)
		{
			JsonBox::Object _info= fblist[i].getObject();
			JsonBox::Object _user=_info["user"].getObject();
			string fbid = _user["id"].getString();
			if(fbid == returnedID)
			{
				searchedRow = i;
				break;
			}
		}
		KS::KSLog("% %", __FILE__, __LINE__);
//		CCArray* cellArray = lqView->getCellArray();
		CCTableViewCell* cell = ccView->cellAtIndex(searchedRow);
		CCMenuItem* realItem = (CCMenuItem*)cell->getChildByTag(CELL_MENUS)->getChildByTag(INVITE_TAG);
		realItem->removeFromParentAndCleanup(true);
		
		
//		KS::KSLog("% %", __FILE__, __LINE__);
//		for(int i = 0; i<cellArray->count(); i++)
//		{
//			LQTableViewCell* cell = (LQTableViewCell*)cellArray->objectAtIndex(i);
//			if(cell->index == searchedRow)
//			{
//				CCMenuItem* realItem = (CCMenuItem*)cell->getChildByTag(CELL_MENUS)->getChildByTag(INVITE_TAG);
//				realItem->removeFromParentAndCleanup(true);
//				break;
//			}
//		}
		KS::KSLog("% %", __FILE__, __LINE__);
		
		if(mediator->getHeartSize() < 5)
		{
			mediator->addHeartSprite();
		}
		NSDefault::setHeartNumber(NSDefault::getHeartNumber() + 1);
		//remainHeartFnt->setString(KS_Util::stringWithFormat("%d", NSDefault::getHeartNumber()).c_str());
	}
	else
	{
		
	}
}
void FacebookRank::sendHeartFinish(JsonBox::Value v)
{
	KS::KSLog("sendheart finish");
	JsonBox::Object vo = v.getObject();
	JsonBox::Object gParam;
	gParam["category"] = "facebook";
	gParam["log"] = vo["id"].getString() + "에게 하트보냄";
	graphdog->command("writelog", &gParam, 0, 0);
	
	
	
	//string fbid = vo["id"].getString();
	if(vo["error"].getInt() == FBConnectorResult::FBSuccess) // succesx
	{
		string returnedID = vo["id"].getString();
		NSDefault::setSendable(returnedID);
		int searchedRow;
		for(int i = 0; i<fblist.size(); i++)
		{
			JsonBox::Object _info= fblist[i].getObject();
			JsonBox::Object _user=_info["user"].getObject();
			string fbid = _user["id"].getString();
			if(fbid == returnedID)
			{
				searchedRow = i;
				break;
			}
		}
		
		CCTableViewCell* cell = ccView->cellAtIndex(searchedRow);
		CCMenuItem* realItem = (CCMenuItem*)cell->getChildByTag(CELL_MENUS)->getChildByTag(HEART_TAG);
		KS::KSLog("% %", __FILE__, __LINE__);
		realItem->removeFromParentAndCleanup(true);
		
//		KS::KSLog("% %", __FILE__, __LINE__);
//		CCArray* cellArray = lqView->getCellArray();
//		for(int i = 0; i<cellArray->count(); i++)
//		{
//			LQTableViewCell* cell = (LQTableViewCell*)cellArray->objectAtIndex(i);
//			if(cell->index == searchedRow)
//			{
//				KS::KSLog("% %", __FILE__, __LINE__);
//				CCMenuItem* realItem = (CCMenuItem*)cell->getChildByTag(CELL_MENUS)->getChildByTag(HEART_TAG);
//				KS::KSLog("% %", __FILE__, __LINE__);
//				realItem->removeFromParentAndCleanup(true);
//				break;
//			}
//		}
		KS::KSLog("% %", __FILE__, __LINE__);
	}
	else
	{
		
	}
}


void FacebookRank::SENDHEART(CCObject* item)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	KS::KSLog("sendHeart start");
	CCMenuItem* realItem = (CCMenuItem*)item;
	int row = (int)realItem->getUserData();
	JsonBox::Object _info=fblist[row].getObject();
    JsonBox::Object _user=_info["user"].getObject();
	string fbid = _user["id"].getString();
	
	string bbmsg = KS_Util::stringWithFormat(gt("bbyong").c_str(), FBConnector::get()->userName.c_str());
	//	string bbmsg = "제발 한경수!";
	//	KSModalAlert("", bbmsg.c_str(), 1,
	//						  "OK", 0, 0);
//	::send
//	declval<<#class _Tp#>>()
	
	FBConnector::get()->sendRequest(fbid,  bbmsg, "HEART",
									this, fb_selector(ThisClassType::sendHeartFinish));
}
void FacebookRank::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
//    CCPoint offset = table->getContentOffset();
//    int p = cell->getIdx();
//    CCLOG("cell touched at index: %i", p);
//    datalist->removeObjectAtIndex(p);
//    table->removeCellAtIndex(p);
//    //여기까지 하면 이빨 빠진 듯 지워진다.
//    table->reloadData();
//    //여기까지 하면 잘 지워지지만 포코스가 무조건 맨 위로 맞춰진다. 그렇다고 원하는 인덱스 위치로 이동하는 메소드는 없다. 테이블뷰를 확장할 수도 있겠지만, 일단 제공되는 기능만으로 원하는 퍼퍼먼스를 만들어 보겠다.
//	
//	//위에서 지우는 당시 스크롤 뷰의 오프셋을 기억했다가 다시로딩 후 설정해준다.
//	//숫자 4 (화면에 보여지는 셀 갯수)는 맨위의 4개가 눌러졌을 때..
//	//오프셋이 다시 설정되면 이상하게 작동하기 때문이다.
//	//그리고 리로드하면 맨 위로 가는데 굳이 실행할 필요가 없어서 이기도 하다.
//    if(p > 4) table->setContentOffset(offset);
}


void FacebookRank::tableCellHighlight(CCTableView* table, CCTableViewCell* cell)
{
    CCLog("하일라이트");
//    CCLayerColor* mask = CCLayerColor::create(ccc4(200, 0, 0, 100), 320, 80);
//    cell->addChild(mask,100, 44);
}

void FacebookRank::tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell)
{
//    CCLog("언 하일라이트");
//    cell->removeChildByTag(44, true);
    
}

//CCTableViewDataSource
//cell(카톡게임에서 친구한명의 정보를 나타내는 영력)의 크기를 정의한다.
CCSize FacebookRank::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(FB_W, FB_CELL_H);
}

#define SAFE_REMOVECHILD(X) do{if(X){ X->removeFromParentAndCleanup(true); X = 0;}}while(0);
//reload가 호출 되거나, 스크롤이 움직여 안보이는 셀이 보여질 때 호출된다.
CCTableViewCell* FacebookRank::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    //테이블이 사용 중인 셀이 있다면 재활용한다. 테이블 뷰는 똑같은 모양에 데이터만 다른 경우가 많다. 그래서 하나의 셀을 재사용하고 데이터만 바꿔준다.
    CustomTableViewCell *cell = (CustomTableViewCell*)table->dequeueCell();
    if (!cell) { // 없다면 생성
		cell = new CustomTableViewCell();
		cell->init();
        cell->autorelease();
	}
	else
	{ // 원래 있던거면 지움. ㅋㅋㅋ 재사용  조까
		SAFE_REMOVECHILD(cell->profileSprite);
		SAFE_REMOVECHILD(cell->nameTTF);
		SAFE_REMOVECHILD(cell->rankTTF);
		SAFE_REMOVECHILD(cell->_checked);
		SAFE_REMOVECHILD(cell->rankSprite);
		SAFE_REMOVECHILD(cell->_menu);
		SAFE_REMOVECHILD(cell->inviteMsg);
		SAFE_REMOVECHILD(cell->scoreBM);
		
		
	}
    //datalist에서 데이터를 가져온다. 여기서는 CCString을 했지만, CCObject를 상속받은 커스텀 클래스를 만들어서 여러 정보를 담을 수 있을 것이다.
	JsonBox::Object _info=fblist[idx].getObject();
    JsonBox::Object _user=_info["user"].getObject();
	int score = _info["score"].getInt();
	string name = _user["name"].getString();
	string fbid = _user["id"].getString();
	string myid = FBConnector::get()->userID;
	
	cell->profileSprite =  FBConnector::get()->getProfileSprite(_user["id"].getString(), FB_PROFILE_W, FB_PROFILE_H);
	
    if(cell->profileSprite!=NULL)
	{
        cell->profileSprite->setPosition(ccp(50, 17));
        cell->addChild(cell->profileSprite,1);
    }
	
	
	
	cell->_back = 0;
	if(fbid == myid)
	{
		cell->_back = CCSprite::create("facebook_list2.png");
	}
//	if(fbid != myid)
//	{
//		cell->_back = CCSprite::create("facebook_list1.png");
//	}
//	else
//	{
//		cell->_back = CCSprite::create("facebook_list2.png");
//	}
	

	
	
	
	cell->nameTTF = CCLabelTTF::create(name.c_str(), defaultFont, 9.f);
	cell->nameTTF->setColor(ccc3(0, 0, 0));
	cell->nameTTF->setAnchorPoint(ccp(0, 0.5));
	cell->nameTTF->setPosition(ccp(76, 25));
	cell->addChild(cell->nameTTF, 1);
	
	//CCLabelTTF* rankTTF = CCLabelTTF::create(KS_Util::stringWithFormat("%d", row+1).c_str(), defaultFont, 12.f);
	int _ranking = idx+1;
	
	if(score < 0)
	{
		cell->rankTTF = CCLabelBMFont::create("-", "facebook_count.fnt");
		if(score < 0)
		{
			cell->rankTTF->setString("-");
		}
		cell->rankTTF->setPosition(ccp(17, 17));
		cell->addChild(cell->rankTTF, 1);
		if(cell->_back == 0)
		{
			cell->_back = CCSprite::create("facebook_list1.png");
		}
	}
	else
	{
		if(_ranking==1){
			cell->rankSprite = CCSprite::create("main_facebook_gold.png");
			cell->rankSprite->setPosition(ccp(17, 17));
			cell->addChild(cell->rankSprite,1);
			if(cell->_back == 0)
			{
				cell->_back = CCSprite::create("facebook_goldback.png");
			}
		}else if(_ranking==2){
			cell->rankSprite = CCSprite::create("main_facebook_silver.png");
			cell->rankSprite->setPosition(ccp(17, 17));
			cell->addChild(cell->rankSprite,1);
			if(cell->_back == 0)
			{
				cell->_back = CCSprite::create("facebook_silverback.png");
			}
		}else if(_ranking==3){
			cell->rankSprite = CCSprite::create("main_facebook_bronze.png");
			cell->rankSprite->setPosition(ccp(17, 17));
			cell->addChild(cell->rankSprite,1);
			if(cell->_back == 0)
			{
				cell->_back = CCSprite::create("facebook_bronzeback.png");
			}
		}else{
			cell->rankTTF = CCLabelBMFont::create(KS_Util::stringWithFormat("%d", idx+1).c_str(), "facebook_count.fnt");
			cell->rankTTF->setPosition(ccp(17, 17));
			cell->addChild(cell->rankTTF, 1);
			if(cell->_back == 0)
			{
				cell->_back = CCSprite::create("facebook_list1.png");
			}
		}
	}
	
	cell->_back->setAnchorPoint(ccp(0, 0));
	cell->addChild(cell->_back);
	CCPoint opPoint = ccp(205, 18);
	if(fbid != myid)
	{
		cell->_checked;
		if(score < 0 )
			cell->_checked = CCSprite::create("facebook_heartsendwait.png");
		else
			cell->_checked = CCSprite::create("facebook_heartsendwait.png");
		cell->_checked->setPosition(opPoint);
		cell->addChild(cell->_checked, 1);
	}
	string scoreStr;
	cell->_menu = CCMenu::create(0);
	cell->_menu->setPosition(ccp(0, 0));
	cell->_menu->setTag(CELL_MENUS);
	cell->addChild(cell->_menu, 1);
	if(score < 0)
	{
		scoreStr = gt("invite").c_str();
		if(NSDefault::isInvitable(fbid))
		{
			CCMenuItem* invite = CCMenuItemImage::create("facebook_heartplus.png", "facebook_heartplus.png",
														 this, menu_selector(FacebookRank::INVITE));
			invite->setTag(INVITE_TAG);
			invite->setUserData((void*)idx);
			invite->setPosition(opPoint					);
			cell->_menu->addChild(invite, 2);
			
		}
		cell->inviteMsg = CCLabelTTF::create(scoreStr.c_str(), defaultFont, 14.f);
		cell->inviteMsg->setColor(ccc3(0, 0, 0));
		cell->inviteMsg->setAnchorPoint(ccp(0, 0.5f));
		cell->inviteMsg->setPosition(ccp(85, 10));
		cell->addChild(cell->inviteMsg, 1);
	}
	else
	{
		scoreStr = KS_Util::stringWithFormat("%d", score);
		
		if(NSDefault::isSendable(fbid) && fbid != myid)
		{
			CCMenuItem* sendHeart = CCMenuItemImage::create("facebook_heartsend.png", "facebook_heartsend.png",
															this, menu_selector(FacebookRank::SENDHEART));
			sendHeart->setTag(HEART_TAG);
			sendHeart->setUserData((void*)idx);
			cell->_menu->addChild(sendHeart, 1);
			cell->_menu->setPosition(opPoint);
		}
		
		
		
		if(_ranking == 1)
		{
			cell->scoreBM = CCLabelBMFont::create(scoreStr.c_str(), "facebook_gold.fnt");
		}
		else if(_ranking == 2)
		{
			cell->scoreBM = CCLabelBMFont::create(scoreStr.c_str(), "facebook_silver.fnt");
		}
		else if(_ranking == 3)
		{
			cell->scoreBM = CCLabelBMFont::create(scoreStr.c_str(), "facebook_bronze.fnt");
		}
		else
		{
			cell->scoreBM = CCLabelBMFont::create(scoreStr.c_str(), "facebook_normal.fnt");
		}
		cell->scoreBM->setAnchorPoint(ccp(0, 0.5f));
		cell->scoreBM->setPosition(ccp(77, 10));
		cell->addChild(cell->scoreBM, 1);
	}
	
	return cell;
}

//테이블이 셀 갯수에 대한 정보를 가져가는 곳이다.
unsigned int FacebookRank::numberOfCellsInTableView(CCTableView *table)
{
    return fblist.size();
}








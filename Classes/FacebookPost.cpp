//
//  FacebookPost.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 5..
//
//

#include "FacebookPost.h"
#include "Bridge.h"
#include "NSDefault.h"
#include "KSBaseScene.h"
#include "IntroMain.h"
#include "KSoundEngine.h"
#include "SceneUtil.h"
//#include "IntroHead.h"

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


FacebookPost::~FacebookPost()
{
	FBDelegator::getInstance()->removeTarget(this);
}
#define SAFE_REMOVECHILD(X) do{if(X){ X->removeFromParentAndCleanup(true); X = 0;}}while(0);
//reload가 호출 되거나, 스크롤이 움직여 안보이는 셀이 보여질 때 호출된다.
CCTableViewCell* FacebookPost::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    //테이블이 사용 중인 셀이 있다면 재활용한다. 테이블 뷰는 똑같은 모양에 데이터만 다른 경우가 많다. 그래서 하나의 셀을 재사용하고 데이터만 바꿔준다.
    PostTableViewCell *cell = (PostTableViewCell*)table->dequeueCell();
    if (!cell) { // 없다면 생성
		cell = new PostTableViewCell();
		cell->init();
        cell->autorelease();
	}
	else
	{ // 원래 있던거면 지움. ㅋㅋㅋ 재사용  조까
		SAFE_REMOVECHILD(cell->profileSprite);
		SAFE_REMOVECHILD(cell->timeTTF);
		SAFE_REMOVECHILD(cell->_menu);
		SAFE_REMOVECHILD(cell->_back);
		SAFE_REMOVECHILD(cell->nameTTF);
	}
    JsonBox::Object _info=fbMailList[idx].getObject();
	JsonBox::Object _from=_info["from"].getObject();
	string name = _from["name"].getString();
	string fbid = _from["id"].getString();
	string time = "from";//_info["created_time"].getString();
	
	cell->profileSprite =  FBConnector::get()->getProfileSprite(fbid, FB_PROFILE_W, FB_PROFILE_H);
	
    if(cell->profileSprite!=NULL)
	{
        cell->profileSprite->setPosition(ccp(34, 17));
        cell->addChild(cell->profileSprite,1);
    }
	
	cell->_back = CCSprite::create("facebook_list3.png");
	
	cell->_back->setAnchorPoint(ccp(0, 0));
	cell->addChild(cell->_back);
	cell->nameTTF = CCLabelTTF::create(name.c_str(), defaultFont, 13.f);
	cell->nameTTF->setColor(ccc3(0, 0, 0));
	cell->nameTTF->setAnchorPoint(ccp(0, 0.5));
	cell->nameTTF->setPosition(ccp(60, 12));
	cell->addChild(cell->nameTTF, 1);
	
	cell->timeTTF = CCLabelTTF::create(time.c_str(), defaultFont, 7.f);
	cell->timeTTF->setColor(ccc3(0, 0, 0));
	cell->timeTTF->setAnchorPoint(ccp(0, 0.5f));
	cell->timeTTF->setPosition(ccp(60, 25));
	cell->addChild(cell->timeTTF, 1);
	
	cell->_menu = CCMenu::create(0);
	cell->_menu->setPosition(ccp(0, 0));
	cell->_menu->setTag(CELL_MENUS);
	cell->addChild(cell->_menu, 1);
	CCMenuItem* getHeart = CCMenuItemImage::create("facebook_heartplus.png", "facebook_heartplus.png",
												   this, menu_selector(FacebookPost::GETHEART));
	getHeart->setTag(GETHEART_TAG);
	getHeart->setUserData((void*)idx);
	getHeart->setPosition(ccp(204, 16));
	cell->_menu->addChild(getHeart, 2);
	
	return cell;
}



void FacebookPost::GETHEART(CCObject* item)
{
	CCMenuItem* realItem = (CCMenuItem*)item;
	int row = (int)realItem->getUserData();
	JsonBox::Object _info=fbMailList[row].getObject();
    string mailID = _info["id"].getString();
	KS::KSLog("mailID = %", mailID.c_str());
	
	FBConnector::get()->removeRequests(mailID, this, fb_selector(FacebookPost::removeRequestFinish));
}



void FacebookPost::removeRequestFinish(JsonBox::Value v)
{
	JsonBox::Object o2 = v.getObject();
	
	if(o2["error"].getInt() == FBConnectorResult::FBSuccess)
	{
		string mailID = o2["id"].getString();
		
		for(auto iter = fbMailList.begin(); iter != fbMailList.end(); ++iter)
		{
			JsonBox::Object _obj = iter->getObject();
			string searchedID = _obj["id"].getString();
			if(searchedID == mailID)
			{
				fbMailList.erase(iter);
				break;
			}
		}
		
		postBoxView->reloadData();

		if(mediator->getHeartSize() < 5)
			mediator->addHeartSprite();
//		if(pHead->getHeartSize() < 5)
//			pHead->addHeartSprite();
		NSDefault::setHeartNumber(NSDefault::getHeartNumber() + 1);
		//remainHeartFnt->setString(KS_Util::stringWithFormat("%d", NSDefault::getHeartNumber()).c_str());
	}
	
}
void FacebookPost::loadRequest()
{
	FBConnector::get()->getRequests(this, fb_selector(FacebookPost::getRequestFinish));
	fbLoading->setVisible(true);
}
void FacebookPost::getRequestFinish(JsonBox::Value v2)
{
	
	KS::KSLog("--------------------------getRequestFinish-----------------------");
	fbLoading->setVisible(false);
	fbMailList = v2.getArray();
	// v2 를 한번 출력해본다.
	// 여기서 티켓이 왔는지도 처리함.
	//FBConnector::get()->sendRequest(v2 에 있다., "누가 보냈습니다.", "gameticketnoreq", 0, 0);
	
	postBoxView->reloadData();
	//postBoxView->setVisible(true);
	//facebookPostBox->setVisible(false);
	//facebookBack->setVisible(true);
}

bool FacebookPost::init()
{
	//	CCNode::init();
	//	CCNode::create()
	CCLayer::init();
	setTouchEnabled(true);
	CCSprite* back = CCSprite::create("main_postbox_back.png");
	
	addChild(back);
	thiz = back;
	
	
	float loadingX = FB_X + FB_W / 2.f;
	float loadingY = FB_Y + FB_H / 2.f;
	auto animator = SceneUtil::playAnimation("loading.png", 0.1, 9, 9, 56, 56, true);
	animator.second->runAction(animator.first);
	thiz->addChild(animator.second, INT_MAX);
	animator.second->setPosition(ccp(loadingX, loadingY));
	fbLoading = animator.second;
	fbLoading->setVisible(false);

	
	postBoxView = CCTableView::create(this, CCSizeMake(FB_W, FB_H));
	postBoxView->setDirection(kCCScrollViewDirectionVertical);
	postBoxView->setVerticalFillOrder(kCCTableViewFillTopDown);
	postBoxView->setPosition(ccp(15, 52));
	postBoxView->setDelegate(this);
	

 	thiz->addChild(postBoxView,10);

	
	CCMenu* _menu = CCMenu::create(0);
	_menu->setPosition(CCPointZero);
	thiz->addChild(_menu);
	{
		CCMenuItem* item = CCMenuItemImage::create("main_font_back.png", "main_font_back_down.png",
																this, menu_selector(FacebookPost::BACK));
		item->setPosition(ccp(132, 26));
		_menu->addChild(item, 2);
	}
		//	this->runAction(CCRepeat::create(CCSequence::createWithTwoActions(CCMoveBy::create(1.f, ccp(0, 100)),
	//																		  CCMoveBy::create(1.f, ccp(0, -100))), 100));
	return true;
}

void FacebookPost::BACK(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	mediator->showBackToFacebook();	
}

void FacebookPost::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
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


void FacebookPost::tableCellHighlight(CCTableView* table, CCTableViewCell* cell)
{
    CCLog("하일라이트");
	//    CCLayerColor* mask = CCLayerColor::create(ccc4(200, 0, 0, 100), 320, 80);
	//    cell->addChild(mask,100, 44);
}

void FacebookPost::tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell)
{
	//    CCLog("언 하일라이트");
	//    cell->removeChildByTag(44, true);
    
}

//CCTableViewDataSource

//cell(카톡게임에서 친구한명의 정보를 나타내는 영력)의 크기를 정의한다.
CCSize FacebookPost::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(FB_W, FB_CELL_H);
}

//테이블이 셀 갯수에 대한 정보를 가져가는 곳이다.
unsigned int FacebookPost::numberOfCellsInTableView(CCTableView *table)
{
    return fbMailList.size();
}



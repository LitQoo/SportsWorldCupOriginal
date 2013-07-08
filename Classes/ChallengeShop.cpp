//
//  ChallengeShop.cpp
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 6. 21..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "ChallengeShop.h"

#include "GameShop.h"
#include "KS_Util.h"
#include "GameSystem.h"
#include "Bridge.h"
#include "KSModalAlert.h"
#include "PriorityMenu.h"
#include "NSDefault.h"
#include "KSoundEngine.h"
#include "GDTranslate.h"
#include "ScrollMenu.h"
#include "SceneUtil.h"

using namespace GDTranslate;

const int ChallengeShop::LIMITNUMBER = 5;
class ChallengeItemLayer : public CCLayerColor
{
private:
	std::map<int, std::string> item2ItemName;
	std::map<std::string, CCSprite*> checks;
	
	int createIndex;
	ChallengeShop* challengeShop;
public:
	map<string, GameShop::ITEM_SELTYPE> selectedItems;
	ChallengeItemLayer(){
		createIndex = 0;
	}
	void setChallengeShop(ChallengeShop* s){challengeShop = s;}
	CREATE_FUNC(ChallengeItemLayer);
	static ChallengeItemLayer* create(const std::deque<std::string>& d) \
	{ \
		ChallengeItemLayer *pRet = new ChallengeItemLayer(); \
		if (pRet && pRet->init(d)) \
		{ \
			pRet->autorelease(); \
			return pRet; \
		} \
		else \
		{ \
			delete pRet; \
			pRet = NULL; \
			return NULL; \
		} \
	}

	bool init()
	{
		CCLayerColor::initWithColor(ccc4(255, 0, 0, 0));
		
//		CCSprite* test = CCSprite::create("ak_rightfire.png");
//		test->setPosition(ccp(0, 0));
//		addChild(test);
		return true;
	}
	void ITEMSELECT(CCObject* m)
	{
		CCMenuItemToggle* e = (CCMenuItemToggle*)m;
//		CCLog("%s", item2ItemName[(int)e].c_str());
		
		int i = e->getSelectedIndex();
//		desc->setString(gt("time").c_str());
		//선택 해지할 때
		if(i == 0)
		{
			CCLog("unsel");
			CCLog("%x", checks[item2ItemName[e->getTag()]]);
			checks[item2ItemName[e->getTag()]]->setVisible(false);
			
//			checks[2]->setVisible(false);
			selectedItems[item2ItemName[e->getTag()]] = GameShop::NONE;
		}
		//선택할 때
		else if(i == 1)
		{
			int trueCount = challengeShop->getTrueCount();
			if(trueCount >= ChallengeShop::LIMITNUMBER)
			{
				e->setSelectedIndex(0);
				KSModalAlert("", gt("itemnumberlimit2").c_str(), 1,
							 "ok", NOPARAM);
			}
			else
			{
				checks[item2ItemName[e->getTag()]]->setVisible(true);
				selectedItems[item2ItemName[e->getTag()]] = GameShop::SEL;
			}			
		}
		
		CCString* t = CCString::create(gt(item2ItemName[e->getTag()]).c_str());
		CCNotificationCenter::sharedNotificationCenter()->postNotification("onDesc", t);
		challengeShop->refreshPrice();
	}
	
	void addItemds(const std::string& item1, const std::string& item2, float& x, float y, float pad)
	{
		ScrollMenu* _menu = ScrollMenu::create(0);
		
//		_menu->setTouchPriority(1);
		_menu->setPosition(CCPointZero);
		addChild(_menu);
		
		{
			CCMenuItem* _Item1 = CCMenuItemImage::create(item1.c_str(), item1.c_str());
			CCMenuItem* _Item2 = CCMenuItemImage::create(item1.c_str(), item1.c_str());

//			code2ItemName[(int)_Item1]
			
			_Item1->setScale(0.9f);
			_Item2->setScale(0.9f);
			CCMenuItemToggle* items = CCMenuItemToggle::createWithTarget(this, menu_selector(ThisClassType::ITEMSELECT), _Item1, _Item2, 0);
			items->setTag(createIndex);
			
			//Check Sprite 끼워넣음.
			CCSprite* checkSprite = CCSprite::create("item_check.png");
			checkSprite->setVisible(false);
			items->addChild(checkSprite, 2);
			checkSprite->setPosition(ccp( 50, 50));
			checks[item1] = checkSprite;
			
			item2ItemName[createIndex] = item1;
			items->setPosition(ccp(x, y));
			_menu->addChild(items, 2);
			
			x += pad;
			createIndex++;
		}
		{
			CCMenuItem* _Item1 = CCMenuItemImage::create(item2.c_str(), item2.c_str());
			CCMenuItem* _Item2 = CCMenuItemImage::create(item2.c_str(), item2.c_str());
			_Item1->setScale(0.9f);
			_Item2->setScale(0.9f);
			CCMenuItemToggle* items = CCMenuItemToggle::createWithTarget(this, menu_selector(ThisClassType::ITEMSELECT), _Item1, _Item2, 0);
			items->setTag(createIndex);

			//Check Sprite 끼워넣음.
			CCSprite* checkSprite = CCSprite::create("item_check.png");
			checkSprite->setVisible(false);
			items->addChild(checkSprite, 2);
			checkSprite->setPosition(ccp( 50, 50));
			checks[item2] = checkSprite;
			
			item2ItemName[createIndex] = item2;
			items->setPosition(ccp(x, y));
			_menu->addChild(items, 2);
			
			x += pad;
			createIndex++;
		}	
	}
	bool init(const std::deque<std::string>& d)
	{
		CCLayerColor::initWithColor(ccc4(255, 0, 0, 0));
		
		int i = 0;
		float x = 80;
		float y = 51;
		float pad = 77;
		
		
		for(auto e : d)
		{
			CCLog("%s", e.c_str());
			if(e == "AK")
			{
				addItemds("item_bird1.png", "item_large1.png", x, y, pad);
			}
			else if(e == "BS")
			{
				addItemds("item_tipin1.png", "item_longguide1.png", x, y, pad);
			}
			else if(e == "AC")
			{
				addItemds("item_wind1.png", "item_vibration1.png", x, y, pad);
			}
			else if(e == "HG")
			{
				addItemds("item_hanggliding1.png", "item_hanggliding2.png", x, y, pad);
			}
			else if(e == "SK")
			{
				addItemds("item_skwide1.png", "item_curvespeed1.png", x, y, pad);
			}
			else if(e == "HW")
			{
				addItemds("item_highhurdle1.png", "item_wideview1.png", x, y, pad);
			}
//			i++;
		}
		
//		CCSprite* test = CCSprite::create("ak_rightfire.png");
//		test->setPosition(ccp(0, 0));
//		addChild(test);
		return true;
	}
};

ChallengeShop::ChallengeShop()
{
	position.exp = ccp(53, 207);
	position.shield = ccp(132, 207);
	position.time = ccp(211, 207);
	
	levelLimits["exp"] = 1;
	levelLimits["shield"] = 10;
	levelLimits["item_random"] = 1; // 25
	// 새
	levelLimits["item_bird1.png"] = 5; // 25
	levelLimits["item_large1.png"] = 25; // 25
	
	// 양궁
	levelLimits["item_wind1.png"] = 5; // 25
	levelLimits["item_vibration1.png"] = 25; // 25
	
	// 행글
	levelLimits["item_hanggliding1.png"] = 5; // 25
	levelLimits["item_hanggliding2.png"] = 25; // 25
	
	// 허들
	levelLimits["item_highhurdle1.png"] = 5; // 25
	levelLimits["item_wideview1.png"] = 25; // 25
	
	// 스키
	levelLimits["item_skwide1.png"] = 5; // 25
	levelLimits["item_curvespeed1.png"] = 25; // 25
	
	// 농구
	levelLimits["item_longguide1.png"] = 5; // 25
	levelLimits["item_tipin1.png"] = 25; // 25
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChallengeShop::onDesc),
																  "onDesc", 0);
}
ChallengeShop::~ChallengeShop()
{
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}
void ChallengeShop::registerWithTouchDispatcher()
{
//	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
}
bool ChallengeShop::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = pTouch;

	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
	
	return true;
}

int ChallengeShop::getRealPrice()
{
	return getTotalPrice();
}
int ChallengeShop::getTotalPrice()
{
	std::map<std::string, int> itemPricesTable;// = priceTable();
	itemPricesTable["exp"] = 400;
	itemPricesTable["shield"] = 1000;
	itemPricesTable["item_random"] = 800;
	itemPricesTable["item_bird1.png"] = itemPricesTable["item_wind1.png"] =
	itemPricesTable["item_hanggliding1.png"] = itemPricesTable["item_highhurdle1.png"] =
	itemPricesTable["item_skwide1.png"] = itemPricesTable["item_tipin1.png"] = 700;
	itemPricesTable["item_large1.png"] = itemPricesTable["item_vibration1.png"] =
	itemPricesTable["item_hanggliding2.png"] = itemPricesTable["item_wideview1.png"] =
	itemPricesTable["item_survespeed1.png"] = itemPricesTable["item_longguide1.png"] = 1300;
	
	int totalPrice = 0;
	for(auto iter = selectedItems.begin(); iter != selectedItems.end(); ++iter)
	{
		if(iter->second)
			totalPrice += itemPricesTable.at(iter->first);
	}
	
	for(auto iter = itemLayer->selectedItems.begin();
		iter != itemLayer->selectedItems.end(); ++iter)
	{
		if(iter->second)
			totalPrice += itemPricesTable.at(iter->first);
	}
	return totalPrice;
}

bool ChallengeShop::init()
{
	CCSprite::init();
	
	playInfo->createWorldcupOrder();
	CCSprite* back = CCSprite::create("itemshop_back.png");
	
	addChild(back);
	thiz = back;
	
	//134, 93
	CCSprite* scrollBack = CCSprite::create("itemshop_scrollbar.png");
	scrollBack->setPosition(ccp(134, 93));
	thiz->addChild(scrollBack, 2);
	
	desc = CCLabelTTF::create("", defaultFont, 14.f);
	desc->setPosition(ccp(130, 66));
	thiz->addChild(desc, 4);
	
	
	CCMenu* pMenu = CCMenu::create();
	pMenu->setTouchPriority(kCCMenuHandlerPriority - 1);
	pMenu->setPosition(CCPointZero);
	thiz->addChild(pMenu, 2);
	
	CCMenu* _menu = CCMenu::create(0);
	_menu->setPosition(CCPointZero);
	thiz->addChild(_menu);
	
	

	ChallengeItemLayer* il = ChallengeItemLayer::create(playInfo->getWorldcupOrder());
	il->setChallengeShop(this);
	itemLayer = il;
	
	il->setContentSize(CCSizeMake(654 + 60, 70));
	il->ignoreAnchorPointForPosition(false);
//	CCLayerColor *layer = CCLayerColor::create(ccc4(255, 0, 255, 255));
//	layer->setAnchorPoint(ccp(0,0));
//	layer->setPosition(ccp(0,0));
//	layer->ignoreAnchorPointForPosition(false);
	CCScrollView* sv = CCScrollView::create();
	
	sv->setViewSize(CCSizeMake(238, 86)); // 238
//	sv->setContentSize(CCSizeMake(654, 70));
	sv->setDirection(CCScrollViewDirection::kCCScrollViewDirectionHorizontal);
	
	sv->setContentOffset(ccp(0,0 ));
	sv->setPosition(ccp(15, 87));
	sv->setContainer(il);
	sv->setDelegate(this);
	thiz->addChild(sv, 100);
	bar = ScrollBar::creatScrollbar(sv, 0, "itemshop_scroll_button.png");
	bar->setDynamicScrollSize(false);
//	sv->addChild(bar);
//	getCamera()->setEyeXYZ(0, 0.00003f, 0.f);
	{
		
	}
	
	
	{
		auto ar1 = SceneUtil::playAnimation("item_random.png", 0.5, 2, 2, 75, 75, true);
		auto ar2 = SceneUtil::playAnimation("item_random.png", 0.5f, 2, 2, 75, 75, true);
		auto ar3 = SceneUtil::playAnimation("item_random.png", 0.5f, 2, 2, 75, 75, true);
		auto ar4 = SceneUtil::playAnimation("item_random.png", 0.5f, 2, 2, 75, 75, true);
		CCSprite* r1 = ar1.second;
		r1->runAction(ar1.first);
		
		CCSprite* r2 = ar2.second;
		r2->runAction(ar2.first);
		
		CCSprite* r3 = ar3.second;
		r3->runAction(ar3.first);
		
		CCSprite* r4 = ar4.second;
		r4->runAction(ar4.first);
		
		CCMenuItem* timeItem1 = CCMenuItemSprite::create(r1, r2);
		CCMenuItem* timeItem2 = CCMenuItemSprite::create(r3, r4);
		timeItem1->setScale(0.9f);
		timeItem2->setScale(0.9f);
		times = CCMenuItemToggle::createWithTarget(this, menu_selector(THIS::RANDOM), timeItem1, timeItem2, 0);
		times->setPosition(position.time);
		_menu->addChild(times, 2);
		
		auto l_e = playInfo->getLV_EXP(NSDefault::getExp());
		if(l_e.first < levelLimits["item_random"])
		{
			CCMenuItem* lock = CCMenuItemImage::create("item_lock.png", "item_lock.png",
													   this, menu_selector(THIS::LEVELLIMIT));
			pMenu->addChild(lock, 2);
			lock->setPosition(position.time);
			lock->setUserData((void*)"item_random");
		}
	}
	{
		CCMenuItem* shieldItem1 = CCMenuItemImage::create("item_shield1.png", "item_shield1.png");
		CCMenuItem* shieldItem2 = CCMenuItemImage::create("item_shield1.png", "item_shield1.png");
		shieldItem1->setScale(0.9f);
		shieldItem2->setScale(0.9f);
		shields = CCMenuItemToggle::createWithTarget(this, menu_selector(THIS::SHIELD), shieldItem1, shieldItem2, 0);
		shields->setPosition(position.shield);
		_menu->addChild(shields, 2);
		
		auto l_e = playInfo->getLV_EXP(NSDefault::getExp());
		if(l_e.first < levelLimits["shield"])
		{
			CCMenuItem* lock = CCMenuItemImage::create("item_lock.png", "item_lock.png",
													   this, menu_selector(THIS::LEVELLIMIT));
			pMenu->addChild(lock, 2);
			lock->setPosition(position.shield);
			lock->setUserData((void*)"shield");
		}
	}
	{
		CCMenuItem* expItem1 = CCMenuItemImage::create("item_exp1.png", "item_exp1.png");
		CCMenuItem* expItem2 = CCMenuItemImage::create("item_exp1.png", "item_exp1.png");
		expItem1->setScale(0.9f);
		expItem2->setScale(0.9f);
		exps = CCMenuItemToggle::createWithTarget(this, menu_selector(THIS::EXP), expItem1, expItem2, 0);
		exps->setPosition(position.exp);
		_menu->addChild(exps, 2);
		
		auto l_e = playInfo->getLV_EXP(NSDefault::getExp());
		if(l_e.first < levelLimits["exp"])
		{
			CCMenuItem* lock = CCMenuItemImage::create("item_lock.png", "item_lock.png",
													   this, menu_selector(THIS::LEVELLIMIT));
			pMenu->addChild(lock, 2);
			lock->setPosition(position.exp);
			lock->setUserData((void*)"exp");
		}
	}
	
	{
		CCSprite* ch = CCSprite::create("item_check.png");
		ch->setPosition(ccp(73, 225));
		thiz->addChild(ch, 2);
		ch->setVisible(false);
		checks.push_back(ch);
	}
	{
		CCSprite* ch = CCSprite::create("item_check.png");
		ch->setPosition(ccp(152, 225));
		thiz->addChild(ch, 2);
		ch->setVisible(false);
		checks.push_back(ch);
	}
	{
		CCSprite* ch = CCSprite::create("item_check.png");
		ch->setPosition(ccp(231, 225));
		thiz->addChild(ch, 2);
		ch->setVisible(false);
		checks.push_back(ch);
	}
	
	price = CCLabelBMFont::create("0", "total_gold.fnt");
	price->setPosition(ccp(180, 24));
	price->setColor(ccc3(0, 0, 0));
	thiz->addChild(price, 2);
//	this->setIsTouchEnabled(true);
	
	return true;
}

int ChallengeShop::getTrueCount()
{
	int trueCount = 0;
	for(auto iter = selectedItems.begin(); iter != selectedItems.end(); ++iter)
	{
		if(iter->second == true)
			trueCount++;
	}
	for(auto iter = itemLayer->selectedItems.begin(); iter != itemLayer->selectedItems.end(); ++iter)
	{
		if(iter->second == true)
			trueCount++;
	}
	return trueCount;
}
void ChallengeShop::RANDOM(CCObject* s)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	CCMenuItemToggle* _s = (CCMenuItemToggle*)s;
	
	int i = _s->getSelectedIndex();
	desc->setString(gt("time").c_str());
	//선택 해지할 때
	if(i == 0)
	{
		checks[2]->setVisible(false);
		selectedItems["item_random"] = GameShop::NONE;
//		offTime();
	}
	//선택할 때
	else if(i == 1)
	{
		int trueCount = getTrueCount();
		
		if(trueCount >= LIMITNUMBER)
		{
			_s->setSelectedIndex(0);
			KSModalAlert("", gt("itemnumberlimit2").c_str(), 1,
						 "ok", NOPARAM);
		}
		else
		{
//			isSaled = false;
			checks[2]->setVisible(true);
			selectedItems["item_random"] = GameShop::SEL;
			
//			onTime();
		}
		
		
	}
	refreshPrice();
//	refreshPriceTable();
}
void ChallengeShop::SHIELD(CCObject* s)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	CCMenuItemToggle* _s = (CCMenuItemToggle*)s;
	
	int i = _s->getSelectedIndex();
	desc->setString(gt("shield").c_str());
	//선택 해지할 때
	if(i == 0)
	{
		checks[1]->setVisible(false);
		selectedItems["shield"] = GameShop::NONE;
//		isSaled = false;
//		offShield();
	}
	//선택할 때
	else if(i == 1)
	{
		int trueCount = getTrueCount();
		if(trueCount >= LIMITNUMBER)
		{
			_s->setSelectedIndex(0);
			KSModalAlert("", gt("itemnumberlimit2").c_str(), 1,
						 "ok", NOPARAM);
		}
		else
		{
//			isSaled = false;
			checks[1]->setVisible(true);
			selectedItems["shield"] = GameShop::SEL;
//			onShield();
			
		}
	}
	refreshPrice();
//	refreshPriceTable();
}

void ChallengeShop::onDesc(CCObject* e)
{
	CCString* s = (CCString*)e;
	desc->setString(gt(s->getCString()).c_str());
}

map<string, GameShop::ITEM_SELTYPE> ChallengeShop::getSelectedItems()
{
	map<string, GameShop::ITEM_SELTYPE> ret;
	ret = itemLayer->selectedItems;
	for(auto i : selectedItems)
	{
		ret[i.first] = i.second;
//		if(i.second == true)
//		{
//			ret[i.first] = true;
//		}
	}
	return ret;
}
void ChallengeShop::EXP(CCObject* s)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	CCMenuItemToggle* _s = (CCMenuItemToggle*)s;
	desc->setString(gt("exp").c_str());
	int i = _s->getSelectedIndex();
	//선택 해지할 때
	if(i == 0)
	{
//		isSaled = false;
		checks[0]->setVisible(false);
		selectedItems["exp"] = GameShop::NONE;
//		offExp();
	}
	//선택할 때
	else if(i == 1)
	{
		int trueCount = getTrueCount();
		
		if(trueCount >= LIMITNUMBER)
		{
			_s->setSelectedIndex(0);
			KSModalAlert("", gt("itemnumberlimit2").c_str(), 1,
						 "ok", NOPARAM);
		}
		else
		{
//			isSaled = false;
			checks[0]->setVisible(true);
			selectedItems["exp"] = GameShop::SEL;
//			onExp();
		}
	}
	refreshPrice();
//	price->setString(KS_Util::stringWithFormat("%d", getTotalPrice()).c_str());
//	refreshPriceTable();
}

void ChallengeShop::refreshPrice()
{
	price->setString(CCString::createWithFormat("%d", getRealPrice())->getCString());
}
void ChallengeShop::LEVELLIMIT(CCObject* m)
{
	const char* category = (const char*)((CCMenuItemImage*)m)->getUserData();
	string category_s = category;
	CCString* p = CCString::createWithFormat("Level limit is %d.", levelLimits[category_s]);
	CCNotificationCenter::sharedNotificationCenter()->postNotification("showAlert", (CCObject*)p);
//	mediator->showAlert(KS_Util::stringWithFormat("Level limit is %d.", levelLimits[category_s]).c_str());
}
void ChallengeShop::onEnterTransitionDidFinish()
{
	CCSprite::onEnterTransitionDidFinish();
}
void ChallengeShop::onExit()
{
	
	
	CCSprite::onExit();
}

//
//  GameShop.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 12..
//
//

#include "GameShop.h"
#include "KS_Util.h"
#include "GameSystem.h"
#include "Bridge.h"
#include "KSModalAlert.h"
#include "PriorityMenu.h"
#include "NSDefault.h"
#include "KSoundEngine.h"
#include "GDTranslate.h"
#include "SceneUtil.h"
using namespace GDTranslate;
const float GameShop::SALERATE = 0.7f;
bool GameShop::init()
{
	CCSprite::init();
	
	
	
	
	
	return true;
}
void GameShop::LEVELLIMIT(CCObject* m)
{
	const char* category = (const char*)((CCMenuItemImage*)m)->getUserData();
	string category_s = category;
	mediator->showAlert(KS_Util::stringWithFormat("Level limit is %d.", levelLimits[category_s]).c_str());
}
void GameShop::initMenu(string gamename, string itemfile1, string itemfile2)
{
	CCSprite* back = CCSprite::create("itemshop_back.png");
	
	addChild(back);
	thiz = back;
	levelLimits[item1Name] = 5;
	levelLimits[item2Name] = 25;
	CCMenu* pMenu = CCMenu::create();
	pMenu->setTouchPriority(kCCMenuHandlerPriority - 1);
	pMenu->setPosition(CCPointZero);
	thiz->addChild(pMenu, 2);
	
	CCMenu* _menu = CCMenu::create(0);
	_menu->setPosition(CCPointZero);
	thiz->addChild(_menu);
	
	{
		auto ar1 = SceneUtil::playAnimation("item_random.png", 0.5f, 2, 2, 75, 75, true);
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
		
//		CCMenuItem* qwe =
		CCMenuItem* timeItem1 = CCMenuItemSprite::create(r1, r2);
		CCMenuItem* timeItem2 = CCMenuItemSprite::create(r3, r4);
		times = CCMenuItemToggle::createWithTarget(this, menu_selector(ThisClassType::RANDOM), timeItem1, timeItem2, 0);
		times->setPosition(position.time);
		_menu->addChild(times, 2);
		
		auto l_e = playInfo->getLV_EXP(NSDefault::getExp());
		if(l_e.first < levelLimits["item_random"])
		{
			CCMenuItem* lock = CCMenuItemImage::create("item_lock.png", "item_lock.png",
																	this, menu_selector(ThisClassType::LEVELLIMIT));
			pMenu->addChild(lock, 2);
			lock->setPosition(position.time);
			lock->setUserData((void*)"item_random");
		}
	}
	{
		CCMenuItem* shieldItem1 = CCMenuItemImage::create("item_shield1.png", "item_shield1.png");
		CCMenuItem* shieldItem2 = CCMenuItemImage::create("item_shield1.png", "item_shield1.png");
		shields = CCMenuItemToggle::createWithTarget(this, menu_selector(ThisClassType::SHIELD), shieldItem1, shieldItem2, 0);
		shields->setPosition(position.shield);
		_menu->addChild(shields, 2);
		
		auto l_e = playInfo->getLV_EXP(NSDefault::getExp());
		if(l_e.first < levelLimits["shield"])
		{
			CCMenuItem* lock = CCMenuItemImage::create("item_lock.png", "item_lock.png",
																	this, menu_selector(ThisClassType::LEVELLIMIT));
			pMenu->addChild(lock, 2);
			lock->setPosition(position.shield);
			lock->setUserData((void*)"shield");
		}
	}
	{
		CCMenuItem* expItem1 = CCMenuItemImage::create("item_exp1.png", "item_exp1.png");
		CCMenuItem* expItem2 = CCMenuItemImage::create("item_exp1.png", "item_exp1.png");
		exps = CCMenuItemToggle::createWithTarget(this, menu_selector(ThisClassType::EXP), expItem1, expItem2, 0);
		exps->setPosition(position.exp);
		_menu->addChild(exps, 2);
		
		auto l_e = playInfo->getLV_EXP(NSDefault::getExp());
		if(l_e.first < levelLimits["exp"])
		{
			CCMenuItem* lock = CCMenuItemImage::create("item_lock.png", "item_lock.png",
																	this, menu_selector(ThisClassType::LEVELLIMIT));
			pMenu->addChild(lock, 2);
			lock->setPosition(position.exp);
			lock->setUserData((void*)"exp");
		}
	}
	{
		CCMenuItem* item1 = CCMenuItemImage::create(itemfile1.c_str(), itemfile1.c_str());
		CCMenuItem* item2 = CCMenuItemImage::create(itemfile1.c_str(), itemfile1.c_str());
		CCMenuItemToggle* items = CCMenuItemToggle::createWithTarget(this, menu_selector(ThisClassType::GAMEITEM1), item1, item2, 0);
		gameItem1 = items;
		items->setPosition(position.gameItem1);
		_menu->addChild(items, 2);
		
		auto l_e = playInfo->getLV_EXP(NSDefault::getExp());
		if(l_e.first < levelLimits[item1Name])
		{
			CCMenuItem* lock = CCMenuItemImage::create("item_lock.png", "item_lock.png",
																	this, menu_selector(ThisClassType::LEVELLIMIT));
			pMenu->addChild(lock, 2);
			lock->setPosition(position.gameItem1);
			lock->setUserData((void*)item1Name.c_str());
		}
	}
	{
		CCMenuItem* item1 = CCMenuItemImage::create(itemfile2.c_str(), itemfile2.c_str());
		CCMenuItem* item2 = CCMenuItemImage::create(itemfile2.c_str(), itemfile2.c_str());
		CCMenuItemToggle* items = CCMenuItemToggle::createWithTarget(this, menu_selector(ThisClassType::GAMEITEM2), item1, item2, 0);
		gameItem2 = items;
		items->setPosition(position.gameItem2);
		_menu->addChild(items, 2);

		auto l_e = playInfo->getLV_EXP(NSDefault::getExp());
		if(l_e.first < levelLimits[item2Name])
		{
			CCMenuItem* lock = CCMenuItemImage::create("item_lock.png", "item_lock.png",
																	this, menu_selector(ThisClassType::LEVELLIMIT));
			pMenu->addChild(lock, 2);
			lock->setPosition(position.gameItem2);
			lock->setUserData((void*)item2Name.c_str());
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
	{
		CCSprite* ch = CCSprite::create("item_check.png");
		ch->setPosition(ccp(112, 147));
		thiz->addChild(ch, 2);
		ch->setVisible(false);
		checks.push_back(ch);
	}
	{
		CCSprite* ch = CCSprite::create("item_check.png");
		ch->setPosition(ccp(192, 147));
		thiz->addChild(ch, 2);
		ch->setVisible(false);
		checks.push_back(ch);
	}

	if(playInfo->currentGame == gamename)
	{
		for( auto iter = playInfo->selectedItems.begin(); iter != playInfo->selectedItems.end(); ++iter)
		{
			if(iter->second == GameShop::SEL)
			{
				isSaled = true;
				break;
			}
		}
		selectedItems = playInfo->selectedItems;
		if(playInfo->selectedItems["item_random"] == GameShop::SEL)
		{
			times->setSelectedIndex(1);
			checks[2]->setVisible(true);
			onTime();
		}
		if(playInfo->selectedItems["shield"] == GameShop::SEL)
		{
			shields->setSelectedIndex(1);
			checks[1]->setVisible(true);
			onShield();
		}
		if(playInfo->selectedItems["exp"] == GameShop::SEL)
		{
			exps->setSelectedIndex(1);
			checks[0]->setVisible(true);
			onExp();
		}
		if(playInfo->selectedItems[item1Name] == GameShop::SEL)
		{
			gameItem1->setSelectedIndex(1);
			checks[3]->setVisible(true);
			onItem1();
		}
		if(playInfo->selectedItems[item2Name] == GameShop::SEL)
		{
			gameItem2->setSelectedIndex(1);
			checks[4]->setVisible(true);
			onItem2();
		}
		if(playInfo->selectedItems[item2Name] == GameShop::SEL)
		{
			CCLog("???");
		}
	}
	
	saleTable = CCSprite::create("itemshop_totalsale.png");
	saleTable->setPosition(ccp(135, 22));
	thiz->addChild(saleTable, 3);
	
	strokePrice = CCSprite::create("itemshop_sale.png");
	strokePrice->setPosition(ccp(130, 32));
	thiz->addChild(strokePrice, 5);
	
	price = CCLabelBMFont::create("0", "total_gold.fnt");
	price->setPosition(ccp(180, 24));
	price->setColor(ccc3(0, 0, 0));
	thiz->addChild(price, 2);
	
	originalPrice = CCLabelBMFont::create("0", "total_salegold.fnt");
	originalPrice->setPosition(ccp(135, 24));
	originalPrice->setColor(ccc3(0, 0, 0));
	thiz->addChild(originalPrice, 4);
	originalPrice->setVisible(false);
	
	saledPrice = CCLabelBMFont::create("0", "total_salegold.fnt");
	saledPrice->setPosition(ccp(201, 24));
	saledPrice->setColor(ccc3(0, 0, 0));
	thiz->addChild(saledPrice, 4);
	saledPrice->setVisible(false);
	
	refreshPriceTable();
	
	desc = CCLabelTTF::create("", defaultFont, 14.f);
	desc->setPosition(ccp(130, 66));
	thiz->addChild(desc, 4);
}

void GameShop::refreshPriceTable()
{
	if(isSaled)
	{
		saleTable->setVisible(true);
		strokePrice->setVisible(true);
		saledPrice->setVisible(true);
		originalPrice->setVisible(true);
		price->setVisible(false);
		
		originalPrice->setString(KS_Util::stringWithFormat("%d", getTotalPrice()).c_str());
		saledPrice->setString(KS_Util::stringWithFormat("%d", (int)(getTotalPrice() * SALERATE)).c_str());
	}
	else
	{
		// 180, 24
		saleTable->setVisible(false);
		strokePrice->setVisible(false);
		saledPrice->setVisible(false);
		originalPrice->setVisible(false);
		price->setVisible(true);
		
		price->setString(KS_Util::stringWithFormat("%d", getTotalPrice()).c_str());
	}
}
void GameShop::RANDOM(CCObject* s)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	CCMenuItemToggle* _s = (CCMenuItemToggle*)s;
	
	int i = _s->getSelectedIndex();
	desc->setString(gt("random").c_str());
	//선택 해지할 때
	if(i == 0)
	{
		checks[2]->setVisible(false);
		selectedItems["item_random"] = GameShop::NONE;
		isSaled = false;
		offTime();
	}
	//선택할 때
	else if(i == 1)
	{
		int trueCount = 0;
		for(auto iter = selectedItems.begin(); iter != selectedItems.end(); ++iter)
		{
			if(iter->second == true)
				trueCount++;
		}
		
		if(trueCount >= 3)
		{
			_s->setSelectedIndex(0);
			KSModalAlert("", gt("itemnumberlimit").c_str(), 1,
								  "ok", NOPARAM);
		}
		else
		{
			int excludeCount = 0;
			for(auto iter = selectedItems.begin(); iter != selectedItems.end(); ++iter)
			{
				if(iter->second == true && iter->first != "shield" && iter->first != "exp")
				{
					excludeCount++;
				}
			}
			
			if(excludeCount >= 2) // time, shield 를 제외한 아이템 개수가 2 개이상이면
			{
				_s->setSelectedIndex(0);
				KSModalAlert("", gt("itemnumberlimit").c_str(), 1,
							 "ok", NOPARAM);
			}
			else
			{
				isSaled = false;
				checks[2]->setVisible(true);
				selectedItems["item_random"] = GameShop::SEL;
				
				onTime();
			}
			
		}
		
		
	}
	refreshPriceTable();
}
void GameShop::SHIELD(CCObject* s)
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
		isSaled = false;
		offShield();
	}
	//선택할 때
	else if(i == 1)
	{
		int trueCount = 0;
		for(auto iter = selectedItems.begin(); iter != selectedItems.end(); ++iter)
		{
			if(iter->second == true)
				trueCount++;
		}
		
		if(trueCount >= 3)
		{
			_s->setSelectedIndex(0);
			KSModalAlert("", gt("itemnumberlimit").c_str(), 1,
								  "ok", NOPARAM);
		}
		else
		{
			isSaled = false;
			checks[1]->setVisible(true);
			selectedItems["shield"] = GameShop::SEL;
			onShield();
			
		}
	}
	refreshPriceTable();
}
void GameShop::EXP(CCObject* s)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	CCMenuItemToggle* _s = (CCMenuItemToggle*)s;
	desc->setString(gt("exp").c_str());
	int i = _s->getSelectedIndex();
	//선택 해지할 때
	if(i == 0)
	{
		isSaled = false;
		checks[0]->setVisible(false);
		selectedItems["exp"] = GameShop::NONE;
		offExp();
	}
	//선택할 때
	else if(i == 1)
	{
		int trueCount = 0;
		for(auto iter = selectedItems.begin(); iter != selectedItems.end(); ++iter)
		{
			if(iter->second == true)
				trueCount++;
		}
		
		if(trueCount >= 3)
		{
			_s->setSelectedIndex(0);
			KSModalAlert("", gt("itemnumberlimit").c_str(), 1,
								  "ok", NOPARAM);
		}
		else
		{
			isSaled = false;
			checks[0]->setVisible(true);
			selectedItems["exp"] = GameShop::SEL;
			onExp();
		}
	}
	refreshPriceTable();
}

void GameShop::GAMEITEM1(CCObject* s)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	CCMenuItemToggle* _s = (CCMenuItemToggle*)s;
	
	int i = _s->getSelectedIndex();
	desc->setString(getItem1Desc().c_str());
	//선택 해지할 때
	if(i == 0)
	{
		isSaled = false;
		checks[3]->setVisible(false);
		offItem1();
	}
	//선택할 때
	else if(i == 1)
	{
		int trueCount = 0;
		for(auto iter = selectedItems.begin(); iter != selectedItems.end(); ++iter)
		{
			if(iter->second == true)
				trueCount++;
		}
		
		if(trueCount >= 3)
		{
			_s->setSelectedIndex(0);
			KSModalAlert("", gt("itemnumberlimit").c_str(), 1,
								  "ok",NOPARAM);
		}
		else
		{
			if(selectedItems["item_random"] == true && selectedItems[item2Name] == true)
			{
				_s->setSelectedIndex(0);
				KSModalAlert("", gt("itemnumberlimit").c_str(), 1,
							 "ok", NOPARAM);
			}
			else
			{
				isSaled = false;
				checks[3]->setVisible(true);
				onItem1();
			}
		}
	}
	refreshPriceTable();
}
void GameShop::GAMEITEM2(CCObject* s)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	CCMenuItemToggle* _s = (CCMenuItemToggle*)s;
	desc->setString(getItem2Desc().c_str());
	int i = _s->getSelectedIndex();
	//선택 해지할 때
	if(i == 0)
	{
		isSaled = false;
		checks[4]->setVisible(false);
		offItem2();
	}
	//선택할 때
	else if(i == 1)
	{
		int trueCount = 0;
		for(auto iter = selectedItems.begin(); iter != selectedItems.end(); ++iter)
		{
			if(iter->second == true)
				trueCount++;
		}
		
		if(trueCount >= 3)
		{
			_s->setSelectedIndex(0);
			KSModalAlert("", gt("itemnumberlimit").c_str(), 1,
								  "ok", NOPARAM);
		}
		else
		{
			if(selectedItems["item_random"] == true && selectedItems[item1Name] == true)
			{
				_s->setSelectedIndex(0);
				KSModalAlert("", gt("itemnumberlimit").c_str(), 1,
							 "ok", NOPARAM);
			}
			else
			{
				isSaled = false;
				checks[4]->setVisible(true);
				onItem2();
			}
			
		}
	}
	refreshPriceTable();
}
GameShop::~GameShop()
{
	
}
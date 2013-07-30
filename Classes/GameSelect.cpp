//
//  GameSelect.cpp
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 6..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//
#include "GameSelect.h"
#include <map>
#include <string>
#include <algorithm>
#include "NSDefault.h"
#include "ACSaveData.h"
#include "AKSaveData.h"
#include "BS2SaveData.h"
#include "HWSaveData.h"
#include "Bridge.h"
#include "WCSaveData.h"
#include "KSoundEngine.h"
#include "SKSaveData.h"
#include "HGSaveData.h"
#include "KSModalAlert.h"
#include "GDTranslate.h"
using namespace GDTranslate;
using namespace std;
#define ARRAY_SIZE(x) ((sizeof x) / (sizeof *x))



ContainerElement::CATEGORY_T GameSelect::game2Category(const string& name)
{
	if(NSDefault::getIsOpenedGame(name))
	{
		return ContainerElement::OPENED;
	}
	else
	{
		return ContainerElement::LOCKED;
	}
}

GameSelect::GameSelect()
{
	currentOrder = 0;
	isMoving = false;
	
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ThisClassType::onUnlockStageWithFriends),
																  "onUnlockStageWithFriends", 0);
	
	
	
//	opendGames = opendGames;
//	lockedGames = lockedGames;
//	CCLog("opend game");
//	for(auto i : opendGames)
//	{
//		CCLog("%s", i.c_str());
//	}
//	
//	CCLog("closed game");
//	for(auto i : lockedGames)
//	{
//		CCLog("%s", i.c_str());
//	}
}

bool GameSelect::init()
{
	//
	CCLayer::init();
	this->setTouchEnabled(true);
	thiz = CCSprite::create("gameselect_back.png");
	addChild(thiz);
	
	NSDefault::setOpenGame("SK");
	NSDefault::setOpenGame("AK");
	NSDefault::setOpenGame("AC");
	NSDefault::setOpenGame("WORLDCUP");
	
	vector< ContainerElement > spriteFile;
	CCLog("%d", playInfo->skscore);
	spriteFile.push_back(ContainerElement{"SK", "start_ski.png", playInfo->skscore, playInfo->skweekly, menu_selector(ThisClassType::RECORD_SKI), game2Category("SK")});
	spriteFile.push_back(ContainerElement{"AK", "start_clay.png", playInfo->akscore, playInfo->akweekly, menu_selector(ThisClassType::RECORD_CLAY), game2Category("AK")});
	spriteFile.push_back(ContainerElement{"AC", "start_archery.png", playInfo->acscore, playInfo->acweekly, menu_selector(ThisClassType::RECORD_ARCHER), game2Category("AC")});
	spriteFile.push_back(ContainerElement{"HG", "start_hanggliding.png", playInfo->hgscore, playInfo->hgweekly,
		menu_selector(ThisClassType::RECORD_HANG), game2Category("HG")});
	spriteFile.push_back(ContainerElement{"BS", "start_basketball.png", playInfo->bsscore, playInfo->bsweekly, menu_selector(ThisClassType::RECORD_BASKETBALL), game2Category("BS")});
	spriteFile.push_back(ContainerElement{"HW", "start_hurdle.png", playInfo->hwscore, playInfo->hwweekly, menu_selector(ThisClassType::RECORD_HURDLE), game2Category("HW")});
	spriteFile.push_back(ContainerElement{"WORLDCUP", "start_comingsoon.png", playInfo->wcscore, playInfo->wcweekly, menu_selector(ThisClassType::RECORD_WORLDCUP),
		ContainerElement::WORLDCUP});
	
	opendGames.clear();
	lockedGames.clear();
	for(auto i : spriteFile)
	{
		if(i.CATEGORY == ContainerElement::OPENED || i.CATEGORY == ContainerElement::WORLDCUP)
		{
			opendGames.push_back(i.gameCode);
		}
		else if(i.CATEGORY == ContainerElement::LOCKED)
		{
			lockedGames.push_back(i.gameCode);
		}
	}
	
	if(!lockedGames.empty())
	{
		soonOpenGame = *lockedGames.begin();
		NSDefault::setSoonOpenGame(soonOpenGame); // 열릴 게임 세팅.
	}
		
	
	// 일단 순서대로 0,1,2,3,4,5... 넣음.
	for(int i=0; i<sizeof(gameOrder) / sizeof(*gameOrder); i++)
	{
		gameOrder[i].seq = i;
	}
	
	for(int spriteIndex = 0; spriteIndex<spriteFile.size(); ++spriteIndex)
	{
		gameOrder[spriteIndex].ct = spriteFile[spriteIndex].CATEGORY;
	}
	
	// 정렬
	stable_sort(&gameOrder[0], &gameOrder[sizeof(gameOrder) / sizeof(*gameOrder)]);
	
	// 섞고
	random_shuffle(&gameOrder[0], &gameOrder[opendGames.size() - 1], [=](int n){
		return this->well512.GetValue(n-1);
	} );
	
	
	CCLog("after sort");
	for(auto i : gameOrder)
	{
		CCLog("%d --", i.seq);
	}
	int lockedIndex = -1;
	for(int i=0; i<sizeof(gameOrder) / sizeof(*gameOrder); i++)
	{
		ContainerElement::CATEGORY_T ct = spriteFile[gameOrder[i].seq].CATEGORY;
		
		
		// 오픈된 게임에 대해서는 메뉴를 달음.
		if(ct == ContainerElement::OPENED || ct == ContainerElement::WORLDCUP)
		{
			string a = spriteFile[gameOrder[i].seq].titleFile;
			int maxScore = spriteFile[gameOrder[i].seq].maxScore;
			int weeklyScore = spriteFile[gameOrder[i].seq].weeklyMaxScore;
			SEL_MenuHandler handler = spriteFile[gameOrder[i].seq].recordHandler;
			
			GameTitle* temp = new GameTitle;
			gameTitles.push_back(temp);
			temp->initWithFile(a.c_str());
			temp->autorelease();
			
			
			//			CCSprite* temp = CCSprite::create(a.c_str());
			temp->setPosition(ccp(134, 144 - 144 * (i)));
			thiz->addChild(temp, 2);
			gameSprites.push_back(temp);
			CCMenu* _menu = CCMenu::create(0);
			_menu->setPosition(ccp(0, 0));
			temp->addChild(_menu, 3);
			temp->menu = _menu;
			
			CCMenuItem* record = CCMenuItemImage::create("record.png", "record.png", this, handler);
			temp->record = record;
			
			record->setPosition(ccp(222, 125)); // 222, 125
			_menu->addChild(record, 2, GameTitle::menu_record_tag);
			if((maxScore < 0 || weeklyScore < 0) == false)
			{
				CCSprite* gameMax = CCSprite::create("gameover_max.png");
				gameMax->setScale(0.5f);
				gameMax->setPosition(ccp(72, 38));
				temp->addChild(gameMax, 2);
				
				CCLabelBMFont* maxFnt = CCLabelBMFont::create(KS_Util::stringWithFormat("%d", maxScore).c_str(), "gameover_score1.fnt");
				maxFnt->setPosition(ccp(72, 20));
				temp->addChild(maxFnt, 2);
				
				CCSprite* gameWeekly = CCSprite::create("gameover_weekly.png");
				gameWeekly->setScale(0.5f);
				gameWeekly->setPosition(ccp(192, 38));
				temp->addChild(gameWeekly, 2);
				
				CCLabelBMFont* weeklyFnt = CCLabelBMFont::create(KS_Util::stringWithFormat("%d", weeklyScore).c_str(), "gameover_score1.fnt");
				weeklyFnt->setPosition(ccp(192, 20));
				temp->addChild(weeklyFnt, 2);
			}
		}
		else if(ct == ContainerElement::LOCKED)
		{
			// 닫힌 게임에 대해서...
			lockedIndex = i;
			break;
		}
	}
	
	// 닫힌 게임에 대해서 처리.
	if(lockedIndex != -1) // 닫힌 게임이 있다면
	{
		// 일단 처음에 살짝 오픈된거 보여줌.
		{
			string a = spriteFile[gameOrder[lockedIndex].seq].titleFile;
			int maxScore = spriteFile[gameOrder[lockedIndex].seq].maxScore;
			int weeklyScore = spriteFile[gameOrder[lockedIndex].seq].weeklyMaxScore;
			SEL_MenuHandler handler = spriteFile[gameOrder[lockedIndex].seq].recordHandler;
			
			
			GameTitle* temp = new GameTitle;
			gameTitles.push_back(temp);
			temp->initWithFile(a.c_str());
			temp->autorelease();
			
			temp->setColor(ccc3(100, 100, 100));
			temp->setPosition(ccp(134, 144 - 144 * (lockedIndex)));
			thiz->addChild(temp, 2);
			gameSprites.push_back(temp);
			
			CCSprite* lockScreen = CCSprite::create("start_unlock.png");
			temp->lockScreen = lockScreen;
			lockScreen->setPosition(ccp(132, 66));
			temp->addChild(lockScreen);
			
			CCMenu* _menu = CCMenu::create(0);
			temp->menu = _menu;
			_menu->setPosition(ccp(0, 0));
			temp->addChild(_menu, 3);
			//unlock_gold.png
			//unlock_invite.png
			
			CCMenuItem* sendTk = CCMenuItemImage::create("unlock_invite.png", "unlock_invite.png", this,
														 menu_selector(ThisClassType::sendTicket));
			
			sendTk->setPosition(ccp(64, 36));
			_menu->addChild(sendTk, 2, GameTitle::menu_invite);
			
			CCMenuItem* withGold = CCMenuItemImage::create("unlock_gold.png", "unlock_gold.png", this,
														   menu_selector(ThisClassType::unlockWithGold));
			withGold->setPosition(ccp(201, 36));
			_menu->addChild(withGold, 2, GameTitle::menu_with_gold);
		}
		// 그 뒤로는 완전 닫힌거 보여줌.
		lockedIndex++;
		
		for(int i=lockedIndex; i<sizeof(gameOrder) / sizeof(*gameOrder); i++)
		{
			string a = spriteFile[gameOrder[i].seq].titleFile;
			int maxScore = spriteFile[gameOrder[i].seq].maxScore;
			int weeklyScore = spriteFile[gameOrder[i].seq].weeklyMaxScore;
			SEL_MenuHandler handler = spriteFile[gameOrder[i].seq].recordHandler;
			
			
			CCSprite* temp = CCSprite::create("start_lockedgame.png");
			temp->setPosition(ccp(134, 144 - 144 * (i)));
			thiz->addChild(temp, 2);
			gameSprites.push_back(temp);
			
			CCSprite* lockScreen = CCSprite::create("start_lock.png");
			lockScreen->setPosition(ccp(132, 72));
			temp->addChild(lockScreen);
		}
	}
	return true;
}
GameSelect::~GameSelect()
{
	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}
void GameSelect::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}
bool GameSelect::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	slidedWithMove = false;
	CCTouch* touch = pTouch;

	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	location = thiz->convertToNodeSpace(location);
	CCRect ggg = CCRectMake(0, 0, thiz->getContentSize().width, thiz->getContentSize().height);
	CCRectApplyAffineTransform(ggg, thiz->nodeToWorldTransform());
	if(ggg.containsPoint( location))
	{
		beginPoint = location;
	}
	else
	{
		beginPoint = ccp(-100, -100);
	}
	return true;
}
void GameSelect::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	CCTouch* touch = pTouch;
	
	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	location = thiz->convertToNodeSpace(location);
	
	CCRect ggg = CCRectMake(0, 0, thiz->getContentSize().width, thiz->getContentSize().height);
	CCRectApplyAffineTransform(ggg, thiz->nodeToWorldTransform());
	endPoint = location;
	if(isMoving)
		return;
	if(beginPoint.y >= 0)
	{
		if(endPoint.y - beginPoint.y >= 70)
		{ // downward
			if(GAME_COUNT - 1 <= currentOrder)
				return;
			isMoving = true;
			currentOrder++;
			for(auto iter = gameSprites.begin(); iter != gameSprites.end(); ++iter)
			{
				(*iter)->runAction(CCSequence::createWithTwoActions(CCMoveBy::create(0.15f, ccp(0, 144)), CCCallFunc::create(this, callfunc_selector(GameSelect::finishMoving))));
			}
			slidedWithMove = true;
			beginPoint.y = endPoint.y;
			KSoundEngine::sharedEngine()->playSound("gameselect.mp3");
		}
		else if(beginPoint.y - endPoint.y >= 70)
		{ // upward
			if(0 >= currentOrder)
				return;
			isMoving = true;
			currentOrder--;
			for(auto iter = gameSprites.begin(); iter != gameSprites.end(); ++iter)
			{
				(*iter)->runAction(CCSequence::createWithTwoActions(CCMoveBy::create(0.15f, ccp(0, -144)), CCCallFunc::create(this, callfunc_selector(GameSelect::finishMoving))));
			}
			slidedWithMove = true;
			beginPoint.y = endPoint.y;
			KSoundEngine::sharedEngine()->playSound("gameselect.mp3");
		}
	}
}
void GameSelect::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(slidedWithMove)
		return;
	CCTouch* touch = pTouch;
	
	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	location = thiz->convertToNodeSpace(location);
	
	CCRect ggg = CCRectMake(0, 0, thiz->getContentSize().width, thiz->getContentSize().height);
	CCRectApplyAffineTransform(ggg, thiz->nodeToWorldTransform());
	endPoint = location;
	if(isMoving)
		return;
	if(beginPoint.y >= 0)
	{
		if(beginPoint.y <= 30)
		{ // downward
			if(GAME_COUNT - 1 <= currentOrder)
				return;
			isMoving = true;
			currentOrder++;
			for(auto iter = gameSprites.begin(); iter != gameSprites.end(); ++iter)
			{
				(*iter)->runAction(CCSequence::createWithTwoActions(CCMoveBy::create(0.2f, ccp(0, 144)), CCCallFunc::create(this, callfunc_selector(GameSelect::finishMoving))));
			}
			KSoundEngine::sharedEngine()->playSound("gameselect.mp3");
		}
		else if(beginPoint.y >= thiz->getContentSize().height - 50)
		{ // upward
			if(0 >= currentOrder)
				return;
			isMoving = true;
			currentOrder--;
			for(auto iter = gameSprites.begin(); iter != gameSprites.end(); ++iter)
			{
				(*iter)->runAction(CCSequence::createWithTwoActions(CCMoveBy::create(0.2f, ccp(0, -144)), CCCallFunc::create(this, callfunc_selector(GameSelect::finishMoving))));
			}
			KSoundEngine::sharedEngine()->playSound("gameselect.mp3");
		}
	}
}

void GameSelect::hideGames()
{
	for(auto iter = gameSprites.begin(); iter != gameSprites.end(); ++iter)
	{
		(*iter)->runAction(CCFadeOut::create(0.2f));
		KS::KSLog("%", (*iter)->getChildrenCount());
		CCArray* children = (*iter)->getChildren();
		if(children)
		{
			for(int i=0; i<children->count(); i++)
			{
				CCNode* o = (CCNode*)children->objectAtIndex(i);
				o->setVisible(false);
				o->runAction(CCFadeOut::create(0.2f));
			}
		}
	}
//	KS::setAllVisible(_menu, false);	
}
void GameSelect::showGames()
{
	if(getCurrentIndex() == GameSelect::COMINGSOON)
	{ // 스타트에 -gold 표시하는 코드.
		mediator->showWorldcupMark(true);
	}
	else
	{ // 스타트에 -gold 표시 해제.
		mediator->showWorldcupMark(false);
	}
	for(auto iter = gameSprites.begin(); iter != gameSprites.end(); ++iter)
	{
		(*iter)->runAction(CCFadeIn::create(0.1f));
		CCLog("%", (*iter)->getChildrenCount());
		CCArray* children = (*iter)->getChildren();
		if(children)
		{
			for(int i=0; i<children->count(); i++)
			{
				CCNode* o = (CCNode*)children->objectAtIndex(i);
				o->setVisible(true);
				o->runAction(CCFadeIn::create(0.2f));
			}
		}
	}
//	KS::setAllVisible(_menu, true);
}

void GameSelect::syncInfo()
{
	
}
void GameSelect::finishMoving()
{
	isMoving = false;
	
	if(getCurrentIndex() == GameSelect::COMINGSOON)
	{ // 스타트에 -gold 표시하는 코드.
		mediator->showWorldcupMark(true);
	}
	else
	{ // 스타트에 -gold 표시 해제.
		mediator->showWorldcupMark(false);
	}
}

void GameSelect::RECORD_HURDLE(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	KS::KSLog("%", __FUNCTION__);
	mediator->showLegendaryRecord("HW");
}
void GameSelect::RECORD_ARCHER(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	KS::KSLog("%", __FUNCTION__);
	mediator->showLegendaryRecord("AC");
}
void GameSelect::RECORD_BASKETBALL(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	KS::KSLog("%", __FUNCTION__);
	mediator->showLegendaryRecord("BS");
}
void GameSelect::RECORD_CLAY(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	KS::KSLog("%", __FUNCTION__);
	mediator->showLegendaryRecord("AK");
}

void GameSelect::RECORD_WORLDCUP(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	KS::KSLog("%", __FUNCTION__);
	mediator->showLegendaryRecord("WORLDCUP");
}
void GameSelect::RECORD_SKI(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	KS::KSLog("%", __FUNCTION__);
	mediator->showLegendaryRecord("SK");
}
void GameSelect::RECORD_HANG(CCObject*)
{
	KSoundEngine::sharedEngine()->playSound("select.mp3");
	KS::KSLog("%", __FUNCTION__);
	mediator->showLegendaryRecord("HG");
}


void GameSelect::sendTicket(CCObject*)
{
	CCNotificationCenter::sharedNotificationCenter()->postNotification("onRequestTicket");
}
void GameSelect::unlockWithGold(CCObject*)
{
	KSModalAlert("", gt("openstagewithgold").c_str(), 2,
				 "NO", NOPARAM,
				 "YES", this, callfunc_selector(ThisClassType::UNLOCKSTAGE));
}

void GameSelect::onUnlockStageWithFriends(CCObject*)
{
	NSDefault::setOpenGame(soonOpenGame);
	gameTitles[currentOrder]->menu->removeFromParentAndCleanup(true);
	gameTitles[currentOrder]->lockScreen->removeFromParentAndCleanup(true);
	gameTitles[currentOrder]->setColor(ccc3(255, 255, 255));
	playInfo->justOpen = true;
	
	CCNotificationCenter::sharedNotificationCenter()->postNotification("openNewGameEffect");
}
void GameSelect::UNLOCKSTAGE()
{
	if(NSDefault::getGold() >= 5000)
	{
		
		NSDefault::setGold(NSDefault::getGold() - 5000);
		CCNotificationCenter::sharedNotificationCenter()->postNotification("refreshGold");
		CCLog("game %s", soonOpenGame.c_str());
		NSDefault::setOpenGame(soonOpenGame);
		gameTitles[currentOrder]->menu->removeFromParentAndCleanup(true);
		gameTitles[currentOrder]->lockScreen->removeFromParentAndCleanup(true);
		gameTitles[currentOrder]->setColor(ccc3(255, 255, 255));
		playInfo->justOpen = true;
		KS::KSLog("% %d", __FUNCTION__, __LINE__);
		CCNotificationCenter::sharedNotificationCenter()->postNotification("openNewGameEffect");
	}
	else
	{
		KSModalAlert("", gt("require_coin").c_str(), 1,
					 "OK", NOPARAM);
	}
	
	
}
void GameSelect::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
}
void GameSelect::onExit()
{
	
	
	CCLayer::onExit();
}

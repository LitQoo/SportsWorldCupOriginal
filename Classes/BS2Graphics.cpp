//
//  Graphics.cpp
//  BasketWorldCup2
//
//  Created by ksoo k on 13. 1. 14..
//
//

#include "BS2Graphics.h"
#include "BS2GameScreen.h"
#include "KS_Util.h"
#include "GameSystem.h"
#include "Bridge.h"
#include "BS2BasketGame.h"
#include "BS2Play.h"
#include "Bridge.h"

void Graphics::preCreateMiniRank()
{
	
}

void Graphics::initBatchNode()
{
	item_tracer = CCSpriteBatchNode::create("tracer_item.png");
	gs->addChild(item_tracer, zorder::ITEM_TRACER);
	for(int i=0; i<TRACER_NUMBER; i++)
	{
		CCSprite* a = CCSprite::create("tracer.png");
		//a->setScale(1.f * 12.f / 18.f);
		gs->addChild(a, zorder::TRACER);
		a->setVisible(false);
		push_backTracer(a);
	}
//	comboBatchNode = CCSpriteBatchNode::create("combo.png");
//	gs->addChild(comboBatchNode, 15);
	
	
	
	cleanShotBatchNode = CCSpriteBatchNode::create("clean.png");
	
	gs->addChild(cleanShotBatchNode, zorder::CLEAN_SHOT);
	
	luckyBatchNode = CCSpriteBatchNode::create("lucky.png");
	gs->addChild(luckyBatchNode, zorder::LUCKY);
	
	coinBatchNode = CCSpriteBatchNode::create("coin.png");
	gs->addChild(coinBatchNode, zorder::COIN);
	
	goalFrontBatchNode = CCSpriteBatchNode::create("goalfront.png");
	goalFrontBatchNode->getTexture()->setAntiAliasTexParameters();
	gs->addChild(goalFrontBatchNode, zorder::FRONT_POST);
	// 40x40
	goalFront = CCSprite::createWithTexture(goalFrontBatchNode->getTexture(),
													   CCRectMake(Graphics::GOALFRONT_WIDTH*0,0,Graphics::GOALFRONT_WIDTH, Graphics::GOALFRONT_HEIGHT));
	goalFront->setAnchorPoint(ccp(0.5, 1));
}

void Graphics::hideTracer()
{
	// 딜레이 후 점점 사라짐.
	float _s = 0.f;
	for(int i=0; i<TRACER_NUMBER; i++, _s += 0.03f)
	{
		CCSprite* a = CCSprite::create("tracer.png");
		//a->setScale(1.f * 12.f / 18.f);
		
		if(tracer[i]->isVisible() == false)
			break;
		gs->addChild(a, zorder::TRACER);
		a->setPosition(tracer[i]->getPosition());
		auto action1 = CCDelayTime::create(_s);
		auto action2 = CCFadeOut::create(0.3f);
		auto action3 = CCCallFuncN::create(gs,  callfuncN_selector(KSBaseScene::deleteSprite));
		auto totalAction = CCSequence::create(action1, action2, action3, 0);
		a->runAction(totalAction);
	}
	
	for_each(tracer.begin(), tracer.end(), [](CCSprite* spr)
			 {
				 spr->setVisible(false);
			 });
	
	for(int i=0; i<item_tracer->getChildrenCount(); i++, _s += 0.03f)
	{
		CCSprite* _spr = (CCSprite*)item_tracer->getChildren()->objectAtIndex(i);
		auto action1 = CCDelayTime::create(_s);
		auto action2 = CCFadeOut::create(0.3f);
		auto action3 = CCCallFuncN::create(gs,  callfuncN_selector(KSBaseScene::deleteSprite));
		auto totalAction = CCSequence::create(action1, action2, action3, 0);
		_spr->runAction(totalAction);
	}
}
void Graphics::initGraphics()
{
	zeroSprite = CCProgressTimer::create(CCSprite::create("back0.png"));
	zeroSprite->setScale(SCREEN_WIDTH / 480.f * 2.f);
	zeroSprite->setAnchorPoint(ccp(0, 0));
	zeroSprite->setPosition(ccp(0, 0));
	zeroSprite->setType(CCProgressTimerType::kCCProgressTimerTypeBar);
	zeroSprite->setPercentage(0);
	gs->addChild(zeroSprite);
	
	CCPoint comboPosition = ccp(SCREEN_WIDTH/2.f, 120 - 40);
	CCSprite* goalMove = CCSprite::create("goal_move.png");
	gs->addChild(goalMove, zorder::POST_MOVE_BAR);
	goalMove->setAnchorPoint(ccp(0, 0.5));
	goalMove->setPosition(ccp(0, SCREEN_HEIGHT/2.f));
}
void Graphics::goalInEffect(int score)
{
	CCAnimation* goalAnimation = CCAnimation::create();
	goalAnimation->setDelayPerUnit(0.07f);
	goalAnimation->addSpriteFrameWithTexture(goalFrontBatchNode->getTexture(), CCRectMake(GOALFRONT_WIDTH*0, 0, GOALFRONT_WIDTH, GOALFRONT_HEIGHT));
	goalAnimation->addSpriteFrameWithTexture(goalFrontBatchNode->getTexture(), CCRectMake(GOALFRONT_WIDTH*1, 0, GOALFRONT_WIDTH, GOALFRONT_HEIGHT));
	goalAnimation->addSpriteFrameWithTexture(goalFrontBatchNode->getTexture(), CCRectMake(GOALFRONT_WIDTH*2, 0, GOALFRONT_WIDTH, GOALFRONT_HEIGHT));
	goalAnimation->addSpriteFrameWithTexture(goalFrontBatchNode->getTexture(), CCRectMake(GOALFRONT_WIDTH*3, 0, GOALFRONT_WIDTH, GOALFRONT_HEIGHT));
	goalAnimation->addSpriteFrameWithTexture(goalFrontBatchNode->getTexture(), CCRectMake(GOALFRONT_WIDTH*4, 0, GOALFRONT_WIDTH, GOALFRONT_HEIGHT));
	goalAnimation->addSpriteFrameWithTexture(goalFrontBatchNode->getTexture(), CCRectMake(GOALFRONT_WIDTH*5, 0, GOALFRONT_WIDTH, GOALFRONT_HEIGHT));
	goalFront->runAction(CCAnimate::create(goalAnimation));
	return;
}

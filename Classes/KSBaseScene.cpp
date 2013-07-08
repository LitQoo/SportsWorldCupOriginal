//
//  KSBaseScene.cpp
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 12. 3..
//  Copyright (c) 2012년 LitQoo Inc. All rights reserved.
//
#include "KSBaseScene.h"


bool KSBaseScene::init()
{
	CCLayerColor::initWithColor(ccc4(0, 0, 0, 255));
	return true;
}
pair<CCActionInterval*, CCSprite*> KSBaseScene::playAnimation(const char* spriteName, float delay, int cols, int frames, int width_per_each, int height_per_each, bool isInfinite)
{
	CCAnimation* Animation = CCAnimation::create();
	CCSprite* sprite = CCSprite::create(spriteName);
	Animation->setDelayPerUnit(delay);
	for(int f = 1; f<=frames; f++) // 프레임 개수만큼.
	{
		int col = (f - 1 ) % cols;
		int row = (f - 1 ) / cols;
		Animation->addSpriteFrameWithTexture(sprite->getTexture(), CCRectMake(width_per_each*col, height_per_each * row, width_per_each, height_per_each));
		
	}
	
	CCActionInterval* ac;
	if(isInfinite)
	{
		ac = CCRepeatForever::create(CCAnimate::create(Animation));
	}
	else
	{
		ac = CCAnimate::create(Animation);
	}
	
	CCSprite* ret = CCSprite::createWithTexture(sprite->getTexture(), CCRectMake(0, 0, width_per_each, height_per_each));
	return pair<CCActionInterval*, CCSprite*>(ac, ret);
}
pair<CCActionInterval*, CCSprite*> KSBaseScene::playAnimation(CCSpriteBatchNode* batchNode, float delay, int cols, int frames, int width_per_each, int height_per_each, bool isInfinite)
{
	CCSprite* Sprite = CCSprite::createWithTexture(batchNode->getTexture(), CCRectMake(0, 0, width_per_each, height_per_each));
	CCAnimation* Animation = CCAnimation::create();
	Animation->setDelayPerUnit(delay);
	
	for(int f = 1; f<=frames; f++) // 프레임 개수만큼.
	{
		int col = (f - 1 ) % cols;
		int row = (f - 1 ) / cols;
		Animation->addSpriteFrameWithTexture(batchNode->getTexture(), CCRectMake(width_per_each*col, height_per_each * row, width_per_each, height_per_each));
	}

	CCActionInterval* ac;
	if(isInfinite)
	{
		ac = CCRepeatForever::create(CCAnimate::create(Animation));
	}
	else
	{
		ac = CCAnimate::create(Animation);
	}
	
	batchNode->addChild(Sprite);
	return pair<CCActionInterval*, CCSprite*>(ac, Sprite);

}
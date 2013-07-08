//
//  HGCoin.h
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 6. 19..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCupOriginal_HGCoin_cpp
#define SportsWorldCupOriginal_HGCoin_cpp



#include "cocos2d.h"
#include "AdvanceSprite.h"
USING_NS_CC;


class HGCoin : public AdvanceSprite
{
private:
	CCAnimation* selfAnimation;
	
public:
	
	HGCoin();
	virtual ~HGCoin();
	
	virtual bool init();
	
	CREATE_FUNC(HGCoin);
	void createAnimation(CCAnimation* animation, std::string imgFile, int cols, int frames, int width_per_each, int height_per_each)
	{
		///	adHuman->add
		CCSprite* sprite = CCSprite::create(imgFile.c_str());
		for(int f = 1; f<=frames; f++) // 프레임 개수만큼.
		{
			int col = (f - 1 ) % cols;
			int row = (f - 1 ) / cols;
			animation->addSpriteFrameWithTexture(sprite->getTexture(), CCRectMake(width_per_each*col, height_per_each * row, width_per_each, height_per_each));
		}
	}
	CCRect createAnimation(CCAnimation* animation, std::string imgFile)
	{
		///	adHuman->add
		CCSprite* sprite = CCSprite::create(imgFile.c_str());
		animation->addSpriteFrameWithTexture(sprite->getTexture(), sprite->getTextureRect());
		return sprite->getTextureRect();
	}
	
	
	CC_SYNTHESIZE(float, posX, PosX);
	CC_SYNTHESIZE(float, posY, PosY);
	CC_SYNTHESIZE(bool, isEat, IsEat);
};

#endif


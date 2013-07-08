//
//  MiddleObs.h
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 6. 18..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCupOriginal_MiddleObs_cpp
#define SportsWorldCupOriginal_MiddleObs_cpp

#include "cocos2d.h"
USING_NS_CC;
#include "AdvanceSprite.h"

class HGMiddleObs : public AdvanceSprite
{
private:
	CCAnimation* selfAnimation;
	

public:
	HGMiddleObs();
	virtual ~HGMiddleObs();
	virtual bool init();
	bool initWithStoneIndex(int );
	CREATE_FUNC(HGMiddleObs);
	static HGMiddleObs* createWithStoneIndex(int i) \
	{ \
		HGMiddleObs *pRet = new HGMiddleObs(); \
		if (pRet && pRet->initWithStoneIndex(i)) \
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
//	CC_SYNTHESIZE(bool, scoring, Scoring);
	CC_SYNTHESIZE(float, width, Width);
	CC_SYNTHESIZE(CCSprite*, obsSprite, ObsSprite);
	CC_SYNTHESIZE(bool, isAttack, IsAttack); // 얘가 공격을 했는지 여부.
	CC_SYNTHESIZE(bool, isChecked, IsChecked);
};

#endif

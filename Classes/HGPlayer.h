//
//  HGPlayer.h
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 6. 17..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCupOriginal_HGPlayer_cpp
#define SportsWorldCupOriginal_HGPlayer_cpp

#include "cocos2d.h"
#include "AdvanceSprite.h"

USING_NS_CC;


class HGPlayer : public AdvanceSprite
{
private:
	CCAnimation* goAnimation;
	
public:
	static float UNBEATABLE_TIME;
	enum {NORMAL, UNBEATABLE} STATE;
	void updateState(float dt){
		if(STATE == UNBEATABLE)
		{
			unbeatableRemainTime -= dt;
			if(unbeatableRemainTime <= 0)
			{
				STATE = NORMAL;
				setColor(ccc3(255, 255, 255));
			}
		}
	}
	void damaged()
	{
		STATE = UNBEATABLE;
		unbeatableRemainTime = UNBEATABLE_TIME;
		runAction(CCBlink::create(1.2, 7));
//							 CCSequence::create(
//												CCBlink::create(1.2, 7),
//												KSFunc({ STATE &= ~SUPER;} ),
//												0));
//
//		setColor(ccc3(255, 0, 0));
	}
	HGPlayer();
	virtual ~HGPlayer();
	virtual bool init();
	void startScheduler()
	{
		schedule(schedule_selector(HGPlayer::updateState));
	}
	CREATE_FUNC(HGPlayer);
	
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
	
	CC_SYNTHESIZE(float, posX, PosX);
	CC_SYNTHESIZE(float, posY, PosY);
	CC_SYNTHESIZE(float, unbeatableRemainTime, UnbeatableRemainTime);
};

#endif

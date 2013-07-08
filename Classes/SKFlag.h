//
//  SKFlag.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 5. 21..
//
//

#ifndef __SportsWorldCup__SKFlag__
#define __SportsWorldCup__SKFlag__
#include "SKObstacle.h"
#include "cocos2d.h"
USING_NS_CC;
#include "AdvanceSprite.h"
// 만들 때 obsAnimation new 하고 파괴될 때 파괴함.
class SKFlag : public SKObject
{
private:
	static int cycle;
public:
	bool right;
	static const float JUDGE_HEIGHT;
	SKFlag()
	{
		
		right = false;
		isJudged = false;

		obsSize = 0;
		isFirstSetD = isFirstSetX = true;
	}
	virtual ~SKFlag()
	{
	}
	virtual void createSprite();
	static const int SIZE = 1; //@ 30
};


#endif /* defined(__SportsWorldCup__SKFlag__) */

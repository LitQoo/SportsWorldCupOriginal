//
//  KSBaseScene.h
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 12. 3..
//  Copyright (c) 2012년 LitQoo Inc. All rights reserved.
//

#ifndef BasketWorldCup2_KSBaseScene_cpp
#define BasketWorldCup2_KSBaseScene_cpp

#include "cocos2d.h"
#include <algorithm>
using namespace std;
USING_NS_CC;

#define SCENE_FUNC(_A) static CCScene* scene() { CCScene* scene = CCScene::create(); _A * layer = _A::create(); scene->addChild(layer); return scene; }
class KSBaseScene : public CCLayerColor
{
public:
	KSBaseScene(){}
	virtual ~KSBaseScene(){}
	virtual bool init();
	// 배치노드, 딜레이 기간, 가로개수, 프레임수, 가로크기, 세로 크기, 무한반복여부, 리턴값은 스프라이트포인터
	pair<CCActionInterval*, CCSprite*> playAnimation(CCSpriteBatchNode* batchNode, float delay, int cols, int frames, int width_per_each, int height_per_each, bool isInfinite);
	pair<CCActionInterval*, CCSprite*> playAnimation(const char* spriteName, float delay, int cols, int frames, int width_per_each, int height_per_each, bool isInfinite);
	void deleteSprite(CCNode* _n)
	{
		_n->getParent()->removeChild(_n, true);
	}
	CREATE_FUNC(KSBaseScene);
	SCENE_FUNC(KSBaseScene);
//	LAYER_NODE_FUNC(KSBaseScene);
};
#endif

//
//  Scrollbar.cpp
//  ExTableView
//
//  Created by 이정우 on 13. 6. 19..
//
//

#include "ScrollBar.h"

#define THIS ScrollBar
#define barFixSize 10.0f
#define barMinSize 20.0f

THIS::THIS () {
	
	dynamicScrollSize = true;
}

THIS::~THIS() {
	//    if (bar) {
	//        bar = NULL;
	//    }
}
bool THIS::init(CCScrollView* sv, float offset, const std::string& h, const std::string& v)
{
	scrollView = sv;
	CCLayerColor::initWithColor(ccc4(0, 255, 0, 0));
	
	CCScrollViewDirection type = sv->getDirection();
	switch(type)
	{
		case kCCScrollViewDirectionHorizontal:
			barH = CCScale9Sprite::create(h.c_str());
			barH->setAnchorPoint(ccp(0.5, 0.f));
			barH->setPosition(ccp(0, offset));
			addChild(barH);
			sv->cocos2d::CCLayer::addChild(this);
			setBarRefresh();
			break;
		case kCCScrollViewDirectionVertical:
			barV = CCScale9Sprite::create(v.c_str());
			barV->setAnchorPoint(ccp(1.0, 0.5f));
			barV->setPosition(ccp(0, offset));
			addChild(barV);
			sv->cocos2d::CCLayer::addChild(this);
			setBarRefresh();
			break;
		case kCCScrollViewDirectionBoth:
			barH = CCScale9Sprite::create(h.c_str());
			barH->setAnchorPoint(ccp(0.5, 0.f));
			barH->setPosition(ccp(0, offset));
			addChild(barH);
			setBarRefreshH();
			
			barV = CCScale9Sprite::create(v.c_str());
			barV->setAnchorPoint(ccp(1.0, 0.5f));
			barV->setPosition(ccp(0, offset));
			addChild(barV);
			setBarRefreshV();
			
			sv->cocos2d::CCLayer::addChild(this);
			break;
	}
	return true;
}
THIS* THIS::creatScrollbar(CCScrollView* sv, float offset, const std::string& h,
						   const std::string& v)
{
    THIS* obj = new THIS();
	
    if(obj->init(sv, offset, h, v)) {
        obj->autorelease();
        return obj;
    } else {
        delete obj;
        return NULL;
    }
}

void THIS::setBarRefreshH()
{
	CCScrollView* sv = scrollView;
	CCSize viewSize = sv->getViewSize();
	float total = sv->getContainer()->getContentSize().width - viewSize.width;
	float scrollBarSize = viewSize.width * viewSize.width / total;
	
	if(dynamicScrollSize)
		barH->setContentSize(CCSizeMake(scrollBarSize, barH->getContentSize().height));
	else
		barH->setContentSize(barH->getOriginalSize());
	CCSize barContentSize = barH->getContentSize();//, at);
	
	float offset = -sv->getContentOffset().x;
	float offsetY = -sv->getContentOffset().y;
	float percent = offset / total;
	percent = MIN(1, percent);
	percent = MAX(0, percent);
	percent = percent * (viewSize.width - barContentSize.width) / viewSize.width;
	float position = (percent * viewSize.width) + barContentSize.width / 2.f;
	barH->setPosition(ccp(offset + position, offsetY));
}
void THIS::setBarRefreshV()
{
	CCScrollView* sv = scrollView;
	CCSize viewSize = sv->getViewSize();
	float total = sv->getContainer()->getContentSize().height - viewSize.height;
	float scrollBarSize = viewSize.height * viewSize.height / total;
	if(dynamicScrollSize)
		barV->setContentSize(CCSizeMake(barV->getContentSize().width, scrollBarSize));
	else
		barV->setContentSize(barV->getOriginalSize());
	CCSize barContentSize = barV->getContentSize();//, at);
	
	float offset = -sv->getContentOffset().y;
	float offsetX = -sv->getContentOffset().x;
	float percent = offset / total;
	percent = MIN(1, percent);
	percent = MAX(0, percent);
	percent = percent * (viewSize.height - barContentSize.height) / viewSize.height;
	float position = ((percent * viewSize.height) + barContentSize.height / 2.f);
	barV->setPosition(ccp(offsetX + viewSize.width, offset + position));
}
//주기적으로 호출 되면서 내용을 갱신할 함수 -> scrollViewDidScroll에서 호출됨.
void THIS::setBarRefresh() {
	CCScrollView* sv = scrollView;
	if(sv->getDirection() == kCCScrollViewDirectionHorizontal)
	{
		setBarRefreshH();
	}
	else if(sv->getDirection() == kCCScrollViewDirectionVertical)
	{
		setBarRefreshV();
	}
	else if(sv->getDirection() == kCCScrollViewDirectionBoth)
	{
		setBarRefreshV();
		setBarRefreshH();
	}
	//	position = MIN(0.f, position);
	
	
	//	bar->setPosition(ccp(offset, 0));
	//	CCLog("%f / %f .. %f", offset, total, bar->getPosition().x);
	
	
}





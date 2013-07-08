//
//  Scrollbar.h
//  ExTableView
//
//  Created by 이정우 on 13. 6. 19..
//
//

#ifndef __ExTableView__Scrollbar__
#define __ExTableView__Scrollbar__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
USING_NS_CC;
USING_NS_CC_EXT;


class ScrollBar : public CCLayerColor {
public:
    //자신의 크기를 정하고, 바를 생성
    ScrollBar();
    virtual ~ScrollBar();
    static ScrollBar* creatScrollbar(CCScrollView* table, float offset, const std::string& h = "barH.png",
									 const std::string& v = "barV.png");
    bool init(CCScrollView* sv, float offset, const std::string& h, const std::string& v);
    //주기적으로 호출 되면서 내용을 갱신할 함수
    void setBarRefresh();
protected:
	void setBarRefreshH();
	void setBarRefreshV();
	bool dynamicScrollSize; // 스크롤 크기가 컨텐츠 사이즈에 따라 바뀔지 결정하는 함수.
public:
	bool getDynamicScrollSize(){return dynamicScrollSize;}
	void setDynamicScrollSize(bool b)
	{
		dynamicScrollSize = b;
		setBarRefresh();
	}
	
	CC_SYNTHESIZE(CCScale9Sprite*, barV, BarV);
	CC_SYNTHESIZE(CCScale9Sprite*, barH, BarH);
	CC_SYNTHESIZE(CCScrollView*, scrollView, ScrollView);
};

#endif /* defined(__ExTableView__Scrollbar__) */

//
//  LQButton.cpp
//  ArcherWorldCupX
//
//  Created by Kim HyunSoo on 12. 8. 8..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "ACLQButton.h"


bool LQButton::init(){
	if(!LQLabel::init()){
		return false;
	}
    
	this->setTouchEnabled(true);
    target=NULL;
    selector=NULL;
    return false;
}


void LQButton::setTargetAndSelector(CCObject* _target, SEL_MenuHandler _selector){
    this->target = _target;
    this->selector = _selector;
    
}
void LQButton::registerWithTouchDispatcher()
{
	
	//CCLayer::registerWithTouchDispatcher();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}



bool LQButton::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){

	return true;
}

void LQButton::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	
}
void  LQButton::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	

}
void  LQButton::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCTouch* touch = pTouch;
    CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
    location = this->convertToNodeSpace(location);
    

    
    if(backRect->boundingBox().containsPoint(location) && this->isVisible())
    {
        int maxDepth = 10;
        bool isParentVisible=true;
        CCNode *tempparent = this;
        while (maxDepth-- && isParentVisible) {
            if(!tempparent->isVisible())isParentVisible=false;
            tempparent=tempparent->getParent();
            if (tempparent==NULL)break;
        }
        
        if(target!=NULL && isParentVisible){
         (target->*selector)(this);   
        }
    }

}            


LQButton::~LQButton(){
    
}
#include "KSModalAlert.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
const char* defaultFonta = "";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
const char* defaultFonta = "AppleGothic";
#endif
int KSCoverLayer::touchPriority = INT_MIN + 1000;
USING_NS_CC;
bool KSCoverLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLog("beganCover");
	return true; // swallow;
}
void KSModalAlert(char const* title,	char const *message, int nargs, ...)
{
	CCNode* runningScene = CCDirector::sharedDirector()->getRunningScene();
	
	CCSize screenSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize();
	
	KSCoverLayer* coverLayer = KSCoverLayer::create();
//	coverLayer->ignoreAnchorPointForPosition(false)
//	coverLayer->setAnchorPoint(ccp(0.5f, 0.5f));
	int zOrder = INT_MAX;

//	coverLayer->setPosition(ccp(240, 160));
	runningScene->addChild(coverLayer, zOrder);

	float fontSize = 20;
	int fontWidth = 240;

	CCLabelTTF* titleText = CCLabelTTF::create(title, defaultFonta, fontSize);

	titleText->setScale(1);
	titleText->setColor(ccc3(255,255,255));
	CCSize dimension = CCSizeMake(300, 0);
	CCLabelTTF* contentText = CCLabelTTF::create(message, defaultFonta, fontSize,  dimension, kCCTextAlignmentCenter);
	contentText->setColor(ccc3(255, 255, 255));

	
	float btnHeight;
	{ // get btnHeight;
		CCSprite* button1 = new CCSprite();
		button1->init();
		button1->initWithFile("dialogbutton.png");
		btnHeight = button1->boundingBox().size.height;
		button1->release();
	}

	int margin = 30;

	CCSprite* newWindow = CCSprite::create("alert.png");
//	newWindow->init();
//	newWindow->setTextureRect(CCRectMake(0, 0, contentText->boundingBox().size.width + margin*2,
//		margin + titleText->boundingBox().size.height + margin + contentText->boundingBox().size.height + 
//		margin + btnHeight + margin));
//	newWindow->setColor(ccc3(255, 255, 255));

//	CCSprite* backNewWindow = new CCSprite();
//	backNewWindow->init();
//	int borderSize = 4;
//	backNewWindow->setTextureRect(CCRectMake(0, 0, contentText->boundingBox().size.width + margin*2 + borderSize, 
//		margin + titleText->boundingBox().size.height + margin + contentText->boundingBox().size.height + 
//		margin + btnHeight + margin + borderSize));
//	backNewWindow->setColor(ccc3(0, 0, 0));

//	coverLayer->addChild(backNewWindow, INT_MAX);
//	backNewWindow->release();
//	backNewWindow->setPosition(ccp(screenSize.width / 2, screenSize.height / 2));
	coverLayer->addChild(newWindow, zOrder);

//	newWindow->release();
	
	newWindow->setPosition(ccp(screenSize.width / 2, screenSize.height / 2));
	newWindow->addChild(titleText);
	newWindow->addChild(contentText);

	titleText->setAnchorPoint(ccp(0.5, 1));
	contentText->setAnchorPoint(ccp(0.5, 1));

	titleText->setPosition(ccp(newWindow->boundingBox().size.width / 2., newWindow->boundingBox().size.height - margin));
	contentText->setPosition(ccp(newWindow->boundingBox().size.width / 2., newWindow->boundingBox().size.height - titleText->boundingBox().size.height - margin*2));
	
//	KS::KSLog("button P = %", KSCoverLayer::touchPriority - 1);
//	PriorityMenu* menus = PriorityMenu::menuWithItems( (KSCoverLayer::touchPriority - 1), 0);
	CCMenu* menus = CCMenu::create(0);
	menus->setTouchPriority(KSCoverLayer::touchPriority - 1);
	//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(menus, -301, true);
	menus->setAnchorPoint(ccp(0.5, 0));
	
	
	menus->setPosition(ccp( newWindow->getTextureRect().getMidX(),
		newWindow->getTextureRect().getMinY() + margin ));
	
	CCArray* menuItems = new CCArray();
	menuItems->init();
	
	va_list argptr; 
	va_start(argptr, nargs);        /* 가변 인자 처리 시작 */
	for (int i = 0; i < nargs; i++) // menuItems 에 아이템을 추가
	{
		char const* opt = va_arg(argptr, char const*);
		CCObject* target = va_arg(argptr, CCObject*);
		SEL_CallFunc selector = va_arg(argptr, SEL_CallFunc);
		// nargs 는 버튼의 개수.
		CCSprite* button1 = CCSprite::create("dialogbutton.png");
		CCSprite* button2 = CCSprite::create("dialogbutton.png");

		{
			CCLabelTTF* buttonLabel = CCLabelTTF::create(opt, defaultFonta, fontSize);
			//buttonLabel->setScale(1);

			buttonLabel->setPosition(ccp( button1->getTextureRect().getMidX(),
				button1->getTextureRect().getMidY() ));
			buttonLabel->setColor(ccc3(255, 255, 255));
			button1->addChild(buttonLabel, zOrder);
		}
		{
			CCLabelTTF* buttonLabel = CCLabelTTF::create(opt, defaultFonta, fontSize);
			buttonLabel->setColor(ccc3(255, 255, 255));
			//buttonLabel->setScale(1);
			buttonLabel->setPosition(ccp( button2->getTextureRect().getMidX(),
				button2->getTextureRect().getMidY() ));
			button2->addChild(buttonLabel, zOrder);
		}

		KSCallBack* kcb = KSCallBack::callBackWithOption(coverLayer, target, selector);
		CCMenuItemSprite* menu = CCMenuItemSprite::create(button1, button2, kcb, menu_selector(KSCallBack::Execute));
		menu->setScale(1);
		menuItems->addObject(menu);	
	}

	////////////////////////////////////////////////////////////////////////////////////////
	CCObject* iter = NULL;
	CCARRAY_FOREACH(menuItems, iter)
	{
		menus->addChild((CCMenuItem*)iter, 1);
	}
//	for(auto iter = menuItems->begin(); iter != menuItems->end(); ++iter)
//	{
//		menus->addChild(*iter, 1);
//	}

	menus->alignItemsHorizontally();
	newWindow->addChild(menus, zOrder);
	//coverLayer->addChild(menus, INT_MAX);

	menuItems->release();
	//menus->release();
	va_end(argptr);                 /* 가변 인자 처리 끝 */


	/////////////////////////////////////

}

bool KSCoverLayer::init()
{
	if ( !this->CCLayerColor::initWithColor(cocos2d::ccc4(0, 0, 0, 0)))
		return false;
	
	CCSprite* b = CCSprite::create();
	b->setTextureRect(CCRectMake(0, 0, 600, 320));
	b->setColor(ccc3(0, 0, 0));
//	b->setOpacity(150);
	b->setOpacity(0);
	
	b->runAction(CCFadeTo::create(0.5f, 150));
//	b->setContentSize(CCSizeMake(600, 320));
	b->setPosition(ccp(240, 160));
	b->setAnchorPoint(ccp(0.5f, 0.5f));
	addChild(b,1);
//	this->ignoreAnchorPointForPosition(true);
	setTouchEnabled(true);
	
	return true;
}


void KSCallBack::Execute(CCObject *menu_item)
{
	//CCLog("Excute");
	CC_UNUSED_PARAM(menu_item);
	
	if(selectorTarget && selector)
		(selectorTarget->*selector)();
	CCNode* par = this->getParent();//->getParent(); // cover ?? ???? scene ?? a??
	par->removeFromParentAndCleanup(true);
}

















#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "cocos2d.h"
#include "KSoundEngine.h"
#include "SWIntro.h"

#include "GameSystem.h"
#include "GraphDog.h"
#include "SaveData.h"
#include "NSDefault.h"
#include "StartOption.h"
#include "UIMediator.h"
#include "LoadingScene.h"
#include "SKGameScene.h"
#include "SKPlay.h"
#include "HGGameScene.h"
#include "Bridge.h"
#include "LuaManager.h"
USING_NS_CC;
#include "ProbSelector.h"
#include "CCLuaEngine.h"
#include "GDSaveData.h"

typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];
} Resource;
static Resource resource1 = { cocos2d::CCSizeMake(480, 320),   "resources-iphone" };
static Resource resource2 = { cocos2d::CCSizeMake(960, 640),   "resources-iphonehd" };
static Resource resource3 = { cocos2d::CCSizeMake(1024, 768),  "resources-ipad"   };
static Resource resource4 = { cocos2d::CCSizeMake(2048, 1536), "resources-ipadhd" };

static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(480, 320);




AppDelegate::AppDelegate()
{
//	LuaManager::shared()->loadZip("sc.zip");
}

AppDelegate::~AppDelegate() 
{
}


bool AppDelegate::applicationDidFinishLaunching() {
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    pDirector->setOpenGLView(pEGLView);
	
    // Set the design resolution
    pEGLView->setDesignResolutionSize(designResolutionSize.width,
									  designResolutionSize.height,
									  kResolutionNoBorder);
	
	
	
	
    // --------------------------------------------------------------------------------------
    CCSize frameSize = pEGLView->getFrameSize();
	pDirector->setContentScaleFactor(2.f); // 640 / 320.f
//	pDirector->setDisplayStats(true);
	CCLog("frameSize = %f %f", frameSize.width, frameSize.height);
//	pDirector->setContentScaleFactor(2.0f);// * (frameSize.width / frameSize.height) * (480.f / 320.f));
	CCFileUtils::sharedFileUtils()->addSearchPath("res_game");
//	pDirector->setBackground("main_back.png");
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
	
	saveData->createJSON();
	gdSaveData->createJSON();
	// 업데이트한 유저에 대해서는 설치날짜를 안잡음. 0임.
	if(saveData->getValue("SU", "") == "")
	{
		if(NSDefault::getInstallTime() == 0)
		{
			NSDefault::setInstalltime(playInfo->getCurrentTime_s());
		}
	}
	
	if(NSDefault::getInstallTime() < 1374713847) // 13-7-25 일 정도
	{
		NSDefault::setOpenGame("AK");
		NSDefault::setOpenGame("AC");
		NSDefault::setOpenGame("BS");
//		NSDefault::setOpenGame("HG");
		NSDefault::setOpenGame("HW");
		NSDefault::setOpenGame("SK");
	}
	
	CCLog("%d", playInfo->getCurrentTime_s());
//	if(NSDefault::getInstallTime() < )
	
	

	
	if(NSDefault::getUserName() == "")
	{
		CCScene *pScene = StartOption::scene();
		pDirector->runWithScene(pScene);
	}
	else
	{
		CCScene *pScene = LoadingScene<SWIntro, decltype(INIT_MODE::STARTUP)>::scene(INIT_MODE::STARTUP);
		pDirector->runWithScene(pScene);
	}
	
	// run
	KS::KSLog("--------------------------------");
	KSoundEngine::sharedEngine()->setBGM(NSDefault::getBGM());
	KSoundEngine::sharedEngine()->setEffect(NSDefault::getEffectSound());
	
	
	
	KSoundEngine::sharedEngine()->registerSoundOnEffect("scoring1.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("scoring2.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("select.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("bounce.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("start.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("throw.mp3");
	
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_applyitem.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_backboard01.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_crowd01.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_endzero.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_levelup.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_rim02.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_takegold01.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_takemedal.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_throwballinzero.mp3");
//	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_timeover.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_timewarning.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_tipinfull.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_useheart01.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_useitem.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_usetipin.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_wagnet04.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("se_crowd02.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("crashonhurdle.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("normaljump.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("bbang.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("birddeath.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("gameselect.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("uhuh.mp3");
	//KSoundEngine::sharedEngine()->registerSoundOnEffect("bonustime.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("shield.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("usemoney.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("useheart.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("getmedal.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("levelup.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnBGM("RetroSpace.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnBGM("hotshots.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("SE_arrow.wav");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("SE_pull.wav");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("SE_slot.wav");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("shout.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("readygo.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("slot.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("fever.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("hurryup.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("timeover.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("p0.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("p1.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("p2.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("p3.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("p4.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("p5.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("p6.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("p7.mp3");
	KSoundEngine::sharedEngine()->registerSoundOnEffect("exitzero.mp3");
	
	KSoundEngine::sharedEngine()->registerSoundOnEffect("skigihap.wav");
	playInfo->initLvTable();
	FBConnector::get()->setup("119950698180257");
    return true;
}

bool onceExit = false;
// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
//    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	if(playInfo->gamePtr && !CCDirector::sharedDirector()->isPaused())
		playInfo->gamePtr->PAUSE(0);
	onceExit = true;
	CCDirector::sharedDirector()->pause();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	if(!playInfo->gamePtr)
		CCDirector::sharedDirector()->resume();
	
	// 한번이라도 나갔다가 들어오면, 서버에서 다시 시간을 받음...
	if(onceExit)
	{
		if(playInfo->pIntroHead)
			playInfo->pIntroHead->syncTime();
	}
	// if you use SimpleAudioEngine, it must resume here
	if(NSDefault::getBGM())
		CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

//
//  FBConnector.h
//  archervszombie
//
//  Created by LitQoo on 12. 12. 17..
//
//


#ifndef fbconnector_h
#define fbconnector_h


#include "cocos-ext.h"
#include "cocos2d.h"
#include "curl/curl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "LQTableViewCell.h"
#include "JNIKelper.h"
#include <pthread.h>
#include "FBDelegator.h"
#include "FBJniHelper.h"

using namespace cocos2d;
using namespace std;

struct MemoryStruct {
    char *memory;
    size_t size;
};

enum FBConnectorResult {
    FBSuccess = 0,
    FBErrorCancel = 2,
    FBErrorNeedLogin =3,
    FBErrorCheckNetwork =4,
    FBErrorETC=5
};

class FBConnector:public CCNode{
	
public:
	string loginedMyFID;
    string userID;
    string userName;
    string appID;
    bool sendPermission;
    static FBConnector* get()
    {
        static FBConnector* _ins = 0;
        if(_ins == 0){
            _ins = new FBConnector();
            _ins->init();
//			_ins->setup("337927382992863");
        }
        return _ins;
    }
    
    bool isUsed(); //< 로그인 한번이라도 성공하면 켜짐. 서버에 저장은 아님.
    bool isLogin(); //<  현재 로그인 중인가?
    void setup(string aID); //< 알필요 없
    void setUser(string fID); //< 알필요 없
	void setUserName(string fName); //< 알필요 없
    void setSendPermission(bool state); //< 알필요 없
    void login(CCObject *target,FBSelType selector); //< isEnable 참일 때 실행...
    void getScores(CCObject *target,FBSelType selector); //< 친구들 목록 불러오기
    void sendScore(int nScore,CCObject *target,FBSelType selector); //< 서버에 점수 보내기
    void sendRequest(string fID,string pMsg,string pData,CCObject *target, FBSelType selector); //< 하트 보내는거, pdata 에 구분자.
	// 함께하기, arg(친구 페북아이디(받아온것), 보낼 메세지, 메세지 타입(내가 설정), 타겟, 셀렉터)
    void sendRequestResult(int result,int delekey,string _value); //< 알필요 없음
    void sendInvite(string fID,string pAppName,string pInviteMsg,string pLink, string pIconUrl,CCObject *target, FBSelType selector); // 초대
	// 초대하기, arg(친구 페북아이디(받아온것), 남들에게 보여질 앱 이름, 메세지 내용, 생성된 사이트 링크, 앱 아이콘 이미지 주소(pLink/icon.png), 타겟, 셀렉터)
    void getRequests(CCObject *target,FBSelType selector);
	// getScores와 비슷
    void removeRequests(string rID,CCObject *target,FBSelType selector);
	// arg(getRequests에서 받은 요청의 id, 타겟, 샐랙터)
    CCSprite* getProfileSprite(string userid, int fbw, int fbh);
    void logout();
	
private:
    int downloadIndex;
    bool imagesDown;
    CCArray *sprites;
    string buffer;
    
    void CreateNewSession();
    void customize(int delekey);
    static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
    void imageDown();
    static void *t_function(void *data);
    bool init();
    
    
};



class FBSprite : public cocos2d::CCSprite {
    
    
public:
	//	float w;
	//	float h;
    CCSprite *realSprite;
    string url;
    bool isDown;
	float realSpriteScaleX;
	float realSpriteScaleY;
    
    virtual bool init(string pURL){
        if(!CCSprite::initWithFile("facebook_imageblank.png")){
            return false;
        }
        url = pURL;
        realSprite=NULL;
        isDown=false;
		realSpriteScaleX=1.f;
		realSpriteScaleY=1.f;
        return true;
    }
    static FBSprite* create(string pURL){
        FBSprite* _rt = new FBSprite;
        _rt->init(pURL);
        _rt->autorelease();
        return _rt;
    }
	
    void createRealSprite(CCTexture2D *pTexture){
        if(this->isDown)return;
        
        this->realSprite=CCSprite::createWithTexture(pTexture);
        this->realSprite->setAnchorPoint(ccp(0.5,0.5));
		CCSize s = getContentSize();
        this->realSprite->setPosition(ccp(s.width / 2, s.height / 2));
		this->realSprite->setScaleX(realSpriteScaleX);
		this->realSprite->setScaleY(realSpriteScaleY);
        this->addChild(this->realSprite,2);
        this->isDown=true;
		
		//		float sw = w / realSprite->getContentSize().width;
		//		float sh = h / realSprite->getContentSize().height;
		//		realSprite->setScaleX(sw);
		//		realSprite->setScaleY(sh);
		
    }
    
    ~FBSprite(){
        
    }
private:
};

#endif
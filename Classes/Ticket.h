//
//  SendTicket.h
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 6. 27..
//  Copyright (c) 2013년 LitQoo Inc. All rights reserved.
//

#ifndef SportsWorldCupOriginal_SendTicket_cpp
#define SportsWorldCupOriginal_SendTicket_cpp

#include "cocos2d.h"

#include "JsonBox.h"
USING_NS_CC;

struct TicketData : public CCObject
{
	std::string data;
	struct _from
	{
		std::string fbid;
		std::string fbname;
	}from;
	std::string ticketid;
	
};
class Ticket : public CCLayer
{
private:
	CCArray* ticketDatas;
public:
	Ticket();
	virtual ~Ticket();
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	CREATE_FUNC(Ticket);
	virtual void registerWithTouchDispatcher();
	
	void EXIT(CCObject*);
	void ASK(CCObject*);
	void SENDTICKET(JsonBox::Value v2);
	void T_YES(CCObject*);
	void T_YES_OK(JsonBox::Value v2);
	void T_NO(CCObject*);
	void T_OK(CCObject*);
	void createTicketSend(); // 티켓요청을 보낼 때.
	void createRecvTicketReq(CCArray*); // 티켓 요청을 받았을 때,
	void createRecvTicket(CCArray*); // 티켓을 받았을 때
};

#endif

//
//  BasketShop.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 11..
//
//

#ifndef __SportsWorldCup__BasketShop__
#define __SportsWorldCup__BasketShop__
#include "cocos2d.h"
#include "JsonBox.h"
#include "GameShop.h"
#include "Bridge.h"
#include "KS_Util.h"
USING_NS_CC;
#include "GDTranslate.h"
using namespace GDTranslate;
class BasketShop : public GameShop
{
protected:
	virtual void onTime(){
		
	}
	virtual void offTime(){
		
	}
	virtual void onShield(){
		
	}
	virtual void offShield(){
		
	}
	virtual void onExp(){
		
	}
	virtual void offExp(){
		
	}
	virtual void onItem1(){
		selectedItems["item_tipin1.png"] = GameShop::SEL;
	}
	virtual void offItem1(){
		selectedItems["item_tipin1.png"] = GameShop::NONE;
	}
	virtual void onItem2(){
		selectedItems["item_longguide1.png"] = GameShop::SEL;
	}
	virtual void offItem2(){
		selectedItems["item_longguide1.png"] = GameShop::NONE;
	}
	virtual string getItem1Desc(){
		return gt("item_tipin1.png").c_str();
	}
	virtual string getItem2Desc(){
		return gt("item_longguide1.png").c_str();
	}
public:

	BasketShop()
	{
		item1Name = "item_tipin1.png";
		item2Name = "item_longguide1.png";
	}
	virtual ~BasketShop();
	virtual bool init();
	virtual map<string, int> priceTable(){
		map<string, int> 	 itemPrices;
		itemPrices["exp"] = 400;
		itemPrices["shield"] = 1000;
		itemPrices["item_random"] = 800;
		itemPrices["item_tipin1.png"] = 700;
		itemPrices["item_longguide1.png"] = 1300;
		return itemPrices;
	}
	static BasketShop* create(UIMediator *m)
	{
		BasketShop *pRet = new BasketShop();
		pRet->setMediator(m);
		if (pRet && pRet->init())
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}
};

#endif /* defined(__SportsWorldCup__BasketShop__) */

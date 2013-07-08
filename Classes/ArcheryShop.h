//
//  ArcheryShop.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 11..
//
//

#ifndef __SportsWorldCup__ArcheryShop__
#define __SportsWorldCup__ArcheryShop__
#include "cocos2d.h"
#include "JsonBox.h"
#include "GameShop.h"
#include "Bridge.h"
#include "KS_Util.h"
#include "GDTranslate.h"
using namespace GDTranslate;
USING_NS_CC;
class ArcheryShop : public GameShop{
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
		selectedItems["item_wind1.png"] = GameShop::SEL;
	}
	virtual void offItem1(){
		selectedItems["item_wind1.png"] = GameShop::NONE;
	}
	virtual void onItem2(){
		selectedItems["item_vibration1.png"] = GameShop::SEL;
	}
	virtual void offItem2(){
		selectedItems["item_vibration1.png"] = GameShop::NONE;
	}
	virtual string getItem1Desc(){
		return gt("item_wind1.png").c_str();
	}
	virtual string getItem2Desc(){
		return gt("item_vibration1.png").c_str();
	}
public:
	ArcheryShop()
	{
		item1Name = "item_wind1.png";
		item2Name = "item_vibration1.png";
	}
	virtual ~ArcheryShop();
	virtual bool init();
	virtual map<string, int> priceTable(){
		map<string, int> 	 itemPrices;
		itemPrices["exp"] = 400;
		itemPrices["shield"] = 1000;
		itemPrices["item_random"] = 800;
		itemPrices["item_wind1.png"] = 700;
		itemPrices["item_vibration1.png"] = 1300;
		return itemPrices;
	}
	static ArcheryShop* create(UIMediator *m)
	{
		ArcheryShop *pRet = new ArcheryShop();
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

#endif /* defined(__SportsWorldCup__ArcheryShop__) */

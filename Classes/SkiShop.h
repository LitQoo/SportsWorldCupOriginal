//
//  ArcheryShop.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 3. 11..
//
//

#ifndef __SportsWorldCup__TaekwonShop__
#define __SportsWorldCup__TaekwonShop__
#include "cocos2d.h"
#include "JsonBox.h"
#include "GameShop.h"
#include "Bridge.h"
#include "KS_Util.h"
USING_NS_CC;

#include "GDTranslate.h"
using namespace GDTranslate;
class SkiShop : public GameShop{
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
		selectedItems["item_skwide1.png"] = GameShop::SEL;
	}
	virtual void offItem1(){
		selectedItems["item_skwide1.png"] = GameShop::NONE;
	}
	virtual void onItem2(){
		selectedItems["item_curvespeed1.png"] = GameShop::SEL;
	}
	virtual void offItem2(){
		selectedItems["item_curvespeed1.png"] = GameShop::NONE;
	}
	
	virtual string getItem1Desc(){
		return gt("windvalue").c_str();
	}
	virtual string getItem2Desc(){
		return gt("noshake").c_str();
	}
public:

	SkiShop()
	{
		item1Name = "item_skwide1.png";
		item2Name = "item_curvespeed1.png";
	}
	virtual ~SkiShop();
	virtual bool init();
	virtual map<string, int> priceTable(){
		map<string, int> 	 itemPrices;
		itemPrices["exp"] = 400;
		itemPrices["shield"] = 1000;
		itemPrices["item_random"] = 800;
		itemPrices["item_skwide1.png"] = 700;
		itemPrices["item_curvespeed1.png"] = 1300;
		return itemPrices;
	}
	static SkiShop* create(UIMediator *m)
	{
		SkiShop *pRet = new SkiShop();
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

//
//  NSDefault.h
//  HurdleWorldCup
//
//  Created by ksoo k on 12. 4. 12..
//  Copyright (c) 2012≥‚ __MyCompanyName__. All rights reserved.
//
#pragma once

#include <string>
#include "GameSystem.h"
#include "cocos2d.h"
#include <sstream>
#include "KS_Util.h"
#include "SaveData.h"

using namespace cocos2d;
using namespace std;





class NSDefault
{
public:
	static string getUserName()
	{
		return saveData->getValue("SU", "");
	}
	static void setUserName(const string& n)
	{
		saveData->setKeyValue("SU", n);
	}
	
	static string getCountry()
	{
		return saveData->getValue("S_COUNTRY", "");
	}
	static void setCountry(const string& c)
	{
		saveData->setKeyValue("S_COUNTRY", c);
	}
	
	static int getEffectSound()
	{
		return saveData->getValue("S_EFFECTSOUND", 1);
	}
	static void setEffectSound(int s)
	{
		saveData->setKeyValue("S_EFFECTSOUND", s);
	}
	static int getBGM()
	{
		return saveData->getValue("S_BGM", 1);
	}
	static void setBGM(int b)
	{
		saveData->setKeyValue("S_BGM", b);
	}
	
	static int getVib()
	{
		return saveData->getValue("S_VIBRATION", 1);
	}
	static void setVib(int b)
	{
		saveData->setKeyValue("S_VIBRATION", b);
	}
	
	static void setGold(int p)
	{
		saveData->setKeyValue("S_PRICE", p);
	}
	static int getGold();
	static int getRuby();
	static void setRuby(int p)
	{
		saveData->setKeyValue("S_RUBY", p);
	}
	
	static void setHeartNumber(int b)
	{
		saveData->setKeyValue("S_HEARTNUMBER", b);
	}
	static int getHeartNumber()
	{
		return saveData->getValue("S_HEARTNUMBER", GameSystem::DEFAULT_MAX_HEART);
	}
	
	static int getGoldMedal(){
		return saveData->getValue("S_GOLD", 0);
	}
	static int getSilverMedal(){
		return saveData->getValue("S_SILVER", 0);
	}
	static int getCopperMedal(){
		return saveData->getValue("S_COPPER", 0);
	}
	static int getTop10Medal(){
		return saveData->getValue("S_TOP10", 0);
	}
	static void incGoldMedal(){setGoldMedal(getGoldMedal()+1);	}
	static void incSilverMedal()	{	setSilverMedal(getSilverMedal()+1); }
	static void incCopperMedal(){ setCopperMedal(getCopperMedal() + 1); }
	static void incTop10Medal(){ setTop10Medal(getTop10Medal() + 1);	}
	
	static void setGoldMedal(int g){
		saveData->setKeyValue("S_GOLD", g);
	}
	static void setSilverMedal(int s){
		saveData->setKeyValue("S_SILVER", s);
	}
	static void setCopperMedal(int c){
		saveData->setKeyValue("S_COPPER", c);
	}
	static void setTop10Medal(int t10){
		saveData->setKeyValue("S_TOP10", t10);
	}
	
	static void setExp(int b)
	{
		saveData->setKeyValue("S_EXP", b);
	}
	static int getExp()
	{
		return saveData->getValue("S_EXP", 0);
	}


	
	static void setHeartBaseTime(int b)
	{
		CCLog(" = %d", b);
		saveData->setKeyValue("S_HEARTBASETIME", b);
	}
	static int getHeartBaseTime()
	{
		return saveData->getValue("S_HEARTBASETIME", GameSystem::getCurrentTime_s());

	}
	
	
	
	static void setReview(int r){
		saveData->setKeyValue("S_REVIEW", r);
	}
	static int isReview(){
		return saveData->getValue("S_REVIEW", 0);
	}
	
	static int isSendable(string fbid, int base_s = 24 * 60 * 60); // 3 hours
	static void setSendable(string fbid);
	static int isInvitable(string fbid, int base_s = 24 * 60 * 60); // 3 hours
	static void setInvitable(string fbid);
	
	static int getInstallTime();
	static void setInstalltime(int t);
//	static int getTimeTodayFree()
//	{
//		return saveData->getValue("S_BUYTIME", 0);
//	}
//	static void setTimeTodayFree()
//	{
//		saveData->setKeyValue("S_BUYTIME", GameSystem::getCurrentTime_s());
//	}
//	static bool isTodayFree()
//	{
//		return GameSystem::getCurrentTime_s() - NSDefault::getTimeTodayFree() <= 60*24*24;
//	}
//	
	static bool isTodayFree()
	{
		return saveData->getValue("S_FREETODAY", 0);
	}
	static void setTodayFree(bool r)
	{
		saveData->setKeyValue("S_FREETODAY", r);
	}
	static int getFreeTodayBuyTime()
	{
		return saveData->getValue("S_FREETODAYTIME", 0);
	}
	static void setFreeTodayBuyTime(int t)
	{
		saveData->setKeyValue("S_FREETODAYTIME", t);
	}
	static void setInviteIn24H()
	{
		saveData->setKeyValue("S_INVITETIME", GameSystem::getCurrentTime_s());
	}

	static bool getInviteIn24H()
	{
		int savedTime = saveData->getValue("S_INVITETIME", 0);
		return GameSystem::getCurrentTime_s() - savedTime <= 60*24*24;
	}
	
	static void incArcherCount()
	{
		setArcherCount(getArcherCount() + 1);
	}
	static void setArcherCount(int n)
	{
		return saveData->setKeyValue("AC_COUNT", n);
	}
	static int getArcherCount()
	{
		return saveData->getValue("AC_COUNT", 0);
	}
	
	static void incClayCount()
	{
		setClayCount(getClayCount() + 1);
	}
	static void setClayCount(int n)
	{
		return saveData->setKeyValue("AK_COUNT", n);
	}
	static int getClayCount()
	{
		return saveData->getValue("AK_COUNT", 0);
	}
	
	static void incSkiCount()
	{
		setSkiCount(getSkiCount() + 1);
	}
	static void setSkiCount(int n)
	{
		return saveData->setKeyValue("SK_COUNT", n);
	}
	static int getSkiCount()
	{
		return saveData->getValue("SK_COUNT", 0);
	}
	static void setHanggCount(int n)
	{
		return saveData->setKeyValue("HG_COUNT", n);
	}
	static void incHanggCount()
	{
		setHanggCount(getSkiCount() + 1);
	}
	static int getHanggCount()
	{
		return saveData->getValue("HG_COUNT", 0);
	}
	static void incBasketCount()
	{
		setBasketCount(getBasketCount() + 1);
	}
	static void setBasketCount(int n)
	{
		return saveData->setKeyValue("BS_COUNT", n);
	}
	static int getBasketCount()
	{
		return saveData->getValue("BS_COUNT", 0);
	}
	
	
	static void incHurdleCount()
	{
		setHurdleCount(getHurdleCount() + 1);
	}
	static void setHurdleCount(int n)
	{
		return saveData->setKeyValue("HW_COUNT", n);
	}
	static int getHurdleCount()
	{
		return saveData->getValue("HW_COUNT", 0);
	}
	
	static void incRandomItemCount()
	{
		setRandomItemCount(getRandomItemCount() + 1);
	}
	static void setRandomItemCount(int n)
	{
		return saveData->setKeyValue("SW_RANDOMITEM", n);
	}
	
	static int getRandomItemCount()
	{
		return saveData->getValue("SW_RANDOMITEM", 0);
	}
	
	static int getLastAttendance()
	{
		return saveData->getValue("SW_ATT", 0);
	}
	static void setLastAttendance(int a)
	{
		saveData->setKeyValue("SW_ATT", a);
	}
	
	static int getAttendanceSeq()
	{
		return saveData->getValue("SW_ATTSEQ", 0);
	}
	
	static void setAttendanceSeq(int a)
	{
		saveData->setKeyValue("SW_ATTSEQ", a);
	}
	
	// 각 게임들의 오픈 여부.
	static void setOpenGame(const std::string& name)
	{
		saveData->setKeyValue(name, 1);
	}
	
	static int getIsOpenedGame(const std::string& name)
	{
		return saveData->getValue(name, 0);
	}
	
	static string getArrivedTicketId()
	{
		return saveData->getValue("SW_ARRVTICKETID", "");
	}
	
	static void setArrivedTicketId(const std::string& n)
	{
		saveData->setKeyValue("SW_ARRVTICKETID", n);
	}

	static int getTicketIsStarted()
	{
		return saveData->getValue("SW_TICKETSTART", 0);
	}
	static void setTicketIsStarted(int b)
	{
		saveData->setKeyValue("SW_TICKETSTART", b);
	}

	static string getSoonOpenGame()
	{
		return saveData->getValue("SW_SOONOPEN", "");
	}
	static void setSoonOpenGame(const std::string& n)
	{
		saveData->setKeyValue("SW_SOONOPEN", n);
	}
};
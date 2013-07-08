//
//  NSDefault.m
//  HurdleWorldCup
//
//  Created by ksoo k on 12. 4. 12..
//  Copyright (c) 2012�� __MyCompanyName__. All rights reserved.
//

#include "NSDefault.h"
#include "SaveData.h"

int NSDefault::isSendable(string fbid, int base_s) // 3 hours
{
	int ii = saveData->getValue(fbid, 0);
	
	if(ii + base_s < GameSystem::getCurrentTime_s())
	{
		return 1;
	}
	else
		return 0;
}
void NSDefault::setSendable(string fbid)
{
	saveData->setKeyValue(fbid, GameSystem::getCurrentTime_s());
}

int NSDefault::isInvitable(string fbid, int base_s)
{
	fbid = "INVITE" + fbid;
	int ii = atoi(saveData->getValue(fbid, "0").c_str());
	
	if(ii + base_s < GameSystem::getCurrentTime_s())
	{
		return 1;
	}
	else
		return 0;
}
void NSDefault::setInvitable(string fbid)
{
	fbid = "INVITE" + fbid;
	ostringstream oss;oss << GameSystem::getCurrentTime_s();
	saveData->setKeyValue(fbid, oss.str());
}

int NSDefault::getGold()
{
	return saveData->getValue("S_PRICE", 99999); // 5000
}
int NSDefault::getRuby()
{
	return saveData->getValue("S_RUBY", 5);
}
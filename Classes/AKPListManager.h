//
//  PListManager.h
//  AngryKeeper
//
//  Created by ksoo k on 12. 10. 22..
//
//

#ifndef __AngryKeeper__PListManager__
#define __AngryKeeper__PListManager__

#include <map>
#include <string>
#include "cocos2d.h"


USING_NS_CC;
using namespace std;
class PListManager
{
private:
	map<string, CCDictionary*> plistDictionary;// = CCFileUtils::create(plistPath);
	
public:
	static PListManager* sharedObject()
	{
		static PListManager* _ins = 0;
		if(_ins == 0)
		{
			_ins = new PListManager();
		}
		return _ins;
	}
	
	
private:
	PListManager()
	{
		//plistDictionary = CCFileUtils::create(plistPath);
	}
public:
	CCDictionary* getPList(string key)
	{
		if(plistDictionary.find(key) == plistDictionary.end())
		{
			CCDictionary* _temp = CCDictionary::createWithContentsOfFile (key.c_str());
			_temp->retain(); // not release
			plistDictionary[key] = _temp;
			return _temp;
		}
		else
		{
			return plistDictionary[key];
		}
	}
};


#endif /* defined(__AngryKeeper__PListManager__) */

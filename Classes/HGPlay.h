//
//  ACPlay.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 1. 24..
//
//

#ifndef __SportsWorldCup__HGPlay__
#define __SportsWorldCup__HGPlay__

#include "KSProtectVar.h"
#include <map>
#include <string>
using namespace std;

// 다른씬에서 쓰는 게임 관련 변수.
class HGPlay
{
public:
	KSProtectVar<int> gameScore;
	KSProtectVar<int> combo;
	map<string, bool> items;
public:
	static HGPlay* sharedObject()
	{
		static HGPlay* _ins = 0;
		if(_ins == 0)
		{
			_ins = new HGPlay();
		}
		return _ins;
	}
public:
	void initVars()
	{
		combo = 0;
		gameScore = 0;
	}
private:
	HGPlay() : combo(0), gameScore(0)
	
	{}
};
extern HGPlay* hgplay;

#endif /* defined(__SportsWorldCup__ACPlay__) */

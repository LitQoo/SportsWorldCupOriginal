//
//  WCPlay.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 4. 4..
//
//

#ifndef __SportsWorldCup__WCPlay__
#define __SportsWorldCup__WCPlay__

#include "KSProtectVar.h"
#include <map>
#include <string>
using namespace std;

// 다른씬에서 쓰는 게임 관련 변수.
class WCPlay
{
public:
//	KSProtectVar<int> gameScore;
//	KSProtectVar<int> accScore;
//	std::map<std::string, int> scoreEachGame;
public:
	static WCPlay* sharedObject()
	{
		static WCPlay* _ins = 0;
		if(_ins == 0)
		{
			_ins = new WCPlay();
		}
		return _ins;
	}
public:
	void initVars()
	{
//		accScore = 0;
//		gameScore = 0;
	}
	int totalGameScore();
private:
	WCPlay()
	
	{}
};

extern WCPlay* wcplay;


#endif /* defined(__SportsWorldCup__WCPlay__) */

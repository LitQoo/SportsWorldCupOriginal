//
//  ProbSelector.cpp
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 6. 25..
//
//

#include "ProbSelector.h"
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>

#include <assert.h>
#include "Well512.h"
int ProbSelector::sel(double args, ...)
{
	va_list argptr;
	va_start(argptr, args);        
	std::vector<float> ps;
	ps.push_back(args);
	for (int i = 0;; i++) 
	{
		double f = va_arg(argptr, double);
		if(f == 0.0)
			break;
		
		
		ps.push_back(f);
		//			cocos2d::CCLog("%f", f);
	}
	
	
	
	static Well512 well512;
	
	float sum = std::accumulate(ps.begin(), ps.end(), 0.f);
	assert(sum > 0.9f && sum <= 1.1f);
	
	float _01 = well512.GetFloatValue();
//	cocos2d::CCLog("%f", _01);
	float t = 0;
	int _index = 0;
	bool finded = false;
	
	for(auto i : ps) // C++ 11 라면 이 줄을 활성화.
//	for(std::vector<float>::iterator iter = ps.begin(); iter != ps.end(); ++iter)
	{
//		float i = *iter;
		t += i;
		_index++;
		if(_01 < t)
		{
			finded = true;
			break;
			//				va_end(argptr);
			//				return _index - 1;
		}
	}
	va_end(argptr);
	if(finded)
		return _index - 1;
	else
		return -1;

}
//void ProbSelector::pushProb(double f)
//{
//	ps.push_back(f);
//}


//int ProbSelector::sel()
//{
//	double sum = std::accumulate(ps.begin(), ps.end(), 0);
//	assert(sum > 0.9 && sum <= 1.1);
//
//	float _01 = well512.GetFloatValue();
//	double t = 0;
//	int _index = 0;
//	for(auto i : ps)
//	{
//		t += i;
//		_index++;
//		if(_01 < t)
//		{
//			return _index - 1;
//		}
//	}
//	
//	return -1;
//}


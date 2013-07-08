// hsnks100@gmail.com 한경수 

#ifndef _KSGeometry_h
#define _KSGeometry_h

#include "cocos2d.h"

cocos2d::CCPoint operator-(const cocos2d::CCPoint& t)
{
	return ccp(-t.x, -t.y);
}

cocos2d::CCPoint operator+(const cocos2d::CCPoint& left, const cocos2d::CCPoint& right)
{
	cocos2d::CCPoint p;
	p.x = left.x + right.x;
	p.y = left.y + right.y;
	return p;
};

cocos2d::CCPoint operator-(const cocos2d::CCPoint& left, const cocos2d::CCPoint& right)
{
	cocos2d::CCPoint p;
	p.x = left.x - right.x;
	p.y = left.y - right.y;
	return p;
};

cocos2d::CCPoint operator*(const cocos2d::CCPoint& left, float a)
{
	cocos2d::CCPoint p;
	p.x = left.x * a;
	p.y = left.y * a;
	return p;
};

cocos2d::CCPoint operator/(const cocos2d::CCPoint& left, float a)
{
	cocos2d::CCPoint p;
	p.x = left.x * a;
	p.y = left.y * a;
	return p;
};

cocos2d::CCSize operator+(const cocos2d::CCSize& left, const cocos2d::CCSize& right)
{
	cocos2d::CCSize p;
	p.width = left.width + right.width;
	p.height = left.height + right.height;
	return p;
};

cocos2d::CCSize operator-(const cocos2d::CCSize& left, const cocos2d::CCSize& right)
{
	cocos2d::CCSize p;
	p.width = left.width - right.width;
	p.height = left.height - right.height;
	return p;
};

cocos2d::CCSize operator*(const cocos2d::CCSize& left, float m)
{
	cocos2d::CCSize p;
	p.width = left.width * m;
	p.height = left.height * m;
	return p;
};

cocos2d::CCSize operator/(const cocos2d::CCSize& left, float m)
{
	cocos2d::CCSize p;
	p.width = left.width / m;
	p.height = left.height / m;
	return p;
};

cocos2d::CCSize operator-(const cocos2d::CCSize& t)
{
	return CCSizeMake(-t.width, -t.height);
}

cocos2d::CCRect rtSetScale(const cocos2d::CCRect& t, float sc, CCPoint ap = ccp(0.5f, 0.5f))
{
	CCRect ret = t;
	CCPoint move = ccp(t.size.width * ap.x - t.size.width * sc * ap.x,
					   t.size.height * ap.y - t.size.height * sc * ap.y);
	ret.origin = ret.origin + move;
	ret.size = ret.size * sc;
	return ret;
}

#endif

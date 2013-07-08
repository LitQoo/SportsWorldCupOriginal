#pragma once

#include "cocos2d.h"

USING_NS_CC;
#include <string>
class KSLabelTTF : public CCLabelTTF
{
public:
	static CCSprite* create(std::string str, std::string fontName, float fontSize, ccColor3B color, float strokeSize, ccColor3B strokeColor);
};


#ifndef __CARROT_H__
#define __CARROT_H__

#include "cocos2d.h"
using namespace cocos2d;
//creating a class representing every image in the layer in other word every square in the layer surface and walls
class Carrot:public cocos2d::Sprite
{

public:

	cocos2d::Vec2 getposition();
	void setposition(float x, float y);
	cocos2d::Rect getrect();
	void removeFromParent();
	void getkey();
	Carrot(cocos2d::Layer* layer);
private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	//desctructor
	//this is where we initiate the sprite with a file path,
	cocos2d::Sprite* CARROT;
};
#endif //__CARROT_H__
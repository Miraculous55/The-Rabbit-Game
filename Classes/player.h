
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
using namespace cocos2d;
//creating a class representing every image in the layer in other word every square in the layer surface and walls
class OurPlayer
{

public:

	cocos2d::Vec2 getposition();
	void setposition(float x, float y);
	cocos2d::Rect getrect();
	void turnLeft(float z);
	void turnRight();
	OurPlayer(cocos2d::Layer* layer, bool jump );
private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	//desctructor
	//this is where we initiate the sprite with a file path,
	cocos2d::Sprite* Pico;

};
#endif
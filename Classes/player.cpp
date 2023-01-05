#include "player.h"

USING_NS_CC;
Action* action; 
OurPlayer::OurPlayer(cocos2d::Layer* layer, bool jump) {
	
	Pico = Sprite::create("lapin1.png");
	Pico->setScale(0.2);
	Pico->setPosition(Point(origin.x + 100, origin.y + 100));

	auto  Playerbody = PhysicsBody::createBox(Size(Pico->getContentSize().width-60, Pico->getContentSize().height), PhysicsMaterial(0, 0, 0));
	Playerbody->setCollisionBitmask(1);
	Playerbody->setContactTestBitmask(true);
	Playerbody->setRotationEnable(false);

	Pico->setPhysicsBody(Playerbody);

	layer->addChild(Pico, 1);
};

cocos2d::Vec2 OurPlayer::getposition() {
	return Pico->getPosition();
}

void  OurPlayer::setposition(float x, float y) {
	Pico->setPosition(Point(x, y));
}


cocos2d::Rect OurPlayer::getrect() {
	return Pico->getBoundingBox();
}



void OurPlayer::turnLeft(float z) {
	Pico->setRotation3D(Vec3(0,z,0));
}

void OurPlayer::turnRight()
{
	int x = Pico->getRotation3D().y ;
	if (x == 180)
	{
		Pico->setRotation3D(Vec3(0, 0, 0));
	}

}
#include "Carrot.h"

USING_NS_CC;

Carrot::Carrot(cocos2d::Layer* layer) {
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	CARROT = Sprite::create("carrot.png");
	CARROT->setScale(0.05);
	CARROT->setPosition(Point(origin.x +550, origin.y+150));
	layer->addChild(CARROT, 1);
};


cocos2d::Vec2 Carrot::getposition() {
	return CARROT->getPosition();
}
void  Carrot::setposition(float x, float y) {
	CARROT->setPosition(Point(x, y));
}
//
void Carrot::removeFromParent() {
	CARROT->setVisible(false);
}
void Carrot::getkey(){
	auto action = MoveBy::create(20, Vec2(0,-1));
	CARROT->runAction(action);
}
cocos2d::Rect Carrot::getrect() {
	return CARROT->getBoundingBox();
}
#include "menu.h"
#include "HelloWorldScene.h"
#include "AudioEngine.h"


USING_NS_CC;

Scene* MainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = MainMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Layer::init())
    {
        return false;
    }



    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::create("background.jpg");
    background->setPosition(Vec2(origin.x + visibleSize.width/2,origin.y+visibleSize.height/2));
    this->addChild(background, -4);

    auto titre = Sprite::create("titre.png");
    titre->setPosition(Vec2(visibleSize.width /2 , visibleSize.height / 2 + 70));
    titre->setScale(1.3);
    this->addChild(titre, -4);

    auto playItem = MenuItemImage::create("play.png","play.png", CC_CALLBACK_1(MainMenu::GoToHelloWorldScene, this));
    playItem->setScale(0.2);
    auto menu = Menu::create(playItem, NULL);
    menu->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 40);
    this->addChild(menu);

    return true;
}



void MainMenu::GoToHelloWorldScene(cocos2d::Ref* pSender)
{
    cocos2d::AudioEngine::preload("audio/Mouse.mp3");
    cocos2d::AudioEngine::play2d("audio/Mouse.mp3");
    auto scene = MyWorld::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5,scene));
}
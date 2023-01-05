
#ifndef __Lvl2_H__
#define __Lvl2_H__
#include "cocos2d.h"
#include "player.h"
#include "Carrot.h"



class Lvl2 : public cocos2d::Layer
{
public:
    void update(float dt);

    void moveright(float dt);
    void moveleft(float dt);
    void movetop(float dt);
    static cocos2d::Scene* createScene();

    virtual bool init();

    bool onContactBegin(cocos2d::PhysicsContact& contact);
    

    
    // implement the "static create()" method manually
    CREATE_FUNC(Lvl2);
    OurPlayer* player;
    Carrot* carrot;
    Carrot* carrot1;
    Carrot* carrot2;
    Sprite* wall;
    Sprite* lava;
    
    std::string text;
    Label* scoreLabel;

    int count = 0;
    bool isUpPressed = false;
    bool isRightpressed = false;
    bool isLeftpressed = false;
    bool isSpacePressed = false;
    bool OnGround = true;
    bool iscarrotcollected = false;
    bool iscarrot1collected = false;
    bool iscarrot2collected = false;
};

#endif // __Lvl2_H__

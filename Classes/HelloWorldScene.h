
#ifndef __MYWORLD_SCENE_H__
#define __MYWORLD_SCENE_H__
#include "cocos2d.h"
#include "player.h"
#include "Carrot.h"




class MyWorld : public cocos2d::Layer
{
private:
    int count;
    bool ifUpPressed = false;
    bool ifRightpressed = false;
    bool ifLeftpressed = false;
    bool ifSpacePressed = false;
    bool isOnGround = true;
    bool carrotcollected = false;
    bool carrotcollected1 = false;
    bool carrotcollected2 = false;
public:
    void update(float dt);
    void moveright(float dt);
    void moveleft(float dt);
    void movetop(float dt);
   
    static cocos2d::Scene* createScene();

    virtual bool init();

    bool onContactBegin(cocos2d::PhysicsContact& contact);
    

    
    // implement the "static create()" method manually
    CREATE_FUNC(MyWorld);
    // our player (rabbit)
    OurPlayer* player;
    // carrot
    Carrot* carrot;
    // carrot
    Carrot* carrot1;
    // carrot
    Carrot* carrot2;
    //ground 
    Sprite* wall;

    std::string text;

    Label* scoreLabel;



};

#endif // __MYWORLD_SCENE_H__

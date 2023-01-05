
#ifndef __Lvl3_H__
#define __Lvl3_H__
#include "cocos2d.h"
#include "player.h"
#include "Carrot.h"
#include "ui/UIRichText.h"



class Lvl3 : public cocos2d::Layer
{
public:

    bool UpPressed = false;
    bool Rightpressed = false;
    bool Leftpressed = false;
    bool SpacePressed = false;
    bool Ground = true;
    bool haveCarrot = false;
    bool haveCarrot1 = false;
    bool haveCarrot2 = false;


    void update(float dt);
    void moveright(float dt);
    void moveleft(float dt);
    void movetop(float dt);
    static cocos2d::Scene* createScene();
    cocos2d::Label *timeTxt;
    virtual bool init();
    bool onContactBegin(cocos2d::PhysicsContact& contact);

   
    

    
    // implement the "static create()" method manually
    CREATE_FUNC(Lvl3);
    OurPlayer* player;
    Carrot* carrot;
    Carrot* carrot2;
    Carrot* carrot3;
    Sprite* wall;
    Sprite* wall2;
    Sprite* wall3;
    Sprite* lava1;
    Sprite* lava2;
    int count = 0;
    std::string text;
    Label* scoreLabel;
};

#endif // __Lvl2_H__

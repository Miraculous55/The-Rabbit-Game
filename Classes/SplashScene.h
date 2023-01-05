#ifndef __WELCOME_SCENE_H__
#define __WELCOME_SCENE_H__

#include "cocos2d.h"
#include "menu.h"
class SplashScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    //method for initialize my instance
    virtual bool init();
    // implement the "static create()" method manually
    CREATE_FUNC(SplashScene);
private:
    //method for switching scene
    void GoToMenuScene(float dt);
};

#endif // __WELCOME_SCENE_H__

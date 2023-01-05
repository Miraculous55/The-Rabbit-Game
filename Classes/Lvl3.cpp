#include "Lvl3.h"
#include "GameOver.h"
#include "ui/UITextField.h"
#include "AudioEngine.h"

USING_NS_CC;



Scene* Lvl3::createScene()
{
    auto scene = Scene::createWithPhysics();

    scene->getPhysicsWorld()->setGravity(Vec2(0, -400));

    auto layer = Lvl3::create();
    scene->addChild(layer);
    return scene;
}

// Print useful error message instead of segfaulting when files are not there

// on "init" you need to initialize your instance
bool Lvl3::init()
{
    if (!Layer::init())
    {
        return false;
    }

    //------------------------------------------------------------create backgroud color-------------------------------------------------

    LayerColor* _bgColor = LayerColor::create(Color4B(206, 248, 252, 255));
    this->addChild(_bgColor, -10);

    //---------------------------------------------create variables for positioning our instance-------------------------------------------------------

    Size visibleSize = Director::getInstance()->getWinSize();
    
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //-------------------------------------------------create an edge box for our game------------------------------------------------------

    auto edgebody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    edgebody->setCollisionBitmask(3);
    edgebody->setContactTestBitmask(true);
    auto edgeNode = Node::create();
    edgeNode->setPosition(Point((visibleSize.width / 2) , (visibleSize.height / 2)));
    edgeNode->setPhysicsBody(edgebody);
    this->addChild(edgeNode);
    
    this->setScale(1.3);

    //-----------------------------------Score Text--------------------

    text = StringUtils::format("%d / 3", count);

    scoreLabel = Label::createWithTTF(text, "fonts/arial.ttf", 24);
    scoreLabel->setTextColor(Color4B::BLACK);
    scoreLabel->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
    this->addChild(scoreLabel, 10);


    wall = Sprite::create("ground3.jpg");
    wall->setScale(0.5);
    auto Frame4body = PhysicsBody::createBox(Size(wall->getContentSize().width, wall->getContentSize().height), PhysicsMaterial(0, 0, 0));
    Frame4body->setCollisionBitmask(2);
    Frame4body->setContactTestBitmask(true);
    wall->setAnchorPoint(Vec2());
    wall->setPosition(Point( origin.x,  origin.y));
    if (Frame4body != nullptr)
    {
        Frame4body->setDynamic(false);
        wall->setPhysicsBody(Frame4body);
    }
    this->addChild(wall);


    wall2 = Sprite::create("ground3.jpg");
    wall2->setScale(0.5);
    auto Frame2body = PhysicsBody::createBox(Size(wall2->getContentSize().width, wall2->getContentSize().height), PhysicsMaterial(0, 0, 0));
    Frame2body->setCollisionBitmask(2);
    Frame2body->setContactTestBitmask(true);
    wall2->setAnchorPoint(Vec2());
    wall2->setPosition(Point(origin.x + 250, origin.y));
    if (Frame2body != nullptr)
    {
        Frame2body->setDynamic(false);
        wall2->setPhysicsBody(Frame2body);
    }
    this->addChild(wall2);

    wall3 = Sprite::create("ground2.jpg");
    wall3->setScale(0.5);
    auto Frame3body = PhysicsBody::createBox(Size(wall3->getContentSize().width, wall3->getContentSize().height), PhysicsMaterial(0, 0, 0));
    Frame3body->setCollisionBitmask(2);
    Frame3body->setContactTestBitmask(true);
    wall3->setAnchorPoint(Vec2());
    wall3->setPosition(Point(origin.x + 500, origin.y));
    if (Frame3body != nullptr)
    {
        Frame3body->setDynamic(false);
        wall3->setPhysicsBody(Frame3body);
    }
    this->addChild(wall3);


    player = new OurPlayer(this,!Ground );
    player->setposition(origin.x + 20 , origin.y + 100);
    carrot = new Carrot(this);
    carrot->setposition(origin.x + 100, origin.y +100);
    carrot2 = new Carrot(this);
    carrot2->setposition(origin.x + 350, origin.y + 120);
    carrot3 = new Carrot(this);
    carrot3->setposition(origin.x + 650, origin.y + 130);
    lava1 = Sprite::create("lava.png");
    lava1->setAnchorPoint(Vec2());
    lava1->setPosition(Point(origin.x + 200, origin.y + 30));
    lava1->setScale(0.25);
    this->addChild(lava1);
    lava2 = Sprite::create("lava.png");
    lava2->setAnchorPoint(Vec2());
    lava2->setPosition(Point(origin.x + 450, origin.y + 30));
    lava2->setScale(0.25);
    this->addChild(lava2);
    


    auto eventListener = EventListenerKeyboard::create();

    eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
     
            switch (keyCode) {
                //case wich key is pressed
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                Leftpressed = true;
                this->schedule(SEL_SCHEDULE(&Lvl3::moveleft), 0.01);
                break;

            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                Rightpressed = true;
                this->schedule(SEL_SCHEDULE(&Lvl3::moveright), 0.01);
                break;

            case EventKeyboard::KeyCode::KEY_SPACE:
                if (SpacePressed == false)
                {
                    this->schedule(SEL_SCHEDULE(&Lvl3::movetop), 0.01);
                    SpacePressed = true;
                    Ground = false;
                    cocos2d::AudioEngine::preload("audio/jump.mp3");
                    cocos2d::AudioEngine::play2d("audio/jump.mp3", false, 0.3f);
                }
                break;
            } 
    };
    //key released event listener
    eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event)
    {
            switch (keyCode) {
                //case wich key is pressed
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                Leftpressed = false;
                this->unschedule(SEL_SCHEDULE(&Lvl3::moveleft));
                cocos2d::AudioEngine::pauseAll();
                break;
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                Rightpressed = false;
                this->unschedule(SEL_SCHEDULE(&Lvl3::moveright));
                cocos2d::AudioEngine::pauseAll();
                break;
            case EventKeyboard::KeyCode::KEY_SPACE:
                this->unschedule(SEL_SCHEDULE(&Lvl3::movetop));
                break;         
            }
    };

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
    //we create a contact listener to detect the collision of the ball with te walls and the obstacles
    auto contactListener = EventListenerPhysicsContact::create();
    //make a call back to the function everytime a contact happen
    contactListener->onContactBegin = CC_CALLBACK_1(Lvl3::onContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);



    this->scheduleUpdate();


    return true;
}


bool Lvl3::onContactBegin(cocos2d::PhysicsContact& contact)
{
    PhysicsBody* a = contact.getShapeA()->getBody();
    PhysicsBody* b = contact.getShapeB()->getBody();
    auto p = contact.getContactData()->points;

    // check if the bodies have collided
    if ((1 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask()) || (2 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask()))
    {
        SpacePressed = false;
        Ground = true;
    }
    else if ((1 == a->getCollisionBitmask() && 4 == b->getCollisionBitmask()) || (4 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask()))
    {
        SpacePressed = false;
    }
    return true;
}
 
void Lvl3::update(float dt) {
        
        Size v = Director::getInstance()->getWinSize();
        Rect boxPlayer = player->getrect();
        Rect boxCarrot = carrot->getrect();
        Rect boxCarrot1 = carrot2->getrect();
        Rect boxCarrot2 = carrot3->getrect();
        Rect boxLava1 = lava1->getBoundingBox();
        Rect boxLava2 = lava2->getBoundingBox();
        
        Layer::setAnchorPoint(Vec2(player->getposition().x / v.width, player->getposition().y /v.height));
        if (boxPlayer.intersectsRect(boxCarrot))
        {   
            if (!haveCarrot)
            {
                cocos2d::AudioEngine::preload("audio/key.mp3");
                cocos2d::AudioEngine::play2d("audio/key.mp3", false, 0.3f);
                haveCarrot = true;
                carrot->removeFromParent();
                count++;
                scoreLabel->setString(StringUtils::format("%d / 3", count));
            }

        }
        if (boxPlayer.intersectsRect(boxCarrot1))
        {
            if (!haveCarrot1)
            {
                cocos2d::AudioEngine::preload("audio/key.mp3");
                cocos2d::AudioEngine::play2d("audio/key.mp3", false, 0.3f);
                haveCarrot1 = true;
                carrot2->removeFromParent();
                count++;
                scoreLabel->setString(StringUtils::format("%d / 3", count));
            }

        }
        if (boxPlayer.intersectsRect(boxCarrot2))
        {
            if (!haveCarrot2)
            {
                cocos2d::AudioEngine::preload("audio/key.mp3");
                cocos2d::AudioEngine::play2d("audio/key.mp3", false, 0.3f);
                haveCarrot2 = true;
                carrot3->removeFromParent();
                count++;
                scoreLabel->setString(StringUtils::format("%d / 3", count));
            }

        }
        if (boxPlayer.intersectsRect(boxLava1))
        {
            auto scene = Lvl3::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(0.2, scene));
        }
        if (boxPlayer.intersectsRect(boxLava2))
        {
            auto scene = Lvl3::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(0.2, scene));
        }
        if (Leftpressed)
        {
            player->turnLeft(180);
        }
        else if (Rightpressed)
        {
            player->turnRight();

        }
        //condition to moove to next level 
        if (count == 3) {
            UpPressed = false;
            auto scene = GameOver::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(0.2, scene));
        }


}

void Lvl3::moveright(float dt) {
    Vec2 playerPos = player->getposition();
    player->setposition(playerPos.x + 80 * dt, playerPos.y);
}
void Lvl3::moveleft(float dt) {
    Vec2 ballpos = player->getposition();
    player->setposition(ballpos.x - 80 * dt, ballpos.y);
}
void Lvl3::movetop(float dt) {
    Vec2 ballpos = player->getposition();
    player->setposition(ballpos.x, ballpos.y + 160 * dt);
}
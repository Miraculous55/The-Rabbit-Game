#include "Lvl2.h"
#include "Lvl3.h"
#include "AudioEngine.h"

USING_NS_CC;


Scene* Lvl2::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0, -400));
    auto layer = Lvl2::create();
    scene->addChild(layer);
    return scene;
}

// Print useful error message instead of segfaulting when files are not there

// on "init" you need to initialize your instance
bool Lvl2::init()
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
    //-----------------------------------Score Text--------------------
    text = StringUtils::format("%d / 3", count);

    scoreLabel = Label::createWithTTF(text, "fonts/arial.ttf", 24);
    scoreLabel->setTextColor(Color4B::BLACK);
    scoreLabel->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
    this->addChild(scoreLabel, 10);

    //-------------------------------------------------create an edge box for our game------------------------------------------------------

    auto edgebody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    edgebody->setCollisionBitmask(3);
    edgebody->setContactTestBitmask(true);
    auto edgeNode = Node::create();
    edgeNode->setPosition(Point((visibleSize.width / 2) , (visibleSize.height / 2)));
    edgeNode->setPhysicsBody(edgebody);
    this->addChild(edgeNode);

    //---------------------------------------------------------zoom on the scene--------------------------------------------
    this->setScale(1.3);

    //------------------------------creating grounds and wall for the map -------------------------------
    wall = Sprite::create("ground2.jpg");
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

    auto gound2 = Sprite::create("ground2.jpg");
    gound2->setScale(0.5);
    auto gound2body = PhysicsBody::createBox(Size(gound2->getContentSize().width, gound2->getContentSize().height), PhysicsMaterial(0, 0, 0));
    gound2body->setCollisionBitmask(2);
    gound2body->setContactTestBitmask(true);
    gound2->setAnchorPoint(Vec2());
    gound2->setPosition(Point(origin.x + 490, origin.y ));
    if (gound2body != nullptr)
    {
        gound2body->setDynamic(false);
        gound2->setPhysicsBody(gound2body);
    }
    this->addChild(gound2);

    // creating instances from classes 

    player = new OurPlayer(this,!OnGround);
    player->setposition(origin.x+50,origin.y+100);

    carrot = new Carrot(this);
    carrot->setposition(origin.x +200, origin.y + 120 );
    carrot1 = new Carrot(this);
    carrot1->setposition(origin.x + 500, origin.y + 150);
    carrot2 = new Carrot(this);
    carrot2->setposition(origin.x + 700, origin.y + 130);

    lava = Sprite::create("lava.png");
    lava->setAnchorPoint(Vec2());
    lava->setPosition(Point(origin.x + 400, origin.y + 30));
    lava->setScale(0.45);
    this->addChild(lava);

    


    auto eventListener = EventListenerKeyboard::create();

    eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
     
            switch (keyCode) {
                //case wich key is pressed
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW: 
                isLeftpressed = true;
                this->schedule(SEL_SCHEDULE(&Lvl2::moveleft), 0.01);
                break;

            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                isRightpressed = true;
                this->schedule(SEL_SCHEDULE(&Lvl2::moveright), 0.01);
                break;

            case EventKeyboard::KeyCode::KEY_SPACE:
                if (isSpacePressed == false)
                {
                    cocos2d::AudioEngine::preload("audio/jump.mp3");
                    cocos2d::AudioEngine::play2d("audio/jump.mp3", false, 0.3f);
                    this->schedule(SEL_SCHEDULE(&Lvl2::movetop), 0.01);
                    isSpacePressed = true;
                    OnGround = false;
                }
                break;

            } 
      
    };

    eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event)
    {
   
            switch (keyCode) {
                //case wich key is pressed
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                isLeftpressed = false;
                this->unschedule(SEL_SCHEDULE(&Lvl2::moveleft));
                cocos2d::AudioEngine::pauseAll();
                break;

            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                isRightpressed = false;
                this->unschedule(SEL_SCHEDULE(&Lvl2::moveright));
                cocos2d::AudioEngine::pauseAll();
                break;

            case EventKeyboard::KeyCode::KEY_SPACE:
                this->unschedule(SEL_SCHEDULE(&Lvl2::movetop));
                break;
            }
    };

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
    //we create a contact listener to detect the collision of the ball with te walls and the obstacles
    auto contactListener = EventListenerPhysicsContact::create();
    //make a call back to the function everytime a contact happen
    contactListener->onContactBegin = CC_CALLBACK_1(Lvl2::onContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);


    this->scheduleUpdate();


    return true;
}
void Lvl2::moveright(float dt) {
    Vec2 playerPos = player->getposition();
    player->setposition(playerPos.x + 80* dt, playerPos.y);
}
void Lvl2::moveleft(float dt) {
    Vec2 playerPos = player->getposition();
    player->setposition(playerPos.x - 80 * dt, playerPos.y);
}
void Lvl2::movetop(float dt) {
    Vec2 playerPos = player->getposition();
    player->setposition(playerPos.x, playerPos.y + 160 * dt);
}

bool Lvl2::onContactBegin(cocos2d::PhysicsContact& contact)
{
    PhysicsBody* a = contact.getShapeA()->getBody();
    PhysicsBody* b = contact.getShapeB()->getBody();
    auto p = contact.getContactData()->points;

    // check if the bodies have collided
    if ((1 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask()) || (2 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask()))
    {
        isSpacePressed = false;
        OnGround = true;
    }
    if ((1 == a->getCollisionBitmask() && 3 == b->getCollisionBitmask()) || (3 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask())) {
        if (isLeftpressed)
        {
            player->setposition(player->getposition().x - 40, player->getposition().y);
           
        }
        else if (isRightpressed)
        {
            player->setposition(player->getposition().x + 40, player->getposition().y);
          
        }
    }
    return true;
}
 


void Lvl2::update(float dt) {
        Rect boxPlayer = player->getrect();
        Rect boxCarrot = carrot->getrect();
        Rect boxCarrot1 = carrot1->getrect();
        Rect boxCarrot2 = carrot2->getrect();
        Rect boxLava = lava->getBoundingBox();
        Size v = Director::getInstance()->getWinSize();

        Layer::setAnchorPoint(Vec2(player->getposition().x / v.width, player->getposition().y /v.height));

        if (boxPlayer.intersectsRect(boxCarrot))
        {
            carrot->removeFromParent();
            if (!iscarrotcollected)
            {
                cocos2d::AudioEngine::preload("audio/key.mp3");
                cocos2d::AudioEngine::play2d("audio/key.mp3", false, 0.3f);
                iscarrotcollected = true;
                count++;
                scoreLabel->setString(StringUtils::format("%d / 3", count));
            } 
        }
        if (boxPlayer.intersectsRect(boxCarrot1))
        {
            carrot1->removeFromParent();
            if (!iscarrot1collected)
            {
                cocos2d::AudioEngine::preload("audio/key.mp3");
                cocos2d::AudioEngine::play2d("audio/key.mp3", false, 0.3f);
                iscarrot1collected = true;
                count++;
                scoreLabel->setString(StringUtils::format("%d / 3", count));
            }
        }
        if (boxPlayer.intersectsRect(boxCarrot2))
        {
            carrot2->removeFromParent();
            if (!iscarrot2collected)
            {
                cocos2d::AudioEngine::preload("audio/key.mp3");
                cocos2d::AudioEngine::play2d("audio/key.mp3", false, 0.3f);
                iscarrot2collected = true;
                count++;
                scoreLabel->setString(StringUtils::format("%d / 3", count));
            }
        }
        if (isLeftpressed)
        {
            player->turnLeft(180);
        }
        else if (isRightpressed)
        {
            player->turnRight();

        }

        if (boxPlayer.intersectsRect(boxLava))
        {
            auto scene = Lvl2::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(0.2, scene));
        }
        //condition to moove to next level 
        if (count == 3) {
            auto scene = Lvl3::createScene();
            Director::getInstance()->replaceScene(scene);
        }
}
#include "HelloWorldScene.h"
#include "Lvl2.h"
#include "AudioEngine.h"

USING_NS_CC;

Scene* MyWorld::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0, -400));
    auto layer = MyWorld::create();
    scene->addChild(layer);
    return scene;
}

// Print useful error message instead of segfaulting when files are not there

// on "init" you need to initialize your instance
bool MyWorld::init()
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

    //-------------------------------------------------initialize variables------------------------------------------------------
    count = 0;
    //-------------------------------------------------create an edge box for our game------------------------------------------------------


    auto edgebody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    edgebody->setCollisionBitmask(3);
    edgebody->setContactTestBitmask(true);
    auto edgeNode = Node::create();
    edgeNode->setPosition(Point((visibleSize.width / 2), (visibleSize.height / 2)));
    edgeNode->setPhysicsBody(edgebody);
    this->addChild(edgeNode);

    //---------------------------------------------------------zoom on the scene--------------------------------------------
    this->setScale(1.3);
    //-----------------------------------Score Text--------------------

    text = StringUtils::format("%d / 3", count);

    scoreLabel = Label::createWithTTF(text, "fonts/arial.ttf", 24);
    scoreLabel->setTextColor(Color4B::BLACK);
    scoreLabel->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
    this->addChild(scoreLabel, 10);


    //------------------------------creating grounds and wall for the map -------------------------------

    wall = Sprite::create("ground.jpg");
    wall->setScale(0.5);
    auto Frame4body = PhysicsBody::createBox(Size(wall->getContentSize().width, wall->getContentSize().height), PhysicsMaterial(0, 0, 0));
    Frame4body->setCollisionBitmask(2);
    Frame4body->setContactTestBitmask(true);
    wall->setAnchorPoint(Vec2());
    wall->setPosition(Point(origin.x, origin.y));
    if (Frame4body != nullptr)
    {
        Frame4body->setDynamic(false);
        wall->setPhysicsBody(Frame4body);
    }
    this->addChild(wall);

    // creating instances from classes 
    //create instance of player class
    player = new OurPlayer(this, !isOnGround);
    //create instance of carrot
    carrot = new Carrot(this);
    carrot1 = new Carrot(this);
    carrot1->setposition(player->getposition().x + 100, player->getposition().y);
    carrot2 = new Carrot(this);
    carrot2->setposition(player->getposition().x + 200, player->getposition().y + 50);

    

    // keey board events listener
    auto eventListener = EventListenerKeyboard::create();

    eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {

        switch (keyCode) {
            //case wich key is pressed
            //case left arrow pressed
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            ifLeftpressed = true;
            this->schedule(SEL_SCHEDULE(&MyWorld::moveleft), 0.01);
            break;
            //case right arrow pressed
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            ifRightpressed = true;
            this->schedule(SEL_SCHEDULE(&MyWorld::moveright), 0.01);
            break;
            //case space pressed
        case EventKeyboard::KeyCode::KEY_SPACE:
            //if space is pressed can't jump another time
            if (ifSpacePressed == false)
            {
                this->schedule(SEL_SCHEDULE(&MyWorld::movetop), 0.01);
                ifSpacePressed = true;
                isOnGround = false;
                //play jump sound
                cocos2d::AudioEngine::preload("audio/jump.mp3");
                cocos2d::AudioEngine::play2d("audio/jump.mp3", false, 0.3f);
            }
            break;

        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            ifUpPressed = true;
            break;

        }
    };

    // event listener when key released
    eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event)
    {
        switch (keyCode) {
            //case wich key is pressed
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case EventKeyboard::KeyCode::KEY_A:
            ifLeftpressed = false;
            this->unschedule(SEL_SCHEDULE(&MyWorld::moveleft));
            cocos2d::AudioEngine::pauseAll();
            break;

        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            ifRightpressed = false;
            cocos2d::AudioEngine::pauseAll();
            this->unschedule(SEL_SCHEDULE(&MyWorld::moveright));
            break;

        case EventKeyboard::KeyCode::KEY_SPACE:
            this->unschedule(SEL_SCHEDULE(&MyWorld::movetop));
            break;

        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            ifUpPressed = false;
            break;

        }
    };
    //add event listener of keyboard to the scene
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
    //we create a contact listener to detect the collision of the ball with te walls and the obstacles
    auto contactListener = EventListenerPhysicsContact::create();
    //make a call back to the function everytime a contact happen
    contactListener->onContactBegin = CC_CALLBACK_1(MyWorld::onContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    //----------------------------------------- schedule update soo we can track state of the game----------
    this->scheduleUpdate();


    return true;
}



bool MyWorld::onContactBegin(cocos2d::PhysicsContact& contact)
{
    PhysicsBody* a = contact.getShapeA()->getBody();
    PhysicsBody* b = contact.getShapeB()->getBody();
    auto p = contact.getContactData()->points;

    // check if the ground collided with the player
    if ((1 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask()) || (2 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask()))
    {
        //if collided then the player is on ground (just to can't jump twice)
        ifSpacePressed = false;
        isOnGround = true;
    }
    //if the player and edge of screen collided
    if ((1 == a->getCollisionBitmask() && 3 == b->getCollisionBitmask()) || (3 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask())) {
        //if the right button pressed
        if (ifRightpressed)
        {
            //stop the player from moving
            this->unschedule(SEL_SCHEDULE(&MyWorld::moveright));
            //set the position of player -10 from his initial position
            player->setposition(player->getposition().x - 10, player->getposition().y);
        }
        //if the left button pressed
        else if (ifLeftpressed)
        {
            //stop the player from moving
            this->unschedule(SEL_SCHEDULE(&MyWorld::moveleft));
            //set the position of player +10 from his initial position
            player->setposition(player->getposition().x + 10, player->getposition().y);
        }
    }



    return true;
}



void MyWorld::update(float dt) {

    Size v = Director::getInstance()->getWinSize();

    // making the AnchorPoint follow the player
    Layer::setAnchorPoint(Vec2(player->getposition().x / v.width, player->getposition().y / v.height));
    // creating Rect for each sprite to detect it position and collision with other sprites

    Rect boxPlayer = player->getrect();
    Rect boxCarrot = carrot->getrect();
    Rect boxCarrot1 = carrot1->getrect();
    Rect boxCarrot2 = carrot2->getrect();

    if (boxPlayer.intersectsRect(boxCarrot))
    {
        carrot->removeFromParent();
        if (!carrotcollected)
        {
            cocos2d::AudioEngine::preload("audio/key.mp3");
            cocos2d::AudioEngine::play2d("audio/key.mp3", false, 0.3f);
            carrotcollected = true;
            count++;
            scoreLabel->setString(StringUtils::format("%d / 3", count));

        }


    }
    if (boxPlayer.intersectsRect(boxCarrot2))
    {
        carrot2->removeFromParent();
        if (!carrotcollected2)
        {
            cocos2d::AudioEngine::preload("audio/key.mp3");
            cocos2d::AudioEngine::play2d("audio/key.mp3", false, 0.3f);
            carrotcollected2 = true;
            count++;
            scoreLabel->setString(StringUtils::format("%d / 3", count));
        }

    }
    //if player intersect with carrot
    if (boxPlayer.intersectsRect(boxCarrot1))
    {
        //remove carrot
        carrot1->removeFromParent();
        //if carrot not collided with the player before
        if (!carrotcollected1)
        {
            //play a sound of eating carrot
            cocos2d::AudioEngine::preload("audio/key.mp3");
            cocos2d::AudioEngine::play2d("audio/key.mp3", false, 0.3f);
            //turn the bool to true so the count can add just 1
            carrotcollected1 = true;
            // count ++
            count++;
            // update the score
            scoreLabel->setString(StringUtils::format("%d / 3", count));

        }

    }


    //condition to turn left and right
    if (ifLeftpressed)
    {
        player->turnLeft(180);
    }
    else if (ifRightpressed)
    {
        player->turnRight();

    }

    //-------------------------------------------------condition of winning------------------------------------------------------
    //if count ==3 move to the next level
    if (count == 3) {
        auto scene = Lvl2::createScene();
        Director::getInstance()->replaceScene(scene);
    }


}

// method of mouvement
//method to move right
void MyWorld::moveright(float dt) {
    //get the position od the player
    Vec2 playerPos = player->getposition();
    //set position of the player to his initial pos + 80
    player->setposition(playerPos.x + 80 * dt, playerPos.y);
}
void MyWorld::moveleft(float dt) {
    Vec2 ballpos = player->getposition();
    player->setposition(ballpos.x - 80 * dt, ballpos.y);
}
void MyWorld::movetop(float dt) {
    Vec2 ballpos = player->getposition();
    player->setposition(ballpos.x, ballpos.y + 160 * dt);
}
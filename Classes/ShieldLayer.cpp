#include "LayerManager/MainScene.h"
#include "GameStat.h"
#include "ShieldLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
// on "init" you need to initialize your instance
bool ShieldLayer::init(int type)
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto listener = EventListenerTouchOneByOne::create();//加入触摸监听器
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(ShieldLayer::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto layercol = LayerColor::create(Color4B(0,0,0,130), visibleSize.width, visibleSize.height);
    //layercol->setPosition(visibleSize.width/2,visibleSize.height/2);
    //this->addChild(layercol,1);
    
    return true;
}
bool ShieldLayer::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){
    if(touch->getLocation().y<196){
        log("屏蔽层y坐标为%f",touch->getLocation().y);
        return false;
    }
    return true;
}


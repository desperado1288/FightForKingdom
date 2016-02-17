#include "LayerManager/MainScene.h"
#include "HeroList/TeamListLayer.h"
#include "NewUser/LoginScene.h"
#include "NewUser/SelectZhujiaoLayer.h"
#include "PopupLayer.h"
#include <ui/CocosGUI.h>
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
// on "init" you need to initialize your instance
bool SelectZhujiaoLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("evolution.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("fonts/newBmpFont.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("commonBtn.plist");
    auto listener = EventListenerTouchOneByOne::create();//加入触摸监听器
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(SelectZhujiaoLayer::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(SelectZhujiaoLayer::onTouchEnded, this);

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    
    
    //进入选主角界面
    string zhujiao1 = "zhujiao1";
    string zhujiao2 = "zhujiao2";
    string zhujiao3 = "zhujiao3";
    
    auto hero1 = Sprite::create(herobodypath(zhujiao1));
    auto hero2 = Sprite::create(herobodypath(zhujiao2));
    auto hero3 = Sprite::create(herobodypath(zhujiao3));
    hero1->setPosition(visibleSize.width/3-50,visibleSize.height*2/3-50);
    hero2->setPosition(visibleSize.width/3*2+50,visibleSize.height*2/3-50);
    hero3->setPosition(visibleSize.width/2,visibleSize.height*2/3-hero1->getContentSize().height-60);
    this->addChild(hero1,3,ZHUJIAO_TAG);
    this->addChild(hero2,3,ZHUJIAO_TAG+1);
    this->addChild(hero3,3,ZHUJIAO_TAG+2);
    
    hero1->setScale(2);
    hero2->setScale(2);
    hero3->setScale(2);
    hero2->setOpacity(0);
    hero3->setOpacity(0);
    hero1->runAction(EaseBounceIn::create(ScaleTo::create(0.5f, 0.8f)));
    hero2->runAction(Sequence::create(DelayTime::create(0.5f),FadeIn::create(0.01f),EaseBounceIn::create(ScaleTo::create(0.5f, 0.8f)),NULL));
    hero3->runAction(Sequence::create(DelayTime::create(1.0f),FadeIn::create(0.01f),EaseBounceIn::create(ScaleTo::create(0.5f, 0.8f)),CallFuncN::create(CC_CALLBACK_1(SelectZhujiaoLayer::showWenzi,this)),NULL));
    selected_index = 5;
    
    //用于攻城战弹出框
    return true;
}
void SelectZhujiaoLayer::showWenzi(Ref* pSender){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    string zhujiao1 = "zhujiao1";
    
    auto hero1 = Sprite::create(herobodypath(zhujiao1));
    
    auto typelabel1 = Label::createWithTTF("战神", "字体/瘦金体.ttf", 35);
    auto label1 = Label::createWithTTF("高暴击，高血防", "字体/瘦金体.ttf", 30);
    auto typelabel2 = Label::createWithTTF("游侠", "字体/瘦金体.ttf", 35);
    //typelabel2->enableOutline(Color4B::ORANGE,1);
    auto label2 = Label::createWithTTF("高敏捷，攻击范围大", "字体/瘦金体.ttf", 30);
    auto typelabel3 = Label::createWithTTF("鬼才", "字体/瘦金体.ttf", 35);
    auto label3 = Label::createWithTTF("高谋略，高暴击，全体伤害", "字体/瘦金体.ttf", 30);
    typelabel1->setPosition(visibleSize.width/3-50,visibleSize.height*2/3-50-hero1->getContentSize().height/2);
    label1->setPosition(visibleSize.width/3-50,visibleSize.height*2/3-50-hero1->getContentSize().height/2-40);
    
    typelabel2->setPosition(visibleSize.width/3*2+50,visibleSize.height*2/3-50-hero1->getContentSize().height/2);
    label2->setPosition(visibleSize.width/3*2+50,visibleSize.height*2/3-50-hero1->getContentSize().height/2-40);
    typelabel3->setPosition(visibleSize.width/2,visibleSize.height*2/3-hero1->getContentSize().height*3/2-60);
    label3->setPosition(visibleSize.width/2,visibleSize.height*2/3-hero1->getContentSize().height*3/2-40-60);
    this->addChild(typelabel1,3);
    this->addChild(label1,3);
    this->addChild(typelabel2,3);
    this->addChild(label2,3);
    this->addChild(typelabel3,3);
    this->addChild(label3,3);
    typelabel1->setColor(Color3B::ORANGE);
    typelabel2->setColor(Color3B::ORANGE);
    typelabel3->setColor(Color3B::ORANGE);
    label1->setColor(Color3B::BLACK);
    label2->setColor(Color3B::BLACK);
    label3->setColor(Color3B::BLACK);

    //开始游戏按钮
    auto btnsp = Sprite::createWithSpriteFrameName("btn_main_normal.png");
    auto btn_begin = Scale9Sprite::createWithSpriteFrameName("btn_main_normal.png");
    auto btn_label = Label::createWithTTF("Start", "7=蒙纳漫画体.ttf", 30);
    auto btnbegin = ControlButton::create(btn_label,btn_begin);
    btnbegin->setPreferredSize(btnsp->getContentSize());
    btnbegin->setScaleRatio(0.85f);
    btnbegin->setPosition(visibleSize.width/3*2+50, visibleSize.height*2/3-hero1->getContentSize().height-60);
    this->addChild(btnbegin,3,TEXTFIELD_TAG+2);
    //btn_label->setPosition(btnbegin->getContentSize().width/2,btn_begin->getContentSize().height/2-10);
    btnbegin->addTargetWithActionForControlEvents(this, cccontrol_selector(SelectZhujiaoLayer::startGame), Control::EventType::TOUCH_UP_INSIDE);

}
void SelectZhujiaoLayer::startGame(Ref* pSender, Control::EventType event){
    if(selected_index==5){
        log("没选英雄");
        auto _popLayer = new PopupLayer();
        _popLayer->init(6);
        if(_popLayer){
            this->getParent()->addChild(_popLayer,6);
        }
    }
    else{
        //存用户服务器
        GameStat::getInstance()->userType = selected_index;
        char sql[32];
        sprintf(sql,"\"%s\",%d,1,1,1,0,0",GameStat::getInstance()->userName.c_str(),601+selected_index);
        in_newplayer_mode = 1;
        //用户第一个主角英雄存入数据库
        GameStat::getInstance()->saveToSQLite("User_hero",documentpath1.c_str(),sql);
        //开始游戏
        auto _MainScene = MainScene::create();
        if(_MainScene){
            GameStat::getInstance()->new_player_stage = 0;
            Director::getInstance()->replaceScene(_MainScene);
        }
    }
    
    
}
void SelectZhujiaoLayer::GoToMainScene(Ref* pSender, Control::EventType event){
    CCLOG("开始游戏");
    
}
bool SelectZhujiaoLayer::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){
    for(int i = 0;i<3;i++){
        if(this->getChildByTag(ZHUJIAO_TAG+i)->getBoundingBox().containsPoint(touch->getLocation())){
            /*if(selected_index==i){
                //为双击,开始游戏
                selected_index = 5;
                return true;
            }*/
            selected_index = i;//为0,1,2
            return true;
        }
    }
    return false;
}
void SelectZhujiaoLayer::onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event){
    /*if(selected_index==5){
        //正式进入游戏
        auto _MainScene = MainScene::create();
        if(_MainScene)
            Director::getInstance()->replaceScene(_MainScene);
    }*/
    this->removeChildByTag(SELECTFRAME_TAG);
    auto frame = Sprite::create("dialog/边框.png");
    frame->setPosition(this->getChildByTag(ZHUJIAO_TAG+selected_index)->getPositionX()+190,this->getChildByTag(ZHUJIAO_TAG+selected_index)->getPositionY());
    frame->setScale(1.2f);
    this->addChild(frame,3,SELECTFRAME_TAG);
    
}


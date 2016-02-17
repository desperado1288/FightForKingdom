#include "LayerManager/MainScene.h"
#include "GameStat.h"
#include "HeroList/TeamListLayer.h"
#include "HeroList/ZhenFa.h"
#include "Compete/CompeteScene.h"
#include <ui/CocosGUI.h>
#include "ModifiedSourceCode/DMenu.h"
#include "LayerManager/ZhengZhanLayer.h"
#include "cocos-ext.h"
#include "WholeMapLayer.h"
#include "ModifiedSourceCode/DControlButton.h"
#include "LayerManager/HelloWorldLayer.h"
#include "PopupLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
// on "init" you need to initialize your instance
bool PopupLayer::init(int type)
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    gongchengtype = type;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("evolution.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("fonts/newBmpFont.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("commonBtn.plist");
    auto listener = EventListenerTouchOneByOne::create();//加入触摸监听器
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(PopupLayer::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    
    auto layercol = LayerColor::create(Color4B(0,0,0,130), visibleSize.width, visibleSize.height);
    //layercol->setPosition(visibleSize.width/2,visibleSize.height/2);
    this->addChild(layercol,1);
    
    auto tanchusp = Sprite::create("bg_tanchukuang.png");
    tanchusp->setPosition(visibleSize.width/2,visibleSize.height/2);
    this->addChild(tanchusp,2);
    
    
    //用于攻城战弹出框
    if(type==0||type==1||type==2){
    auto fanhui = Sprite::createWithSpriteFrameName("btn_main_normal.png");
    auto sprite9normal = Scale9Sprite::createWithSpriteFrameName("btn_main_normal.png");
    //auto sprite9label = Scale9Sprite::createWithSpriteFrameName("icon_anniuwenzi_fanhui.png");
    auto sprite9label = Sprite::createWithSpriteFrameName("icon_anniuwenzi_fanhui.png");
    auto btn = DControlButton::create(sprite9normal);
    //auto btnLabel = ControlButton::create(sprite9label);
    auto sprite9selected = Scale9Sprite::createWithSpriteFrameName("btn_buy_normal.png");
    //btn->addChild(btnLabel,1);
    btn->addChild(sprite9label,2);
    this->addChild(btn,3,999);
    btn->setScaleRatio(0.85f);
    btn->setPreferredSize(fanhui->getContentSize());
    btn->setPosition(visibleSize.width/2+110,visibleSize.height/2-50);
    sprite9label->setPosition(btn->getContentSize().width/2, btn->getContentSize().height/2);
    btn->addTargetWithActionForControlEvents(this, cccontrol_selector(PopupLayer::back), Control::EventType::TOUCH_UP_INSIDE);
    btn->setBackgroundSpriteForState(sprite9selected, Control::State::HIGH_LIGHTED);

    
    auto comfirmsp = Scale9Sprite::createWithSpriteFrameName("btn_main_normal.png");
    //auto sprite9label = Scale9Sprite::createWithSpriteFrameName("icon_anniuwenzi_fanhui.png");
    auto comfirmlabel = Sprite::createWithSpriteFrameName("icon_anniuwenzi_querenxiao.png");
    auto comfirmbtn = DControlButton::create(comfirmsp);
    //auto btnLabel = ControlButton::create(sprite9label);
    auto comfirm_selected = Scale9Sprite::createWithSpriteFrameName("btn_buy_normal.png");
    //btn->addChild(btnLabel,1);
    comfirmbtn->addChild(comfirmlabel,2);
    this->addChild(comfirmbtn,3,998);
    comfirmbtn->setScaleRatio(0.85f);
    comfirmbtn->setPreferredSize(fanhui->getContentSize());
    comfirmbtn->setPosition(visibleSize.width/2-110,visibleSize.height/2-50);
    comfirmlabel->setPosition(comfirmbtn->getContentSize().width/2, comfirmbtn->getContentSize().height/2);
    comfirmbtn->addTargetWithActionForControlEvents(this, cccontrol_selector(PopupLayer::GoToQiangduo), Control::EventType::TOUCH_UP_INSIDE);
    comfirmbtn->setBackgroundSpriteForState(comfirm_selected, Control::State::HIGH_LIGHTED);

    
    auto des_wenzi = Label::createWithSystemFont("", "宋体", 30);
    if(type == 2){
        des_wenzi->setString("策略遭到克制,对我军不利,是否继续攻城:");
        des_wenzi->setColor(Color3B::RED);
    }
    else if(type==1){
        des_wenzi->setString("策略克制对手,我军必能凯旋，是否继续攻城:");
        des_wenzi->setColor(Color3B::ORANGE);
    }
    else if(type == 0){
        des_wenzi->setString("您还没有选择攻城策略，请先选择");
        des_wenzi->setColor(Color3B::MAGENTA);
        ((DControlButton*)this->getChildByTag(998))->setEnabled(false);
    }
    des_wenzi->setDimensions(tanchusp->getContentSize().width-80, 80);
    des_wenzi->setPosition(visibleSize.width/2,visibleSize.height/2+60);
    this->addChild(des_wenzi,3,101);
    //TO DO...返回主页
    }
    //用于登陆弹出信息
    else if(type==5||type==6){
        auto guanbi_pic = Sprite::createWithSpriteFrameName("icon_dianjiguanbi.png");
        guanbi_pic->setPosition(visibleSize.width/2,visibleSize.height/2-tanchusp->getContentSize().height/2);
        this->addChild(guanbi_pic,4);
        auto wenzi = Label::createWithTTF("", "7=蒙纳漫画体.ttf", 30);
        if(type==5)
            wenzi->setString("Username or password is blank");
        else if(type==6)
            wenzi->setString("Please select a hero");
        wenzi->setPosition(visibleSize.width/2,visibleSize.height/2+20);
        this->addChild(wenzi,3);
        wenzi->setColor(Color3B::ORANGE);

    }
    return true;
}
void PopupLayer::GoToQiangduo(Ref* pSender, Control::EventType event){
    CCLOG("开始抢夺");
    //策略克制
    if(gongchengtype==1){
        for (int i=0; i<GameStat::getInstance()->userCompeteMap.size(); i++) {
            GameStat::getInstance()->userCompeteMap.at(i).attackDisplay*=1.2f;
        }
    }
    else if(gongchengtype==2){
        for (int i=0; i<GameStat::getInstance()->userCompeteMap.size(); i++) {
            GameStat::getInstance()->userCompeteMap.at(i).attackDisplay*=0.8f;
        }
    }
    GameStat::getInstance()->exp = 200;
    GameStat::getInstance()->money = 300;
    GameStat::getInstance()->shengwang = 455;

    GameStat::getInstance()->isGongcheng = true;
    GameStat::getInstance()->isUnlockSkill = false;

    auto _CompeteScene = CompeteScene::create();
    if(_CompeteScene)
        Director::getInstance()->pushScene(_CompeteScene);//替换场景，战斗结束再回来
    this->removeFromParentAndCleanup(true);
}
void PopupLayer::back(Ref* pSender, Control::EventType event){

    //TO DO....恢复上一层的touch
    this->removeFromParentAndCleanup(true);
}
bool PopupLayer::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){
    if(gongchengtype==5||gongchengtype==6){
        removeFromParentAndCleanup(true);
    }
    return true;
}


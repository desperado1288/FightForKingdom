#include "LayerManager/MainScene.h"
#include "GameStat.h"
#include "WholeMapLayer.h"
#include "Compete/CompeteScene.h"
#include "HeroList/TeamListLayer.h"
#include "ModifiedSourceCode/DMenu.h"
#include "PopupLayer.h"
#include <ui/CocosGUI.h>
#include "cocos-ext.h"
#include "ModifiedSourceCode/DControlButton.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace cocos2d::extension;
// on "init" you need to initialize your instance

bool WholeMapLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("commonBtn.plist");

    auto listener = EventListenerTouchOneByOne::create();//加入触摸监听器
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(WholeMapLayer::onTouchsBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(WholeMapLayer::onTouchsMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(WholeMapLayer::onTouchsEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    

    auto bg = Sprite::create("bg_cuju.jpg");
    bg->setPosition(visibleSize.width/2,visibleSize.height/2);
    this->addChild(bg,1);
    CCLOG("进入大地图");
    auto cheng1 = MenuItemImage::create("chengchi/城池0.png","chengchi/城池0.png",CC_CALLBACK_1(WholeMapLayer::tanchukuang,this));
    
    auto cheng2 = MenuItemImage::create("chengchi/城池0.png","chengchi/城池0.png",CC_CALLBACK_1(WholeMapLayer::tanchukuang,this));
    cheng1->setPosition(visibleSize.width/3,visibleSize.height/2);
    cheng2->setPosition(visibleSize.width*2/3,visibleSize.height/2);
    auto menu = Menu::create(cheng1,cheng2,NULL);
    this->addChild(menu,2);
    menu->setPosition(Vec2::ZERO);
    auto name1 = Label::createWithSystemFont("用户:John\n坚守城池", "宋体", 30);
    name1->setColor(Color3B::BLACK);
    auto name2 = Label::createWithSystemFont("用户:李归帆\n  出城迎敌", "宋体", 30);
    name2->setColor(Color3B::BLACK);
    name1->setPosition(visibleSize.width/3,visibleSize.height/2+cheng1->getContentSize().height/2+20);
    name2->setPosition(visibleSize.width/3*2,visibleSize.height/2+cheng1->getContentSize().height/2+20);
    this->addChild(name1,2);
    this->addChild(name2,2);
    cheng1->setTag(20);
    cheng2->setTag(21);

    //围城  策略
    auto strategybtn = Sprite::createWithSpriteFrameName("btn_qq.png");
    auto strategy1sp = Scale9Sprite::createWithSpriteFrameName("btn_qq.png");
    auto strategy1label = Label::createWithSystemFont("围城", "宋体", 25);
    strategy1label->setColor(Color3B::BLACK);
    auto strategy1btn = DControlButton::create(strategy1sp);
    auto strategy1_selected = Scale9Sprite::createWithSpriteFrameName("btn_wx.png");
    
    strategy1btn->addChild(strategy1label,2);
    this->addChild(strategy1btn,2,991);
    strategy1btn->setScaleRatio(0.9f);
    strategy1btn->setPreferredSize(strategybtn->getContentSize());
    strategy1btn->setPosition(visibleSize.width/4,visibleSize.height/2-140);
    strategy1label->setPosition(strategy1btn->getContentSize().width/2, strategy1btn->getContentSize().height/2);
    strategy1btn->addTargetWithActionForControlEvents(this, cccontrol_selector(WholeMapLayer::selectStrategy), Control::EventType::TOUCH_UP_INSIDE);
    strategy1btn->setBackgroundSpriteForState(strategy1_selected, Control::State::HIGH_LIGHTED);

    
    auto strategy2sp = Scale9Sprite::createWithSpriteFrameName("btn_qq.png");
    auto strategy2label = Label::createWithSystemFont("全力攻城", "宋体", 25);
    strategy2label->setColor(Color3B::BLACK);
    auto strategy2btn = DControlButton::create(strategy2sp);
    auto strategy2_selected = Scale9Sprite::createWithSpriteFrameName("btn_wx.png");
    
    strategy2btn->addChild(strategy2label,2);
    this->addChild(strategy2btn,2,992);
    strategy2btn->setScaleRatio(0.85f);
    strategy2btn->setPreferredSize(strategybtn->getContentSize());
    strategy2btn->setPosition(visibleSize.width/4,visibleSize.height/2-230);
    strategy2label->setPosition(strategy2btn->getContentSize().width/2, strategy2btn->getContentSize().height/2);
    strategy2btn->addTargetWithActionForControlEvents(this, cccontrol_selector(WholeMapLayer::selectStrategy), Control::EventType::TOUCH_UP_INSIDE);
    strategy2btn->setBackgroundSpriteForState(strategy2_selected, Control::State::HIGH_LIGHTED);

    
    strategy_type = 0;
    return 0;
}
void WholeMapLayer::tanchukuang(Ref* pSender){
    int tag = ((MenuItem*)pSender)->getTag();
    CCLOG("点中第%d个城池",tag);
    int type;
    if((tag==20&&strategy_type==1)||(tag==21&&strategy_type==2))//用户获胜
        type = 1;
    else if((tag==20&&strategy_type==2)||(tag==21&&strategy_type==1))
        type = 2;
    else if(strategy_type==0)
        type = 0;//没选策略
    auto _PopupLayer = new PopupLayer();
    _PopupLayer->init(type);
    if(_PopupLayer){
        this->getParent()->addChild(_PopupLayer,5,798);
    }
    
    
}
void WholeMapLayer::selectStrategy(Ref* pSender, Control::EventType event){
    ((DControlButton*)this->getChildByTag(991))->setHighlighted(false);
    ((DControlButton*)this->getChildByTag(992))->setHighlighted(false);
    int tag = ((DControlButton*)pSender)->getTag();
    CCLOG("选择策略为%d",tag);
    ((DControlButton*)this->getChildByTag(tag))->setHighlighted(true);
    //((DControlButton*)this->getChildByTag(tag))->setScale(1.25f);
    strategy_type = tag-990;
    
}
void WholeMapLayer::GoToGongcheng(Ref* pSender){
    GameStat::getInstance()->isGongcheng = true;
    auto _CompeteScene = CompeteScene::create();
    if(_CompeteScene)
        Director::getInstance()->pushScene(_CompeteScene);//替换场景，战斗结束再回来

}

bool WholeMapLayer::onTouchsBegan(cocos2d::Touch* touch,cocos2d::Event* event){
    return false;
}
void WholeMapLayer::onTouchsMoved(cocos2d::Touch* touch,cocos2d::Event* event){
    
}
void WholeMapLayer::onTouchsEnded(cocos2d::Touch* touch,cocos2d::Event* event){
    
}


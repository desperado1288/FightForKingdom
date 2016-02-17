#include "LayerManager/MainScene.h"
#include "GameStat.h"
#include "Compete/CompeteScene.h"
#include "Compete/CompeteLayer.h"

#include "HeroList/ZhenFa.h"
#include "HeroList/TeamListLayer.h"
#include "NewUser/NewPlayerLayer.h"
#include "ModifiedSourceCode/DMenu.h"
#include "ModifiedSourceCode/DControlButton.h"
#include "PopNewHero.h"

#include "cocos-ext.h"
#include <ui/CocosGUI.h>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
// on "init" you need to initialize your instance
bool PopNewHero::init(int type)
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
    auto listener = EventListenerTouchOneByOne::create();//加入触摸监听器
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(PopNewHero::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    
    auto layercol = LayerColor::create(Color4B(0,0,0,130), visibleSize.width, visibleSize.height);
    //layercol->setPosition(visibleSize.width/2,visibleSize.height/2);
    this->addChild(layercol,1);
    
    auto tanchusp = Sprite::createWithSpriteFrameName("bg_qiancengta_gold.png");
    tanchusp->setPosition(visibleSize.width/2,visibleSize.height/2);
    this->addChild(tanchusp,2);
    
    auto title = Label::createWithTTF("恭喜您获得", "字体/华文新魏.TTF", 50);
    title->setColor(Color3B::YELLOW);
    title->setPosition(visibleSize.width/2,visibleSize.height/2+sizeY/2+50);
    this->addChild(title,3);

    //新英雄
    char sql[1024];
    sprintf(sql,"select * from Heros where number = %d",GameStat::getInstance()->heroIndex[0]);
    GameStat::getInstance()->getTotalHeroIndex("Heros", documentpath2.c_str(),sql);//

    auto herosp = Sprite::create(herobodypath(randomhero.engname));
    herosp->setPosition(visibleSize.width/2,visibleSize.height/2);
    herosp->setScale(0.69f);
    this->addChild(herosp,3);
    auto heroname = Label::createWithTTF(randomhero.name, "字体/华文新魏.TTF", 30);
    heroname->setPosition(visibleSize.width/2,visibleSize.height/2-tanchusp->getContentSize().height/2-40);
    this->addChild(heroname,3);
    if(randomhero.quality==1)
        heroname->setColor(Color3B::MAGENTA);
    else if(randomhero.quality==2)
        heroname->setColor(Color3B::BLUE);
    else if(randomhero.quality==3)
        heroname->setColor(Color3B::GREEN);
    else if(randomhero.quality==0)
        heroname->setColor(Color3B::ORANGE);
    

    
    return true;
}
void PopNewHero::back(Ref* pSender, Control::EventType event){

    //TO DO....恢复上一层的touch
    this->removeFromParentAndCleanup(true);
}
bool PopNewHero::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){
    if(new_stage==3){
        auto _newPlayerLayer = new NewPlayerLayer();
        _newPlayerLayer->init(0);
        this->getParent()->addChild(_newPlayerLayer,4,NEWPLAYER_LAYER_TAG);
    }
    removeFromParentAndCleanup(true);
    
    return true;
}


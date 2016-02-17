#include "LayerManager/MainScene.h"
#include "LayerManager/ZhenRongLayer.h"
#include "HeroList/HeroInfoLayer.h"
#include "HeroList/TeamListLayer.h"
#include "Compete/CompeteScene.h"

#include "ModifiedSourceCode/DMenu.h"
#include "NewUser/NewPlayerLayer.h"
#include <ui/CocosGUI.h>
#include "cocos-ext.h"
#include "GameStat.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace cocos2d::extension;
// on "init" you need to initialize your instance

bool HeroInfoLayer::init(int type,int index)
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    team_type = type;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto bg = Sprite::create("bg/card_info_bg_1.jpg");
    bg->setPosition(visibleSize.width/2,visibleSize.height/2);
    this->addChild(bg,1);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("commonBtn.plist");
    auto closeLayerSp = Sprite::createWithSpriteFrameName("btn_blue_normal.png");
    //((Menu*)this->getParent()->getChildByTag(MAIN_MENU_TAG))->pause();//暂停主界面中的菜单
    
    auto psc9normal = Scale9Sprite::createWithSpriteFrameName("btn_blue_normal.png");
    auto label = Label::createWithTTF("关 闭", "字体/华文新魏.TTF", 30);
    
    auto psc9selected = Scale9Sprite::createWithSpriteFrameName("btn_blue_disable.png");
    auto button = ControlButton::create(label, psc9normal);
    button->setPosition(Point(visibleSize.width/2, 88+closeLayerSp->getContentSize().height));
    button->setPreferredSize(closeLayerSp->getContentSize());
    button->setBackgroundSpriteForState(psc9selected, Control::State::SELECTED);//TO DO....不知道这句有什么用.怎么才能换图片selected
    //button->setTitleColorForState(Color3B(255,255,255), Control::State::HIGH_LIGHTED);
    button->addTargetWithActionForControlEvents(this, cccontrol_selector(HeroInfoLayer::buttonCallBack), Control::EventType::TOUCH_DOWN);
    button->addTargetWithActionForControlEvents(this, cccontrol_selector(HeroInfoLayer::buttonTouchEnded), Control::EventType::TOUCH_UP_INSIDE);
    //button->setSelected(true);
    this->addChild(button, 1);
    button->setScaleRatio(0.8);

    //TO DO....
    auto listener = EventListenerTouchOneByOne::create();//加入触摸监听器
    listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HeroInfoLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HeroInfoLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HeroInfoLayer::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    
    
    //英雄详细信息加入
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("fonts/newBmpFont.plist");
    HeroType hero1;
    if(type==1)
        hero1 = GameStat::getInstance()->userTeam[index];
    else if(type==2){
        hero1 = GameStat::getInstance()->userCompeteMap[index];

    }
    GameStat::getInstance()->passive_unlock_battle_hero = hero1;
    string heropath = herobodypath(hero1.engname);
    auto herosp = Sprite::create(heropath);
    herosp->setPosition(visibleSize.width/2+15,visibleSize.height-88-herosp->getContentSize().height/2-40);
    this->addChild(herosp,2);
    
    auto heroname = Label::createWithTTF(hero1.name, "字体/华文新魏.TTF", 30);
    heroname->setColor(Color3B::MAGENTA);
    heroname->setPosition(visibleSize.width/2,visibleSize.height-88-herosp->getContentSize().height-40-25);
    this->addChild(heroname,2);
    //等级
    char label1[32];

    auto level_label = Label::createWithTTF("等级：", "字体/华文新魏.TTF", 30);
    sprintf(label1,"%d级",hero1.level);
    auto level = Label::createWithTTF(label1, "字体/华文新魏.TTF", 30);
    level_label->setPosition(visibleSize.width/6,visibleSize.height-88-herosp->getContentSize().height-40-25);
    level->setPosition(level_label->getPositionX()+level_label->getContentSize().width/2+level->getContentSize().width/2+15,level_label->getPositionY());
    level_label->setColor(Color3B::ORANGE);
    level->setColor(Color3B::GREEN);
    this->addChild(level,2);
    this->addChild(level_label,2);
    
    //攻击力、血量
    auto attack = Sprite::createWithSpriteFrameName("icon_wenzi_gongjili.png");
    auto health = Sprite::createWithSpriteFrameName("icon_wenzi_xueliangzhi.png");
    auto agility = Label::createWithTTF("敏捷", "字体/华文新魏.TTF", 30);
    auto critic = Label::createWithTTF("暴击", "字体/华文新魏.TTF", 30);
    auto defense = Label::createWithTTF("防御", "字体/华文新魏.TTF", 30);

    attack->setPosition(visibleSize.width/4,visibleSize.height-88-herosp->getContentSize().height-40-25-50);
    health->setPosition(visibleSize.width/4,visibleSize.height-88-herosp->getContentSize().height-40-25-50-60);
    agility->setPosition(visibleSize.width/4*3-30,visibleSize.height-88-herosp->getContentSize().height-40-25-50+5);
    critic->setPosition(visibleSize.width/4*3-30,visibleSize.height-88-herosp->getContentSize().height-40-25-50-30);
    defense->setPosition(visibleSize.width/4*3-30,visibleSize.height-88-herosp->getContentSize().height-40-25-50-65);
    this->addChild(attack,2);
    this->addChild(health,2);
    this->addChild(agility,2);
    this->addChild(critic,2);
    this->addChild(defense,2);

    auto attack_label = LabelAtlas::create("0123456789", "fonts/number_yellow.png",49,88,'0');
    auto health_label = LabelAtlas::create("0123456789", "fonts/number_yellow.png",49,88,'0');
    auto agility_label = LabelAtlas::create("0123456789", "fonts/number_yellow.png",49,88,'0');
    auto critic_label = LabelAtlas::create("0123456789", "fonts/number_yellow.png",49,88,'0');
    auto defense_label = LabelAtlas::create("0123456789", "fonts/number_yellow.png",49,88,'0');
    this->addChild(attack_label,2);
    this->addChild(health_label,2);
    this->addChild(agility_label,2);
    this->addChild(critic_label,2);
    this->addChild(defense_label,2);
    attack_label->setScale(0.45f);
    health_label->setScale(0.45f);
    agility_label->setScale(0.45f);
    critic_label->setScale(0.45f);
    defense_label->setScale(0.45f);

    sprintf(label1, "%d",(int)hero1.attackDisplay);
    attack_label->setString(label1);
    sprintf(label1, "%d",(int)hero1.healthDisplay);
    health_label->setString(label1);
    sprintf(label1, "%d",(int)hero1.agilityDisplay);
    agility_label->setString(label1);
    sprintf(label1, "%d",(int)hero1.criticDisplay);
    critic_label->setString(label1);
    sprintf(label1, "%d",(int)hero1.defenseDisplay);
    defense_label->setString(label1);
    attack_label->setPosition(visibleSize.width/4+attack->getContentSize().width-20,visibleSize.height-88-herosp->getContentSize().height-40-25-50-20);
    health_label->setPosition(visibleSize.width/4+health->getContentSize().width-20,visibleSize.height-88-herosp->getContentSize().height-40-25-50-60-20);
    agility_label->setPosition(visibleSize.width/4*3-30+agility->getContentSize().width-20,visibleSize.height-88-herosp->getContentSize().height-40-25-50+5-20);
    critic_label->setPosition(visibleSize.width/4*3-30+critic->getContentSize().width-20,visibleSize.height-88-herosp->getContentSize().height-40-25-50-30-20);
    defense_label->setPosition(visibleSize.width/4*3-30+defense->getContentSize().width-20,visibleSize.height-88-herosp->getContentSize().height-40-25-50-65-20);
//滚动页面
    auto scrollList = ui::ScrollView::create();
    scrollList->setTouchEnabled(true);
    scrollList->setSwallowTouches(false);
    scrollList->setBounceEnabled(true);
    scrollList->setSize(Size(560,visibleSize.height/3-60));
    scrollList->setAnchorPoint(Vec2(0.5,0.5));
    scrollList->setPosition(Point(visibleSize.width/2,visibleSize.height/3));
    scrollList->setInnerContainerSize(Size(560,visibleSize.height*2/3));//TO DO...inner size
    CCLOG("team size is : %lu",team.size());
    this->addChild(scrollList,2,SCROLL_LIST_TAG);
//技能
    auto skillsp = Sprite::createWithSpriteFrameName("icon_wenzi_jineng.png");
    //skillsp->setPosition(visibleSize.width/4-attack->getContentSize().width,visibleSize.height-88-herosp->getContentSize().height-40-25-50-60-50);
    skillsp->setPosition(skillsp->getContentSize().width+20,scrollList->getInnerContainerSize().height-skillsp->getContentSize().height/2-10);
    scrollList->addChild(skillsp,2);
    auto skillname = Label::createWithTTF(hero1.active_skill_name, "字体/华文新魏.TTF", 30);
    skillname->setColor(Color3B::ORANGE);
    skillname->enableOutline(Color4B::BLACK,1);
    skillname->enableShadow();
    //skillname->setPosition(visibleSize.width/4+skillsp->getContentSize().width/2+skillname->getContentSize().width/2+20-attack->getContentSize().width,visibleSize.height-88-herosp->getContentSize().height-40-25-50-60-50);
    skillname->setPosition(skillsp->getPositionX()+skillsp->getContentSize().width/2+skillname->getContentSize().width/2+20,skillsp->getPositionY());
    scrollList->addChild(skillname,2);
    auto skilldes = Label::createWithTTF(hero1.active_skill_describe, "字体/华文新魏.TTF", 20);
    //skilldes->setPosition(visibleSize.width/2,visibleSize.height-88-herosp->getContentSize().height-40-25-50-60-50-45-30);
    if(skilldes->getContentSize().width>=1000)
        skilldes->setDimensions(500, 70);
    else
        skilldes->setDimensions(500, 50);
    
    skilldes->setPosition(scrollList->getInnerContainerSize().width/2,scrollList->getInnerContainerSize().height-skillname->getContentSize().height-skilldes->getDimensions().height/2-20);
    scrollList->addChild(skilldes,2);

    
    //被动技能
    auto passkillsp = Label::createWithTTF("被动技", "字体/华文新魏.TTF", 25);
    //passkillsp->setPosition(visibleSize.width/4-attack->getContentSize().width,visibleSize.height-88-herosp->getContentSize().height-40-25-50-60-50-100);
    passkillsp->setPosition(skillsp->getPositionX(),skilldes->getPositionY()-skilldes->getDimensions().height/2-passkillsp->getContentSize().height/2-20);
    scrollList->addChild(passkillsp,2);
    auto passkillname = Label::createWithTTF(hero1.passive_skill_name, "字体/华文新魏.TTF", 30);
    passkillname->setColor(Color3B::ORANGE);
    passkillname->enableOutline(Color4B::BLACK,1);
    passkillname->enableShadow();

    //passkillname->setPosition(visibleSize.width/4+passkillsp->getContentSize().width/2+passkillname->getContentSize().width/2+20-attack->getContentSize().width,visibleSize.height-88-herosp->getContentSize().height-40-25-50-60-50-100);
    passkillname->setPosition(passkillsp->getPositionX()+passkillsp->getContentSize().width/2+passkillname->getContentSize().width/2+20,passkillsp->getPositionY());
    scrollList->addChild(passkillname,2);
    auto passkilldes = Label::createWithTTF(hero1.passive_skill_describe, "字体/华文新魏.TTF", 20);
    if(passkilldes->getContentSize().width>=1000)
        passkilldes->setDimensions(500, 70);
    else
        passkilldes->setDimensions(500, 50);
    
    passkilldes->setPosition(scrollList->getInnerContainerSize().width/2,passkillsp->getPositionY()-passkillsp->getContentSize().height/2-passkilldes->getDimensions().height/2-30);
    scrollList->addChild(passkilldes,2);
    

    //被动技能是否解锁
    auto has_pass = Label::createWithTTF("", "字体/华文新魏.TTF", 30);
    if(hero1.isPassiveActivated){
        has_pass->setString("已解锁");
        has_pass->setColor(Color3B::ORANGE);
    }
    else{
        has_pass->setString("未解锁");
        has_pass->setColor(Color3B::RED);
    }
    //has_pass->setPosition(passkillname->getPositionX()+passkillname->getContentSize().width/2+has_pass->getContentSize().width/2+20,passkillname->getPositionY());
    has_pass->setPosition(passkillname->getPositionX()+passkillname->getContentSize().width/2+has_pass->getContentSize().width/2+30,passkillname->getPositionY());
    scrollList->addChild(has_pass,2,HAS_PASS_TAG);
    
//解锁按钮
    auto unlocksp = Sprite::createWithSpriteFrameName("btn_lingxing_normal.png");
    auto unlocksprite = Scale9Sprite::createWithSpriteFrameName("btn_lingxing_normal.png");
    auto unlocklabel = Label::createWithTTF("解锁", "字体/华文新魏.TTF", 30);
    auto unlockbutton = ControlButton::create(unlocklabel, unlocksprite);
    //unlockbutton->setPosition(has_pass->getPositionX()+has_pass->getContentSize().width/2+unlocksp->getContentSize().width/2+20,passkillname->getPositionY());
    unlockbutton->setPreferredSize(unlocksp->getContentSize());
    unlockbutton->setPosition(has_pass->getPositionX()+has_pass->getContentSize().width/2+unlockbutton->getContentSize().width/2+30,passkillname->getPositionY());
    //button->setTitleColorForState(Color3B(255,255,255), Control::State::HIGH_LIGHTED);
    unlockbutton->addTargetWithActionForControlEvents(this, cccontrol_selector(HeroInfoLayer::buttonCallBack), Control::EventType::TOUCH_DOWN);
    unlockbutton->addTargetWithActionForControlEvents(this, cccontrol_selector(HeroInfoLayer::unlock_passskill), Control::EventType::TOUCH_UP_INSIDE);
    //button->setSelected(true);
    scrollList->addChild(unlockbutton, 1);
    unlockbutton->setScaleRatio(0.9f);
//------------------------
    
//缘分---------------------
    auto relationsp = Label::createWithTTF("天命", "字体/华文新魏.TTF", 30);
    relationsp->setColor(Color3B::ORANGE);
    //relationsp->setPosition(visibleSize.width/4-attack->getContentSize().width,visibleSize.height-88-herosp->getContentSize().height-40-25-50-60-50-100-100);
    relationsp->setPosition(skillsp->getPositionX(),passkilldes->getPositionY()-passkilldes->getDimensions().height/2-relationsp->getContentSize().height/2-30);
    scrollList->addChild(relationsp,2);
    
    for (int i = 0; i<hero1.relation.size(); ++i) {
        auto relation1 = Label::createWithTTF(hero1.relation[i].relationDescribe, "字体/华文新魏.TTF", 21);
        //relation1->setPosition(visibleSize.width/2,relationsp->getPositionY()-50*(i+1));
        relation1->setDimensions(500, 50);
        relation1->setPosition(scrollList->getInnerContainerSize().width/2,relationsp->getPositionY()-relationsp->getContentSize().height/2-(relation1->getDimensions().height/2+20)*(i+1));
        scrollList->addChild(relation1,2);
        //TO DO....实现缘分变成金色
        //relation1->setColor(Color3B::ORANGE);
        relation1->setDimensions(500, 50);
        relation1->setAlignment(TextHAlignment::LEFT);
    }
    
//-------------------------
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(HeroInfoLayer::change_unlock_label), "change_unlock", NULL);
//英雄小传
    if(new_stage==1){
        auto _newPlayerLayer = new NewPlayerLayer();
        _newPlayerLayer->init(5);
        this->addChild(_newPlayerLayer,4,NEWPLAYER_LAYER_TAG);//不知道为什么不能getParent()->add
    }
    
    return true;
}
void HeroInfoLayer::unlock_passskill(Ref* pSender, Control::EventType event){
    GameStat::getInstance()->isUnlockSkill = 1;
    GameStat::getInstance()->isGongcheng = false;
    GameStat::getInstance()->exp = 0;
    GameStat::getInstance()->money = 300;
    GameStat::getInstance()->shengwang = 300;
    
    auto _CompeteScene = CompeteScene::create();
    if(_CompeteScene)
        Director::getInstance()->pushScene(_CompeteScene);//替换场景，战斗结束再回来
}
void HeroInfoLayer::change_unlock_label(Ref* pSender){
    ((Label*)this->getChildByTag(SCROLL_LIST_TAG)->getChildByTag(HAS_PASS_TAG))->setString("已解锁");
    ((Label*)this->getChildByTag(SCROLL_LIST_TAG)->getChildByTag(HAS_PASS_TAG))->setColor(Color3B::GREEN);

}
void HeroInfoLayer::buttonCallBack(Ref* pSender, Control::EventType event) {
    CCLOG("Touch the Button");
}
void HeroInfoLayer::buttonTouchEnded(Ref* pSender, Control::EventType event) {
    CCLOG("touch ended button");
    CCLOG("返回上一层");
    //TO DO....恢复上一层按钮点击
    if (team_type==1) {
        this->getParent()->getChildByTag(TEAMLIST_LAYER_TAG)->getChildByTag(SCROLL_LIST_TAG)->getChildByTag(HERO_LISTMENU_TAG)->resume();
    }
    if(new_stage==2){
        __NotificationCenter::getInstance()->postNotification("newPlayer",(Ref*)(intptr_t)(new_stage));
        new_step++;
    }
    removeFromParentAndCleanup(true);
}

void HeroInfoLayer::close(Ref* pSender){
    CCLOG("返回上一层");
    removeFromParentAndCleanup(true);
}


bool HeroInfoLayer::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){
    CCLOG("On Touch Began!");
    if(new_stage==1){
        auto _newPlayerLayer = new NewPlayerLayer();
        _newPlayerLayer->init(6);
        this->addChild(_newPlayerLayer,4,NEWPLAYER_LAYER_TAG);//不知道为什么不能getParent()->add
    }

    return true;
}
void HeroInfoLayer::onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event){
    CCLOG("on touch moved");
}
void HeroInfoLayer::onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event){
    CCLOG("On Touch Ended!");
}



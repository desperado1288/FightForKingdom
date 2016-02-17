#include "LayerManager/MainScene.h"
#include "GameStat.h"
#include "ZhuangbeiListLayer.h"
#include "LayerManager/ZhenRongLayer.h"
#include <ui/CocosGUI.h>
#include "ModifiedSourceCode/DMenu.h"
#include "LayerManager/HelloWorldLayer.h"
#include "HeroList/TeamListLayer.h"

USING_NS_CC;

using namespace std;
using namespace cocos2d::ui;

// on "init" you need to initialize your instance
ZhuangbeiListLayer::ZhuangbeiListLayer(){}
ZhuangbeiListLayer::~ZhuangbeiListLayer(){}

bool ZhuangbeiListLayer::init(int type)
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("heroInfo.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("evolution.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("fonts/newBmpFont.plist");
//TO DO...layer 的listener可以去掉

    auto bg = Sprite::create("bg_neirong.png");
    bg->setPosition(visibleSize.width/2,visibleSize.height/2+33);
    this->addChild(bg,1);
    auto titleicon = Sprite::create("icon_titlebg.png");
    titleicon->setPosition(visibleSize.width/2,visibleSize.height-(176+33));
    this->addChild(titleicon,1);
    auto titlelabel = Sprite::createWithSpriteFrameName("title_zhenrong.png");//加“阵容”标题
    titleicon->addChild(titlelabel,2);
    titlelabel->setPosition(titleicon->getContentSize().width/2,titleicon->getContentSize().height/2);
    auto separator = Sprite::createWithSpriteFrameName("line.png");//加分割线
    separator->setPosition(visibleSize.width/2,visibleSize.height-322);
    this->addChild(separator,1);
    //TO DO...返回主页
    auto fanhuiItem = MenuItemImage::create("btn_arrow_right_disable.png","btn_arrow_right_disable.png",this,NULL);
    auto fanhui = Sprite::create("btn_arrow_right_disable.png");
    auto wujianghun = MenuItemSprite::create(fanhui,fanhui,CC_CALLBACK_1(ZhuangbeiListLayer::back, this));
    wujianghun->setPosition(wujianghun->getContentSize().width/2-visibleSize.width/2,0);
    fanhuiItem->setPosition(visibleSize.width/2-fanhuiItem->getContentSize().width/2,0);
    auto menu1 = Menu::create(wujianghun,fanhuiItem,NULL);
    wujianghun->setRotation(-180);
    auto returnlabel = Sprite::createWithSpriteFrameName("title_zhenrong.png");
    returnlabel->setScale(0.8f);
    wujianghun->addChild(returnlabel,2);
    returnlabel->setRotation(-180);
    returnlabel->setPosition(wujianghun->getContentSize().width/2,wujianghun->getContentSize().height/2);

    menu1->setPosition(visibleSize.width/2,visibleSize.height-207);
    this->addChild(menu1,3);

    
    //武将、魂的标签
    auto wujiangTag = MenuItemImage::create("btn-yeqian-wujiangan.png","btn-yeqian-wujiang.png",this,NULL);
    wujiangTag->setPosition(wujiangTag->getContentSize().width,visibleSize.height-264-wujiangTag->getContentSize().height/2);
    auto menu2 = Menu::create(wujiangTag,NULL);
    menu2->setPosition(wujiangTag->getContentSize().width+66,visibleSize.height-264-wujiangTag->getContentSize().height/2);
    this->addChild(menu2,2);
    
//下拉列表
    int equip_size = 0;
    for(int i=0; i<userzhuangbei.size(); ++i) {
        if(userzhuangbei[i].type==type)
            equip_size++;
    }
    if(type==-1)
        equip_size = userzhuangbei.size();
    
    auto scrollList = ui::ScrollView::create();
    scrollList->setTouchEnabled(true);
    scrollList->setBounceEnabled(true);
    scrollList->setSize(Size(560,570));
    scrollList->setAnchorPoint(Vec2(0.5,0.5));
    scrollList->setPosition(Point(visibleSize.width/2,visibleSize.height/2-50));
    scrollList->setInnerContainerSize(Size(560,equip_size*180));//TO DO...inner size
    CCLOG("team size is : %lu,此项装备size:%d",userzhuangbei.size(),equip_size);
    
    this->addChild(scrollList,3,SCROLL_LIST_TAG);
    int j=0;
    //TO DO...scrollview加入listener接受touch事件
    auto menu = (DMenu*)DMenu::create();//用DMenu，自己加的继承Menu的方法
    menu->setPosition(Point::ZERO);
    scrollList->addChild(menu,6,HERO_LISTMENU_TAG);
    menu->setTouchlimit(scrollList);//判断menu到scrollView边框外，不能触发touch
    menu->setSwallowTouches(false);
    
    if(equip_size>0){
        auto listener = EventListenerTouchOneByOne::create();//加入触摸监听器
        listener->setSwallowTouches(true);
        listener->onTouchBegan = CC_CALLBACK_2(ZhuangbeiListLayer::onTouchBegan, this);
        listener->onTouchMoved = CC_CALLBACK_2(ZhuangbeiListLayer::onTouchMoved, this);
        listener->onTouchEnded = CC_CALLBACK_2(ZhuangbeiListLayer::onTouchEnded, this);
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, menu);
    }
    //开始英雄列表
    for(int i=0; i<userzhuangbei.size(); ++i) {
        if(userzhuangbei[i].type!=type&&type!=-1)
            continue;
        auto teamframe = Sprite::createWithSpriteFrameName("icon-zhuanshenglandi.png");
        teamframe->setPosition(visibleSize.width/2-40,scrollList->getInnerContainerSize().height-j*180-teamframe->getContentSize().height/2);
        scrollList->addChild(teamframe,4);
        //加英雄头像和属性
        string heropicstr = weaponpath(userzhuangbei[i].engname);
        //TO DO....
        auto herohead = MenuItemImage::create();
        display_type = type;
        if(type == -1)//纯显示装备列表
            herohead = MenuItemImage::create(heropicstr,heropicstr,CC_CALLBACK_1(ZhuangbeiListLayer::showHeroActionMenu, this));
        else{//阵型Layer回调函数
            herohead = MenuItemImage::create(heropicstr,heropicstr,CC_CALLBACK_1(ZhuangbeiListLayer::selectHeroToTeam, this));
            //((Layer*)this->getParent()->getChildByTag(ZHENFA_LAYER_TAG))->setTouchEnabled(false);
            /*if(isInCompeteTeam(userzhuangbei[i].position)){
                CCLOG("已上阵");
                herohead = MenuItemImage::create(heropicstr,heropicstr,CC_CALLBACK_1(ZhuangbeiListLayer::exchangeEquip, this));
            }*/
        }
        herohead->setPosition(Point(21+herohead->getContentSize().width/2+10+5,scrollList->getInnerContainerSize().height-j*180-teamframe->getContentSize().height/2));
        herohead->setScale(0.75f);
        herohead->setTag(HERO_SELECT_TAG+i);//用于回调阵型Layer和长按显示武将详情
        Vec2 pos = this->convertToWorldSpace(herohead->getPosition());
        //CCLOG("位置在x:%f ,y:%f,Menu英雄为:%s",pos.x,pos.y,team[i].name.c_str());
        menu->addChild(herohead,3);

        auto label2 = LabelAtlas::create("0123456789", "number_green.png",49,88,'0');
        label2->setScale(0.35f);
        int displaystat = round(userzhuangbei[i].propertyDisplay);
        char str[50];
        scrollList->addChild(label2, 6);
        sprintf(str, "%d",displaystat);
        label2->setString(str);
        label2->setPosition(20+teamframe->getContentSize().width/2,scrollList->getInnerContainerSize().height-j*180-teamframe->getContentSize().height/2-56);
//装备名字
        auto equipname = Label::createWithTTF(userzhuangbei[i].name, "字体/华文新魏.TTF",30);
        scrollList->addChild(equipname, 6);
        equipname->setPosition(40+teamframe->getContentSize().width/2,8+scrollList->getInnerContainerSize().height-j*180-teamframe->getContentSize().height/2-16);
        if(userzhuangbei[i].quality==1)
            equipname->setColor(Color3B::MAGENTA);
        else if(userzhuangbei[i].quality==2)
            equipname->setColor(Color3B::BLUE);
        else if(userzhuangbei[i].quality==3)
            equipname->setColor(Color3B::GREEN);
        else if(userzhuangbei[i].quality==0)
            equipname->setColor(Color3B::ORANGE);

        
        
        if(userzhuangbei[i].position<6){//装备于某英雄
        char sql[64];
        sprintf(sql,"装备于%s",userteam[userzhuangbei[i].position].name.c_str());
        auto label3 = Label::createWithTTF(sql, "字体/华文新魏.TTF", 20);
        scrollList->addChild(label3, 6);
        label3->setPosition(scrollList->getContentSize().width-41-100+15,5+scrollList->getInnerContainerSize().height-j*180-teamframe->getContentSize().height/2+24);
        }
        
        j++;
    }

    return true;
}
bool ZhuangbeiListLayer::isInCompeteTeam(int index){
    if (userzhuangbei[index].position<6) {
        return true;
    }
    
    return false;
}
void ZhuangbeiListLayer::showHeroActionMenu(Ref* pSender){
    
}
void ZhuangbeiListLayer::showHeroInfo(float dt){
    this->unschedule(schedule_selector(TeamListLayer::showHeroInfo));
    CCLOG("显示英雄信息");
    GameStat::getInstance()->show_hero_info = touch_hero_index;
    isResponseTouchEnded = false;
}

bool ZhuangbeiListLayer::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){
    CCLOG("On Touch Began!");
    
    for (auto i=0; i<userzhuangbei.size(); ++i) {
        //TO DO...遍历所有英雄头像的节点//转换成父节点menu坐标系终于可以用了!
        if(this->getChildByTag(SCROLL_LIST_TAG)->getChildByTag(HERO_LISTMENU_TAG)->getChildByTag(HERO_SELECT_TAG+i)->getBoundingBox().containsPoint(this->getChildByTag(SCROLL_LIST_TAG)->getChildByTag(HERO_LISTMENU_TAG)->convertToNodeSpace(touch->getLocation()))){
            CCLOG("触发点击武将:%s",userzhuangbei[i].name.c_str());
            //this->schedule(schedule_selector(TeamListLayer::showHeroInfo),1);
            touch_hero_index = i;
            isResponseTouchEnded = true;
            return true;
        }
    }
    return false;
}

void ZhuangbeiListLayer::onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event){
    CCLOG("on touch moved");
    //this->unschedule(schedule_selector(TeamListLayer::showHeroInfo));//只要移动就判断取消长按功能
}
void ZhuangbeiListLayer::onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event){
    CCLOG("On Touch Ended!");
    //this->unschedule(schedule_selector(TeamListLayer::showHeroInfo));//只要移动就判断取消长按功能
    if(!isResponseTouchEnded)
        return;
    //手动控制TouchEnded，相应按钮
    if(display_type!=-1){
        CCLOG("标签:%d  装备名字:%s",touch_hero_index,userzhuangbei[touch_hero_index].name.c_str());
        //TO DO.....
        //ZhenFa::setSelectedHero(tag-HERO_SELECT_TAG);
        /*if(isInCompeteTeam(userzhuangbei[touch_hero_index].index)){
            Size visibleSize = Director::getInstance()->getVisibleSize();
            CCLOG("已在阵中，无法点击加入");
            auto noticesp = Label::createWithBMFont("fonts/heroName.fnt", "玉麒麟 卢俊义");//TO DO...以后换字体
            noticesp->setPosition(visibleSize.width/2,visibleSize.height/2);
            auto seq = Sequence::create(MoveBy::create(1.0f, Point(0,100)), FadeOut::create(1.0f),RemoveSelf::create(),NULL);
            noticesp->runAction(seq);
            this->addChild(noticesp,4);
            return;

        }*/
        __NotificationCenter::getInstance()->postNotification("addZhuangbei",(Ref*)(intptr_t)(touch_hero_index));
        CCLOG("选中英雄，返回上一层");
        //TO DO....恢复上一层的touch
        //this->getParent()->getChildByTag(ZHENFA_LAYER_TAG)->resume();
        this->getParent()->removeChildByTag(SHIELD_LAYER_TAG+5);

        removeFromParentAndCleanup(true);
    }

}
Rect ZhuangbeiListLayer::getRect(int tag){
    auto sp = (MenuItemImage*)(this->getChildByTag(SCROLL_LIST_TAG)->getChildByTag(HERO_LISTMENU_TAG)->getChildByTag(HERO_SELECT_TAG+tag));
    Size size = sp->getContentSize();
    Vec2 pos = this->convertToWorldSpace(sp->getPosition());
    CCLOG("位置在x:%f ,y:%f,Menu英雄为:%s",pos.x,pos.y,team[tag].name.c_str());
    return Rect(pos.x-size.width / 2 ,pos.y-size.height / 2, size.width ,size.height);
}
void ZhuangbeiListLayer::selectHeroToTeam(Ref* pSender){
    auto btn = (MenuItemSprite*)pSender;
    int tag = btn->getTag();
}
void ZhuangbeiListLayer::back(Ref* pSender){
    CCLOG("remove this layer!");
    if(display_type==-1){
        auto _HelloWorldLayer = HelloWorldLayer::create();
        if(_HelloWorldLayer)
            this->getParent()->addChild(_HelloWorldLayer,2,HELLOWORLD_LAYER_TAG);
    }
//TO DO....恢复上一层的touch
    this->getParent()->removeChildByTag(SHIELD_LAYER_TAG+5);
    this->getParent()->removeChildByTag(SHIELD_LAYER_TAG);

    this->removeFromParentAndCleanup(true);
}
void ZhuangbeiListLayer::exchangeEquip(Ref* pSender){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    //CCLOG("已在阵中，无法点击加入");
    auto noticesp = Label::createWithBMFont("fonts/heroName.fnt", "玉麒麟 卢俊义");//TO DO...以后换字体
    noticesp->setPosition(visibleSize.width/2,visibleSize.height/2);
    auto seq = Sequence::create(MoveBy::create(1.0f, Point(0,100)), FadeOut::create(1.0f),RemoveSelf::create(),NULL);
    noticesp->runAction(seq);
    this->addChild(noticesp,4);
}




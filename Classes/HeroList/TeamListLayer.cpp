#include "MainScene.h"
#include "LayerManager/HelloWorldLayer.h"

#include "HeroList/TeamListLayer.h"
#include "HeroList/ZhenFa.h"
#include "HeroList/HeroInfoLayer.h"
#include "ModifiedSourceCode/DMenu.h"
#include <ui/CocosGUI.h>
#include "GameStat.h"

USING_NS_CC;

using namespace std;
using namespace cocos2d::ui;

// on "init" you need to initialize your instance
TeamListLayer::TeamListLayer(){}
TeamListLayer::~TeamListLayer(){}
bool TeamListLayer::init(int type)
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
    auto listener = EventListenerTouchOneByOne::create();//加入触摸监听器
    listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(TeamListLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(TeamListLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(TeamListLayer::onTouchEnded, this);
    //Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

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
    auto wujianghun = MenuItemSprite::create(fanhui,fanhui,CC_CALLBACK_1(TeamListLayer::back, this));
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
    
    /*auto wujiangTag = MenuItemImage::create("btn-yeqian-wujiangan.png","btn-yeqian-wujiang.png",this,NULL);
    auto hunTag = MenuItemImage::create("btn-yeqian-wujiangxingxiuan.png","btn-yeqian-wujiangxingxiu.png",this,NULL);
    wujiangTag->setPosition(wujiangTag->getContentSize().width,visibleSize.height-264-wujiangTag->getContentSize().height/2);
    hunTag->setPosition(wujiangTag->getContentSize().width*1.6,visibleSize.height-264-wujiangTag->getContentSize().height/2);
    auto menu2 = Menu::create(wujiangTag,hunTag,NULL);
    menu2->setPosition(wujiangTag->getContentSize().width+66,visibleSize.height-264-wujiangTag->getContentSize().height/2);
    menu2->alignItemsHorizontally();
    //显示武将列表
    this->addChild(menu2,2);
    */
    
    
    auto scrollList = ui::ScrollView::create();
    scrollList->setTouchEnabled(true);
    scrollList->setBounceEnabled(true);
    scrollList->setSize(Size(560,570));
    scrollList->setAnchorPoint(Vec2(0.5,0.5));
    scrollList->setPosition(Point(visibleSize.width/2,visibleSize.height/2-50));
    scrollList->setInnerContainerSize(Size(560,team.size()*180));//TO DO...inner size
    CCLOG("team size is : %lu",team.size());
    
    /*auto listener1 = EventListenerTouchOneByOne::create();
    listener1->onTouchBegan = CC_CALLBACK_2(TeamListLayer::onTouchBegan, this);
    listener1->onTouchMoved = CC_CALLBACK_2(TeamListLayer::onTouchMoved, this);
    listener1->onTouchEnded = CC_CALLBACK_2(TeamListLayer::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener1, scrollList);
*/
    this->addChild(scrollList,3,SCROLL_LIST_TAG);
    int j=0;
    //TO DO...scrollview加入listener接受touch事件
    auto menu = (DMenu*)DMenu::create();//用DMenu，自己加的继承Menu的方法
    menu->setPosition(Point::ZERO);
    scrollList->addChild(menu,6,HERO_LISTMENU_TAG);
    menu->setTouchlimit(scrollList);//判断menu到scrollView边框外，不能触发touch
    menu->setSwallowTouches(false);
    auto listener1 = EventListenerTouchOneByOne::create();//加入触摸监听器
    //listener1->setSwallowTouches(true);   //TO DO....有可能要复原
    listener1->onTouchBegan = CC_CALLBACK_2(TeamListLayer::onTouchBegan, this);
    listener1->onTouchMoved = CC_CALLBACK_2(TeamListLayer::onTouchMoved, this);
    listener1->onTouchEnded = CC_CALLBACK_2(TeamListLayer::onTouchEnded, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener1, menu);

    //开始英雄列表
    for(int i=0; i<team.size(); ++i) {
        auto teamframe = Sprite::createWithSpriteFrameName("icon-zhuanshenglandi.png");
        teamframe->setPosition(visibleSize.width/2-40,scrollList->getInnerContainerSize().height-j*180-teamframe->getContentSize().height/2);
        scrollList->addChild(teamframe,4);
        //加英雄头像和属性
        string heropicstr = heropath(team[i].engname);
        /*auto herohead = Button::create(heropicstr);
        herohead->setTouchEnabled(true);
        
        if(type==-1)
            herohead->addTouchEventListener(CC_CALLBACK_1(TeamListLayer::showHeroInfo, this));
        else if(type==1)//阵型Layer回调函数
            herohead->addTouchEventListener(CC_CALLBACK_1(TeamListLayer::selectHeroToTeam, this));

        herohead->setPosition(Point(41+herohead->getContentSize().width/2+10,scrollList->getInnerContainerSize().height-j*180-teamframe->getContentSize().height/2));
        herohead->setTag(500+i);//用于回调阵型Layer和长按显示武将详情
        scrollList->addChild(herohead,6,(HERO_SELECT_TAG+i));
        */
        //TO DO....
        auto herohead = MenuItemImage::create();
        display_type = type;
        if(type == -1)//纯显示武将列表
            herohead = MenuItemImage::create(heropicstr,heropicstr,CC_CALLBACK_1(TeamListLayer::showHeroActionMenu, this));
        else if (type == 1 || type==2){//阵型Layer回调函数
            herohead = MenuItemImage::create(heropicstr,heropicstr,CC_CALLBACK_1(TeamListLayer::selectHeroToTeam, this));
            //((Layer*)this->getParent()->getChildByTag(ZHENFA_LAYER_TAG))->setTouchEnabled(false);
            if(isInCompeteTeam(team[i].index)){
                CCLOG("已上阵");
                herohead = MenuItemImage::create(heropicstr,heropicstr,CC_CALLBACK_1(TeamListLayer::alreadyInTeam, this));
            }
        }
        herohead->setPosition(Point(41+herohead->getContentSize().width/2+10+5,scrollList->getInnerContainerSize().height-j*180-teamframe->getContentSize().height/2));
        herohead->setTag(HERO_SELECT_TAG+i);//用于回调阵型Layer和长按显示武将详情
        Vec2 pos = this->convertToWorldSpace(herohead->getPosition());
        //CCLOG("位置在x:%f ,y:%f,Menu英雄为:%s",pos.x,pos.y,team[i].name.c_str());

        menu->addChild(herohead,3);
        /*auto menu1 = Menu::create(herohead,NULL);
        menu1->setTouchlimit(scrollList);
        menu1->setPosition(Point::ZERO);
        */
        //scrollList->addChild(menu1,6,(HERO_SELECT_TAG+i));
        //TO DO....
        
        auto label1 = LabelAtlas::create("0123456789", "number_green.png",49,88,'0');
        label1->setScale(0.35f);
        scrollList->addChild(label1, 6);
        char str[50];
        int displaystat = round(team[i].healthDisplay);
        sprintf(str, "%d",displaystat);
        CCLOG("生命是:%s",str);
        label1->setString(str);
        label1->setPosition(20+teamframe->getContentSize().width/2,scrollList->getInnerContainerSize().height-j*180-teamframe->getContentSize().height/2-16);
        
        auto label2 = LabelAtlas::create("0123456789", "number_green.png",49,88,'0');
        label2->setScale(0.35f);
        displaystat = round(team[i].attackDisplay);

        scrollList->addChild(label2, 6);
        sprintf(str, "%d",displaystat);
        label2->setString(str);
        label2->setPosition(20+teamframe->getContentSize().width/2,scrollList->getInnerContainerSize().height-j*180-teamframe->getContentSize().height/2-56);
 
        auto label3 = LabelAtlas::create("0123456789", "number_green.png",49,88,'0');
        label3->setScale(0.35f);
        scrollList->addChild(label3, 6);
        displaystat = round(team[i].agilityDisplay);

        sprintf(str, "%d",displaystat);
        label3->setString(str);
        label3->setPosition(scrollList->getContentSize().width-41-100,scrollList->getInnerContainerSize().height-j*180-teamframe->getContentSize().height/2+24);

        auto label4 = LabelAtlas::create("0123456789", "number_green.png",49,88,'0');
        label4->setScale(0.35f);
        scrollList->addChild(label4, 6);
        displaystat = round(team[i].defenseDisplay);

        sprintf(str, "%d",displaystat);
        label4->setString(str);
        label4->setPosition(scrollList->getContentSize().width-41-100,scrollList->getInnerContainerSize().height-j*180-teamframe->getContentSize().height/2-17);

        auto label5 = LabelAtlas::create("0123456789", "number_green.png",49,88,'0');
        label5->setScale(0.35f);
        displaystat = round(team[i].criticDisplay);

        scrollList->addChild(label5, 6);
        sprintf(str, "%d",displaystat);
        label5->setString(str);
        label5->setPosition(scrollList->getContentSize().width-41-100,scrollList->getInnerContainerSize().height-j*180-teamframe->getContentSize().height/2-58);
        
        j++;
    }
    //((Layer*)this->getParent()->getChildByTag(ZHENFA_LAYER_TAG))->pause();

    return true;
}
bool TeamListLayer::isInCompeteTeam(int index){
    for (auto it = competeTeam.begin(); it!=competeTeam.end(); ++it) {
        //CCLOG("正在检查competeMap中的%s",it->second.name.c_str());
        if (it->second.index == index) {
            return true;
        }
    }
    return false;
}
void TeamListLayer::showHeroActionMenu(Ref* pSender){
    
}
void TeamListLayer::showHeroInfo(float dt){
    this->unschedule(schedule_selector(TeamListLayer::showHeroInfo));
    CCLOG("显示英雄信息");
    //this->getChildByTag(SCROLL_LIST_TAG)->getChildByTag(HERO_LISTMENU_TAG)->pause();//TO DO....目的:停止触发此头像的touchEnded，和所有其他按钮的功能
    //this->getChildByTag(SCROLL_LIST_TAG)->getChildByTag(HERO_LISTMENU_TAG)->resume();//TO DO....目的:停止触发此头像的touchEnded，和所有其他按钮的功能
    GameStat::getInstance()->show_hero_info = touch_hero_index;
    isResponseTouchEnded = false;
    //TO DO...显示英雄详细信息
    auto _HeroInfoLayer = new HeroInfoLayer();
    _HeroInfoLayer->init(1,touch_hero_index);
    if(_HeroInfoLayer){
        this->getParent()->addChild(_HeroInfoLayer,7,HEROINFO_LAYER_TAG);
    }

}

bool TeamListLayer::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){
    CCLOG("On Touch Began!");
    /*for (int i=0; i<6; ++i) {
        if (this->getRect(100+i).containsPoint(touch->getLocation())){//判断触摸点是否在目标的范围内
            return true;
        }
    }*/
    for (auto i=0; i<team.size(); ++i) {
        //TO DO...遍历所有英雄头像的节点//转换成父节点menu坐标系终于可以用了!
        if(this->getChildByTag(SCROLL_LIST_TAG)->getChildByTag(HERO_LISTMENU_TAG)->getChildByTag(HERO_SELECT_TAG+i)->getBoundingBox().containsPoint(this->getChildByTag(SCROLL_LIST_TAG)->getChildByTag(HERO_LISTMENU_TAG)->convertToNodeSpace(touch->getLocation()))){
            CCLOG("触发点击武将:%s",team[i].name.c_str());
            this->schedule(schedule_selector(TeamListLayer::showHeroInfo),1);
            touch_hero_index = i;
            isResponseTouchEnded = true;
            return true;
        }
    }
    return false;
}

void TeamListLayer::onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event){
    CCLOG("on touch moved");
    this->unschedule(schedule_selector(TeamListLayer::showHeroInfo));//只要移动就判断取消长按功能
}
void TeamListLayer::onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event){
    CCLOG("On Touch Ended!");
    this->unschedule(schedule_selector(TeamListLayer::showHeroInfo));//只要移动就判断取消长按功能
    if(!isResponseTouchEnded)
        return;
    //手动控制TouchEnded，相应按钮
    if(display_type==1||display_type==2){
        CCLOG("标签:%d   英雄名字:%s",touch_hero_index,team[touch_hero_index].name.c_str());
        //TO DO.....
        //ZhenFa::setSelectedHero(tag-HERO_SELECT_TAG);
        if(isInCompeteTeam(team[touch_hero_index].index)){
            Size visibleSize = Director::getInstance()->getVisibleSize();
            CCLOG("已在阵中，无法点击加入");
            auto noticesp = Label::createWithBMFont("fonts/heroName.fnt", "玉麒麟 卢俊义");//TO DO...以后换字体
            noticesp->setPosition(visibleSize.width/2,visibleSize.height/2);
            auto seq = Sequence::create(MoveBy::create(1.0f, Point(0,100)), FadeOut::create(1.0f),RemoveSelf::create(),NULL);
            noticesp->runAction(seq);
            this->addChild(noticesp,4);
            return;

        }
        __NotificationCenter::getInstance()->postNotification("addHero",(Ref*)(intptr_t)(touch_hero_index));
        CCLOG("选中英雄，返回上一层");
        //TO DO....恢复上一层的touch
        //this->getParent()->getChildByTag(ZHENFA_LAYER_TAG)->resume();
        this->getParent()->removeChildByTag(SHIELD_LAYER_TAG+5);

        removeFromParentAndCleanup(true);
    }

}
Rect TeamListLayer::getRect(int tag){
    auto sp = (MenuItemImage*)(this->getChildByTag(SCROLL_LIST_TAG)->getChildByTag(HERO_LISTMENU_TAG)->getChildByTag(HERO_SELECT_TAG+tag));
    Size size = sp->getContentSize();
    Vec2 pos = this->convertToWorldSpace(sp->getPosition());
    CCLOG("位置在x:%f ,y:%f,Menu英雄为:%s",pos.x,pos.y,team[tag].name.c_str());
    return Rect(pos.x-size.width / 2 ,pos.y-size.height / 2, size.width ,size.height);
}
void TeamListLayer::selectHeroToTeam(Ref* pSender){
    auto btn = (MenuItemSprite*)pSender;
    int tag = btn->getTag();
    //CCLOG("选中的英雄标签:%d",tag);
    //CCLOG("标签:%d   英雄名字:%s",tag,team[tag-HERO_SELECT_TAG].name.c_str());
    //TO DO.....
    //ZhenFa::setSelectedHero(tag-HERO_SELECT_TAG);
    //__NotificationCenter::getInstance()->postNotification("addHero",(Ref*)(intptr_t)(tag-HERO_SELECT_TAG));
    //CCLOG("选中英雄，返回上一层");
    //TO DO....恢复上一层的touch
    //this->getParent()->getChildByTag(ZHENFA_LAYER_TAG)->resume();

    //removeFromParentAndCleanup(true);
}
void TeamListLayer::back(Ref* pSender){
    CCLOG("remove this layer!");
    auto _HelloWorldLayer = HelloWorldLayer::create();
    if(_HelloWorldLayer)
        this->getParent()->addChild(_HelloWorldLayer,2,HELLOWORLD_LAYER_TAG);
//TO DO....恢复上一层的touch
    this->getParent()->removeChildByTag(SHIELD_LAYER_TAG+5);
    this->getParent()->removeChildByTag(SHIELD_LAYER_TAG);

    this->removeFromParentAndCleanup(true);
}
void TeamListLayer::alreadyInTeam(Ref* pSender){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    //CCLOG("已在阵中，无法点击加入");
    auto noticesp = Label::createWithBMFont("fonts/heroName.fnt", "玉麒麟 卢俊义");//TO DO...以后换字体
    noticesp->setPosition(visibleSize.width/2,visibleSize.height/2);
    auto seq = Sequence::create(MoveBy::create(1.0f, Point(0,100)), FadeOut::create(1.0f),RemoveSelf::create(),NULL);
    noticesp->runAction(seq);
    this->addChild(noticesp,4);
}




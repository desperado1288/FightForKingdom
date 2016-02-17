#include "LayerManager/MainScene.h"
#include "LayerManager/ZhenRongLayer.h"
#include "LayerManager/HelloWorldLayer.h"

#include "HeroList/ZhenFa.h"
#include "HeroList/HeroInfoLayer.h"
#include "HeroList/TeamListLayer.h"
#include "ZhuangbeiListLayer.h"
#include "ModifiedSourceCode/DControlButton.h"
#include "ModifiedSourceCode/DMenu.h"

#include "ShieldLayer.h"
#include "GameStat.h"

#include <ui/CocosGUI.h>
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace cocos2d::ui;

// on "init" you need to initialize your instance
bool ZhenRongLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    level_restriction.push_back(3);
    level_restriction.push_back(5);
    level_restriction.push_back(10);
    level_restriction.push_back(15);


    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("heroInfo.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("evolution.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("fonts/newBmpFont.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("commonBtn.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("newHead.plist");
    //TO DO...layer 的listener可以去掉
    auto listener = EventListenerTouchOneByOne::create();//加入触摸监听器
    //listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(ZhenRongLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(ZhenRongLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(ZhenRongLayer::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    auto bg = Sprite::create("bg_neirong.png");
    bg->setPosition(visibleSize.width/2,visibleSize.height/2+33);
    this->addChild(bg,1);
    auto titleicon = Sprite::create("icon_titlebg.png");
    titleicon->setPosition(visibleSize.width/2,visibleSize.height-(176+33));
    this->addChild(titleicon,1);
    auto titlelabel = Sprite::createWithSpriteFrameName("title_shengjiwujiang.png");//加“阵容”标题
    titleicon->addChild(titlelabel,2);
    titlelabel->setPosition(titleicon->getContentSize().width/2,titleicon->getContentSize().height/2);
    //TO DO...返回主页
    auto wenzi_fanhui = Sprite::createWithSpriteFrameName("icon_anniuwenzi_fanhui.png");
    auto fanhui = Sprite::create("btn_arrow_right_disable.png");
    auto sprite9normal = Scale9Sprite::create("btn_arrow_right_disable.png");
    //auto sprite9label = Scale9Sprite::createWithSpriteFrameName("icon_anniuwenzi_fanhui.png");
    auto sprite9label = Sprite::createWithSpriteFrameName("icon_anniuwenzi_fanhui.png");
    auto btn = DControlButton::create(sprite9normal);
    //auto btnLabel = ControlButton::create(sprite9label);
    auto sprite9selected = Scale9Sprite::createWithSpriteFrameName("btn_arrow_right.png");
    //btn->addChild(btnLabel,1);
    btn->addChild(sprite9label,2);
    this->addChild(btn,2,999);
    btn->setScaleRatio(0.8f);
    //btnLabel->setPreferredSize(wenzi_fanhui->getContentSize());
    btn->setPreferredSize(fanhui->getContentSize());
    btn->setPosition(btn->getContentSize().width/2,visibleSize.height-207);
    //btnLabel->setPosition(btn->getContentSize().width/2, btn->getContentSize().height/2);
    sprite9label->setPosition(btn->getContentSize().width/2, btn->getContentSize().height/2);
    btn->addTargetWithActionForControlEvents(this, cccontrol_selector(ZhenRongLayer::back), Control::EventType::TOUCH_UP_INSIDE);
    btn->setBackgroundSpriteForState(sprite9selected, Control::State::HIGH_LIGHTED);


    
    //武将、魂的标签
    auto frame1 = Sprite::createWithSpriteFrameName("icon_touxiangdefault.png");
    
    scrollList = ui::ScrollView::create();
    scrollList->setTouchEnabled(true);
    scrollList->setBounceEnabled(true);
    scrollList->setSize(Size(580,frame1->getContentSize().height));
    scrollList->setAnchorPoint(Vec2(0.5,0.5));
    scrollList->setPosition(Point(visibleSize.width/2,visibleSize.height-264-50));
    scrollList->setInnerContainerSize(Size(6*frame1->getContentSize().width+5,frame1->getContentSize().height));//TO DO...inner size
    scrollList->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    CCLOG("team size is : %lu",team.size());
    this->addChild(scrollList,3,SCROLL_HEAD_TAG);
    int j=0;
    //TO DO...scrollview加入listener接受touch事件
    auto menu = (DMenu*)DMenu::create();//用DMenu，自己加的继承Menu的方法
    menu->setPosition(Point::ZERO);
    for(int i=0; i<6; ++i){
        /*auto sp = Sprite::create(heroheadpath(it->second.name));
        auto s9sprite = Scale9Sprite::create(heroheadpath(it->second.name));
        auto btn = ControlButton::create(s9sprite);
        scrollList->addChild(btn,3,HERO_HEAD_TAG+j);
        btn->setTag(HERO_HEAD_TAG+j);
        btn->setPreferredSize(sp->getContentSize());
        btn->setPosition(frame1->getContentSize().width/2+j*(frame1->getContentSize().width+5),frame1->getContentSize().height/2);
        btn->addTargetWithActionForControlEvents(this, cccontrol_selector(ZhenRongLayer::selectHero), Control::EventType::TOUCH_UP_INSIDE);
        */
        //TO DO...用UIButton，上边用的是CCControlButton
        auto sp1 = Sprite::createWithSpriteFrameName("icon_touxiangdefault.png");
        auto spColor = Sprite::createWithSpriteFrameName("container3.png");
        sp1->setPosition(frame1->getContentSize().width/2+j*(frame1->getContentSize().width+5),frame1->getContentSize().height/2);
        spColor->setPosition(frame1->getContentSize().width/2+j*(frame1->getContentSize().width+5),frame1->getContentSize().height/2);
        scrollList->addChild(sp1,2);
        scrollList->addChild(spColor,3);
        
        auto spbg = Sprite::createWithSpriteFrameName("icon_touxiangbg.png");
        spbg->setPosition(frame1->getContentSize().width/2+j*(frame1->getContentSize().width+5),frame1->getContentSize().height/2);
        scrollList->addChild(spbg,4,HERO_HEAD_BG_TAG+j);
        spbg->setOpacity(122);

        if(i>=2){//从第三个框开始
            if(new_stage==1||(GameStat::getInstance()->userLevel<5&&i>=3)||(GameStat::getInstance()->userLevel<10&&i>=4)||(GameStat::getInstance()->userLevel<15&&i>=5)){//用户2级,3,5,10,15分别开一个栏
                auto sp = Sprite::createWithSpriteFrameName("btn_close.png");
                auto cant_select = MenuItemSprite::create(sp,sp,CC_CALLBACK_1(ZhenRongLayer::choose_hero,this));
                cant_select->setPosition(frame1->getContentSize().width/2+j*(frame1->getContentSize().width+5),frame1->getContentSize().height/2);
                auto menu_select = Menu::create(cant_select,NULL);
                menu_select->setPosition(Vec2::ZERO);
                scrollList->addChild(menu_select,3);
                cant_select->setTag(CANT_SELECT_TAG+i);
                j++;
                continue;
            }
            
        }
        auto btn1 = ui::Button::create();
        btn1->loadTextures(heroheadpath(userteam[i].engname), heroheadpath(userteam[i].engname));
        btn1->setPosition(Vec2(frame1->getContentSize().width/2+j*(frame1->getContentSize().width+5),frame1->getContentSize().height/2));
        btn1->setTag(HERO_HEAD_TAG+j);
        scrollList->addChild(btn1,3,HERO_HEAD_TAG+j);
        btn1->addTouchEventListener(CC_CALLBACK_2(ZhenRongLayer::chooseHero, this));
        btn1->setPressedActionEnabled(true);
        //TO DO...边框
        j++;
    }
    
    auto yingxiong = Sprite::create("heroCard/004gongsunsheng2.png");
    //scrollList1->setSwallowTouches(false);//不吞噬touch信息
    scrollList1 = ui::ScrollView::create();
    scrollList1->setTouchEnabled(true);
    scrollList1->setBounceEnabled(true);
    scrollList1->setSwallowTouches(false);
    scrollList1->setSize(Size(visibleSize.width,visibleSize.height/2));
    scrollList1->setAnchorPoint(Vec2(0.5,0.5));
    scrollList1->setPosition(Point(visibleSize.width/2,visibleSize.height/2));
    scrollList1->setInnerContainerSize(Size(6*visibleSize.width,yingxiong->getContentSize().height));//TO DO...inner size
    scrollList1->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    this->addChild(scrollList1,4,SCROLL_CARD_TAG+1);
    
    //scrollList1->jumpToPercentHorizontal(50.0);
    for(auto i=0; i<6; ++i){
        if(i>=2){//从第三个框开始
            if(new_stage==1||(GameStat::getInstance()->userLevel<5&&i>=3)||(GameStat::getInstance()->userLevel<10&&i>=4)||(GameStat::getInstance()->userLevel<15&&i>=5)){//用户2级,3,5,10,15分别开一个栏
                char label[1024];
                sprintf(label,"开启需要玩家等级%d级",level_restriction[i-2]);
                auto lock_label = Label::createWithTTF(label, "字体/华文新魏.TTF", 30);
                lock_label->setColor(Color3B::BLACK);
                scrollList1->addChild(lock_label,3,HERO_CARD_TAG+i);
                lock_label->setPosition(visibleSize.width/2+i*visibleSize.width,yingxiong->getContentSize().height/2);
                continue;
            }
        }
        auto btn2 = ui::Button::create();
        btn2->loadTextures(herocardpath(userteam[i].engname), herocardpath(userteam[i].engname));
        btn2->setPosition(Vec2(visibleSize.width/2+i*visibleSize.width,visibleSize.height/4-50));
        btn2->setTag(HERO_CARD_TAG+i);
        scrollList1->addChild(btn2,3,HERO_CARD_TAG+i);
        btn2->addTouchEventListener(CC_CALLBACK_2(ZhenRongLayer::showInfoHero, this));
        btn2->setSwallowTouches(false);
        //btn2->setPressedActionEnabled(true);
        auto heroname = Label::createWithTTF(userteam[i].name, "字体/华文新魏.TTF", 30);
        if(userteam[i].quality==1)
            heroname->setColor(Color3B::MAGENTA);
        else if(userteam[i].quality==2)
            heroname->setColor(Color3B::BLUE);
        else if(userteam[i].quality==3)
            heroname->setColor(Color3B::GREEN);
        else if(userteam[i].quality==0)
            heroname->setColor(Color3B::ORANGE);
        heroname->setPosition(Vec2(visibleSize.width/2-40+i*visibleSize.width,visibleSize.height/4-250));
        scrollList1->addChild(heroname,4);
        
        char label1[32];
        
        sprintf(label1,"%d级",userteam[i].level);
        auto level = Label::createWithTTF(label1, "字体/华文新魏.TTF", 30);
        level->setPosition(Vec2(visibleSize.width/2+40+i*visibleSize.width,visibleSize.height/4-250));
        level->setColor(Color3B::GREEN);
        scrollList1->addChild(level,4);

    //装备框------------
        auto menu1 = Menu::create();
        menu1->setSwallowTouches(false);
        menu1->setTouchlimit(scrollList1);
        menu1->setPosition(Point::ZERO);
        scrollList1->addChild(menu1,4,ZHUANGBEI_MENU_TAG+i);
        for (int k = 0; k<6; ++k) {//加6个可点击边框
            auto sp = Sprite::createWithSpriteFrameName("icon_baoshikuang_highlight.png");
            auto zhenfaFrame1 = MenuItem::create();
            if(k==5)
                zhenfaFrame1 = MenuItemSprite::create(sp,sp,NULL);
            else
                zhenfaFrame1 = MenuItemSprite::create(sp,sp,CC_CALLBACK_1(ZhenRongLayer::addZhuangbei, this));
            
            auto equiplabel1 = Label::createWithTTF("", "字体/华文新魏.TTF", 25);
            if (k<=2) {
                zhenfaFrame1->setPosition((i+1)*visibleSize.width-120,btn2->getPositionY()-(k-1)*150);
            }
            else
                zhenfaFrame1->setPosition(i*visibleSize.width+120,btn2->getPositionY()-(k-4)*150);
            if(k==0)
                equiplabel1->setString("武器");
            else if(k==1)
                equiplabel1->setString("战马");
            else if(k==2)
                equiplabel1->setString("书籍");
            else if(k==3)
                equiplabel1->setString("护甲");
            else if(k==4)
                equiplabel1->setString("饰物");
            equiplabel1->setPosition(zhenfaFrame1->getPositionX(),zhenfaFrame1->getPositionY()-zhenfaFrame1->getContentSize().height/2-equiplabel1->getContentSize().height/2-10);
            equiplabel1->setColor(Color3B::BLUE);
            scrollList1->addChild(equiplabel1,5);
        //加号
            auto addsp = Sprite::createWithSpriteFrameName("btn_zengjiajuese_normal.png");
            addsp->setPosition(zhenfaFrame1->getPosition());
            scrollList1->addChild(addsp,5);
            menu1->addChild(zhenfaFrame1,1,ZHUANGBEI_FRAME_TAG+k);
            
        }
        for (int p=0; p<userteam[i].zhuangbei.size(); ++p) {
            auto zhuangbei1 = Sprite::create(weaponpath(userteam[i].zhuangbei[p].engname));
            if (userteam[i].zhuangbei[p].type<=3) {
                zhuangbei1->setPosition((i+1)*visibleSize.width-120,btn2->getPositionY()-(userteam[i].zhuangbei[p].type-2)*150);
            }
            else
                zhuangbei1->setPosition(i*visibleSize.width+120,btn2->getPositionY()-(userteam[i].zhuangbei[p].type-5)*150);
            scrollList1->addChild(zhuangbei1,6,ZHUANGBEI_TAG+userteam[i].zhuangbei[p].type+i*10-1);
            log("此装备标签为%d",ZHUANGBEI_TAG+userteam[i].zhuangbei[p].type+i*10-1);
            zhuangbei1->setScale(0.6f);

        }
        
        
    //-----------------
    }
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ZhenRongLayer::changeHeroFromList), "addZhuangbei", NULL);

    scrollIndex = 0;//TO DO....滚动的页数
    this->getChildByTag(SCROLL_HEAD_TAG)->getChildByTag(HERO_HEAD_BG_TAG)->setOpacity(0);
    schedule(schedule_selector(ZhenRongLayer::init_page),0.01f);

    return true;
}
void ZhenRongLayer::changeHeroFromList(Ref* pSender){
    int newTag = (int)(intmax_t)pSender;//根据userzhuangbei的位置
    CCLOG("change hero to index:%d, at position: %d",newTag,_tag);
    string heropicname = weaponpath(userzhuangbei[newTag].engname);
    if(existsEquip){//替换英雄
        CCLOG("替换英雄");
        //competeTeam.erase(_tag%10);//删掉Compete队伍中对应位置的hero TO DO.....应该直接map[tag]=hero1就行，不管之前有没有
    }
    else{//选取新英雄加入空位置,即existsHero=false
        //TO DO...暂时不需要填写内容
        CCLOG("加入新英雄");
    }
//前一件装备-------
    if(userzhuangbei[newTag].position<6)
        this->getChildByTag(SCROLL_CARD_TAG+1)->removeChildByTag(ZHUANGBEI_TAG+_tag%10+userzhuangbei[newTag].position*10);
    log("此装备标签为%d",ZHUANGBEI_TAG+_tag%10+herotag*10);
    ZhenRongLayer::findEquipAndRemove(_tag%10,herotag);
//----------------
    userzhuangbei[newTag].position = herotag;
    userteam[herotag].zhuangbei.push_back(userzhuangbei[newTag]);//记录上场位置
    //TO DO...需要存入database
    CCLOG("加入了新英雄 %s 进入位置: %d",competeTeam[_tag%10].name.c_str(),(_tag%10));
    char sql[1024];
    sprintf(sql,"update User_equip set position = %d where username = \"%s\" and zhuangbei_index = %d and number_index = %d",herotag,user_name.c_str(),userzhuangbei[newTag].index,userzhuangbei[newTag].number_index);
    GameStat::getInstance()->saveToUserInfo("User_equip", documentpath1.c_str(), sql);

    //TO DO...设定上场位置
    auto herosp1 = Sprite::create(heropicname);
    //auto heroItem = MenuItemSprite::create(sp1, sp1,NULL);//TO DO....需要换成replaceHero
    herosp1->setTag(_tag%10+ZHUANGBEI_TAG+herotag*10);
    this->getChildByTag(SCROLL_CARD_TAG+1)->addChild(herosp1,6);
    herosp1->setPosition(this->getChildByTag(SCROLL_CARD_TAG+1)->getChildByTag(ZHUANGBEI_MENU_TAG+herotag)->getChildByTag(_tag%10+ZHUANGBEI_FRAME_TAG)->getPosition());
    herosp1->setScale(0.6f);
}

void ZhenRongLayer::addZhuangbei(Ref* pSender){
    auto sp = (MenuItemSprite*)pSender;//返回的是MenuItemImage不是menu
    _tag = sp->getTag();//70多
    CCLOG("选中边框标签是:%d",_tag);
    herotag = ((Menu*)sp->getParent())->getTag()%10;//表示第几个英雄
    
    if (!(this->getChildByTag(SCROLL_CARD_TAG+1)->getChildByTag(_tag%10+ZHUANGBEI_TAG+herotag*10))) {//50-55的tag是边框，说明没有上场英雄在这个位置
        CCLOG("此位置为空");
        existsEquip = false;
    }
    else
        existsEquip = true;

    auto _ZhuangbeiListLayer = new ZhuangbeiListLayer();
    _ZhuangbeiListLayer->init(_tag%10+1);
    
    if(_ZhuangbeiListLayer){
        CCLOG("加入选择武将界面");
        this->getParent()->addChild(_ZhuangbeiListLayer,6,TEAMLIST_LAYER_TAG);
    }
    auto _shieldLayer = new ShieldLayer();
    _shieldLayer->init(0);
    if(_shieldLayer){
        this->getParent()->addChild(_shieldLayer,5,SHIELD_LAYER_TAG+5);
    }

    CCLOG("现在selectedHeroTag是:%d",_tag);
    
}
void ZhenRongLayer::findEquipAndRemove(int type,int position){//对于典韦被动还需要修改,type的取值问题
    /*for(int i=0;i<userzhuangbei.size();++i){
        if(userzhuangbei[i].type==(type+1)&&userzhuangbei[i].position==position){
            log("装备名:%s,位置%d,对应武将%s",userzhuangbei[i].name.c_str(),position,userteam[position].name.c_str());
            userzhuangbei[i].position = 7;
            
        }
    }*/
    for (int i=0; i<userteam[position].zhuangbei.size(); ++i) {
        if(userteam[position].zhuangbei[i].type == (type+1)){
            userteam[position].zhuangbei.erase(userteam[position].zhuangbei.begin()+i);
            userteam[position].zhuangbei[i].position = 7;
            char sql[1024];
            sprintf(sql,"update User_equip set position = 7 where username = \"%s\" and zhuangbei_index = %d and number_index = %d",user_name.c_str(),userteam[position].zhuangbei[i].index,userteam[position].zhuangbei[i].number_index);
            GameStat::getInstance()->saveToUserInfo("User_equip", documentpath1.c_str(), sql);
            log("删除英雄%s的第%d件装备",userteam[position].name.c_str(),(type+1));

        }
    }
    
}

void ZhenRongLayer::init_page(float dt){
    if(new_stage==1){//新手指导，只有两个人的时候
        scrollList1->jumpToPercentHorizontal(20);
        scrollList->jumpToPercentHorizontal(20);
        this->getChildByTag(SCROLL_HEAD_TAG)->getChildByTag(HERO_HEAD_BG_TAG+1)->setOpacity(0);
        this->getChildByTag(SCROLL_HEAD_TAG)->getChildByTag(HERO_HEAD_BG_TAG)->setOpacity(122);
    }
    unschedule(schedule_selector(ZhenRongLayer::init_page));
}
void ZhenRongLayer::choose_hero(Ref* pSender){
    auto sp = (MenuItemSprite*)pSender;
    int spTag = (sp->getTag())%10;//此为button标签的个位数
    scrollList1->jumpToPercentHorizontal(spTag*20);
    scrollList->getChildByTag(HERO_HEAD_BG_TAG+scrollIndex)->setOpacity(122);
    scrollIndex = spTag;//上一个头像变暗，当前头像变亮
    scrollList->getChildByTag(HERO_HEAD_BG_TAG+scrollIndex)->setOpacity(0);
    scrollList->jumpToPercentHorizontal(20*spTag);

}
bool ZhenRongLayer::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){
    CCLOG("On Touch Began!");
    pt = touch->getStartLocation();
    auto point = scrollList1->getInnerContainer()->getPosition();
    CCLOG("now innerContainer position is x:%f y:%f",point.x,point.y);
    return true;
}
void ZhenRongLayer::onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event){
    CCLOG("on touch moved");
    //auto point = scrollList1->getInnerContainer()->getPosition();
    //CCLOG("now innerContainer position is x:%f y:%f",point.x,point.y);
}
void ZhenRongLayer::onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event){
    CCLOG("On Touch Ended!");
    scrollList1->stopScroll();
    if (touch->getLocation().x<=(pt.x-80)) {//scroll to the next page
        scrollList1->jumpToPercentHorizontal(20*((scrollIndex==(5))?scrollIndex:(scrollIndex+1)));
        scrollList->jumpToPercentHorizontal(20*((scrollIndex==(5))?scrollIndex:(scrollIndex+1)));
        if (scrollIndex!=(competeTeam.size()-1)) {
            scrollList->getChildByTag(HERO_HEAD_BG_TAG+scrollIndex)->setOpacity(122);
            scrollList->getChildByTag(HERO_HEAD_BG_TAG+(++scrollIndex))->setOpacity(0);
        }
    }
    else if (touch->getLocation().x>=(pt.x+80)) {//scroll to the previous page
        scrollList1->jumpToPercentHorizontal(20*((scrollIndex==0)?scrollIndex:(scrollIndex-1)));
        scrollList->jumpToPercentHorizontal(20*((scrollIndex==0)?scrollIndex:(scrollIndex-1)));
        if (scrollIndex!=0) {
            scrollList->getChildByTag(HERO_HEAD_BG_TAG+scrollIndex)->setOpacity(122);
            scrollList->getChildByTag(HERO_HEAD_BG_TAG+(--scrollIndex))->setOpacity(0);
        }
    }
    //TO DO....
    //trying to stop the scrolling view instantly
    
    /*auto point = scrollList1->getInnerContainer()->getPosition();
    if (touch->getLocation().x<=(pt.x-80)) {//scroll to the next page
        scrollList1->getInnerContainer()->setPosition(Vec2((scrollIndex==5)?scrollIndex:(scrollIndex+++1)*(-640),0));
    }
    else if (touch->getLocation().x>=(pt.x+80)) {//scroll to the previous page
        scrollList1->getInnerContainer()->setPosition(Vec2((scrollIndex==0)?scrollIndex:(scrollIndex---1)*(-640),0));
    }*/
}
void ZhenRongLayer::showInfoHero(Ref* pSender,Widget::TouchEventType type){
    int tag = ((ui::Button*)pSender)->getTag();
    CCLOG("阵容列表:显示第%d个",tag);
    switch (type) {
        case Widget::TouchEventType::BEGAN:
            CCLOG("Button Touch Began!");
            break;
        case Widget::TouchEventType::MOVED:
            CCLOG("Button Touch Moved!");
            break;
        case Widget::TouchEventType::ENDED:{
            CCLOG("Button Touch Ended!");
            CCLOG("显示英雄详细信息!");
//TO DO....让这个层button停止作用
            auto _HeroInfoLayer = new HeroInfoLayer();
            _HeroInfoLayer->init(2, tag-HERO_CARD_TAG);
            if(_HeroInfoLayer){
                this->getParent()->addChild(_HeroInfoLayer,4,HEROINFO_LAYER_TAG);
                //TO DO....英雄其他信息跟着更改
            }
        }
            break;
                
        default:
            break;
    }
}


void ZhenRongLayer::chooseHero(Ref* pSender,Widget::TouchEventType type){
    switch (type) {
        case Widget::TouchEventType::BEGAN:
            CCLOG("Button Touch Began!");
            break;
        case Widget::TouchEventType::MOVED:
            CCLOG("Button Touch Moved!");
            break;
        case Widget::TouchEventType::ENDED:{
           CCLOG("Button Touch Ended!");
            auto btnsp = (ui::Button*)pSender;
            int btnTag = (btnsp->getTag())%10;//此为button标签的个位数
            scrollList1->jumpToPercentHorizontal(btnTag*20);
            scrollList->getChildByTag(HERO_HEAD_BG_TAG+scrollIndex)->setOpacity(122);
            scrollIndex = btnTag;//上一个头像变暗，当前头像变亮
            scrollList->getChildByTag(HERO_HEAD_BG_TAG+scrollIndex)->setOpacity(0);
            scrollList->jumpToPercentHorizontal(20*btnTag);
            //TO DO....英雄其他信息跟着更改
        }
            break;
   
        default:
            break;
    }
}
void ZhenRongLayer::selectHero(Ref* pSender,Control::EventType event){
    auto btnsp = (ControlButton*)pSender;
    int btnTag = (btnsp->getTag())%10;//此为button标签的个位数
    scrollList1->jumpToPercentHorizontal(btnTag*20);
    scrollList->getChildByTag(HERO_HEAD_BG_TAG+scrollIndex)->setOpacity(122);
    scrollIndex = btnTag;//上一个头像变暗，当前头像变亮
    scrollList->getChildByTag(HERO_HEAD_BG_TAG+scrollIndex)->setOpacity(0);
    scrollList->jumpToPercentHorizontal(20*btnTag);
    //TO DO....英雄其他信息跟着更改
    
}
void ZhenRongLayer::back(Ref* pSender,Control::EventType event){
    CCLOG("remove this layer!");
    
    //TO DO....恢复上一层的touch
    auto _HelloWorldLayer = HelloWorldLayer::create();
    if(_HelloWorldLayer){
        //_HelloWorldLayer->setDelegator(_MainLayer);
        this->getParent()->addChild(_HelloWorldLayer,2,HELLOWORLD_LAYER_TAG);
    }
    this->getParent()->removeChildByTag(SHIELD_LAYER_TAG);
    this->removeFromParentAndCleanup(true);
}




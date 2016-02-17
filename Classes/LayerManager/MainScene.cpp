#include "LayerManager/MainScene.h"
#include "LayerManager/HelloWorldLayer.h"
#include "LayerManager/ZhenRongLayer.h"
#include "LayerManager/ZhengZhanLayer.h"

#include "HeroList/TeamListLayer.h"
#include "HeroList/ZhenFa.h"
#include "Compete/BattleResultLayer.h"
#include "GongchengQixie/GongchengZhenfa.h"

#include "cocos-ext.h"
#include <cocosbuilder/CocosBuilder.h>
#include "NewPlayerLayer.h"
#include "ShieldLayer.h"
#include "GameStat.h"
#include "MainLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;

bool MainScene::init(){
    if(!Scene::init()){
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("newAlwaysShow.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UI/res_fx/normal_bullet.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("newCommon.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("newBattle.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("nopack/qiancengta.plist");//出新英雄的背景
    
    auto btnFrame_normal = Sprite::createWithSpriteFrameName("btn_dibuzhukong_normal.png");
    auto btnFrame_clicked = Sprite::createWithSpriteFrameName("btn_dibuzhukong_highlight.png");
    auto btnFrame_normal2 = Sprite::createWithSpriteFrameName("btn_dibuzhukong_normal.png");
    auto btnFrame_clicked2 = Sprite::createWithSpriteFrameName("btn_dibuzhukong_highlight.png");
    auto btnFrame_normal3 = Sprite::createWithSpriteFrameName("btn_dibuzhukong_normal.png");
    auto btnFrame_clicked3 = Sprite::createWithSpriteFrameName("btn_dibuzhukong_highlight.png");
    auto btnFrame_normal4 = Sprite::createWithSpriteFrameName("btn_dibuzhukong_normal.png");
    auto btnFrame_clicked4 = Sprite::createWithSpriteFrameName("btn_dibuzhukong_highlight.png");
    auto btnFrame_normal5 = Sprite::createWithSpriteFrameName("btn_dibuzhukong_normal.png");
    auto btnFrame_clicked5 = Sprite::createWithSpriteFrameName("btn_dibuzhukong_highlight.png");

    auto liangshan = Sprite::createWithSpriteFrameName("btn_liangshan.png");
    auto zhenrong = Sprite::createWithSpriteFrameName("icon_zhenrong.png");
    auto zhengzhan = Sprite::createWithSpriteFrameName("icon_zhengzhan.png");
    auto xingxia = Sprite::createWithSpriteFrameName("icon_xingxia.png");
    auto shop = Sprite::createWithSpriteFrameName("icon_shangdian.png");
    //加上下边框
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gameFrame.plist");
    auto upFrame = Sprite::createWithSpriteFrameName("icon_shanghuabian.png");
    auto downFrame = Sprite::createWithSpriteFrameName("icon_xiahuabian.png");
    downFrame->setPosition(visibleSize.width/2,downFrame->getContentSize().height/2);
    upFrame->setPosition(visibleSize.width/2,visibleSize.height-upFrame->getContentSize().height/2);
    this->addChild(downFrame,2);
    this->addChild(upFrame,2);
    //加入所有底层功能
    auto liangshan2 = Sprite::createWithSpriteFrameName("btn_liangshan.png");
    liangshan2->setScale(0.8f);
    btnFrame_normal->addChild(liangshan);
    btnFrame_clicked->addChild(liangshan2);
    auto btn1 = MenuItemSprite::create(btnFrame_normal,btnFrame_clicked,CC_CALLBACK_1(MainScene::bottomFunction,this));
    
    liangshan->setPosition(btn1->getContentSize().width/2,btn1->getContentSize().height/2);
    liangshan2->setPosition(btn1->getContentSize().width/2,btn1->getContentSize().height/2);
    
    auto zhenrong2 = Sprite::createWithSpriteFrameName("icon_zhenrong.png");
    zhenrong2->setScale(0.8f);
    btnFrame_normal2->addChild(zhenrong);
    btnFrame_clicked2->addChild(zhenrong2);
    auto btn2 = MenuItemSprite::create(btnFrame_normal2,btnFrame_clicked2,CC_CALLBACK_1(MainScene::bottomFunction,this));
    zhenrong->setPosition(btn2->getContentSize().width/2,btn2->getContentSize().height/2);
    zhenrong2->setPosition(btn2->getContentSize().width/2,btn2->getContentSize().height/2);

    auto zhengzhan2 = Sprite::createWithSpriteFrameName("icon_zhengzhan.png");
    zhengzhan2->setScale(0.8f);
    btnFrame_normal3->addChild(zhengzhan);
    btnFrame_clicked3->addChild(zhengzhan2);
    auto btn3 = MenuItemSprite::create(btnFrame_normal3,btnFrame_clicked3,CC_CALLBACK_1(MainScene::bottomFunction,this));
    zhengzhan->setPosition(btn2->getContentSize().width/2,btn2->getContentSize().height/2);
    zhengzhan2->setPosition(btn2->getContentSize().width/2,btn2->getContentSize().height/2);
    
    auto xingxia2 = Sprite::createWithSpriteFrameName("icon_xingxia.png");
    xingxia2->setScale(0.8f);
    btnFrame_normal4->addChild(xingxia);
    btnFrame_clicked4->addChild(xingxia2);
    auto btn4 = MenuItemSprite::create(btnFrame_normal4,btnFrame_clicked4,CC_CALLBACK_1(MainScene::bottomFunction,this));
    xingxia->setPosition(btn2->getContentSize().width/2,btn2->getContentSize().height/2);
    xingxia2->setPosition(btn2->getContentSize().width/2,btn2->getContentSize().height/2);
    
    auto shop2 = Sprite::createWithSpriteFrameName("icon_shangdian.png");
    shop2->setScale(0.8f);
    btnFrame_normal5->addChild(shop);
    btnFrame_clicked5->addChild(shop2);
    auto btn5 = MenuItemSprite::create(btnFrame_normal5,btnFrame_clicked5,CC_CALLBACK_1(MainScene::bottomFunction,this));
    shop->setPosition(btn2->getContentSize().width/2,btn2->getContentSize().height/2);
    shop2->setPosition(btn2->getContentSize().width/2,btn2->getContentSize().height/2);
    
    //liangshan1->setTag(11);
    /*auto menu2 = Menu::create(liangshan1,zhenrong1,zhengzhan1,xingxia1,shop1,NULL);
    menu2->alignItemsHorizontallyWithPadding(0.0f);
    menu2->setPosition(visibleSize.width/2,btn1->getContentSize().height/2+downFrame->getContentSize().height-2);
    this->addChild(menu2);
    */
    menu1 = Menu::create(btn1,btn2,btn3,btn4,btn5,NULL);
    menu1->alignItemsHorizontally();
    menu1->alignItemsHorizontallyWithPadding(0.0f);
    menu1->setPosition(visibleSize.width/2,btn1->getContentSize().height/2+downFrame->getContentSize().height-2);
    this->addChild(menu1,2,MAIN_MENU_TAG);
//底层功能TAG
    btn1->setTag(11);
    btn2->setTag(12);
    btn3->setTag(13);
    btn4->setTag(14);
    btn5->setTag(15);
    
//TO DO....阵型   &   武将列表
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("more.plist");
    auto zhenxing = Sprite::createWithSpriteFrameName("icon_zhanghaoshezhi.png");
    auto zhenxingitem = MenuItemSprite::create(zhenxing, zhenxing, CC_CALLBACK_1(MainScene::showZhenXing,this));
    auto wujiang = Sprite::createWithSpriteFrameName("icon_tujian.png");
    auto wujiangitem = MenuItemSprite::create(wujiang, wujiang, CC_CALLBACK_1(MainScene::showTeamList,this));
    auto menu4 = Menu::create(zhenxingitem,wujiangitem,NULL);
    menu4->alignItemsHorizontally();
    menu4->setPosition(visibleSize.width/4,194+zhenxing->getContentSize().height);
    this->addChild(menu4,2);
    
//攻城列队
    auto gongcheng = Sprite::createWithSpriteFrameName("icon_bangzhu.png");
    auto gongchengitem = MenuItemSprite::create(gongcheng, gongcheng, CC_CALLBACK_1(MainScene::showGongchengTeam,this));
    auto menu5 = Menu::create(gongchengitem,NULL);
    menu5->setPosition(Vec2::ZERO);
    gongchengitem->setPosition(visibleSize.width/2,194+zhenxing->getContentSize().height);
    this->addChild(menu5,2);

//背景
    auto bg = Sprite::create("building.jpeg");
	bg->setPosition(visibleSize.width/2, visibleSize.height/2);
    this->addChild(bg,1);
    auto topleft = Sprite::createWithSpriteFrameName("bg-banner_top_left.png");
    auto topright = Sprite::createWithSpriteFrameName("bg-banner_top_right.png");
    topleft->setAnchorPoint(Vec2(0,1));
    topright->setAnchorPoint(Vec2(1,1));
    topleft->setPosition(0,visibleSize.height-upFrame->getContentSize().height+2);
    topright->setPosition(visibleSize.width,visibleSize.height-upFrame->getContentSize().height+2);
    this->addChild(topleft,2);
    this->addChild(topright,2);
    
//用户等级，名字，银两，声望------------
    char wenzi[32];
    sprintf(wenzi,"%d",99);
    auto level = Label::createWithTTF(wenzi, "字体/华文新魏.TTF", 24);
    auto name = Label::createWithTTF("十八罗汉", "字体/华文新魏.TTF", 24);
    level->setPosition(40,visibleSize.height-upFrame->getContentSize().height-topleft->getContentSize().height/4);
    name->setPosition(50+name->getContentSize().width/2+30,visibleSize.height-upFrame->getContentSize().height-topleft->getContentSize().height/4);
    this->addChild(level,3,USERLEVEL_TAG);
    this->addChild(name,3,USERNAME_TAG);
    
    sprintf(wenzi,"%d",99999);
    auto money = Label::createWithTTF(wenzi, "字体/华文新魏.TTF", 24);
    money->setPosition(visibleSize.width/2+50+money->getContentSize().width/2,visibleSize.height-upFrame->getContentSize().height-topright->getContentSize().height*3/4);
    this->addChild(money,3,USERMONEY_TAG);
    
    sprintf(wenzi,"%d",99999);
    auto reputation = Label::createWithTTF(wenzi, "字体/华文新魏.TTF", 24);
    reputation->setPosition(visibleSize.width*3/4+50+reputation->getContentSize().width/2,visibleSize.height-upFrame->getContentSize().height-topright->getContentSize().height*3/4);
    this->addChild(reputation,3,USERREPUTATION_TAG);

//抽将功能页面HelloWorldLayer加载,MainLayer没用可以删掉
    //TO DO...抽装备选项
    auto _HelloWorldLayer = HelloWorldLayer::create();
    auto _MainLayer = MainLayer::create();
    if(_HelloWorldLayer){
        _HelloWorldLayer->setDelegator(_MainLayer);
        this->addChild(_HelloWorldLayer,1,HELLOWORLD_LAYER_TAG);
    }
    if(_MainLayer)
        this->addChild(_MainLayer,1);
    
//载入数据-----------------------
    //string dou = FileUtils::getInstance()->fullPathForFilename("wholeheros.db");
    //log("存放路径为%s",dou.c_str());
    //string documentpath1 = FileUtils::getInstance()->fullPathForFilename("UserInfo.db");
    
    log("存放路径为%s",documentpath1.c_str());
    GameStat::getInstance()->readFromUserInfo("Users_Info", documentpath1.c_str());
    GameStat::getInstance()->readFromUserHeros("User_hero", documentpath1.c_str(), 0);

    char sql[1024];
    sprintf(sql, "select * from User_equip where username = \"%s\"", user_name.c_str());//根据用户名读取英雄
    GameStat::getInstance()->readfromEquipInfo("User_equip", documentpath1.c_str(),sql);
    
    //将玩家信息：等级、名字、银两、声望、元宝等从服务器加载
    MainScene::resetUserInfo();
    
//-------------------------------

//载入攻城车
    GongChengType gongcheng1;
    gongcheng1.engname = "gongchengche";
    gongcheng1.type = 1;
    gongcheng1.attack = 210;
    gongcheng1.durable = 4;
    gongcheng1.attack_type = 1;
    gongcheng1.bullet_type = 1;
    GongChengType gongcheng2;
    gongcheng2.engname = "toushiche";
    gongcheng2.type = 1;
    gongcheng2.attack = 240;
    gongcheng2.durable = 3;
    gongcheng2.attack_type = 1;
    gongcheng2.bullet_type = 1;
    
    GongChengType gongcheng3;
    gongcheng3.engname = "paoshiche";
    gongcheng3.type = 1;
    gongcheng3.attack = 260;
    gongcheng3.durable = 3;
    gongcheng3.attack_type = 1;
    gongcheng3.bullet_type = 1;
    GongChengType gongcheng4;
    gongcheng4.engname = "nuche";
    gongcheng4.type = 2;
    
    gongcheng4.attack = 210;
    gongcheng4.durable = 5;
    gongcheng4.attack_type = 1;
    gongcheng4.bullet_type = 2;
    
    GongChengType gongcheng5;
    gongcheng5.engname = "junuche";
    gongcheng5.type = 2;
    gongcheng5.attack = 350;
    gongcheng5.durable = 3;
    gongcheng5.attack_type = 1;
    gongcheng5.bullet_type = 2;
    
    GongChengType gongcheng6;
    gongcheng6.engname = "liannu";
    gongcheng6.type = 2;
    gongcheng6.attack = 200;
    gongcheng6.durable = 5;
    gongcheng6.attack_type = 3;
    gongcheng6.bullet_type = 2;
    GameStat::getInstance()->userqixie.push_back(gongcheng1);
    GameStat::getInstance()->userqixie.push_back(gongcheng2);
    GameStat::getInstance()->userqixie.push_back(gongcheng3);
    GameStat::getInstance()->userqixie.push_back(gongcheng4);
    GameStat::getInstance()->userqixie.push_back(gongcheng5);
    GameStat::getInstance()->userqixie.push_back(gongcheng6);
    
    //敌军守城器械赋值
    GameStat::getInstance()->enemyCompeteQixie[0] = gongcheng6;
    GameStat::getInstance()->enemyCompeteQixie[0].position = 0;
    
    GameStat::getInstance()->enemyCompeteQixie[1] = gongcheng5;
    GameStat::getInstance()->enemyCompeteQixie[1].position = 1;
    
    GameStat::getInstance()->enemyCompeteQixie[2] = gongcheng4;
    GameStat::getInstance()->enemyCompeteQixie[2].position = 2;
    

//进入新手指导
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainScene::receive_notice_from_battle), "newPlayer", NULL);//一般是从战斗场景跳回调用
    if(in_newplayer_mode)
        MainScene::newPlayerStart(new_stage);

    return true;
}

void MainScene::receive_notice_from_battle(Ref* pSender){
    MainScene::resetUserInfo();
    MainScene::newPlayerStart(new_stage);
}
void MainScene::resetUserInfo(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto upFrame = Sprite::createWithSpriteFrameName("icon_shanghuabian.png");
    auto topright = Sprite::createWithSpriteFrameName("bg-banner_top_right.png");
    
    auto name = (Label*)this->getChildByTag(USERNAME_TAG);
    auto level = (Label*)this->getChildByTag(USERLEVEL_TAG);
    auto money = (Label*)this->getChildByTag(USERMONEY_TAG);
    auto reputation = (Label*)this->getChildByTag(USERREPUTATION_TAG);

    name->setString(user_name);
    char wenzi[64];
    sprintf(wenzi,"%d",user_level);
    level->setString(wenzi);
    sprintf(wenzi,"%d",user_money);
    money->setString(wenzi);
    sprintf(wenzi,"%d",user_shengwang);
    reputation->setString(wenzi);

    level->setPosition(40,visibleSize.height-upFrame->getContentSize().height-topright->getContentSize().height/4);
    name->setPosition(50+name->getContentSize().width/2+30,visibleSize.height-upFrame->getContentSize().height-topright->getContentSize().height/4);
    money->setPosition(visibleSize.width/2+50+money->getContentSize().width/2,visibleSize.height-upFrame->getContentSize().height-topright->getContentSize().height*3/4);
    reputation->setPosition(visibleSize.width*3/4+50+reputation->getContentSize().width/2,visibleSize.height-upFrame->getContentSize().height-topright->getContentSize().height*3/4);

}
//新手引导
void MainScene::newPlayerStart(int type){
    if(type==0){//第一阶段
        createNewPlayerLayer(0);
    }
    else if(type==1){//第二阶段
        if(new_step==0){
            createNewPlayerLayer(3);
            new_step++;
        }

    }
    else if(type==2){//第三阶段开始
        if(new_step==0){
            createNewPlayerLayer(7);
        }
    }
    else if(type==3){//第四阶段
        if(new_step==0){
            createNewPlayerLayer(9);
        }

    }
    
}

void MainScene::createNewPlayerLayer(int type){
    
    auto _newPlayerLayer = new NewPlayerLayer();
    _newPlayerLayer->init(type);
    this->addChild(_newPlayerLayer,5,NEWPLAYER_LAYER_TAG);

    //new_step++;

}

void MainScene::bottomFunction(Ref* pSender){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto basicFunction = (MenuItemSprite*)pSender;
    int basicTag = basicFunction->getTag();
    
    //TO DO....清除所有子节点
    this->removeChildByTag(SHIELD_LAYER_TAG);
    this->removeChildByTag(HELLOWORLD_LAYER_TAG);
    this->removeChildByTag(MAIN_LAYER_TAG);//800
    this->removeChildByTag(TEAMLIST_LAYER_TAG);//801
    this->removeChildByTag(ZHENGZHAN_LAYER_TAG);//802
    this->removeChildByTag(ACTIVITY_LAYER_TAG);//803
    this->removeChildByTag(SHOP_LAYER_TAG);//804
    this->removeChildByTag(ZHENFA_LAYER_TAG);//805
    this->removeChildByTag(ZHENRONG_LAYER_TAG);//806
    this->removeChildByTag(HEROINFO_LAYER_TAG);//810

    if(basicTag!=11){
        auto _shieldLayer = new ShieldLayer();
        _shieldLayer->init(0);
        if(_shieldLayer){
            this->addChild(_shieldLayer,3,SHIELD_LAYER_TAG);
        }
    }
    if (basicTag==11) {
        CCLOG("切换到Main页");
        if (this->getChildByTag(MAIN_MENU_TAG)) {
            CCLOG("可以用");
        }
        //((MenuItemSprite*)this->getChildByTag(MAIN_MENU_TAG)->getChildByTag(11))->selected();
        auto _HelloWorldLayer = HelloWorldLayer::create();
        if(_HelloWorldLayer){
            //_HelloWorldLayer->setDelegator(_MainLayer);
            this->addChild(_HelloWorldLayer,1,HELLOWORLD_LAYER_TAG);
        }
        if(new_stage==3){//新手指导模式,进行剧情副本
            //this->removeChildByTag(NEWPLAYER_LAYER_TAG);
            createNewPlayerLayer(10);
        }
    }

    if (basicTag==12) {
        CCLOG("切换到阵容页");
        
        auto _ZhenRongLayer = ZhenRongLayer::create();
        if(_ZhenRongLayer){
            this->addChild(_ZhenRongLayer,4,ZHENRONG_LAYER_TAG);
        }
        if(new_stage>0){//新手指导模式
            //this->removeChildByTag(NEWPLAYER_LAYER_TAG);
            if(new_stage==1)
                createNewPlayerLayer(4);

        }

    }
    if (basicTag==13) {
        CCLOG("切换到战争页");
        auto _ZhengZhanLayer = ZhengZhanLayer::create();
        if(_ZhengZhanLayer)
            this->addChild(_ZhengZhanLayer,4,ZHENGZHAN_LAYER_TAG);
        if(new_stage==0){//新手指导模式,进行剧情副本
            createNewPlayerLayer(1);
        }
        else if(new_stage==2){//新手指导模式,进行剧情副本
            createNewPlayerLayer(8);
        }
        else if(new_stage==3){//新手指导模式,进行剧情副本
            //this->removeChildByTag(NEWPLAYER_LAYER_TAG);
            createNewPlayerLayer(8);
        }
    }
    if (basicTag==14) {
        CCLOG("切换到活动页");
        cocosbuilder::NodeLoaderLibrary *ccNodeLoaderLibrary = cocosbuilder::NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
        cocosbuilder::CCBReader *ccbReader = new cocosbuilder::CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();
        auto ccbCriticbg = ccbReader->readNodeGraphFromFile("ccb-buff/showCritic.ccbi", this);
        this->addChild(ccbCriticbg,5);
        ccbCriticbg->runAction(Sequence::create(DelayTime::create(0.5f),RemoveSelf::create(), NULL));
        ccbCriticbg->setPosition(0,0);
    }
    if (basicTag==15) {
        CCLOG("切换到抽奖&集市页");
        //auto layercol = LayerColor::create(Color4B(0,0,0,122), visibleSize.width, visibleSize.height);
        //layercol->setPosition(visibleSize.width/2,visibleSize.height/2);
        //this->addChild(layercol,8);
        cocosbuilder::NodeLoaderLibrary *ccNodeLoaderLibrary = cocosbuilder::NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
        cocosbuilder::CCBReader *ccbReader = new cocosbuilder::CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();
        /*auto ccbCriticbg = ccbReader->readNodeGraphFromFile("skill_zhurigong_fly.ccbi", this);
        this->addChild(ccbCriticbg,6,5);
        ccbCriticbg->setPosition(visibleSize.width/4,visibleSize.height/2);
        auto seq = Sequence::create(MoveTo::create(0.3f, Point(visibleSize.width*3/4,visibleSize.height/2)),CallFuncN::create(CC_CALLBACK_1(MainScene::skill_hurt, this)),RemoveSelf::create(),NULL);
        seq->setTag(20);
         ccbCriticbg->runAction(seq);
        */
        auto ccbCriticbg = ccbReader->readNodeGraphFromFile("ccb_fx/RuYunLong.ccbi", this);
        ccbCriticbg->setPosition(visibleSize.width/2,visibleSize.height/2);
        this->addChild(ccbCriticbg,5);
        /*auto bulletsp = Sprite::createWithSpriteFrameName("normal_bullet_02.png");
        bulletsp->setPosition(visibleSize.width/4,visibleSize.height/2);
        this->addChild(bulletsp,3);
        auto seq = Sequence::create(MoveTo::create(0.3f, Point(visibleSize.width*3/4,visibleSize.height/2)),CallFuncN::create(CC_CALLBACK_1(MainScene::skill_hurt, this)),RemoveSelf::create(),NULL);
        bulletsp->runAction(seq);
*/

        //schedule(schedule_selector(MainScene::skill_hurt),0.25f);
    }

}


void MainScene::skill_hurt(Ref* pSender){
    //this->getChildByTag(5)->removeFromParentAndCleanup(true);
    //unschedule(schedule_selector(MainScene::skill_hurt));
    cocosbuilder::NodeLoaderLibrary *ccNodeLoaderLibrary = cocosbuilder::NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
    cocosbuilder::CCBReader *ccbReader = new cocosbuilder::CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();
    auto ccbCriticbg = ccbReader->readNodeGraphFromFile("skill_zhurigong_hurt.ccbi", this);
    this->addChild(ccbCriticbg,6,6);
    ccbCriticbg->setPosition(640*3/4,568);
     /*
    auto hurtsp = Sprite::createWithSpriteFrameName("normal_bullet_01.png");
    hurtsp->setPosition(640*3/4,568);
    this->addChild(hurtsp,3);
    hurtsp->runAction(Sequence::create(DelayTime::create(0.1f),RemoveSelf::create(), NULL));
      */
    pthread_attr_t tarrt;
}



void MainScene::showZhenXing(Ref* pSender){
    this->removeChildByTag(HELLOWORLD_LAYER_TAG);
    this->removeChildByTag(MAIN_LAYER_TAG);//800
    this->removeChildByTag(SHIELD_LAYER_TAG);
    auto _shieldLayer = new ShieldLayer();
    _shieldLayer->init(0);
    if(_shieldLayer){
        this->addChild(_shieldLayer,3,SHIELD_LAYER_TAG);
    }

    auto _ZhenFa = ZhenFa::create();
    if(_ZhenFa)
        this->addChild(_ZhenFa,4,ZHENFA_LAYER_TAG);
}
void MainScene::showGongchengTeam(Ref* pSender){
    this->removeChildByTag(HELLOWORLD_LAYER_TAG);
    this->removeChildByTag(MAIN_LAYER_TAG);//800
    this->removeChildByTag(SHIELD_LAYER_TAG);
    
    auto _shieldLayer = new ShieldLayer();
    _shieldLayer->init(0);
    if(_shieldLayer){
        this->addChild(_shieldLayer,3,SHIELD_LAYER_TAG);
    }

    auto _GongchengZhenfa = GongchengZhenfa::create();
    if(_GongchengZhenfa)
        this->addChild(_GongchengZhenfa,4,GONGCHENGZHENFA_LAYER_TAG);
}


void MainScene::showTeamList(Ref* pSender){
    this->removeChildByTag(HELLOWORLD_LAYER_TAG);
    this->removeChildByTag(MAIN_LAYER_TAG);//800
    this->removeChildByTag(SHIELD_LAYER_TAG);
    auto _shieldLayer = new ShieldLayer();
    _shieldLayer->init(0);
    if(_shieldLayer){
        this->addChild(_shieldLayer,3,SHIELD_LAYER_TAG);
    }

    CCLOG("显示所有武将！");
    auto _TeamListLayer = new TeamListLayer();
    _TeamListLayer->init(-1);
    if (_TeamListLayer) {
        this->addChild(_TeamListLayer,4,TEAMLIST_LAYER_TAG);
    }
}






#include "LayerManager/MainScene.h"
#include "LayerManager/HelloWorldLayer.h"
#include "LayerManager/ZhengZhanLayer.h"

#include "HeroList/TeamListLayer.h"
#include "HeroList/ZhenFa.h"
#include "Compete/CompeteScene.h"

#include "ModifiedSourceCode/DControlButton.h"
#include "ModifiedSourceCode/DMenu.h"
#include "GameStat.h"
#include "WholeMapLayer.h"
#include "cocos-ext.h"
#include <ui/CocosGUI.h>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
// on "init" you need to initialize your instance
bool ZhengZhanLayer::init()
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
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("commonBtn.plist");
    auto bg = Sprite::create("bg_neirong.png");
    bg->setPosition(visibleSize.width/2,visibleSize.height/2+33);
    this->addChild(bg,1);
    auto titleicon = Sprite::create("icon_titlebg.png");
    titleicon->setPosition(visibleSize.width/2,visibleSize.height-(176+33));
    this->addChild(titleicon,1);
    auto titlelabel = Sprite::createWithSpriteFrameName("title_zhengzhan.png");//加“阵容”标题
    titleicon->addChild(titlelabel,2);
    titlelabel->setPosition(titleicon->getContentSize().width/2,titleicon->getContentSize().height/2);
    //TO DO...返回主页
    /*
    auto fanhuiItem = MenuItemImage::create("btn_arrow_right_disable.png","btn_arrow_right_disable.png",this,NULL);
    auto fanhui = Sprite::create("btn_arrow_right_disable.png");
    auto wujianghun = MenuItemSprite::create(fanhui,fanhui,CC_CALLBACK_1(ZhengZhanLayer::back, this));
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
*/
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
    //btnLabel->setScaleRatio(0.8f);
    //btnLabel->setScale(1.2f);
    
    //btnLabel->setPreferredSize(wenzi_fanhui->getContentSize());
    btn->setPreferredSize(fanhui->getContentSize());
    btn->setPosition(btn->getContentSize().width/2,visibleSize.height-207);
    //btnLabel->setPosition(btn->getContentSize().width/2, btn->getContentSize().height/2);
    sprite9label->setPosition(btn->getContentSize().width/2, btn->getContentSize().height/2);
    btn->addTargetWithActionForControlEvents(this, cccontrol_selector(ZhengZhanLayer::back), Control::EventType::TOUCH_UP_INSIDE);
    btn->setBackgroundSpriteForState(sprite9selected, Control::State::HIGH_LIGHTED);
    //显示武将列表
    auto scrollList = ui::ScrollView::create();
    scrollList->setTouchEnabled(true);
    scrollList->setBounceEnabled(true);
    scrollList->setSize(Size(560,700));
    scrollList->setAnchorPoint(Vec2(0.5,0.5));
    scrollList->setPosition(Point(visibleSize.width/2,visibleSize.height/2-50));
    scrollList->setInnerContainerSize(Size(560,800));//TO DO...inner size根据征战的种类
    CCLOG("team size is : %lu",team.size());
    
    this->addChild(scrollList,3,SCROLL_LIST_TAG);
    auto menu = (DMenu*)DMenu::create();//用DMenu，自己加的继承Menu的方法
    menu->setPosition(Point::ZERO);
    scrollList->addChild(menu,6,HERO_LISTMENU_TAG);
    menu->setTouchlimit(scrollList);//判断menu到scrollView边框外，不能触发touch
    menu->setSwallowTouches(false);
    
    auto fubenItem = MenuItemImage::create("icon_juqingfuben.png","icon_juqingfuben.png",CC_CALLBACK_1(ZhengZhanLayer::fuben, this));
    auto menu2 = Menu::create(fubenItem,NULL);
    menu2->setPosition(Vec2::ZERO);
    fubenItem->setPosition(visibleSize.width/2,visibleSize.height/3*2);
    this->addChild(menu2,3);

    auto qiangduoItem = MenuItemImage::create("icon_wanjiatiaozhan.png","icon_wanjiatiaozhan.png",CC_CALLBACK_1(ZhengZhanLayer::qiangduo, this));
    auto menu3 = Menu::create(qiangduoItem,NULL);
    menu3->setPosition(Vec2::ZERO);
    qiangduoItem->setPosition(visibleSize.width/2,visibleSize.height/2);
    this->addChild(menu3,3);

    return true;
}
void ZhengZhanLayer::qiangduo(Ref* pSender){
    auto _WholeMapLayer = new WholeMapLayer();
    _WholeMapLayer->init();
    if(_WholeMapLayer){
        CCLOG("加入选择武将界面");
        this->getParent()->addChild(_WholeMapLayer,5,901);//替换场景，战斗结束再回来
    }

   /* GameStat::getInstance()->isGongcheng = true;
    auto _CompeteScene = CompeteScene::create();
    if(_CompeteScene)
        Director::getInstance()->pushScene(_CompeteScene);//替换场景，战斗结束再回来
*/
    
}
void ZhengZhanLayer::fuben(Ref* pSender){
    //TO DO....把战斗结束将要得到的奖励填好,对应点击的副本
    /*int tag = ((ControlButton*)pSender)->getTag();
    //TO DO...根据tag从database调出对应奖励
    GameStat::getInstance()->exp = exp;
    GameStat::getInstance()->money = money;
    GameStat::getInstance()->gongxun = gongxun;
*/
    GameStat::getInstance()->isGongcheng = false;
    GameStat::getInstance()->isUnlockSkill = 0;

    GameStat::getInstance()->exp = 100;
    GameStat::getInstance()->money = 300;
    GameStat::getInstance()->shengwang = 455;
    //string documentpath2 = FileUtils::getInstance()->fullPathForFilename("wholeheros.db");
    CCLOG("正在新手指导中:%d",in_newplayer_mode);
    GameStat::getInstance()->heroName.clear();
    GameStat::getInstance()->heroIndex.clear();

    if(in_newplayer_mode){
        char sql[1024];
        if(new_stage==0){
            sprintf(sql,"select * from FubenHeros where number = %d",300);
            GameStat::getInstance()->getTotalHeroIndex("FubenHeros", documentpath2.c_str(),sql);//
            randomhero.position = 4;
            enemyteam[4] = randomhero;//录入张角
            sprintf(sql,"select * from FubenHeros where number = %d",400);
            GameStat::getInstance()->getTotalHeroIndex("FubenHeros", documentpath2.c_str(),sql);//
            randomhero.position = 0;
            enemyteam[0] = randomhero;//录入小兵
            randomhero.position = 2;
            enemyteam[2] = randomhero;//录入小兵

            //副本一:给玩家录入曹操，典韦和许褚
            sprintf(sql,"select * from Heros where number = %d",19);
            GameStat::getInstance()->getTotalHeroIndex("Heros", documentpath2.c_str(),sql);//
            //改数据
            randomhero.attackDisplay = 3500;
            randomhero.healthDisplay = 15000;
            randomhero.criticDisplay = 1000;
            randomhero.position = 1;
            userteam[1] = randomhero;//录入许褚
            sprintf(sql,"select * from Heros where number = %d",20);
            GameStat::getInstance()->getTotalHeroIndex("Heros", documentpath2.c_str(),sql);//
            randomhero.attackDisplay = 3200;
            randomhero.healthDisplay = 12000;
            randomhero.agilityDisplay = 1000;
            randomhero.position = 2;
            userteam[2] = randomhero;//录入典韦
            sprintf(sql,"select * from Heros where number = %d",1);
            GameStat::getInstance()->getTotalHeroIndex("Heros", documentpath2.c_str(),sql);//
            randomhero.attackDisplay = 2800;
            randomhero.healthDisplay = 15000;
            randomhero.criticDisplay = 500;
            randomhero.position = 4;
            userteam[4] = randomhero;//录入曹操
            //获得英雄-----------
            //载入战斗结束获得
            GameStat::getInstance()->shengwang = 0;
            GameStat::getInstance()->heroName.push_back("caohong");
            GameStat::getInstance()->heroIndex.push_back(9);
            
            //------------------

        }
        else if(new_stage==2){
            enemyteam.clear();
            sprintf(sql,"select * from FubenHeros where number = %d",301);
            GameStat::getInstance()->getTotalHeroIndex("FubenHeros", documentpath2.c_str(),sql);//
            randomhero.position = 4;
            enemyteam[4] = randomhero;//录入张宝
            sprintf(sql,"select * from FubenHeros where number = %d",400);
            GameStat::getInstance()->getTotalHeroIndex("FubenHeros", documentpath2.c_str(),sql);//
            randomhero.position = 0;
            enemyteam[0] = randomhero;//录入小兵1
            randomhero.position = 2;
            enemyteam[2] = randomhero;//录入小兵1
            
            sprintf(sql,"select * from FubenHeros where number = %d",401);
            GameStat::getInstance()->getTotalHeroIndex("FubenHeros", documentpath2.c_str(),sql);//
            randomhero.position = 3;
            enemyteam[3] = randomhero;//录入小兵2
            randomhero.position = 5;
            enemyteam[5] = randomhero;//录入小兵2

            //副本一:给玩家录入关羽、张飞
            sprintf(sql,"select * from Heros where number = %d",51);
            GameStat::getInstance()->getTotalHeroIndex("Heros", documentpath2.c_str(),sql);//
            //改数据
            randomhero.attackDisplay = 3500;
            randomhero.healthDisplay = 15000;
            randomhero.criticDisplay = 1000;
            randomhero.position = 4;
            userteam[4] = randomhero;//录入关羽
            sprintf(sql,"select * from Heros where number = %d",52);
            GameStat::getInstance()->getTotalHeroIndex("Heros", documentpath2.c_str(),sql);//
            randomhero.attackDisplay = 3200;
            randomhero.healthDisplay = 12000;
            randomhero.agilityDisplay = 1000;
            randomhero.position = 2;
            randomhero.penetrateProp=1;//张飞带穿刺
            userteam[2] = randomhero;//录入张飞
            //获得英雄-----------
            //载入战斗结束获得
            GameStat::getInstance()->shengwang = 0;
            
            //------------------
            
        }
        else if(new_stage==3){
            enemyteam.clear();
            sprintf(sql,"select * from FubenHeros where number = %d",302);
            GameStat::getInstance()->getTotalHeroIndex("FubenHeros", documentpath2.c_str(),sql);//
            randomhero.position = 4;
            enemyteam[4] = randomhero;//录入华雄
            randomhero.penetrateProp = 0;
            sprintf(sql,"select * from FubenHeros where number = %d",400);
            GameStat::getInstance()->getTotalHeroIndex("FubenHeros", documentpath2.c_str(),sql);//
            randomhero.position = 0;
            enemyteam[0] = randomhero;//录入小兵1
            randomhero.position = 2;
            enemyteam[2] = randomhero;//录入小兵1
            
            sprintf(sql,"select * from FubenHeros where number = %d",401);
            GameStat::getInstance()->getTotalHeroIndex("FubenHeros", documentpath2.c_str(),sql);//
            randomhero.position = 3;
            randomhero.relation.clear();
            enemyteam[3] = randomhero;//录入小兵2
            randomhero.position = 5;
            enemyteam[5] = randomhero;//录入小兵2
            
            //副本一:给玩家录入孙坚、韩当、黄盖
            sprintf(sql,"select * from Heros where number = %d",112);
            GameStat::getInstance()->getTotalHeroIndex("Heros", documentpath2.c_str(),sql);//
            //改数据
            randomhero.attackDisplay = 3800;
            randomhero.healthDisplay = 18000;
            randomhero.criticDisplay = 1000;
            randomhero.position = 1;
            userteam[1] = randomhero;//录入孙坚
            sprintf(sql,"select * from Heros where number = %d",114);
            GameStat::getInstance()->getTotalHeroIndex("Heros", documentpath2.c_str(),sql);//
            randomhero.attackDisplay = 3200;
            randomhero.healthDisplay = 15000;
            randomhero.agilityDisplay = 1000;
            randomhero.position = 0;
            //randomhero.penetrateProp=1;//张飞带穿刺
            userteam[0] = randomhero;//录入韩当
            sprintf(sql,"select * from Heros where number = %d",115);
            GameStat::getInstance()->getTotalHeroIndex("Heros", documentpath2.c_str(),sql);//
            randomhero.attackDisplay = 3500;
            randomhero.healthDisplay = 14000;
            randomhero.agilityDisplay = 1000;
            randomhero.position = 2;
            userteam[2] = randomhero;//录入黄盖

            team[0].position = 4;//主角三人顺序
            team[1].position = 3;
            team[2].position = 5;
            //team[2].DoubleMoveProp = 1;
            userteam[4] = team[0];
            userteam[3] = team[1];
            userteam[5] = team[2];
            log("%s两次概率%f;混乱攻击%f",team[team.size()-1].name.c_str(),team[team.size()-1].DoubleMoveProp,team[team.size()-1].chaosProp);
            log("%s两次概率%f;混乱攻击%f",userteam[5].name.c_str(),userteam[5].DoubleMoveProp,userteam[5].chaosProp);
            log("%s两次概率%f;混乱攻击%f",team[2].name.c_str(),team[2].DoubleMoveProp,team[2].chaosProp);

            //获得英雄-----------
            //载入战斗结束获得
            GameStat::getInstance()->shengwang = 0;
            
            //------------------
            
        }
        else if(new_stage>4){
            enemyteam.clear();
            sprintf(sql,"select * from FubenHeros where number = %d",302);
            GameStat::getInstance()->getTotalHeroIndex("FubenHeros", documentpath2.c_str(),sql);//
            randomhero.position = 4;
            enemyteam[4] = randomhero;//录入华雄
            
            sprintf(sql,"select * from FubenHeros where number = %d",400);
            GameStat::getInstance()->getTotalHeroIndex("FubenHeros", documentpath2.c_str(),sql);//
            randomhero.position = 0;
            enemyteam[0] = randomhero;//录入小兵1
            randomhero.position = 2;
            enemyteam[2] = randomhero;//录入小兵1
            
            sprintf(sql,"select * from FubenHeros where number = %d",401);
            GameStat::getInstance()->getTotalHeroIndex("FubenHeros", documentpath2.c_str(),sql);//
            randomhero.position = 3;
            enemyteam[3] = randomhero;//录入小兵2
            randomhero.position = 5;
            enemyteam[5] = randomhero;//录入小兵2
            
            //副本一:给玩家录入孙坚、韩当、黄盖
            sprintf(sql,"select * from Heros where number = %d",112);
            GameStat::getInstance()->getTotalHeroIndex("Heros", documentpath2.c_str(),sql);//
            //改数据
            randomhero.attackDisplay = 3500;
            randomhero.healthDisplay = 15000;
            randomhero.criticDisplay = 1000;
            randomhero.position = 1;
            userteam[1] = randomhero;//录入孙坚
            sprintf(sql,"select * from Heros where number = %d",114);
            GameStat::getInstance()->getTotalHeroIndex("Heros", documentpath2.c_str(),sql);//
            randomhero.attackDisplay = 3200;
            randomhero.healthDisplay = 12000;
            randomhero.agilityDisplay = 1000;
            randomhero.position = 0;
            //randomhero.penetrateProp=1;//张飞带穿刺
            userteam[0] = randomhero;//录入韩当
            sprintf(sql,"select * from Heros where number = %d",115);
            GameStat::getInstance()->getTotalHeroIndex("Heros", documentpath2.c_str(),sql);//
            randomhero.attackDisplay = 3200;
            randomhero.healthDisplay = 12000;
            randomhero.agilityDisplay = 1000;
            randomhero.position = 2;
            userteam[2] = randomhero;//录入黄盖
            
            sprintf(sql,"select * from Heros where number = %d",53);
            GameStat::getInstance()->getTotalHeroIndex("Heros", documentpath2.c_str(),sql);//
            randomhero.attackDisplay = 3200;
            randomhero.healthDisplay = 12000;
            randomhero.agilityDisplay = 1000;
            randomhero.position = 5;
            randomhero.DoubleMoveProp = 1;
            userteam[5] = randomhero;//录入马超
            

            team[0].position = 4;//主角三人顺序
            team[2].position = 3;
            userteam[4] = team[0];
            userteam[3] = team[2];
            
            
            //获得英雄-----------
            //载入战斗结束获得
            GameStat::getInstance()->shengwang = 0;
            //------------------
            
        }

    }
    //log("张角名字是:%s",enemyteam[4].engname.c_str());
    
    auto _CompeteScene = CompeteScene::create();
    if(_CompeteScene)
        Director::getInstance()->pushScene(_CompeteScene);//替换场景，战斗结束再回来
    
}
void ZhengZhanLayer::back(Ref* pSender, Control::EventType event){
    CCLOG("remove this layer!");
    auto _HelloWorldLayer = HelloWorldLayer::create();
    if(_HelloWorldLayer){
        //_HelloWorldLayer->setDelegator(_MainLayer);
        this->getParent()->addChild(_HelloWorldLayer,1,HELLOWORLD_LAYER_TAG);
    }

    //TO DO....恢复上一层的touch
    this->getParent()->removeChildByTag(SHIELD_LAYER_TAG);

    this->removeFromParentAndCleanup(true);
}

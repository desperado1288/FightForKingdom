#include "LayerManager/MainScene.h"
#include "GameStat.h"
#include "Compete/CompeteLayer.h"
#include "Compete/BattleResultLayer.h"
#include "Compete/CompeteScene.h"
#include "HeroList/TeamListLayer.h"
#include "PopNewHero.h"
#include <ui/CocosGUI.h>
#include "cocos-ext.h"
#include <cocosbuilder/CocosBuilder.h>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace cocosbuilder;
using namespace cocos2d::extension;
// on "init" you need to initialize your instance

bool BattleResultLayer::init(int resultType)
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("battleWin.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("battleLose.plist");
    
    
    
    //TO DO....
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(BattleResultLayer::onTouchBegan,this);
    listener->onTouchMoved = CC_CALLBACK_2(BattleResultLayer::onTouchMoved,this);
    listener->onTouchEnded = CC_CALLBACK_2(BattleResultLayer::onTouchEnded,this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

    cocosbuilder::NodeLoaderLibrary *ccNodeLoaderLibrary = cocosbuilder::NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
    cocosbuilder::CCBReader *ccbReader = new cocosbuilder::CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();
    
    //读取一个ccbi的文件,生成一个CCNode实例
    //TO DO....完胜，胜利，险胜，失败根据“resultType”
    if(resultType==1){
        ccbResult = ccbReader->readNodeGraphFromFile("ccb-res/battleWin1.ccbi", this);
        ccbResult->runAction(Sequence::create(DelayTime::create(2.5f),CallFuncN::create(CC_CALLBACK_1(BattleResultLayer::showStat, this)),NULL));
    }
    else if(resultType==2){
        ccbResult = ccbReader->readNodeGraphFromFile("ccb-res/battleWin2.ccbi", this);
        ccbResult->runAction(Sequence::create(DelayTime::create(2.25f),CallFuncN::create(CC_CALLBACK_1(BattleResultLayer::showStat, this)),NULL));
    }
    else if(resultType==3){//险胜
        ccbResult = ccbReader->readNodeGraphFromFile("ccb-res/battleWin3.ccbi", this);
        ccbResult->runAction(Sequence::create(DelayTime::create(1.85f),CallFuncN::create(CC_CALLBACK_1(BattleResultLayer::showStat, this)),NULL));
    }
    else if(resultType==4){//失败
        ccbResult = ccbReader->readNodeGraphFromFile("ccb-res/battleLose.ccbi", this);
        ccbResult->runAction(Sequence::create(DelayTime::create(1.85f),CallFuncN::create(CC_CALLBACK_1(BattleResultLayer::showStat, this)),NULL));
    }
    
    //设置坐标
    //animationsTest->setPosition(visibleSize.width/3,visibleSize.height*0.4);
    if(ccbResult != NULL) {
        this->addChild(ccbResult,1,CCB_BATTLE_RESULT);
    }
    
    
    
    isBattleStatsDisplayed = 0;//TO DO...表示还没有播放完毕战斗结束数据统计：
    //加入本场MVP，即输出最高人员
    return true;
}
void BattleResultLayer::buttonCallBack(Ref* pSender, Control::EventType event) {
    CCLOG("Touch the Button");
}
void BattleResultLayer::buttonTouchEnded(Ref* pSender, Control::EventType event) {
    CCLOG("touch ended button");
    CCLOG("返回上一层");
    removeFromParentAndCleanup(true);

}

void BattleResultLayer::close(Ref* pSender){
    CCLOG("返回上一层");
    removeFromParentAndCleanup(true);
}

void BattleResultLayer::showStat(Ref* pSender){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    isBattleStatsDisplayed = 1;
    /*string name = GameStat::getInstance()->mvpName;
     string namepic = "touxiang/"+name+".jpg";
     auto mvpSp = Sprite::create(namepic);
     mvpSp->setPosition(373,594);
     this->addChild(mvpSp,2);
     auto mvpLabel = Label::createWithSystemFont("MVP", "Avenir Black", 30);
     mvpLabel->setPosition(271,603);
     this->addChild(mvpLabel,2);
     */
    //经验
    auto expLabel = Label::createWithTTF("经验:", "字体/华文新魏.TTF", 30);
    expLabel->setColor(Color3B::BLACK);
    expLabel->setPosition(visibleSize.width/2-220,visibleSize.height/2+150);
    auto exp = LabelAtlas::create("0123456789-", "battle/number_yellow.png",49,88,'0');
    char str[50];
    sprintf(str, "%d",GameStat::getInstance()->exp);
    exp->setString(str);
    exp->setScale(0.6f);
    exp->setAnchorPoint(Point(0.5,0.5));
    exp->setOpacity(0);
    exp->setPosition(visibleSize.width/2-220+exp->getContentSize().width/2*0.6f+expLabel->getContentSize().width/2+10,visibleSize.height/2+150);
    //exp->setAnchorPoint(Point(0.5,0.5));
    this->addChild(expLabel,2);//
    this->addChild(exp,2,EXP_TAG);//
    //auto delay = DelayTime::create(time);
    exp->runAction(EaseIn::create(FadeIn::create(0.5f), 1));
    //银子
    auto moneyLabel = Label::createWithTTF("银两:", "字体/华文新魏.TTF", 30);
    moneyLabel->setColor(Color3B::BLACK);

    moneyLabel->setPosition(visibleSize.width/2-35,visibleSize.height/2+150);
    auto money = LabelAtlas::create("0123456789-", "battle/number_yellow.png",49,88,'0');
    //char* str;
    sprintf(str, "%d",GameStat::getInstance()->money);
    money->setString(str);
    money->setScale(0.6f);
    money->setOpacity(0);
    money->setPosition(visibleSize.width/2-35+money->getContentSize().width/2*0.6f+moneyLabel->getContentSize().width/2+10,visibleSize.height/2+150);
    money->setAnchorPoint(Point(0.5,0.5));
    this->addChild(moneyLabel,2);//
    this->addChild(money,2,MONEY_TAG);//
    money->runAction(Sequence::create(EaseIn::create(FadeIn::create(0.5f), 1),NULL));
    //功勋
    auto shengwang_label = Label::createWithTTF("声望:", "字体/华文新魏.TTF", 30);
    shengwang_label->setColor(Color3B::BLACK);
    shengwang_label->setPosition(visibleSize.width/2+150,visibleSize.height/2+150);
    auto shengwang = LabelAtlas::create("0123456789-", "battle/number_yellow.png",49,88,'0');
    //char* str;
    sprintf(str, "%d",GameStat::getInstance()->shengwang);
    shengwang->setString(str);
    shengwang->setScale(0.6f);
    shengwang->setOpacity(0);
    shengwang->setPosition(visibleSize.width/2+150+shengwang->getContentSize().width/2*0.6f+shengwang_label->getContentSize().width/2+10,visibleSize.height/2+150);
    shengwang->setAnchorPoint(Point(0.5,0.5));
    this->addChild(shengwang_label,2);//
    this->addChild(shengwang,2,GONGXUN_TAG);//
    //auto fade = FadeIn::create(0.5f);
    //auto delay = DelayTime::create(time);
    shengwang->runAction(Sequence::create(EaseIn::create(FadeIn::create(0.5f), 1),NULL));
    //TO DO....
    
//玩家增加经验、金钱,并修改数据库----------
    user_money += battle_money;
    user_shengwang += battle_shengwang;
    userexpExt += userexp;
    char sql[1024];
    log("用户银两%d,经验%d,声望%d,新手阶段:%d.",user_money,userexpExt,user_shengwang,new_stage);
    //string documentpath1 = FileUtils::getInstance()->fullPathForFilename("UserInfo.db");
    //string documentpath2 = FileUtils::getInstance()->fullPathForFilename("wholeheros.db");

    if(userexpExt>=userexpNext){//用户升级
        //用户信息更改
        userexpExt -= userexpNext;
        user_level++;
        
        //英雄信息更改
        for(auto it = team.begin();it!=team.end();it++){
            it->level = user_level;
            it->attackDisplay = it->attack+it->attackins*(user_level-1);
            it->defenseDisplay = it->defense+it->defenseins*(user_level-1);
            it->healthDisplay = it->health+it->healthins*(user_level-1);
            it->criticDisplay = it->critic+it->criticins*(user_level-1);
            it->agilityDisplay = it->agility+it->agilityins*(user_level-1);
            //存入玩家数据库
            sprintf(sql,"update User_hero set hero_level = %d where username = \"%s\" and hero_index = %d and hero_number = %d",user_level,user_name.c_str(),it->index,it->user_hero_number);
            GameStat::getInstance()->saveToUserInfo("User_hero", documentpath1.c_str(), sql);
        }
        for(auto it = userteam.begin();it!=userteam.end();it++){
            it->second.level = user_level;
            it->second.attackDisplay = it->second.attack+it->second.attackins*(user_level-1);
            it->second.defenseDisplay = it->second.defense+it->second.defenseins*(user_level-1);
            it->second.healthDisplay = it->second.health+it->second.healthins*(user_level-1);
            it->second.criticDisplay = it->second.critic+it->second.criticins*(user_level-1);
            it->second.agilityDisplay = it->second.agility+it->second.agilityins*(user_level-1);
        }
        sprintf(sql,"恭喜您升到了%d级，实力大大提高了",user_level);
        auto upgradelabel = Label::createWithTTF(sql, "字体/华文新魏.TTF", 25);
        upgradelabel->setPosition(visibleSize.width/2,visibleSize.height/2-180);
        upgradelabel->setColor(Color3B::MAGENTA);
        this->addChild(upgradelabel,3);
    }
    //不管升级与否，都要修改的
    sprintf(sql,"update Users_Info set userlevel = %d, new_stage = %d, exp_already = %d, money = %d, yuanbao = %d, reputation = %d where username = \"%s\"",user_level,new_stage,userexpExt,user_money,user_jinzi,user_shengwang,user_name.c_str());
    GameStat::getInstance()->saveToUserInfo("Users_Info", documentpath1.c_str(), sql);
//---------------------
    
//解锁被动------------
    if(is_unlock_skill){
    auto passlabel = Label::createWithTTF("", "字体/华文新魏.TTF", 25);
        if(new_stage==2){//表示第一个曹洪解锁必定成功
            GameStat::getInstance()->unlock_complete = 1;
        }
    if(GameStat::getInstance()->unlock_complete){
        passlabel->setString("成功解锁被动技能！");
        passlabel->setDimensions(160, 100);

        //存数据
        unlock_hero.isPassiveActivated = 1;
        for(auto it = team.begin();it!=team.end();++it){
            if(unlock_hero.index==it->index){//所有一样index的英雄都解锁
                it->isPassiveActivated = 1;
                it->addBlock = 0.3f;
            }
            if(unlock_hero.position<6){
                userteam[unlock_hero.position].isPassiveActivated = 1;
            }
        }
        sprintf(sql,"update User_hero set has_passive_skill = %d where username = \"%s\" and hero_index = %d and hero_number = %d",unlock_hero.isPassiveActivated,user_name.c_str(),unlock_hero.index,unlock_hero.user_hero_number);
        GameStat::getInstance()->saveToUserInfo("User_hero", documentpath1.c_str(), sql);
        __NotificationCenter::getInstance()->postNotification("change_unlock",(Ref*)(intptr_t)(new_stage));

    }else{
        passlabel->setString("被动技能触发次数不足，请您再进行一次解锁战");
        passlabel->setDimensions(160, 130);

    }
    this->addChild(passlabel,3);
    
    auto unlockhero = Sprite::create(heropath(unlock_hero.engname));
    unlockhero->setPosition(visibleSize.width/2-150,visibleSize.height/2);
    this->addChild(unlockhero,3);
    passlabel->setPosition(unlockhero->getPositionX(),unlockhero->getPositionY()-unlockhero->getContentSize().height/2-passlabel->getContentSize().height/2-50);
    passlabel->setColor(Color3B::MAGENTA);
    
    auto unlockheroname = Label::createWithTTF(unlock_hero.name, "字体/华文新魏.TTF", 25);
    unlockheroname->setPosition(visibleSize.width/2+100,visibleSize.height/2-unlockheroname->getContentSize().height/2-20);
    unlockheroname->setOpacity(0);
    if(mvp_hero.quality==1)
        unlockheroname->setColor(Color3B::MAGENTA);
    else if(mvp_hero.quality==2)
        unlockheroname->setColor(Color3B::BLUE);
    else if(mvp_hero.quality==3)
        unlockheroname->setColor(Color3B::GREEN);
    else if(mvp_hero.quality==0)
        unlockheroname->setColor(Color3B::ORANGE);
    }
//-------------------
    //获得武将
    //char sql[1024];
    GameStat::getInstance()->readFromUserInfo("Users_Info", documentpath1.c_str());

    float time = 0.5f;
    for(int i=0;i<GameStat::getInstance()->heroName.size();i++){
        sprintf(sql,"select * from Heros where number = %d",GameStat::getInstance()->heroIndex[i]);
        GameStat::getInstance()->getTotalHeroIndex("Heros", documentpath2.c_str(),sql);//

        auto herosp = Sprite::create(heropath(GameStat::getInstance()->heroName[i]));
        herosp->setPosition(visibleSize.width/2-150+i*herosp->getContentSize().width*1.5,visibleSize.height/2);
        herosp->setOpacity(0);
        this->addChild(herosp,2,GETHERO_TAG+i);
        //英雄名字
        auto heronamesp = Label::createWithTTF(GameStat::getInstance()->random_hero.name, "字体/华文新魏.TTF", 25);
        heronamesp->setPosition(visibleSize.width/2-150+i*herosp->getContentSize().width*1.5,visibleSize.height/2-herosp->getContentSize().height/2-20);
        heronamesp->setOpacity(0);
        this->addChild(heronamesp,2,GETHERO_NAME_TAG+i);
        if(GameStat::getInstance()->random_hero.quality==1)
            heronamesp->setColor(Color3B::MAGENTA);
        else if(GameStat::getInstance()->random_hero.quality==2)
            heronamesp->setColor(Color3B::BLUE);
        else if(GameStat::getInstance()->random_hero.quality==3)
            heronamesp->setColor(Color3B::GREEN);
        else if(GameStat::getInstance()->random_hero.quality==0)
            heronamesp->setColor(Color3B::ORANGE);

        herosp->runAction(Sequence::create(DelayTime::create(0.5f+i*0.5f),EaseIn::create(FadeIn::create(0.5f), 1),NULL));
        heronamesp->runAction(Sequence::create(DelayTime::create(0.5f+i*0.5f),EaseIn::create(FadeIn::create(0.5f), 1),NULL));
        time+=0.5f;
    }
//获得新装备
    for(int i=0;i<equip_index.size();i++){
        sprintf(sql,"select * from Zhuangbei where type = %d and number = %d",equip_index[i].first,equip_index[i].second);
        GameStat::getInstance()->getEquipInfo("Zhuangbei", documentpath2.c_str(),sql);//
        //存储-----------
        sprintf(sql,"\"%s\",%d,1,%d,%d,0,7",GameStat::getInstance()->userName.c_str(),randomzhuangbei.index,user_level,randomzhuangbei.type);
        GameStat::getInstance()->saveToSQLite("User_equip", documentpath1.c_str(),sql);//
        sprintf(sql, "select * from User_equip where username = \"%s\"", user_name.c_str());//根据用户名读取英雄
        GameStat::getInstance()->readfromEquipInfo("User_equip", documentpath1.c_str(), sql);
        //---------------
        auto weaponsp = Sprite::create(weaponpath(randomzhuangbei.engname));
        weaponsp->setPosition(visibleSize.width/2-150+i*weaponsp->getContentSize().width*1.5,visibleSize.height/2);
        weaponsp->setOpacity(0);
        this->addChild(weaponsp,2);
        //装备名字
        auto weaponnamesp = Label::createWithTTF(randomzhuangbei.name, "字体/华文新魏.TTF", 25);
        weaponnamesp->setPosition(visibleSize.width/2-150+i*weaponsp->getContentSize().width*1.5,visibleSize.height/2-weaponsp->getContentSize().height/2-20);
        weaponnamesp->setOpacity(0);
        this->addChild(weaponnamesp,2);
        if(randomzhuangbei.quality==1)
            weaponnamesp->setColor(Color3B::MAGENTA);
        else if(randomzhuangbei.quality==2)
            weaponnamesp->setColor(Color3B::BLUE);
        else if(randomzhuangbei.quality==3)
            weaponnamesp->setColor(Color3B::GREEN);
        else if(randomzhuangbei.quality==0)
            weaponnamesp->setColor(Color3B::ORANGE);
        
        weaponsp->runAction(Sequence::create(DelayTime::create(0.5f+i*0.5f),EaseIn::create(FadeIn::create(0.5f), 1),NULL));
        weaponnamesp->runAction(Sequence::create(DelayTime::create(0.5f+i*0.5f),EaseIn::create(FadeIn::create(0.5f), 1),NULL));
        time+=0.5f;
    }
//-------------------
    if(new_stage==1){//新英雄加入
        auto herosp = Sprite::create(heropath(GameStat::getInstance()->heroName[0]));
        auto notice = Label::createWithTTF("自动加入阵中", "字体/华文新魏.TTF", 30);
        notice->setPosition(visibleSize.width/2-150,visibleSize.height/2-herosp->getContentSize().height/2-50);
        notice->setColor(Color3B::BLACK);
        notice->setOpacity(0);
        notice->runAction(Sequence::create(DelayTime::create(0.5f),EaseIn::create(FadeIn::create(0.5f), 1),NULL));
        this->addChild(notice,2);
    }
    //MVP
    auto herosp = Sprite::create(heropath(mvp_hero.engname));
    herosp->setPosition(visibleSize.width/2+100,visibleSize.height/2);
    herosp->setOpacity(0);
    this->addChild(herosp,2,MVP_TAG);
    
    //MVP标题
    auto mvplabel = Label::createWithTTF("MVP：", "字体/华文新魏.TTF", 30);
    mvplabel->setColor(Color3B::ORANGE);
    mvplabel->setPosition(visibleSize.width/2,visibleSize.height/2);
    this->addChild(mvplabel,2);
    //mvp英雄名字
    auto heronamesp = Label::createWithTTF(mvp_hero.name, "字体/华文新魏.TTF", 25);
    heronamesp->setPosition(visibleSize.width/2+100,visibleSize.height/2-herosp->getContentSize().height/2-20);
    heronamesp->setOpacity(0);
    if(mvp_hero.quality==1)
        heronamesp->setColor(Color3B::MAGENTA);
    else if(mvp_hero.quality==2)
        heronamesp->setColor(Color3B::BLUE);
    else if(mvp_hero.quality==3)
        heronamesp->setColor(Color3B::GREEN);
    else if(mvp_hero.quality==0)
        heronamesp->setColor(Color3B::ORANGE);

    this->addChild(heronamesp,2,MVP_NAME_TAG);
    herosp->runAction(Sequence::create(DelayTime::create(time),EaseIn::create(FadeIn::create(0.5f), 1),NULL));
    heronamesp->runAction(Sequence::create(DelayTime::create(time),EaseIn::create(FadeIn::create(0.5f), 1),CallFuncN::create(CC_CALLBACK_1(BattleResultLayer::showStatEnded, this)),NULL));

}
void BattleResultLayer::showStatEnded(Ref* pSender){
    if(GameStat::getInstance()->heroName.size()>0){//说明有新英雄
        isBattleStatsDisplayed = 2;
    }
    else
        isBattleStatsDisplayed = 3;
}
bool BattleResultLayer::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){
    CCLOG("On Touch Began!");
    if(isBattleStatsDisplayed==0)
        return false;
    else if(isBattleStatsDisplayed==1){
        //TO DO...以后会有掉落物品和更多项
        this->getChildByTag(EXP_TAG)->setOpacity(255);
        this->getChildByTag(MONEY_TAG)->setOpacity(255);
        this->getChildByTag(GONGXUN_TAG)->setOpacity(255);
        if(GameStat::getInstance()->heroName.size()>0){//说明有新英雄
            isBattleStatsDisplayed = 2;
        }
        else
            isBattleStatsDisplayed = 3;
    }
    else if(isBattleStatsDisplayed==2){
        if(GameStat::getInstance()->heroName.size()>0){
            auto _popnewhero = new PopNewHero();
            _popnewhero->init(0);
            if(_popnewhero){
                this->getParent()->addChild(_popnewhero,5);
            }
        }
        isBattleStatsDisplayed = 3;
    }
    else if(isBattleStatsDisplayed==3){
        this->removeChildByTag(CCB_BATTLE_RESULT);
        //TO DO....去掉战斗场景,以后加场景切换动画，需要修改popScene代码
        //清除战场获得英雄
        GameStat::getInstance()->heroName.clear();
        GameStat::getInstance()->heroIndex.clear();

        __NotificationCenter::getInstance()->postNotification("newPlayer",(Ref*)(intptr_t)(new_stage));

        Director::getInstance()->popScene();
        //this->getParent()->removeChildByTag(101);
    }
    return true;
}

void BattleResultLayer::onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event){
    CCLOG("on touch moved");
}
void BattleResultLayer::onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event){
    CCLOG("On Touch Ended!");
}

BattleResultLayer::BattleResultLayer(){}
BattleResultLayer::~BattleResultLayer(){}



#include "LayerManager/MainScene.h"
#include "GameStat.h"
#include "Compete/CompeteLayer.h"
#include "Compete/CompeteRole.h"
#include "Compete/BattleResultLayer.h"
#include "HeroList/TeamListLayer.h"
#include "GongchengQixie/QixieListLayer.h"
#include <map>
USING_NS_CC;
using namespace std;

// on "init" you need to initialize your instance
void CompeteLayer::endJiazai(){
    if(new_stage==0&&juqing_move>=1){//即开场剧情结束之后可跳过
        //结束战斗
    //去除曹操三人
        userteam.erase(userteam.find(1));
        userteam.erase(userteam.find(2));
        userteam.erase(userteam.find(4));
        
    //曹洪加入
        char sql[64];
        sprintf(sql,"\"%s\",%d,%d,1,1,1,0",GameStat::getInstance()->userName.c_str(),9,1);
        GameStat::getInstance()->saveToSQLite("User_hero", documentpath1.c_str(),sql);
        
        /*
        sprintf(sql,"select * from Heros where number = %d",9);
        GameStat::getInstance()->getTotalHeroIndex("Heros", "/Users/dingyi/wholeheros.db",sql);//
        randomhero.position = 1;
        randomhero.user_hero_number = 1;
        randomhero.isPassiveActivated = 0;
        userteam[1] = randomhero;
        team.push_back(randomhero);
        */
        team.clear();
        userteam.clear();
        GameStat::getInstance()->readFromUserHeros("User_hero", documentpath1.c_str(), 0);

        
    }
    else if(new_stage==2&&juqing_move>=1){
        //去除张飞关羽
        userteam.erase(userteam.find(2));
        userteam.erase(userteam.find(4));
        
    }
    else if(new_stage==3&&juqing_move>=1){//副本三结束
        //结束战斗
        //去除孙坚三人
        userteam[0] = userteam[4];
        userteam[1] = userteam[3];
        userteam[2] = userteam[5];
        userteam[0].position = 0;
        userteam[1].position = 1;
        userteam[2].position = 2;
        
        userteam.erase(userteam.find(3));
        userteam.erase(userteam.find(4));
        userteam.erase(userteam.find(5));
        team[0].position = 0;//主角三人顺序
        team[1].position = 1;
        team[2].position = 2;
        //team[2].DoubleMoveProp = 1;
        
        //获得青铜剑
        equip_index.push_back(pair<int,int>(1,30));
    }
    new_stage++;
    new_step = 0;
    end_delay_time = 0.5f;
    scheduleUpdate();
}
void CompeteLayer::buttonTouchEnded(Ref* pSender, Control::EventType event) {
    //CCLOG("touch ended button");
    int tag = ((ControlButton*)pSender)->getTag();
    if (tag==20) {
        if(juqing_move>0){
            CCLOG("执行跳过战斗动画");
            unschedule(schedule_selector(CompeteLayer::playFight));
            unschedule(schedule_selector(CompeteLayer::dialog));
            CompeteLayer::endJiazai();
        
        }
    }
    if (tag==21) {
        CCLOG("重播战斗动画");
        unschedule(schedule_selector(CompeteLayer::playFight));//TO DO加上if(如果在运行schedule)
        for(int i=0;i<competeTeam.size();i++){
            userRole[i]->recover();
            enemyRole[i]->recover();
        }
        beginTime = 1.0f;
        animation_index = 0;
        if(juqing_move==2||juqing_move==0)
            return;
        schedule(schedule_selector(CompeteLayer::playFight),0.01f);
    }
}

bool CompeteLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    isgongchengzhan = GameStat::getInstance()->isGongcheng;

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto listener = EventListenerTouchOneByOne::create();//加入触摸监听器
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(CompeteLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(CompeteLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(CompeteLayer::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    //TO DO...以后加读取战斗地图
    auto competeBackground = Sprite::create("building.jpeg");
    competeBackground->setPosition(visibleSize.width/2,visibleSize.height/2);
    this->addChild(competeBackground,1);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("card.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("commonBtn.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UI/res_fx/normal_bullet.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("dialog/dialogueLeft.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("dialog/dialogueRight.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UI/res_fx/hudun.plist");

    //--------------
    
//设定各英雄摆放位置
    auto sp = Sprite::createWithSpriteFrameName("4_btm.png");
    float height = sp->getContentSize().height*0.69f;
    
    for(int i=0;i<6;i++){
        if(i>=3){//TO DO....把position写成条件判断
            if(isgongchengzhan){
                userposition_withqixie.push_back(Vec2((visibleSize.width/2+(i-4)*(186)),(height-62)));
                enemyposition_withqixie.push_back(Vec2((visibleSize.width/2+(i-4)*(186)),visibleSize.height-(height-62)));
                
                
            }else{
                userposition.push_back(Vec2((visibleSize.width/2+(i-4)*(186)),(height+88)));
                enemyposition.push_back(Vec2((visibleSize.width/2+(i-4)*(186)),visibleSize.height-(height+88)));
            }
        }
        else{
            if(isgongchengzhan){
                userposition_withqixie.push_back(Vec2((visibleSize.width/2+(i-1)*(186)),(height*2-62+24)));
                enemyposition_withqixie.push_back(Vec2((visibleSize.width/2+(i-1)*(186)),visibleSize.height-(height*2-62+24)));
                user_qixieposition.push_back(Vec2((visibleSize.width/2+(i-1)*(186)),(height*3-62+48)));
                enemy_qixieposition.push_back(Vec2((visibleSize.width/2+(i-1)*(186)),visibleSize.height-(height*3-62+48)));
                
            }
            else{
                userposition.push_back(Vec2((visibleSize.width/2+(i-1)*(186)),(height*2+88+44)));
                enemyposition.push_back(Vec2((visibleSize.width/2+(i-1)*(186)),visibleSize.height-(height*2+88+44)));
            }
        }
    }
//--------------------------

    //跳过、重播button
    auto skipFightSp = Sprite::createWithSpriteFrameName("btn_main_normal.png");
    auto psc9normal = Scale9Sprite::createWithSpriteFrameName("btn_main_normal.png");
    auto psc9normal1 = Scale9Sprite::createWithSpriteFrameName("btn_main_normal.png");
    auto label_skip = Label::createWithTTF("跳 过", "字体/华文新魏.TTF", 30);
    label_skip->enableShadow();
    auto label_repeatover = Label::createWithTTF("重 播", "字体/华文新魏.TTF", 30);
    label_repeatover->enableShadow();
    //auto psc9selected = Scale9Sprite::createWithSpriteFrameName("btn_blue_disable.png");
    auto button_skip = ControlButton::create(label_skip, psc9normal);
    button_skip->setPreferredSize(skipFightSp->getContentSize());
    button_skip->setPosition(Point(visibleSize.width-skipFightSp->getContentSize().width, 100+skipFightSp->getContentSize().height/2));

    auto button_repeatover = ControlButton::create(label_repeatover, psc9normal1);
    button_repeatover->setPreferredSize(skipFightSp->getContentSize());
    button_repeatover->setPosition(Point(skipFightSp->getContentSize().width, 100+skipFightSp->getContentSize().height/2));

    //button->setTitleColorForState(Color3B(255,255,255), Control::State::HIGH_LIGHTED);
    button_skip->addTargetWithActionForControlEvents(this, cccontrol_selector(CompeteLayer::buttonCallBack), Control::EventType::TOUCH_DOWN);
    button_skip->addTargetWithActionForControlEvents(this, cccontrol_selector(CompeteLayer::buttonTouchEnded), Control::EventType::TOUCH_UP_INSIDE);
    button_repeatover->addTargetWithActionForControlEvents(this, cccontrol_selector(CompeteLayer::buttonCallBack), Control::EventType::TOUCH_DOWN);
    button_repeatover->addTargetWithActionForControlEvents(this, cccontrol_selector(CompeteLayer::buttonTouchEnded), Control::EventType::TOUCH_UP_INSIDE);

    //button->setSelected(true);
    this->addChild(button_skip, 3,20);
    button_skip->setScaleRatio(0.8);
    this->addChild(button_repeatover,3,21);
    button_repeatover->setScaleRatio(0.8);

    
    //开始战斗
    //TODO:...round最多3个，之后拼剩余血量
    round = 1;
    //每回合首先根据敏捷排序（如果有人能全体加属性则在那个步骤里再排序一次）
    //把vector转换成map方便position定位，每回合结尾需要将map通过一次vector排序再传回来
    
    //TO DO...战斗结束后显示每个人得到经验
    //vector<PAIR>,以下是赋值
    if(is_unlock_skill){//表示解锁技能的副本，与正常副本不同
        userVec.push_back(PAIR(1,unlock_hero));//前一界面需载入好unlock_hero
        userMap.insert(map<int,HeroType>::value_type(1, unlock_hero));
        if(unlock_hero.index==9){//曹洪解锁战
            char sql[1024];
            sprintf(sql,"select * from FubenHeros where number = %d",401);
            GameStat::getInstance()->getTotalHeroIndex("FubenHeros", documentpath2.c_str(),sql);//
            for(int i=0;i<6;i++){//曹洪解锁战，对面6个小兵
                randomhero.position = i;
                enemyVec.push_back(PAIR(i,randomhero));
                enemyMap.insert(map<int,HeroType>::value_type(i, randomhero));
            }
        }
        unlock_hit_time = 0;
    }
    else if(isgongchengzhan){//攻城战赋值
        for(auto it = competeTeam.begin();it!=competeTeam.end();++it){
            userVec.push_back(PAIR(it->first,it->second));
        }
        //TO DO....敌方数据还没做
        for(auto it = competeTeam.begin();it!=competeTeam.end();++it){
            enemyVec.push_back(PAIR(it->first,it->second));
        }
        //TO DO....改成map了  userMap就可以不用赋值了,把下边所有userMap都改成userCompeteMap
        for(auto it = competeTeam.begin();it!=competeTeam.end();++it){//完成enemyMap的赋值,英雄的位置为它的KEY
            userMap.insert(map<int,HeroType>::value_type(it->first, it->second));
        }
        
        for(auto it = competeTeam.begin();it!=competeTeam.end();++it){//完成enemyMap的赋值,英雄的位置为它的KEY
            enemyMap.insert(map<int,HeroType>::value_type(it->first, it->second));
        }
        for(auto it = qixiecompeteTeam.begin();it!=qixiecompeteTeam.end();++it){//完成enemyMap的赋值,英雄的位置为它的KEY
            userqixieMap.insert(map<int,GongChengType>::value_type(it->first, it->second));
        }
        
        for(auto it = enemyqixieTeam.begin();it!=enemyqixieTeam.end();++it){//完成enemyMap的赋值,英雄的位置为它的KEY
            enemyqixieMap.insert(map<int,GongChengType>::value_type(it->first, it->second));
        }

        
    }
    else{//正常战斗
        for(auto it = competeTeam.begin();it!=competeTeam.end();++it){
            userVec.push_back(PAIR(it->first,it->second));
        }

        for(auto it = enemy_competeTeam.begin();it!=enemy_competeTeam.end();++it){
            enemyVec.push_back(PAIR(it->first,it->second));
        }
        //改成map了
        for(auto it = competeTeam.begin();it!=competeTeam.end();++it){
            userMap.insert(map<int,HeroType>::value_type(it->first, it->second));
        }

        for(auto it = enemy_competeTeam.begin();it!=enemy_competeTeam.end();++it){//完成enemyMap的赋值,英雄的位置为它的KEY
            enemyMap.insert(map<int,HeroType>::value_type(it->first, it->second));
        }
    }
    
    userWin = false;
    beginTime = 1;
    fightTime = 3;//TO DO....玩家设定战斗播放速度
    normal_dlytime = 1.5f;//
    double_dlytime = 2.1f;//双击每个animation间隔时长
    
    updateTime = 2;
    userIndex = 0;//玩家和地方vector攻击序列,按agility排
    enemyIndex = 0;
    
    //将排序好的vector赋值给map
    sort(userVec.begin(),userVec.end(),CompeteLayer::SortByAgility);
    sort(enemyVec.begin(),enemyVec.end(),CompeteLayer::SortByAgility);
    //确定哪一方先出手。用敏捷来比较
    if(userVec[0].second.agilityDisplay>=enemyVec[0].second.agilityDisplay)//判断条件谁先手
        usersTurn = true;
    else
        usersTurn = false;
    initUsersRole();//初始化所有英雄，位置/CompeteRole
    
    if(new_stage==0||new_stage==2||new_stage==3){
        CompeteLayer::beginDialog();
        return true;
    }
    

    beginFight();//战斗逻辑计算
    return true;
}
//加载对话,以后用XML读取
void CompeteLayer::beginDialog(){
    vector<int> heromove_poses;//移动位置
    vector<int> heromove_user;//移动是否玩家方，0为敌方移动

    if(new_stage==0){//表示新手副本一

        dialog_wenzi.push_back("这是什么地方啊？？好像是个战场");//0
        dialog_direction.push_back(1);
        dialog_wenzi.push_back("什么人,竟敢乱闯！知道这是什么地方吗!");//1
        dialog_direction.push_back(2);
        dialog_wenzi.push_back("...(这人周身一股魔气，仿佛无法靠近)");//2
        dialog_direction.push_back(1);
        dialog_wenzi.push_back("给我上！");//3
        dialog_direction.push_back(2);
        dialog_wenzi.push_back("小贼！我一人就结果了你");//4
        dialog_direction.push_back(2);
        dialog_wenzi.push_back("我靠。。。这要跪了。。");//5
        dialog_direction.push_back(1);
        dialog_wenzi.push_back("这位小兄弟受伤了，快来人！随我杀退张角逆贼");//6
        dialog_direction.push_back(1);
        dialog_wenzi.push_back("是！主公！这位小兄弟暂且休息一下");//7
        dialog_direction.push_back(1);
        dialog_wenzi.push_back("苍天已死，黄天当立;岁在甲子，天下大吉。给我上!");//7
        dialog_direction.push_back(2);
        
        heromove_poses.push_back(4);
        heromove_user.push_back(0);
        heromove_position.push_back(heromove_poses);//张角移动
        heromove_isuser.push_back(heromove_user);
        heromove_dialogindex.push_back(1);//第二段文字之前播放
        
        heromove_poses.clear();
        heromove_user.clear();
        heromove_poses.push_back(0);
        heromove_poses.push_back(2);
        heromove_user.push_back(0);
        heromove_user.push_back(0);
        heromove_position.push_back(heromove_poses);//小兵1和2移动
        heromove_isuser.push_back(heromove_user);
        heromove_dialogindex.push_back(4);//第四段文字之前播放
        
        heromove_poses.clear();
        heromove_user.clear();
        heromove_poses.push_back(4);
        heromove_user.push_back(1);
        
        heromove_position.push_back(heromove_poses);//曹操移动
        heromove_isuser.push_back(heromove_user);
        heromove_dialogindex.push_back(6);//第6段文字之前播放
        
        heromove_poses.clear();
        heromove_user.clear();
        heromove_poses.push_back(1);
        heromove_poses.push_back(2);
        heromove_user.push_back(1);
        heromove_user.push_back(1);
        
        heromove_position.push_back(heromove_poses);//典韦和许褚移动
        heromove_isuser.push_back(heromove_user);
        heromove_dialogindex.push_back(7);//第7段文字之前播放
        log("size是%lu",dialog_wenzi.size());
        userRole[0]->runAction(MoveTo::create(2, userposition[0]));
        //userRole[0]->runAction(Sequence::create(ScaleTo::create(0.5f, 1),ScaleTo::create(0.5f, 0.69f),ScaleTo::create(0.5f, 1),ScaleTo::create(0.5f, 0.69f), CallFuncN::create(CC_CALLBACK_1(CompeteLayer::dialog1, this)),NULL));
        userRole[0]->runAction(Sequence::create(ScaleTo::create(0.5f, 1),ScaleTo::create(0.5f, 0.69f),ScaleTo::create(0.5f, 1),ScaleTo::create(0.5f, 0.69f),NULL));//角色进入战场动作
        
    }
    else if(new_stage==2){//新手副本二

        dialog_wenzi.push_back("前面有股部队阻挡我们的去路");//0
        dialog_direction.push_back(1);
        dialog_wenzi.push_back("来人，报上名来！");//1
        dialog_direction.push_back(2);
        dialog_wenzi.push_back("我乃……");//2
        dialog_direction.push_back(1);
        dialog_wenzi.push_back("给我上！");//3
        dialog_direction.push_back(2);
        dialog_wenzi.push_back("。。。");//4
        dialog_direction.push_back(1);
        dialog_wenzi.push_back("小兄弟，我们来助你！");//5
        dialog_direction.push_back(1);
        dialog_wenzi.push_back("燕人张飞在此！");//6
        dialog_direction.push_back(1);
        //dialog_wenzi.push_back("关某在此，看尔敢插标卖首！");//7
        //dialog_direction.push_back(1);
        
        heromove_poses.push_back(4);
        heromove_user.push_back(0);
        heromove_poses.push_back(3);
        heromove_user.push_back(0);
        heromove_poses.push_back(5);
        heromove_user.push_back(0);
        heromove_position.push_back(heromove_poses);//张角移动
        heromove_isuser.push_back(heromove_user);
        heromove_dialogindex.push_back(1);//第二段文字之前播放
        
        heromove_poses.clear();
        heromove_user.clear();
        heromove_poses.push_back(0);
        heromove_poses.push_back(2);
        heromove_user.push_back(0);
        heromove_user.push_back(0);
        heromove_position.push_back(heromove_poses);//小兵1和2移动
        heromove_isuser.push_back(heromove_user);
        heromove_dialogindex.push_back(4);//第四段文字之前播放
        
        heromove_poses.clear();
        heromove_user.clear();
        heromove_poses.push_back(2);
        heromove_user.push_back(1);
        heromove_poses.push_back(4);
        heromove_user.push_back(1);
        
        heromove_position.push_back(heromove_poses);//张飞关羽移动
        heromove_isuser.push_back(heromove_user);
        heromove_dialogindex.push_back(5);//第6段文字之前播放
        
        log("size是%lu",dialog_wenzi.size());
        userRole[1]->runAction(MoveTo::create(2, userposition[1]));
        userRole[1]->runAction(Sequence::create(ScaleTo::create(0.5f, 1),ScaleTo::create(0.5f, 0.69f),ScaleTo::create(0.5f, 1),ScaleTo::create(0.5f, 0.69f), NULL));
        userRole[0]->runAction(MoveTo::create(2, userposition[0]));
        //userRole[0]->runAction(Sequence::create(ScaleTo::create(0.5f, 1),ScaleTo::create(0.5f, 0.69f),ScaleTo::create(0.5f, 1),ScaleTo::create(0.5f, 0.69f), CallFuncN::create(CC_CALLBACK_1(CompeteLayer::dialog1, this)),NULL));
        userRole[0]->runAction(Sequence::create(ScaleTo::create(0.5f, 1),ScaleTo::create(0.5f, 0.69f),ScaleTo::create(0.5f, 1),ScaleTo::create(0.5f, 0.69f),NULL));
        
    }
    else if(new_stage==3){//新手副本三

        dialog_wenzi.push_back("西凉贼众果然勇猛，我们快抵挡不住了。。");//0
        dialog_direction.push_back(1);
        dialog_wenzi.push_back("主公，跟他们拼了！");//1
        dialog_direction.push_back(1);
        dialog_wenzi.push_back("一群乌合之众，看你们能坚持多久");//2
        dialog_direction.push_back(2);
        dialog_wenzi.push_back("前方是何人部队，董卓势大，我等前来助阵");//3
        dialog_direction.push_back(1);
        dialog_wenzi.push_back("我乃孙坚字文台。小兄弟如此，我等感激不尽！");//4
        dialog_direction.push_back(1);
        dialog_wenzi.push_back("看我杀退贼军");//5
        dialog_direction.push_back(1);
        //dialog_wenzi.push_back("关某在此，看尔敢插标卖首！");//6
        //dialog_direction.push_back(1);
        
        heromove_poses.push_back(3);
        heromove_user.push_back(1);
        heromove_poses.push_back(4);
        heromove_user.push_back(1);
        heromove_poses.push_back(5);
        heromove_user.push_back(1);
        heromove_position.push_back(heromove_poses);//张角移动
        heromove_isuser.push_back(heromove_user);
        heromove_dialogindex.push_back(3);//第4段文字之前播放
        
        log("size是%lu",dialog_wenzi.size());
        enemyRole[3]->runAction(MoveTo::create(2, enemyposition[3]));
        //enemyRole[5]->runAction(Sequence::create(MoveTo::create(2,enemyposition[5]), CallFuncN::create(CC_CALLBACK_1(CompeteLayer::dialog1, this)),NULL));
        enemyRole[5]->runAction(Sequence::create(MoveTo::create(2,enemyposition[5]),NULL));
        
    }
    dialog_index = 0;
    
    heromove_index=0;
    dialogbeginTime = 2;
    schedule(schedule_selector(CompeteLayer::dialog));//播放对话
    
}
//和dialog一起使用的英雄移动
void CompeteLayer::heroMove(Ref* pSender){
    int moveindex = heromove_index;
    for(int i=0;i<heromove_position[heromove_index].size();i++){
        if(heromove_isuser[moveindex][i]){
            //auto seq = Sequence::create(MoveTo::create(0.5f, userposition[heromove_position[moveindex][i]]),NULL);
            //if(i==heromove_position[heromove_index].size()-1)
                //seq = Sequence::create(seq,CallFuncN::create(CC_CALLBACK_1(CompeteLayer::dialog1,this)),NULL);
            userRole[heromove_position[moveindex][i]]->runAction(MoveTo::create(0.5f, userposition[heromove_position[moveindex][i]]));
        }
        else{
            enemyRole[heromove_position[heromove_index][i]]->runAction(MoveTo::create(0.5f, enemyposition[heromove_position[moveindex][i]]));
        }
    }
    heromove_index++;//有可能因为时间差出错
    dialogbeginTime = 1;
    schedule(schedule_selector(CompeteLayer::dialog));
}
void CompeteLayer::dialog(float dt){
    dialogbeginTime -= dt;
    if(dialogbeginTime<=0){
        if(dialog_index>=dialog_direction.size()){
            unschedule(schedule_selector(CompeteLayer::dialog));
            if(juqing_move==0){
                log("正式开始战斗，退出剧情");
                juqing_move++;
                beginFight();
            }
            else if(juqing_move==2){
                CompeteLayer::endJiazai();
            }
            return;
        }
        dialogbeginTime = 2;
        int index = dialog_index;
        auto dialogframe1 = Sprite::create();
        auto dialog1 = Label::createWithTTF(dialog_wenzi[index], "字体/华文新魏.TTF", 30);
        if(dialog_direction[index]==1){
            dialogframe1 = Sprite::createWithSpriteFrameName("duihuakuang_shangfang.png");
            dialogframe1->setPosition(userposition[0].x+dialogframe1->getContentSize().width/3+sizeX/2,userposition[0].y+dialogframe1->getContentSize().height/3);
            dialog1->setPosition(dialogframe1->getContentSize().width/2+20,dialogframe1->getContentSize().height/2-15);

        }
        else if(dialog_direction[dialog_index]==2){
            if(juqing_move==2){
                dialogframe1 = Sprite::createWithSpriteFrameName("duihuakuang_shangfang.png");
                dialogframe1->setPosition(userposition[4].x+dialogframe1->getContentSize().width/3+sizeX/2-45,userposition[4].y+40+45);
                dialog1->setPosition(dialogframe1->getContentSize().width/2+20,dialogframe1->getContentSize().height/2-15);
            }
            else if(juqing_move==0){
                dialogframe1 = Sprite::createWithSpriteFrameName("duihuakuang_xiafang.png");
                dialogframe1->setPosition(enemyposition[4].x,enemyposition[4].y-sizeY);
                dialog1->setPosition(dialogframe1->getContentSize().width/2-10,dialogframe1->getContentSize().height/2-15);
            }
        }
        dialog_index++;
        dialog1->setColor(Color3B::BLACK);
        dialogframe1->addChild(dialog1,1);
        dialog1->setDimensions(dialogframe1->getContentSize().width*0.7,dialogframe1->getContentSize().height*0.65);
        this->addChild(dialogframe1,7);
        /*for(int i=0;i<heromove_dialogindex.size();i++){
        if(dialog_index==heromove_dialogindex[heromove_index]&&heromove_index<heromove_dialogindex.size()){
            if(heromove_index==4){
                //log("错误");
            }
            dialogframe1->runAction(Sequence::create(DelayTime::create(1.2f),FadeOut::create(0.3f),RemoveSelf::create(),NULL));
            
            return;
        }
        */
        if(dialog_index==heromove_dialogindex[heromove_index]&&heromove_index<heromove_dialogindex.size()){
            if(heromove_index==4){
                //log("错误");
            }
            unschedule(schedule_selector(CompeteLayer::dialog));
            dialog1->runAction(Sequence::create(DelayTime::create(1.2f),FadeOut::create(0.3f),RemoveSelf::create(),NULL));
            dialogframe1->runAction(Sequence::create(DelayTime::create(1.2f),FadeOut::create(0.3f),CallFuncN::create(CC_CALLBACK_1(CompeteLayer::heroMove,this)),RemoveSelf::create(),NULL));
            
            return;
        }
        dialog1->runAction(Sequence::create(DelayTime::create(1.2f),FadeOut::create(0.3f),RemoveSelf::create(),NULL));

        dialogframe1->runAction(Sequence::create(DelayTime::create(1.2f),FadeOut::create(0.3f),RemoveSelf::create(),NULL));
        //dialog_index++;

    }
}
//剧情中攻击动作,暂时没用
void CompeteLayer::juqing_attack_move(Ref* pSender){
    if (new_stage==0) {
        if(juqing_move==0){
            vector<int> tag2, isCritic;
            vector<float> damage;
            vector<int> block;
            damage.push_back(enemyMap[4].attackDisplay);
            isCritic.push_back(0);
            tag2.push_back(0);
            block.push_back(0);
            enemyRole[4]->attack(0, 4, tag2, 0, 0, 0);
            userRole[0]->attackDamage(damage, 0, isCritic, block,1.1f,0);
            userRole[0]->runAction(Sequence::create(DelayTime::create(1.1f),CallFuncN::create(CC_CALLBACK_1(CompeteLayer::roleRecover,this)), NULL));
        }
    }
}
//重播战斗动画是调用
void CompeteLayer::roleRecover(Ref* pSender){
    ((CompeteRole*)pSender)->recover();
    ((CompeteRole*)pSender)->healthBar->setPercentage(0);
}
//攻城战中，器械攻击逻辑
void CompeteLayer::qixieAttackHeros(){
    qixieIsEnemy = true;
    int userqixieIndex = 0;
    int enemyqixieIndex = 0;
    qixietime = 0;
    //qixienum1 = 0;
    bool userTurn = true;
    while(true){
        if(userTurn){
            if(userqixieMap.find(userqixieIndex)==userqixieMap.end()){//此位置无器械
                userTurn = false;
                userqixieIndex++;
                continue;
            }
            else{
                qixieDamage(userqixieIndex,true);
                //userqixieMap.find(userqixieIndex)->second.durable--;
                if(userqixieMap.find(userqixieIndex)->second.durable<=0)
                    userqixieMap.erase(userqixieMap.find(userqixieIndex));
                userqixieIndex++;
                userTurn = false;
            }
            if(enemyMap.size()==0){//TO DO...玩家胜利
                CCLOG("玩家胜利");
                userWin = true;
                //unschedule(schedule_selector(CompeteLayer::beginCompete));
                //scheduleUpdate();
                qixienumber.push_back(qixieattack_list.size()-1);

                break;
            }
            else if(userMap.size()==0){//TO DO...玩家胜利
                CCLOG("敌方胜利");
                userWin = false;
                //unschedule(schedule_selector(CompeteLayer::beginCompete));
                //scheduleUpdate();
                qixienumber.push_back(qixieattack_list.size()-1);

                break;
            }

            if(enemyqixieIndex>=enemyqixieTeam.size()&&userqixieIndex>=qixiecompeteTeam.size()){
                qixiedelay_time.push_back(qixietime);
                qixienumber.push_back(qixieattack_list.size()-1);
                return;
            }

        }
        else{
            if(enemyqixieMap.find(enemyqixieIndex)==enemyqixieMap.end()){
                userTurn = true;
                enemyqixieIndex++;
                continue;
            }
            else{
                qixieDamage(enemyqixieIndex, false);
                //enemyqixieMap.find(enemyqixieIndex)->second.durable--;
                if(enemyqixieMap.find(enemyqixieIndex)->second.durable<=0)
                    enemyqixieMap.erase(enemyqixieMap.find(enemyqixieIndex));
                enemyqixieIndex++;
                userTurn = true;
                
            }
            if(enemyMap.size()==0){//TO DO...玩家胜利
                CCLOG("玩家胜利");
                userWin = true;
                qixienumber.push_back(qixieattack_list.size()-1);

                //unschedule(schedule_selector(CompeteLayer::beginCompete));
                //scheduleUpdate();
                break;
            }
            else if(userMap.size()==0){//TO DO...玩家胜利
                CCLOG("敌方胜利");
                userWin = false;
                qixienumber.push_back(qixieattack_list.size()-1);

                //unschedule(schedule_selector(CompeteLayer::beginCompete));
                //scheduleUpdate();
                break;
            }

            if(enemyqixieIndex>=enemyqixieTeam.size()&&userqixieIndex>=qixiecompeteTeam.size()){
                qixiedelay_time.push_back(qixietime);
                qixienumber.push_back(qixieattack_list.size()-1);
                return;
            }

        }
    }
}
//出现剧情对话时，点击屏幕加快进程
bool CompeteLayer::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){
    if(juqing_move==0){
        dialogbeginTime = 0.01f;
    }
    else if(juqing_move==2){
        dialogbeginTime = 0.01f;
    }
    return true;
}

void CompeteLayer::onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event){}
void CompeteLayer::onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event){}

//器械伤害计算
void CompeteLayer::qixieDamage(int index,bool isEnemy){
    CCLOG("攻城:器械为%d",index);
    qixienum1++;
    qixieis_enemy.push_back(isEnemy);
    qixietime+=2.0f;
    if(isEnemy){
        vector<int> vecTag = findAttackPosition(index, 1, isEnemy);
        qixieattack_list.push_back(pair<int,vector<int>>(index,vecTag));//TO DO....

        int damage = userqixieMap.at(index).attack;
        vector<int> vecDamage;
        vecDamage.push_back(damage);
        qixiedamage_list.push_back(vecDamage);
        enemyMap.at(vecTag[0]).healthDisplay -= damage;
        if(floor(enemyMap.at(vecTag[0]).healthDisplay)<=0){//TO DO...敌方死亡清除
            enemyMap.erase(enemyMap.find(vecTag[0]));
        }
    }
    else{
        vector<int> vecTag = findAttackPosition(index, 1, isEnemy);
        qixieattack_list.push_back(pair<int,vector<int>>(index,vecTag));//TO DO....
        
        int damage = enemyqixieMap.at(index).attack;
        vector<int> vecDamage;
        vecDamage.push_back(damage);
        qixiedamage_list.push_back(vecDamage);
        userMap.at(vecTag[0]).healthDisplay -= damage;
        if(floor(userMap.at(vecTag[0]).healthDisplay)<=0){//TO DO...敌方死亡清除
            userMap.erase(userMap.find(vecTag[0]));
        }
    }
}
//战斗英雄攻击和结束逻辑
void CompeteLayer::beginFight(){
    CCLOG("第%d回合!",round);
    if(isgongchengzhan){
        qixieAttackHeros();
        CompeteLayer::reArrange();
        //TO DO....此行为确定哪一方先出手。先用敏捷来比较，以后用总战力
        if(userVec[0].second.agilityDisplay>=enemyVec[0].second.agilityDisplay)//判断条件谁先手
            usersTurn = true;
        else
            usersTurn = false;
    }
    while (true) {
        
        float a1,a2;//a1是技能，a2是双击判断
        /*srand((unsigned)time(NULL));//定义随机数 0-1 取第四个值
        for(int q=0;q<5;q++){
            a1 = rand()/double(RAND_MAX);
            if(q==4)
                a2 = rand()/double(RAND_MAX);//用于判断是否双击
        }*/
        a1 = arc4random()%100/100.0f;
        a2 = arc4random()%100/100.0f;

        if(usersTurn){//玩家方进攻
            int roleTag = userVec[userIndex].second.position;//找到攻击方英雄的位置
            CompeteRole* role = userRole[roleTag];
            /*if(a<=role->prob)
             beginTime = role->act_skill_duration;*/
            int posit = (roleTag>=3)?(roleTag-3):roleTag;
            vector<int> vecTag = CompeteLayer::findAttackPosition(posit, 1, true);//返回攻击目标
            int tag = vecTag[0];//
            if(a1>=role->prob){//普通攻击
                isUseSkill = false;
                float doubleAttackProp = doubleAttackProb(userVec[userIndex].second.agilityDisplay,enemyMap.find(tag)->second.agilityDisplay);
                if (a2<doubleAttackProp) {//此次攻击发动双击
                    isDoubleAttack = true;
                    beginTime = 3.0f;
                }
                else{
                    isDoubleAttack = false;
                }
            }
            else//触发技能
                isUseSkill = true;
            //------------------加载动画属性
            if(new_stage==0){//新手副本一的特殊设定
                if(round==1&&userIndex==0){//第一回合典韦攻击
                    isUseSkill = false;
                }
                else if(round==1&&userIndex==1){//第一回合主角攻击双击
                    isDoubleAttack = true;
                }
                else if(round==2&&userIndex==0){//第二回合典韦攻击放技能，并双击
                    isUseSkill = true;
                    isDoubleAttack = true;
                }
            }
            is_enemy.push_back(1);
            is_skill.push_back(isUseSkill);
            
            //------------------END
            
            //TODO:...类许褚技能special = 21
            /*if (userVec[userIndex].second.special==21) {
                float healthPerp = 1.0f - role->curHP/role->totalHp;
                userMap.find(userVec[userIndex].second.position)->second.attackDisplay = competeTeam.find(userVec[userIndex].second.position)->second.attackDisplay * (1+healthPerp);
                CCLOG("许褚裸衣");
            }*/
            
            CCLOG("我方行动: 位置%d,名字%s", roleTag, userVec[userIndex].second.name.c_str());
            damage(userIndex, true);//TO DO....damage的第二个parameter没用
            usersTurn=false;
            
        //二次行动----------
            if(userVec[userIndex].second.DoubleMoveProp>0){
                if(attack_list[attack_list.size()-2].first == userVec[userIndex].second.position && is_enemy[is_enemy.size()-2] && userVec[userIndex].second.just_used_doublemove){//上一回合是两次的第一次
                    log("两次结束");
                    userVec[userIndex].second.just_used_doublemove = 0;
                    //usersTurn = false;
                }
                else{
                    log("哇哈哈,%f",userVec[userIndex].second.DoubleMoveProp);
                    float prob1 = arc4random()%100/100.0f;
                    if(prob1<userVec[userIndex].second.DoubleMoveProp){
                        usersTurn = true;
                        userVec[userIndex].second.just_used_doublemove = 1;
                        userIndex--;

                        //passive_skill.push_back(0);//此次攻击没有被动
                    }
                }

            }
        //------------------
            userIndex++;

            //排序
            if (enemyIndex>=enemyVec.size()) {
                usersTurn=true;
            }
            //判断是否到下个回合
            if(enemyIndex>=enemyVec.size()&&userIndex>=userVec.size())
            {
                CCLOG("第%d回合!",round);
                roundAnimationIndex.push_back(attack_list.size()-1);
                round++;//回合数变量+1
                //attack_list.push_back(pair<int,vector<int>>(round-1+5,vec));//TO DO...加入显示下一回合动画
                enemyIndex=0;
                userIndex=0;
                if(isgongchengzhan)
                    qixieAttackHeros();
                if(enemyMap.size()==0){//玩家胜利
                    CCLOG("玩家胜利");
                    userWin = true;
                    break;
                }
                else if(userMap.size()==0){//敌方胜利
                    CCLOG("敌方胜利");
                    userWin = false;
                    break;
                }

                CompeteLayer::reArrange();
                if(userVec[0].second.agilityDisplay>=enemyVec[0].second.agilityDisplay)//判断下回合谁先手
                    usersTurn = true;
                else
                    usersTurn = false;
            }
        }
        else{//敌方某英雄行动
            int roleTag = enemyVec[enemyIndex].second.position;
            CCLOG("敌方行动: 位置%d,名字%s", roleTag, enemyVec[enemyIndex].second.name.c_str());
            CompeteRole* enemy = enemyRole[roleTag];
            int posit = (roleTag>=3)?(roleTag-3):roleTag;
            vector<int> vecTag = CompeteLayer::findAttackPosition(posit, 1, false);
            int tag = vecTag[0];//取第一个是因为返回的只会是1个元素的vector
            if(a1>=enemy->prob){//普通攻击
                isUseSkill = false;
                float doubleAttackProp = doubleAttackProb(enemyVec[userIndex].second.agilityDisplay,userMap.find(tag)->second.agilityDisplay);
                if (a2<doubleAttackProp) {//将要双击
                    isDoubleAttack = true;
                    beginTime = 3.0f;
                }
                else{
                    isDoubleAttack = false;
                }
            }
            else
                isUseSkill = true;
            //------------------加载动画属性
            if(new_stage==0){
                if(round==1&&enemyIndex==0){//张角第一回合放技能
                    isUseSkill = true;
                    isDoubleAttack = false;
                }
            }
            is_enemy.push_back(0);
            is_skill.push_back(isUseSkill);
            //delay_time.push_back(delayTime);
            
            if(enemy)
            {
                CCLOG("now is attacking by 武将: %s",enemy->resName.c_str());
                CompeteLayer::damage(enemyIndex, false);
            }
            else//TO DO....好像是这轮敌方没有可以行动的
            {
                //beginTime=-1;
            }
            usersTurn=true;
            
            enemyIndex++;
            
            if (userIndex>=userVec.size()) {
                usersTurn=false;
            }
            if(enemyIndex>=enemyVec.size()&&userIndex>=userVec.size())
            {
                CCLOG("第%d回合!",round);
                roundAnimationIndex.push_back(attack_list.size()-1);
                round++;//回合数变量+1
                enemyIndex=0;
                userIndex=0;
                if(isgongchengzhan)
                    qixieAttackHeros();
                if(enemyMap.size()==0){//TO DO...玩家胜利
                    CCLOG("玩家胜利");
                    userWin = true;
                    break;
                }
                else if(userMap.size()==0){//TO DO...玩家胜利
                    CCLOG("敌方胜利");
                    userWin = false;
                    break;
                }

                CompeteLayer::reArrange();
                if(userVec[0].second.agilityDisplay>=enemyVec[0].second.agilityDisplay)//判断下回合谁先手
                    usersTurn = true;
                else
                    usersTurn = false;
            }
        }
    }
    animation_index = 0;
    qixiebeginTime = 1.0f;
    round = 0;
    roundAnimationIndex.push_back(attack_list.size()-1);
    beginTime = 1.0f;
    if(isgongchengzhan)
        schedule(schedule_selector(CompeteLayer::playGongcheng),0.01f);
    else
        schedule(schedule_selector(CompeteLayer::playFight),0.01f);
}
//判断下一个行动的英雄,以及战斗结束判断
void CompeteLayer::findNextAttackHero(){
    for (auto it = userMap.begin(); it!=userMap.end();) {
        if(floor(it->second.healthDisplay)<=0){//删掉血小于0的entry
            CCLOG("清除死亡部队: %s of size: %lu",it->second.name.c_str(),userMap.size());
            userMap.erase(it++);
        }
        else
            it++;
    }
    for (auto it = enemyMap.begin(); it!=enemyMap.end();) {
        if(floor(it->second.healthDisplay)<=0){
            CCLOG("clear health below 0 unit : %s, of size %lu",it->second.name.c_str(),enemyMap.size());
            enemyMap.erase(it++);//delete
        }
        else
            it++;
    }
    while(enemyIndex<enemyVec.size()) {//确定下一个行动的敌方部队index
        if(enemyMap.find(enemyVec[enemyIndex].second.position)!=enemyMap.end()){
            CCLOG("检查下一个执行攻击的部队，目标剩余血量: %f",enemyMap.find(enemyVec[enemyIndex].second.position)->second.healthDisplay);
            break;
        }
        else{
            enemyIndex++;
            CCLOG("目标死亡，切换下一个");
        }
    }
    while(userIndex<userVec.size()) {//确定下一个行动的我方部队的index
        if(userMap.find(userVec[userIndex].second.position)!=userMap.end()){//表示此部队还活着
            CCLOG("检查下一个执行攻击的部队，目标剩余血量: %f",userMap.find(userVec[userIndex].second.position)->second.healthDisplay);
            break;
        }
        else{
            userIndex++;
            CCLOG("目标死亡，切换下一个");
        }
    }
    
    if(enemyMap.size()==0){//玩家胜利
        CCLOG("玩家胜利");
        userWin = true;
        //unschedule(schedule_selector(CompeteLayer::beginCompete));
        //scheduleUpdate();
        break;
    }
    else if(userMap.size()==0){//敌方胜利
        CCLOG("敌方胜利");
        userWin = false;
        break;
    }
    log("user序列为: %d,敌方序列为: %d",userIndex,enemyIndex);
}
/*void CompeteLayer::beginAnimation(float dt){
    playFight();
    unschedule(schedule_selector(CompeteLayer::beginAnimation));
}*/

//调用结束动画
void CompeteLayer::update(float dt){
    end_delay_time-=dt;
    if (end_delay_time<=0 && (enemyMap.size()==0||userMap.size()==0)) {
        CompeteLayer::showGameResult(userWin);
        unscheduleUpdate();
    }
}
//播放攻城武器动画
void CompeteLayer::playGongcheng(float dt){
    qixiebeginTime -= dt;
    if(qixiebeginTime<=0){
        if(animation_index>qixienumber[round]){
            unschedule(schedule_selector(CompeteLayer::playGongcheng));
            if(round==0)
                animation_index = 0;
            else
                animation_index = roundAnimationIndex[round-1] + 1;
            beginTime = 1.0f;
            schedule(schedule_selector(CompeteLayer::playFight),0.01f);

            return;
        }
        qixiebeginTime = 1.2f;//
        if (qixieis_enemy[animation_index]) {//我方进攻
            //TO DO...播放技能动画
            userQixieRole[qixieattack_list[animation_index].first]->attack(false, (qixieattack_list[animation_index].first+300), (qixieattack_list[animation_index].second), 0, true,qixiedelay_time[animation_index]);
            for(int j=0; j<qixieattack_list[animation_index].second.size();j++){
                vector<float> dmg;
                vector<int> crit;
                crit.push_back(0);
                dmg.push_back(qixiedamage_list[animation_index][j]);
                vector<int> block;
                block.push_back(0);

                enemyRole[qixieattack_list[animation_index].second[j]]->attackDamage(dmg, false, crit,block,0.3f,passive_skill[animation_index].second[j]);

            }
        }
        else{//敌方进攻
            enemyQixieRole[qixieattack_list[animation_index].first]->attack(false, (qixieattack_list[animation_index].first+400), (qixieattack_list[animation_index].second), 0, false,qixiedelay_time[animation_index]);
            for(int j=0; j<qixieattack_list[animation_index].second.size();j++){
                vector<float> dmg;
                vector<int> crit;
                crit.push_back(0);
                dmg.push_back(qixiedamage_list[animation_index][j]);
                vector<int> block;
                block.push_back(0);

                userRole[qixieattack_list[animation_index].second[j]]->attackDamage(dmg, false, crit,block,0.3f,passive_skill[animation_index].second[j]);

            }
            
        }
        animation_index++;
    }
}
//播放英雄攻击动画
void CompeteLayer::playFight(float dt){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    beginTime -= dt;
    //log("剩余%f时间到下一轮攻击",beginTime);
    if(abs(beginTime-0.7f)<=0.01f&&new_stage==0&&animation_index==4){//新手副本一:主角格挡停顿
        auto shuoming_underline = Sprite::create("bg_yellow_msg.png");
        auto shuoming_wenzi = Label::createWithTTF("受到伤害一定概率格挡，几率由双方敏捷决定", "字体/华文新魏.TTF", 26);
        shuoming_wenzi->setDimensions(visibleSize.width*2/3, 100);
        shuoming_wenzi->setPosition(visibleSize.width/2,visibleSize.height/2-24);
        shuoming_underline->setPosition(visibleSize.width/2,visibleSize.height/2);
        
        shuoming_underline->runAction(Sequence::create(DelayTime::create(0.6f),FadeOut::create(0.2f),RemoveSelf::create(), NULL));
        shuoming_wenzi->runAction(Sequence::create(DelayTime::create(0.6f),FadeOut::create(0.2f),RemoveSelf::create(), NULL));
        this->addChild(shuoming_underline,7,SHUOMING_XIAHUAXIAN);
        this->addChild(shuoming_wenzi,8,SHUOMING_WORD);
    
        Director::getInstance()->getActionManager()->pauseAllRunningActions();
        unschedule(schedule_selector(CompeteLayer::playFight));
        schedule(schedule_selector(CompeteLayer::resumeActions),2);
    }
    if(beginTime<=0){
        if(animation_index>=attack_list.size()){
            //totalDelayTime+=3.0f;
            unschedule(schedule_selector(CompeteLayer::playFight));
            if(new_stage==0){
                //结尾剧情载入
                dialog_wenzi.push_back("各位英雄留名");//0
                dialog_direction.push_back(1);
                dialog_wenzi.push_back("我乃曹操,字孟德,今日前来正是为了清剿黄巾军");//1
                dialog_direction.push_back(2);
                dialog_wenzi.push_back("阁下的英雄气概让人敬佩万分！旁边这两位呢？");//2
                dialog_direction.push_back(1);
                dialog_wenzi.push_back("某姓典名韦,看到这战场激烈,特来相助");//3
                dialog_direction.push_back(2);
                dialog_wenzi.push_back("洒家是许褚字仲康,本是山寨主,兄弟们被张角座下裴元绍欺负,今特来报仇");//4
                dialog_direction.push_back(2);
                dialog_wenzi.push_back("二位均有万夫莫当之勇啊！");//5
                dialog_direction.push_back(1);
                dialog_wenzi.push_back("小兄弟临危不惧,让人好生钦佩。但在这乱世中,单枪匹马怎么行,曹洪何在!");//6
                dialog_direction.push_back(2);
                dialog_wenzi.push_back("小兄弟,初次见面本应厚礼相赠,我将曹洪派来助你一臂之力！");//7
                dialog_direction.push_back(2);
                dialog_wenzi.push_back("孟德如此厚待,我便接受了。待我闯出一片天，我再杀他个干干净那……净！");//7
                dialog_direction.push_back(1);
                dialogbeginTime = 0.5f;
                juqing_move++;
                schedule(schedule_selector(CompeteLayer::dialog));
                return;
            }
            else if(new_stage==2){
                //结尾剧情
                dialog_wenzi.push_back("请问各位英雄尊姓大名");//0
                dialog_direction.push_back(1);
                dialog_wenzi.push_back("我乃张飞,字翼德,乘国家危难今特来投军");//1
                dialog_direction.push_back(2);
                dialog_wenzi.push_back("阁下真可谓一夫当关万夫莫开！旁边这位呢？");//2
                dialog_direction.push_back(1);
                dialog_wenzi.push_back("某是关云长,和翼德志气相投，也是来投军的。");//3
                dialog_direction.push_back(2);
                dialog_wenzi.push_back("小兄弟，我们去投军了，但愿以后有再会之期!");//4
                dialog_direction.push_back(2);
                dialog_wenzi.push_back("二位英雄再会！");//5
                dialog_direction.push_back(1);
                dialogbeginTime = 0.5f;
                juqing_move++;
                schedule(schedule_selector(CompeteLayer::dialog));
                return;
            }
            else if(new_stage==3){
                //结尾剧情
                dialog_wenzi.push_back("虽然杀退了敌军，但主力仍在。");//0
                dialog_direction.push_back(1);
                dialog_wenzi.push_back("择日再战吧，下次定要全歼敌军！");//1
                dialog_direction.push_back(1);
                dialogbeginTime = 0.5f;
                juqing_move++;
                schedule(schedule_selector(CompeteLayer::dialog));
                return;
            }

            if(new_stage>=0){//表示还处于新手引导阶段
                new_stage++;//进入新手下一阶段
                new_step = 0;
            }
            end_delay_time = 1.0f;
            scheduleUpdate();
            return;
        }
        beginTime = delay_time[animation_index];
        if(animation_index>roundAnimationIndex[round]){
            //TO DO...播放下一回合动画
            if(isgongchengzhan){
                animation_index = qixienumber[round]+1;
                round++;
                qixiebeginTime = 1.0f;
                schedule(schedule_selector(CompeteLayer::playGongcheng));
                unschedule(schedule_selector(CompeteLayer::playFight));
//TO DO ....播放攻城机械攻击动作
                CCLOG("下回合开始!");
                return;
            }
            //对应的下边i值需要变化
        }
        if (is_enemy[animation_index]) {//我方进攻
            //TO DO...播放技能动画
            if(new_stage==0){
                if(animation_index==2){//第一回合主角双击
                    auto shuoming_underline = Sprite::create("bg_yellow_msg.png");
                    auto shuoming_wenzi = Label::createWithTTF("攻击之后一定概率发动双击，几率由双方敏捷决定", "字体/华文新魏.TTF", 26);
                    shuoming_wenzi->setDimensions(visibleSize.width*2/3, 100);
                    shuoming_wenzi->setPosition(visibleSize.width/2,visibleSize.height/2-24);
                    shuoming_underline->setPosition(visibleSize.width/2,visibleSize.height/2);
                    
                    shuoming_underline->runAction(Sequence::create(DelayTime::create(0.6f),FadeOut::create(0.2f),RemoveSelf::create(), NULL));
                    shuoming_wenzi->runAction(Sequence::create(DelayTime::create(0.6f),FadeOut::create(0.2f),RemoveSelf::create(), NULL));
                    this->addChild(shuoming_underline,7,SHUOMING_XIAHUAXIAN);
                    this->addChild(shuoming_wenzi,8,SHUOMING_WORD);
                }

            }
            if(new_stage==1){//第一场被动解锁战
                if(animation_index==0){//第一回合曹洪攻击,动画暂停
                    auto shuoming_underline = Sprite::create("bg_yellow_msg.png");
                    auto shuoming_wenzi = Label::createWithTTF("被动特效触发一定次数即可成功解锁", "字体/华文新魏.TTF", 26);
                    shuoming_wenzi->setDimensions(visibleSize.width*2/3, 100);

                    shuoming_wenzi->setPosition(visibleSize.width/2,visibleSize.height/2-24);
                    shuoming_underline->setPosition(visibleSize.width/2,visibleSize.height/2);
                    
                    shuoming_underline->runAction(Sequence::create(DelayTime::create(0.6f),FadeOut::create(0.2f),RemoveSelf::create(), NULL));
                    shuoming_wenzi->runAction(Sequence::create(DelayTime::create(0.6f),FadeOut::create(0.2f),RemoveSelf::create(), NULL));
                    this->addChild(shuoming_underline,7,SHUOMING_XIAHUAXIAN);
                    this->addChild(shuoming_wenzi,8,SHUOMING_WORD);
                    
                }
                
            }

            userRole[attack_list[animation_index].first]->attack(is_skill[animation_index], (attack_list[animation_index].first+100), (attack_list[animation_index].second), double_list[animation_index], true,0);

            for(int j=0; j<attack_list[animation_index].second.size();j++){
                vector<float> dmg;
                vector<int> crit;
                dmg.push_back(damage_list[animation_index][j]);
                crit.push_back(critic_list[animation_index][j]);
                vector<int> block;
                block.push_back(block_list[animation_index][j]);

                float time=1.1f;
                if(userRole[attack_list[animation_index].first]->resName=="caoxing"||userRole[attack_list[animation_index].first]->resName=="huangzhong"){
                    time = 0.6f;
                }
                enemyRole[attack_list[animation_index].second[j]]->attackDamage(dmg, true, crit,block,damage_delaytime_list[animation_index][j],passive_skill[animation_index].second[j]);

                /*if(j==0&&double_list[animation_index]==1){
                    dmg.push_back(damage_list[animation_index][damage_list[animation_index].size()-1]);
                    crit.push_back(critic_list[animation_index][critic_list[animation_index].size()-1]);
                    block.push_back(block_list[animation_index][block_list[animation_index].size()-1]);
                    enemyRole[attack_list[animation_index].second[j]]->attackDamage(dmg, true, crit,block,time,passive_skill[animation_index].second[j]);

                }else{
                    enemyRole[attack_list[animation_index].second[j]]->attackDamage(dmg, false, crit,block,time,passive_skill[animation_index].second[j]);
                }
                 */
            }
            /*if(double_list[animation_index]){
                enemyRole[attack_list[animation_index].second[0]]->costDamage(floor(damage_list[animation_index][damage_list[animation_index].size()-1]), double_list[animation_index], critic_list[animation_index][critic_list[animation_index].size()-1],0.9f);
            }*/
            if(new_stage==0){
                if(animation_index==2){
                    Director::getInstance()->getActionManager()->pauseAllRunningActions();
                    unschedule(schedule_selector(CompeteLayer::playFight));
                    schedule(schedule_selector(CompeteLayer::resumeActions),2);
                }
            }
            if(new_stage==1){
                if(animation_index==0){
                    Director::getInstance()->getActionManager()->pauseAllRunningActions();
                    unschedule(schedule_selector(CompeteLayer::playFight));
                    schedule(schedule_selector(CompeteLayer::resumeActions),2);
                }
            }


        }
        else{//敌方进攻
            if(new_stage==0){
                if(animation_index==1){//第一回合张角发动技能
                    auto shuoming_underline = Sprite::create("bg_yellow_msg.png");
                    auto shuoming_wenzi = Label::createWithTTF("英雄一定概率发动技能，红色底框表示主动技能", "字体/华文新魏.TTF", 26);
                    shuoming_wenzi->setDimensions(visibleSize.width*2/3, 100);
                    shuoming_wenzi->setPosition(visibleSize.width/2,visibleSize.height/2-24);
                    shuoming_underline->setPosition(visibleSize.width/2,visibleSize.height/2);
                    
                    shuoming_underline->runAction(Sequence::create(DelayTime::create(0.6f),FadeOut::create(0.2f),RemoveSelf::create(), NULL));
                    shuoming_wenzi->runAction(Sequence::create(DelayTime::create(0.6f),FadeOut::create(0.2f),RemoveSelf::create(), NULL));
                    this->addChild(shuoming_underline,7,SHUOMING_XIAHUAXIAN);
                    this->addChild(shuoming_wenzi,8,SHUOMING_WORD);
                }
                
            }

            enemyRole[attack_list[animation_index].first]->attack(is_skill[animation_index], (attack_list[animation_index].first+200), (attack_list[animation_index].second), double_list[animation_index], false,0);

            for(int j=0; j<attack_list[animation_index].second.size();j++){
                vector<float> dmg;
                vector<int> crit;
                dmg.push_back(damage_list[animation_index][j]);
                crit.push_back(critic_list[animation_index][j]);
                vector<int> block;
                block.push_back(block_list[animation_index][j]);

                float time=1.1f;
                if(enemyRole[attack_list[animation_index].first]->resName=="caoxing"||enemyRole[attack_list[animation_index].first]->resName=="huangzhong"){//射箭单位不移动，减去移动时间
                    time = 0.6f;
                }
                userRole[attack_list[animation_index].second[j]]->attackDamage(dmg, true, crit,block,damage_delaytime_list[animation_index][j],passive_skill[animation_index].second[j]);

            if(new_stage==0){
                if(animation_index==1){
                    Director::getInstance()->getActionManager()->pauseAllRunningActions();
                    unschedule(schedule_selector(CompeteLayer::playFight));
                    schedule(schedule_selector(CompeteLayer::resumeActions),2);
                }
            }


        }
        animation_index++;
    
    }

}
void CompeteLayer::resumeActions(float dt){
    unschedule(schedule_selector(CompeteLayer::resumeActions));
    if(new_stage==0){
        if(animation_index==2){//因为上边+1了
            Director::getInstance()->getActionManager()->resumeTarget(enemyRole[4]);
            Director::getInstance()->getActionManager()->resumeTargets(enemyRole[4]->getChildren());
            Director::getInstance()->getActionManager()->resumeTarget(userRole[2]);
            Director::getInstance()->getActionManager()->resumeTargets(userRole[2]->getChildren());
            Director::getInstance()->getActionManager()->resumeTarget(userRole[0]);
            Director::getInstance()->getActionManager()->resumeTarget(userRole[1]);
            Director::getInstance()->getActionManager()->resumeTarget(userRole[4]);
            Director::getInstance()->getActionManager()->resumeTarget(this->getChildByTag(910));

            CompeteLayer::resumeTargetByTag(enemyRole[2]->getTag());

            CompeteLayer::resumeTargetByTag(userRole[0]->getTag());
            CompeteLayer::resumeTargetByTag(userRole[1]->getTag());
            CompeteLayer::resumeTargetByTag(userRole[2]->getTag());
            CompeteLayer::resumeTargetByTag(userRole[4]->getTag());

            //beginTime = 3;
        }
        if(animation_index==3){//第二回合游侠
            Director::getInstance()->getActionManager()->resumeTarget(userRole[0]);
            Director::getInstance()->getActionManager()->resumeTargets(userRole[0]->getChildren());

            Director::getInstance()->getActionManager()->resumeTarget(enemyRole[4]);
            Director::getInstance()->getActionManager()->resumeTarget(enemyRole[0]);
            
            CompeteLayer::resumeTargetByTag(userRole[0]->getTag());
            CompeteLayer::resumeTargetByTag(userRole[1]->getTag());
            CompeteLayer::resumeTargetByTag(userRole[2]->getTag());
            CompeteLayer::resumeTargetByTag(userRole[4]->getTag());

            CompeteLayer::resumeTargetByTag(enemyRole[0]->getTag());

        }
        if(animation_index==4){//第一回合小兵攻击主角格挡
            //Director::getInstance()->getActionManager()->resumeTarget(userRole[0]);
            //Director::getInstance()->getActionManager()->resumeTargets(userRole[0]->getChildren());
            
            Director::getInstance()->getActionManager()->resumeTarget(enemyRole[0]);
            Director::getInstance()->getActionManager()->resumeTarget(userRole[0]);

            CompeteLayer::resumeTargetByTag(enemyRole[0]->getTag());
            

            //恢复主角所有文字
            CompeteLayer::resumeTargetByTag(userRole[0]->getTag());
        }
    }
    else if(new_stage==1){
        if(animation_index==1){//因为上边+1了
            Director::getInstance()->getActionManager()->resumeTarget(enemyRole[1]);
            Director::getInstance()->getActionManager()->resumeTarget(userRole[1]);
            Director::getInstance()->getActionManager()->resumeTargets(userRole[1]->getChildren());

            CompeteLayer::resumeTargetByTag(enemyROle[1]->getTag());
            
        }
    }
    Director::getInstance()->getActionManager()->resumeTarget(this->getChildByTag(501));//这句永远不会出错，即使tag不存在

    Director::getInstance()->getActionManager()->resumeTarget(this->getChildByTag(SHUOMING_WORD));//恢复说明文字
    Director::getInstance()->getActionManager()->resumeTarget(this->getChildByTag(SHUOMING_XIAHUAXIAN));

    schedule(schedule_selector(CompeteLayer::playFight),0.01f);
}
//配合resumeActions一起使用
void CompeteLayer::resumeTargetByTag(int tag){
    
    Director::getInstance()->getActionManager()->resumeTarget(this->getChildByTag(DAMAGEFONT_TAG+tag));//伤害值
    Director::getInstance()->getActionManager()->resumeTarget(this->getChildByTag(DAMAGE_CRITIC_TAG+tag));//暴击
    Director::getInstance()->getActionManager()->resumeTarget(this->getChildByTag(DAMAGE_BLOCK_TAG+tag));//格挡
    Director::getInstance()->getActionManager()->resumeTarget(this->getChildByTag(DAMAGE_BLOCK_TAG*2+tag));//格挡护盾
    Director::getInstance()->getActionManager()->resumeTarget(this->getChildByTag(DAMAGEFONT_DOUBLE_TAG+tag));//双击文字
    Director::getInstance()->getActionManager()->resumeTarget(this->getChildByTag(DAMAGE_DOUBLE_TAG+tag));//双击伤害
    
}
//伤害计算
void CompeteLayer::damage(int index, bool isEnemy){
    isCritic = false;
    if(isEnemy){
        int roleTag = userVec[userIndex].second.position;
        int posit = (roleTag>=3)?(roleTag-3):roleTag;
        HeroType myhero = userVec[userIndex].second;
        delayTime = normal_dlytime;
        if(!isUseSkill){
            CCLOG("普通攻击");
            vector<int> vecTag;
            vector<int> pass_skill;
            if(myhero.penetrateProp>0){//穿透攻击
                pair<vector<int>,vector<int>> pair1 = CompeteLayer::findAttackPenetration(posit, 1, isEnemy);
                vecTag = pair1.first;
                pass_skill = pair1.second;
            }
            else{
                vecTag = CompeteLayer::findAttackPosition(posit, 1, isEnemy);
                pass_skill = vector<int>(vecTag.size(),0);
            }
            attack_list.push_back(pair<int,vector<int>>(roleTag,vecTag));//TO DO....
            for (int i=0; i<vecTag.size(); ++i) {
                if(i==1){
                    log("adding positions from enemy: %d,穿透",vecTag[i]);
                }else{
                log("adding positions from enemy: %d",vecTag[i]);
                }
            }
            vector<int> critic_doubleAttack;//是否暴击
            vector<float> damage1;//伤害值
            vector<int> block_defense;//是否格挡
            vector<float> damage_delaytime;//攻击---显示伤害值数字时间间隔
            
            for(int i=0;i<vecTag.size();++i){
                if(myhero.engname=="caoxing"||myhero.engname=="huangzhong"){
                    damage_delaytime.push_back(0.6f);
                }
                else{
                    damage_delaytime.push_back(1.1f);
                }
                HeroType hero1 = enemyMap.find(vecTag[i])->second;
                float damage = myhero.attackDisplay*(1-attack_perp(hero1.defenseDisplay, hero1.level));
                if(pass_skill[i]==2){//穿透
                    log("穿透伤害!穿透比例为%f",myhero.penetrateProp);
                    damage*=myhero.penetrateProp;
                }
                damage1.push_back(damage);
                float critic_damage = criticDamage(myhero.criticDisplay, myhero.level, damage);
                critic_doubleAttack.push_back((int)isCritic);//第一个是否暴击加入进去  TO DO....
            
            //格挡概率
                int blockprop = blockProb(myhero.agilityDisplay, hero1.agilityDisplay);
                block_defense.push_back(blockprop);
                critic_damage *= (blockprop)?0.75:1;
            //---------------------
                enemyMap.find(vecTag[i])->second.healthDisplay -= floor(critic_damage);
                CCLOG("对目标 %s的伤害为%d,是否格挡:%d, 剩余血量为: %f",hero1.name.c_str(),(int)critic_damage,blockprop,enemyMap.find(vecTag[i])->second.healthDisplay);
                if(floor(enemyMap.find(vecTag[i])->second.healthDisplay)<=0){
                    CCLOG("角色:%s死亡!",enemyMap.find(vecTag[i])->second.name.c_str());
                    userMap[userVec[userIndex].second.position].battle_kill++;
                    //enemyRole[tag]->runAction(Sequence::create(DelayTime::create(1.1f),CallFuncN::create(CC_CALLBACK_1(CompeteLayer::die,this)),NULL));
                    if(i==0)
                        isDoubleAttack = false;//TO DO...如果第一击死了无论如何都没有双击了,以后可以加死了也有双击效果
                }

            }
            //TO DO...在这里加入英雄死亡动画，需要有一个delayTime跟据攻击方攻击时间，一般是1.1f，如果这个地方可以的话，共6个地方需要加
            if(floor(enemyMap.find(vecTag[0])->second.healthDisplay)>0){//如果双击的话第一击没死
                if(isDoubleAttack){
                    for(int i=0;i<vecTag.size();++i){
                        if(myhero.engname=="caoxing"||myhero.engname=="huangzhong"){
                            damage_delaytime.push_back(0.6f+0.9f);
                        }
                        else{
                            damage_delaytime.push_back(1.1f+0.9f);
                        }
                        if(enemyMap.find(vecTag[i])->second.healthDisplay<=0)
                            continue;
                        HeroType hero1 = enemyMap.find(vecTag[i])->second;
                        float damage = myhero.attackDisplay*(1-attack_perp(hero1.defenseDisplay, hero1.level));
                        if(i==1){
                            log("穿透伤害!穿透比例为%f",myhero.penetrateProp);
                            damage*=myhero.penetrateProp;
                            pass_skill.push_back(2);
                            attack_list[attack_list.size()-1].second.push_back(vecTag[i]);
                        }
                        else{
                            pass_skill.push_back(1);//1表示双击
                            attack_list[attack_list.size()-1].second.push_back(vecTag[i]);

                        }
                        damage1.push_back(damage);
                        float critic_damage = criticDamage(myhero.criticDisplay, myhero.level, damage*0.75f);
                        critic_doubleAttack.push_back((int)isCritic);//
                        
                    //格挡概率
                        int blockprop = blockProb(myhero.agilityDisplay, hero1.agilityDisplay);
                        block_defense.push_back(blockprop);
                        critic_damage *= (blockprop)?0.75:1;
                    //---------------------
                        enemyMap.find(vecTag[i])->second.healthDisplay -= floor(critic_damage);
                        CCLOG("触发双击对目标 %s的伤害为%d,是否格挡:%d, 剩余血量为: %f",hero1.name.c_str(),(int)critic_damage,blockprop,enemyMap.find(vecTag[i])->second.healthDisplay);
                        if(floor(enemyMap.find(vecTag[i])->second.healthDisplay)<=0){
                            CCLOG("角色:%s死亡!",enemyMap.find(vecTag[i])->second.name.c_str());
                            userMap[userVec[userIndex].second.position].battle_kill++;
                        }
                        
                    }
                    delayTime = double_dlytime;
                }
            }
            //push所有数值
            double_list.push_back(isDoubleAttack);
            //totalDelayTime+=delayTime;
            delay_time.push_back(delayTime);//TO DO...加延迟时间
            damage_list.push_back(damage1);
            critic_list.push_back(critic_doubleAttack);
            block_list.push_back(block_defense);
            passive_skill.push_back(pair<int,vector<int>>(0,pass_skill));
            damage_delaytime_list.push_back(damage_delaytime);
        }
        else{//发动技能
            CCLOG("发动技能");
            CompeteLayer::computeAllDamage(myhero.target, isEnemy);
        }
    }
    else{//敌方行动
        int roleTag = enemyVec[enemyIndex].second.position;
        int posit = (roleTag>=3)?(roleTag-3):roleTag;
        HeroType myhero = enemyVec[enemyIndex].second;
        delayTime = normal_dlytime;
        if(!isUseSkill){
            CCLOG("普通攻击");
            vector<int> vecTag;
            vector<int> pass_skill;

            if(myhero.penetrateProp>0){//穿透
                pair<vector<int>,vector<int>> pair1 = CompeteLayer::findAttackPenetration(posit, 1, isEnemy);
                vecTag = pair1.first;
                pass_skill = pair1.second;
            }
            else{
                vecTag = CompeteLayer::findAttackPosition(posit, 1, isEnemy);
                pass_skill = vector<int>(vecTag.size(),0);

            }
            //vector<int> vecTag = CompeteLayer::findAttackPosition(posit, 1, isEnemy);
            attack_list.push_back(pair<int,vector<int>>(roleTag,vecTag));//TO DO...
            for (auto i=0; i<vecTag.size(); ++i){
                if(i==1){
                    log("adding positions from enemy: %d,穿透",vecTag[i]);
                }
                log("adding positions from enemy: %d",vecTag[i]);
            }
            vector<int> critic_doubleAttack;
            vector<float> damage1;
            vector<int> block_defense;
            vector<float> damage_delaytime;
            
            for(int i=0;i<vecTag.size();++i){
                if(myhero.engname=="caoxing"||myhero.engname=="huangzhong"){
                    damage_delaytime.push_back(0.6f);
                }
                else{
                    damage_delaytime.push_back(1.1f);
                }

                HeroType hero1 = userMap.find(vecTag[i])->second;
                float damage = enemyVec[enemyIndex].second.attackDisplay*(1-attack_perp(hero1.defenseDisplay, hero1.level));
                //TO DO....加暴击
                if(pass_skill[i]==2){//穿透
                    log("穿透伤害!穿透比例为%f",myhero.penetrateProp);
                    damage*=myhero.penetrateProp;
                    
                }

                if((new_stage==0||new_stage==2)&&vecTag[i]==0){
                    damage1.push_back(1);//第一场副本对主角伤害为1
                }
                else{
                    damage1.push_back(damage);
                }
                float critic_damage = criticDamage(myhero.criticDisplay, myhero.level, damage);
                critic_doubleAttack.push_back((int)isCritic);
            //格挡概率
                int blockprop = blockProb(myhero.agilityDisplay, hero1.agilityDisplay);
                if(blockprop&&is_unlock_skill){
                    unlock_cal_time(hero1,9);//9是格挡
                }
                if(new_stage==0&&round==1&&myhero.position==0){//第一回合位置0的小兵攻击主角
                    blockprop = 1;
                }
                block_defense.push_back(blockprop);
                critic_damage *= (blockprop)?0.75:1;

            //-------
                if((new_stage==0||new_stage==2)&&vecTag[i]==0){
                    userMap.find(vecTag[i])->second.healthDisplay -= 1;
                }else{
                    userMap.find(vecTag[i])->second.healthDisplay -= floor(critic_damage);
                }
                CCLOG("对目标 %s的伤害为%d,是否格挡:%d,剩余血量为: %f",hero1.name.c_str(),(int)critic_damage,blockprop,userMap.find(vecTag[i])->second.healthDisplay);
                if(floor(userMap.find(vecTag[i])->second.healthDisplay)<=0){
                    CCLOG("角色:%s死亡!",userMap.find(vecTag[i])->second.name.c_str());
                    enemyMap[enemyVec[enemyIndex].second.position].battle_kill++;
                    if(i==0)
                        isDoubleAttack = false;
                }
            }
            if(floor(userMap.find(vecTag[0])->second.healthDisplay)>0){//如果双击的话第一击没死
                if(isDoubleAttack){
                    for(int i=0;i<vecTag.size();++i){
                        if(myhero.engname=="caoxing"||myhero.engname=="huangzhong"){
                            damage_delaytime.push_back(0.6f+0.9f);
                        }
                        else{
                            damage_delaytime.push_back(1.1f+0.9f);
                        }

                        if(userMap.find(vecTag[i])->second.healthDisplay<=0)
                            continue;
                        HeroType hero1 = userMap.find(vecTag[i])->second;

                        float damage = myhero.attackDisplay*(1-attack_perp(hero1.defenseDisplay, hero1.level));
                        if(i==1){//穿透
                            log("穿透伤害!穿透比例为%f",myhero.penetrateProp);
                            damage*=myhero.penetrateProp;
                            pass_skill.push_back(2);
                            attack_list[attack_list.size()-1].second.push_back(vecTag[i]);//记录双击后边的穿透目标

                        }
                        else{
                            pass_skill.push_back(1);//双击
                            attack_list[attack_list.size()-1].second.push_back(vecTag[i]);//记录双击后边的穿透目标

                        }

                        if((new_stage==0||new_stage==2)&&vecTag[i]==0){
                            damage1.push_back(1);//第一场副本对主角伤害为1
                        }
                        else{
                            damage1.push_back(damage);
                        }

                        float critic_damage = criticDamage(myhero.criticDisplay, myhero.level, damage*0.75f);
                        critic_doubleAttack.push_back((int)isCritic);//第一个是否暴击加入进去  TO DO....
                        //格挡概率
                        int blockprop = blockProb(myhero.agilityDisplay, hero1.agilityDisplay);
                        block_defense.push_back(blockprop);
                        critic_damage *= (blockprop)?0.75:1;
                        if(blockprop&&is_unlock_skill){
                        unlock_cal_time(hero1,9);//9是格挡
                        }

                        if((new_stage==0||new_stage==2)&&vecTag[i]==0){
                            userMap.find(vecTag[i])->second.healthDisplay -= 1;
                        }else{
                            userMap.find(vecTag[i])->second.healthDisplay -= floor(critic_damage);
                        }
                        CCLOG("触发双击:对目标 %s的伤害为%f,是否格挡:%d,剩余血量为: %f",hero1.name.c_str(), critic_damage, blockprop, userMap.find(vecTag[0])->second.healthDisplay);
                        critic_doubleAttack.push_back((int)isCritic);
                        if(floor(userMap.find(vecTag[i])->second.healthDisplay)<=0){
                            CCLOG("角色:%s死亡!",userMap.find(vecTag[i])->second.name.c_str());
                            enemyMap[enemyVec[enemyIndex].second.position].battle_kill++;
                        }
                    }
                    delayTime = double_dlytime;

                }
            }
            double_list.push_back(isDoubleAttack);
            //totalDelayTime += delayTime;
            delay_time.push_back(delayTime);
            damage_list.push_back(damage1);
            critic_list.push_back(critic_doubleAttack);
            block_list.push_back(block_defense);
            passive_skill.push_back(pair<int,vector<int>>(0,pass_skill));
            damage_delaytime_list.push_back(damage_delaytime);
        }
        else{//发动技能
            CCLOG("发动技能");
            vector<int> vecTag;
            CompeteLayer::computeAllDamage(myhero.target, isEnemy);
            
        }
    }
}
//计算发动技能后,所有目标单位伤害
void CompeteLayer::computeAllDamage(int target,bool isEnemy){
    if (isEnemy) {
        int roleTag = userVec[userIndex].second.position;
        int posit = (roleTag>=3)?(roleTag-3):roleTag;

        HeroType myHero = userVec[userIndex].second;
        vector<int> vecTag;
        vector<int> pass_skill;
        if(myHero.penetrateProp>0){//穿透
            pair<vector<int>,vector<int>> pair1 = CompeteLayer::findAttackPenetration(posit, target, isEnemy);
            vecTag = pair1.first;
            pass_skill = pair1.second;
        }
        else{
            vecTag = CompeteLayer::findAttackPosition(posit, target, isEnemy);
            pass_skill = vector<int>(vecTag.size(),0);
        }
        
        attack_list.push_back(pair<int,vector<int>>(roleTag,vecTag));//TO DO...

        float damage = myHero.attackDisplay*myHero.parameter1;
        CCLOG("发动技能,攻击力:%f攻击系数::%f",myHero.attackDisplay,myHero.parameter1);
        CCLOG("技能只攻击伤害为:%f",damage);
        if(myHero.attackType==1||myHero.attackType==3)//看破和敏捷是一回事,附加敏捷伤害
            damage += myHero.agilityDisplay*myHero.parameter2;
        else if(myHero.attackType==2){//附加暴击伤害
            damage += myHero.criticDisplay*myHero.parameter2;
        }
        vector<float> damagelist;
        vector<int> critic_doubleAttack;
        vector<int> block_defense;
        vector<float> damage_delaytime;
        
        for (auto i=0; i<vecTag.size(); ++i) {//遍历所有攻击目标
            if(myHero.engname=="caoxing"||myHero.engname=="huangzhong"){
                damage_delaytime.push_back(0.6f);
            }
            else{
                damage_delaytime.push_back(1.1f);
            }
            
            HeroType hero1 = enemyMap.find(vecTag[i])->second;
            CCLOG("目标减血：%s",hero1.name.c_str());
            CCLOG("防御抵挡伤害系数为:%f",(float)attack_perp(hero1.defenseDisplay, hero1.level));
            float damage1=damage*(1-attack_perp(hero1.defenseDisplay, hero1.level));//攻击单体的伤害下边要显示出来
            if(pass_skill[i]==2){//表示穿透
                damage1*=myHero.penetrateProp;
                log("穿透比例为%f,位置为%d",myHero.penetrateProp,i);
            }
            damagelist.push_back(damage1);
            float critic_damage = criticDamage(myHero.criticDisplay, myHero.level, damage1);
            critic_doubleAttack.push_back((int)isCritic);//第一个是否暴击加入进去  TO DO....
            //格挡概率
            int blockprop = blockProb(myHero.agilityDisplay, hero1.agilityDisplay);
            block_defense.push_back(blockprop);
            critic_damage *= ((blockprop)?0.75:1);

            //----------------
            enemyMap.find(vecTag[i])->second.healthDisplay -= floor(critic_damage);//最终减少的血量
            CCLOG("对目标 %s的伤害为%f,是否格挡:%d,剩余血量为: %f",hero1.name.c_str(),critic_damage,blockprop,enemyMap.find(vecTag[i])->second.healthDisplay);
            if(floor(enemyMap.find(vecTag[i])->second.healthDisplay)<=0){
                CCLOG("角色:%s死亡!",enemyMap.find(vecTag[i])->second.name.c_str());
                userMap[userVec[userIndex].second.position].battle_kill++;
                if(i==0)
                    isDoubleAttack = false;//TO DO...如果第一击死了无论如何都没有双击了,以后可以加死了也有双击效果
            }
        }
        if(floor(enemyMap.find(vecTag[0])->second.healthDisplay)>0){//如果双击的话,并且主目标技能伤害没死
            if(isDoubleAttack){
                CCLOG("发动技能之后双击!");
                vector<int> tag1;
                tag1.push_back(vecTag[0]);
                pass_skill.push_back(1);//主目标双击
                for(int i=0;i<pass_skill.size();++i){//找到所有双击需要穿透的目标
                    if(pass_skill[i]==2){
                        if(enemyMap.find(vecTag[i])->second.healthDisplay>0){
                            attack_list[attack_list.size()-1].second.push_back(vecTag[i]);//记录双击后边的穿透目标
                            tag1.push_back(i);
                            pass_skill.push_back(2);
                        }
                    }
                }
                delayTime = double_dlytime;
                for (int i=0; i<tag1.size(); ++i) {
                    if(myHero.engname=="caoxing"||myHero.engname=="huangzhong"){
                        damage_delaytime.push_back(0.6f+0.9f);
                    }
                    else{
                        damage_delaytime.push_back(1.1f+0.9f);
                    }

                    HeroType hero1 = enemyMap.find(tag1[i])->second;
                    float damage = myHero.attackDisplay*(1-attack_perp(hero1.defenseDisplay, hero1.level));
                    if(i>=1){//表示穿透
                        damage*=myHero.penetrateProp;
                        log("穿透比例为%f,位置为%d",myHero.penetrateProp,i);
                        attack_list[attack_list.size()-1].second.push_back(tag1[i]);//记录双击后边的穿透目标
                    }
                    else{
                        pass_skill.push_back(1);
                        attack_list[attack_list.size()-1].second.push_back(tag1[i]);//记录双击后边的穿透目标

                    }
                    
                    float critic_damage = criticDamage(myHero.criticDisplay, myHero.level, damage*0.75f);
                    critic_doubleAttack.push_back((int)isCritic);//第一个是否暴击加入进去  TO DO....
                    damagelist.push_back(damage);
                    //格挡概率
                    int blockprop = blockProb(myHero.agilityDisplay, hero1.agilityDisplay);
                    block_defense.push_back(blockprop);
                    critic_damage *= (blockprop)?0.75:1;
                    
                    enemyMap.find(tag1[i])->second.healthDisplay -= floor(critic_damage);
                    CCLOG("对目标 %s的伤害为%f,是否格挡:%d,剩余血量为:%f",hero1.name.c_str(), critic_damage, blockprop,enemyMap.find(tag1[i])->second.healthDisplay);
                    if(floor(enemyMap.find(tag1[i])->second.healthDisplay)<=0){
                        CCLOG("角色:%s死亡!",hero1.name.c_str());
                        userMap[userVec[userIndex].second.position].battle_kill++;
                    }

                }
            }
            else{
                delayTime = normal_dlytime;
            }
        }
        double_list.push_back(isDoubleAttack);
        delay_time.push_back(delayTime);
        damage_list.push_back(damagelist);
        critic_list.push_back(critic_doubleAttack);
        block_list.push_back(block_defense);
        passive_skill.push_back(pair<int,vector<int>>(0,pass_skill));
        damage_delaytime_list.push_back(damage_delaytime);
    }
    else{
        int roleTag = enemyVec[enemyIndex].second.position;
        int posit = (roleTag>=3)?(roleTag-3):roleTag;
        
        HeroType myHero = enemyVec[enemyIndex].second;
        vector<int> vecTag;
        vector<int> pass_skill;
        if(myHero.penetrateProp>0){//穿透
            pair<vector<int>,vector<int>> pair1 = CompeteLayer::findAttackPenetration(posit, target, isEnemy);
            vecTag = pair1.first;
            pass_skill = pair1.second;
        }
        else{
            vecTag = CompeteLayer::findAttackPosition(posit, target, isEnemy);
            pass_skill = vector<int>(vecTag.size(),0);
        }
        
        attack_list.push_back(pair<int,vector<int>>(roleTag,vecTag));//TO DO...

        float damage = myHero.attackDisplay*myHero.parameter1;
        CCLOG("发动技能,攻击力:%f攻击系数::%f",myHero.attackDisplay,myHero.parameter1);
        CCLOG("技能只攻击伤害为:%f",damage);
        if(myHero.attackType==1||myHero.attackType==3)//看破和敏捷是一回事,如果附加敏捷伤害
            damage += myHero.agilityDisplay*myHero.parameter2;
        else if(myHero.attackType==2){//如果附加暴击伤害
            damage+=myHero.criticDisplay*myHero.parameter2;
        }
        vector<float> damagelist;
        vector<int> critic_doubleAttack;
        vector<int> block_defense;
        vector<float> damage_delaytime;
        
        for (auto i=0; i<vecTag.size(); ++i) {//遍历所有攻击目标
            if(myHero.engname=="caoxing"||myHero.engname=="huangzhong"){
                damage_delaytime.push_back(0.6f);
            }
            else{
                damage_delaytime.push_back(1.1f);
            }

            HeroType hero1 = userMap.find(vecTag[i])->second;
            CCLOG("目标减血：%s",hero1.name.c_str());
            CCLOG("防御抵挡伤害系数为:%f",(float)attack_perp(hero1.defenseDisplay, hero1.level));
            float damage1=damage*(1-attack_perp(hero1.defenseDisplay, hero1.level));//攻击单体的伤害下边要显示出来
            if(pass_skill[i]==2){//表示穿透
                damage1*=myHero.penetrateProp;
                log("穿透比例为%f,位置为%d",myHero.penetrateProp,i);
            }

            if((new_stage==0||new_stage==2)&&vecTag[i]==0){
                damagelist.push_back(1);//第一场副本对主角伤害为1
            }
            else{
                damagelist.push_back(damage1);
            }
            float critic_damage = criticDamage(myHero.criticDisplay, myHero.level, damage1);
            critic_doubleAttack.push_back((int)isCritic);
            //格挡概率
            int blockprop = blockProb(myHero.agilityDisplay, hero1.agilityDisplay);
            block_defense.push_back(blockprop);
            critic_damage *= (blockprop)?0.75:1;
            if(blockprop&&is_unlock_skill){
                unlock_cal_time(hero1,9);//9是格挡
            }

            //----------------
            if((new_stage==0||new_stage==2)&&vecTag[i]==0){
                userMap.find(vecTag[i])->second.healthDisplay -= 1;
            }else{
                userMap.find(vecTag[i])->second.healthDisplay -= floor(critic_damage);//最终减少的血量
            }
            CCLOG("对目标 %s的伤害为%f,是否格挡:%d,剩余血量为: %f",hero1.name.c_str(), critic_damage, blockprop,userMap.find(vecTag[i])->second.healthDisplay);
            if(floor(userMap.find(vecTag[i])->second.healthDisplay)<=0){//向下取整
                CCLOG("角色:%s死亡!",userMap.find(vecTag[i])->second.name.c_str());
                enemyMap[enemyVec[enemyIndex].second.position].battle_kill++;
                if(i==0)
                    isDoubleAttack = false;//TO DO...如果第一击死了无论如何都没有双击了,以后可以加死了也有双击效果

                //userRole[*it]->runAction(Sequence::create(DelayTime::create(1.1f),CallFuncN::create(CC_CALLBACK_1(CompeteLayer::die,this)),NULL));
            }
        }
        if(floor(userMap.find(vecTag[0])->second.healthDisplay)>0){//如果双击的话第一击没死
            if(isDoubleAttack){
                CCLOG("发动技能之后双击!");
                vector<int> tag1;
                tag1.push_back(vecTag[0]);
                pass_skill.push_back(1);//主目标双击
                for(int i=0;i<pass_skill.size();++i){//穿刺
                    if(pass_skill[i]==2){
                        if(userMap.find(vecTag[i])->second.healthDisplay>0){
                            attack_list[attack_list.size()-1].second.push_back(vecTag[i]);//记录双击后边的穿透目标
                            tag1.push_back(i);
                            pass_skill.push_back(2);
                        }
                    }
                
                }
                delayTime = double_dlytime;
                for(int i=0;i<tag1.size();++i){
                    if(myHero.engname=="caoxing"||myHero.engname=="huangzhong"){
                        damage_delaytime.push_back(0.6f+0.9f);
                    }
                    else{
                        damage_delaytime.push_back(1.1f+0.9f);
                    }

                    HeroType hero1 = userMap.find(tag1[i])->second;
                    float damage = myHero.attackDisplay*(1-attack_perp(hero1.defenseDisplay, hero1.level));
                    if(i>=1){//表示穿透
                        damage*=myHero.penetrateProp;
                        log("穿透比例为%f,位置为%d",myHero.penetrateProp,i);
                        attack_list[attack_list.size()-1].second.push_back(tag1[i]);//记录双击后边的穿透目标
                    }
                    else{
                        attack_list[attack_list.size()-1].second.push_back(tag1[i]);//记录双击后边的穿透目标
                    }
                    float critic_damage = criticDamage(myHero.criticDisplay, myHero.level, damage*0.75f);
                    critic_doubleAttack.push_back((int)isCritic);//加入双击是否暴击
                //格挡概率
                    int blockprop = blockProb(myHero.agilityDisplay, hero1.agilityDisplay);
                    block_defense.push_back(blockprop);
                    critic_damage *= (blockprop)?0.75:1;
                    if(blockprop&&is_unlock_skill){
                        unlock_cal_time(hero1,9);//9是格挡
                    }
                //---------------
                    if((new_stage==0||new_stage==2)&&vecTag[0]==0){
                        damagelist.push_back(1);//第一场副本对主角伤害为1
                        userMap.find(vecTag[0])->second.healthDisplay -= 1;
                    }
                    else{
                        damagelist.push_back(damage);
                        userMap.find(vecTag[0])->second.healthDisplay -= floor(critic_damage);
                    }

                    CCLOG("对目标 %s的伤害为%f,是否格挡:%d,剩余血量为: %f",hero1.name.c_str(), critic_damage, blockprop,userMap.find(vecTag[0])->second.healthDisplay);
                    if(floor(userMap.find(vecTag[0])->second.healthDisplay)<=0){
                        CCLOG("角色:%s死亡!",hero1.name.c_str());
                        enemyMap[enemyVec[enemyIndex].second.position].battle_kill++;
                    }
                }
            }
            else{
                delayTime = normal_dlytime;

            }
        }
        double_list.push_back(isDoubleAttack);
        delay_time.push_back(delayTime);
        damage_list.push_back(damagelist);
        critic_list.push_back(critic_doubleAttack);
        block_list.push_back(block_defense);
        passive_skill.push_back(pair<int,vector<int>>(0,pass_skill));
        damage_delaytime_list.push_back(damage_delaytime);
    }
}
//找到本次攻击的所有攻击目标，附加穿透目标
pair<vector<int>,vector<int>> CompeteLayer::findAttackPenetration(int beginPosition,int attackType, bool isEnemy){//目前只穿透主目标
    vector<int> pass_skill_index;
    bool include_penetration = 0;
    vector<int> tag = findAttackPosition(beginPosition, attackType, isEnemy);
    pass_skill_index.push_back(0);//主目标肯定不是穿透目标
    for(int i=1;i<tag.size();++i){
        if(tag[i]==tag[0]+3){
            include_penetration = 1;
            pass_skill_index.push_back(2);
        }
        else{
            pass_skill_index.push_back(0);
        }
    }
    
    if((!include_penetration)){
        if(isEnemy&&enemyMap.find(tag[0]+3)!=enemyMap.end()){
            tag.push_back(tag[0]+3);
            pass_skill_index.push_back(2);
        }
        else if((!isEnemy)&&userMap.find(tag[0]+3)!=userMap.end()){
            tag.push_back(tag[0]+3);
            pass_skill_index.push_back(2);
        }
    }
    
    return pair<vector<int>,vector<int>>(tag,pass_skill_index);
    
    
}
//找到本次攻击的所有攻击目标
vector<int> CompeteLayer::findAttackPosition(int beginPosition,int attackType,bool isEnemy){//beginPosition是从默认目标开始找
    vector<int> attackPositions;
    if (isEnemy) {//表示攻击目标位敌方
        if (attackType==1) {//攻击单体
            if (enemyMap.find(beginPosition)==enemyMap.end()) {//TO DO....这里有问题，后排还是打后排
                //往旁边的目标寻找
                attackPositions.push_back(CompeteLayer::findNextPosition(beginPosition, beginPosition, isEnemy));
                
                return attackPositions;
            }
            else{//默认目标还存活，即攻击默认目标
                attackPositions.push_back(beginPosition);
                return attackPositions;
            }
        }
        else if(attackType==2){//攻击一排
 //目前一排攻击，如果第一排只有1个人，不管位置如何还是打第一排
            if(enemyMap.find(0)!=enemyMap.end())
                attackPositions.push_back(0);
            if(enemyMap.find(1)!=enemyMap.end())
                attackPositions.push_back(1);
            if(enemyMap.find(2)!=enemyMap.end())
                attackPositions.push_back(2);
            if(attackPositions.size()==0){//前排全部战死
                if(enemyMap.find(3)!=enemyMap.end())
                    attackPositions.push_back(3);
                if(enemyMap.find(4)!=enemyMap.end())
                    attackPositions.push_back(4);
                if(enemyMap.find(5)!=enemyMap.end())
                    attackPositions.push_back(5);
            }

            return attackPositions;
            
        }
        else if(attackType==3){//攻击一列
            int pos = CompeteLayer::findNextPosition(beginPosition, beginPosition, isEnemy);
            attackPositions.push_back(pos);//先按单体攻击一样找目标
            if(pos<=2&&enemyMap.find(pos+3)!=enemyMap.end())
                attackPositions.push_back(pos+3);
            return attackPositions;
        }
        else if(attackType==4){//攻击全体
            for(auto it = enemyMap.begin();it!=enemyMap.end();++it){
                CCLOG("adding positions from enemy: %d",it->first);
                attackPositions.push_back(it->first);
            }
            return attackPositions;
        }
        else if(attackType==5){//随机单体
            int randomIndex = arc4random()%(enemyVec.size());
            attackPositions.push_back(enemyVec[randomIndex].first);
            return attackPositions;
        }
        else if(attackType==6){//随机三人
            
        }
        else if(attackType==7){//攻击后排全体
            if(enemyMap.find(3)!=enemyMap.end())
                attackPositions.push_back(3);
            if(enemyMap.find(4)!=enemyMap.end())
                attackPositions.push_back(4);
            if(enemyMap.find(5)!=enemyMap.end())
                attackPositions.push_back(5);
            if (attackPositions.size()==0) {//表示后排全阵亡，所以攻击前排
                CCLOG("后排阵亡，攻击前排");
                for (auto it=enemyMap.begin(); it!=enemyMap.end(); ++it) {
                    attackPositions.push_back(it->first);
                }
            }
            for (auto it=attackPositions.begin(); it!=attackPositions.end(); ++it) {
                CCLOG("adding positions from enemy: %d",*it);
            }
            return attackPositions;
        }
        else if(attackType==8){//攻击血最少的目标,假设只有一个
            float minHealth = 10000;
            int minIndex;
            for(auto it = enemyMap.begin();it!=enemyMap.end();++it){
                if(it->second.healthDisplay<minHealth){
                    minHealth = it->second.healthDisplay;
                    minIndex = it->first;
                }
            }
            attackPositions.push_back(minIndex);
            return attackPositions;
        }
        else if(attackType==9){//前后排各一人
            vector<int> firstRandNum;//为第一排的数量
            vector<int> secondRandNum;
            for (int i=0; i<competeTeam.size(); i++) {
                if(enemyMap.find(i)!=enemyMap.end()&&i<3)
                    firstRandNum.push_back(i);
                if(enemyMap.find(i)!=enemyMap.end()&&i>=3)
                    secondRandNum.push_back(i);
            }
            if(firstRandNum.size()!=0){//说明第一排有人
                if(secondRandNum.size()==0){
                    if(firstRandNum.size()==3){//3选2
                        int randomIndex = arc4random()%firstRandNum.size();
                        for(int i=0;i<3;i++){
                            if(i!=randomIndex)
                                attackPositions.push_back(firstRandNum[i]);
                        }
                    }
                    else{//有1或者2个都是全部加进去，然后返回
                        for(int i=0;i<firstRandNum.size();i++)
                            attackPositions.push_back(firstRandNum[i]);
                    }
                    return attackPositions;
                    
                }
                else{//第二排也有人
                    int randomIndex = arc4random()%firstRandNum.size();
                    attackPositions.push_back(firstRandNum[randomIndex]);
                    randomIndex = arc4random()%secondRandNum.size();
                    attackPositions.push_back(secondRandNum[randomIndex]);
                    return attackPositions;
                }
            }
            else{//第一排没人,只加入第二排
                if(secondRandNum.size()==3){//3选2
                    int randomIndex = arc4random()%secondRandNum.size();
                    for(int i=0;i<3;i++){
                        if(i!=randomIndex)
                            attackPositions.push_back(secondRandNum[i]);
                    }
                }
                else{//有1或者2个都是全部加进去，然后返回
                    for(int i=0;i<secondRandNum.size();i++)
                        attackPositions.push_back(secondRandNum[i]);
                }
                return attackPositions;
            }
            return attackPositions;
        }
        else if(attackType==10){//攻击目标及前后左右
            int pos = CompeteLayer::findNextPosition(beginPosition, beginPosition, isEnemy);
            attackPositions.push_back(pos);//先按单体攻击一样找目标
            if(enemyMap.find(pos-1)!=enemyMap.end())
                attackPositions.push_back(pos-1);
            if(enemyMap.find(pos+1)!=enemyMap.end())
                attackPositions.push_back(pos+1);
            if(enemyMap.find(pos+3)!=enemyMap.end())
                attackPositions.push_back(pos+3);

            return attackPositions;

        }

    }
    else{//敌方攻击玩家方,复制一遍上边的内容即可
        if (attackType==1) {//攻击单体
            if (userMap.find(beginPosition)==userMap.end()) {
                //往旁边的目标寻找
                attackPositions.push_back(CompeteLayer::findNextPosition(beginPosition, beginPosition, isEnemy));
                return attackPositions;
            }
            else{//默认目标还存活，即攻击默认目标
                attackPositions.push_back(beginPosition);
                return attackPositions;
            }
        }
        else if(attackType==2){//攻击一排
            if(userMap.find(0)!=userMap.end())
                attackPositions.push_back(0);
            if(userMap.find(1)!=userMap.end())
                attackPositions.push_back(1);
            if(userMap.find(2)!=userMap.end())
                attackPositions.push_back(2);
            if(attackPositions.size()==0){//前排全部战死
                if(userMap.find(3)!=userMap.end())
                    attackPositions.push_back(3);
                if(userMap.find(4)!=userMap.end())
                    attackPositions.push_back(4);
                if(userMap.find(5)!=userMap.end())
                    attackPositions.push_back(5);
            }
                return attackPositions;
            
        }
        else if(attackType==3){//攻击一列
            int pos = CompeteLayer::findNextPosition(beginPosition, beginPosition, isEnemy);
            attackPositions.push_back(pos);//先按单体攻击一样找目标
            if(pos<=2&&userMap.find(pos+3)!=userMap.end())
                attackPositions.push_back(pos+3);
            return attackPositions;
        }
        else if(attackType==4){//攻击全体
            for(auto it = userMap.begin();it!=userMap.end();++it){
                attackPositions.push_back(it->first);
                CCLOG("adding positions from user: %d",it->first);
            }
            return attackPositions;
        }
        else if(attackType==5){//随机单体 TO DO.....
            int randomIndex = arc4random()%(userMap.size());
            attackPositions.push_back(userVec[randomIndex].first);
            return attackPositions;
        }
        else if(attackType==6){//随机三人
            
        }
        else if(attackType==7){//攻击后排全体
            if(userMap.find(3)!=userMap.end())
                attackPositions.push_back(3);
            if(userMap.find(4)!=userMap.end())
                attackPositions.push_back(4);
            if(userMap.find(5)!=userMap.end())
                attackPositions.push_back(5);
            if (attackPositions.size()==0) {//表示后排全阵亡了，所以攻击前排
                CCLOG("后排阵亡，攻击前排");
                for (auto it=userMap.begin(); it!=userMap.end(); ++it) {
                    attackPositions.push_back(it->first);
                }
            }
            for (auto it=attackPositions.begin(); it!=attackPositions.end(); ++it) {
                CCLOG("adding positions from enemy: %d",*it);
            }

            return attackPositions;
        }
        else if(attackType==8){//攻击血最少的目标,假设只有一个
            float minHealth = 10000;
            int minIndex;
            for(auto it = userMap.begin();it!=userMap.end();++it){
                if(it->second.healthDisplay<minHealth){
                    minHealth = it->second.healthDisplay;
                    minIndex = it->first;
                }
            }
            attackPositions.push_back(minIndex);
            return attackPositions;
        }
        else if(attackType==9){
            vector<int> firstRandNum;//为第一排的数量
            vector<int> secondRandNum;
            for (int i=0; i<competeTeam.size(); i++) {
                if(userMap.find(i)!=userMap.end()&&i<3)
                    firstRandNum.push_back(i);
                if(userMap.find(i)!=userMap.end()&&i>=3)
                    secondRandNum.push_back(i);
            }
            if(firstRandNum.size()!=0){//说明第一排有人
                if(secondRandNum.size()==0){
                    if(firstRandNum.size()==3){//3选2
                        int randomIndex = arc4random()%firstRandNum.size();
                        for(int i=0;i<3;i++){
                            if(i!=randomIndex)
                                attackPositions.push_back(firstRandNum[i]);
                        }
                    }
                    else{//有1或者2个都是全部加进去，然后返回
                        for(int i=0;i<firstRandNum.size();i++)
                            attackPositions.push_back(firstRandNum[i]);
                    }
                    return attackPositions;
                    
                }
                else{//第二排也有人
                    int randomIndex = arc4random()%firstRandNum.size();
                    attackPositions.push_back(firstRandNum[randomIndex]);
                    randomIndex = arc4random()%secondRandNum.size();
                    attackPositions.push_back(secondRandNum[randomIndex]);
                    return attackPositions;
                }
            }
            else{//第一排没人,只加入第二排
                if(secondRandNum.size()==3){//3选2
                    int randomIndex = arc4random()%secondRandNum.size();
                    for(int i=0;i<3;i++){
                        if(i!=randomIndex)
                            attackPositions.push_back(secondRandNum[i]);
                    }
                }
                else{//有1或者2个都是全部加进去，然后返回
                    for(int i=0;i<secondRandNum.size();i++)
                        attackPositions.push_back(secondRandNum[i]);
                }
                return attackPositions;
            }
            return attackPositions;
        }
        else if(attackType==10){//攻击目标及前后左右
            int pos = CompeteLayer::findNextPosition(beginPosition, beginPosition, isEnemy);
            attackPositions.push_back(pos);//先按单体攻击一样找目标
            if(userMap.find(pos-1)!=userMap.end())
                attackPositions.push_back(pos-1);
            if(userMap.find(pos+1)!=userMap.end())
                attackPositions.push_back(pos+1);
            if(userMap.find(pos+3)!=userMap.end())
                attackPositions.push_back(pos+3);
            
            return attackPositions;
            
        }

    }
    
    return attackPositions;
}
//找到攻击的主目标
int CompeteLayer::findNextPosition(const int startingPosition,int beginPosition,bool isEnemy){
    if(isEnemy){
        if(enemyMap.find(beginPosition)==enemyMap.end()){
            if(beginPosition==0){
                return findNextPosition(startingPosition, 1, isEnemy);
            }
            else if (beginPosition==2)
                return findNextPosition(startingPosition, 1, isEnemy);
            else if (beginPosition==1){
                if(startingPosition==0)
                    return findNextPosition(startingPosition, (startingPosition+3), isEnemy);
                else if(startingPosition==1){
                    if(enemyMap.find(beginPosition-1)==enemyMap.end()){
                        if (enemyMap.find(beginPosition+1)==enemyMap.end()) {
                            //左右两边都死掉了
                            return findNextPosition(startingPosition, (startingPosition+3), isEnemy);
                        }
                        else
                            return (beginPosition+1);
                    }
                    else{
                        if (enemyMap.find(beginPosition+1)==enemyMap.end()) {
                            return (beginPosition-1);
                        }
                        else//左右都有活人
                            return (enemyMap.at(beginPosition-1).healthDisplay<=enemyMap.at(beginPosition+1).healthDisplay) ? (beginPosition-1):(beginPosition+1);
                    }
                }
                else if(startingPosition==2){
                    return findNextPosition(startingPosition, (startingPosition+3), isEnemy);
                }
            }
            else if(beginPosition==3){
                return findNextPosition(startingPosition, (beginPosition+1), isEnemy);
            }
            else if(beginPosition==4){
                if(startingPosition==0){
                    if(enemyMap.find(2)==enemyMap.end()){
                        if (enemyMap.find(5)==enemyMap.end()) {
                            CCLOG("玩家获胜!");
                            unschedule(schedule_selector(CompeteLayer::beginCompete));

                            return 0;//此条件不可能发生
                        }
                        else
                            return 5;
                    }
                    else
                        return 2;
                }
                else if(startingPosition==1){
                    if(enemyMap.find(beginPosition-1)==enemyMap.end()){
                        if (enemyMap.find(beginPosition+1)==enemyMap.end()) {
                            CCLOG("玩家获胜!");
                            unschedule(schedule_selector(CompeteLayer::beginCompete));

                            return 0;//此条件不可能发生
                        }
                        else
                            return (beginPosition+1);
                    }
                    else{
                        if (enemyMap.find(beginPosition+1)==enemyMap.end()) {
                            //左右两边都死掉了
                            return (beginPosition-1);
                        }
                        else//左右都有活人
                            return (enemyMap.at(beginPosition-1).healthDisplay<=enemyMap.at(beginPosition+1).healthDisplay) ? (beginPosition-1):(beginPosition+1);
                    }
                }
                else if(startingPosition==2){
                    if(enemyMap.find(0)==enemyMap.end()){
                        if (enemyMap.find(3)==enemyMap.end()) {
                            CCLOG("玩家获胜!");
                            unschedule(schedule_selector(CompeteLayer::beginCompete));
                            
                            return 0;//此条件不可能发生
                        }
                        else
                            return 3;
                    }
                    else
                        return 0;

                }

            }
            else if(beginPosition==5){//只可能starting是2
                return findNextPosition(startingPosition, (beginPosition-1), isEnemy);
            }
        }
        else{
            return beginPosition;
        }
    }
    else{
        //重复一遍上边的所有内容，把enemyMap改成userMap
        if(userMap.find(beginPosition)==userMap.end()){
            if(beginPosition==0){
                return findNextPosition(startingPosition, 1, isEnemy);
            }
            else if (beginPosition==2)
                return findNextPosition(startingPosition, 1, isEnemy);
            else if (beginPosition==1){
                if(startingPosition==0)
                    return findNextPosition(startingPosition, (startingPosition+3), isEnemy);
                else if(startingPosition==1){
                    if(userMap.find(beginPosition-1)==userMap.end()){
                        if (userMap.find(beginPosition+1)==userMap.end()) {
                            //左右两边都死掉了
                            return findNextPosition(startingPosition, (startingPosition+3), isEnemy);
                        }
                        else
                            return (beginPosition+1);
                    }
                    else{
                        if (userMap.find(beginPosition+1)==userMap.end()) {
                            //左右两边都死掉了
                            return (beginPosition-1);
                        }
                        else//左右都有活人
                            return (userMap.at(beginPosition-1).healthDisplay<=userMap.at(beginPosition+1).healthDisplay) ? (beginPosition-1):(beginPosition+1);
                    }
                }
                else if(startingPosition==2){
                    return findNextPosition(startingPosition, (startingPosition+3), isEnemy);
                }
            }
            else if(beginPosition==3){
                return findNextPosition(startingPosition, (beginPosition+1), isEnemy);
            }
            else if(beginPosition==4){
                if(startingPosition==0){
                    if(userMap.find(2)==userMap.end()){
                        if (userMap.find(5)==userMap.end()) {
                            CCLOG("玩家获胜!");
                            unschedule(schedule_selector(CompeteLayer::beginCompete));

                            return 0;//此条件不可能发生
                        }
                        else
                            return 5;
                    }
                    else
                        return 2;
                }
                else if(startingPosition==1){
                    if(userMap.find(beginPosition-1)==userMap.end()){
                        if (userMap.find(beginPosition+1)==userMap.end()) {
                            //左右两边都死掉了
                            CCLOG("玩家获胜!");
                            unschedule(schedule_selector(CompeteLayer::beginCompete));
                            
                            return 0;//此条件不可能发生
                        }
                        else
                            return (beginPosition+1);
                    }
                    else{
                        if (userMap.find(beginPosition+1)==userMap.end()) {
                            //左右两边都死掉了
                            return (beginPosition-1);
                        }
                        else//左右都有活人
                            return (userMap.at(beginPosition-1).healthDisplay<=userMap.at(beginPosition+1).healthDisplay) ? (beginPosition-1):(beginPosition+1);
                    }
                }
                else if(startingPosition==2){
                    if(userMap.find(0)==userMap.end()){
                        if (userMap.find(3)==userMap.end()) {
                            CCLOG("玩家获胜!");
                            unschedule(schedule_selector(CompeteLayer::beginCompete));
                            
                            return 0;//此条件不可能发生
                        }
                        else
                            return 3;
                    }
                    else
                        return 0;
                }
                
            }
            else if(beginPosition==5){
                return findNextPosition(startingPosition, (beginPosition-1), isEnemy);
            }
        }
        else{
            return beginPosition;
        }
    }
    return beginPosition;
}

void CompeteLayer::initUsersRole(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    for (auto it=userMap.begin();it!=userMap.end();++it) {
        CompeteRole* roleUser = new CompeteRole();
        it->second.battle_kill = 0;
        roleUser->init(it->second, this);//TODO:需要补充完整初始化所有Sprite的属性
        if(it->first>=3){//攻城战中，敌我双方英雄位置
            if(isgongchengzhan){
                roleUser->setPosition(userposition_withqixie[it->first]);
            }
            else{
                roleUser->setPosition(userposition[it->first]);
            }
        }
        else{
            if(isgongchengzhan){
                roleUser->setPosition(userposition_withqixie[it->first]);
            }
            else{
                roleUser->setPosition(userposition[it->first]);
            }
        }
        roleUser->pos = roleUser->getPosition();
        roleUser->enemy = 1;
        roleUser->tag = 100+it->first;
        this->addChild(roleUser,6,100+it->first);//TO DO.....z-index，tag：101是英雄列表里第一个，后边逐个+1
        userRole[it->first] = roleUser;//TO DO...有可能需要变成map，便于定位
    }
    //非攻城战，双方位置
    for (auto it=enemyMap.begin();it!=enemyMap.end();++it) {
        CompeteRole* roleUser = new CompeteRole();
        it->second.battle_kill = 0;
        roleUser->init(it->second, this);
        if(it->first>=3){
            if(isgongchengzhan){
                roleUser->setPosition(enemyposition_withqixie[it->first]);
            }
            else{
                roleUser->setPosition(enemyposition[it->first]);
            }
        }
        else{
            if(isgongchengzhan){
                roleUser->setPosition(enemyposition_withqixie[it->first]);
            }
            else{
                roleUser->setPosition(enemyposition[it->first]);
            }
        }
        roleUser->pos = roleUser->getPosition();
        roleUser->enemy = 2;
        roleUser->tag = 200+it->first;

        this->addChild(roleUser,6,200+it->first);//靠近中间的为tag0-2,外围是3-5
        enemyRole[it->first] = roleUser;
    }
//攻城战，器械位置
    if(isgongchengzhan){
        for(int i=0;i<userqixieMap.size();i++){
            QixieRole* qixieUser = new QixieRole();
            qixieUser->init(userqixieMap.at(i), this);//TO DO....需要补充完整初始化所有Sprite的属性
            QixieRole* qixieEnemy = new QixieRole();
            qixieEnemy->init(enemyqixieMap.at(i), this);
            
            qixieUser->setPosition(user_qixieposition[i]);
            qixieEnemy->setPosition(enemy_qixieposition[i]);
            
            qixieUser->pos = qixieUser->getPosition();
            qixieEnemy->pos = qixieEnemy->getPosition();
            this->addChild(qixieUser,5,300+i);
            this->addChild(qixieEnemy,5,400+i);
            userQixieRole.push_back(qixieUser);
            enemyQixieRole.push_back(qixieEnemy);
        }
    }
    if(new_stage==0){//新手副本一,初始位置，之后需要移动到战场
        userRole[0]->setPosition(userposition[0].x,-120);
        userRole[4]->setPosition(userposition[4].x,-150);
        userRole[1]->setPosition(-120,userposition[1].y);
        userRole[2]->setPosition(visibleSize.width+120,userposition[2].y);

        enemyRole[0]->setPosition(enemyposition[0].x,visibleSize.height+120);
        enemyRole[2]->setPosition(enemyposition[2].x,visibleSize.height+120);
        enemyRole[4]->setPosition(enemyposition[4].x,visibleSize.height+120);

    }
    else if(new_stage==2){//新手副本二,初始位置，之后需要移动到战场
        userRole[0]->setPosition(userposition[0].x,-120);
        userRole[4]->setPosition(-120,userposition[4].y);
        userRole[1]->setPosition(userposition[1].x,-120);
        userRole[2]->setPosition(visibleSize.width+120,userposition[2].y);
        userRole[1]->setScale(2);
        
        enemyRole[3]->setPosition(enemyposition[3].x,visibleSize.height+120);
        enemyRole[5]->setPosition(enemyposition[5].x,visibleSize.height+120);
        enemyRole[0]->setPosition(-120,enemyposition[0].y);
        enemyRole[2]->setPosition(visibleSize.width+120,enemyposition[2].y);
        enemyRole[4]->setPosition(enemyposition[4].x,visibleSize.height+120);
        
    }
    else if(new_stage==3){//新手副本三,初始位置，之后需要移动到战场
        userRole[3]->setPosition(userposition[3].x,-120);
        userRole[4]->setPosition(userposition[4].x,-120);
        userRole[5]->setPosition(userposition[5].x,-120);
        
        enemyRole[3]->setPosition(enemyposition[3].x,visibleSize.height+120);
        enemyRole[5]->setPosition(enemyposition[5].x,visibleSize.height+120);
        
    }
}
//给双方vector按敏捷排序，每一回合首先调用
void CompeteLayer::reArrange(){
    userVec.clear();//先清空两个vector,再从map赋值
    enemyVec.clear();
    for(auto it=userMap.begin();it!=userMap.end();++it){
        userVec.push_back(PAIR(it->first,it->second));
    }
    for(auto it=enemyMap.begin();it!=enemyMap.end();++it){
        enemyVec.push_back(PAIR(it->first,it->second));
    }
    sort(userVec.begin(),userVec.end(),CompeteLayer::SortByAgility);
    sort(enemyVec.begin(),enemyVec.end(),CompeteLayer::SortByAgility);
}
//战斗结果
void CompeteLayer::showGameResult(bool isUserWin){
    auto visibleSize = Director::getInstance()->getVisibleSize();
//MVP输入------------
    int mvp_index = 0;
    int kill_number = 0;
    if(isUserWin){
        for(auto it = userMap.begin();it!=userMap.end();++it){
            if(it->second.battle_kill>kill_number){
                kill_number = it->second.battle_kill;
                mvp_index = it->first;
            }
        }
        GameStat::getInstance()->mvphero = userMap.at(mvp_index);
    }
    else{
        for(auto it = enemyMap.begin();it!=enemyMap.end();++it){
            if(it->second.battle_kill>kill_number){
                kill_number = it->second.battle_kill;
                mvp_index = it->first;
            }
        }
        GameStat::getInstance()->mvphero = enemyMap.at(mvp_index);
    }
//------------------
//解锁被动-----------
    if(unlock_hit_time>=10){
        GameStat::getInstance()->unlock_complete = 1;
    }
    else{
        GameStat::getInstance()->unlock_complete = 0;
    }
//------------------
    auto _BattleResultLayer = new BattleResultLayer();
    if (isUserWin) {
        int deadNum = competeTeam.size()-userMap.size();
        int type = 1;
        if (competeTeam.size()==6) {
            if (deadNum<=2)
                type = 1;
            else if(deadNum<=4)
                type = 2;
            else if(deadNum<=5)
                type = 3;
        }
        else if(competeTeam.size()==5){
            if (deadNum<=2)
                type = 1;
            else if(deadNum<=3)
                type = 2;
            else if(deadNum<=4)
                type = 3;
        }
        else if(competeTeam.size()<5){
            if (deadNum<=1)
                type = 1;
            else if(deadNum<=2)
                type = 2;
            else if(deadNum<=3)
                type = 3;
        }
        _BattleResultLayer->init(type);
        //TO DO...战斗画面变黑
    }
    else{//玩家失败//TO DO....以后设置惜败和惨败
        _BattleResultLayer->init(4);
    }
    auto layercol = LayerColor::create(Color4B(0,0,0,130), visibleSize.width, visibleSize.height);
    //layercol->setPosition(visibleSize.width/2,visibleSize.height/2);
    this->addChild(layercol,8);

    if(_BattleResultLayer)
        this->getParent()->addChild(_BattleResultLayer,4,101);
}
bool CompeteLayer::SortByAgility(const PAIR &h1, const PAIR &h2)//注意：本函数的参数的类型一定要与vector中元素的类型一致
{
    return h1.second.agilityDisplay > h2.second.agilityDisplay;//升序排列
}
//英雄死亡动画
void CompeteLayer::die(Ref* pSender){
    auto sp = (CompeteRole*)pSender;
    sp->dieOut();
    //TODO:死亡需要有动画
    //sp->removeAllChildren();//remove英雄对应的血条
}
float CompeteLayer::doubleAttackProb(float agl1, float agl2){
    if (agl1>=(agl2*3)) {
        return 1;
    }
    else if(agl1>=(agl2*2)){
        return (0.6+0.4*(agl1-agl2*2)/agl2);
    }
    else if(agl1>=agl2){
        return (0.2+0.4*(agl1-agl2)/agl2);
    }
    else if(agl1<agl2)
        return (0.1+0.1*agl1/agl2);
    return 0;
}

int CompeteLayer::blockProb(float agl1, float agl2){//agl1是攻击方
    float block_prob;
    if (agl1>=(agl2*2)) {
        block_prob = 0;
    }
    else if(agl1>=agl2){//0--20%概率格挡
        block_prob = 0.2-0.2*(agl1-agl2)/agl2;
    }
    else if(agl1>=(agl2/2))//20%-50%
        block_prob = 0.5-0.3*2*(agl1-agl2/2)/agl2;
    else if(agl1>=(agl2/3))//50%-80%
        block_prob = 0.8-0.3*6*(agl1-agl2/3)/agl2;
    else if(agl1<(agl2/3))//80%-90%
        block_prob = 0.9-0.1*3*agl1/agl2;
    else
        block_prob = 1;
    float p1 = arc4random()%100/100.0f;
    if(p1<block_prob)
        return 1;//表示格挡
    else
        return 0;
}
float CompeteLayer::criticDamage(float critic, int level, float damage){
    float a1 = arc4random()%100/100.0f;
    /*srand((unsigned)time(NULL));//定义随机数 0-1 取第四个值
    for(int q=0;q<4;q++){
        a1 = rand()/double(RAND_MAX);//用于判断是否双击
    }
     */
    float criticProb = critic_prob(critic,level);
    CCLOG("随机数a1是:%f",a1);
    if(a1<=criticProb){
        CCLOG("触发暴击，伤害为%f",damage*1.5f);
        damage*=1.5f;
        isCritic = true;
    }
    else{
        CCLOG("没暴击");
        isCritic = false;
    }
    
    return damage;

}
void CompeteLayer::unlock_cal_time(HeroType hero,int type){
    for (auto it = hero.special_value.begin(); it!=hero.special_value.end(); ++it) {
        if(it->first==type){
            unlock_hit_time++;
            log("被动次数加1,目前%d次",unlock_hit_time);
        }
    }
}
void CompeteLayer::buttonCallBack(Ref* pSender, Control::EventType event) {
    CCLOG("Touch the Button");
}

#include "LayerManager/MainScene.h"
#include "HeroList/ZhenFa.h"
#include "HeroList/TeamListLayer.h"
#include "Compete/CompeteScene.h"
#include "LayerManager/HelloWorldLayer.h"
#include "ModifiedSourceCode/DMenu.h"
#include "ShieldLayer.h"
#include "GameStat.h"

USING_NS_CC;
using namespace std;
// on "init" you need to initialize your instance
ZhenFa::ZhenFa(){}
ZhenFa::~ZhenFa(){}

int selectedHero = -1;
bool ZhenFa::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    //selectedHero = &(-1);
    auto listener = EventListenerTouchOneByOne::create();//加入触摸监听器
    listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(ZhenFa::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(ZhenFa::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(ZhenFa::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto bg = Sprite::create("bg_neirong.png");
    bg->setPosition(visibleSize.width/2,visibleSize.height/2+33);
    this->addChild(bg,1);
    auto titleicon = Sprite::create("icon_titlebg.png");
    titleicon->setPosition(visibleSize.width/2,visibleSize.height-(176+33));
    this->addChild(titleicon,1);
    //TO DO...返回主页
    auto fanhuiItem = MenuItemImage::create("btn_arrow_right_disable.png","btn_arrow_right_disable.png",this,NULL);
    auto fanhui = Sprite::create("btn_arrow_right_disable.png");
    auto wujianghun = MenuItemSprite::create(fanhui,fanhui,CC_CALLBACK_1(ZhenFa::back, this));
    wujianghun->setRotation(-180);
    wujianghun->setPosition(wujianghun->getContentSize().width/2-visibleSize.width/2,0);
    fanhuiItem->setPosition(visibleSize.width/2-fanhuiItem->getContentSize().width/2,0);
    auto menu1 = Menu::create(wujianghun,fanhuiItem,NULL);
    menu1->setPosition(visibleSize.width/2,visibleSize.height-207);
    this->addChild(menu1,3);
    
//TO DO.....

    auto menu = DMenu::create();
    menu->setSwallowTouches(false);
    for (int i = 0; i<6; ++i) {//加6个可点击边框
        auto zhenfaFrame1 = MenuItemImage::create("icon-qiankunlu-haver.png","icon-qiankunlu-haver.png",CC_CALLBACK_1(ZhenFa::addHero, this));
        if (i<=2) {
            zhenfaFrame1->setPosition(150+i*170,visibleSize.height/2+150);
        }
        else
            zhenfaFrame1->setPosition(150+(i-3)*170,visibleSize.height/2-50);
        menu->addChild(zhenfaFrame1,3,FRAME_TAG+i);

    }
    menu->setPosition(Point::ZERO);
    this->addChild(menu,3,FRAME_MENU_TAG);
    
    heroMenu = DMenu::create();//英雄的Menu  TO DO...加入已在阵中的英雄
    heroMenu->setSwallowTouches(false);
    for (auto it=competeTeam.begin(); it!=competeTeam.end(); ++it) {
        CCLOG("自动加入已在阵中武将:%s,所在位置为%d",it->second.name.c_str(),it->first);
        string heropicname = heropath(it->second.engname);
        auto sp1 = Sprite::create(heropicname);
        auto heroItem = MenuItemSprite::create(sp1, sp1,NULL);//TO DO....需要换成replaceHero
        heroItem->setTag(it->first+HERO_TAG);
        heroMenu->addChild(heroItem,4);
        heroItem->setPosition(this->getChildByTag(FRAME_MENU_TAG)->getChildByTag(it->first+FRAME_TAG)->getPosition());
    }
    heroMenu->setPosition(Point::ZERO);
    this->addChild(heroMenu,4,HERO_MENU_TAG);
    //TO DO....用于进入英雄列表界面返回选中的英雄
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(ZhenFa::changeHeroFromList), "addHero", NULL);
    //NotificationObserver* ob = new NotificationObserver(this, callfuncO_selector(TeamListLayer::selectHeroToTeam), "addHero", NULL);
    
    return true;
}
bool ZhenFa::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){
    CCLOG("On Touch Began!");
    for (int i=0; i<6; ++i) {
        if (this->getRect(FRAME_TAG+i,FRAME_MENU_TAG).containsPoint(touch->getLocation())){//判断触摸点是否在某个边框的范围内
            if (this->getChildByTag(HERO_MENU_TAG)->getChildByTag(HERO_TAG+i)) {//此边框上有上阵英雄
                CCLOG("有上阵英雄");
                moveTag = i;
                //拖动时处于最上方
                this->getChildByTag(HERO_MENU_TAG)->getChildByTag(HERO_TAG+i)->setZOrder(6);
                return true;

            }
        }
    }
    return false;
}

void ZhenFa::onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event){
    CCLOG("On Touch Moved, movingTag is: %d", moveTag);
    this->getChildByTag(HERO_MENU_TAG)->getChildByTag(HERO_TAG+moveTag)->setPosition(touch->getLocation());
    
}
void ZhenFa::onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event){
    CCLOG("On Touch Ended!");
    auto sp = this->getChildByTag(HERO_MENU_TAG)->getChildByTag(HERO_TAG+moveTag);
    //sp->autorelease();
    char sql[1024];
    for (int i=0; i<6; i++) {
        if (this->getRect(FRAME_TAG+i,FRAME_MENU_TAG).containsPoint(touch->getLocation())){//
            //CCLOG("contact with frame number: %d, whose positionX is: %d",i,(int)this->getChildByTag(200+i)->getPositionX());
            sp->setPosition(this->getChildByTag(FRAME_MENU_TAG)->getChildByTag(FRAME_TAG+i)->getPosition());
            HeroType hero1 = competeTeam[moveTag];
            if(this->getChildByTag(HERO_MENU_TAG)->getChildByTag(HERO_TAG+i) && i!=moveTag){//此边框已有武将，则交换
                this->getChildByTag(HERO_MENU_TAG)->getChildByTag(HERO_TAG+i)->setPosition(this->getChildByTag(FRAME_MENU_TAG)->getChildByTag(FRAME_TAG+moveTag)->getPosition());
                this->getChildByTag(HERO_MENU_TAG)->getChildByTag(HERO_TAG+i)->setTag(HERO_TAG+moveTag);
                
                sprintf(sql,"update User_hero set team_position = %d where username = \"%s\" and hero_index = %d and hero_number = %d",i,user_name.c_str(),userteam[moveTag].index,userteam[moveTag].user_hero_number);
                GameStat::getInstance()->saveToUserInfo("User_hero", documentpath1.c_str(), sql);

                log("交换位置从%d到%d",moveTag,i);
                sprintf(sql,"update User_hero set team_position = %d where username = \"%s\" and hero_index = %d and hero_number = %d",moveTag,user_name.c_str(),userteam[i].index,userteam[i].user_hero_number);
                GameStat::getInstance()->saveToUserInfo("User_hero", documentpath1.c_str(), sql);
                

                
                competeTeam[moveTag] = competeTeam[i];
                competeTeam[moveTag].position = moveTag;
                
                //此位置换到moveTag
            }
            /*else if(this->getChildByTag(HERO_MENU_TAG)->getChildByTag(HERO_TAG+i) && i==moveTag){//结束点还在原来的位置
                sp->setPosition(this->getChildByTag(FRAME_MENU_TAG)->getChildByTag(FRAME_TAG+moveTag)->getPosition());
            }*/
            else if(!(this->getChildByTag(HERO_MENU_TAG)->getChildByTag(HERO_TAG+i))){//如果目标是空位置
                log("交换到空位置从%d到%d",moveTag,i);
                sprintf(sql,"update User_hero set team_position = %d where username = \"%s\" and hero_index = %d and hero_number = %d",i,user_name.c_str(),userteam[moveTag].index,userteam[moveTag].user_hero_number);
                GameStat::getInstance()->saveToUserInfo("User_hero", documentpath1.c_str(), sql);

                competeTeam.erase(competeTeam.find(moveTag));//删除原来的key
            }
            //moveTag换到此位置

            sp->setTag(HERO_TAG+i);
            sp->setZOrder(4);
            //CCLOG("此精灵Z轴位置为%d",this->getChildByTag(HERO_MENU_TAG)->getChildByTag(HERO_TAG+moveTag)->getZOrder());
            hero1.position = i;
            competeTeam[i] = hero1;
            return;
        }
    }
    //表示end点没在任何一个边框内
    sp->setPosition(this->getChildByTag(FRAME_MENU_TAG)->getChildByTag(FRAME_TAG+moveTag)->getPosition());//恢复原来的位置
}
Rect ZhenFa::getRect(int tag, int type){
    auto sp = this->getChildByTag(type)->getChildByTag(tag);
    Size size = sp->getContentSize();
    return Rect(sp->getPositionX()-size.width / 2 ,sp->getPositionY()-size.height / 2, size.width ,size.height);
}
/*void ZhenFa::resetHeroPosition(int tag){
    auto sp = this->getChildByTag(100+tag);
    if(tag>=4){sp->setPosition((200+sp->getContentSize().width+20),(530-(tag-4)*(sp->getContentSize().height+20)));}
    //到任意边框的位置武将停留
    else{sp->setPosition(200,(530-tag*(sp->getContentSize().height+20)));}//到任意边框的位置武将停留
}*/
void ZhenFa::changeHeroFromList(Ref* pSender){
    int newTag = (int)(intmax_t)pSender;
    CCLOG("change hero to index:%d, at position: %d",newTag,_tag);
    string heropicname = heropath(team[newTag].engname);
    char sql[1024];

    if(existsHero){//替换英雄
        CCLOG("替换英雄");
        this->getChildByTag(HERO_MENU_TAG)->removeChildByTag(HERO_TAG+_tag%10);
        //competeTeam.erase(_tag%10);//删掉Compete队伍中对应位置的hero TO DO.....应该直接map[tag]=hero1就行，不管之前有没有
        sprintf(sql,"update User_hero set team_position = 7 where username = \"%s\" and hero_index = %d and hero_number = %d",user_name.c_str(),userteam[_tag%10].index,userteam[_tag%10].user_hero_number);
        GameStat::getInstance()->saveToUserInfo("User_hero", documentpath1.c_str(), sql);

    }
    else{//选取新英雄加入空位置,即existsHero=false
        //TO DO...暂时不需要填写内容
        CCLOG("加入新英雄");
    }
    team[newTag].position = _tag%10;//记录上场位置
    competeTeam[_tag%10] = team[newTag];//map赋值，不管之前是替换还是新加入都可以运行<position,hero>所以不需要再设定position TO DO....
    //TO DO...需要存入database
    sprintf(sql,"update User_hero set team_position = %d where username = \"%s\" and hero_index = %d and hero_number = %d",_tag%10,user_name.c_str(),team[newTag].index,team[newTag].user_hero_number);
    GameStat::getInstance()->saveToUserInfo("User_hero", documentpath1.c_str(), sql);

    CCLOG("加入了新英雄 %s 进入位置: %d",competeTeam[_tag%10].name.c_str(),(_tag%10));
    //TO DO...设定上场位置
    auto sp1 = Sprite::create(heropicname);
    auto heroItem = MenuItemSprite::create(sp1, sp1,NULL);//TO DO....需要换成replaceHero
    heroItem->setTag(_tag%10+HERO_TAG);
    heroMenu->addChild(heroItem,4);
    heroItem->setPosition(this->getChildByTag(FRAME_MENU_TAG)->getChildByTag(_tag%10+FRAME_TAG)->getPosition());
}
void ZhenFa::addHero(Ref* pSender){
    auto sp = (MenuItemSprite*)pSender;//返回的是MenuItemImage不是menu
    _tag = sp->getTag();
    CCLOG("选中边框标签是:%d",_tag);
    selectedFrame = _tag;
    auto _TeamListLayer = new TeamListLayer();
    if (!(this->getChildByTag(HERO_MENU_TAG)->getChildByTag(_tag%10+HERO_TAG))) {//50-55的tag是边框，说明没有上场英雄在这个位置
        CCLOG("此位置为空");
        existsHero = false;
        _TeamListLayer->init(1);
    }
    else{//这个位置有武将  所以是更换武将,heroList里会多出一个"下阵"按钮
        //TO DO.....以后加入下阵按钮
        CCLOG("此位置有上阵英雄");
        existsHero = true;
        _TeamListLayer->init(2);
    }
    if(_TeamListLayer){
    CCLOG("加入选择武将界面");
    this->getParent()->addChild(_TeamListLayer,6,TEAMLIST_LAYER_TAG);
    }
    //this->removeChildByTag(SHIELD_LAYER_TAG);
    auto _shieldLayer = new ShieldLayer();
    _shieldLayer->init(0);
    if(_shieldLayer){
        this->getParent()->addChild(_shieldLayer,5,SHIELD_LAYER_TAG+5);
    }

    CCLOG("现在selectedHeroTag是:%d",selectedHero);
    
}


void ZhenFa::replaceHero(Ref* pSender){
    CCLOG("更换武将");
}
void ZhenFa::back(Ref* pSender){
    CCLOG("remove this layer!");
    auto _HelloWorldLayer = HelloWorldLayer::create();
    if(_HelloWorldLayer){
        //_HelloWorldLayer->setDelegator(_MainLayer);
        this->getParent()->addChild(_HelloWorldLayer,2,HELLOWORLD_LAYER_TAG);
    }

    //TO DO....恢复上一层的touch
    this->getParent()->removeChildByTag(SHIELD_LAYER_TAG);

    this->removeFromParentAndCleanup(true);
}

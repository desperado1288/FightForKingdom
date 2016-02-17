#include "MainLayer.h"
#include "GameStat.h"
#include "LayerManager/HelloWorldLayer.h"
#include "Compete/CompeteScene.h"
USING_NS_CC;
using namespace std;

// on "init" you need to initialize your instance

bool MainLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    return true;
}

void MainLayer::showUserTeam(){
    auto listener = EventListenerTouchOneByOne::create();//加入触摸监听器
    listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(MainLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MainLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MainLayer::onTouchEnded, this);
    //Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, -128);
    
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    
    auto zhenrongbg = Sprite::create("zhenrongbg1.jpg");
    Size visibleSize = Director::getInstance()->getVisibleSize();
    zhenrongbg->setPosition(visibleSize.width/2,visibleSize.height/2);
    this->addChild(zhenrongbg);
    //cocos2d::Vector<Sprite> vecSprite;
    for(int i=0;i<GameStat::getInstance()->userTeam.size();i++){//加武将列表和上阵边框
        string heroPicPath ="touxiang/"+GameStat::getInstance()->userTeam[i].name+".jpg";
        auto heroSprite1 = Sprite::create(heroPicPath.c_str());
        auto frameSprite = Sprite::create("wujiangbiankuang.jpg");
        if(i>=4){//TO DO....把position写成条件判断
            heroSprite1->setPosition((200+heroSprite1->getContentSize().width+20),(530-(i-4)*(heroSprite1->getContentSize().height+20)));
            frameSprite->setPosition((200+400+frameSprite->getContentSize().width+20),(530-(i-4)*(frameSprite->getContentSize().height+20)));
        }
        else{
            heroSprite1->setPosition(200,(530-i*(heroSprite1->getContentSize().height+20)));
            frameSprite->setPosition(200+400,(530-i*(frameSprite->getContentSize().height+20)));
        }
        this->addChild(frameSprite,5,200+i);
        this->addChild(heroSprite1,6,100+i);//TO DO.....z-index，tag：101是英雄列表里第一个，后边逐个+1
        vecHero.push_back(heroSprite1);
        vecFrame.push_back(frameSprite);
    }
    auto startFight = MenuItemImage::create("start.jpg","start.jpg",CC_CALLBACK_1(MainLayer::GoToTeamWorkScene,this));
    startFight->setPosition(900,visibleSize.height/2);
    auto menu = Menu::create(startFight,NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu,8);
    
}
bool MainLayer::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){
    CCLOG("On Touch Began! vector size: %d",(int)vecHero.size());
    for (int i=0; i<vecHero.size(); ++i) {
        if (this->getRect(100+i).containsPoint(touch->getLocation())){//判断触摸点是否在目标的范围内
            moveTag = i;
            return true;
        }
    }
    return false;
}
void MainLayer::onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event){
    CCLOG("On Touch Moved, movingTag is: %d", moveTag);
    this->getChildByTag(moveTag+100)->setPosition(touch->getLocation());
}
void MainLayer::onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event){
    CCLOG("On Touch Ended! vector frame size: %d",(int)vecFrame.size());
    auto sp = this->getChildByTag(moveTag+100);
    for (int i=0; i<vecFrame.size(); i++) {
        if (this->getRect(200+i).containsPoint(touch->getLocation())){//判断触摸点是否在目标的范围内
            CCLOG("contact with frame number: %d, whose positionX is: %d",i,(int)this->getChildByTag(200+i)->getPositionX());
            if(finalTeamIndex[i]<10 && finalTeamIndex[i]>0){//说明这个边框位置有武将了，所以需要交换位置
                resetHeroPosition(finalTeamIndex[i]-1);
            }
            sp->setPosition(this->getChildByTag(200+i)->getPosition());//到任意边框的位置武将停留,tag为对应落点的边框tag+200
            finalTeamIndex[i] = moveTag+1;//记录阵容的位置
            CCLOG("finalTeamIndex is: %d|%d|%d",finalTeamIndex[0],finalTeamIndex[1],finalTeamIndex[2]);
            return;
        }
    }
    //表示end点没在任何一个边框内
    resetHeroPosition(moveTag);
}
Rect MainLayer::getRect(int tag){
    auto sp = this->getChildByTag(tag);
    Size size = sp->getContentSize();
    return Rect(sp->getPositionX()-size.width / 2 ,sp->getPositionY()-size.height / 2, size.width ,size.height);
}
    
void MainLayer::resetHeroPosition(int tag){
    auto sp = this->getChildByTag(100+tag);
    if(tag>=4){sp->setPosition((200+sp->getContentSize().width+20),(530-(tag-4)*(sp->getContentSize().height+20)));}
    //到任意边框的位置武将停留
    else{sp->setPosition(200,(530-tag*(sp->getContentSize().height+20)));}//到任意边框的位置武将停留
}
void MainLayer::GoToTeamWorkScene(Ref* pSender){
    /*CCLOG("清除之前容量：%lu",GameStat::getInstance()->userCompeteTeam.size());
    vector<HeroType>().swap(GameStat::getInstance()->userCompeteTeam);//用swap清除vector空间并释放内存
    vector<HeroType>().swap(GameStat::getInstance()->enemyCompeteTeam);//用swap清除敌方阵容vector
    CCLOG("清除之后容量：%lu",GameStat::getInstance()->userCompeteTeam.size());
    CCLOG("oh yeah!:%lu",(int)array_length(finalTeamIndex));
    for (int i=0; i<array_length(finalTeamIndex);i++){
        //TO DO.....到下一个Scene再处理这个的值
        //finalTeamIndex[i]--;//final现在对应英雄列表里的位置，即对应指定英雄，array的index则对应上场的位置。其中小于0的值为不上武将
        CCLOG("number %d is: %d",i,finalTeamIndex[i]);
        if(finalTeamIndex[i]==0){
            CCLOG("still not complete selecting 8 heros");
            return;
        }

    }
    for (int i=0; i<array_length(finalTeamIndex);i++){
        //TO DO.....加入敌我双方vector序列
        GameStat::getInstance()->userCompeteTeam.push_back(GameStat::getInstance()->userTeam[finalTeamIndex[i]-1]);
        GameStat::getInstance()->enemyCompeteTeam.push_back(GameStat::getInstance()->userTeam[finalTeamIndex[i]-1]);
        GameStat::getInstance()->userCompeteTeam[i].position = i;//最终确定上阵位置
        GameStat::getInstance()->enemyCompeteTeam[i].position = i;//最终确定上阵位置
        CCLOG("%s is in position %d",GameStat::getInstance()->userTeam[finalTeamIndex[i]-1].name.c_str(),i);
    }*/
    CCLOG("Start fighting!");
    auto scene = CompeteScene::create();
    Director::getInstance()->replaceScene(scene);
    
}



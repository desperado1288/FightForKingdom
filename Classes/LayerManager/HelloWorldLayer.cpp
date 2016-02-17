#include "LayerManager/HelloWorldLayer.h"
#include "LayerManager/MainLayer.h"
#include "LayerManager/MainScene.h"
#include "PopNewHero.h"
#include "GameStat.h"

USING_NS_CC;


bool HelloWorldLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorldLayer::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 3);
    
    //
	auto choujiang = Sprite::create("dianjiangtai.PNG");
	auto choujiangMenu = MenuItemSprite::create(choujiang, choujiang, CC_CALLBACK_1(HelloWorldLayer::randomSelectHero, this));
    //链接数据库抽英雄，1.存储到玩家数据库，2.存储到GameStat class
    choujiangMenu->setPosition(visibleSize.width / 4, visibleSize.height / 2);
    //teamMenu->setEnabled(false);
    
	auto menu1 = Menu::create(choujiangMenu, NULL);
    menu1->setPosition(Vec2::ZERO);
	this->addChild(menu1, 2,5);
    
    //初始化玩家信息，从usersql.db放到GameStat类里0代表是直接全部读取usersql
    
    //用于测试vector<HeroType>的使用情况
    /*CCLOG("xuhuang's attack is: %d",GameStat::getInstance()->userTeam[2].attack);
    CCLOG("xuhuang's attack is: %d",GameStat::getInstance()->userTeam[2].defense);
    CCLOG("lvbu's attack is: %d",GameStat::getInstance()->userTeam[6].attack);
    CCLOG("lvbu's attack is: %d",GameStat::getInstance()->userTeam[6].defense);
     */
    //TO DO....
    //GameStat::getInstance()->readFromSQLite("userlist", "/Users/dingyi/Desktop/project1/Game12/Resources/usersql.db", 0);
    //((Menu*)this->getChildByTag(5))->setEnabled(false);//停止上一层的两个
    
    
    return true;
}
//抽奖的方法
void HelloWorldLayer::randomSelectHero(Ref* pSender){
	//TO DO....部分英雄概率不同
    Size visibleSize = Director::getInstance()->getVisibleSize();
	int randomInt = rand() % maxhero + 1;//随机抽取英雄ID 1...max
    CCLOG("here is number: %d",randomInt);
    //string documentpath2 = FileUtils::getInstance()->fullPathForFilename("wholeheros.db");

	//GameStat::getInstance()->readFromSQLite("herolist", "/Users/dingyi/Desktop/project1/Game12/Resources/herosql.db", randomInt);
    GameStat::getInstance()->getRandomHeroFromDB("Heros", documentpath2.c_str());
    //if(GameStat::getInstance()->isNewHero){//true表示有新英雄

    
    if(new_stage == 3){
        HeroType hero1 = team[team.size()-1];
        userteam[2] = hero1;
        //userteam[2] = team[team.size()-1];
        
    }
    log("%s两次概率%f;混乱攻击%f",userteam[2].name.c_str(),team[team.size()-1].DoubleMoveProp,team[team.size()-1].chaosProp);
    GameStat::getInstance()->heroIndex.clear();
    GameStat::getInstance()->heroIndex.push_back(team[team.size()-1].index);
    auto _PopNewHero = new PopNewHero();
    _PopNewHero->init(0);
    if(_PopNewHero){
        this->getParent()->addChild(_PopNewHero,3);
    }
}

void HelloWorldLayer::viewUsersHero(Ref* pSender){//显示武将列表的方法，添加层
    CCLOG("go");
    this->getDelegator()->showUserTeam();
    this->getChildByTag(5)->pause();//停止上一层的两个
}


void HelloWorldLayer::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

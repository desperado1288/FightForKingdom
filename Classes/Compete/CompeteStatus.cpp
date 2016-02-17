#include "Compete/CompeteStatus.h"
#include "LayerManager/HelloWorldLayer.h"
#include "GameStat.h"
USING_NS_CC;
using namespace std;

// on "init" you need to initialize your instance

bool CompeteStatus::init()
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

void CompeteStatus::showFightStatus(float percentage, int round){//delegator from competeLayer来显示回合数和两方血量
    
    CCLOG("show fight status!");
}

#ifndef __COMPETESTATUS_LAYER_H__
#define __COMPETESTATUS_LAYER_H__

#include "cocos2d.h"
#include "GameStat.h"
#include "Compete/CompeteLayer.h"
using namespace std;

class CompeteStatus : public cocos2d::Layer,public FightStatusDisplayDelegator
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void showFightStatus(float percentage, int round);//delegator from competeLayer来显示回合数和两方血量

    CREATE_FUNC(CompeteStatus);
private:
    
};

#endif // __COMPETESTATUS_LAYER_H__

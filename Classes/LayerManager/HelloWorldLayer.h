#ifndef __HELLOWORLD_LAYER_H__
#define __HELLOWORLD_LAYER_H__

#include "cocos2d.h"
const int maxhero = 10;
const int HERO_SPRITE_TAG = 999;
const int HERO_ACTION_TAG = 998;

class TeamDisplayDelegator{
public:
	virtual void showUserTeam() = 0;
};

class HelloWorldLayer : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void randomSelectHero(cocos2d::Ref* pSender);//抽奖
    void viewUsersHero(cocos2d::Ref* pSender);//查看玩家阵容
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorldLayer);
private:
    CC_SYNTHESIZE(TeamDisplayDelegator*, _Delegator, Delegator);
};

#endif // __HELLOWORLD_LAYER_H__

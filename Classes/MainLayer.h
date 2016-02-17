
#ifndef __MAIN_LAYER_H__
#define __MAIN_LAYER_H__

#include "cocos2d.h"
#include "LayerManager/HelloWorldLayer.h"
using namespace std;
#define array_length(array) sizeof(array)/sizeof(array[0])
class MainLayer : public cocos2d::Layer,public TeamDisplayDelegator
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void showUserTeam();
    virtual bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event);
    cocos2d::Rect getRect(int tag);
    void resetHeroPosition(int tag);//重置某个武将在队列中的位置，在移动阵型的过程中,tag为0-max列表中的英雄数量，需要用tag+1来锁定child中的tag
    void GoToTeamWorkScene(cocos2d::Ref* pSender);

    // a selector callback
    // implement the "static create()" method manually
    CREATE_FUNC(MainLayer);
private:
    std::vector<cocos2d::Sprite*> vecHero;
    std::vector<cocos2d::Sprite*> vecFrame;
    int moveTag = 0;
    int finalTeamIndex[8];//因为初始是0，所以为了方便移动位置，最终为1-8而不是0-7，所以确定阵型按钮之后都-1.
    //暂不支持从边框拖回英雄列表！！！
};

#endif // __MAIN_LAYER_H__


#ifndef __ZHUANGBEILIST_LAYER_H__
#define __ZHUANGBEILIST_LAYER_H__

#include "cocos2d.h"
#include "GameStat.h"
#include "LayerManager/MainScene.h"
using namespace std;


class ZhuangbeiListLayer : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    ZhuangbeiListLayer();
    ~ZhuangbeiListLayer();
    virtual bool init(int type);//type就是纯显示武将或者  -1:纯显示 ; 1:武将上阵  2:替换武将
    void showHeroInfo(float dt);
    void showHeroActionMenu(cocos2d::Ref* pSender);
    virtual bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event);
    Rect getRect(int tag);
    void back(cocos2d::Ref* pSender);
    void selectHeroToTeam(Ref* pSender);
    bool isInCompeteTeam(int index);
    void exchangeEquip(cocos2d::Ref* pSender);

    //CREATE_FUNC(ZhuangbeiListLayer);
private:
    int touch_hero_index;
    int display_type;
    bool isResponseTouchEnded;
    
};

#endif // __ZHUANGBEILIST_LAYER_H__

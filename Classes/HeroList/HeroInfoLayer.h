#ifndef __HEROINFO_LAYER_H__
#define __HEROINFO_LAYER_H__

#include "cocos2d.h"
#include "LayerManager/MainScene.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::extension;
using namespace std;

const int HAS_PASS_TAG = 900;
class HeroInfoLayer : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(int type,int index);//type = 1 全英雄;type=2 阵容competeTeam
    void close(cocos2d::Ref* pSender);
    virtual bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event);
    void buttonCallBack(cocos2d::Ref* pSender, Control::EventType event);
    void buttonTouchEnded(cocos2d::Ref* pSender, Control::EventType event);
    void unlock_passskill(Ref* pSender, Control::EventType event);
    void change_unlock_label(Ref* pSender);

    int team_type;
    //CREATE_FUNC(HeroInfoLayer);
private:
};

#endif // __HEROINFO_LAYER_H__

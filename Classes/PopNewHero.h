
#ifndef __POPNEWHERO_LAYER_H__
#define __POPNEWHERO_LAYER_H__

#include "cocos2d.h"
#include "GameStat.h"
#include "LayerManager/MainScene.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::extension;
using namespace std;

class PopNewHero : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(int type);//
    void back(cocos2d::Ref* pSender, Control::EventType event);
    bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event);

    //CREATE_FUNC(PopNewHero);
private:
    int gongchengtype;
};

#endif // __POPNEWHERO_LAYER_H__

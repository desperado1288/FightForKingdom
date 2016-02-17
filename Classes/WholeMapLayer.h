#ifndef __WHOLEMAP_LAYER_H__
#define __WHOLEMAP_LAYER_H__

#include "cocos2d.h"
#include "LayerManager/MainScene.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::extension;
using namespace std;

class WholeMapLayer : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void GoToGongcheng(cocos2d::Ref* pSender);
    void tanchukuang(cocos2d::Ref* pSender);
    void selectStrategy(Ref* pSender, Control::EventType event);


    virtual bool onTouchsBegan(cocos2d::Touch* touch,cocos2d::Event* event);
    virtual void onTouchsMoved(cocos2d::Touch* touch,cocos2d::Event* event);
    virtual void onTouchsEnded(cocos2d::Touch* touch,cocos2d::Event* event);

    CREATE_FUNC(WholeMapLayer);
private:
    int strategy_type;
};

#endif // __WHOLEMAP_LAYER_H__

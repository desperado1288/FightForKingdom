
#ifndef __POPUP_LAYER_H__
#define __POPUP_LAYER_H__

#include "cocos2d.h"
#include "GameStat.h"
#include "LayerManager/MainScene.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::extension;
using namespace std;

class PopupLayer : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(int type);//type就是纯显示武将或者  -1:纯显示 ; 1:武将上阵  2:替换武将
    void back(cocos2d::Ref* pSender, Control::EventType event);
    void GoToQiangduo(Ref* pSender, Control::EventType event);
    bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event);

    //CREATE_FUNC(PopupLayer);
private:
    int gongchengtype;
};

#endif // __POPUP_LAYER_H__

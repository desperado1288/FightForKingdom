#ifndef __BATTLERESULT_LAYER_H__
#define __BATTLERESULT_LAYER_H__

#include "cocos2d.h"
#include "LayerManager/MainScene.h"
#include "cocos-ext.h"
#include "CompeteScene.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::extension;
using namespace std;

const int CCB_BATTLE_RESULT = 900;
const int EXP_TAG = 901;
const int MONEY_TAG = 902;
const int GONGXUN_TAG = 903;
const int GETHERO_TAG = 910;
const int GETHERO_NAME_TAG = 920;
const int MVP_TAG = 930;
const int MVP_NAME_TAG = 931;

class BattleResultLayer : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    BattleResultLayer();
    ~BattleResultLayer();
    virtual bool init(int resultType);//TO DO...1:完胜 2:胜利 3:险胜 4:失败
    void close(cocos2d::Ref* pSender);
    void showStat(cocos2d::Ref* pSender);
    void showStatEnded(cocos2d::Ref* pSender);
    virtual bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event);
    void buttonCallBack(cocos2d::Ref* pSender, Control::EventType event);
    void buttonTouchEnded(cocos2d::Ref* pSender, Control::EventType event);
    //CREATE_FUNC(BattleResultLayer);
private:
    int isBattleStatsDisplayed;//0正在运行动画，1:开始显示统计数据，可以点击快速显示。2:显示完毕
    Node* ccbResult = Node::create();
};

#endif // __BATTLERESULT_LAYER_H__

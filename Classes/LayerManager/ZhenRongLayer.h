
#ifndef __ZHENRONG_LAYER_H__
#define __ZHENRONG_LAYER_H__

#include "cocos2d.h"
#include "GameStat.h"
#include "LayerManager/MainScene.h"
#include "cocos-ext.h"
#include <ui/CocosGUI.h>
USING_NS_CC_EXT;
using namespace std;
using namespace cocos2d::ui;


#define heroheadpath(str) "heroTouxiang/"+str+".png"
#define herocardpath(str) "heroBody/"+str+".png"
const int HERO_HEAD_TAG = 20;
const int HERO_HEAD_BG_TAG = 30;
const int SCROLL_HEAD_TAG = 201;
const int HERO_CARD_TAG = 40;
const int SCROLL_CARD_TAG = 301;

const int ZHUANGBEI_MENU_TAG = 60;
const int ZHUANGBEI_FRAME_TAG = 70;
const int ZHUANGBEI_TAG = 80;

const int CANT_SELECT_TAG = 50;
class ZhenRongLayer : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();//type就是纯显示武将或者  -1:纯显示 ; 1:武将上阵  2:替换武将
    virtual bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event);
    void back(cocos2d::Ref* pSender,Control::EventType event);
    void selectHero(cocos2d::Ref* pSender,Control::EventType event);
    void chooseHero(cocos2d::Ref* pSender,Widget::TouchEventType type);
    void showInfoHero(cocos2d::Ref* pSender,Widget::TouchEventType type);
    void addZhuangbei(Ref* pSender);
    void changeHeroFromList(Ref* pSender);
    void findEquipAndRemove(int type,int position);

    ui::ScrollView* scrollList1;//card
    
    ui::ScrollView* scrollList;//头像

    void choose_hero(Ref* pSender);
    void init_page(float dt);
    bool existsEquip;
    int scrollIndex;
    int _tag;
    int herotag;
    CREATE_FUNC(ZhenRongLayer);
private:
    Point pt;
    vector<int> level_restriction;
};

#endif // __ZHENRONG_LAYER_H__

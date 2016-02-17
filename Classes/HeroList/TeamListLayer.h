
#ifndef __TEAMLIST_LAYER_H__
#define __TEAMLIST_LAYER_H__

#include "cocos2d.h"
#include "GameStat.h"
#include "MainScene.h"

#define team GameStat::getInstance()->userTeam
#define competeTeam GameStat::getInstance()->userCompeteMap
#define enemy_competeTeam GameStat::getInstance()->enemyCompeteMap
#define heropath(str) "heroTouxiang/"+str+".png"
#define herobodypath(str) "heroBody/"+str+".png"
#define weaponpath(str) "weaponCard/"+str+".png"


using namespace std;

const int HERO_SELECT_TAG = 100;
const int HERO_LISTMENU_TAG = 99;
const int SCROLL_LIST_TAG = 98;
class TeamListLayer : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    TeamListLayer();
    ~TeamListLayer();
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
    void alreadyInTeam(cocos2d::Ref* pSender);

    //CREATE_FUNC(TeamListLayer);
private:
    int touch_hero_index;
    int display_type;
    bool isResponseTouchEnded;
    
};

#endif // __TEAMLIST_LAYER_H__


#ifndef __QIXIELIST_LAYER_H__
#define __QIXIELIST_LAYER_H__

#include "cocos2d.h"
#include "GameStat.h"
#include "LayerManager/MainScene.h"
using namespace std;
#define qixieteam GameStat::getInstance()->userqixie
#define qixiecompeteTeam GameStat::getInstance()->userCompeteQixie
#define qixiepath(str) "qixie/"+str+".png"

class QixieListLayer : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    QixieListLayer();
    ~QixieListLayer();
    virtual bool init(int type);//type就是纯显示武将或者  -1:纯显示 ; 1:武将上阵  2:替换武将
    void showHeroInfo(float dt);
    void showHeroActionMenu(cocos2d::Ref* pSender);
    virtual bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event);
    Rect getRect(int tag);
    void back(cocos2d::Ref* pSender);
    void selectHeroToTeam(Ref* pSender);
    bool isInCompeteTeam(std::string name);
    void alreadyInTeam(cocos2d::Ref* pSender);
    
private:
    int touch_hero_index;
    int display_type;
    
};

#endif // __QIXIELIST_LAYER_H__

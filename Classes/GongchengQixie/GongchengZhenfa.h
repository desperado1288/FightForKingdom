#ifndef __GONGCHENGZHENFA_LAYER_H__
#define __GONGCHENGZHENFA_LAYER_H__

#include "cocos2d.h"
#include "LayerManager/MainScene.h"
using namespace std;

class GongchengZhenfa : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    GongchengZhenfa();
    ~GongchengZhenfa();
    virtual bool init();
    virtual bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event);
    Rect getRect(int tag, int type);//type=1是边框，2是武将图片
    void resetHeroPosition(int tag);//重置某个武将在队列中的位置，在移动阵型的过程中,tag为0-max列表中的英雄数量，需要用tag+1来锁定child中的tag
    void addHero(cocos2d::Ref* pSender);
    void replaceHero(cocos2d::Ref* pSender);
    void back(Ref* pSender);
    void changeHeroFromList(Ref* pSender);
    CREATE_FUNC(GongchengZhenfa);
private:
    int moveTag = 0;
    int finalTeamIndex[3];
    int selectedFrame = -1;
    int _tag = -1;//选中的position，即将更换或者增加武将上场  如果选中的是边框：50多  英雄：60多
    bool existsHero = false;//用于判断添加武将的时候是替换还是加入  false是加入
    Menu* heroMenu;
};

#endif // __GONGCHENGZHENFA_LAYER_H__

#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::extension;
using namespace std;

const int USERNAME_TEXTFIELD_TAG = 61;
const int PASSWORD_TEXTFIELD_TAG = 62;
const int TEXTFIELD_TAG = 101;
const int ZHUJIAO_TAG = 110;

class LoginScene : public cocos2d::Scene
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void bottomFunction(cocos2d::Ref* pSender);
    void showZhenXing(cocos2d::Ref* pSender);
    void showTeamList(cocos2d::Ref* pSender);
    void gotoMainScene(Ref* pSender);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode,cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode,cocos2d::Event* event);
    virtual bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event);
    void startGame(Ref* pSender, Control::EventType event);

    void showGongchengTeam(Ref* pSender);
    void selectHero(Ref* pSender);
    void showWenzi(Ref* pSender);

    CREATE_FUNC(LoginScene);
private:
    cocos2d::TextFieldTTF* username;
    cocos2d::TextFieldTTF* password;

};

#endif // __LOGIN_SCENE_H__

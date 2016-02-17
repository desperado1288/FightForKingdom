#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#define team GameStat::getInstance()->userTeam
#define new_stage GameStat::getInstance()->new_player_stage
#define new_step GameStat::getInstance()->new_small_step
#define is_unlock_skill GameStat::getInstance()->isUnlockSkill


#define in_newplayer_mode GameStat::getInstance()->is_in_newplayer_mode
#define enemyteam GameStat::getInstance()->enemyCompeteMap
#define userteam GameStat::getInstance()->userCompeteMap

#define randomhero GameStat::getInstance()->random_hero
#define randomzhuangbei GameStat::getInstance()->random_zhuangbei
#define userzhuangbei GameStat::getInstance()->zhuangbei_list
#define hero_relation GameStat::getInstance()->relations
//用户信息
#define userexp GameStat::getInstance()->exp
#define userexpExt GameStat::getInstance()->expExisted
#define userexpNext GameStat::getInstance()->expNextLevel
#define battle_money GameStat::getInstance()->money
#define battle_shengwang GameStat::getInstance()->shengwang
#define user_money GameStat::getInstance()->user_yinliang
#define user_shengwang GameStat::getInstance()->user_reputation
#define user_jinzi GameStat::getInstance()->user_yuanBao

#define user_level GameStat::getInstance()->userLevel
#define user_name GameStat::getInstance()->userName

//#define documentpath1 FileUtils::getInstance()->fullPathForFilename("UserInfo.db")
#define documentpath2 FileUtils::getInstance()->fullPathForFilename("wholeheros.db")
//#define documentpath1 "/Users/dingyi/Desktop/project1/Game12/Resources/UserInfo.db"
//#define documentpath2 "/Users/dingyi/Desktop/project1/Game12/Resources/wholeheros.db"
const std::string writablepath = cocos2d::FileUtils::getInstance()->getWritablePath();
const std::string documentpath1 = writablepath+"UserInfo.db";


//const std::string documentpath2 = "/Users/dingyi/Desktop/project1/Game12/Resources/wholeheros.db";
//const std::string documentpath1 = "/Users/dingyi/Desktop/project1/Game12/Resources/UserInfo.db";

const int SHIELD_LAYER_TAG = 790;
const int HELLOWORLD_LAYER_TAG = 799;
const int MAIN_LAYER_TAG = 800;
const int TEAMLIST_LAYER_TAG = 801;
const int ZHENGZHAN_LAYER_TAG = 802;
const int ACTIVITY_LAYER_TAG = 803;
const int SHOP_LAYER_TAG = 804;
const int ZHENFA_LAYER_TAG = 805;
const int ZHENRONG_LAYER_TAG = 806;
const int QIXIELIST_LAYER_TAG =807;
const int GONGCHENGZHENFA_LAYER_TAG = 808;
const int HEROINFO_LAYER_TAG = 810;
const int MAIN_MENU_TAG = 700;

const int NEWPLAYER_LAYER_TAG = 900;

const int USERLEVEL_TAG = 20;
const int USERNAME_TAG = 21;
const int USERMONEY_TAG = 22;
const int USERREPUTATION_TAG = 23;



class MainScene : public cocos2d::Scene
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void bottomFunction(cocos2d::Ref* pSender);
    void showZhenXing(cocos2d::Ref* pSender);
    void showTeamList(cocos2d::Ref* pSender);
    void skill_hurt(Ref* pSender);
    void createNewPlayerLayer(int type);
    void showGongchengTeam(Ref* pSender);
    void receive_notice_from_battle(Ref* pSender);//战斗结束之后继续新手指导

    cocos2d::Menu* menu1;
    // a selector callback
    //开始新手指导
    void newPlayerStart(int type);
    void resetUserInfo();
    CREATE_FUNC(MainScene);
};

#endif // __MAIN_SCENE_H__

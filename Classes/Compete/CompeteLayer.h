#ifndef __COMPETE_LAYER_H__
#define __COMPETE_LAYER_H__

#include "cocos2d.h"
#include "CompeteRole.h"
#include "QixieRole.h"
#include "GameStat.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::extension;

using namespace std;
#define array_length(array) sizeof(array)/sizeof(array[0])
#define attack_perp(defense,level) (float)defense/((float)defense+60*level+400)
#define critic_prob(critic,level) (float)critic/((float)critic+60*level+1000)

//敏捷双击写在cpp里的方程

//DEFINE 很长的单例总数据class的玩家和敌人上场vector
#define enemyTeam GameStat::getInstance()->enemyCompeteTeam
#define enemyqixieTeam GameStat::getInstance()->enemyCompeteQixie
#define unlock_hero GameStat::getInstance()->passive_unlock_battle_hero
#define mvp_hero GameStat::getInstance()->mvphero
#define equip_index GameStat::getInstance()->equipIndex



#define randomFloat do{                         \\\
    srand((unsigned)time(NULL));                \\\
    for(int i=0;i<4;i++)                        \\\
        randomFloat = rand()/double(RAND_MAX);  \\\
}while(0)

#define PAIR pair<int,HeroType>

const int sizeX = 186*0.69f;//为武将图片的宽和高
const int sizeY = 234*0.69f;

const int SHUOMING_XIAHUAXIAN = 175;
const int SHUOMING_WORD = 176;

const int DAMAGEFONT_TAG = 1000;
const int DAMAGEFONT_DOUBLE_TAG = 1500;
const int DAMAGE_CRITIC_TAG = 2000;
const int DAMAGE_DOUBLE_TAG = 2500;
const int DAMAGE_BLOCK_TAG = 3000;
const int DAMAGE_BLOCK_TAG1 = 3500;

class FightStatusDisplayDelegator{
public:
	virtual void showFightStatus(float percentage, int round) = 0;
};

class CompeteLayer : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    static bool SortByAgility(const PAIR &h1, const PAIR &h2);//按敏捷排序
    void beginFight();
    void beginDialog();
    void beginCompete(float dt);
    void beginAnimation(float dt);
    virtual bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event);

    void initUsersRole();
    vector<int> findAttackPosition(int beginPosition,int attackType,bool isEnemy);
    pair<vector<int>,vector<int>> findAttackPenetration(int beginPosition,int attackType, bool isEnemy);
    int findNextPosition(const int startingPosition,int beginPosition,bool isEnemy);

    void damage(int index,bool inEnemy);
    void computeAllDamage(int target,bool isEnemy);
    void reArrange();
    void showGameResult(bool isUserWin);
    void update(float dt);
    void die(cocos2d::Ref* pSender);
    int runActionTag;
    float doubleAttackProb(float agl1, float agl2);
    int blockProb(float agl1, float agl2);
    float criticDamage(float critic, int level, float damage);
    void playFight(float dt);
    void buttonCallBack(Ref* pSender, Control::EventType event);
    void buttonTouchEnded(Ref* pSender, Control::EventType event);

    void qixieAttackHeros();
    void qixieDamage(int index,bool isEnemy);
    void playGongcheng(float dt);
    
    
    void dialog(float dt);
    void dialog1(Ref* pSender);
    void heroMove(Ref* pSender);

    void juqing_attack_move(Ref* pSender);
    void roleRecover(Ref* pSender);

    void resumeActions(float dt);
    void resumeTargetByTag(int tag);

    void unlock_cal_time(HeroType hero,int type);
    void endJiazai();
    CREATE_FUNC(CompeteLayer);
    
private:
    vector<Vec2> userposition,enemyposition;
    vector<Vec2> userposition_withqixie,enemyposition_withqixie,user_qixieposition,enemy_qixieposition;

    CC_SYNTHESIZE(FightStatusDisplayDelegator*, _Delegator, Delegator);
    map<int,CompeteRole*> userRole;//TO DO....为什么不能用<CompeteRole*>
    map<int,CompeteRole*> enemyRole;
    vector<QixieRole*> userQixieRole;
    vector<QixieRole*> enemyQixieRole;
    bool qixieIsEnemy;
    
    vector<PAIR> userVec;//用于从map赋值排序
    vector<PAIR> enemyVec;
    int round;
    map<int, HeroType> userMap;
    map<int, HeroType> enemyMap;
    map<int, GongChengType> userqixieMap;
    map<int, GongChengType> enemyqixieMap;

    bool userWin;//玩家是否胜利
    float beginTime;
    float dialogbeginTime;
    float delayTime;
    float totalDelayTime;
    float updateTime;
    float fightTime;
    int userIndex;
    int enemyIndex;
    bool usersTurn;
    bool isDoubleAttack;
    bool isUseSkill;
    bool isCritic;
    
//记录伤害值，动画播放
    vector<pair<int,vector<int>>> attack_list;
    vector<pair<int,vector<int>>> passive_skill;
    vector<vector<float>> damage_list;
    vector<vector<int>> critic_list;
    vector<vector<int>> block_list;
    vector<int> double_list;
    vector<int> is_skill;
    vector<int> is_enemy;
    vector<vector<float>> damage_delaytime_list;//每次受到伤害的延迟时间
    vector<float> delay_time;
    int animation_index;
    float end_delay_time;
    vector<int> roundAnimationIndex;
    
    vector<pair<int,vector<int>>> qixieattack_list;
    vector<vector<int>> qixiedamage_list;
    float qixietime;
    vector<float> qixiedelay_time;//每回合一共
    vector<int> qixieis_enemy;
    int qixienum1;
    vector<int> qixienumber;
    bool isgongchengzhan;
    float qixiebeginTime;
    
//------------剧情播放:文字+英雄移动
    int juqing_move;
    int dialog_index;
    vector<string> dialog_wenzi;
    vector<int> dialog_direction;//1是right 2是left
    
    int heromove_index;
    vector<vector<int>> heromove_isuser;
    vector<vector<int>> heromove_position;
    
    vector<int> heromove_dialogindex;
//--------------
    float normal_dlytime;
    float double_dlytime;
    
    //解锁
    int unlock_hit_time;
};

#endif // __COMPETE_LAYER_H__

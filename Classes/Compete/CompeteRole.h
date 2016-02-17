#ifndef __COMPETEROLE_H__
#define __COMPETEROLE_H__
#include "GameStat.h"
#include "cocos2d.h"

class CompeteLayer;

const int healthBar_Tag = 120;
const int healthbarbg_tag = 121;
const int NAME_TAG = 123;
const int HERO_FRAME_TAG = 15;

const int SKILL_BG_TAG = 128;
const int SKILL_LABEL_TAG = 129;


class CompeteRole : public cocos2d::Sprite
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    bool init(HeroType hero,CompeteLayer* layer);
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    void healthBarLogic(float dt);
    virtual ~CompeteRole();
    void setHp();
    void whetherDead();
    void attack(bool isSkill, int tag1, vector<int> tag2, bool isDoubleAttack, bool isEnemy,float time);//发动攻击动作
    void attackDamage(vector<float> damage, bool isDouble, vector<int> isCritic,vector<int> isblock, float time,int pass_type);
    void costDamage(float damage, bool isDouble, bool isCritic,float time);
    void changeHealth(cocos2d::Ref* pSender);
    void addFont(cocos2d::Ref* pSender);
    void showCritic(cocos2d::Ref* pSender);
    void hurtEffect(Ref* pSender);
    void faSkill(Ref* pSender);

    void dieOut();//英雄死亡动画
    void recover();//英雄恢复初始状态
    // a selector callback
    //CREATE_FUNC(CompeteRole); 
public:
    CompeteLayer* player;
    HeroType mhero;
    float changeHp;
    float delayTime;
    std::string resName;
    int level;
    int chaId;
    int enemy;//1玩家2敌人
    //CCArray *attackArray;
    float ap;
    int target,attackType,changeType,special;
    float prob;
    float parameter1,parameter2;
    ProgressTimer* healthBar;
    int posX;
    int position;
    int resNum;
    float curHP;
    float totalHp;
    int dir;
    int attackcount;
    cocos2d::Vec2 pos;
    vector<float> getDamage;
    bool attackDouble;
    int attackDoubleIndex;
    int attackCriticIndex;
    vector<int> attackCritic;
    vector<int> attackBlock;
    //TO DO....5种增益和减益是否已有
    bool isAttackUp;
    bool isDefenseUp;
    bool isCriticUp;
    bool isAgilityUp;
    bool isKanpoUp;
    bool isAttackDown;
    bool isDefenseDown;
    bool isCriticDown;
    bool isAgilityDown;
    bool isKanpoDown;
    bool isDeepDamage;//是否伤害加深
    bool isChaos;//是否混乱
    bool isDizzy;//是否眩晕
    int tag;
//被动技能
    int is_passive_attack;
    //被动技能解锁战，解锁指标。
    int pass_hit_time;//包括格挡，双击等
    vector<pair<int,float>> special_type;
    void remove();
private:
    string act_skill_name;
    string pas_skill_name;
    vector<int> target_tag;
};

#endif // __COMPETEROLE_H__

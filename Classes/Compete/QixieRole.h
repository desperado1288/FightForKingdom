#ifndef __QIXIEROLE_H__
#define __QIXIEROLE_H__
#include "GameStat.h"
#include "cocos2d.h"

class CompeteLayer;

//const int healthBar_Tag = 120;
//const int healthbarbg_tag = 121;
//const int HERO_FRAME_TAG = 15;
const int BULLET_TAG = 11;
class QixieRole : public cocos2d::Sprite
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    bool init(GongChengType hero,CompeteLayer* layer);
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    void healthBarLogic(float dt);
    virtual ~QixieRole();
    void setHp();
    void whetherDead();
    void attack(bool isSkill, int tag1, vector<int> tag2, bool isDoubleAttack, bool isEnemy,float time);//发动攻击动作
    void attackDamage(vector<float> damage, bool isDouble, vector<int> isCritic, float time);
    void costDamage(float damage, bool isDouble, bool isCritic,float time);
    void changeHealth(cocos2d::Ref* pSender);
    void addFont(cocos2d::Ref* pSender);
    void hurtEffect(cocos2d::Ref* pSender);

    void showCritic(cocos2d::Ref* pSender);
    void dieOut();//英雄死亡动画
    void recover();//英雄恢复初始状态
    // a selector callback
    //CREATE_FUNC(CompeteRole); 
public:
    CompeteLayer* player;
    GongChengType mhero;
    float changeHp;
    float delayTime;
    std::string resName;
    int target,attackType,changeType,special;
    ProgressTimer* healthBar;
    int posX;
    int position;
    int resNum;
    float curHP;
    float totalHp;
    int dir;
    int bullettype;
    int attackcount;
    cocos2d::Vec2 pos;
    vector<float> getDamage;
    bool attackDouble;
    int attackDoubleIndex;
    int attackCriticIndex;
    vector<int> attackCritic;
    //TO DO....5种增益和减益是否已有
    
    
    
    void remove();
private:
    vector<int> target_tag;
};

#endif // __QIXIEOLE_H__

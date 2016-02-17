#ifndef __GAME_STAT_H__
#define __GAME_STAT_H__

#include "sqlite3.h"
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

typedef struct{
    int type;
    float value;//增益数值
    bool increase;
    int startRound;//buff开始生效的回合数
    float duration;//剩余持续时间
    int endRound;
}Buff;
//TO DO...装备列表
typedef struct{
    string name;//装备名称
    string engname;
    int type;//种类：武器，防具，马等
    int index;//在总数据库里的序列号
    int position;//装备栏的位置:0-5,没被装备上=-1
    int level;
    int evolution_stage;
    int number_index;
    int special_index;
    int special_type;//武器特殊属性序列号
    //int specialTypeByRelation;//缘分武将特殊属性
    float special_value;//特殊属性加成,比如暴击、双击+20%
    float special_value_relation;//缘分武将特殊加成
    
    int property;//属性，根据装备种类是不同属性
    int propertyIns;
    int propertyDisplay;
    
    string info;//装备描述信息
    string specialInfo;//特效描述文字
    string special_relate_info;
    vector<string> relation_name;
    vector<int> relationHeroIndex;//缘分英雄序号
    int quality;
    
}ZhuangBeiType;

typedef struct {//缘分
    int relation_index;//1-6
    string relation_name;
    string relationDescribe;
    int type;//对应武将=1,对应装备=2

    vector<int> indexHero;//缘分描述对应的缘分英雄在总database里的对应序号index
    int extra_type;
    float extra_value;
    
    int zhuangbei_type;//武器，防具等等
    int indexZhuangbei;//缘分描述对应的缘分装备在总database里的对应序号index
    
    bool isAchieved;//前端数值
}RelationSeries;

typedef struct
{
    //int type;//类型
    std::string name;//武将姓名
    string engname;
    int kingdom;//国家
    int quality;//品质
    int heroType;//1是武将，0是谋士
    int index;//总数据库对应的序号
    int user_hero_number;//对应用户此英雄的第几个
    int evolution_stage;
    //string exname;
    string heroInfo;//武将生涯描述
    string active_skill_name;//主动技能名字
    string active_skill_describe;//主动技能描述
    string active_skill_animation_name;//主动技能动画文件名
    float active_skill_duration;
    
    string passive_skill_name;//被动技能名字
    string passive_skill_describe;//被动技能描述
    int isPassiveActivated;//是否已激活

    //int activeSkillID;//主动技能编号
    //int passiveSkillID;//被动技能编号
    float prob;//主动技能释放概率
    float parameter1;//主动技能伤害：攻击系数
    float parameter2;//主动技能伤害：敏捷/暴击系数
    int target;//主动技能攻击的目标单位编号
    int attackType;//主动技能伴随伤害：敏捷/暴击/..编号
    int changeType;//增益减益属性编号
    float change_value;
    float active_special;
    int special;//特殊技能编号
    vector<pair<int,float>> special_value;//按照special_index序列，pair是special_type和special_value
    
    int characterType;//主角类型（初始选择）：1是游侠，2是。。，3是鬼才
    int position;//上场位置
    //TO DO...有可能要加阵容列表位置信息
    float attack;//攻击，直接从玩家数据库或者抽奖时从总数据库跟随着当前等级相关，不是初始攻击
    float attackDisplay;//面板攻击，在attack的基础上随装备和缘分加成改变
    float attackBattle;//战斗时攻击,在面板的基础上随场上技能之类的变化
    //TO DO....有可能把攻击、血成长也加进来
    float health;//面板血量
    float healthDisplay;//总的血量
    float healthBattle;//总的攻击
    float defense;
    float defenseDisplay;
    float defenseBattle;//
    //TO DO.....魔防以后加
    float critic;
    float criticDisplay;
    float criticBattle;//
    float agility;
    float agilityDisplay;
    float agilityBattle;//
    float antiCritic;//韧性，防爆
    int statTotal;//总战斗力，根据所有面板属性，方便队列排序
    //成长
    int attackins,defenseins,healthins,agilityins,criticins;
    int battle_kill;
    //TO DO....4种增益和减益是否已有
    vector<Buff> heroBuff;
    //TO DO....装备提供的敏捷，暴击等属性
    float criticByEquip = 0;
    //--------------------------------
    int level;//等级
    int exlevel;//品质
    int xp;//经验
    int swordid;//武器id;
    int baowuid;//宝贝;
    int transformLevel;//转生次数
    vector<RelationSeries> relation;//6条缘分
    vector<ZhuangBeiType> zhuangbei;
    //TO DO....装备增加的特效,同时也是被动技能特效
    //1
    float addCriticProp=0;
    //2
    float addDoubleAttackProp=0;
    //3
    float penetrateProp=0;
    float bounceProp;//4
    //5
    float addDamageProp=0;
    //6
    float ignoreDefProp=0;
    //7周围联动
    float companyAttackProp=0;//联动
    //8
    float DoubleMoveProp=0;//两次
    int just_used_doublemove = 0;
    //9
    float attackFirstProp=0;//先手
    //10
    float fenZhanProp=0;//奋战攻击类型1是普通，2是吕布的
    //11
    float poisonProp=0;
    //12
    float addSkillDamgeProp=0;
    //13
    float dizzyProp=0;
    //14
    float chaosProp=0;
    //15
    float addSkillProp=0;
    //16
    float ignoreblock=0;
    float absorbHealth=0;//17
    float restoreHealthfromLast=0;//18
    float soloImprovement=0;//19一夫当关
    //float extraDamage=0;//附带敏捷=1、暴击=2伤害
    //---------------------
    float decPhyDamage=0;//22
    float decMagDamage=0;//23
    float ignoreDebuff=0;//24
    //---------------------
    //31
    float addAgility=0;
    //32
    float addCritic=0;
    float addAttack=0;//33
    float addDefense=0;//34
    float addBlock=0;
    float addHealth=0;//36
    
}HeroType;
typedef struct{
    int type;
    string engname;
    int attack;
    int attack_type;
    int durable;//耐久,降为0就会被摧毁
    int bullet_type;
    int position;
    
}GongChengType;



class GameStat : public cocos2d::Object
{
	//TO DO........用于以后input玩家的账号密码
	//CC_SYNTHESIZE_PASS_BY_REF(string, _UserID, UserID);
	
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    
	/*const char* HERO_STAT_DATABASE = "herosql.db";
     const char* HERO_STAT_TABLE = "herolist";
     const char* USER_STAT_DATABASE = "usersql.db";
     const char* USER_STAT_TABLE = "userlist";
     */
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    bool init();
	GameStat();
	~GameStat();
    
    //TO DO.....需要有一个只显示玩家获得过的英雄及其被动是否激活的database
    vector<GongChengType> userqixie;
    std::map<int,GongChengType> userCompeteQixie;
    map<int,GongChengType> enemyCompeteQixie;
    vector<HeroType> userTeam;//玩家所有英雄信息，可通过总战斗力排序或用品质排序
    vector<HeroType> allHeros;//
    //vector<HeroType> userCompeteTeam;//确定by MainLayer.cpp,英雄场上位置和在vector中的顺序一致，1-8
    std::map<int,HeroType> userCompeteMap;
    std::map<int,HeroType> enemyCompeteMap;

    vector<HeroType> enemyCompeteTeam;
    bool isNewHero;//是否抽到新英雄.
    int indexOfTable;//用于存储readFromSQLite方法的index，便于load方法调取.0是读usersql
    // implement the "static create()" method manually
	static GameStat* getInstance();
	// a selector callback
    void getTotalHeroIndex(const char* table, const char* dbFilename,const char* sql);
    void getRandomHeroFromDB(const char* table, const char* dbFilename);
    void saveNewHeroToUser(const char* table, const char* dbFilename);
    void readFromUserHeros(const char* table, const char* dbFilename,int index);
    void loadUserHerosbyIndex(const char* table, const char* dbFilename,const char* sql);

	sqlite3* prepareTableInDB(const char* table, const char* dbFilename);
	void saveToSQLite(const char* table, const char* dbFilename,const char* sql);
	void readFromSQLite(const char* table, const char* dbFilename,int index);
    void readFromSQLiteWholesql(const char* table, const char* dbFilename,int index);
    int readFromUserInfo(const char* table, const char* dbFilename);
    void saveToUserInfo(const char* table, const char *dbFilename,const char* sql);
    void readFromRelation(const char* table, const char *dbFilename,const int relation_index);
    void getEquipInfo(const char* table, const char* dbFilename,const char* sql);
    void readfromEquipInfo(const char* table, const char* dbFilename,const char* sql);

    
    void selectEnemyTeam(const char* table, const char* dbFilename,const char* sql);

	bool searchInTeamName(int index,string str);
    int finalEnemyTeamIndex[8];
    //TO DO....显示战斗结束统计数据
    string mvpName;
    int exp;
    int money;
    int shengwang;
    vector<std::string> heroName;
    vector<int> heroIndex;
    string mvp_engname;
    string mvp_name;
    HeroType mvphero;
    int mvp_quality;
    vector<pair<int,int>> equipIndex;
    //
	//int indexInsert();
    //TO DO...需要从database初始化的内容
//用户信息-----------
    string userName;
    int new_player_stage;//新手指导阶段
    int new_small_step;//每个阶段细分的步骤
    int userType;//主角类型,1,2,3:战神,游侠,鬼才
    int userLevel=0;
    int expExisted;
    int expNextLevel;
    int user_yinliang;
    int user_reputation;
    int user_yuanBao;
    int tiliRemaining;//体力
    int tili;
    int junlingRemaining;//军令
    int junling;
    int vip;
    int competeSpeed;//战斗速度倍率
//-------------
    
    
    int random_index;
    HeroType random_hero;
    ZhuangBeiType random_zhuangbei;
    vector<ZhuangBeiType> zhuangbei_list;
    
    vector<RelationSeries> relations;
    int user_hero_number;
    vector<int> totaldb_index;
    
    int show_hero_info;
    int show_hero_info_in_competeTeam;
    
    int isUnlockSkill;
    bool isGongcheng;
    int is_in_newplayer_mode;
    HeroType passive_unlock_battle_hero;
    int unlock_complete;
    //string username;
	CREATE_FUNC(GameStat);
private:
	static GameStat* sharedGameStat;
};

#endif // __GAME_STAT_H___



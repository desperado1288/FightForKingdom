#include "GameStat.h"
#include "LayerManager/MainScene.h"

USING_NS_CC;
using namespace std;

// on "init" you need to initialize your instance
GameStat::GameStat(){
	//prepareTableInDB("UserRecord", "sql.db");
    
}
GameStat::~GameStat(){}
bool GameStat::init(){
    this->isNewHero = false;
    CCLOG("In new UserRecord()");
    return true;
}
GameStat* GameStat::sharedGameStat = nullptr;

GameStat* GameStat::getInstance(){
	//sharedRecord = new UserRecord();
    
	if (sharedGameStat == NULL){
		sharedGameStat = new GameStat();
		sharedGameStat->init();
	}
	return sharedGameStat;
}
sqlite3* GameStat::prepareTableInDB(const char* table, const char* dbFilename) {
	sqlite3 *pDB = NULL;
	char *errorMsg = NULL;
	if (SQLITE_OK != sqlite3_open(dbFilename, &pDB)) {
		CCLOG("Open the Game Data Table falied\n");
		return NULL;
	}
	//创建玩家数据库
	//TO DO...... 加上exp smallint,expnow smallint,explevelup smallint
    string str1 = "Users_Info";
    string str2 = "User_equip";
    string str3 = "User_hero";
    string sql;
    if(table==str1){
        sql = "create table if not exists " + std::string(table) + "(username varchar(50),userlevel int,new_stage int,exp_already int,exp_levelup int,money int,reputation int,other_resources int,yuanbao int,vip_number int,tili_upper int,tili_now int,xingdongli_upper int,xingdongli_now int,primary key(username))";
        CCLOG("正在新建table %s！",table);
    }
    else if(table==str2){
        sql = "create table if not exists " + string(table) + "(username varchar(50),zhuangbei_index int,number_index int,level int,equip_position int,evolution_stage int,position int,primary key(username,zhuangbei_index,equip_position,number_index))";
        CCLOG("正在新建table %s！",table);

    }
    else if(table==str3){
        sql = "create table if not exists " + std::string(table) + "(username varchar(50),hero_index int,hero_number int,/*数量序号*/hero_level int,evolution_stage int,team_position int,has_passive_skill int,primary key(username,hero_index,hero_number))";
        CCLOG("正在新建table %s！",table);
    }
	sqlite3_exec(pDB, sql.c_str(), NULL, NULL, &errorMsg); // create a table;
    
	if (errorMsg != NULL) {
		CCLOG("In PrepareDBTable exec sql %s fail with msg: %s", sql.c_str(), errorMsg);
		sqlite3_close(pDB);
		return NULL;
	}
    
	return pDB;
}
//没有此武将return false有-return true
bool GameStat::searchInTeamName(int index, string str){
	/*for (int i; i < this->userTeam.size(); i++){
		if (this->userTeam[i].name == str)
			return false;
	}*/
    for (int i; i < this->allHeros.size(); i++){
        if (this->allHeros[i].index == index)
            return false;
    }
	return true;
}

int loadUserRecord(void* para, int n_column, char** column_value, char **column_name) {
	GameStat* record = (GameStat*)para;
	string sql1 = "\"";
	//赋值class里的userTeam and teamName;
	if (GameStat::getInstance()->searchInTeamName((int)(intptr_t)column_value[1], column_value[0])){//判断已有没有此将//TO DO...可能有错
        GameStat::getInstance()->isNewHero=true;//设置为true: 抽到新英雄
        HeroType hero1;
        hero1.name = column_value[0];//赋值武将名字

        CCLOG("Hero name is: %s",hero1.name.c_str());
		sql1 += column_value[0];
		sql1 += "\"";
        if(GameStat::getInstance()->indexOfTable==0){}
        //hero1.index= (int)column_value[1];//遍历前18个储存点
        float abc;
        sscanf(column_value[1], "%f", &abc);
        //GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].attack = (int)abc;
        //TO DO....两种方法不知道哪个更快
        hero1.attack = abc;
        hero1.attackDisplay = abc;
        CCLOG("attack is :%f",hero1.attack);
        sscanf(column_value[2], "%f", &abc);
        //GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].defense = (int)abc;
        hero1.defense = abc;
        hero1.defenseDisplay = abc;
        sscanf(column_value[3], "%f", &abc);
        //GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].health = (int)abc;
        hero1.health = abc;
        hero1.healthDisplay = abc;
        sscanf(column_value[4], "%f", &abc);
        hero1.agility = abc;
        hero1.agilityDisplay = abc;
        sscanf(column_value[5], "%f", &abc);

        hero1.critic = abc;
        hero1.criticDisplay = abc;
        sscanf(column_value[11], "%f", &abc);

        hero1.antiCritic = abc;
        sscanf(column_value[12], "%f", &abc);

        hero1.target = (int)abc;
        
        sscanf(column_value[13], "%f", &abc);
        hero1.prob = abc;
        sscanf(column_value[14], "%f", &abc);
        hero1.parameter1 = abc;
        sscanf(column_value[15], "%f", &abc);
        hero1.attackType = (int)abc;
        sscanf(column_value[16], "%f", &abc);
        hero1.parameter2 = abc;
        sscanf(column_value[17], "%f", &abc);
        hero1.changeType = (int)abc;
        sscanf(column_value[18], "%f", &abc);
        hero1.special = (int)abc;
        hero1.level = 1;//TO DO....加入英雄等级
        GameStat::getInstance()->userTeam.push_back(hero1);

        CCLOG("adding new entry %s : %f,capacity is %lu",column_name[14],GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].parameter1,GameStat::getInstance()->userTeam.size());
        CCLOG("adding new entry %s : %d",column_name[15],GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].attackType);
        CCLOG("adding new entry %s : %f",column_name[16],GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].parameter2);
        CCLOG("adding new entry %s : %d",column_name[17],GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].changeType);

        //TO DO...以后加入攻击成长等
        //hero1.attackins = (int)column_value[7];

		//TO DO....等else写完放到return 0 前边
        /*if(GameStat::getInstance()->indexOfTable!=0){
            for (int i; i < n_column; i++){
                sql1 += ",";
                sql1 += column_value[i];//用insert * 语句准备加入sqlite database
            }
            GameStat::getInstance()->saveToSQLite("userlist", "/Users/dingyi/Desktop/project1/Game12/Resources/usersql.db", sql1.c_str());
        }
        CCLOG("空存储位置刚刚+1！");
        */
	}
	else{//没有此武将
		//TO DO....转换成魂
	}
	return 0;
    
}
int getHeroIndice(void* para, int n_column, char** column_value, char **column_name) {
    float abc;
    sscanf(column_value[0], "%f", &abc);

    for(int i=0;i<GameStat::getInstance()->totaldb_index.size();i++){
        
        if(GameStat::getInstance()->totaldb_index[i]==(int)abc)
            return 0;
    }
    GameStat::getInstance()->totaldb_index.push_back((int)abc);
    return 0;
}

void GameStat::getRandomHeroFromDB(const char* table, const char* dbFilename){
    char sql[1024];
    sqlite3* pDB = NULL;
    CCLOG("Now I\'m going to read index in original database");
    //if (pDB != NULL) {
    //	int count = 0;
    char *errorMsg;
    sprintf(sql, "select distinct number from %s", table);//从总database里select出刚抽的武将,"DISTINCT"
    
    if(sqlite3_open(dbFilename,&pDB)==SQLITE_OK){
        CCLOG("read succss");
        totaldb_index.clear();//TO DO...清空总数据库index
        sqlite3_exec(pDB, sql, getHeroIndice, this, &errorMsg);
        if (errorMsg != NULL) {
            CCLOG("exec sql %s fail with msg: %s", sql, errorMsg);
            sqlite3_close(pDB);
            return;
        }
    }else{
        CCLOG("read is failure");
        sqlite3_close(pDB);
    }
    
    //
    /*vector<int> heroIndice;
    for(int i=0;i<n_column;i++){
        float abc;
        sscanf(column_value[i], "%f", &abc);
        if (((int)abc)==heroIndice[heroIndice.size()-1]) {
            continue;//防止重复index录入
        }
        heroIndice.push_back((int)abc);
    }*/
    int random_ind = arc4random()%totaldb_index.size();
    CCLOG("随机出的卡牌是第:%d个",random_ind);
    char sql1[1024];
    //totaldb_index[random_ind] = 81;
    sprintf(sql1,"select * from Heros where number = %d",totaldb_index[random_ind]);
    random_index = totaldb_index[random_ind];
    if(searchInTeamName(totaldb_index[random_ind], "abc")){//判断已有没有此将//TO DO...可能有错
        isNewHero=true;//设置为true: 抽到新英雄
    }
    else
        isNewHero=false;//设置为true: 抽到新英雄
    //string documentpath1 = FileUtils::getInstance()->fullPathForFilename("UserInfo.db");
    //string documentpath2 = FileUtils::getInstance()->fullPathForFilename("wholeheros.db");

    GameStat::getInstance()->isNewHero = GameStat::getInstance()->searchInTeamName(totaldb_index[random_ind], "abc");
    GameStat::getInstance()->readFromRelation("Relation", documentpath2.c_str(),totaldb_index[random_ind]);//读取天命
    
    HeroType hero1;
    int number = 1;
    if(isNewHero){
        hero1.relation = relations;
        hero1.position = 7;
        hero1.level = userLevel;
        hero1.user_hero_number = 1;
        hero1.index = totaldb_index[random_ind];
        hero1.evolution_stage = 0;
        hero1.isPassiveActivated = 0;//TO DO.....需要修改
        if(new_player_stage==3){
            hero1.position = 2;
        }
        userTeam.push_back(hero1);
        log("两次概率%f;混乱攻击%f",hero1.DoubleMoveProp,hero1.chaosProp);
        GameStat::getInstance()->loadUserHerosbyIndex("Heros", documentpath2.c_str(),sql1);
    }
    else{//有此英雄，增加number,存到user_hero中
        for(int i = 0; i<userTeam.size();i++){
            if (userTeam[i].index==totaldb_index[random_ind]) {
                if(number==1)
                    random_hero = userTeam[i];//TO DO...有可能多次赋值
                number++;
            }
        }
        random_hero.user_hero_number = number;
        userTeam.push_back(randomhero);
    }
    //random_hero.relation = relations;
    //userTeam.push_back(GameStat::getInstance()->random_hero);//TO DO...缺少排序，同名字的武将放到一起;访问数据库之后执行
    //被动技能默认没开启
    sprintf(sql1,"\"%s\",%d,%d,%d,1,%d,%d",userName.c_str(),totaldb_index[random_ind],number,userLevel,hero1.position,hero1.isPassiveActivated);
    //无论是否用户有此武将，都是一个存储方法

    GameStat::getInstance()->saveToSQLite("User_hero", documentpath1.c_str(),sql1);

}

int getHeroData(void* para, int n_column, char** column_value, char **column_name) {
    string sql1 = "\"";
    float abc;
    sscanf(column_value[19], "%f", &abc);

    if ((int)abc==1){//判断是第一条special，全信息
        GameStat::getInstance()->isNewHero=true;//设置为true: 抽到新英雄
        //HeroType hero1;
        GameStat::getInstance()->random_hero.name = column_value[0];//赋值武将名字
        GameStat::getInstance()->random_hero.engname = column_value[1];
        CCLOG("Hero name is: %s",GameStat::getInstance()->random_hero.name.c_str());
        sql1 = sql1 + column_value[0] + "\",\"" + column_value[1];
        sql1 += "\"";
        //if(GameStat::getInstance()->indexOfTable==0){}
        //hero1.index= (int)column_value[1];//遍历前18个储存点
        sscanf(column_value[2], "%f", &abc);
        //GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].attack = (int)abc;
        //TO DO....两种方法不知道哪个更快
        GameStat::getInstance()->random_hero.index = abc;
        sscanf(column_value[3], "%f", &abc);
        GameStat::getInstance()->random_hero.level = abc;
        sscanf(column_value[4], "%f", &abc);
        GameStat::getInstance()->random_hero.attack = abc;
        GameStat::getInstance()->random_hero.attackDisplay = abc;
        CCLOG("attack is :%f",GameStat::getInstance()->random_hero.attack);
        sscanf(column_value[5], "%f", &abc);
        //GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].defense = (int)abc;
        GameStat::getInstance()->random_hero.defense = abc;
        GameStat::getInstance()->random_hero.defenseDisplay = abc;
        sscanf(column_value[6], "%f", &abc);
        //GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].health = (int)abc;
        GameStat::getInstance()->random_hero.health = abc;
        GameStat::getInstance()->random_hero.healthDisplay = abc;
        sscanf(column_value[7], "%f", &abc);
        GameStat::getInstance()->random_hero.agility = abc;
        GameStat::getInstance()->random_hero.agilityDisplay = abc;
        sscanf(column_value[8], "%f", &abc);
        GameStat::getInstance()->random_hero.critic = abc;
        GameStat::getInstance()->random_hero.criticDisplay = abc;
        sscanf(column_value[9], "%f", &abc);
        GameStat::getInstance()->random_hero.antiCritic = abc;
        sscanf(column_value[10], "%f", &abc);
        GameStat::getInstance()->random_hero.target = (int)abc;
        sscanf(column_value[11], "%f", &abc);
        GameStat::getInstance()->random_hero.prob = abc;
        sscanf(column_value[12], "%f", &abc);
        GameStat::getInstance()->random_hero.parameter1 = abc;
        sscanf(column_value[13], "%f", &abc);
        GameStat::getInstance()->random_hero.attackType = (int)abc;
        sscanf(column_value[14], "%f", &abc);
        GameStat::getInstance()->random_hero.parameter2 = abc;
        sscanf(column_value[15], "%f", &abc);
        GameStat::getInstance()->random_hero.changeType = (int)abc;
        sscanf(column_value[16], "%f", &abc);
        GameStat::getInstance()->random_hero.change_value = abc;
        sscanf(column_value[17], "%f", &abc);
        GameStat::getInstance()->random_hero.active_special = abc;
        sscanf(column_value[18], "%f", &abc);
        GameStat::getInstance()->random_hero.special = (int)abc;

        sscanf(column_value[20], "%f", &abc);//19是special_index
        float abc2;
        sscanf(column_value[21], "%f", &abc2);
        GameStat::getInstance()->random_hero.special_value.push_back(pair<int,float>((int)abc,abc2));//special_type, special_value
        
        GameStat::getInstance()->random_hero.active_skill_name = column_value[23];
        GameStat::getInstance()->random_hero.active_skill_animation_name = column_value[24];
        sscanf(column_value[25], "%f", &abc);
        GameStat::getInstance()->random_hero.active_skill_duration = abc;

        GameStat::getInstance()->random_hero.active_skill_describe = column_value[26];
        GameStat::getInstance()->random_hero.passive_skill_name = column_value[27];
        GameStat::getInstance()->random_hero.passive_skill_describe = column_value[28];
        GameStat::getInstance()->random_hero.heroInfo = column_value[29];
        sscanf(column_value[30], "%f", &abc);
        GameStat::getInstance()->random_hero.kingdom = (int)abc;
        sscanf(column_value[31], "%f", &abc);
        GameStat::getInstance()->random_hero.quality = (int)abc;
        sscanf(column_value[32], "%f", &abc);
        GameStat::getInstance()->random_hero.heroType = (int)abc;
        sscanf(column_value[33], "%f", &abc);
        GameStat::getInstance()->random_hero.attackins = (int)abc;
        sscanf(column_value[34], "%f", &abc);
        GameStat::getInstance()->random_hero.defenseins = (int)abc;
        sscanf(column_value[35], "%f", &abc);
        GameStat::getInstance()->random_hero.healthins = (int)abc;
        sscanf(column_value[36], "%f", &abc);
        GameStat::getInstance()->random_hero.agilityins = (int)abc;
        sscanf(column_value[37], "%f", &abc);
        GameStat::getInstance()->random_hero.criticins = (int)abc;

        
        CCLOG("adding new entry %s : %f",column_name[12],GameStat::getInstance()->random_hero.parameter1);
        CCLOG("adding new entry %s : %d",column_name[13],GameStat::getInstance()->random_hero.attackType);
        CCLOG("adding new entry %s : %f",column_name[14],GameStat::getInstance()->random_hero.parameter2);
        CCLOG("adding new entry %s : %d",column_name[15],GameStat::getInstance()->random_hero.changeType);
        
        //if(GameStat::getInstance()->indexOfTable!=0){
            for (int i=0; i < n_column; i++){
                sql1 += ",";
                sql1 += column_value[i];//用insert * 语句准备加入sqlite database
            }
            //GameStat::getInstance()->saveToSQLite("userlist", "/Users/dingyi/Desktop/project1/Game12/Resources/usersql.db", sql1.c_str());
        
        CCLOG("空存储位置刚刚+1！");
    }
    else{//第二、三、四条special属性赋值
        sscanf(column_value[20], "%f", &abc);//19是special_index
        float abc2;
        sscanf(column_value[21], "%f", &abc2);
        GameStat::getInstance()->random_hero.special_value.push_back(pair<int,float>((int)abc,abc2));//special_type, special_value

    }
    return 0;
    
    
}
void GameStat::selectEnemyTeam(const char* table, const char* dbFilename,const char* sql){
    getTotalHeroIndex(table,dbFilename,sql);
}
void GameStat::getTotalHeroIndex(const char* table, const char* dbFilename,const char* sql){
    sqlite3* pDB = NULL;
    CCLOG("Now I\'m going to read index in original database");
    //if (pDB != NULL) {
    //	int count = 0;
    char *errorMsg;
    random_hero.special_value.clear();
    if(sqlite3_open(dbFilename,&pDB)==SQLITE_OK){
        CCLOG("read success");
        sqlite3_exec(pDB, sql, getHeroData, this, &errorMsg);
        if (errorMsg != NULL) {
            CCLOG("exec sql %s fail with msg: %s", sql, errorMsg);
            sqlite3_close(pDB);
            return;
        }
    }else{
        CCLOG("read is failure");
        sqlite3_close(pDB);
    }
    
}
void GameStat::saveNewHeroToUser(const char* table, const char* dbFilename){
    char sql[1024];
    sqlite3* pDB = NULL;
    char *errorMsg;
    
    if(sqlite3_open(dbFilename,&pDB)==SQLITE_OK){
        CCLOG("read success");
        sqlite3_exec(pDB, sql, loadUserRecord, this, &errorMsg);
        if (errorMsg != NULL) {
            CCLOG("exec sql %s fail with msg: %s", sql, errorMsg);
            sqlite3_close(pDB);
            return;
        }
    }else{
        CCLOG("read is failure");
        sqlite3_close(pDB);
    }

    
}
void  GameStat::readFromSQLite(const char* table, const char* dbFilename,int index) {
    char sql[1024];
    //this->indexOfTable = index;
    //sqlite3* pDB = prepareTableInDB(table, dbFilename);
    sqlite3* pDB = NULL;
    CCLOG("Now I\'m going to read index in original database");
    //if (pDB != NULL) {
    //	int count = 0;
    char *errorMsg;
    if(index==0){
        sprintf(sql, "select * from %s", table);//if index=0, means that将从usersql里调出全部信息到userTeam
    }
    else{
        sprintf(sql, "select * from %s where number = %d", table, index);//从总database里select出刚抽的武将
    }
    if(sqlite3_open(dbFilename,&pDB)==SQLITE_OK){
        CCLOG("read succss");
        sqlite3_exec(pDB, sql, loadUserRecord, this, &errorMsg);
        if (errorMsg != NULL) {
            CCLOG("exec sql %s fail with msg: %s", sql, errorMsg);
            sqlite3_close(pDB);
            return;
        }
    }else{
        CCLOG("read is failure");
        sqlite3_close(pDB);
    }
}
//先读取用户英雄信息，再根据index从总数据库调
int loadHeroDataToUser(void* para, int n_column, char** column_value, char **column_name) {
    float abc;
    sscanf(column_value[19], "%f", &abc);//19是special_index

    if(((int)abc)==1){
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].name = column_value[0];//赋值武将名字
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].engname = column_value[1];
    CCLOG("Hero name is: %s,%s",GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].name.c_str(),GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].engname.c_str());

    /*sscanf(column_value[2], "%f", &abc);
    //GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].attack = (int)abc;
    //TO DO....两种方法不知道哪个更快
    GameStat::getInstance()->random_hero.index = abc;
    sscanf(column_value[3], "%f", &abc);
    GameStat::getInstance()->random_hero.level = abc;
     */
    sscanf(column_value[4], "%f", &abc);
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].attack = abc;
    CCLOG("用户英雄通过index从总库调回attack is :%f",GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].attack);
    sscanf(column_value[5], "%f", &abc);
    //GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].defense = (int)abc;
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].defense = abc;
    sscanf(column_value[6], "%f", &abc);
    //GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].health = (int)abc;
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].health = abc;
    sscanf(column_value[7], "%f", &abc);
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].agility = abc;
    sscanf(column_value[8], "%f", &abc);
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].critic = abc;
    sscanf(column_value[9], "%f", &abc);
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].antiCritic = abc;
    sscanf(column_value[10], "%f", &abc);
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].target = (int)abc;
    sscanf(column_value[11], "%f", &abc);
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].prob = abc;
    sscanf(column_value[12], "%f", &abc);
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].parameter1 = abc;
    sscanf(column_value[13], "%f", &abc);
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].attackType = (int)abc;
    sscanf(column_value[14], "%f", &abc);
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].parameter2 = abc;
    sscanf(column_value[15], "%f", &abc);
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].changeType = (int)abc;
    sscanf(column_value[16], "%f", &abc);
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].change_value = abc;
    sscanf(column_value[17], "%f", &abc);
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].active_special = abc;
    sscanf(column_value[18], "%f", &abc);
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].special = (int)abc;
    
    sscanf(column_value[20], "%f", &abc);//19是special_index
    float abc2;
    sscanf(column_value[21], "%f", &abc2);
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].special_value.push_back(pair<int,float>((int)abc,abc2));//special_type, special_value
    
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].active_skill_name = column_value[23];
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].active_skill_animation_name = column_value[24];
    sscanf(column_value[25], "%f", &abc);
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].active_skill_duration = abc;
    
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].active_skill_describe = column_value[26];
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].passive_skill_name = column_value[27];
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].passive_skill_describe = column_value[28];
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].heroInfo = column_value[29];
    sscanf(column_value[30], "%f", &abc);
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].kingdom = (int)abc;
    sscanf(column_value[31], "%f", &abc);
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].quality = (int)abc;
    sscanf(column_value[32], "%f", &abc);
    GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].heroType = (int)abc;
        sscanf(column_value[33], "%f", &abc);
        team[team.size()-1].attackins = (int)abc;
        sscanf(column_value[34], "%f", &abc);
        team[team.size()-1].defenseins = (int)abc;
        sscanf(column_value[35], "%f", &abc);
        team[team.size()-1].healthins = (int)abc;
        sscanf(column_value[36], "%f", &abc);
        team[team.size()-1].agilityins = (int)abc;
        sscanf(column_value[37], "%f", &abc);
        team[team.size()-1].criticins = (int)abc;
        
        //算出等级之后的属性Display
        
        team[team.size()-1].attackDisplay = team[team.size()-1].attack+(user_level-1)*team[team.size()-1].attackins;
        team[team.size()-1].defenseDisplay = team[team.size()-1].defense+(user_level-1)*team[team.size()-1].defenseins;
        team[team.size()-1].healthDisplay = team[team.size()-1].health+(user_level-1)*team[team.size()-1].healthins;
        team[team.size()-1].agilityDisplay = team[team.size()-1].agility+(user_level-1)*team[team.size()-1].agilityins;
        team[team.size()-1].criticDisplay = team[team.size()-1].critic+(user_level-1)*team[team.size()-1].criticins;
        CCLOG("用户英雄通过index从总库调回attack is :%f,%d,%f",team[team.size()-1].attack,team[team.size()-1].attackins,team[team.size()-1].attackDisplay);


    }
    else{//第二、三、四条special属性赋值
        sscanf(column_value[20], "%f", &abc);//19是special_index
        float abc2;
        sscanf(column_value[21], "%f", &abc2);
        GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1].special_value.push_back(pair<int,float>((int)abc,abc2));//special_type, special_value
        
    }

    return 0;
}
int loadUserHeros(void* para, int n_column, char** column_value, char **column_name) {
    float abc;
    sscanf(column_value[1], "%f", &abc);
    HeroType hero1;
    hero1.index = (int)abc;
    sscanf(column_value[2], "%f", &abc);
    hero1.user_hero_number = (int)abc;
    sscanf(column_value[3], "%f", &abc);
    hero1.level = user_level;
    sscanf(column_value[4], "%f", &abc);
    hero1.evolution_stage = (int)abc;
    sscanf(column_value[5], "%f", &abc);
    hero1.position = (int)abc;
    sscanf(column_value[6], "%f", &abc);
    hero1.isPassiveActivated = (int)abc;
    
    //string documentpath1 = FileUtils::getInstance()->fullPathForFilename("wholeheros.db");

    //attack数值作为一个标志
    
    GameStat::getInstance()->readFromRelation("Relation", documentpath2.c_str(),hero1.index);//读取天命
    hero1.relation = hero_relation;
    GameStat::getInstance()->userTeam.push_back(hero1);
    
    char sql1[1024];
    sprintf(sql1,"select * from Heros where number = %d",hero1.index);//从总数据库调，不需要用户名
    CCLOG("数据库语句为%s",sql1);

    GameStat::getInstance()->loadUserHerosbyIndex("Heros", documentpath2.c_str(), sql1);
    //算出按照等级的属性
    
    if(hero1.position<6){
        GameStat::getInstance()->userCompeteMap[hero1.position] = GameStat::getInstance()->userTeam[GameStat::getInstance()->userTeam.size()-1];
    }
    //
    return 0;


}
void GameStat::loadUserHerosbyIndex(const char* table, const char* dbFilename,const char* sql) {
    sqlite3* pDB = NULL;
    CCLOG("Now I\'m going to read index in original database");
    char *errorMsg;
    if(sqlite3_open(dbFilename,&pDB)==SQLITE_OK){
        CCLOG("read succss");
        sqlite3_exec(pDB, sql, loadHeroDataToUser, this, &errorMsg);
        if (errorMsg != NULL) {
            CCLOG("exec sql %s fail with msg: %s", sql, errorMsg);
            sqlite3_close(pDB);
            return;
        }
    }else{
        CCLOG("read is failure");
        sqlite3_close(pDB);
    }
}

void GameStat::readFromUserHeros(const char* table, const char* dbFilename,int index) {
    sqlite3* pDB = GameStat::prepareTableInDB(table, dbFilename);

    char sql[1024];
    //sqlite3* pDB = NULL;
    CCLOG("Now I\'m going to read index in original database");
    char *errorMsg;
    if(index==0){
        sprintf(sql, "select * from %s where username = \"%s\"", table, userName.c_str());//根据用户名读取英雄
    }
    else{
        sprintf(sql, "select * from %s where number = %d", table, index);//从总database里select出刚抽的武将
    }
    CCLOG("数据库名称%s",dbFilename);
    if(sqlite3_open(dbFilename,&pDB)==SQLITE_OK){
        CCLOG("read succss");
        sqlite3_exec(pDB, sql, loadUserHeros, this, &errorMsg);
        if (errorMsg != NULL) {
            CCLOG("exec sql %s fail with msg: %s", sql, errorMsg);
            sqlite3_close(pDB);
            return;
        }
    }else{
        CCLOG("read is failure");
        sqlite3_close(pDB);
    }
}

int loadHeroRecord(void* para, int n_column, char** column_value, char **column_name) {
	string sql1 = "\"";
	//赋值class里的userTeam and teamName;
        HeroType hero1;
        hero1.name = column_value[0];//赋值武将名字
        
        CCLOG("Hero name is: %s",hero1.name.c_str());
        float abc;
    
	
	return 0;
    
}

void  GameStat::readFromSQLiteWholesql(const char* table, const char* dbFilename,int index) {
	char sql[1024];
    sqlite3* pDB = NULL;
	CCLOG("Now I\'m going to read index in original database");
	//if (pDB != NULL) {
	//	int count = 0;
    char *errorMsg;
    sprintf(sql, "select * from %s where number = %d", table, 1);//从总database里select出刚抽的武将
    
	if(sqlite3_open(dbFilename,&pDB)==SQLITE_OK){
        CCLOG("read succss");
        sqlite3_exec(pDB, sql, loadHeroRecord, this, &errorMsg);
        if (errorMsg != NULL) {
            CCLOG("exec sql %s fail with msg: %s", sql, errorMsg);
            sqlite3_close(pDB);
            return;
        }
    }else{
        CCLOG("read is failure");
        sqlite3_close(pDB);
    }
}
int loadRecordCount(void* para, int n_column, char** column_value, char** column_name)
{
	int *pCount = (int*)para;
	sscanf(column_value[0], "%d", pCount);
	return 0;
}
void GameStat::saveToSQLite(const char* table, const char *dbFilename,const char* sql) {
	sqlite3* pDB = GameStat::prepareTableInDB(table, dbFilename);
	//if (pDB != NULL){
		//TO DO......
		/*int count = 0;
         char *errorMsg;
         sqlite3_exec(pDB, sql, loadRecordCount, &count, &errorMsg);
         if (errorMsg != NULL){
         CCLOG("reason: %s, Error Message: %s \n", sql, errorMsg);
         sqlite3_close(pDB);
         
         }
         if (count){
         CCLOG("updating new score!");
         sprintf(sql, "update %s set score = %d where id = \"%s\"", table, this->getScore(), this->getUserID().c_str());
         }
         
         else{
         */
    //sqlite3* pDB = NULL;

    char *errorMsg;
    CCLOG("add new entry to User's DB");
    string sql1 = sql;
    sql1 = "insert into "+(string)table+" values("+sql+")";//确定最终的sql语句，添加新武将信息
    log("存入数据库语句为%s",sql1.c_str());
    if(sqlite3_open(dbFilename,&pDB)==SQLITE_OK){

        sqlite3_exec(pDB, sql1.c_str(), NULL, NULL, &errorMsg);
        if (errorMsg != NULL){
            CCLOG("exec sql %s fail with msg: %s", sql1.c_str(), errorMsg);
            sqlite3_close(pDB);
            return;
        }
    }else{
        CCLOG("read is failure");
        sqlite3_close(pDB);
    }
	sqlite3_close(pDB); // 
}

int loadUserInfo(void* para, int n_column, char** column_value, char **column_name) {
    float abc;
    sscanf(column_value[1], "%f", &abc);
    GameStat::getInstance()->userLevel = (int)abc;
    sscanf(column_value[2], "%f", &abc);
    GameStat::getInstance()->new_player_stage = (int)abc;
    sscanf(column_value[3], "%f", &abc);
    GameStat::getInstance()->expExisted = (int)abc;
    sscanf(column_value[4], "%f", &abc);
    GameStat::getInstance()->expNextLevel = (int)abc;
    sscanf(column_value[5], "%f", &abc);
    GameStat::getInstance()->user_yinliang = (int)abc;
    sscanf(column_value[6], "%f", &abc);
    GameStat::getInstance()->user_yuanBao = (int)abc;
    sscanf(column_value[7], "%f", &abc);
    GameStat::getInstance()->user_reputation = (int)abc;

    return 0;
    
}

int GameStat::readFromUserInfo(const char* table, const char* dbFilename){
    char sql[1024];
    sqlite3* pDB = GameStat::prepareTableInDB(table, dbFilename);

    //sqlite3* pDB = NULL;
    CCLOG("Now I\'m going to read index in original database");
    char *errorMsg;
    sprintf(sql, "select * from %s where username = \"%s\"", table, userName.c_str());//根据用户名读取英雄
    CCLOG("数据库名称%s",dbFilename);
    if(sqlite3_open(dbFilename,&pDB)==SQLITE_OK){
        CCLOG("read succss");
        sqlite3_exec(pDB, sql, loadUserInfo, this, &errorMsg);
        if (errorMsg != NULL) {
            CCLOG("exec sql %s fail with msg: %s", sql, errorMsg);
            sqlite3_close(pDB);
            return 0;
        }
    }else{
        CCLOG("read is failure");
        sqlite3_close(pDB);
        return 0;
    }
    if(userLevel==0)
        return 0;
    return 1;
}
//改用户等级，银两等等
void GameStat::saveToUserInfo(const char* table, const char *dbFilename,const char* sql){
    sqlite3* pDB = NULL;
    char *errorMsg;
    CCLOG("add new entry to User's DB");
    if(sqlite3_open(dbFilename,&pDB)==SQLITE_OK){
        
        sqlite3_exec(pDB, sql, NULL, NULL, &errorMsg);
        if (errorMsg != NULL){
            CCLOG("exec sql %s fail with msg: %s", sql, errorMsg);
            sqlite3_close(pDB);
            return;
        }
    }else{
        CCLOG("read is failure");
        sqlite3_close(pDB);
    }
    sqlite3_close(pDB); //
}

int loadHeroRelation(void* para, int n_column, char** column_value, char **column_name) {
    
    float abc;
    sscanf(column_value[5], "%f", &abc);
    if((int)abc==1){//最终是1个英雄index生成5-6个relation组成的relations
        RelationSeries relation1;

        sscanf(column_value[1], "%f", &abc);
        relation1.relation_index = (int)abc;
        relation1.relation_name = column_value[2];
        relation1.relationDescribe = column_value[3];

        sscanf(column_value[4], "%f", &abc);
        relation1.type = (int)abc;
        sscanf(column_value[6], "%f", &abc);
        relation1.indexHero.push_back((int)abc);
        sscanf(column_value[7], "%f", &abc);
        relation1.extra_type = (int)abc;
        sscanf(column_value[8], "%f", &abc);
        relation1.extra_value = abc;
        log("缘分为%s",relation1.relationDescribe.c_str());
        hero_relation.push_back(relation1);
    }
    else{
        sscanf(column_value[6], "%f", &abc);
        hero_relation[hero_relation.size()-1].indexHero.push_back((int)abc);
    }
    return 0;
    
}

void GameStat::readFromRelation(const char* table, const char *dbFilename,const int relation_index){
    char sql[1024];
    sqlite3* pDB = NULL;
    CCLOG("Now I\'m going to read index in original database");
    //if (pDB != NULL) {
    //	int count = 0;
    char *errorMsg;
    sprintf(sql, "select * from %s where hero_index = %d", table, relation_index);//从总database里select出刚抽的武将
    relations.clear();
    if(sqlite3_open(dbFilename,&pDB)==SQLITE_OK){
        CCLOG("read succss");
        sqlite3_exec(pDB, sql, loadHeroRelation, this, &errorMsg);
        if (errorMsg != NULL) {
            CCLOG("exec sql %s fail with msg: %s", sql, errorMsg);
            sqlite3_close(pDB);
            return;
        }
    }else{
        CCLOG("read is failure");
        sqlite3_close(pDB);
    }

}

int getEquipData(void* para, int n_column, char** column_value, char **column_name) {
    string sql1 = "\"";
    float abc;
    sscanf(column_value[8], "%f", &abc);
    
    if ((int)abc==1){//判断是第一条special，全信息
        GameStat::getInstance()->random_zhuangbei.name = column_value[0];//赋值武将名字
        GameStat::getInstance()->random_zhuangbei.engname = column_value[1];
        sscanf(column_value[2], "%f", &abc);
        GameStat::getInstance()->random_zhuangbei.index = (int)abc;
        sscanf(column_value[3], "%f", &abc);
        GameStat::getInstance()->random_zhuangbei.type = (int)abc;
        GameStat::getInstance()->random_zhuangbei.specialInfo = column_value[4];
        GameStat::getInstance()->random_zhuangbei.relation_name.push_back(column_value[5]);
        
        sscanf(column_value[6], "%f", &abc);
        GameStat::getInstance()->random_zhuangbei.relationHeroIndex.push_back((int)abc);
        GameStat::getInstance()->random_zhuangbei.special_relate_info = column_value[7];

        sscanf(column_value[9], "%f", &abc);
        GameStat::getInstance()->random_zhuangbei.special_type = (int)abc;
        sscanf(column_value[10], "%f", &abc);
        GameStat::getInstance()->random_zhuangbei.special_value = abc;
        sscanf(column_value[11], "%f", &abc);
        GameStat::getInstance()->random_zhuangbei.special_value_relation = abc;
        sscanf(column_value[12], "%f", &abc);
        GameStat::getInstance()->random_zhuangbei.quality = (int)abc;
        sscanf(column_value[13], "%f", &abc);
        GameStat::getInstance()->random_zhuangbei.property = (int)abc;
        sscanf(column_value[14], "%f", &abc);
        GameStat::getInstance()->random_zhuangbei.propertyIns = (int)abc;
        GameStat::getInstance()->random_zhuangbei.info = column_value[15];

        randomzhuangbei.propertyDisplay = randomzhuangbei.property+(user_level-1)*randomzhuangbei.propertyIns;
        
        CCLOG("adding new entry %s : %d",column_name[9],GameStat::getInstance()->random_zhuangbei.special_type);
        CCLOG("adding new entry %s : %f",column_name[10],GameStat::getInstance()->random_zhuangbei.special_value);
        CCLOG("adding new entry %s : %f",column_name[11],GameStat::getInstance()->random_zhuangbei.special_value_relation);
        CCLOG("adding new entry %s : %d",column_name[12],GameStat::getInstance()->random_zhuangbei.quality);
        
        
        CCLOG("空存储位置刚刚+1！");
    }
    else{//第二、三、四条special属性赋值
        float abc2;
        sscanf(column_value[21], "%f", &abc2);
        GameStat::getInstance()->random_zhuangbei.relation_name.push_back(column_value[5]);//special_type, special_value
        
    }
    return 0;
    
    
}
void GameStat::getEquipInfo(const char* table, const char* dbFilename,const char* sql){
    sqlite3* pDB = NULL;
    CCLOG("Now I\'m going to read index in original database");
    //if (pDB != NULL) {
    //	int count = 0;
    char *errorMsg;
    random_zhuangbei.relation_name.clear();
    random_zhuangbei.relationHeroIndex.clear();

    if(sqlite3_open(dbFilename,&pDB)==SQLITE_OK){
        CCLOG("read success");
        sqlite3_exec(pDB, sql, getEquipData, this, &errorMsg);
        if (errorMsg != NULL) {
            CCLOG("exec sql %s fail with msg: %s", sql, errorMsg);
            sqlite3_close(pDB);
            return;
        }
    }else{
        CCLOG("read is failure");
        sqlite3_close(pDB);
    }
    //TO DO.....缺少装备的number_index计算
    
}
int loadUserZhuangbei(void* para, int n_column, char** column_value, char **column_name) {
    //ZhuangBeiType zhuangbei1;
    randomzhuangbei.relation_name.clear();
    randomzhuangbei.relationHeroIndex.clear();
    

    float abc;
    sscanf(column_value[1], "%f", &abc);
    randomzhuangbei.index = (int)abc;
    sscanf(column_value[2], "%f", &abc);
    randomzhuangbei.number_index = (int)abc;
    sscanf(column_value[3], "%f", &abc);
    randomzhuangbei.level = (int)abc;
    sscanf(column_value[4], "%f", &abc);
    randomzhuangbei.type = (int)abc;
    sscanf(column_value[5], "%f", &abc);
    randomzhuangbei.evolution_stage = (int)abc;
    sscanf(column_value[6], "%f", &abc);
    randomzhuangbei.position = (int)abc;
    
    
    //GameStat::getInstance()->readFromRelation("Relation", documentpath2.c_str(),hero1.index);//读取天命
    //hero1.relation = hero_relation;
    
    char sql1[1024];
    sprintf(sql1,"select * from Zhuangbei where number = %d and type = %d",randomzhuangbei.index,randomzhuangbei.type);//从总数据库调，不需要用户名
    CCLOG("数据库语句为%s",sql1);
    GameStat::getInstance()->getEquipInfo("Zhuangbei", documentpath2.c_str(), sql1);
    //算出按照等级的属性
    
    if(randomzhuangbei.position<6){//如果在某个英雄身上装备,放到英雄身上
        GameStat::getInstance()->userCompeteMap[randomzhuangbei.position].zhuangbei.push_back(randomzhuangbei);
    }
    GameStat::getInstance()->zhuangbei_list.push_back(randomzhuangbei);

    return 0;
    
    
}


void GameStat::readfromEquipInfo(const char* table, const char* dbFilename,const char* sql){
    sqlite3* pDB = GameStat::prepareTableInDB(table, dbFilename);

    //sqlite3* pDB = NULL;
    CCLOG("Now I\'m going to read index in original database");
    //if (pDB != NULL) {
    //	int count = 0;
    char *errorMsg;
    if(sqlite3_open(dbFilename,&pDB)==SQLITE_OK){
        CCLOG("read success");
        sqlite3_exec(pDB, sql, loadUserZhuangbei, this, &errorMsg);
        if (errorMsg != NULL) {
            CCLOG("exec sql %s fail with msg: %s", sql, errorMsg);
            sqlite3_close(pDB);
            return;
        }
    }else{
        CCLOG("read is failure");
        sqlite3_close(pDB);
    }
    
}

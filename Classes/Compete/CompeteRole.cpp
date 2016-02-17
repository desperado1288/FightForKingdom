#include "Compete/CompeteRole.h"
#include "GameStat.h"
#include "Compete/CompeteLayer.h"
#include "LayerManager/MainScene.h"
#include "cocos-ext.h"
#include <cocosbuilder/CocosBuilder.h>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
//封装精灵
bool CompeteRole::init(HeroType hero, CompeteLayer *layer){
    if(!CCSprite::init()){
        return false;
    }
    act_skill_name = hero.active_skill_name;
    pas_skill_name = hero.passive_skill_name;
    player=layer;
    mhero=hero;
    changeHp=0;
    resName=hero.engname;
    string heroPicPath ="heroBody/"+resName+".png";
    CCLOG("creating sprite name:%s",heroPicPath.c_str());
    //create(heroPicPath);//TO DO...创建Sprite
    initWithFile(heroPicPath);
    auto heroFrame = Sprite::createWithSpriteFrameName("4_btm.png");
    this->addChild(heroFrame,-1,HERO_FRAME_TAG);
//------------英雄名字
    auto heroname = Label::createWithTTF(hero.name, "字体/华文新魏.TTF", 30);
    if(hero.quality==1)
        heroname->setColor(Color3B::MAGENTA);
    else if(hero.quality==2)
        heroname->setColor(Color3B::BLUE);
    else if(hero.quality==3)
        heroname->setColor(Color3B::GREEN);
    else if(hero.quality==0)
        heroname->setColor(Color3B::ORANGE);
    heroname->setPosition(getContentSize().width/2,-10);
    addChild(heroname,1,NAME_TAG);
//-------------//
    setScale(0.69f);
    //heroFrame->setScale(0.8f);
    heroFrame->setPosition(this->getContentSize().width/2,this->getContentSize().height/2);
    level=hero.level;
    chaId=0;
    //attack=20;
    target=hero.target;
    parameter1 = hero.parameter1;
    parameter2 = hero.parameter2;
    changeType = hero.changeType;
    //index = hero.index;
    prob = hero.prob;
    attackType = hero.attackType;
    special = hero.special;
    position=hero.position;
    resNum=0;
    curHP=hero.healthDisplay;
    totalHp=hero.healthDisplay;
    dir=0;
    attackcount=0;
    //TO DO...战斗中的属性
    isAttackUp=0;
    isDefenseUp=0;
    isCriticUp=0;
    isAgilityUp=0;
    isKanpoUp=0;
    isAttackDown=0;
    isDefenseDown=0;
    isCriticDown=0;
    isAgilityDown=0;
    isKanpoDown=0;
    isDeepDamage=0;//是否伤害加深
    isChaos=0;//是否混乱
    isDizzy=0;//眩晕
    //解锁被动
    pass_hit_time = 0;
    special_type = hero.special_value;
    //isDoubleAttack = false;
    //isCritic = false;
    //-------------------------//
    
    auto barbg=Sprite::create("barbg.png");//TO DO.....加血条图片
    addChild(barbg,1,healthbarbg_tag);
    barbg->setPosition(getContentSize().width/2,getContentSize().height/2+90);
    barbg->setScaleX(1.5);
    auto hebar=Sprite::create("healthbar.png");
    healthBar = ProgressTimer::create(hebar);
    healthBar->setType(kCCProgressTimerTypeBar);
    healthBar->setPercentage(100);
    healthBar->setMidpoint(Vec2(0.0f,1.0f));
    healthBar->setBarChangeRate(Vec2(1,0));
    healthBar->setPosition(getContentSize().width/2,getContentSize().height/2+90);
    healthBar->setScaleX(1.5);
    addChild(healthBar,1,healthBar_Tag);
    schedule(schedule_selector(CompeteRole::healthBarLogic));
    
    //TO DO...有可能要放到下边
//-----------
    return true;
}
void CompeteRole::healthBarLogic(float dt){
    healthBar->setPercentage(((float)curHP/(float)totalHp) *100);
}
void CompeteRole::remove()
{
    removeFromParentAndCleanup(true);
}
void CompeteRole::attack(bool isSkill,int tag1, vector<int> tag2, bool isDoubleAttack, bool isEnemy, float time)//tag1是攻击方
{
    target_tag = tag2;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    int para = 1;
    if (!isEnemy) {
        para=-1;
    }

    for(auto it = target_tag.begin();it!=target_tag.end();it++){
        if(isEnemy)
            *it+=200;
        else
            *it+=100;
    }
    //float distance = abs(((CompeteRole*)player->getChildByTag(target_tag[0]))->pos.y-((CompeteRole*)player->getChildByTag(tag1))->pos.y)-sizeY-30;
    Vec2 moveto = Vec2(((CompeteRole*)player->getChildByTag(target_tag[0]))->pos.x,((CompeteRole*)player->getChildByTag(target_tag[0]))->pos.y-para*(sizeY+30));
    float angle,slope;
    /*if(isEnemy){
        slope = (((CompeteRole*)player->getChildByTag(target_tag[0]))->pos-((CompeteRole*)player->getChildByTag(tag1))->pos).getAngle()*(-para);
    }else
        slope = (((CompeteRole*)player->getChildByTag(tag1))->pos-((CompeteRole*)player->getChildByTag(target_tag[0]))->pos).getAngle()*(-para);
*/
    if(isEnemy){
        slope = (((CompeteRole*)player->getChildByTag(target_tag[0]))->pos-pos).getAngle()*(-para);
    }else
        slope = (pos-((CompeteRole*)player->getChildByTag(target_tag[0]))->pos).getAngle()*(-para);

    angle = slope*180/3.1415926;
    if(!isEnemy)
        angle+=2*(90-angle);
    CCLOG("子弹角度为%f,radian:%f",angle,slope);
    //delayTime = time;
    //TO DO....攻击动作时间由不同hero决定
        auto moveAct = MoveTo::create(0.5f, moveto);//移动过去
        auto moveActBack = MoveTo::create(0.25f, pos);
        auto bounceMove = EaseExponentialInOut::create(moveAct);
        auto rotateAct = RotateBy::create(0.3f, -30*para);
        auto bounce1 = EaseBounceInOut::create(rotateAct);
        //TO DO.....以后rotate换成攻击动作同时特效
    //暂定，普攻和技能都是normal_bullet
    if(resName=="caoxing"||resName=="huangzhong"){
        auto seq = Sequence::create(bounce1,bounce1->reverse(), NULL);
        if(isDoubleAttack){//双击转圈
            auto doubleAct = RotateBy::create(0.6f, 360*para);
            seq = Sequence::create(seq,DelayTime::create(0.2f),doubleAct, NULL);
            //双击子弹,开始透明度为0,过0.8秒FadeIn
            
            auto normal_bullet = Sprite::createWithSpriteFrameName("normal_bullet_02.png");
            //normal_bullet->setPosition(player->getChildByTag(tag1)->getPosition());
            normal_bullet->setPosition(pos);
            normal_bullet->setOpacity(0);
            player->addChild(normal_bullet,7,BULLET_TAG+7);
            //此时angle还是默认角度
            normal_bullet->setRotation(angle);
            normal_bullet->runAction(Sequence::create(DelayTime::create(0.8f),FadeIn::create(0.05f),MoveTo::create(0.6f, player->getChildByTag(target_tag[0])->getPosition()),CallFuncN::create(CC_CALLBACK_1(CompeteRole::hurtEffect, this)),RemoveSelf::create(),NULL));
            
        }
        runAction(seq);
        for(int i=0;i<target_tag.size();i++){
            auto normal_bullet = Sprite::createWithSpriteFrameName("normal_bullet_02.png");
            //normal_bullet->setPosition(player->getChildByTag(tag1)->getPosition());
            normal_bullet->setPosition(pos);

            player->addChild(normal_bullet,7,BULLET_TAG+i);
            if(isEnemy){
                slope = (player->getChildByTag(target_tag[i])->getPosition()-pos).getAngle()*(-para);
            }else
                slope = (pos-player->getChildByTag(target_tag[i])->getPosition()).getAngle()*(-para);
            angle = slope*180/3.1415926;
            if(!isEnemy)
                angle+=2*(90-angle);

            normal_bullet->setRotation(angle);
            normal_bullet->runAction(Sequence::create(MoveTo::create(0.6f, player->getChildByTag(target_tag[i])->getPosition()),CallFuncN::create(CC_CALLBACK_1(CompeteRole::hurtEffect, this)),RemoveSelf::create(),NULL));

        }
    }
    /*if(resName=="zhangjiao"){
     auto seq = Sequence::create(bounce1,bounce1->reverse(),CallFuncN::create(CC_CALLBACK_1(CompeteRole::faSkill, this)), NULL);
     if(isDoubleAttack){
     auto seq1 = Sequence::create(seq,DelayTime::create(0.2f),doubleAct,DelayTime::create(0.2f), NULL);
     runAction(Sequence::create(DelayTime::create(time),seq1,NULL));
     
     }
     }*/

    else{//不是曹性,黄忠
        if(isSkill){//发动技能
            //TO DO...技能动作              0.5         0.3     0.3                     0.2                 0.2
            auto doubleAct = RotateBy::create(0.6f, 360*para);

            
        
            auto seq = Sequence::create(bounceMove,bounce1,bounce1->reverse(),NULL);
            if(isDoubleAttack){//双击转圈
                seq = Sequence::create(seq,DelayTime::create(0.2f),doubleAct, NULL);
            }
            seq = Sequence::create(seq,DelayTime::create(0.2f),moveActBack,NULL);
            runAction(Sequence::create(DelayTime::create(time),seq,NULL));

        
            
        }
        else{//普通攻击
            auto seq1 = Sequence::create(bounceMove,bounce1,bounce1->reverse(), NULL);
            if(isDoubleAttack){//双击转圈
                auto doubleAct = RotateBy::create(0.6f, 360*para);
                seq1 = Sequence::create(seq1,DelayTime::create(0.2f),doubleAct, NULL);
            }
            auto seq = Sequence::create(seq1,DelayTime::create(0.2f),moveActBack, NULL);
            runAction(Sequence::create(DelayTime::create(time),seq,NULL));
        }

    }
    //显示技能文字
    if(isSkill){
        auto skill_label = Label::createWithSystemFont(act_skill_name, "字体/华文新魏.TTF", 30);
        //skill_label->setPosition(visibleSize.width/2,visibleSize.height/2);
        //player->addChild(skill_label,8);
        addChild(skill_label,2,SKILL_LABEL_TAG);
        skill_label->setPosition(getContentSize().width/2,-30);
        skill_label->setColor(Color3B::ORANGE);
        //skill_label->enableOutline(Color4B(255,125,0,255),1);
        skill_label->enableShadow();
        skill_label->runAction(Sequence::create(DelayTime::create(time+0.6f),FadeOut::create(0.2f),RemoveSelf::create(), NULL));
        //技能文字背景
        auto skilllabel_bg = Sprite::createWithSpriteFrameName("bg_duihuakuang_red.png");
        skilllabel_bg->setPosition(getContentSize().width/2,-30);
        addChild(skilllabel_bg,1,SKILL_BG_TAG);
        skilllabel_bg->runAction(Sequence::create(DelayTime::create(time+0.6f),FadeOut::create(0.2f),RemoveSelf::create(), NULL));

    }
    /*auto scal = ScaleBy::create(0.1,1.2);
    
    auto scal2= ScaleTo::create(0.1,0.45);
    */
    //TO DO.....攻击动作以后要改，需要分特技和普攻
    
    //playMusic(skills);
}
void CompeteRole::faSkill(Ref* pSender){
    Size visibleSize = Director::getInstance()->getVisibleSize();

    
    cocosbuilder::NodeLoaderLibrary *ccNodeLoaderLibrary = cocosbuilder::NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
    cocosbuilder::CCBReader *ccbReader = new cocosbuilder::CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();
    auto ccbCriticbg = ccbReader->readNodeGraphFromFile("ccb_fx/RuYunLong.ccbi", this);
    ccbCriticbg->setPosition(visibleSize.width/2,visibleSize.height/2);
    this->getParent()->addChild(ccbCriticbg,5,910);

}
void CompeteRole::hurtEffect(Ref* pSender){
    
}



void CompeteRole::attackDamage(vector<float> damage, bool isDouble, vector<int> isCritic,vector<int> isblock, float time,int pass_type){
    attackCritic = isCritic;
    attackDouble = isDouble;
    attackBlock = isblock;
    getDamage = damage;
    //CCLOG("此次伤害基数为%f",getDamage[0]);
    
    attackDoubleIndex = 0;//双击计数器
//被动文字-----------------
    auto pass_label = Label::createWithTTF("", "字体/华文新魏.TTF", 45);
    pass_label->setColor(Color3B::YELLOW);
    if(pass_type==2){
        pass_label->setString("穿透");
    }
    //pass_label->setPosition(-pass_label->getContentSize().width/2,sizeY+pass_label->getContentSize().height/2+20);
    pass_label->setPosition(pos.x-sizeX/2-pass_label->getContentSize().width/2,pos.y+sizeY/2+pass_label->getContentSize().height/2+20);
    
    pass_label->setOpacity(0);
    getParent()->addChild(pass_label,8);
    pass_label->runAction(Sequence::create(DelayTime::create(time),FadeIn::create(0.01f),DelayTime::create(0.5f),FadeOut::create(0.2f),RemoveSelf::create(),NULL));
//------------------------
//伤害文字-----------------
    auto label2 = LabelAtlas::create("0123456789-", "fonts/number_red.png",49,88,'0');
    float para1 = 1.0f;
    if(pass_type==1){//双击
        auto doubleAtk = Label::createWithSystemFont("双击", "字体/华文新魏.TTF", 45);
        doubleAtk->setColor(Color3B::MAGENTA);
        doubleAtk->enableOutline(Color4B::MAGENTA,1);
        doubleAtk->enableShadow();
        this->getParent()->addChild(doubleAtk,7,DAMAGE_DOUBLE_TAG+tag);
        doubleAtk->setOpacity(0);
        //doubleAtk->setPosition(-40,doubleAtk->getContentSize().height/2);
        doubleAtk->setPosition(pos.x-sizeX/2-pass_label->getContentSize().width/2,pos.y+sizeY/2+pass_label->getContentSize().height/2+20);
        doubleAtk->runAction(Sequence::create(DelayTime::create(time),FadeIn::create(0.01f),DelayTime::create(0.5f),FadeOut::create(0.2f),RemoveSelf::create(),NULL));

        para1 = 0.75f;
    }
    //暴击
    if(attackCritic[0]){
        para1 *= 1.5f;
        label2->critic = 1;
    }
    else{
        label2->critic = 0;
    }
    //格挡
    if(attackBlock[0]){
        para1 *= 0.75f;
        CCLOG("攻击被格挡!");
        if(is_unlock_skill){
            if(special_type[0].first==9){//格挡被动,如果是双击写attackmove里
                pass_hit_time++;
                auto blocklabel = Label::createWithTTF("格挡+1", "字体/华文新魏.TTF", 30);
                blocklabel->setPosition(pos.x+sizeX/2+blocklabel->getContentSize().width/2+50,pos.y);
                this->getParent()->addChild(blocklabel,7,DAMAGE_BLOCK_TAG1+tag);
                blocklabel->setColor(Color3B::ORANGE);
                blocklabel->setOpacity(0);
                blocklabel->runAction(Sequence::create(DelayTime::create(time),FadeIn::create(0.01f),DelayTime::create(0.5f),FadeOut::create(0.2f),RemoveSelf::create(),NULL));
            }
        }
        auto blocklabel = Label::createWithTTF("格挡", "字体/华文新魏.TTF", 30);
        blocklabel->setColor(Color3B::GREEN);
        blocklabel->setOpacity(0);
        this->getParent()->addChild(blocklabel,7,DAMAGE_BLOCK_TAG+tag);
        blocklabel->runAction(Sequence::create(DelayTime::create(time),FadeIn::create(0.01f),DelayTime::create(0.5f),FadeOut::create(0.2f),RemoveSelf::create(),NULL));
        blocklabel->setPosition(pos);
        
        auto hudun = Sprite::createWithSpriteFrameName("fx_hudun_002.png");
        this->getParent()->addChild(hudun,6,DAMAGE_BLOCK_TAG*2+tag);
        hudun->setOpacity(0);
        hudun->runAction(Sequence::create(DelayTime::create(time),FadeIn::create(0.01f),DelayTime::create(0.5f),FadeOut::create(0.2f),RemoveSelf::create(),NULL));
        hudun->setPosition(pos);
    }
    //加载
    char str[50];
    CCLOG("此次伤害基数为%f",getDamage[0]*para1);
    sprintf(str, "%d",(int)((getDamage[0])*para1));
    label2->setString(str);
    label2->setPosition(pos.x-label2->getContentSize().width/2,pos.y+sizeY/2+20);
    //---------------------
    this->getParent()->addChild(label2, 7,DAMAGEFONT_TAG+tag);
    if(pass_type==1)
        label2->setTag(DAMAGEFONT_DOUBLE_TAG+tag);

    auto seq = Sequence::create(DelayTime::create(time),CallFuncN::create(CC_CALLBACK_1(CompeteRole::addFont, this)),NULL);
    label2->runAction(seq);
    label2->setOpacity(0);

//-----------------------
    
}
void CompeteRole::addFont(Ref* pSender){
    LabelAtlas* label1 = (LabelAtlas*)pSender;
    label1->setOpacity(255);
    if(label1->critic){
        CCLOG("攻击暴击!");
        cocosbuilder::NodeLoaderLibrary *ccNodeLoaderLibrary = cocosbuilder::NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
        cocosbuilder::CCBReader *ccbReader = new cocosbuilder::CCBReader(ccNodeLoaderLibrary);
        //ccbReader->autorelease();
        auto ccbCriticbg = ccbReader->readNodeGraphFromFile("ccb-buff/showCritic.ccbi", this);
        
        this->getParent()->addChild(ccbCriticbg,6,DAMAGE_CRITIC_TAG+tag);
        
        ccbCriticbg->runAction(Sequence::create(DelayTime::create(0.5f),FadeOut::create(0.2f),RemoveSelf::create(),NULL));
        ccbCriticbg->setPosition(pos.x,pos.y+sizeY/2+20+label1->getContentSize().height/2);

    }
    label1->runAction(Sequence::create(DelayTime::create(0.4f),FadeOut::create(0.15f),RemoveSelf::create(),NULL));
    float damage;
    sscanf(label1->getString().c_str(), "%f",&damage);
    curHP -= floor(damage);
    log("现剩余血量:%d",(int)curHP);
    if(floor(curHP)<=0){
        dieOut();
    }

    
}
/*void CompeteRole::attackDamage(vector<float> damage, bool isDouble, vector<int> isCritic,vector<int> isblock, float time,int pass_type){
 attackCritic = isCritic;
 attackDouble = isDouble;
 attackBlock = isblock;
 getDamage = damage;
 //CCLOG("此次伤害基数为%f",getDamage[0]);
 
 attackDoubleIndex = 0;//双击计数器
 auto removeFunc =  CallFuncN::create(CC_CALLBACK_1(CompeteRole::addFont, this));
 //被动文字-----------------
 auto pass_label = Label::createWithTTF("", "字体/华文新魏.TTF", 30);
 pass_label->setColor(Color3B::YELLOW);
 if(pass_type==2){
 pass_label->setString("穿透");
 }
 //pass_label->setPosition(-pass_label->getContentSize().width/2,sizeY+pass_label->getContentSize().height/2+20);
 pass_label->setPosition(pos.x-sizeX/2-pass_label->getContentSize().width/2,pos.y+sizeY/2+pass_label->getContentSize().height/2+20);
 
 pass_label->setOpacity(0);
 getParent()->addChild(pass_label,8);
 pass_label->runAction(Sequence::create(DelayTime::create(time),FadeIn::create(0.01f),DelayTime::create(0.5f),FadeOut::create(0.2f),RemoveSelf::create(),NULL));
 
 
 //------------------------
 
 auto sq=Sequence::create(DelayTime::create(time),removeFunc, NULL);
 runAction(sq);
 if(attackDouble){
 auto sq1=Sequence::create(DelayTime::create(time+0.9f),removeFunc, NULL);
 runAction(sq1);
 }
 
}

void CompeteRole::addFont(Ref* pSender)
{
    auto label2 = LabelAtlas::create("0123456789-", "fonts/number_red.png",49,88,'0');
    //TO DO....估计改成player也行
    this->getParent()->addChild(label2, 7,DAMAGEFONT_TAG+tag);
    float para1 = 1.0f;
    if(attackDouble&&attackDoubleIndex==1){
        auto doubleAtk = Label::createWithSystemFont("双击", "字体/华文新魏.TTF", 30);
        doubleAtk->setColor(Color3B::MAGENTA);
        doubleAtk->enableOutline(Color4B::MAGENTA,1);
        doubleAtk->enableShadow();
        label2->addChild(doubleAtk);
        doubleAtk->setPosition(-40,doubleAtk->getContentSize().height/2);
        para1 = 0.75f;
    }
    //暴击
    if(attackCritic[attackDoubleIndex]){
        para1 *= 1.5f;
        CCLOG("攻击暴击!");
        cocosbuilder::NodeLoaderLibrary *ccNodeLoaderLibrary = cocosbuilder::NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
        cocosbuilder::CCBReader *ccbReader = new cocosbuilder::CCBReader(ccNodeLoaderLibrary);
        //ccbReader->autorelease();
        auto ccbCriticbg = ccbReader->readNodeGraphFromFile("ccb-buff/showCritic.ccbi", this);

        this->getParent()->addChild(ccbCriticbg,6,DAMAGE_CRITIC_TAG+tag);
    
        ccbCriticbg->runAction(Sequence::create(DelayTime::create(0.5f),FadeOut::create(0.2f),RemoveSelf::create(),NULL));
        ccbCriticbg->setPosition(this->getPositionX(),this->getPositionY()+sizeY/2+20+label2->getContentSize().height/2);
    }
    //格挡
    if(attackBlock[attackDoubleIndex]){
        para1 *= 0.75f;
        CCLOG("攻击被格挡!");
        if(is_unlock_skill){
            if(special_type[0].first==9){//格挡被动,如果是双击写attackmove里
                pass_hit_time++;
                auto blocklabel = Label::createWithTTF("格挡+1", "字体/华文新魏.TTF", 30);
                blocklabel->setPosition(pos.x+sizeX/2+blocklabel->getContentSize().width/2+50,pos.y);
                this->getParent()->addChild(blocklabel,7);
                blocklabel->setColor(Color3B::ORANGE);
                blocklabel->runAction(Sequence::create(DelayTime::create(0.5f),FadeOut::create(0.2f),RemoveSelf::create(),NULL));
            }
        }
        auto blocklabel = Label::createWithTTF("格挡", "字体/华文新魏.TTF", 30);
        blocklabel->setColor(Color3B::GREEN);
        this->getParent()->addChild(blocklabel,7,DAMAGE_BLOCK_TAG+tag);
        blocklabel->runAction(Sequence::create(DelayTime::create(0.5f),FadeOut::create(0.2f),RemoveSelf::create(),NULL));
        blocklabel->setPosition(this->getPosition());
        
        auto hudun = Sprite::createWithSpriteFrameName("fx_hudun_002.png");
        this->getParent()->addChild(hudun,6,DAMAGE_BLOCK_TAG*2+tag);
        hudun->runAction(Sequence::create(DelayTime::create(0.5f),FadeOut::create(0.2f),RemoveSelf::create(),NULL));
        hudun->setPosition(this->getPosition());
    }

    char str[50];
    CCLOG("此次伤害基数为%f",getDamage[attackDoubleIndex]);
    sprintf(str, "%d",(int)((getDamage[attackDoubleIndex])*para1));
    curHP -= floor(getDamage[attackDoubleIndex]*para1);
    log("现剩余血量:%d",(int)curHP);
    label2->setString(str);
    label2->setPosition(this->getPositionX()-label2->getContentSize().width/2,this->getPositionY()+sizeY/2+20);
//---------------------
    auto fade = FadeOut::create(0.2f);
    auto seq = Sequence::create(DelayTime::create(0.5f),fade,RemoveSelf::create(),NULL);
    //CCAction* repeat = CCRepeat::create( seq );
    label2->runAction(seq);
    //TO DO...加死亡
    if(floor(curHP)<=0){
        dieOut();
    }
    attackDoubleIndex++;

}
*/
void CompeteRole::dieOut(){
    runAction(FadeOut::create(0.3f));
    getChildByTag(HERO_FRAME_TAG)->runAction(FadeOut::create(0.3f));
    getChildByTag(healthBar_Tag)->runAction(FadeOut::create(0.3f));
    getChildByTag(healthbarbg_tag)->runAction(FadeOut::create(0.3f));
    getChildByTag(NAME_TAG)->runAction(FadeOut::create(0.3f));
    unschedule(schedule_selector(CompeteRole::healthBarLogic));
    //this->removeAllChildren();
}
void CompeteRole::recover(){
    setOpacity(255);
    getChildByTag(HERO_FRAME_TAG)->setOpacity(255);
    getChildByTag(healthBar_Tag)->setOpacity(255);
    getChildByTag(healthbarbg_tag)->setOpacity(255);
    getChildByTag(NAME_TAG)->setOpacity(255);

    stopAllActions();
    //TO DO....返回原本的位置
    setPosition(pos);
    setRotation(0);
    curHP = totalHp;
    healthBar->setPercentage(((float)curHP/(float)totalHp) *100);

    schedule(schedule_selector(CompeteRole::healthBarLogic));
}
void CompeteRole::changeHealth(Ref* pSender){
    
    healthBar->setPercentage(curHP/totalHp*100);
    //TO DO...加死亡
    if(floor(curHP)<=0){
        dieOut();
    }
    
}
/*void CompeteRole::showCritic(cocos2d::Ref* pSender){
    cocosbuilder::NodeLoaderLibrary *ccNodeLoaderLibrary = cocosbuilder::NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
    cocosbuilder::CCBReader *ccbReader = new cocosbuilder::CCBReader(ccNodeLoaderLibrary);
    //ccbReader->autorelease();
    auto ccbCriticbg = ccbReader->readNodeGraphFromFile("ccb-buff/showCritic.ccbi", this);
    ccbCriticbg->setOpacity(0);
    this->getParent()->addChild(ccbCriticbg,6);
    ccbCriticbg->runAction(Sequence::create(DelayTime::create(0.5f),FadeOut::create(0.2f),RemoveSelf::create(),NULL));
    ccbCriticbg->setPosition(this->getPositionX(),this->getPositionY()+sizeY/2+20+88/2);
    //ccbCriticbg->setPosition(this->getPositionX(),this->getPositionY()+sizeY/2+20+label2->getContentSize().height/2);
    
}
*/

void CompeteRole::setHp(){}
void CompeteRole::whetherDead(){}
CompeteRole::~CompeteRole(){
    CCLOG("析构函数");
}

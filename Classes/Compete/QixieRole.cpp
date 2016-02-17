#include "Compete/QixieRole.h"
#include "Compete/CompeteRole.h"
#include "Compete/CompeteLayer.h"
#include "GameStat.h"

#include "cocos-ext.h"
#include <cocosbuilder/CocosBuilder.h>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
//封装精灵
bool QixieRole::init(GongChengType hero, CompeteLayer *layer){
    if(!CCSprite::init()){
        return false;
    }
    
    player=layer;
    mhero=hero;
    changeHp=0;
    resName=hero.engname;
    string heroPicPath ="qixie/"+resName+".png";
    CCLOG("creating sprite name:%s",heroPicPath.c_str());
    //create(heroPicPath);//TO DO...创建Sprite
    initWithFile(heroPicPath);
    auto heroFrame = Sprite::createWithSpriteFrameName("4_btm.png");
    this->addChild(heroFrame,-1,HERO_FRAME_TAG);
    //setScale(0.2f);
    //heroFrame->setScale(0.8f);
    heroFrame->setPosition(this->getContentSize().width/2,this->getContentSize().height/2);
    setScale(0.65f);
    //attack=20;
    attackType = hero.attack_type;
    bullettype = hero.bullet_type;
    position=hero.position;
    resNum=0;
    curHP=hero.durable;
    totalHp=hero.durable;
    dir=0;
    attackcount=0;
    //TO DO...战斗中的属性
    //isDoubleAttack = false;
    //isCritic = false;
    //-------------------------//
    
    /*auto barbg=Sprite::create("barbg.png");//TO DO.....加血条图片
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
    */
    //TO DO...有可能要放到下边
//-----------
    return true;
}
void QixieRole::healthBarLogic(float dt){
    healthBar->setPercentage(((float)curHP/(float)totalHp) *100);
}
void QixieRole::remove()
{
    removeFromParentAndCleanup(true);
}
void QixieRole::attack(bool isSkill,int tag1, vector<int> tag2, bool isDoubleAttack, bool isEnemy, float time)//tag1是攻击方
{
    target_tag = tag2;
    for(auto it = target_tag.begin();it!=target_tag.end();it++){
        if(isEnemy)
            *it+=200;
        else
            *it+=100;
    }
    //delayTime = time;
    //TO DO....攻击动作时间由不同hero决定
    int para = 1;
    if (!isEnemy) {
        para=-1;
    }

    //float distance = abs(player->getChildByTag(target_tag[0])->getPositionY()-player->getChildByTag(tag1)->getPositionY())-sizeY;
    float slope = (para*(player->getChildByTag(target_tag[0])->getPosition()-player->getChildByTag(tag1)->getPosition())).getAngle()*(-para);
    float angle = slope*180/3.1415926;
    if(!isEnemy)
        angle+=2*(90-angle);

    CCLOG("子弹角度为%f,radian:%f",angle,slope);
        //auto moveAct = MoveBy::create(0.5f, Vec2(0,distance*para));//移动过去
        //auto moveActBack = MoveBy::create(0.25f, Vec2(0,-distance*para));
        //auto bounceMove = EaseExponentialInOut::create(moveAct);
        auto rotateAct = RotateBy::create(0.3f, -30*para);
        auto bounce1 = EaseBounceInOut::create(rotateAct);
 
        auto seq1 = Sequence::create(bounce1,bounce1->reverse(), NULL);
        auto seq = Sequence::create(seq1,DelayTime::create(0.2f), NULL);
        runAction(seq);
    
    //目前为所有子弹都是攻击默认单体
    if(bullettype==1){
        cocosbuilder::NodeLoaderLibrary *ccNodeLoaderLibrary = cocosbuilder::NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
        cocosbuilder::CCBReader *ccbReader = new cocosbuilder::CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();
        auto fire_bullet = ccbReader->readNodeGraphFromFile("skill_zhurigong_fly.ccbi", this);
        //ccbCriticbg->runAction(Sequence::create(DelayTime::create(0.5f),FadeOut::create(0.2f),RemoveSelf::create(),NULL));
        player->addChild(fire_bullet,7);
        fire_bullet->setRotation(angle);
        fire_bullet->setPosition(player->getChildByTag(tag1)->getPosition());
        auto seq = Sequence::create(MoveTo::create(0.28f, player->getChildByTag(target_tag[0])->getPosition()),CallFuncN::create(CC_CALLBACK_1(QixieRole::hurtEffect, this)),RemoveSelf::create(),NULL);
        fire_bullet->runAction(seq);
        //seq->setTag(20);
    }

    else if(bullettype==2){
        auto normal_bullet = Sprite::createWithSpriteFrameName("normal_bullet_02.png");
        normal_bullet->setPosition(player->getChildByTag(tag1)->getPosition());
        player->addChild(normal_bullet,7);
        normal_bullet->setRotation(angle);
        normal_bullet->runAction(Sequence::create(MoveTo::create(0.3f, player->getChildByTag(target_tag[0])->getPosition()),CallFuncN::create(CC_CALLBACK_1(QixieRole::hurtEffect, this)),RemoveSelf::create(),NULL));
    }
    
    
}
void QixieRole::hurtEffect(Ref* pSender){
    if(bullettype==1){
        //player->getChildByTag(BULLET_TAG)->removeFromParentAndCleanup(true);
        cocosbuilder::NodeLoaderLibrary *ccNodeLoaderLibrary = cocosbuilder::NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
        cocosbuilder::CCBReader *ccbReader = new cocosbuilder::CCBReader(ccNodeLoaderLibrary);
        ccbReader->autorelease();
        auto zhurigong_effect = ccbReader->readNodeGraphFromFile("skill_zhurigong_hurt.ccbi", this);
        player->addChild(zhurigong_effect,7,10);
        zhurigong_effect->setPosition(player->getChildByTag(target_tag[0])->getPosition());

    }
}

void QixieRole::attackDamage(vector<float> damage, bool isDouble, vector<int> isCritic, float time){
    attackCritic = isCritic;
    attackDouble = isDouble;

    getDamage = damage;
    //CCLOG("此次伤害基数为%f",getDamage[0]);

    attackDoubleIndex = 0;//双击计数器
    auto removeFunc =  CallFuncN::create(CC_CALLBACK_1(QixieRole::addFont, this));

    auto sq=Sequence::create(DelayTime::create(1.1f),removeFunc, NULL);
    runAction(sq);
    if(attackDouble){
        auto sq1=Sequence::create(DelayTime::create(1.1f+0.9f),removeFunc, NULL);
        runAction(sq1);
    }
}

void QixieRole::addFont(Ref* pSender)
{
    auto label2 = LabelAtlas::create("0123456789-", "fonts/number_red.png",49,88,'0');
    //TO DO....估计改成player也行
    this->getParent()->addChild(label2, 7,500);
    float para1 = 1.0f;
    /*if(attackDouble&&attackDoubleIndex==1){
        auto doubleAtk = Label::createWithSystemFont("双击", "宋体", 45);
        label2->addChild(doubleAtk);
        doubleAtk->setPosition(-40,doubleAtk->getContentSize().height/2);
        para1 = 0.75f;
    }
    if(attackCritic[attackDoubleIndex]){
        para1 *= 1.5f;
        CCLOG("攻击暴击!");
        cocosbuilder::NodeLoaderLibrary *ccNodeLoaderLibrary = cocosbuilder::NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
        cocosbuilder::CCBReader *ccbReader = new cocosbuilder::CCBReader(ccNodeLoaderLibrary);
        //ccbReader->autorelease();
        auto ccbCriticbg = ccbReader->readNodeGraphFromFile("ccb-buff/showCritic.ccbi", this);

        this->getParent()->addChild(ccbCriticbg,6);
        ccbCriticbg->runAction(Sequence::create(DelayTime::create(0.5f),FadeOut::create(0.2f),RemoveSelf::create(),NULL));
        ccbCriticbg->setPosition(this->getPositionX(),this->getPositionY()+sizeY/2+20+label2->getContentSize().height/2);
        
    }*/

    char str[50];
    CCLOG("此次伤害基数为%f",getDamage[0]);
    sprintf(str, "%d",(int)((getDamage[0])*para1));
    curHP -= floor(getDamage[0]*para1);
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

void QixieRole::dieOut(){
    runAction(FadeOut::create(0.3f));
    /*getChildByTag(HERO_FRAME_TAG)->runAction(FadeOut::create(0.3f));
    getChildByTag(healthBar_Tag)->runAction(FadeOut::create(0.3f));
    getChildByTag(healthbarbg_tag)->runAction(FadeOut::create(0.3f));

    unschedule(schedule_selector(CompeteRole::healthBarLogic));
     */
    //this->removeAllChildren();
}
void QixieRole::recover(){
    setOpacity(255);
    /*getChildByTag(HERO_FRAME_TAG)->setOpacity(255);
    getChildByTag(healthBar_Tag)->setOpacity(255);
    getChildByTag(healthbarbg_tag)->setOpacity(255);
     */
    stopAllActions();
    //TO DO....返回原本的位置
    setPosition(pos);
    setRotation(0);
    curHP = totalHp;
    /*healthBar->setPercentage(((float)curHP/(float)totalHp) *100);

    schedule(schedule_selector(CompeteRole::healthBarLogic));
     */
}
void QixieRole::changeHealth(Ref* pSender){
    
    healthBar->setPercentage(curHP/totalHp*100);
    //TO DO...加死亡
    if(floor(curHP)<=0){
        dieOut();
    }
    
}

void QixieRole::setHp(){}
void QixieRole::whetherDead(){}
QixieRole::~QixieRole(){
    CCLOG("析构函数");
}

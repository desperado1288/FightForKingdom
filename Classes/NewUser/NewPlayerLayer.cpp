#include "LayerManager/MainScene.h"
#include "LayerManager/ZhengZhanLayer.h"
#include "HeroList/TeamListLayer.h"
#include "Compete/CompeteScene.h"
#include "ModifiedSourceCode/DMenu.h"
#include "ModifiedSourceCode/DControlButton.h"

#include "cocos-ext.h"
#include <ui/CocosGUI.h>

#include "NewPlayerLayer.h"
#include "GameStat.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
// on "init" you need to initialize your instance
bool NewPlayerLayer::init(int type)
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    gongchengtype = type;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("evolution.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("fonts/newBmpFont.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("commonBtn.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("dialog/dialogueRight.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("dialog/dialogueLeft.plist");

    auto listener = EventListenerTouchOneByOne::create();//加入触摸监听器
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(NewPlayerLayer::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    
    auto layercol = LayerColor::create(Color4B(0,0,0,130), visibleSize.width, visibleSize.height);
    //layercol->setPosition(visibleSize.width/2,visibleSize.height/2);
    //this->addChild(layercol,1);
    
    //auto miaoshukuang = Sprite::createWithSpriteFrameName("duihuakuang_shangfang.png");
    auto miaoshubg = Sprite::createWithSpriteFrameName("bg_commonTips.png");
    auto miaoshuwenzi = Label::createWithTTF("", "字体/瘦金体.ttf", 30);
    //miaoshuwenzi->setDimensions(miaoshubg->getContentSize().width*3/4, miaoshubg->getContentSize().height*3/4);
    auto clippingnode = ClippingNode::create();
    clippingnode->setInverted(true);
    this->addChild(clippingnode,1);
    clippingnode->addChild(layercol);
    
    auto herosp = Sprite::create(herobodypath(userteam[0].engname));

    if(type==0){
        active_node = Sprite::createWithSpriteFrameName("btn_dibuzhukong_normal.png");
        active_node->setPosition(visibleSize.width/2,141);
        miaoshubg->setPosition(visibleSize.width/2,260);
        miaoshuwenzi->setString("请玩家点击征战，进行剧情");
        miaoshuwenzi->setPosition(miaoshubg->getPosition());
        clippingnode->setStencil(active_node);
        //边框缩小
        auto framesp = Sprite::create("dialog/边框.png");
        framesp->setPosition(visibleSize.width/2,141+75);
        framesp->setScale(2);
        framesp->setRotation(-90);
        framesp->setAnchorPoint(Vec2(0.5,0.5));
        this->addChild(framesp,3);
        framesp->runAction(Sequence::create(ScaleTo::create(0.5f, 0.5f),NULL));
        //小手
        auto xiaohand = Sprite::create("dialog/小手.png");
        xiaohand->setAnchorPoint(Vec2(0,1));
        xiaohand->setPosition(active_node->getPosition());
        this->addChild(xiaohand,3);
        
    }
    else if(type==1){//指示进入副本
        active_node = Sprite::create("icon_juqingfuben.png");
        active_node->setPosition(visibleSize.width/2,visibleSize.height/3*2);
        miaoshubg->setPosition(active_node->getPositionX(),active_node->getPositionY()-active_node->getContentSize().height);
        miaoshuwenzi->setString("请玩家点击征战，进行剧情");
        miaoshuwenzi->setPosition(miaoshubg->getPosition());
        clippingnode->setStencil(active_node);
        //边框缩小
        auto framesp = Sprite::create("dialog/红色框.png");
        framesp->setPosition(active_node->getPosition());
        framesp->setScale(2);
        framesp->setRotation(-90);
        this->addChild(framesp,3);
        framesp->runAction(Sequence::create(ScaleTo::create(0.5f, 1.0f),NULL));
        //补充一个小手
        auto xiaohand = Sprite::create("dialog/小手.png");
        xiaohand->setPosition(visibleSize.width*2/3,active_node->getPositionY()-30);
        this->addChild(xiaohand,3);
        GameStat::getInstance()->isGongcheng = false;
    }
    else if(type==2){//指示退出征战页
        active_node = Sprite::create("btn_arrow_right_disable.png");
        active_node->setPosition(active_node->getContentSize().width/2,visibleSize.height-207);
        miaoshubg->setPosition(active_node->getPositionX(),active_node->getPositionY()-active_node->getContentSize().height);
        miaoshuwenzi->setString("请玩家返回主界面进行后续操作");
        miaoshuwenzi->setPosition(miaoshubg->getPosition());
        clippingnode->setStencil(active_node);
        //边框缩小
        auto framesp = Sprite::create("dialog/红色框.png");
        framesp->setPosition(active_node->getPosition());
        //framesp->setScale(0.3f);
        framesp->setRotation(-90);
        this->addChild(framesp,3);
        framesp->runAction(Sequence::create(ScaleTo::create(0.5f, 0.2f),NULL));
        //补充一个小手
        auto xiaohand = Sprite::create("dialog/小手.png");
        xiaohand->setPosition(active_node->getPosition());
        this->addChild(xiaohand,3);
        GameStat::getInstance()->isGongcheng = false;
    }
    else if(type==3){//进入阵容页
        active_node = Sprite::createWithSpriteFrameName("btn_dibuzhukong_normal.png");
        active_node->setPosition(visibleSize.width/2-active_node->getContentSize().width,141);
        miaoshubg->setPosition(visibleSize.width/2,260);
        miaoshuwenzi->setString("有新英雄加入，请玩家点击阵容");
        miaoshuwenzi->setPosition(miaoshubg->getPosition());
        clippingnode->setStencil(active_node);
        //边框缩小
        auto framesp = Sprite::create("dialog/边框.png");
        framesp->setPosition(visibleSize.width/2-active_node->getContentSize().width,141+75);
        framesp->setScale(2);
        framesp->setRotation(-90);
        framesp->setAnchorPoint(Vec2(0.5,0.5));
        this->addChild(framesp,3);
        framesp->runAction(Sequence::create(ScaleTo::create(0.5f, 0.5f),NULL));
        //小手
        auto xiaohand = Sprite::create("dialog/小手.png");
        xiaohand->setAnchorPoint(Vec2(0,1));
        xiaohand->setPosition(active_node->getPosition());
        this->addChild(xiaohand,3);
        
    }

    else if(type==4){//在阵容页，选择新英雄
        //if(new_stage==1){
        active_node = Sprite::create(herobodypath(userteam[0].engname));
        active_node->setPosition(visibleSize.width/2,visibleSize.height/2);
        miaoshubg->setPosition(visibleSize.width/2,active_node->getPositionY()-120);
        miaoshuwenzi->setString("英雄等级随玩家等级提高，不需要再进行升级");
        miaoshuwenzi->setPosition(miaoshubg->getPosition());
        clippingnode->setStencil(active_node);
        //边框缩小
        auto framesp = Sprite::create("dialog/边框.png");
        framesp->setPosition(visibleSize.width/2-active_node->getContentSize().width,141+75);
        framesp->setScale(5);
        //framesp->setRotation(-90);
        framesp->setAnchorPoint(Vec2(0.5,0.5));
        this->addChild(framesp,3);
        framesp->runAction(Sequence::create(ScaleTo::create(0.5f, 3),NULL));
        //小手
        auto xiaohand = Sprite::create("dialog/小手.png");
        xiaohand->setAnchorPoint(Vec2(0,1));
        xiaohand->setPosition(active_node->getPosition());
        this->addChild(xiaohand,3);
        
    }
    else if(type==5){//只是看被动技能
        active_node = Sprite::create("icon_juqingfuben.png");
        active_node->setPosition(visibleSize.width/2,visibleSize.height-88-herosp->getContentSize().height-40-25-50-60-45-100);
        miaoshubg->setPosition(active_node->getPositionX(),active_node->getPositionY()-active_node->getContentSize().height);
        miaoshuwenzi->setString("每个英雄都具有被动技能，解锁需要一定条件，其中部分英雄不需任何条件");
        miaoshuwenzi->setDimensions(550, 100);
        miaoshuwenzi->setPosition(miaoshubg->getPosition());
        clippingnode->setStencil(active_node);
        //边框缩小
        auto framesp = Sprite::create("dialog/红色框.png");
        framesp->setPosition(active_node->getPosition());
        framesp->setScale(2);
        framesp->setRotation(-90);
        this->addChild(framesp,3);
        framesp->runAction(Sequence::create(ScaleTo::create(0.5f, 0.8f),NULL));
        //补充一个小手
        auto xiaohand = Sprite::create("dialog/小手.png");
        xiaohand->setPosition(visibleSize.width*2/3,active_node->getPositionY()-30);
        this->addChild(xiaohand,3);
    }
    else if(type==6){//英雄info中指示点击解锁按钮
        active_node = Sprite::createWithSpriteFrameName("btn_lingxing_normal.png");
        active_node->setPosition(visibleSize.width*2/3+50,visibleSize.height-88-herosp->getContentSize().height-40-25-50-60-50-100);
        miaoshubg->setPosition(visibleSize.width/2,active_node->getPositionY()+50);
        miaoshuwenzi->setString("请点击解锁被动技能");
        miaoshuwenzi->setPosition(miaoshubg->getPosition());
        clippingnode->setStencil(active_node);
        //边框缩小
        auto framesp = Sprite::create("dialog/边框.png");
        framesp->setPosition(active_node->getPositionX()+100,active_node->getPositionY());
        framesp->setScale(2);
        //framesp->setRotation(-90);
        //framesp->setAnchorPoint(Vec2(0.5f,0.5f));
        this->addChild(framesp,3);
        framesp->runAction(Sequence::create(ScaleTo::create(0.5f, 0.5f),NULL));
        //小手
        auto xiaohand = Sprite::create("dialog/小手.png");
        xiaohand->setAnchorPoint(Vec2(0,1));
        xiaohand->setPosition(active_node->getPosition());
        this->addChild(xiaohand,3);
        
    }
    else if(type==7){
        active_node = Sprite::createWithSpriteFrameName("btn_dibuzhukong_normal.png");
        active_node->setPosition(visibleSize.width/2,141);
        miaoshubg->setPosition(visibleSize.width/2,260);
        miaoshuwenzi->setString("恭喜您成功解锁第一个被动技能~ 请进行后边的剧情");
        miaoshuwenzi->setDimensions(500, 80);
        miaoshuwenzi->setPosition(miaoshubg->getPosition());
        clippingnode->setStencil(active_node);
        //边框缩小
        auto framesp = Sprite::create("dialog/边框.png");
        framesp->setPosition(visibleSize.width/2,141+75);
        framesp->setScale(2);
        framesp->setRotation(-90);
        framesp->setAnchorPoint(Vec2(0.5,0.5));
        this->addChild(framesp,3);
        framesp->runAction(Sequence::create(ScaleTo::create(0.5f, 0.5f),NULL));
        //小手
        auto xiaohand = Sprite::create("dialog/小手.png");
        xiaohand->setAnchorPoint(Vec2(0,1));
        xiaohand->setPosition(active_node->getPosition());
        this->addChild(xiaohand,3);
        
    }

    else if(type==8){//基本和type==1一样，可直接用1
        active_node = Sprite::create("icon_juqingfuben.png");
        active_node->setPosition(visibleSize.width/2,visibleSize.height/3*2);
        miaoshubg->setPosition(active_node->getPositionX(),active_node->getPositionY()-active_node->getContentSize().height);
        miaoshuwenzi->setString("请点击副本进入剧情~");
        miaoshuwenzi->setPosition(miaoshubg->getPosition());
        clippingnode->setStencil(active_node);
        //边框缩小
        auto framesp = Sprite::create("dialog/红色框.png");
        framesp->setPosition(active_node->getPosition());
        framesp->setScale(2);
        framesp->setRotation(-90);
        this->addChild(framesp,3);
        framesp->runAction(Sequence::create(ScaleTo::create(0.5f, 1.0f),NULL));
        //补充一个小手
        auto xiaohand = Sprite::create("dialog/小手.png");
        xiaohand->setPosition(visibleSize.width*2/3,active_node->getPositionY()-30);
        this->addChild(xiaohand,3);
        GameStat::getInstance()->isGongcheng = false;
    }
    else if(type==9){//返回主页，抽英雄
        //if(new_stage==1){
        active_node = Sprite::createWithSpriteFrameName("btn_dibuzhukong_normal.png");
        active_node->setPosition(active_node->getContentSize().width/2,141);
        miaoshubg->setPosition(visibleSize.width/2,active_node->getPositionY()+120);
        miaoshuwenzi->setString("请您回城查看新内容");
        miaoshuwenzi->setPosition(miaoshubg->getPosition());
        clippingnode->setStencil(active_node);
        //边框缩小
        auto framesp = Sprite::create("dialog/边框.png");
        framesp->setPosition(active_node->getPositionX(),141+75);
        framesp->setScale(2);
        framesp->setRotation(-90);
        framesp->setAnchorPoint(Vec2(0.5,0.5));
        this->addChild(framesp,3);
        framesp->runAction(Sequence::create(ScaleTo::create(0.5f, 0.5f),NULL));
        //小手
        auto xiaohand = Sprite::create("dialog/小手.png");
        xiaohand->setAnchorPoint(Vec2(0,1));
        xiaohand->setPosition(active_node->getPosition());
        this->addChild(xiaohand,3);
        
    }
    else if(type==10){//在主页抽取英雄
        //if(new_stage==1){
        active_node = Sprite::create("dianjiangtai.PNG");
        active_node->setPosition(visibleSize.width/4,visibleSize.height/2);
        miaoshubg->setPosition(visibleSize.width/2,active_node->getPositionY()-120);
        miaoshuwenzi->setString("想在这乱世中立足，先招募一名部将吧！");
        miaoshuwenzi->setPosition(miaoshubg->getPosition());
        clippingnode->setStencil(active_node);
        //边框缩小
        auto framesp = Sprite::create("dialog/边框.png");
        framesp->setPosition(active_node->getPositionX(),active_node->getPositionY()+120);
        framesp->setScale(3);
        framesp->setAnchorPoint(Vec2(0.5,0.5));
        framesp->setRotation(-90);
        this->addChild(framesp,3);
        framesp->runAction(Sequence::create(ScaleTo::create(0.5f, 0.8f),NULL));
        //小手
        auto xiaohand = Sprite::create("dialog/小手.png");
        xiaohand->setAnchorPoint(Vec2(0,1));
        xiaohand->setPosition(active_node->getPosition());
        this->addChild(xiaohand,3);
        
    }

    
    //node放到一起变亮
    auto bright_node = Node::create();
    auto spbg = Sprite::createWithSpriteFrameName("bg_commonTips.png");
    spbg->setPosition(miaoshubg->getPosition());
    bright_node->addChild(spbg);
    bright_node->setPosition(Vec2::ZERO);
    bright_node->addChild(active_node);
    clippingnode->setStencil(bright_node);
    miaoshuwenzi->setColor(Color3B::WHITE);
    this->addChild(miaoshuwenzi,3);
    this->addChild(miaoshubg,2);
    
    
    //明亮部分边框
    
    return true;
}
void NewPlayerLayer::back(Ref* pSender, Control::EventType event){

    //TO DO....恢复上一层的touch
    this->removeFromParentAndCleanup(true);
}
bool NewPlayerLayer::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    if(active_node->getBoundingBox().containsPoint(touch->getLocation()))
    {
        /*auto _ZhengZhanLayer = ZhengZhanLayer::create();
        if(_ZhengZhanLayer)
            this->getParent()->addChild(_ZhengZhanLayer,3,ZHENGZHAN_LAYER_TAG);
         */
        
        if(new_stage==1){
            if(new_step==0){
                auto _newPlayerLayer = new NewPlayerLayer();
                    _newPlayerLayer->init(6);
                this->getParent()->addChild(_newPlayerLayer,4,NEWPLAYER_LAYER_TAG);

            }
        }
        removeFromParentAndCleanup(true);
        return false;
    }
    
    
    return true;
}


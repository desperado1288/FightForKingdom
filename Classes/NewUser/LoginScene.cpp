#include "LayerManager/MainScene.h"
#include "HeroList/TeamListLayer.h"
#include "NewUser/LoginScene.h"
#include "NewUser/SelectZhujiaoLayer.h"
#include "cocos-ext.h"
#include <cocosbuilder/CocosBuilder.h>
#include <ui/CocosGUI.h>
#include "PopupLayer.h"
#include "ShieldLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::extension;
using namespace cocos2d::ui;

bool LoginScene::init(){
    if(!Scene::init()){
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto listener = EventListenerTouchOneByOne::create();//加入触摸监听器
    //listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LoginScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LoginScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LoginScene::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("commonBtn.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("newCommon.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("newBattle.plist");

    auto bgsp = Sprite::create("denglujiemian.jpg");
    bgsp->setPosition(visibleSize.width/2,visibleSize.height/2);
    this->addChild(bgsp,1);
    //加上下边框
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gameFrame.plist");
    auto upFrame = Sprite::createWithSpriteFrameName("icon_shanghuabian.png");
    auto downFrame = Sprite::createWithSpriteFrameName("icon_xiahuabian.png");
    downFrame->setPosition(visibleSize.width/2,downFrame->getContentSize().height/2);
    upFrame->setPosition(visibleSize.width/2,visibleSize.height-upFrame->getContentSize().height/2);
    this->addChild(downFrame,2);
    this->addChild(upFrame,2);
    //加入所有底层功能
    //游戏标题
    auto title = Label::createWithTTF("三国风云", "7=蒙纳漫画体.ttf", 60);
    title->setColor(Color3B::BLACK);
    title->setPosition(visibleSize.width/2,visibleSize.height*3/4);
    this->addChild(title,2);
    
    //三个部分
    auto textfield_frame = Sprite::create("bg_yellow_msg.png");
    auto sp1 = Node::create();
    this->addChild(sp1,3,TEXTFIELD_TAG);
    auto sp2 = Node::create();
    this->addChild(sp2,3,TEXTFIELD_TAG+1);
    sp1->setPosition(visibleSize.width/2,visibleSize.height/2+80);
    sp2->setPosition(visibleSize.width/2,visibleSize.height/2-textfield_frame->getContentSize().height-30+80);

    //用户名，密码
    auto listener1 = EventListenerKeyboard::create();
    listener1->onKeyPressed = CC_CALLBACK_2(LoginScene::onKeyPressed, this);
    listener1->onKeyReleased = CC_CALLBACK_2(LoginScene::onKeyReleased, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
    
    
    username = TextFieldTTF::textFieldWithPlaceHolder("<click for input username>", "7=蒙纳漫画体.ttf", 30);
    //username->setPosition(visibleSize.width/2,visibleSize.height/2+80);
    password = TextFieldTTF::textFieldWithPlaceHolder("<click for input password>", "7=蒙纳漫画体.ttf", 30);
    //加入回收计数器
    username->retain();
    password->retain();
    //输入框---边框
    //password->setPosition(visibleSize.width/2,visibleSize.height/2-textfield_frame->getContentSize().height-30+80);
    auto textfield_frame1 = Sprite::create("bg_yellow_msg.png");
    //textfield_frame->setPosition(visibleSize.width/2,visibleSize.height/2+80);
    //textfield_frame1->setPosition(visibleSize.width/2,visibleSize.height/2-textfield_frame->getContentSize().height-30+80);
    sp1->addChild(textfield_frame,2);
    sp2->addChild(textfield_frame1,2);
    sp1->addChild(username,3,USERNAME_TEXTFIELD_TAG);
    sp2->addChild(password,3,PASSWORD_TEXTFIELD_TAG);
    password->setSecureTextEntry(true);
    
    //用户名、密码标题
    auto usernametitle = Label::createWithTTF("用户名", "字体/方正流行体简体.ttf", 35);
    auto passwordtitle = Label::createWithTTF("密码", "字体/方正流行体简体.ttf", 35);
    usernametitle->setPosition(-220,0);
    passwordtitle->setPosition(-220,0);
    sp1->addChild(usernametitle,3);
    sp2->addChild(passwordtitle,3);

    
    //开始游戏按钮
    auto btnsp = Sprite::createWithSpriteFrameName("btn_main_normal.png");
    auto btn_begin = Scale9Sprite::createWithSpriteFrameName("btn_main_normal.png");
    auto btn_label = Label::createWithTTF("Start", "7=蒙纳漫画体.ttf", 30);
    auto btnbegin = ControlButton::create(btn_label,btn_begin);
    btnbegin->setPreferredSize(btnsp->getContentSize());
    btnbegin->setScaleRatio(0.85f);
    btnbegin->setPosition(visibleSize.width/2, visibleSize.height/2-textfield_frame->getContentSize().height*2-60+80);
    this->addChild(btnbegin,3,TEXTFIELD_TAG+2);
    //btn_label->setPosition(btnbegin->getContentSize().width/2,btn_begin->getContentSize().height/2-10);
    btnbegin->addTargetWithActionForControlEvents(this, cccontrol_selector(LoginScene::startGame), Control::EventType::TOUCH_UP_INSIDE);
    return true;
}
void LoginScene::onKeyPressed(EventKeyboard::KeyCode keyCode,cocos2d::Event* event){
    log("Key with keycode %d pressed", keyCode);

}
void LoginScene::onKeyReleased(EventKeyboard::KeyCode keyCode,cocos2d::Event* event){
    log("Key with keycode %d pressed", keyCode);

}
void LoginScene::startGame(Ref* pSender, Control::EventType event){
    Size visibleSize = Director::getInstance()->getVisibleSize();

    int username_length = username->getStringLength();
    int password_length = password->getStringLength();
    //记录username,新的
    GameStat::getInstance()->userName = username->getString();
    log("密码长度为%d",password_length);
    if(password_length==0||username_length==0||password_length==26||username_length==26){
        log("密码或用户名为空");
        auto _popLayer = new PopupLayer();
        _popLayer->init(5);
        if(_popLayer){
            this->addChild(_popLayer,6);
        }
    }
    else{
        //三个部分移动
        if(GameStat::getInstance()->readFromUserInfo("Users_Info", documentpath1.c_str())){//为已注册账号
            auto sp1 = this->getChildByTag(TEXTFIELD_TAG);
            auto sp2 = this->getChildByTag(TEXTFIELD_TAG+1);
            auto sp3 = this->getChildByTag(TEXTFIELD_TAG+2);
            sp1->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(-visibleSize.width,0)),RemoveSelf::create(),NULL));
            sp2->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(visibleSize.width,0)),RemoveSelf::create(),NULL));
            sp3->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(-visibleSize.width,0)),CallFuncN::create(CC_CALLBACK_1(LoginScene::gotoMainScene,this)),RemoveSelf::create(),NULL));

        }
        else{//新账号
        char sql[1024];
        sprintf(sql,"\"%s\", 1, 0, 0,100,0,0,0,0,0,0,0,0,0",GameStat::getInstance()->userName.c_str());
        GameStat::getInstance()->saveToSQLite("Users_Info", documentpath1.c_str(), sql);
        auto sp1 = this->getChildByTag(TEXTFIELD_TAG);
        auto sp2 = this->getChildByTag(TEXTFIELD_TAG+1);
        auto sp3 = this->getChildByTag(TEXTFIELD_TAG+2);
        sp1->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(-visibleSize.width,0)),RemoveSelf::create(),NULL));
        sp2->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(visibleSize.width,0)),RemoveSelf::create(),NULL));
        sp3->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(-visibleSize.width,0)),CallFuncN::create(CC_CALLBACK_1(LoginScene::selectHero,this)),RemoveSelf::create(),NULL));

        }
    }


}
void LoginScene::selectHero(Ref* pSender){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto _shieldLayer = new ShieldLayer();
    _shieldLayer->init(0);
    if(_shieldLayer){
        this->addChild(_shieldLayer,2);
    }
    auto _selectHero = SelectZhujiaoLayer::create();
    if(_selectHero){
        this->addChild(_selectHero,3);
    }
    
    //进入选主角界面
    /*string zhujiao1 = "zhujiao1";
    string zhujiao2 = "zhujiao2";
    string zhujiao3 = "zhujiao3";
    
    auto hero1 = Sprite::create(herobodypath(zhujiao1));
    auto hero2 = Sprite::create(herobodypath(zhujiao2));
    auto hero3 = Sprite::create(herobodypath(zhujiao3));
    hero1->setPosition(visibleSize.width/3-50,visibleSize.height*2/3-50);
    hero2->setPosition(visibleSize.width/3*2+50,visibleSize.height*2/3-50);
    hero3->setPosition(visibleSize.width/2,visibleSize.height*2/3-hero1->getContentSize().height);
    this->addChild(hero1,3,ZHUJIAO_TAG);
    this->addChild(hero2,3,ZHUJIAO_TAG+1);
    this->addChild(hero3,3,ZHUJIAO_TAG+2);
    
    hero1->setScale(2);
    hero2->setScale(2);
    hero3->setScale(2);
    hero2->setOpacity(0);
    hero3->setOpacity(0);
    hero1->runAction(EaseBounceIn::create(ScaleTo::create(0.5f, 0.8f)));
    hero2->runAction(Sequence::create(DelayTime::create(0.5f),FadeIn::create(0.01f),EaseBounceIn::create(ScaleTo::create(0.5f, 0.8f)),NULL));
    hero3->runAction(Sequence::create(DelayTime::create(1.0f),FadeIn::create(0.01f),EaseBounceIn::create(ScaleTo::create(0.5f, 0.8f)),CallFuncN::create(CC_CALLBACK_1(LoginScene::showWenzi,this)),NULL));
*/
}
void LoginScene::showWenzi(Ref* pSender){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    string zhujiao1 = "zhujiao1";
    
    auto hero1 = Sprite::create(herobodypath(zhujiao1));

    auto typelabel1 = Label::createWithTTF("战神", "字体/瘦金体.ttf", 30);
    auto label1 = Label::createWithTTF("特点:高暴击，高血防", "字体/瘦金体.ttf", 30);
    auto typelabel2 = Label::createWithTTF("游侠", "字体/瘦金体.ttf", 30);
    auto label2 = Label::createWithTTF("特点:高敏捷，攻击范围大", "字体/瘦金体.ttf", 30);
    auto typelabel3 = Label::createWithTTF("鬼才", "字体/瘦金体.ttf", 30);
    auto label3 = Label::createWithTTF("特点:高谋略，高暴击，全体伤害", "字体/瘦金体.ttf", 30);
    typelabel1->setPosition(visibleSize.width/3,visibleSize.height*2/3-50-hero1->getContentSize().height/2);
    label1->setPosition(visibleSize.width/3,visibleSize.height*2/3-50-hero1->getContentSize().height/2-40);

    typelabel2->setPosition(visibleSize.width/3*2,visibleSize.height*2/3-50-hero1->getContentSize().height/2);
    label2->setPosition(visibleSize.width/3*2,visibleSize.height*2/3-50-hero1->getContentSize().height/2-40);
    typelabel3->setPosition(visibleSize.width/2,visibleSize.height*2/3-hero1->getContentSize().height*3/2);
    label3->setPosition(visibleSize.width/2,visibleSize.height*2/3-hero1->getContentSize().height*3/2-40);
    this->addChild(typelabel1,3);
    this->addChild(label1,3);
    this->addChild(typelabel2,3);
    this->addChild(label2,3);
    this->addChild(typelabel3,3);
    this->addChild(label3,3);


}
bool LoginScene::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){
    
    for(int i = 0;i<2;i++){
        if((this->getChildByTag(TEXTFIELD_TAG+i)->getChildByTag(USERNAME_TEXTFIELD_TAG+i)->getBoundingBox()).containsPoint(this->getChildByTag(101+i)->convertToNodeSpace(touch->getLocation()))){
            log("弹出键盘");
            ((TextFieldTTF*)this->getChildByTag(TEXTFIELD_TAG+i)->getChildByTag(USERNAME_TEXTFIELD_TAG+i))->attachWithIME();
            break;
        }
    }
    return false;
}
void LoginScene::onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* event){
    
}
void LoginScene::onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event){
    
}


void LoginScene::bottomFunction(Ref* pSender){

}
void LoginScene::gotoMainScene(Ref* pSender){
    auto _MainScene = MainScene::create();
    if(_MainScene){
        GameStat::getInstance()->new_player_stage = 0;
        Director::getInstance()->replaceScene(_MainScene);
    }

}

void LoginScene::showZhenXing(Ref* pSender){
}


void LoginScene::showTeamList(Ref* pSender){
}





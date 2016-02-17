#include "MainLayer.h"
#include "GameStat.h"
#include "Compete/CompeteScene.h"
#include "Compete/CompeteLayer.h"
#include "Compete/CompeteStatus.h"

USING_NS_CC;

bool CompeteScene::init(){
    if(!Scene::init()){
        return false;
    }
    
    auto _CompeteLayer = CompeteLayer::create();
    auto _CompeteStatus = CompeteStatus::create();
    if(_CompeteLayer){
        _CompeteLayer->setDelegator(_CompeteStatus);
        this->addChild(_CompeteLayer,1);
    }
    if(_CompeteStatus)
        this->addChild(_CompeteStatus,2);
    
    
    
    return true;
}
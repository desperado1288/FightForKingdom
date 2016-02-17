#ifndef __COMPETE_SCENE_H__
#define __COMPETE_SCENE_H__

#include "cocos2d.h"

class CompeteScene : public cocos2d::Scene
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    CREATE_FUNC(CompeteScene);
};

#endif // __COMPETE_SCENE_H__

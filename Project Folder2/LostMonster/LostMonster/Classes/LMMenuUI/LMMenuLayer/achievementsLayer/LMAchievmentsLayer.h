//
//  LMAchievmentsLayer.h
//  LMGame
//
//  Created by Pavitra on 03/09/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef LMGame_LMAchievmentsLayer_h
#define LMGame_LMAchievmentsLayer_h


#include "cocos2d.h"
#include "LMMenuLayer.h"

class LMAchievmentsLayer : public cocos2d::CCLayer{
    
private:
    LMMenuLayer *menuLayer;

    
public:
    
    void loadAchievementsScene();
    
    virtual void onEnter();
    virtual void onExit();
    
    static cocos2d::CCScene* scene();
    
    LMAchievmentsLayer();
    ~LMAchievmentsLayer();

};

#endif

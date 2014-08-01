//
//  MonsterNode.h
//  SaveQ
//
//  Created by timou on 14-7-31.
//
//

#ifndef __SaveQ__MonsterNode__
#define __SaveQ__MonsterNode__

#include <iostream>
#include "cocos2d.h"

class MonsterNode : public cocos2d::Node
{
public:
	virtual bool init();  
	CREATE_FUNC(MonsterNode);
    static MonsterNode* createWithSprite(std::string fileName);
//    cocos2d::Sprite *_hpBar;
    cocos2d::Sprite *_sprite;
    
    float hp;
    
    void subHP(float f);
    cocos2d::ProgressTimer *_hpBar;
    
    bool isKill();
    
private:
    float _hurt;
};

#endif /* defined(__SaveQ__MonsterNode__) */

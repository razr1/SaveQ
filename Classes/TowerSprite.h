//
//  TowerSprite.h
//  SaveQ
//
//  Created by timou on 14-7-31.
//
//

#ifndef __SaveQ__TowerSprite__
#define __SaveQ__TowerSprite__

#include <iostream>
#include "cocos2d.h"

class TowerSprite : public cocos2d::Sprite
{
public:
	virtual bool init();  
	CREATE_FUNC(TowerSprite);
    int createMoney;
    int attack;
    
    bool getCreateFlag();
    void setCreateFlage(bool flag);
    std::string spriteName;
    
    static TowerSprite* createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame)
    {
        TowerSprite *sprite = new (std::nothrow) TowerSprite();
        if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
        {
            sprite->autorelease();
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }
    
    static TowerSprite* createWithSpriteFrameName(const std::string& spriteFrameName)
    {
        cocos2d::SpriteFrame *frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
        
#if COCOS2D_DEBUG > 0
        char msg[256] = {0};
        sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str());
        CCASSERT(frame != nullptr, msg);
#endif
        
        return createWithSpriteFrame(frame);
    }
    
    void setBullet(cocos2d::Sprite* bullet);
    cocos2d::Sprite* getBullet();
    
private:
    cocos2d::Sprite *_bullet;
    bool _createFlag;
};

#endif /* defined(__SaveQ__TowerSprite__) */

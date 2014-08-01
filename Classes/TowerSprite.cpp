//
//  TowerSprite.cpp
//  SaveQ
//
//  Created by timou on 14-7-31.
//
//

#include "TowerSprite.h"
USING_NS_CC;

bool TowerSprite::init()
{
	if ( !Sprite::init() )
	{
	    return false;
	}
	createMoney = 0;
    attack = 0;
    _bullet = nullptr;
	
	return true;
}

void TowerSprite::setBullet(cocos2d::Sprite* bullet)
{
    if(_bullet != nullptr)
        _bullet->release();
    bullet->retain();
    _bullet = bullet;
}

cocos2d::Sprite* TowerSprite::getBullet()
{
    return _bullet;
}

bool TowerSprite::getCreateFlag()
{
    return _createFlag;
}

void TowerSprite::setCreateFlage(bool flag)
{
    _createFlag = flag;
    setOpacity(flag?255:255/2);
}
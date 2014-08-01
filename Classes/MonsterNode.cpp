//
//  MonsterNode.cpp
//  SaveQ
//
//  Created by timou on 14-7-31.
//
//

#include "MonsterNode.h"
USING_NS_CC;

MonsterNode* MonsterNode::createWithSprite(std::string fileName)
{
    MonsterNode* pnode = MonsterNode::create();
    
    auto sprite = Sprite::createWithSpriteFrameName(fileName);
    sprite->setTag(1);
    sprite->setScale(0.5f);
    pnode->addChild(sprite);
    pnode->_sprite = sprite;
    
    Point pos = sprite->getPosition();
    auto hpBack = Sprite::createWithSpriteFrameName("monsterHP_1.png");
    hpBack->setPosition(pos.x, sprite->getContentSize().height/4+5);
    pnode->addChild(hpBack);
    
    auto bar = Sprite::createWithSpriteFrameName("monsterHP_2.png");
    auto enemyPlaneHP = ProgressTimer::create(bar);
    
//    enemyPlaneHP->setPercentage(0.0f);
//    enemyPlaneHP->setScale(0.15f);
    // 如果想要显示从左到右的一个动画效果，必须改成(0,y)
    enemyPlaneHP->setMidpoint(Point(1, 0.5f));
    enemyPlaneHP->setBarChangeRate(Point(1.0f, 0));
    enemyPlaneHP->setType(ProgressTimer::Type::BAR);
    enemyPlaneHP->setPercentage(0.0f);
    
    enemyPlaneHP->setPosition(hpBack->getPosition());
    pnode->addChild(enemyPlaneHP);
    pnode->_hpBar = enemyPlaneHP;
    return pnode;
    
}

bool MonsterNode::isKill()
{
    return (_hurt >= hp);
}

void MonsterNode::subHP(float f)
{
    _hurt += f;
    _hpBar->setPercentage(_hurt/hp*100);
}

bool MonsterNode::init()
{
	if ( !Node::init() )
	{
	    return false;
	}
	hp = 1000;
	_hurt = 0;
	return true;
}
//
//  PlayScene.h
//  SaveQ
//
//  Created by timou on 14-7-28.
//
//

#ifndef __SaveQ__PlayScene__
#define __SaveQ__PlayScene__

#include <iostream>
#include "cocos2d.h"
#include "MonsterNode.h"
#include "TowerSprite.h"

class PlayScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(int level=1);
	virtual bool init();
	CREATE_FUNC(PlayScene);
    
    void setCurrLevel(int level);
    int getCurrLevel();
    
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float f);
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void setCurrMoney(int money);
    
private:
    int _currLevel;
    int _playHP;
    bool _addFlag;
    int _currMoney;
    int _showMonsterNum;
    cocos2d::Vec2 _monsterStartPos;
    cocos2d::ValueVector _monsterPaths;
    
    cocos2d::Label *_goldLabel;
    cocos2d::Sprite* _playHPSprite;
    cocos2d::Sprite* _black;
    
    cocos2d::TMXTiledMap *_map;
    cocos2d::TMXLayer *_mapCollibale;
    cocos2d::Vector<cocos2d::Sprite*> _addSprites;
    cocos2d::Vector<TowerSprite*> _towers;
    cocos2d::Vector<cocos2d::Sprite*> _monsters;
    
    cocos2d::Node *_gameOverNode;
//    cocos2d::Vector<cocos2d::Sprite*> _bullets;
    
    std::map < cocos2d::Node*,cocos2d::Node* > _ackList;
    
    cocos2d::Size _visSize;
    inline void showReady();
    inline void showGo();
    void startGame();
    
    float getTurretRotation(cocos2d::Point &point1,cocos2d::Point &point2);
    
    cocos2d::Point tileCoordForPosition(cocos2d::Point position);
    cocos2d::Vec2 tile2Point(const cocos2d::Vec2 &point);
    
    void showAddSprite(cocos2d::Vec2 &pos);
    void showMonster(float f);
    cocos2d::Sprite* createBullet(cocos2d::Vec2 &startPos, cocos2d::Vec2 &endPos);
    void playBomb(cocos2d::Vec2 &pos);
    void gameOver(bool pass=false);
    
};

#endif /* defined(__SaveQ__PlayScene__) */

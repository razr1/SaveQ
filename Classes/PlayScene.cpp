//
//  PlayScene.cpp
//  SaveQ
//
//  Created by timou on 14-7-28.
//
//

#include "PlayScene.h"
#include "TowerSprite.h"
#include "MonsterNode.h"
#include "SelectLevelScene.h"
USING_NS_CC;

typedef struct{
    float length = -1;
    int index = -1;
}LenIndex;

#define ACK_AREA 150

Scene* PlayScene::createScene(int level)
{
	auto scene = Scene::create();
	
	auto layer = PlayScene::create();
    layer->setCurrLevel(level);
		
	scene->addChild(layer);
	
	return scene;
}

bool PlayScene::init()
{
	if ( !Layer::init() )
	{
	    return false;
	}
	_currMoney = 30;
	return true;
}

void PlayScene::onEnter()
{
    Layer::onEnter();
    _visSize = Director::getInstance()->getVisibleSize();
    std::string fileName = StringUtils::format("level%d.tmx",_currLevel);
    _map = TMXTiledMap::create(fileName);
    this->addChild(_map);
    
    _mapCollibale = _map->getLayer("collidable");
    _mapCollibale->setVisible(false);
    
    _black = Sprite::create("black.png");
    _black->setPosition(_visSize.width/2, _visSize.height/2);
    this->addChild(_black,100);
}

void PlayScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Items02-hd.plist");
    
    auto gold = Sprite::createWithSpriteFrameName("gold.png");
    gold->setPosition(_visSize.width*0.2, _visSize.height*0.95);
    gold->setScale(0.8);
    this->addChild(gold,50);
    
    std::string goldStr = StringUtils::format("x%d",_currMoney);
    
    _goldLabel = Label::createWithSystemFont(goldStr, "Marker Felt", 28);
    _goldLabel->setPosition(gold->getPositionX()+46, _visSize.height*0.95);
    this->addChild(_goldLabel,50);
    
    Animation *animation = Animation::create();
    for(int i=0; i<5; i++){
        std::string fileName = StringUtils::format("air0%d.png",i+1);
        animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName));
    }
    
    animation->setDelayPerUnit(1.0f/15);
    AnimationCache::getInstance()->addAnimation(animation, "bombAnima");
    
    
    
    this->showReady();
}
/**
 *  设置金币
 *
 *  @param money <#money description#>
 */
void PlayScene::setCurrMoney(int money)
{
    _currMoney = money;
    std::string goldStr = StringUtils::format("x%d",_currMoney);
    _goldLabel->setString(goldStr);
    
    for(int i=1; i<_addSprites.size(); i++){
        TowerSprite *ts = (TowerSprite *)_addSprites.at(i);
        ts->setCreateFlage(_currMoney >= ts->createMoney);
    }
}

void PlayScene::showReady()
{
    _black->setVisible(true);
    auto ready = Label::createWithSystemFont("Ready", "Marker Felt", 94);
    ready->setPosition(_visSize.width/2, _visSize.height/2);
    ready->setScale(0.1f);
    this->addChild(ready,101);
    
    auto action1 = RotateBy::create(0.3f, 360);
    auto action2 = ScaleTo::create(0.3f, 1);
    
    CallFuncN *callfun = CallFuncN::create([this](cocos2d::Node *node){
        node->stopAllActions();
        this->removeChild(node);
        this->showGo();
    });
    ready->runAction(Sequence::create(Spawn::create(action1,action2, NULL),DelayTime::create(1),callfun, NULL));
}

void PlayScene::showGo()
{
    auto startGo = Label::createWithSystemFont("GO!", "Marker Felt", 94);
    startGo->setPosition(_visSize.width/2, _visSize.height/2);
    startGo->setScale(0.1f);
    this->addChild(startGo,102);
    
    auto action1 = RotateBy::create(0.3f, 360);
    auto action2 = ScaleTo::create(0.3f, 1);
    CallFuncN *callfun = CallFuncN::create([this](cocos2d::Node *node){
        node->stopAllActions();
        this->removeChild(node);
        this->startGame();
    });
    auto action3 = FadeTo::create(0.2f, 0);
    startGo->runAction(Sequence::create(Spawn::create(action1,action2, NULL),DelayTime::create(1),action3,callfun, NULL));
}

/**
 *  开始游戏
 */
void PlayScene::startGame()
{
    auto mapLayer  = _map->getObjectGroup("pathObj");
    auto values = mapLayer->getObject("path1");
    _monsterStartPos.x = values.at("x").asFloat();
    _monsterStartPos.y = values.at("y").asFloat();
    _monsterPaths = mapLayer->getObjects();
    
    _black->setVisible(false);
    
    for(auto &item1 : _monsters){
        this->removeChild(item1->getParent());
    }
    _monsters.clear();
    
    for(auto &item2 : _towers){
        this->removeChild(item2);
    }
    
    _towers.clear();
    _ackList.clear();
    if(_playHPSprite)
        this->removeChild(_playHPSprite);
    
	this->setCurrMoney(30);
    _playHP = 10;
    _showMonsterNum = 0;
    this->schedule(schedule_selector(PlayScene::showMonster), 1.0f);
    this->scheduleUpdate();
    
    auto gameListener = EventListenerTouchOneByOne::create();
    gameListener->onTouchBegan = CC_CALLBACK_2(PlayScene::onTouchBegan, this);
    gameListener->onTouchEnded = CC_CALLBACK_2(PlayScene::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(gameListener, this);
}

/**
 *  创建怪物
 *
 *  @param f <#f description#>
 */
void PlayScene::showMonster(float f)
{
    auto monster = MonsterNode::createWithSprite("bird.png");
    monster->hp = (500+_showMonsterNum)*3;
//    monster->setScale(0.4);
    monster->setPosition(_monsterStartPos);
    this->addChild(monster);
    _monsters.pushBack(monster->_sprite);
    
    Vector<cocos2d::FiniteTimeAction *> arrayOfActions;
    for(int i=1; i<_monsterPaths.size(); i++){
        auto value1 = _monsterPaths[i-1].asValueMap();
        auto value2 = _monsterPaths[i].asValueMap();
        auto pos1 = Vec2(value1.at("x").asFloat(), value1.at("y").asFloat());
        auto pos2 = Vec2(value2.at("x").asFloat(), value2.at("y").asFloat());
        float length = pos1.distance(pos2);
        
        auto moveTo = MoveTo::create(length/80.0f, pos2);
        arrayOfActions.pushBack(moveTo);
    }
    
    auto callbackN = CallFuncN::create([&](Node *node){
        Vec2 pos = node->getPosition();
        _monsters.eraseObject(((MonsterNode*)node)->_sprite);
        this->removeChild(node);
        this->playBomb(pos);
        this->setCurrMoney(_currMoney-10);
        --_playHP;
        if(_playHP <= 0){
            this->gameOver();
            return;
        }
        if(_playHPSprite){
            this->removeChild(_playHPSprite);
        }
        std::string fileName = StringUtils::format("BossHP0%d.png",_playHP);
        _playHPSprite = Sprite::createWithSpriteFrameName(fileName);
        _playHPSprite->setPosition(884, 224+300);
        this->addChild(_playHPSprite);
    });
    arrayOfActions.pushBack(callbackN);
    monster->runAction(Sequence::create(arrayOfActions));
    
    ++_showMonsterNum;
    if(_showMonsterNum >= 25){
        this->unschedule(schedule_selector(PlayScene::showMonster));
    }
}

void PlayScene::update(float f)
{
    if(_towers.size() == 0 || _showMonsterNum==0){
        return;
    }
    
    if(_showMonsterNum >= 25 && _monsters.size() == 0){
        this->gameOver(true);
        return;
    }
    
    //保存加攻击范围的坐标，后面通过排序找出离炮台最近的怪物
    std::vector<LenIndex> lens;
    
    for (auto &towerItem : _towers) {
        Vec2 towerPos = towerItem->getPosition();
        lens.clear();
        
        for(int i=0; i<_monsters.size(); i++){
            auto monsterItem = _monsters.at(i);
            MonsterNode *mn = (MonsterNode *)monsterItem->getParent();
            if(!mn){
                _monsters.erase(i);
                break;
            }
            Vec2 monsterPos = mn->getPosition();
            //怪物是否被杀，被杀后删除所有子弹与该怪物的关联
            if(mn->isKill()){
                _monsters.eraseObject(monsterItem);
                
                auto ackIt = _ackList.cbegin();
                while (ackIt != _ackList.cend()) {
                    if(ackIt->second == mn){
                        ackIt = _ackList.erase(ackIt);
                    }else{
                        ++ackIt;
                    }
                }
                this->removeChild(mn,true);
                this->playBomb(monsterPos);
                break;
            }
            
            //子弹攻击范围
            float length = towerPos.distance(monsterPos);
            if(length <= ACK_AREA){
                LenIndex li;
                li.length = length;
                li.index = i;
                lens.push_back(li);
            }
        }
        
        if(lens.size() < 1)
            continue;
        
        //找出离炮台最近的怪物
        std::sort(lens.begin(), lens.end(), [](const LenIndex &l1, const LenIndex &l2){
            if(l1.length < l2.length){
                return -1;
            }else{
                return 0;
            }
        });
        
        auto ackMonster = _monsters.at(lens[0].index);
        MonsterNode *mn = (MonsterNode *)ackMonster->getParent();
        Vec2 monsterPos = mn->getPosition();
        //旋转炮台
        float r = this->getTurretRotation(monsterPos, towerPos);
        towerItem->setRotation(r);
        if(towerItem->getBullet() == nullptr || towerItem->getBullet()->getNumberOfRunningActions()==0){
            //关联子弹攻击对象
            towerItem->setBullet(this->createBullet(towerPos, monsterPos));
            _ackList[towerItem->getBullet()] = mn;
        }
        
    }
}
/**
 *  创建子弹
 *
 *  @param startPos <#startPos description#>
 *  @param endPos   <#endPos description#>
 *
 *  @return <#return value description#>
 */
cocos2d::Sprite* PlayScene::createBullet(cocos2d::Vec2 &startPos, cocos2d::Vec2 &endPos)
{
    Sprite *bullet = Sprite::createWithSpriteFrameName("bullet_01.png");
    bullet->setScale(0.5);
    bullet->setPosition(startPos);
    auto moveTo = MoveTo::create(0.2f, endPos);
    auto callFunN = CallFuncN::create([&](Node *node){
        MonsterNode *mn = (MonsterNode *)_ackList[node];
        if(mn && !mn->isKill()){
            mn->subHP(100);
        }
        _ackList.erase(node);
        this->removeChild(node);
    });
    this->addChild(bullet);
    bullet->runAction(Sequence::create(moveTo,callFunN, NULL));
    return bullet;
}

/**
 *  怪物被杀后爆炸效果
 *
 *  @param pos <#pos description#>
 */
void PlayScene::playBomb(cocos2d::Vec2 &pos)
{
    this->setCurrMoney(_currMoney+10);
    auto spr = Sprite::createWithSpriteFrameName("air01.png");
    spr->setPosition(pos);
    auto anin = Animate::create(AnimationCache::getInstance()->getAnimation("bombAnima"));
    auto callFunN = CallFuncN::create([&](Node* node){
        this->removeChild(node);
    });
    spr->runAction(Sequence::create(anin,callFunN, NULL));
    this->addChild(spr);
}

/**
 *  根据怪物位置返回炮台旋转角度
 *
 *  @param point1 <#point1 description#>
 *  @param point2 <#point2 description#>
 *
 *  @return <#return value description#>
 */
float PlayScene::getTurretRotation(cocos2d::Point &point1,cocos2d::Point &point2)
{
    float offX = point1.x - point2.x;
    float offY = point1.y - point2.y;
    float radian = atan2f(offY, offX);
    float angle = CC_RADIANS_TO_DEGREES(radian);
    return 90-angle;
}

bool PlayScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(!_addFlag){
        Point touchLocation = touch->getLocation();
        touchLocation = this->convertToNodeSpace(touchLocation);
        Point tilePos = this->tileCoordForPosition(touchLocation);
        int tileGid = _mapCollibale->getTileGIDAt(tilePos);
        if(tileGid){
            Value properties = _map->getPropertiesForGID(tileGid);
            ValueMap map = properties.asValueMap();
            std::string value = map.at("collidable").asString();
            if(value == "true"){
                return false;
            }
        }
        
        //该位置已经有炮塔
        std::string properKey = StringUtils::format("cell_%d_%d",(int)tilePos.x,(int)tilePos.y);
        ValueMap &mapProper =  _map->getProperties();
        if(mapProper.find(properKey) != mapProper.end()){
            return false;
        }
        
    }
    return true;
}

void PlayScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Point touchLocation = touch->getLocation();
    if(_addFlag){
        for(int i=1; i<_addSprites.size(); i++){
            TowerSprite *ts = (TowerSprite *)_addSprites.at(i);
            Point towerPos = ts->convertToNodeSpace(touchLocation);
            if(ts->getBoundingBox().containsPoint(touchLocation)){
                if(ts->getCreateFlag()){
                    auto sprite = TowerSprite::createWithSpriteFrameName(ts->spriteName);
                    Point tilePos = this->tileCoordForPosition(_addSprites.at(0)->getPosition());
                    Vec2 glPos = this->tile2Point(tilePos);
                    sprite->setPosition(glPos.x+_map->getTileSize().width/2,glPos.y-_map->getTileSize().height/2);
                    this->addChild(sprite);
                    
                    _towers.pushBack(sprite);
                    this->setCurrMoney(_currMoney-10);
                    
                    std::string properKey = StringUtils::format("cell_%d_%d",(int)tilePos.x,(int)tilePos.y);
                    ValueMap &mapProper =  _map->getProperties();
                    mapProper.insert(std::make_pair(properKey, Value("collidable")));
                   
                }else{
                    return;
                }
            }
        }
        
        for (Sprite* &item : _addSprites) {
            this->removeChild(item);
        }
        _addFlag = false;
        return;
    }
    
    Point mapPoint = this->tileCoordForPosition(touchLocation);
    
    this->showAddSprite(touchLocation);
}

void PlayScene::showAddSprite(cocos2d::Vec2 &pos)
{
    if(_addFlag)
        return;
    _addFlag = true;
    Sprite *sprite1 = nullptr;
    TowerSprite *sprite2 = nullptr;
    
    if(_addSprites.size() > 0){
        sprite1 = _addSprites.at(0);
        sprite2 = (TowerSprite *)_addSprites.at(1);
    }else{
        sprite1 = Sprite::createWithSpriteFrameName("add.png");
        sprite1->setScale(0.6);
        sprite1->setOpacity(255/2);
        _addSprites.pushBack(sprite1);
        
        sprite2 = (TowerSprite *)TowerSprite::createWithSpriteFrameName("bollte.png");
        sprite2->setScale(1.5f);
        sprite2->spriteName = std::string("bollte.png");
        sprite2->createMoney = 10;
        _addSprites.pushBack(sprite2);
    }
    
    sprite1->setPosition(pos);
    sprite2->setPosition(pos.x, pos.y+40);
    
    sprite2->setCreateFlage(_currMoney >= sprite2->createMoney);
    
    auto bl = Blink::create(0.4f, 1);
    auto rep = RepeatForever::create(bl);
    sprite1->runAction(rep);
    
    this->addChild(sprite1);
    this->addChild(sprite2);
}

void PlayScene::gameOver(bool pass)
{
    this->unscheduleAllSelectors();
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    for(auto &item : _monsters){
        item->getParent()->stopAllActions();
    }
    
    _black->setVisible(true);
    
    auto sprite = Sprite::createWithSpriteFrameName("block.png");
    
    _gameOverNode = Node::create();
    _gameOverNode->setAnchorPoint(Vec2(0.5,0.5));
    _gameOverNode->setPosition(_visSize.width/2,_visSize.height/2);
    _gameOverNode->setContentSize(sprite->getContentSize());
    this->addChild(_gameOverNode,101);
    
    sprite->setPosition(_gameOverNode->getContentSize().width/2,_gameOverNode->getContentSize().height/2);
    _gameOverNode->addChild(sprite);
    
    std::string labelStr = pass ? "牛了过关" : "Game Over";
    
    auto gameOverLabel = Label::createWithSystemFont(labelStr, "Marker Felt", 54);
    gameOverLabel->setPosition(_gameOverNode->getContentSize().width/2, _gameOverNode->getContentSize().height/2);
    _gameOverNode->addChild(gameOverLabel);
    
    std::string menu1 = pass ? "btn-next.png" : "btn-again.png";
    
    auto startMenu = MenuItemSprite::create(Sprite::createWithSpriteFrameName(menu1), Sprite::createWithSpriteFrameName(menu1));
    if(pass){
        startMenu->setCallback([&](Ref *ref){
            _currLevel++;
            if(_currLevel > 3)
                _currLevel = 1;
            this->removeChild(_map);
            std::string fileName = StringUtils::format("level%d.tmx",_currLevel);
            _map = TMXTiledMap::create(fileName);
            this->addChild(_map);
            
            _mapCollibale = _map->getLayer("collidable");
            _mapCollibale->setVisible(false);
            this->removeChild(_gameOverNode);
            this->showReady();
        });
    }else{
        startMenu->setCallback([&](Ref *ref){
            this->removeChild(_gameOverNode);
            this->showReady();
        });
    }
    
    auto selMenu = MenuItemSprite::create(Sprite::createWithSpriteFrameName("btn-select.png"), Sprite::createWithSpriteFrameName("btn-select.png"));
    selMenu->setCallback([&](Ref *ref){
        auto selectLevel = SelectLevelScene::createScene();
        auto tfBL = TransitionSplitCols::create(0.8f, selectLevel);
        Director::getInstance()->replaceScene(tfBL);
    });
    
    auto menu = Menu::create(selMenu,startMenu,NULL);
    menu->alignItemsHorizontallyWithPadding(40);
    menu->setPosition(_gameOverNode->getContentSize().width/2,_gameOverNode->getContentSize().height*0.2);
    _gameOverNode->addChild(menu);
}

// 将屏幕坐标转换为TileMap坐标
Point PlayScene::tileCoordForPosition(Point position){
    // CC_CONTENT_SCALE_FACTOR 在iPhone上视网膜显示返回2。否则返回1
	// 玩家位置的x除以地图的宽，得到的是地图横向的第几个格子（tile）
    int x = (int)(position.x / (_map->getTileSize().width / CC_CONTENT_SCALE_FACTOR()));
    // 玩家位置的y除以地图的高，得到的是地图纵向第几个格子（tile），
	// 因为Cocos2d-x的y轴和TileMap的y轴相反，所以使用地图的高度减去玩家位置的y
    float pointHeight = _map->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
    int y = (int)((_map->getMapSize().height * pointHeight - position.y) / pointHeight);
    return Point(x,y);
}

/**
 *  网络坐标转opengl坐标
 *
 *  @param point <#point description#>
 *
 *  @return <#return value description#>
 */
cocos2d::Vec2 PlayScene::tile2Point(const cocos2d::Vec2 &point)
{
    float x = point.x * _map->getTileSize().width;
    float tileHeight = _map->getTileSize().height;
    float y = (_map->getMapSize().height * tileHeight) - (point.y * tileHeight);
    Vec2 v(x,y);
    return v;
}

void PlayScene::setCurrLevel(int level)
{
    _currLevel = level;
}

int PlayScene::getCurrLevel()
{
    return _currLevel;
}
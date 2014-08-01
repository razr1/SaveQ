//
//  SelectLevelScene.cpp
//  SaveQ
//
//  Created by timou on 14-7-28.
//
//

#include "SelectLevelScene.h"
#include "PlayScene.h"
USING_NS_CC;

Scene* SelectLevelScene::createScene()
{
	auto scene = Scene::create();
	
	auto layer = SelectLevelScene::create();
		
	scene->addChild(layer);
	
	return scene;
}

bool SelectLevelScene::init()
{
	if ( !Layer::init() )
	{
	    return false;
	}
    _currLevel = 1;
	auto visSize = Director::getInstance()->getVisibleSize();
	
	auto bg = Sprite::create("select_bg.png");
    bg->setPosition(visSize.width/2,visSize.height/2);
    this->addChild(bg);
    
    _levelNode = Node::create();
    _levelNode->setContentSize(Size(visSize.width*3, visSize.height));
    _levelNode->setAnchorPoint(Vec2::ZERO);
    _levelNode->setPosition(Vec2::ZERO);
    
    for (int i=0; i<3; i++) {
        std::string fileName = StringUtils::format("level%d_sel.png",i+1);
        auto level1 = Sprite::create(fileName);
        auto levelSize = level1->getContentSize();
        level1->setPosition(visSize.width/2+i*levelSize.width+i*160, visSize.height/2);
        _levelNode->addChild(level1);
        _levelPhotos.pushBack(level1);
    }
    
    this->addChild(_levelNode);
    
    //开始游戏按钮
    auto startMenu = MenuItemSprite::create(Sprite::createWithSpriteFrameName("btn-start.png"), Sprite::createWithSpriteFrameName("btn-start.png"));
    startMenu->setCallback(CC_CALLBACK_1(SelectLevelScene::onPlay, this));
    
    auto menu = Menu::create(startMenu,NULL);
    menu->alignItemsHorizontallyWithPadding(40);
    menu->setPosition(visSize.width/2,visSize.height*0.1);
    this->addChild(menu);
    
    
    auto touchEvent = EventListenerTouchOneByOne::create();
    touchEvent->onTouchBegan = [&](Touch *touch, Event *event){
        _beginPoint = touch->getLocation();
        _beginPos = _levelNode->getPosition();
        return true;
    };
    
    touchEvent->onTouchMoved = [&](Touch *touch, Event *event){
        auto nodePoint = touch->getLocation();
        float moveWidth = nodePoint.x - _beginPoint.x;
        float moveX = _beginPos.x+moveWidth;
        _levelNode->setPosition(moveX, 0);
    };
    
    touchEvent->onTouchEnded = [&](Touch *touch, Event *event){
        auto contentSize = Director::getInstance()->getVisibleSize();
        auto nodePoint = touch->getLocation();
        float moveWidth = nodePoint.x - _beginPoint.x;
        
        auto movePoint = _beginPos;
        if(fabs(moveWidth) > 150){
            float l = (contentSize.width+fabsf(_levelNode->getPositionX()))/contentSize.width;
            _currLevel= moveWidth < 0 ? ceilf(l) : floorf(l);
            _currLevel = MIN(_currLevel, 3);
            movePoint.x =  -1*(_currLevel-1)*contentSize.width;
        }
        _levelNode->runAction(MoveTo::create(0.3, movePoint));
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEvent, this);
	
	return true;
}

void SelectLevelScene::onPlay(Ref *ref)
{
    auto playScene = PlayScene::createScene(_currLevel);
    auto tfBL = TransitionSplitCols::create(0.5f, playScene);
    Director::getInstance()->replaceScene(tfBL);
}
//
//  MainScene.cpp
//  SaveQ
//
//  Created by timou on 14-7-27.
//
//

#include "MainScene.h"
#include "SelectLevelScene.h"
#include "PlayScene.h"
USING_NS_CC;

Scene* MainScene::createScene()
{
	auto scene = Scene::create();
	
	auto layer = MainScene::create();
		
	scene->addChild(layer);
	
	return scene;
}

bool MainScene::init()
{
	if ( !Layer::init() )
	{
	    return false;
	}
    
    auto visSize = Director::getInstance()->getVisibleSize();
	
	auto bg = Sprite::create("main_bg.png");
    bg->setPosition(visSize.width/2,visSize.height/2);
    this->addChild(bg);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("resource.plist");
//    auto spriteBath = SpriteBatchNode::create("resource.pvr.ccz");
//    this->addChild(spriteBath);
    
    auto startMenu = MenuItemSprite::create(Sprite::createWithSpriteFrameName("btn-start.png"), Sprite::createWithSpriteFrameName("btn-start.png"));
    startMenu->setCallback(CC_CALLBACK_1(MainScene::onPlay, this));
    
    auto selMenu = MenuItemSprite::create(Sprite::createWithSpriteFrameName("btn-select.png"), Sprite::createWithSpriteFrameName("btn-select.png"));
    selMenu->setCallback(CC_CALLBACK_1(MainScene::onSelLevel, this));
    
    auto menu = Menu::create(startMenu,selMenu,NULL);
    menu->alignItemsHorizontallyWithPadding(40);
    menu->setPosition(visSize.width/2,visSize.height*0.1);
    this->addChild(menu);
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("BGMusic.mp3",true);
    
	return true;
}

void MainScene::onPlay(Ref *ref)
{
    auto playScene = PlayScene::createScene();
    auto tfBL = TransitionSplitCols::create(0.5f, playScene);
    Director::getInstance()->pushScene(tfBL);
}

void MainScene::onSelLevel(Ref *ref)
{
    auto selectLevel = SelectLevelScene::createScene();
    auto tfBL = TransitionSplitCols::create(0.8f, selectLevel);
    Director::getInstance()->pushScene(tfBL);
    
}


//
//  SelectLevelScene.h
//  SaveQ
//
//  Created by timou on 14-7-28.
//
//

#ifndef __SaveQ__SelectLevelScene__
#define __SaveQ__SelectLevelScene__

#include <iostream>
#include "cocos2d.h"

class SelectLevelScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();  
	CREATE_FUNC(SelectLevelScene);
private:
    void onPlay(cocos2d::Ref *ref);
    int _currLevel;
    cocos2d::Vec2 _beginPoint;
    cocos2d::Vec2 _beginPos;
    
    cocos2d::Node *_levelNode;
    
    cocos2d::Vector<cocos2d::Sprite*> _levelPhotos;
};

#endif /* defined(__SaveQ__SelectLevelScene__) */

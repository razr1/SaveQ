//
//  MainScene.h
//  SaveQ
//
//  Created by timou on 14-7-27.
//
//

#ifndef __SaveQ__MainScene__
#define __SaveQ__MainScene__

#include <iostream>
#include "cocos2d.h"

class MainScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();  
	CREATE_FUNC(MainScene);
    
private:
    void onPlay(cocos2d::Ref *ref);
    void onSelLevel(cocos2d::Ref *ref);
};

#endif /* defined(__SaveQ__MainScene__) */

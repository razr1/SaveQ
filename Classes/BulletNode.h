//
//  BulletNode.h
//  SaveQ
//
//  Created by timou on 14-7-31.
//
//

#ifndef __SaveQ__BulletNode__
#define __SaveQ__BulletNode__

#include <iostream>
#include "cocos2d.h"

class BulletNode : public cocos2d::Node
{
public:
	virtual bool init();  
	CREATE_FUNC(BulletNode);
};

#endif /* defined(__SaveQ__BulletNode__) */

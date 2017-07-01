#ifndef __TXZSCENE_H__
#define __TXZSCENE_H__
#include "cocos2d.h"
class TxzGame : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(TxzGame);
}
#endif
#ifndef __GAMESEL_H__
#define __GAMESEL_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"


class GameSel : public cocos2d::Layer
{
protected:
public:
	static int nGkSet;
	static cocos2d::Scene* createScene();
	virtual bool init();
	// a selector callback
	void menuSelCallback(cocos2d::Ref* pSender);
	//void menuLeftCallBack(Ref* pSender);
	//void menuMoveCallBack(Ref* pSender, int nDir);
	// implement the "static create()" method manually
	CREATE_FUNC(GameSel);
};

#endif // __GAMESEL_H__

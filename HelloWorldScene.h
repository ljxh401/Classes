#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#define MapSize	20
enum{
	eNop,
	eFloor,
	eBall = 2,
	eWall = 4,
	eBox = 8,
	eMan=16,
};
class HelloWorld : public cocos2d::Layer
{
protected:
	unsigned char map[MapSize*MapSize];
	int nBackCnt, szBackRec[3];
	int nWayAuto, szWayAuto[100];
	int nMaxBack;
	cocos2d::Sprite *pSp[MapSize*MapSize];
	float picSize,fYOffset,fXOffset;
	bool bWin;
	int playerPos;
	//cocos2d::Layer *pMenuLayer;
	cocos2d::Sprite *pMan;
	void initMap(int iMap);
	cocos2d::Label *labelMsg;
	cocos2d::EventListenerMouse* _mouseListener;
	void dirButtonEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type, int dir);
	void PushBox(int boxPos);
	void FindWay(int pos);
	void AutoWayCallBack(void);
	void MovBoxCallBack(int boxPos);
	void onMouseDown(cocos2d::Event *event);
	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
	void ResetGame(void);
	void disableMenu(float time);
public:
	static int nGuankaCnt;
	static int nGuankaWin;
    static cocos2d::Scene* createScene();
    virtual bool init();
	virtual void onEnter();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	//void menuLeftCallBack(Ref* pSender);
	//void menuMoveCallBack(Ref* pSender, int nDir);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	~HelloWorld();

};

#endif // __HELLOWORLD_SCENE_H__

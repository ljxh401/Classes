#include "HelloWorldScene.h"
#include "gamesel.h"
USING_NS_CC;
using namespace cocos2d::ui;

int GameSel::nGkSet = 0;
Scene* GameSel::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	// 'layer' is an autorelease object
	auto layer = GameSel::create();
	// add layer as a child to scene
	scene->addChild(layer);
	// return the scene
	return scene;
}

bool GameSel::init()
{
	//////////////////////////////
	// 1. super init first
	int i;
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto *chnStrings = Dictionary::createWithContentsOfFile("chn.xml");

	nGkSet = HelloWorld::nGuankaCnt;

	auto mButton = Button::create();
	//mButton->setTitleText("·µ»ØÓÎÏ·");
	mButton->setTitleText(((String*)chnStrings->objectForKey("retgame"))->getCString());

	mButton->setTitleFontSize(48);
	mButton->setPosition(Vec2(origin.x +visibleSize.width/4, origin.y + mButton->getContentSize().height * 1.5f));
	mButton->addClickEventListener([this](Ref* sender) {
		auto director = Director::getInstance();
		director->popScene();
	});
	addChild(mButton);

#if 1
	mButton = Button::create();
	mButton->setTitleText(((String*)chnStrings->objectForKey("exit"))->getCString());
	mButton->setTitleFontSize(48);
	mButton->setPosition(Vec2(origin.x + visibleSize.width * 3 / 4, origin.y + mButton->getContentSize().height * 1.5f));
	mButton->addClickEventListener([this](Ref* sender) {
		auto director = Director::getInstance();
		director->end();
	});
	addChild(mButton);
#endif

	char gkStr[16];
	for (i = 0; i < 68; i++)
	{
		sprintf(gkStr, "%d", i + 1);
		mButton = Button::create();
		mButton->setTitleText(gkStr);
		mButton->setPosition(Vec2(origin.x + visibleSize.width / 20 + visibleSize.width/6*(i%6)+mButton->getContentSize().width / 2  ,
			origin.y + visibleSize.height*7 / 8 - visibleSize.height/15*(i/6) ));
		mButton->setTitleFontSize(48);
		mButton->addClickEventListener([this, i](Ref* sender) {
			nGkSet = i;	HelloWorld::nGuankaCnt = -1;
			auto director = Director::getInstance();
			director->popScene();
		});
		if (i == HelloWorld::nGuankaCnt)	mButton->setColor(Color3B(0xFF, 0x00, 0x00));
		else if (i>HelloWorld::nGuankaWin)
		{
			mButton->setColor(Color3B(0x80, 0x80, 0x80));
			mButton->setEnabled(false);
		}
		addChild(mButton);
	}
	return true;
}

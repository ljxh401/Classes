#include "HelloWorldScene.h"
#include "TxzMap.h"
#include "gamesel.h"
USING_NS_CC;
using namespace cocos2d::ui;

//template <typename T> std::string tostr(const T& t) { std::ostringstream os; os << t; return os.str(); }
#define MovTime	0.1f
int HelloWorld::nGuankaCnt = 0;
int HelloWorld::nGuankaWin = 0;
HelloWorld::~HelloWorld()
{
	//_eventDispatcher->removeEventListener(_mouseListener);
}
Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void HelloWorld::initMap(int iMap)
{
	int nXlen, nYlen,i,sum,d;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	iMap = (int)txz_mapindex[iMap * 4] + ((int)txz_mapindex[iMap * 4 + 1] << 8); iMap -= 1;
	nXlen = txz_map[iMap++]; nYlen = txz_map[iMap++]; 
	memset(map, eNop, sizeof(map));
	memset(pSp, 0, sizeof(pSp));
	sum = nXlen*nYlen;

	if (nXlen <= nYlen)
	{
		for (i = 0; i < sum; i++)
		{
			d = txz_map[iMap++];
			if(d == 6)			d = eNop;
			else if (d == 0)	d = eFloor;
			else if(d == 3)		d = eBall;
			else if(d == 1)		d = eWall;
			else if(d == 2)		d = eBox;
			else if (d == 4)		d = eMan;
			map[(i%nXlen) + 20 * (i / nXlen)] = d;
		}
	}
	else
	{
		for (i = 0; i < sum; i++)
		{
			d = txz_map[iMap++];
			if (d == 6)			d = eNop;
			else if (d == 0)	d = eFloor;
			else if (d == 3)		d = eBall;
			else if (d == 1)		d = eWall;
			else if (d == 2)		d = eBox;
			else if (d == 4)		d = eMan;
			map[(i%nXlen) * 20 + (i / nXlen)] = d;
		}
		i = nXlen; nXlen = nYlen; nYlen = i;
	}
	picSize = visibleSize.width / (nXlen - 2);

	if ((picSize*nYlen) > visibleSize.height*5/6)
	{
		picSize = (visibleSize.height * 5 / 6) / nYlen;
	}

	fYOffset = visibleSize.height - 80 - picSize*(nYlen-1);
	fXOffset = (visibleSize.width - picSize*(nXlen - 2)) / 2;

	for (i = 0; i < MapSize*MapSize; i++)
	{
			Sprite *pTmp;
			{
				switch (map[i])
				{
				case eFloor:
					pTmp = Sprite::createWithSpriteFrameName("floor.png");
					pTmp->setPosition(Vec2(fXOffset+(i % MapSize - 1) * picSize + 0.5f*picSize, fYOffset + (i / MapSize - 1) * picSize + 0.5f*picSize));
					pTmp->setScale(picSize/30);
					addChild(pTmp);
					break;
				case eBall:
					pTmp = Sprite::createWithSpriteFrameName("ball.png");
					pTmp->setPosition(Vec2(fXOffset + (i % MapSize - 1) * picSize + 0.5f*picSize, fYOffset + (i / MapSize - 1) * picSize + 0.5f*picSize));
					pTmp->setScale(picSize / 30);
					addChild(pTmp);
					break;
				case eWall:
					pTmp = Sprite::createWithSpriteFrameName("wall.png");
					pTmp->setPosition(Vec2(fXOffset + (i % MapSize - 1) * picSize + 0.5f*picSize, fYOffset + (i / MapSize - 1) * picSize + 0.5f*picSize));
					pTmp->setScale(picSize / 30);
					addChild(pTmp);
					break;
				case eBox:
					pTmp = Sprite::createWithSpriteFrameName("floor.png");//pTmp->setColor(Color3B(0x80, 0x30, 0x30));
					pTmp->setPosition(Vec2(fXOffset + (i % MapSize - 1) * picSize + 0.5f*picSize, fYOffset + (i / MapSize - 1) * picSize + 0.5f*picSize));
					pTmp->setScale(picSize / 30);
					addChild(pTmp);
					pTmp = Sprite::createWithSpriteFrameName("box0.png");
					pTmp->setPosition(Vec2(fXOffset + (i % MapSize - 1) * picSize + 0.5f*picSize, fYOffset + (i / MapSize - 1) * picSize + 0.5f*picSize));
					pTmp->setScale(picSize / 30);
					addChild(pTmp, 1);
					pSp[i] = pTmp;
					map[i] = eBox;			//标记为箱子
					break;
				case eMan:
					pTmp = Sprite::createWithSpriteFrameName("floor.png"); //pTmp->setColor(Color3B(0x80, 0x30, 0x30));
					pTmp->setPosition(Vec2(fXOffset + (i % MapSize - 1) * picSize + 0.5f*picSize, fYOffset + (i / MapSize - 1) * picSize + 0.5f*picSize));
					pTmp->setScale(picSize / 30);
					addChild(pTmp);
					pTmp = Sprite::createWithSpriteFrameName("0.png");
					pTmp->setPosition(Vec2(fXOffset + (i % MapSize - 1) * picSize + 0.5f*picSize, fYOffset + (i / MapSize - 1) * picSize + 0.5f*picSize));
					pTmp->setScale(picSize / 30);
					playerPos = i;
					addChild(pTmp, 1);
					pMan = pTmp;
					map[i] = eMan;
					break;
				}
			}
	}
	nBackCnt = 0;
	nWayAuto = 0;
	nMaxBack = 0;
	bWin = false;
}

#if 1
void HelloWorld::onEnter()
{
	//return true;
	Layer::onEnter();
	if (nGuankaCnt==-1)
	{
		nGuankaCnt = GameSel::nGkSet;
#if 1
		FILE *fp;
		//err = fopen_s(&fp, "box.rec", "rb+");
		String path = FileUtils::getInstance()->getWritablePath() + "box.rec";
		fp = fopen(path.getCString(), "rb+");
		fwrite(&nGuankaCnt, sizeof(nGuankaCnt), 1, fp);
		fclose(fp);
#else
		auto fu = FileUtils::getInstance();
		Data dat;
		unsigned char da[8]; memcpy(da, &nGuankaCnt, 4); memcpy(da+4, &nGuankaWin, 4);
		dat.fastSet(da, 8);
		fu->writeDataToFile(dat, "box.rec");
#endif
		ResetGame();
	}
}
#endif


// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

#if 0
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	//closeItem->setAnchorPoint(Vec2(0.5f, 0.5f));
#if 0
	closeItem->setScale(15.0f);
	closeItem->setPosition(Vec2(origin.x + visibleSize.width/2 - closeItem->getContentSize().width ,
		origin.y + visibleSize.height/2- closeItem->getContentSize().height ));
#else
	closeItem->setScale(2.0f);
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width,
		origin.y + closeItem->getContentSize().height));
#endif
	//_mLeft->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    // create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
#endif
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
	char tmpStr[256];
	//auto
#if 0
	labelMsg = Label::createWithTTF("Box", "fonts/Marker Felt.ttf", 36);//, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::TOP);
	sprintf(tmpStr, "Box:%d  ", nGuankaCnt + 1);
	labelMsg->setString(tmpStr);
	labelMsg->setPosition(Vec2(origin.x + visibleSize.width - labelMsg->getContentSize().width/2, origin.y + visibleSize.height-labelMsg->getContentSize().height*3));
	this->addChild(labelMsg);
#endif

	labelMsg = Label::createWithTTF("WeChat: QbasicJacky ", "fonts/Marker Felt.ttf", 36);// , Size::ZERO, TextHAlignment::RIGHT, TextVAlignment::BOTTOM);
	labelMsg->setColor(Color3B(0xFF, 0, 0));
	labelMsg->setPosition(Vec2(origin.x + labelMsg->getContentSize().width / 2, origin.y + labelMsg->getContentSize().height));
	this->addChild(labelMsg);



	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("res/txz.plist");
	initMap(nGuankaCnt);

	Button* mButton;
#if 0
	for (int i = 0; i < 4; i++)
	{
		mButton = Button::create("res/b1.png", "res/b2.png");
		mButton->setAnchorPoint(Vec2(1.3f, 0.5f)); mButton->setRotation(90*i);
		mButton->setPosition(Vec2(origin.x + 640 + (visibleSize.width - 640)/2,origin.y + visibleSize.height/3));
		mButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::dirButtonEvent, this, i));
		mButton->setZoomScale(0.4f);
		mButton->setPressedActionEnabled(true);
		addChild(mButton);
	}
#endif
	auto *chnStrings = Dictionary::createWithContentsOfFile("chn.xml");

	//labelMsg = Label::createWithTTF("test",  "fonts/Marker Felt.ttf", 36);// , Size::ZERO, TextHAlignment::RIGHT, TextVAlignment::BOTTOM);
	//labelMsg = Label::createWithTTF(((String*)chnStrings->objectForKey("guanka"))->getCString(), "Arial", 36);// , Size::ZERO, TextHAlignment::RIGHT, TextVAlignment::BOTTOM);
	labelMsg = Label::createWithTTF("", "fonts/Marker Felt.ttf", 36);// , Size::ZERO, TextHAlignment::RIGHT, TextVAlignment::BOTTOM);
	sprintf(tmpStr, "Game%d", nGuankaCnt + 1);
	labelMsg->setString(tmpStr);
	labelMsg->setColor(Color3B(0xFF, 0, 0));
	labelMsg->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - labelMsg->getContentSize().height));
	this->addChild(labelMsg);
	mButton = Button::create();
	//mButton->setTitleFontName("fonts/Marker Felt.ttf");
	//mButton->setTitleText("BackStep");
	mButton->setTitleFontName("Arial");
	mButton->setTitleText(((String*)chnStrings->objectForKey("backstep"))->getCString());
	//sprintf(tmpStr, "BackStep:%d", (10 - nMaxBack) / 2); mButton->setTitleText(tmpStr); 
	mButton->setEnabled(false);
	mButton->setColor(Color3B(0x80, 0x80, 0x80));
	mButton->setTitleFontSize(32);
	//mButton->setPosition(Vec2(origin.x + visibleSize.width - mButton->getContentSize().width*2, origin.y + visibleSize.height - mButton->getContentSize().height * 1));
	mButton->setPosition(Vec2(origin.x + mButton->getContentSize().width/2, origin.y + visibleSize.height - mButton->getContentSize().height * 1));
	mButton->addClickEventListener([this, mButton](Ref* sender) {
		if (bWin == false && nWayAuto == 0 && nBackCnt > 0)
		{
			int boxCur, boxNew;
			--nBackCnt;
			boxNew = szBackRec[nBackCnt];
			boxCur = boxNew / (MapSize*MapSize); boxNew %= (MapSize*MapSize);
			pSp[boxNew] = pSp[boxCur]; pSp[boxCur] = NULL;
			map[boxCur] &= ~eBox; map[boxNew] |= eBox;
			auto action = Sequence::create(
				MoveTo::create(MovTime, Vec2(fXOffset + (boxNew % MapSize - 1) * picSize + 0.5f*picSize, fYOffset + (boxNew / MapSize - 1) * picSize + 0.5f*picSize)),
				CallFunc::create(CC_CALLBACK_0(HelloWorld::MovBoxCallBack, this, boxNew)),
				nullptr);
			pSp[boxNew]->runAction(action);
			map[playerPos] &= ~eMan; playerPos = boxNew * 2 - boxCur; map[playerPos] |= eMan;
			pMan->setPosition(Vec2(fXOffset + (playerPos % MapSize - 1) * picSize + 0.5f*picSize, fYOffset+(playerPos / MapSize - 1) * picSize + 0.5f*picSize));


			if (nBackCnt)
			{
				mButton->setEnabled(true);
				mButton->setColor(Color3B(0xFF, 0xFF, 0xFF));
			}
			else
			{
				mButton->setEnabled(false);
				mButton->setColor(Color3B(0x80, 0x80, 0x80));
			}
		}
	});
	addChild(mButton, 0, 1024);

	auto pMenuLayer = Layer::create();


	mButton = Button::create();
	//mButton->setTitleFontName("fonts/Marker Felt.ttf");
	//sprintf(tmpStr, "Box:%d  ", nGuankaCnt + 1);
	//mButton->setTitleText(tmpStr);
	mButton->setTitleFontName("Arial");
	mButton->setTitleText(((String*)chnStrings->objectForKey("menu"))->getCString());
	mButton->setTitleFontSize(32);
	mButton->setPosition(Vec2(origin.x + visibleSize.width - mButton->getContentSize().width/2, origin.y + visibleSize.height - mButton->getContentSize().height * 1));
	mButton->addClickEventListener([this](Ref* sender) {
#if 0
		Button *pTmpBtn = (Button*)sender;
		pTmpBtn->setVisible(false);
		Layer *pLayer = (Layer *)getChildByTag(1025);
		pLayer->setVisible(true);
		scheduleOnce((SEL_SCHEDULE)&HelloWorld::disableMenu, 4.0f);
#else
		auto director = Director::getInstance();
		director->pushScene(GameSel::createScene());
	
#endif
	});
	addChild(mButton, 0, 1026);

#if 0
	_mouseListener = EventListenerMouse::create();
	_mouseListener->onMouseDown = CC_CALLBACK_1(HelloWorld::onMouseDown, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);
#else
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
	//listener->onTouchesMoved = CC_CALLBACK_2(ForceTouchTest::onTouchesMoved, this);
	//listener->onTouchesEnded = CC_CALLBACK_2(ForceTouchTest::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this); 
#endif
    return true;
}

void HelloWorld::disableMenu(float time)
{
	auto menu = getChildByTag(1025);
	menu->setVisible(false);
	menu = getChildByTag(1026);
	menu->setVisible(true);
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

const int movTab[] = { -1, MapSize, 1, -MapSize };
void HelloWorld::dirButtonEvent(Ref *pSender, Widget::TouchEventType type, int dir)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
	{
		int tmp;
		tmp=playerPos+movTab[dir];
		if (tmp >= 0 && tmp<MapSize*MapSize && nWayAuto == 0 && bWin==false)
		{
			if (map[tmp] & eBox)PushBox(tmp);
			else				FindWay(tmp);
		}
	}
	break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void HelloWorld::PushBox(int boxPos)
{
	if (((playerPos == (boxPos + 1) || playerPos == (boxPos - 1))
		&& ((playerPos / MapSize) == (boxPos / MapSize)))		//左右相邻
		|| (((playerPos %MapSize) == (boxPos%MapSize))
		&& (playerPos == boxPos + MapSize || playerPos == boxPos - MapSize)))	//上下相邻
	{
		int temp;
		temp = boxPos * 2 - playerPos;
		if (map[temp] <eWall)
		{
			Button *pBtn = (Button*)getChildByTag(1024);
			if (nBackCnt<sizeof(szBackRec) / sizeof(szBackRec[0])){
				szBackRec[nBackCnt++] = temp*(MapSize*MapSize)+ boxPos;
			}
			else
			{
				int i;
				for (i = 0; i < sizeof(szBackRec) / sizeof(szBackRec[0])-1; i++)		szBackRec[i] = szBackRec[i + 1];
				szBackRec[i] = temp*(MapSize*MapSize) + boxPos;
			}
			pBtn->setEnabled(true); pBtn->setColor(Color3B(0xFF, 0xFF, 0xFF));
			map[playerPos] &= ~eMan;
			map[boxPos] |= eMan;
			map[boxPos] &= ~eBox;
			map[temp] |= eBox;
			playerPos = boxPos;
			++nWayAuto;
			auto action = Sequence::create(
				MoveTo::create(MovTime*2.0f, Vec2(fXOffset + (boxPos % MapSize - 1) * picSize + 0.5f*picSize, fYOffset + (boxPos / MapSize - 1) * picSize + 0.5f*picSize)),
				CallFunc::create(CC_CALLBACK_0(HelloWorld::AutoWayCallBack, this)),
				nullptr);
			pMan->runAction(action);
			pSp[temp] = pSp[boxPos]; pSp[boxPos] = NULL;
			action = Sequence::create(
				MoveTo::create(MovTime, Vec2(fXOffset + (temp % MapSize - 1) * picSize + 0.5f*picSize, fYOffset + (temp / MapSize - 1) * picSize + 0.5f*picSize)),
				CallFunc::create(CC_CALLBACK_0(HelloWorld::MovBoxCallBack, this, temp)),
				nullptr);
			pSp[temp]->runAction(action);
#if 0
			if (nMaxBack & 1)
			{
				if (++nMaxBack >= 10)	removeChildByTag(1024);
				else
				{
				}
			}
#endif
		}
	}
}

void HelloWorld::FindWay(int dest)
{
#if 1
	#define MaxWay 100
	typedef struct
	{
		int addr;
		int farther;
	}myway; 	myway way[MaxWay];
	int waybegin = 0, wayend = 1;
	int map_temp[MapSize*MapSize];
	unsigned long i;
	for (i = 0; i<MapSize*MapSize; i++)	map_temp[i] = map[i];

	way[0].addr = playerPos;									//设置起始点
	while (waybegin<wayend)
	{
		if (way[waybegin].addr/MapSize)							//向上查找
		{
			if (map_temp[way[waybegin].addr - MapSize] <eWall)		//可以嵌入
			{
				way[wayend].addr = way[waybegin].addr - MapSize;
				way[wayend].farther = waybegin;
				if (way[wayend].addr == dest)	break;			//找到目标
				map_temp[way[wayend].addr] = eWall;
				if (++wayend >= MaxWay)			break;			//寻找失败
			}
		}

		if (way[waybegin].addr %MaxWay)							//向左边寻找
		{
			if (map_temp[way[waybegin].addr - 1] <eWall)			//可以嵌入
			{
				way[wayend].addr = way[waybegin].addr - 1;
				way[wayend].farther = waybegin;
				if (way[wayend].addr == dest)	break;			//找到目标
				map_temp[way[wayend].addr] = eWall;
				if (++wayend >= MaxWay)	break;					//寻找失败
			}
		}

		if (way[waybegin].addr<(MapSize*MapSize) - MapSize)			//向下寻找
		{
			if (map_temp[way[waybegin].addr + MapSize] <eWall)		//可以嵌入
			{
				way[wayend].addr = way[waybegin].addr + MapSize;
				way[wayend].farther = waybegin;
				if (way[wayend].addr == dest)	break; //找到目标
				map_temp[way[wayend].addr] = eWall;
				if (++wayend >= MaxWay)	break;		//寻找失败
			}
		}

		if ((way[waybegin].addr %MapSize) != (MapSize-1))			//向左边寻找
		{
			if (map_temp[way[waybegin].addr + 1] <eWall)		//可以嵌入
			{
				way[wayend].addr = way[waybegin].addr + 1;
				way[wayend].farther = waybegin;
				if (way[wayend].addr == dest)	break; //找到目标
				map_temp[way[wayend].addr] = eWall;
				if (++wayend >= MaxWay)	break;		//寻找失败
			}
		}
		++waybegin;
	}
	if (waybegin<wayend)				//招到目标
	{
		map[playerPos] &= ~eMan;
		playerPos = dest;
		map[playerPos] |= eMan;
		while (wayend)				//逆运算
		{
			++nWayAuto;
			szWayAuto[nWayAuto] = way[wayend].addr;
			wayend = way[wayend].farther;
		}
		auto action = Sequence::create(
			MoveTo::create(MovTime, Vec2(fXOffset + (szWayAuto[nWayAuto] % MapSize - 1) * picSize + 0.5f*picSize, fYOffset + (szWayAuto[nWayAuto] / MapSize - 1) * picSize + 0.5f*picSize)),
			CallFunc::create(CC_CALLBACK_0(HelloWorld::AutoWayCallBack,this)),
			nullptr);
		pMan->runAction(action);
	}
#endif
}

void HelloWorld::AutoWayCallBack(void)
{
	if (--nWayAuto)
	{
		auto action = Sequence::create(
			MoveTo::create(MovTime, Vec2(fXOffset + (szWayAuto[nWayAuto] % MapSize - 1) * picSize + 0.5f*picSize, fYOffset + (szWayAuto[nWayAuto] / MapSize - 1) * picSize + 0.5f*picSize)),
			CallFunc::create(CC_CALLBACK_0(HelloWorld::AutoWayCallBack, this)),
			nullptr);
		pMan->runAction(action);
	}
}

void HelloWorld::MovBoxCallBack(int boxPos)
{
	if (map[boxPos] == (eBox | eBall))
	{
		int i;
		pSp[boxPos]->setSpriteFrame("box1.png");
		for (i = 0; i < MapSize*MapSize; i++)
		{
			if ((map[i] & eBall) && (map[i] != (eBall | eBox)))
				break;
		}
		if (i == MapSize*MapSize)
		{
			Size visibleSize = Director::getInstance()->getVisibleSize();
			Vec2 origin = Director::getInstance()->getVisibleOrigin();
			auto winlabel = Label::createWithTTF("You Win", "fonts/Marker Felt.ttf", 48, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
			winlabel->setPosition(Vec2(origin.x +visibleSize.width / 2, origin.y + visibleSize.height / 2)); winlabel->setColor(Color3B(0xFF, 0xFF, 0));
			++nGuankaCnt;

			int err;
			FILE *fp;
			//err = fopen_s(&fp, "box.rec", "rb+");
			String path = FileUtils::getInstance()->getWritablePath() + "box.rec";
			fp = fopen(path.getCString(), "rb+");
			fwrite(&nGuankaCnt, sizeof(nGuankaCnt), 1, fp);
			if (nGuankaCnt > nGuankaWin)
			{
				++nGuankaWin;
				fwrite(&nGuankaWin, sizeof(nGuankaWin), 1, fp);
			}
			fclose(fp);

			auto action = Sequence::create(FadeIn::create(0.5), ScaleTo::create(0.5f, 1.5f), ScaleTo::create(0.5f, 1.0f), ScaleTo::create(0.5f, 1.5f), ScaleTo::create(0.5f, 1.0f), ScaleTo::create(0.5f, 1.5f), ScaleTo::create(0.5f, 1.0f), FadeOut::create(1),
				CallFunc::create(CC_CALLBACK_0(HelloWorld::ResetGame, this)),
				nullptr);
			winlabel->setOpacity(0);
			winlabel->runAction(action);
			addChild(winlabel,1);
		}
	}
	else pSp[boxPos]->setSpriteFrame("box0.png");
}

void HelloWorld::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
#if 1
	auto t = touches[0];
	Vec2 tmp=t->getLocation();
	if (nWayAuto == 0 && bWin == false && tmp.y>fYOffset && tmp.x >= fXOffset)
	{
		int x, y;
		tmp.y -= fYOffset;
		tmp.x -= fXOffset;
		x = (int)tmp.x / picSize+1; y = (int)tmp.y / picSize+1;
		if (x >= 0 && x < MapSize && y >= 0 && y < MapSize)
		{
			int tmp = x + y*MapSize;
			if (map[tmp] & eBox)PushBox(tmp);
			else				FindWay(tmp);
		}
	}
#else
	char formatBuffer[256];
	for (auto& t : touches)
	{
#if 0
		float currentForce = t->getCurrentForce();
		float maxForce = t->getMaxForce();
		sprintf(formatBuffer, "Current force value : %0.02f, maximum possible force : %0.02f", currentForce, maxForce);
		_infoLabel->setString(std::string(formatBuffer));
#else
		Vec2 tmp=t->getLocation();
		sprintf(formatBuffer, "%f %f", tmp.x, tmp.y);
		labelMsg->setString(formatBuffer);
#endif
	}
#endif
}
void HelloWorld::onMouseDown(Event *event)
{
	EventMouse* e = (EventMouse*)event;
	if (e->getMouseButton() == 0 && nWayAuto == 0 && bWin==false)
	{
		int x, y;
		x = (int)e->getCursorX() / picSize; y = (int)e->getCursorY() / picSize;
		if (x >= 0 && x < MapSize && y >= 0 && y < MapSize)
		{
			int tmp = x + y*MapSize;
			if (map[tmp] & eBox)PushBox(tmp);
			else				FindWay(tmp);
		}
	}
}

void HelloWorld::ResetGame(void)
{
	if (nGuankaCnt < 68)
	{
		auto scene = HelloWorld::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}




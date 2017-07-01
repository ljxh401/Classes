#include "AppDelegate.h"
#include "CCFileUtils.h"
#include "HelloWorldScene.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(600, 1080);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("cct1", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("cct1");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
   // director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // Set the design resolution
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
#if 0
    Size frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }
#endif
    register_all_packages();

#if 1
	//int err;
	FILE *fp;
	//err = fopen_s(&fp, "box.rec", "rb+");
	//if (err==0)
	String path = FileUtils::getInstance()->getWritablePath() + "box.rec";
	fp = fopen(path.getCString(), "rb+");
	if (fp!=NULL)
	{
		fread(&HelloWorld::nGuankaCnt, sizeof(HelloWorld::nGuankaCnt), 1, fp);
		fread(&HelloWorld::nGuankaWin, sizeof(HelloWorld::nGuankaWin), 1, fp);
		fclose(fp);
	}else
	{
		fp = fopen(path.getCString(), "wb+");
		fwrite(&HelloWorld::nGuankaCnt, sizeof(HelloWorld::nGuankaCnt), 1, fp);
		fwrite(&HelloWorld::nGuankaWin, sizeof(HelloWorld::nGuankaWin), 1, fp);
		fclose(fp);
	}
#else
	auto fu =FileUtils::getInstance();
	auto dat=fu->getDataFromFile("box.rec");
	unsigned char *pTmp = dat.getBytes();
	memcpy(&HelloWorld::nGuankaCnt, pTmp, 4);
	memcpy(&HelloWorld::nGuankaWin, pTmp + 4, 4);
#endif
	//if (HelloWorld::nGuankaWin < 15 || HelloWorld::nGuankaWin >= 68)	
		HelloWorld::nGuankaWin = 67;
	if (HelloWorld::nGuankaCnt > HelloWorld::nGuankaWin || HelloWorld::nGuankaCnt < 0) HelloWorld::nGuankaCnt = HelloWorld::nGuankaWin;
    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

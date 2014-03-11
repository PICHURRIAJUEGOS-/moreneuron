#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "PresentationScene.h"
#include "NeuronMenuScene.h"
#include "MoreNeuronScene.h"
#include "GameOverScene.h"
#include "HelpScene.h"
#include "Config.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto eglView = EGLView::getInstance();
    director->setOpenGLView(eglView);
    //@todo como hacer proporcional??
    //eglView->setFrameSize(720, 1280);
    Size screenSize = eglView->getFrameSize();
    Size designSize = Size(768, 1280);

    //Size designSize = Size(,640);
    CCLog("Screen(%f, %f)\n", screenSize.width, screenSize.height);

    Size smallSize = Size(320, 480);
    Size mediumSize = Size(768, 1024);
    Size largeSize = Size(1536, 2048);
    
    
    EGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::FIXED_HEIGHT);
    BigScreen = false;
    if(screenSize.height >= mediumSize.height) {
      BigScreen = true;
      director->setContentScaleFactor(largeSize.height/designSize.height);
    }else if(screenSize.height >= smallSize.height) {
      director->setContentScaleFactor(mediumSize.height/designSize.height);
    }else {
      director->setContentScaleFactor(smallSize.height/designSize.height);
    }
    Director::sharedDirector()->setContentScaleFactor(1.0f);
    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = Presentation::createScene();
    // run
    director->runWithScene(scene);
    ADMOB_NEURON_LOAD;
    ADMOB_NEURON_SHOW_TOP;
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

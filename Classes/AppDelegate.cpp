#include "AppDelegate.h"
#include "LoadingScene.h"
USING_NS_CC;

static Size designResolutionSize = Size(1280, 960); //分辨率
static Size smallResolutionSize = Size(480, 320);
static Size mediumResolutionSize = Size(1024, 768);
static Size largeResolutionSize = Size(2048, 1536);

#define USE_SIMPLE_AUDIO_ENGINE 1 //音频引擎
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;

/* 构造函数 */
AppDelegate::AppDelegate() {

}

/* 析构函数 */
AppDelegate::~AppDelegate() {
    SimpleAudioEngine::end(); //结束音频引擎的使用
}

/* 设置OpenGL上下文的属性 */
void AppDelegate::initGLContextAttrs() {
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0}; 
    //创建GLContextAttrs结构体 设置其成员的值来定义OpenGL上下文的属性
    //属性：red green blue alpha depth stencil multisamplesCount
    GLView::setGLContextAttrs(glContextAttrs);
    //将定义好的属性应用到OpenGL视图中
}

/* 注册所有的包（packages）*/
static int register_all_packages() {
    return 0; //包管理器（package manager）的标志值
}

/* 初始化Director和场景 */
bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance(); //初始化Director
    auto glview = director->getOpenGLView(); //获取当前OpenGL视图

    if(!glview) { //当前没有OpenGL视图
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX) //Windows Mac Linux
        glview = GLViewImpl::createWithRect("Jinchanchan", Rect(0, 0, designResolutionSize.width, designResolutionSize.height)); //创建一个OpenGL视图
#else
        glview = GLViewImpl::create("Jinchanchan");
#endif
        director->setOpenGLView(glview); //将创建的OpenGL设为Director的当前OpenGL视图
    }

    //director->setDisplayStats(true); //打开显示帧率的设置
    director->setAnimationInterval(1.0f / 60); //每秒60帧

    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER); //设置分辨率
    auto frameSize = glview->getFrameSize(); //获取当前OpenGL视图大小
   
    if (frameSize.height > mediumResolutionSize.height)
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    else if (frameSize.height > smallResolutionSize.height) 
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    else //≤
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    
    register_all_packages(); //注册所有packages

    auto loading = LoadingScene::createScene(); //创建一个LoadingScene场景
    director->runWithScene(loading); //运行Director 将scene设为当前运行场景

    return true;
}

/* 程序进入后台时调用 */
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation(); //停止动画渲染 即暂停游戏画面的更新

    SimpleAudioEngine::getInstance()->pauseBackgroundMusic(); //暂停背景音乐
    SimpleAudioEngine::getInstance()->pauseAllEffects(); //暂停所有音效
}

/* 程序重新进入前台时调用 */
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation(); //开始动画渲染 即恢复游戏画面的更新

    SimpleAudioEngine::getInstance()->resumeBackgroundMusic(); //恢复背景音乐的播放
    SimpleAudioEngine::getInstance()->resumeAllEffects(); //恢复所有音效的播放
}
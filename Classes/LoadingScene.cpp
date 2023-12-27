#include "LoadingScene.h"
#include "SimpleAudioEngine.h"
#include "StartGame.h"
using namespace CocosDenshion;
USING_NS_CC;

/* 创建一个Scene对象 */
Scene* LoadingScene::createScene() {
    return LoadingScene::create();
}

/* 初始化loading场景内容 */
bool LoadingScene::init() {
    if (!Scene::init()) //初始化
        return false; //初始化失败

    auto visibleSize = Director::getInstance()->getVisibleSize(); //屏幕可见区域的大小
    Vec2 origin = Director::getInstance()->getVisibleOrigin(); //原点坐标    

    /* 欢迎标签 */
    auto label = Label::createWithTTF("Welcome to the game!\n        Loading ......", "fonts/Marker Felt.ttf", 75); //创建标签
    label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 2 * visibleSize.height / 3)); //标签位置
    label->setTextColor(Color4B::BLACK); //设置颜色
    this->addChild(label, 1); //添加到场景中

    /* 背景精灵 */
    auto background = Sprite::create("Loading.png");
    background->setContentSize(Size(visibleSize.width, visibleSize.height));
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(background, 0);

    /* 进度条背景 */
    auto loadingBar2 = Sprite::create("loadingbar2.png");
    loadingBar2->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    loadingBar2->setScale(2);
    this->addChild(loadingBar2, 0);

    /* 音效预加载 */
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("backgrondmusic.mp3");//背景音乐
    //SimpleAudioEngine::getInstance()->preloadEffect(".");//音效

    /* 进度条内容 */
    auto loadingBar1 = ProgressTimer::create(Sprite::create("loadingbar1.png")); //创建进度条
    loadingBar1->setType(ProgressTimer::Type::BAR); //设为条形
    loadingBar1->setScale(2); //放大2倍
    loadingBar1->setMidpoint(Vec2(0, 0)); //进度条的起点位置
    loadingBar1->setBarChangeRate(Vec2(1, 0)); //进度条的变化率
    loadingBar1->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2)); //进度条的位置
    this->addChild(loadingBar1, 0); //将进度条添加到场景中

    loadingBar1->setPercentage(0); //初始百分比为0
    double duration = 1.0; //加载时间2s
    /* 创建定时器 每帧逐渐增加进度 */
    this->schedule([=](double deltaTime) {
        double currentPercent = loadingBar1->getPercentage(); //当前百分比
        double increment = (100 / duration) * deltaTime; //每帧增加的百分比
        double newPercent = currentPercent + increment; //计算新的百分比
        loadingBar1->setPercentage(newPercent); //更新进度条的百分比

        if (newPercent >= 100) { //加载完成
            this->unschedule("loading");

            /* 切换到开始游戏主界面 */
            auto newScene = StartGameScene::create(); //创建开始游戏场景对象
            Director::getInstance()->replaceScene(newScene); //切换场景
        }
    }, "loading");

    return true;
}
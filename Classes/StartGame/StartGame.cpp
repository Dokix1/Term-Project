#include "SimpleAudioEngine.h"
#include "StartGame.h"
#include "CreateRoomScene.h"
#include "SetMusicScene.h"
using namespace ui;
using namespace CocosDenshion;
USING_NS_CC;

/* 创建一个Scene对象 */
Scene* StartGameScene::createScene() {
    return StartGameScene::create();
}

/* 点击后退出游戏 */
void StartGameScene::menuCloseCallback(Ref* pSender) {
    //SimpleAudioEngine::sharedEngine()->end(); //释放所有声音资源
    Director::getInstance()->end(); //结束游戏循环

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0); //iOS退出
#endif
}

/* 点击后创建房间 */
void StartGameScene::menuCreateRoomCallback(Ref* pSender) {
    auto newScene = CreateRoomScene::create(); //创建游戏场景
    Director::getInstance()->replaceScene(newScene); //切换到创建房间场景
}

/* 点击后调节音效 */
void StartGameScene::menuSetMusicCallback(Ref* pSender) {
    auto newScene = SetMusicScene::create();
    Director::getInstance()->pushScene(newScene); //切换到调节音效场景 当前场景放入场景栈中
}

/* 初始化StartGameScene场景内容 */
bool StartGameScene::init() {
    if (!Scene::init()) //初始化
        return false; //初始化失败

    auto visibleSize = Director::getInstance()->getVisibleSize(); //屏幕可见区域的大小
    Vec2 origin = Director::getInstance()->getVisibleOrigin(); //原点坐标    

    /* 背景精灵 */
    auto background = Sprite::create("Background/startgame.png");
    background->setContentSize(Size(visibleSize.width, visibleSize.height));
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(background, 0);

    /* 退出程序菜单项 */
    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
        CC_CALLBACK_1(StartGameScene::menuCloseCallback, this));
    float scale = 5.0;
    closeItem->setScale(4); //放大4倍
    float x = origin.x + visibleSize.width - closeItem->getContentSize().width * scale / 2;
    float y = origin.y + closeItem->getContentSize().height * scale / 2;
    closeItem->setPosition(Vec2(x, y)); //设置显示位置

    /* 开始游戏菜单项 */
    auto startItem = MenuItemImage::create("Buttons/Start.png", "Buttons/Start.png",
        CC_CALLBACK_1(StartGameScene::menuCreateRoomCallback, this));
    startItem->setScale(1.3F);
    x = origin.x + visibleSize.width / 6;
    y = origin.y + visibleSize.height / 5;
    startItem->setPosition(Vec2(x, y));

    /* 设置音效菜单项 */
    auto setMusic = MenuItemImage::create("Music/setting.png", "Music/setting.png",
        CC_CALLBACK_1(StartGameScene::menuSetMusicCallback, this));
    setMusic->setScale(0.5);
    x = origin.x + 11 * visibleSize.width / 12;
    y = origin.y + 8 * visibleSize.height / 9;
    setMusic->setPosition(Vec2(x, y));

    /* 创建菜单 */
    auto menu = Menu::create(closeItem, startItem, setMusic, nullptr); //添加菜单项
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    return true;
}
class PopupLayer : public cocos2d::Layer {
public:
    virtual bool init() override;
    CREATE_FUNC(PopupLayer);

    void show();
    void hide();

private:
    void onBackgroundClicked(cocos2d::Ref* sender);
    void onButtonClicked(cocos2d::Ref* sender);

    cocos2d::Menu* buttonMenu;
};
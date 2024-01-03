#include "SimpleAudioEngine.h"
#include "CreateRoomScene.h"
#include "../PlayingScene/PlayingScene.h"
#include "../StartGame/StartGame.h"
#include "../SetMusic/SetMusicScene.h"
USING_NS_CC;

/* 创建一个Scene对象 */
Scene* CreateRoomScene::createScene() {
    return CreateRoomScene::create();
}
/* 点击后调节音效 */
void CreateRoomScene::menuSetMusicCallback(Ref* pSender) {

    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/click.wav");
    auto newScene = SetMusicScene::create();
    Director::getInstance()->pushScene(newScene); //切换到调节音效场景 当前场景放入场景栈中
}

/* 点击后返回主界面 */
void CreateRoomScene::menuCloseCallback(Ref* pSender) {

    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/click.wav");
    auto newScene = StartGameScene::create(); //主界面
    Director::getInstance()->replaceScene(newScene); //切换到主界面
}

/* 点击后进入游戏界面 */
void CreateRoomScene::menuStartGameCallback(Ref* pSender) {

    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/click.wav");
    auto newScene = PlayingScene::create(); //游戏界面
    Director::getInstance()->replaceScene(newScene); //切换到游戏界面
}

/* 初始化CreateRoomScene场景内容 */
bool CreateRoomScene::init() {
    if (!Scene::init()) //初始化
        return false; //初始化失败

    auto visibleSize = Director::getInstance()->getVisibleSize(); //屏幕可见区域的大小
    Vec2 origin = Director::getInstance()->getVisibleOrigin(); //原点坐标    

    /* 背景精灵 */
    auto background = Sprite::create("Background/createRoom.jpg");
    background->setContentSize(Size(visibleSize.width, visibleSize.height));
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(background, 0);

    /* 退出程序菜单项 */
    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
        CC_CALLBACK_1(CreateRoomScene::menuCloseCallback, this));
    float scale = 5.0;
    closeItem->setScale(4); //放大4倍
    float x = origin.x + visibleSize.width - closeItem->getContentSize().width * scale / 2;
    float y = origin.y + closeItem->getContentSize().height * scale / 2;
    closeItem->setPosition(Vec2(x, y)); //设置显示位置

    /* 开始游戏菜单项 */
    auto startItem = MenuItemImage::create("Buttons/Start.png", "Buttons/Start.png",
        CC_CALLBACK_1(CreateRoomScene::menuStartGameCallback, this));
    startItem->setScale(1.3F);
    x = origin.x + 2 * visibleSize.width / 3;
    y = origin.y + visibleSize.height / 5;
    startItem->setPosition(Vec2(x, y));

    /* 设置音效菜单项 */
    auto setMusic = MenuItemImage::create("Music/setting.png", "Music/setting.png",
        CC_CALLBACK_1(CreateRoomScene::menuSetMusicCallback, this));
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
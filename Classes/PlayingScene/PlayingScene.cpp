#include "SimpleAudioEngine.h"
#include "PlayingScene.h"
#include "StartGame.h"
#include "SetMusicScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;
Button* shopbutton;
Button* upbutton;
Button* rebutton;
// 声明监听器对象作为成员变量或全局变量
EventListenerTouchOneByOne* listener;


class PopupLayer : public LayerColor
{
public:
    CREATE_FUNC(PopupLayer);
  
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
    {
        // 获取触摸位置
        cocos2d::Vec2 touchPos = touch->getLocation();
        // 判断触摸位置是否在弹窗内部
        if ((140 <= touchPos.x && touchPos.x <= 1140 && 643 <= touchPos.y && touchPos.y <= 893) ||
            (1026.25 <= touchPos.x && touchPos.x <= 1213.75 && 26.25 <= touchPos.y && touchPos.y <= 213.75)) {
            // 触摸在弹窗内部，不关闭弹窗，继续传递触摸事件
            return false;
        }
        else {
            // 触摸在弹窗外部，关闭弹窗
            hide();
            Director::getInstance()->getEventDispatcher()->removeEventListener(listener);
            shopbutton->setEnabled(true);
            shopbutton->loadTextures("buttons/ShopNormal.png", "buttons/ShopSelected.png", "buttons/ShopSelected.png");
            return false;  // 返回 true 表示已处理触摸事件
        }
    }
    bool init()
    {
        if (!LayerColor::initWithColor(Color4B(0, 0, 0,0)))
        {
            return false;
        }
        // 创建弹窗背景
        auto popupBg = Sprite::create("ChessBoard/Cards.png");
        auto visibleSize = Director::getInstance()->getVisibleSize();
        popupBg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*4/5));
        addChild(popupBg);
        // 点击屏幕外关闭
        listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = CC_CALLBACK_2(PopupLayer::onTouchBegan, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

        // 创建内部按钮
        rebutton = Button::create("buttons/RefreshNormal.png", "buttons/RefreshSelected.png");
        rebutton->setPosition(Vec2(0.875 * visibleSize.width, 0.400 * visibleSize.height));
        rebutton->addClickEventListener(CC_CALLBACK_1(PopupLayer::reonButtonClicked, this));
        addChild(rebutton);
        const int buttonCount = 7;
        for (int i = 0; i < buttonCount; ++i)
        {
            auto button = Button::create("inner_button_normal.png", "inner_button_pressed.png");
            button->setPosition(Vec2(100, 400 - i * 80));
            button->addClickEventListener(CC_CALLBACK_1(PopupLayer::onInnerButtonClicked, this));
            popupBg->addChild(button);
        }

        return true;
    }
    void reonButtonClicked(Ref* sender) {
    };
    void onInnerButtonClicked(Ref* sender)
    {
    }
    void hide()
    {
        removeFromParent();
    }
};


/* 创建一个Scene对象 */
Scene* PlayingScene::createScene() {
    return PlayingScene::create();
}

/* 点击后返回主界面 */
void PlayingScene::menuCloseCallback(Ref* pSender) {
    auto newScene = StartGameScene::create(); //主界面
    Director::getInstance()->replaceScene(newScene); //切换到主界面
}
void PlayingScene::shoponButtonClicked(Ref* sender)
{

    shopbutton->setEnabled(false);
    shopbutton->loadTextures("buttons/ShopSelected.png", "buttons/ShopSelected.png", "buttons/ShopSelected.png");
    auto popupLayer = PopupLayer::create();
    this->addChild(popupLayer);
}
void PlayingScene::uponButtonClicked(Ref* sender) {}
/*倒计时*/
void PlayingScene::updateProgressBar(float dt) {
    // 计算剩余时间
    currentTime -= dt;
    if (currentTime < 0) {
        currentTime = 0;
    }

    // 更新进度条
    float progress = currentTime / totalTime;
    timer->setPercentage(progress * 100);

    // 更新时间标签
    int remainingTime = static_cast<int>(ceil(currentTime));
    timeLabel->setString(std::to_string(remainingTime));

    // 检查是否时间已经用完
    if (currentTime <= 0) {
        unschedule(schedule_selector(PlayingScene::updateProgressBar));
        // 在这里添加时间到达的处理逻辑
    }
}
/* 初始化PlayingScene场景内容 */
bool PlayingScene::init(){
    if (!Scene::init()) //初始化
        return false; //初始化失败
    auto visibleSize = Director::getInstance()->getVisibleSize(); //屏幕可见区域的大小
    Vec2 origin = Director::getInstance()->getVisibleOrigin(); //原点坐标    

    /* 背景精灵 */
    auto background = Sprite::create("playing.png");
    background->setContentSize(Size(visibleSize.width, visibleSize.height));
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(background, 0);

    /* 退出本局游戏菜单项 */
    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
                     CC_CALLBACK_1(PlayingScene::menuCloseCallback, this));
    float scale = 5.0;
    closeItem->setScale(4); //放大4倍
    float x = origin.x + visibleSize.width - closeItem->getContentSize().width * scale / 2;
    float y = origin.y + closeItem->getContentSize().height * scale / 2;
    closeItem->setPosition(Vec2(x, y)); //设置显示位置

    /* 创建菜单 */
    auto menu = Menu::create(closeItem, nullptr); //添加菜单项
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    // 创建按钮
    shopbutton = Button::create("buttons/ShopNormal.png", "buttons/ShopSelected.png");
    shopbutton->setPosition(Vec2(0.875*visibleSize.width, 0.125*visibleSize.height));
    shopbutton->addClickEventListener(CC_CALLBACK_1(PlayingScene::shoponButtonClicked, this));
    addChild(shopbutton);
    // 创建按钮
    upbutton = Button::create("buttons/UpgradeNormal.png", "buttons/UpgradeSelected.png");
    upbutton->setPosition(Vec2(0.125 * visibleSize.width, 0.130 * visibleSize.height));
    upbutton->addClickEventListener(CC_CALLBACK_1(PlayingScene::uponButtonClicked, this));
    addChild(upbutton);
    /*倒计时*/
    //进度条背景
    progressBackground = cocos2d::Sprite::create("Bar/countdownP.png");
    progressBackground->setPosition(visibleSize.width / 2, visibleSize.height / 40 * 39);
    addChild(progressBackground);
    // 创建进度条
    timer = cocos2d::ProgressTimer::create(cocos2d::Sprite::create("Bar/countdown.png"));
    timer->setType(cocos2d::ProgressTimer::Type::BAR);
    timer->setMidpoint(cocos2d::Vec2(0, 0.5));
    timer->setBarChangeRate(cocos2d::Vec2(1, 0));
    timer->setPosition(visibleSize.width/2,visibleSize.height/40*39);
    addChild(timer);

    // 创建时间标签
    timeLabel = cocos2d::Label::createWithTTF("", "fonts/arial.ttf", 24);
    timeLabel->setPosition(visibleSize.width*0.325, visibleSize.height / 40 * 39);
    addChild(timeLabel);

    // 初始化时间
    totalTime = 10.0f;
    currentTime = totalTime;

    // 启动定时器
    schedule(schedule_selector(PlayingScene::updateProgressBar), 0.01f);
    /*auto popupLayer = PopupLayer::create();
    addChild(popupLayer);*/

    return true;
}
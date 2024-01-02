#include "SimpleAudioEngine.h"
#include "SetMusicScene.h"
#include "StartGame.h"
#include "CreateRoomScene.h"
#include "PlayingScene.h"
using namespace CocosDenshion;
USING_NS_CC;

/* 创建一个Scene对象 */
Scene* SetMusicScene::createScene() {
    return SetMusicScene::create();
}

/* 点击后返回主界面 */
void SetMusicScene::menuCloseCallback(Ref* pSender) {
    Director::getInstance()->popScene();
}

/* 播放/暂停背景音乐 */
bool SetMusicScene::onSwitchMusicTouched(Touch* touch, Event* event) {
    /* 正在播放 */
    if (isMusicPlaying) {
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic(); //暂停音乐
        switchMusic->setTexture("Music/pausingMusic.png"); //切换为停止播放图片
    }
    /* 暂停状态 */
    else {
        SimpleAudioEngine::getInstance()->resumeBackgroundMusic(); //播放音乐
        switchMusic->setTexture("Music/playingMusic.png"); //切换回播放图片
    }
    isMusicPlaying = !isMusicPlaying; //切换音乐播放状态
    return true;
}

/* 音量变化 */
void SetMusicScene::volumeSliderCallback(Ref* sender, ui::Slider::EventType eventType) {
    if (eventType == ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
        ui::Slider* slider = dynamic_cast<ui::Slider*>(sender);
        int volume = slider->getPercent(); //音量值

        volumeLabel->setString(std::to_string(volume)); //更新滑动条百分比文本
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume / 100.0f); //设置背景音乐的音量
        //CCLOG("1111111111111: %f", SimpleAudioEngine::getInstance()->getBackgroundMusicVolume());
    }
}

/* 初始化PlayingScene场景内容 */
bool SetMusicScene::init() {
    if (!Scene::init()) //初始化
        return false; //初始化失败

    auto visibleSize = Director::getInstance()->getVisibleSize(); //屏幕可见区域的大小
    Vec2 origin = Director::getInstance()->getVisibleOrigin(); //原点坐标    

    /* 背景精灵 */
    auto background = Sprite::create("Music/setMusic.png");
    background->setContentSize(Size(visibleSize.width, visibleSize.height));
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(background, 0);

    isMusicPlaying = SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying(); //获取播放状态

    /* 播放/暂停图片 */
    if (isMusicPlaying) //正在播放
        switchMusic = Sprite::create("Music/playingMusic.png");
    else //暂停播放
        switchMusic = Sprite::create("Music/pausingMusic.png");
    switchMusic->setScale(1.5);
    switchMusic->setPosition(Vec2(visibleSize.width / 6 + origin.x, 2 * visibleSize.height / 3 + origin.y));
    this->addChild(switchMusic, 1);

    /* Label"Music" */
    auto label = Label::createWithTTF("Music", "fonts/Marker Felt.ttf", 45); //创建Label
    label->setPosition(Vec2(switchMusic->getPositionX(), switchMusic->getPositionY() + switchMusic->getContentSize().height / 2 + label->getContentSize().height / 2 + 10)); //Label位置在图片上方
    this->addChild(label, 1);

    /* 点击事件 */
    auto touchListener = EventListenerTouchOneByOne::create(); //创建触摸事件监听器
    touchListener->onTouchBegan = [this](Touch* touch, Event* event) {
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
        Rect rect = Rect(0, 0, target->getContentSize().width, target->getContentSize().height);
        if (rect.containsPoint(locationInNode)) { //在图片内点击时触发的逻辑
            onSwitchMusicTouched(touch, event); //调用回调函数
            return true;
        }
        return false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, switchMusic); //将监听器绑定到Sprite上

    /* 音量调节滑动条 */
    auto volumeSlider = ui::Slider::create(); //创建滑动条控件
    volumeSlider->loadBarTexture("Bar/sliderBar.png"); //滑动条的背景图片
    volumeSlider->loadSlidBallTextures("Music/sliderNormal.png", "Music/sliderNormal.png", ""); //默认状态图片 被按下时状态图片
    volumeSlider->loadProgressBarTexture("Bar/sliderProgress.png"); //进度条图片
    volumeSlider->setPosition(Vec2(switchMusic->getPositionX() + 2.2 * switchMusic->getContentSize().width + volumeSlider->getContentSize().width / 2 + 10, switchMusic->getPositionY()));
    int volume = static_cast<int>(100 * SimpleAudioEngine::getInstance()->getBackgroundMusicVolume()); //当前音量
    volumeSlider->setPercent(volume); //初始音量百分比
    volumeSlider->setScale(1.5); //将滑动条大小调整为1.5倍
    volumeSlider->addEventListener(CC_CALLBACK_2(SetMusicScene::volumeSliderCallback, this)); //滑动条的事件监听器 指定回调函数
    this->addChild(volumeSlider, 1);

    /* 滑动条百分比 */
    volumeLabel = Label::createWithTTF("60", "fonts/Marker Felt.ttf", 30);
    volumeLabel->setString(std::to_string(volume)); //音量显示
    volumeLabel->setPosition(Vec2(7 * visibleSize.width / 8 + origin.x, 2 * visibleSize.height / 3 + origin.y)); //初始位置
    this->addChild(volumeLabel, 1); //将标签添加到场景中

    /* 退出音效设置 */
    auto closeItem = MenuItemImage::create("Music/setReturn.png", "Music/setReturn.png",
        CC_CALLBACK_1(SetMusicScene::menuCloseCallback, this));
    float scale = 1.2F;
    closeItem->setScale(scale);
    float x = origin.x + closeItem->getContentSize().width * scale / 2;
    float y = origin.y + visibleSize.height - closeItem->getContentSize().height * scale / 2;
    closeItem->setPosition(Vec2(x, y)); //显示位置

    /* 创建菜单 */
    auto menu = Menu::create(closeItem, nullptr); //添加菜单项
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    return true;
}
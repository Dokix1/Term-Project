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
// ����������������Ϊ��Ա������ȫ�ֱ���
EventListenerTouchOneByOne* listener;


class PopupLayer : public LayerColor
{
public:
    CREATE_FUNC(PopupLayer);
  
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
    {
        // ��ȡ����λ��
        cocos2d::Vec2 touchPos = touch->getLocation();
        // �жϴ���λ���Ƿ��ڵ����ڲ�
        if ((140 <= touchPos.x && touchPos.x <= 1140 && 643 <= touchPos.y && touchPos.y <= 893) ||
            (1026.25 <= touchPos.x && touchPos.x <= 1213.75 && 26.25 <= touchPos.y && touchPos.y <= 213.75)) {
            // �����ڵ����ڲ������رյ������������ݴ����¼�
            return false;
        }
        else {
            // �����ڵ����ⲿ���رյ���
            hide();
            Director::getInstance()->getEventDispatcher()->removeEventListener(listener);
            shopbutton->setEnabled(true);
            shopbutton->loadTextures("buttons/ShopNormal.png", "buttons/ShopSelected.png", "buttons/ShopSelected.png");
            return false;  // ���� true ��ʾ�Ѵ������¼�
        }
    }
    bool init()
    {
        if (!LayerColor::initWithColor(Color4B(0, 0, 0,0)))
        {
            return false;
        }
        // ������������
        auto popupBg = Sprite::create("ChessBoard/Cards.png");
        auto visibleSize = Director::getInstance()->getVisibleSize();
        popupBg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*4/5));
        addChild(popupBg);
        // �����Ļ��ر�
        listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = CC_CALLBACK_2(PopupLayer::onTouchBegan, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

        // �����ڲ���ť
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


/* ����һ��Scene���� */
Scene* PlayingScene::createScene() {
    return PlayingScene::create();
}

/* ����󷵻������� */
void PlayingScene::menuCloseCallback(Ref* pSender) {
    auto newScene = StartGameScene::create(); //������
    Director::getInstance()->replaceScene(newScene); //�л���������
}
void PlayingScene::shoponButtonClicked(Ref* sender)
{

    shopbutton->setEnabled(false);
    shopbutton->loadTextures("buttons/ShopSelected.png", "buttons/ShopSelected.png", "buttons/ShopSelected.png");
    auto popupLayer = PopupLayer::create();
    this->addChild(popupLayer);
}
void PlayingScene::uponButtonClicked(Ref* sender) {}
/*����ʱ*/
void PlayingScene::updateProgressBar(float dt) {
    // ����ʣ��ʱ��
    currentTime -= dt;
    if (currentTime < 0) {
        currentTime = 0;
    }

    // ���½�����
    float progress = currentTime / totalTime;
    timer->setPercentage(progress * 100);

    // ����ʱ���ǩ
    int remainingTime = static_cast<int>(ceil(currentTime));
    timeLabel->setString(std::to_string(remainingTime));

    // ����Ƿ�ʱ���Ѿ�����
    if (currentTime <= 0) {
        unschedule(schedule_selector(PlayingScene::updateProgressBar));
        // ���������ʱ�䵽��Ĵ����߼�
    }
}
/* ��ʼ��PlayingScene�������� */
bool PlayingScene::init(){
    if (!Scene::init()) //��ʼ��
        return false; //��ʼ��ʧ��
    auto visibleSize = Director::getInstance()->getVisibleSize(); //��Ļ�ɼ�����Ĵ�С
    Vec2 origin = Director::getInstance()->getVisibleOrigin(); //ԭ������    

    /* �������� */
    auto background = Sprite::create("playing.png");
    background->setContentSize(Size(visibleSize.width, visibleSize.height));
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(background, 0);

    /* �˳�������Ϸ�˵��� */
    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
                     CC_CALLBACK_1(PlayingScene::menuCloseCallback, this));
    float scale = 5.0;
    closeItem->setScale(4); //�Ŵ�4��
    float x = origin.x + visibleSize.width - closeItem->getContentSize().width * scale / 2;
    float y = origin.y + closeItem->getContentSize().height * scale / 2;
    closeItem->setPosition(Vec2(x, y)); //������ʾλ��

    /* �����˵� */
    auto menu = Menu::create(closeItem, nullptr); //��Ӳ˵���
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    // ������ť
    shopbutton = Button::create("buttons/ShopNormal.png", "buttons/ShopSelected.png");
    shopbutton->setPosition(Vec2(0.875*visibleSize.width, 0.125*visibleSize.height));
    shopbutton->addClickEventListener(CC_CALLBACK_1(PlayingScene::shoponButtonClicked, this));
    addChild(shopbutton);
    // ������ť
    upbutton = Button::create("buttons/UpgradeNormal.png", "buttons/UpgradeSelected.png");
    upbutton->setPosition(Vec2(0.125 * visibleSize.width, 0.130 * visibleSize.height));
    upbutton->addClickEventListener(CC_CALLBACK_1(PlayingScene::uponButtonClicked, this));
    addChild(upbutton);
    /*����ʱ*/
    //����������
    progressBackground = cocos2d::Sprite::create("Bar/countdownP.png");
    progressBackground->setPosition(visibleSize.width / 2, visibleSize.height / 40 * 39);
    addChild(progressBackground);
    // ����������
    timer = cocos2d::ProgressTimer::create(cocos2d::Sprite::create("Bar/countdown.png"));
    timer->setType(cocos2d::ProgressTimer::Type::BAR);
    timer->setMidpoint(cocos2d::Vec2(0, 0.5));
    timer->setBarChangeRate(cocos2d::Vec2(1, 0));
    timer->setPosition(visibleSize.width/2,visibleSize.height/40*39);
    addChild(timer);

    // ����ʱ���ǩ
    timeLabel = cocos2d::Label::createWithTTF("", "fonts/arial.ttf", 24);
    timeLabel->setPosition(visibleSize.width*0.325, visibleSize.height / 40 * 39);
    addChild(timeLabel);

    // ��ʼ��ʱ��
    totalTime = 10.0f;
    currentTime = totalTime;

    // ������ʱ��
    schedule(schedule_selector(PlayingScene::updateProgressBar), 0.01f);
    /*auto popupLayer = PopupLayer::create();
    addChild(popupLayer);*/

    return true;
}
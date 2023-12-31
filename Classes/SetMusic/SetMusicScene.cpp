#include "SimpleAudioEngine.h"
#include "SetMusicScene.h"
#include "StartGame.h"
#include "CreateRoomScene.h"
#include "PlayingScene.h"
using namespace CocosDenshion;
USING_NS_CC;

/* ����һ��Scene���� */
Scene* SetMusicScene::createScene() {
    return SetMusicScene::create();
}

/* ����󷵻������� */
void SetMusicScene::menuCloseCallback(Ref* pSender) {
    auto newScene = StartGameScene::create(); //������
    Director::getInstance()->replaceScene(newScene); //�л���������
}

/* ����/��ͣ�������� */
bool SetMusicScene::onSwitchMusicTouched(Touch* touch, Event* event) {
    /* ���ڲ��� */
    if (isMusicPlaying) { 
        SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic(); //��ͣ����
        switchMusic->setTexture("pausingMusic.png");  //�л�Ϊֹͣ����ͼƬ
    }
    /* ��ͣ״̬ */
    else { 
        SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic(); //��������
        switchMusic->setTexture("playingMusic.png"); //�л��ز���ͼƬ
    }
    isMusicPlaying = !isMusicPlaying; //�л����ֲ���״̬
    return true;
}

/* �����仯 */
void SetMusicScene::volumeSliderCallback(Ref* sender, ui::Slider::EventType eventType) {
    if (eventType == ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
        ui::Slider* slider = dynamic_cast<ui::Slider*>(sender);
        int volume = slider->getPercent(); //����ֵ

        volumeLabel->setString(std::to_string(volume)); //���»������ٷֱ��ı�
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(volume / 100.0f); //���ñ������ֵ�����
    }
}

/* ��ʼ��PlayingScene�������� */
bool SetMusicScene::init() {
    if (!Scene::init()) //��ʼ��
        return false; //��ʼ��ʧ��

    auto visibleSize = Director::getInstance()->getVisibleSize(); //��Ļ�ɼ�����Ĵ�С
    Vec2 origin = Director::getInstance()->getVisibleOrigin(); //ԭ������    

    /* �������� */
    auto background = Sprite::create("setMusic.png");
    background->setContentSize(Size(visibleSize.width, visibleSize.height));
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(background, 0);

    isMusicPlaying = true; //���ڲ���
    
    /* ����/��ͣͼƬ */
    switchMusic = Sprite::create("playingMusic.png"); 
    switchMusic->setScale(1.5);
    switchMusic->setPosition(Vec2(visibleSize.width / 6 + origin.x, 2 * visibleSize.height / 3 + origin.y));
    this->addChild(switchMusic, 1);

    /* Label"Music" */
    auto label = Label::createWithTTF("Music", "fonts/Marker Felt.ttf", 45); //����Label
    label->setPosition(Vec2(switchMusic->getPositionX(), switchMusic->getPositionY() + switchMusic->getContentSize().height / 2 + label->getContentSize().height / 2 + 10)); //Labelλ����ͼƬ�Ϸ�
    this->addChild(label, 1);

    /* ����¼� */
    auto touchListener = EventListenerTouchOneByOne::create(); //���������¼�������
    touchListener->onTouchBegan = [this](Touch* touch, Event* event) {
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
        Rect rect = Rect(0, 0, target->getContentSize().width, target->getContentSize().height);
        if (rect.containsPoint(locationInNode)) { //��ͼƬ�ڵ��ʱ�������߼�
            onSwitchMusicTouched(touch, event); //���ûص�����
            return true;
        }
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, switchMusic); //���������󶨵�Sprite��

    /* �������ڻ����� */ 
    auto volumeSlider = ui::Slider::create(); //�����������ؼ�
    volumeSlider->loadBarTexture("sliderBar.png"); //�������ı���ͼƬ
    volumeSlider->loadSlidBallTextures("sliderNormal.png", "sliderNormal.png", ""); //Ĭ��״̬ͼƬ ������ʱ״̬ͼƬ
    volumeSlider->loadProgressBarTexture("sliderProgress.png"); //������ͼƬ
    volumeSlider->setPosition(Vec2(switchMusic->getPositionX() + 2.2 * switchMusic->getContentSize().width + volumeSlider->getContentSize().width / 2 + 10, switchMusic->getPositionY()));
    volumeSlider->setPercent(50); //��ʼ�����ٷֱ�
    volumeSlider->setScale(1.5); //����������С����Ϊ1.5��
    volumeSlider->addEventListener(CC_CALLBACK_2(SetMusicScene::volumeSliderCallback, this)); //���������¼������� ָ���ص�����
    this->addChild(volumeSlider, 1); 

    /* �������ٷֱ� */
    volumeLabel = Label::createWithTTF("60", "fonts/Marker Felt.ttf", 30);
    volumeLabel->setPosition(Vec2(7 * visibleSize.width / 8 + origin.x, 2 * visibleSize.height / 3 + origin.y)); //��ʼλ��
    this->addChild(volumeLabel,1); //����ǩ��ӵ�������

    /* �˳���Ч���� */
    auto closeItem = MenuItemImage::create("setReturn.png", "setReturn.png",
                     CC_CALLBACK_1(SetMusicScene::menuCloseCallback, this));
    float scale = 1.2;
    closeItem->setScale(scale); 
    float x = origin.x + closeItem->getContentSize().width * scale / 2;
    float y = origin.y + visibleSize.height - closeItem->getContentSize().height * scale / 2;
    closeItem->setPosition(Vec2(x, y)); //��ʾλ��

    /* �����˵� */
    auto menu = Menu::create(closeItem, nullptr); //��Ӳ˵���
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    return true;
}
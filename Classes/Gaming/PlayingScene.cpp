#include "SimpleAudioEngine.h"
#include "PlayingScene.h"
#include "StartGame.h"
#include "SetMusicScene.h"
USING_NS_CC;

/* ����һ��Scene���� */
Scene* PlayingScene::createScene() {
    return PlayingScene::create();
}

/* ����󷵻������� */
void PlayingScene::menuCloseCallback(Ref* pSender) {
    auto newScene = StartGameScene::create(); //������
    Director::getInstance()->replaceScene(newScene); //�л���������
}

/* ��ʼ��PlayingScene�������� */
bool PlayingScene::init() {
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

    return true;
}
#ifndef __PLAYING_SCENE_H__
#define __PLAYING_SCENE_H__

#include "cocos2d.h"
#include"../Hero/hero.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

/* PlayingScene�� */
class PlayingScene : public Scene {
public:
    static Scene* createScene(); //����һ��Scene
    virtual bool init(); //��ʼ����������
    cocos2d::ProgressTimer* timer; // ����������
    cocos2d::Sprite* progressBackground; // �������ײ�����
    cocos2d::Label* timeLabel; // ʣ��ʱ���ǩ
    float totalTime; // ��ʱ��
    float currentTime; // ��ǰʱ��
    bool isDragging;//����Ƿ������϶�
    cocos2d::Vec2 initialPosition;//��ʼλ��
    // ���½�������ʱ���ǩ
    void updateProgressBar(float dt);
    void menuCloseCallback(Ref* pSender);
    void menuSetMusicCallback(Ref* pSender);
    void shoponButtonClicked(Ref* sender);
    void uponButtonClicked(Ref* sender);
    void onMouseDown(EventMouse* event);
    void onMouseMove(EventMouse* event);
    void onMouseUp(EventMouse* event);
    CREATE_FUNC(PlayingScene); //�Զ�����һ��ʵ��

    void moveSpriteTo(Vec2 destination);
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    bool onTouchBeganLITTLE(Touch* touch, Event* event);
    static void randCard();

    //���������Ƕ���һ������ָ�롣
    CCSprite* m_pSprite;

};

#endif
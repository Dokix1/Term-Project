#ifndef __PLAYING_SCENE_H__
#define __PLAYING_SCENE_H__

#include "cocos2d.h"
#include "hero.h"
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
    // ���½�������ʱ���ǩ
    void updateProgressBar(float dt);
    void menuCloseCallback(Ref* pSender);
    void menuSetMusicCallback(Ref* pSender);
    void shoponButtonClicked(Ref* sender);
    void uponButtonClicked(Ref* sender);
    CREATE_FUNC(PlayingScene); //�Զ�����һ��ʵ��

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    static void randCard();
};

#endif
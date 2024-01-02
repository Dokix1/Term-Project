#ifndef __PLAYING_SCENE_H__
#define __PLAYING_SCENE_H__

#include "cocos2d.h"
#include"../Hero/hero.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

/* PlayingScene类 */
class PlayingScene : public Scene {
public:
    static Scene* createScene(); //创建一个Scene
    virtual bool init(); //初始化场景内容
    cocos2d::ProgressTimer* timer; // 进度条对象
    cocos2d::Sprite* progressBackground; // 进度条底部背景
    cocos2d::Label* timeLabel; // 剩余时间标签
    float totalTime; // 总时间
    float currentTime; // 当前时间
    bool isDragging;//鼠标是否正在拖动
    cocos2d::Vec2 initialPosition;//初始位置
    // 更新进度条和时间标签
    void updateProgressBar(float dt);
    void menuCloseCallback(Ref* pSender);
    void menuSetMusicCallback(Ref* pSender);
    void shoponButtonClicked(Ref* sender);
    void uponButtonClicked(Ref* sender);
    void onMouseDown(EventMouse* event);
    void onMouseMove(EventMouse* event);
    void onMouseUp(EventMouse* event);
    CREATE_FUNC(PlayingScene); //自动创建一个实例

    void moveSpriteTo(Vec2 destination);
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    bool onTouchBeganLITTLE(Touch* touch, Event* event);
    static void randCard();

    //在这里我们定义一个精灵指针。
    CCSprite* m_pSprite;

};

#endif
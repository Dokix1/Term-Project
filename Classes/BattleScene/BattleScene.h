#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../PlayingScene/PlayingScene.h"
USING_NS_CC;

/* BattleScene类 */
class BattleScene : public Scene {
public:
    static Scene* createScene(); //创建一个Scene
    virtual bool init(); //初始化场景内容
    void menuSetMusicCallback(Ref* pSender);
    void releaseScene();
    void Battle();
    CREATE_FUNC(BattleScene); //自动创建一个实例
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void moveSpriteTo(Vec2 destination);
    bool onTouchBeganLITTLE(Touch* touch, Event* event);
    EventListenerMouse* touchlistener; // 鼠标事件监听器
    //在这里我们定义一个精灵指针
    CCSprite* m_pSprite;
    CCSprite* m_penemy;
    CCSprite* m_pblood;
    CCSprite* m_pbloodback;
    CCSprite* m_penemyblood;
    CCSprite* m_penemybloodback;
    int littlehero_max_blood = 100;
    int littlehero_current_blood = 100;
    int enemyhero_max_blood = 100;
    int enemyhero_current_blood = 100;
};

#endif
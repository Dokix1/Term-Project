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
};

#endif
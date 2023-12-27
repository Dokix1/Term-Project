#ifndef __STARTGAME_SCENE_H__
#define __STARTGAME_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

/* StartGameScene类 */
class StartGameScene : public Scene {
public:
    static Scene* createScene(); //创建一个Scene
    virtual bool init(); //初始化场景内容
    void menuCloseCallback(Ref* pSender);
    void menuCreateRoomCallback(Ref* pSender);
    void menuSetMusicCallback(Ref* pSender);
    CREATE_FUNC(StartGameScene); //自动创建一个实例
};

#endif
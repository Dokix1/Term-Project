#ifndef __CREATEROOM_SCENE_H__
#define __CREATEROOM_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

/* CreateRoomScene类 */
class CreateRoomScene : public Scene {
public:
    static Scene* createScene(); //创建一个Scene
    virtual bool init(); //初始化场景内容
    void menuCloseCallback(Ref* pSender);
    void menuStartGameCallback(Ref* pSender);
    void menuSetMusicCallback(Ref* pSender);
    CREATE_FUNC(CreateRoomScene); //自动创建一个实例
};

#endif
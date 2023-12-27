#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

/* LoadingScene类 */
class LoadingScene : public Scene {
public:
    static Scene* createScene(); //创建一个Scene
    virtual bool init(); //初始化场景内容
    CREATE_FUNC(LoadingScene); //自动创建一个实例
};

#endif
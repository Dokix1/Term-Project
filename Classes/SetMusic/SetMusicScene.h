#ifndef __SETMUSIC_SCENE_H__
#define __SETMUSIC_SCENE_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
USING_NS_CC;

/* SetMusicScene类 */
class SetMusicScene : public Scene {
private:
    Sprite* switchMusic;  //播放开始/暂停
    bool isMusicPlaying;  //音乐播放状态
    Label* volumeLabel;   //滑动条百分比

public:
    static Scene* createScene(); //创建一个Scene
    virtual bool init(); //初始化场景内容
    void menuCloseCallback(Ref* pSender);
    bool onSwitchMusicTouched(Touch* touch, Event* event);
    void volumeSliderCallback(Ref* sender, ui::Slider::EventType eventType);
    CREATE_FUNC(SetMusicScene); //自动创建一个实例
};

#endif
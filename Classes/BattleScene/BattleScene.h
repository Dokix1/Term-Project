#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../PlayingScene/PlayingScene.h"
USING_NS_CC;

/* BattleScene�� */
class BattleScene : public Scene {
public:
    static Scene* createScene(); //����һ��Scene
    virtual bool init(); //��ʼ����������
    void menuSetMusicCallback(Ref* pSender);
    void releaseScene();
    void Battle();
    CREATE_FUNC(BattleScene); //�Զ�����һ��ʵ��
};

#endif
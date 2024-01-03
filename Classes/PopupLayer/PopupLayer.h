#ifndef __POPUP_LAYER_H__
#define __POPUP_LAYER_H__

#include "cocos2d.h"
USING_NS_CC;

/* PopupLayer类 */
/* 创建弹窗层，显示卡牌和刷新按钮，并处理触摸事件 */
class PopupLayer : public LayerColor {
public:
    CREATE_FUNC(PopupLayer); //实例化PopupLayer

    void Cardsinit(); //初始化卡牌
 

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event); //处理触摸事件

    bool init(); //初始化弹窗层 

    void reonButtonClicked(Ref* sender);

    void hide(); //隐藏弹窗层
};

#endif
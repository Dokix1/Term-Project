#pragma once
/*
*攻击特效类
*根据不同的英雄id绑定不同的攻击特效
*/
#ifndef  _ATTACK_EFFECT_H_
#define  _ATTACK_EFFECT_H_

#include "cocos2d.h"
USING_NS_CC;

//攻击特效类
class AttackFunc :public Sprite
{
public:
	static AttackFunc* create(int id,bool is_far, const Vec2& attackerPosition, const Vec2& targetPosition);//根据id创建攻击特效 后面两个变量为攻击者和被攻击者的位置
	void init(int id, bool is_far, const Vec2& attackerPosition, const Vec2& targetPosition);//攻击特效图片的初始化

    // 添加一个方法，用于启动攻击特效的移动动作
    void playAttackAnimation(const Vec2& attackerPosition, const Vec2& targetPosition);

private:
    // 存储攻击特效的目标位置
    Vec2 targetPosition;
};

#endif
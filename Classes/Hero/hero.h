#ifndef  _HERO_H_
#define  _HERO_H_

#include "cocos2d.h"
#include <cstdlib> 
#include <ctime> 
USING_NS_CC;

/* 英雄精灵类 */
class Hero :public Sprite {
protected:
    /* 英雄属性 */
    int attack; //攻击力（每次攻击造成的伤害）
    int defect; //防御力（英雄抵御伤害的能力）  

    int AP;       //法强（英雄的法术攻击能力）  
    int APdefect; //法抗（对法术伤害的抵抗能力）      

    int HP;     //血量（生命值）
    int FullHP; //满血量（最大生命值）

    int MP;     //蓝量（当前的法力值）      
    int FullMP; //满蓝量（最大法力值）    

    int cost; //费用（英雄购买所需金币数量）

    int attackDistance; //攻击范围

    bool survival; //是否存活

    bool red; //是否是敌方阵营

    double move_speed; //移动速度
    int attack_speed;  //攻击速度

    int level; //星级（一星/两星/三星）

public:  
    /* 构造函数 */
    Hero(bool red, int level) : red(red), level(level) {
        survival = true;
        move_speed = 1;
        attack_speed = 1;
    }

    /* 获取攻击力 */ 
    int getAttack() {
        return attack;
    }

    /* 获取防御力 */
    int getDefect() {
        return defect;
    }

    /* 获取法强 */
    int getAP() {
        return AP;
    }

    /* 获取法抗 */
    int getAPdefect() {
        return APdefect;
    }

    /* 获取血量 */
    int getHP() {
        return HP;
    }

    /* 获取满血量 */
    int getFullHP() {
        return FullHP;
    }

    /* 获取蓝量 */
    int getMP() {
        return MP;
    }

    /* 获取满蓝量 */
    int getFullMP() {
        return FullMP;
    }

    /* 获取费用 */
    int getCost() {
        return cost;
    }

    /* 获取攻击范围 */
    int getAttackDistance() {
        return attackDistance;
    }

    /* 获取是否存活 */
    bool isSurvival() {
        return survival;
    }

    /* 获取是否是敌方阵营 */
    bool isRed() {
        return red;
    }

    /* 获取移动速度 */
    double getMoveSpeed() {
        return move_speed;
    }

    /* 获取攻击速度 */
    int getAttackSpeed() {
        return attack_speed;
    }

    /* 获取星级 */
    int getLevel() {
        return level;
    }

    virtual void attackEnemy() {

    }
    //攻击逻辑
    virtual void useSkill() {

    }
    //技能逻辑

    cocos2d::Sprite* blood;
};

/* 物理坦克-0 */
class HeroPhysicalTank : public Hero {
public:
    /* 构造函数 */
    HeroPhysicalTank(bool red, int level) : Hero(red, level) {
        attack = 30;
        defect = 200;
        AP = 0;
        APdefect = 100;
        FullHP = HP = 500;
        MP = 20;
        FullMP = 50;
        cost = 1;
        attackDistance = 1;

        if (level == 2) { //二星英雄

        }
        else if (level == 3) { //三星英雄

        }
    }

    void attackEnemy() override {
        //攻击逻辑
    }

    void useSkill() override {
        //技能逻辑
    }
};

/* 魔法坦克-1 */
class HeroMagicTank : public Hero {
public:
    /* 构造函数 */
    HeroMagicTank(bool red, int level) : Hero(red, level) {
        attack = 20;
        defect = 100;
        AP = 40;
        APdefect = 200;
        FullHP = HP = 400;
        MP = 30;
        FullMP = 40;
        cost = 2;
        attackDistance = 1;

        if (level == 2) { //二星英雄

        }
        else if (level == 3) { //三星英雄

        }
    }

    void attackEnemy() override {
        //攻击逻辑
    }

    void useSkill() override {
        //技能逻辑
    }
};

/* 物理战士-2 */
class HeroPhysicalWarrior : public Hero {
public:
    /* 构造函数 */
    HeroPhysicalWarrior(bool red, int level) : Hero(red, level) {
        attack = 70;
        defect = 100;
        AP = 30;
        APdefect = 100;
        FullHP = HP = 350;
        MP = 0;
        FullMP = 30;
        cost = 3;
        attackDistance = 1;

        if (level == 2) { //二星英雄

        }
        else if (level == 3) { //三星英雄

        }
    }

    void attackEnemy() override {
        //攻击逻辑
    }

    void useSkill() override {
        //技能逻辑
    }
};

/* 魔法战士-3 */
class HeroMagicalWarrior : public Hero {
public:
    /* 构造函数 */
    HeroMagicalWarrior(bool red, int level) : Hero(red, level) {
        attack = 30;
        defect = 100;
        AP = 60;
        APdefect = 100;
        FullHP = HP = 350;
        MP = 10;
        FullMP = 50;
        cost = 1;
        attackDistance = 1;

        if (level == 2) { //二星英雄

        }
        else if (level == 3) { //三星英雄

        }
    }

    void attackEnemy() override {
        //攻击逻辑
    }

    void useSkill() override {
        //技能逻辑
    }
};

/* 射手-4 */
class HeroMarksman : public Hero {
public:
    /* 构造函数 */
    HeroMarksman(bool red, int level) : Hero(red, level) {
        attack = 80;
        defect = 50;
        AP = 30;
        APdefect = 50;
        FullHP = HP = 250;
        MP = 20;
        FullMP = 50;
        cost = 2;
        attackDistance = 3;

        if (level == 2) { //二星英雄

        }
        else if (level == 3) { //三星英雄

        }
    }

    void attackEnemy() override {
        //攻击逻辑
    }

    void useSkill() override {
        //技能逻辑
    }
};

/* 法师-5 */
class HeroMage : public Hero {
public:
    /* 构造函数 */
    HeroMage(bool red, int level) : Hero(red, level) {
        attack = 0;
        defect = 50;
        AP = 100;
        APdefect = 50;
        FullHP = HP = 250;
        MP = 30;
        FullMP = 60;
        cost = 3;
        attackDistance = 3;

        if (level == 2) { //二星英雄

        }
        else if (level == 3) { //三星英雄

        }
    }

    void attackEnemy() override {
        //攻击逻辑
    }

    void useSkill() override {
        //技能逻辑
    }
};

#endif
#pragma once

/*
*此类定义了英雄具有的各种属性
*具体包含的内容见下方注释
*/
#ifndef _HERO_H_
#define _HERO_H_

#include<vector>
#include<string>
#include<cocos2d.h>

USING_NS_CC;

class Hero
{
public:
	double attack;//攻击力
	double defect;//防御力     
	double ap_def;//法抗       
	double ap;//法强           
	double HP;//血量
	double FullHP;//满血量    
	double MP;//蓝       
	double FullMP;//满蓝量     

	int cost;//费用

	int attack_distance;//攻击范围(?)
	bool survival;//是否存活

	bool red;//是否是敌方的
	
	const double move_speed=1;  //移动速度(固定？)
	const int attack_speed=1;   //攻击速度(固定？)

	int level;//星级（一星两星三星）

};

#endif


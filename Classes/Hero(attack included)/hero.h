#pragma once

/*
*���ඨ����Ӣ�۾��еĸ�������
*������������ݼ��·�ע��
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
	double attack;//������
	double defect;//������     
	double ap_def;//����       
	double ap;//��ǿ           
	double HP;//Ѫ��
	double FullHP;//��Ѫ��    
	double MP;//��       
	double FullMP;//������     

	int cost;//����

	int attack_distance;//������Χ(?)
	bool survival;//�Ƿ���

	bool red;//�Ƿ��ǵз���
	
	const double move_speed=1;  //�ƶ��ٶ�(�̶���)
	const int attack_speed=1;   //�����ٶ�(�̶���)

	int level;//�Ǽ���һ���������ǣ�

};

#endif


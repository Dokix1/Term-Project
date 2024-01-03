#ifndef __AI_H__
#define __AI_H__

#include "cocos2d.h"
#include "../Hero/hero.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

/* PlayingScene¿‡ */
class AI {
public:
	int money = 0;
	int population = 0;
	int AIlevel = 3;

	void AICardsinit();
	void AIlevelup();
	void AIStartUp(int number);
	void AIbettle();
};

#endif
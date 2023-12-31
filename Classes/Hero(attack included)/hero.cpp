#include"HeroSprite.h"
#include<string>
#include<vector>


//以下依次为物理坦克0、魔法坦克1、物理战士2、魔法战士3、射手4、法师5
//按照代码的意思应该是id模6代表不同英雄


HeroSprite::HeroSprite(int id) {
	init(id);
}

//读取初始生命值
int HeroSprite::get_hero_hp(int id)
{
	std::vector<int>hero_hp = { 500,400,350,350,250,250 };
	return hero_hp[id % 6];
}

//读取初始物攻数据                  
int HeroSprite::get_hero_atk(int id)
{
	std::vector<int>hero_atk = { 30,20,70,30,80,0 };
	return hero_atk[id % 6];
}

//读取初始防御力                  
int HeroSprite::get_hero_def(int id)
{
	std::vector<int>hero_def = { 200,100,100,100,50,50 };
	return hero_def[id % 6];
}

//读取初始法强                 
int HeroSprite::get_hero_ap(int id)
{
	std::vector<int>hero_ap = { 0,40,30,60,30,100 };
	return hero_ap[id % 6];
}

//读取初始法抗                  
int HeroSprite::get_hero_ap_def(int id)
{
	std::vector<int>hero_ap_def = { 100,200,100,100,50,50 };
	return hero_ap_def[id % 6];
}

//读取初始蓝                  
int HeroSprite::get_hero_mp(int id)
{
	std::vector<int>hero_mp = { 20,30,0,10,20,30 };
	return hero_mp[id % 6];
}


//读取最大蓝量
int HeroSprite::get_hero_FullMP(int id)
{
	std::vector<int>hero_attack_distance = {50,40,30,50,50,60 };
	return hero_attack_distance[id % 6];
}

//读取费用
int HeroSprite::get_hero_cost(int id)
{
	std::vector<int>hero_attack_distance = { 1,2,3,1,2,3};
	return hero_attack_distance[id % 6];
}


//读取攻击距离
int HeroSprite::get_hero_attack_distance(int id)
{
	std::vector<int>hero_attack_distance = { 1,1,1,1,20,20 };
	return hero_attack_distance[id % 6];
}



//下面没做修改
HeroSprite* HeroSprite::create(int id)
{
	HeroSprite* heroSprite = new HeroSprite(id);
	//为了在这里不区分红蓝
	if (id > 5)
	{
		id -= 6;
	}
	heroSprite->init(id);
	heroSprite->autorelease();
	return heroSprite;
}

bool HeroSprite::init(int id)
{
	//英雄图片？
	std::vector<std::string> hero_image =
	{
		"hero0.png",
		"hero1.png",
		"hero2.png",
		"Hero/yongen.png",
		"Hero/haoyun.png",
		"Hero/salefenni.png"
	};

	//添加英雄
	Sprite::initWithFile(hero_image[id]);
	return true;
}

int HeroSprite::myrand()
{
	srand((unsigned)time(NULL));
	int ai_id = rand() % 6;

	if (ai_id == 0 || ai_id == 2)//由于攻击距离短，ai会有bug
	{
		++ai_id;
	}

	return ai_id;
}

//攻击函数
void HeroSprite::attack(HeroSprite* target) {
	// 获取攻击和防御值
	double attackValue = hero.attack;
	double defenseValue = target->hero.defect;

	// 计算实际伤害
	double damage = attackValue - defenseValue;

	// 如果伤害为正值，说明攻击有效
	if (damage > 0) {
		// 扣除目标英雄的血量
		target->hero.HP -= damage;

		// 更新血条显示
		updateBloodBar();

		// 判断目标英雄是否死亡
		if (target->hero.HP <= 0) {
			target->hero.HP = 0;
			target->hero.survival = false;
			// 这里可以添加死亡相关的逻辑，比如播放死亡动画等
		}
	}
}

void HeroSprite::updateBloodBar() {
	// 根据当前血量和满血量比例更新血条
	float percentage = static_cast<float>(hero.HP) / static_cast<float>(hero.FullHP) * 100.0f;
	blood->setScaleX(percentage / 100.0f);
}



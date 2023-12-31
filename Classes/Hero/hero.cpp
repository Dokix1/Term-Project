#include"HeroSprite.h"
#include<string>
#include<vector>


//��������Ϊ����̹��0��ħ��̹��1������սʿ2��ħ��սʿ3������4����ʦ5
//���մ������˼Ӧ����idģ6����ͬӢ��



//��ȡ��ʼ����ֵ
int HeroSprite::get_hero_hp(int id)
{
	std::vector<int>hero_hp = { 500,400,350,350,250,250 };
	return hero_hp[id % 6];
}

//��ȡ��ʼ�﹥����                  
int HeroSprite::get_hero_atk(int id)
{
	std::vector<int>hero_atk = { 30,20,70,30,80,0 };
	return hero_atk[id % 6];
}

//��ȡ��ʼ������                  
int HeroSprite::get_hero_def(int id)
{
	std::vector<int>hero_def = { 200,100,100,100,50,50 };
	return hero_def[id % 6];
}

//��ȡ��ʼ��ǿ                 
int HeroSprite::get_hero_ap(int id)
{
	std::vector<int>hero_ap = { 0,40,30,60,30,100 };
	return hero_ap[id % 6];
}

//��ȡ��ʼ����                  
int HeroSprite::get_hero_ap_def(int id)
{
	std::vector<int>hero_ap_def = { 100,200,100,100,50,50 };
	return hero_ap_def[id % 6];
}

//��ȡ��ʼ��                  
int HeroSprite::get_hero_mp(int id)
{
	std::vector<int>hero_mp = { 20,30,0,10,20,30 };
	return hero_mp[id % 6];
}


//��ȡ�������
int HeroSprite::get_hero_FullMP(int id)
{
	std::vector<int>hero_attack_distance = {50,40,30,50,50,60 };
	return hero_attack_distance[id % 6];
}

//��ȡ����
int HeroSprite::get_hero_cost(int id)
{
	std::vector<int>hero_attack_distance = { 1,2,3,1,2,3};
	return hero_attack_distance[id % 6];
}


//��ȡ��������
int HeroSprite::get_hero_attack_distance(int id)
{
	std::vector<int>hero_attack_distance = { 1,1,1,1,20,20 };
	return hero_attack_distance[id % 6];
}



//����û���޸�
HeroSprite* HeroSprite::create(int id)
{
	HeroSprite* heroSprite = new HeroSprite(id);
	//Ϊ�������ﲻ���ֺ���
	if (id > 2)
	{
		id -= 3;
	}
	heroSprite->init(id);
	heroSprite->autorelease();
	return heroSprite;
}

bool HeroSprite::init(int id)
{
	//Ӣ��ͼƬ��
	std::vector<std::string> hero_image =
	{
		"characters/daji/daji_down.png",
		"characters/yase/yase_down.png",
		"Hero/yasuo.png",
		"Hero/yongen.png",
		"Hero/haoyun.png",
		"Hero/salefenni.png"
	};

	//���Ӣ��
	Sprite::initWithFile(hero_image[id]);
	return true;
}

int HeroSprite::myrand()
{
	srand((unsigned)time(NULL));
	int ai_id = rand() % 3 + 3;

	if (ai_id == 4)//��ɪ���ڹ�������̣�ai����bug
	{
		++ai_id;
	}

	return ai_id;
}




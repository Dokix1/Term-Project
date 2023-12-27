#include"AttackFunc.h"
#include<string>
#include<vector>
#define ATTACK 6//第六张图片为攻击时的光点 使用define定义 可修改!!!!!!
AttackFunc* AttackFunc::create(int id,bool is_far, const Vec2& attackerPosition, const Vec2& targetPosition)//创建攻击特效
{
	if (is_far == 1)//移动光点特效
	{
		AttackFunc* effect = new AttackFunc();
		effect->init(ATTACK, is_far, attackerPosition, targetPosition);// 远程英雄的移动光点特效
		effect->autorelease();//将对象加入到自动释放池中，避免手动管理内存
	}
	AttackFunc* effect = new AttackFunc();
	effect->init(id, 0, attackerPosition, targetPosition);// 物理坦克0、魔法坦克1、物理战士2、魔法战士3、射手4、法师5的攻击特效(英雄身上发光等)
	//音效
	effect->autorelease();//将对象加入到自动释放池中，避免手动管理内存
	return effect;
}
//后面两个变量为攻击者和被攻击者的位置
void AttackFunc::init(int id,bool is_far, const Vec2& attackerPosition, const Vec2& targetPosition)//攻击特效图片的初始化 is_far==1代表是远程 需要加入动态攻击特效
{
	//以下依次为物理坦克0、魔法坦克1、物理战士2、魔法战士3、射手4、法师5的攻击特效图片
	const std::vector<std::string> effects =
	{
		"0.png",
		"1.png",
		"2.png",
		"3.png",
		"4.png",
		"5.png",
		"attack.png"//光点图片
	};
	if (is_far == 1)
	{
		//创建移动的光点"attack.png"
		Sprite::initWithFile(effects[id]);
		playAttackAnimation(attackerPosition, targetPosition);
	}
	else
		Sprite::initWithFile(effects[id]);

	
}

void AttackFunc::playAttackAnimation(const Vec2& attackerPosition, const Vec2& targetPosition)
{
	// 计算攻击特效应该移动的距离
	Vec2 moveDistance = targetPosition - attackerPosition;

	// 计算攻击特效移动的时间，可以根据实际情况调整移动速度
	float moveDuration = 1.0f;
	
	// 创建一个移动动作
	MoveBy* moveAction = MoveBy::create(moveDuration, moveDistance);

	// 添加一个回调函数，在移动完成后自动从父节点移除
	CallFunc* removeAction = CallFunc::create([this]() {
		this->removeFromParentAndCleanup(true);
		});

	// 创建一个序列动作，包括移动和移除两个动作
	Sequence* sequence = Sequence::create(moveAction, removeAction, nullptr);

	// 运行动作
	this->runAction(sequence);
}

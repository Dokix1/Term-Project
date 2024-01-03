#include "SimpleAudioEngine.h"
#include "PlayingScene.h"
#include "BattleScene.h"
#include "SetMusicScene.h"
#include <vector>
#include <time.h>
#include <string>
#include <queue>
#include <utility>
USING_NS_CC;
using namespace CocosDenshion;
using namespace std;
using namespace ui;

const int numRows = 6; //行
const int numCols = 6; //列
const float tileSize = 90.0F; //格子大小

const int boardX_min = 261;
const int boardX_max = 974;
const int boardY_min = 294;
const int boardY_max = 722;
const int sizeX = 118.4;
const int sizeY = 71.2;

extern vector<vector<pair<int, Hero*>>> chessboard; //棋盘数组
vector<vector<pair<int, Hero*>>> chessboardBattle(numRows, vector<pair<int, Hero*>>(numCols, make_pair(-1, nullptr))); //棋盘数组

void put(Hero* h, int col, int row);
void findNearestHero(int i, int j, bool opponent, int& x, int& y);
pair<int, int> isWithinAttackRange(int x, int y, bool opponent);
bool Battle();

/* 创建一个Scene对象 */
Scene* BattleScene::createScene() {
    return BattleScene::create();
}

/* 返回备战界面 */
void BattleScene::menuCloseCallback(Ref* pSender) {
    Director::getInstance()->popScene();
}

/* 点击后调节音效 */
void BattleScene::menuSetMusicCallback(Ref* pSender) {
    auto newScene = SetMusicScene::create();
    Director::getInstance()->pushScene(newScene); //切换到调节音效场景 当前场景放入场景栈中
}

void BattleScene::releaseScene() {
    //释放场景中的资源和对象

    //释放棋盘数组中的英雄对象
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            if (chessboardBattle[i][j].second != nullptr) {
                delete chessboardBattle[i][j].second;
                chessboardBattle[i][j].second = nullptr;
            }
        }
    }
}

/* 初始化BattleScene场景内容 */
bool BattleScene::init() {
    if (!Scene::init()) //初始化
        return false; //初始化失败
    
    for (int i = 0; i < 6; i++) 
        for (int j = 0; j < 6; j++) {
            if (chessboard[i][j].first != -1) {
                chessboardBattle[i][j].first = chessboard[i][j].first;
                switch (chessboard[i][j].first) {
                    
                    case 0:
                        chessboardBattle[i][j].second = new HeroPhysicalTank(chessboard[i][j].second->isRed(), chessboard[i][j].second->getLevel());
                        chessboardBattle[i][j].second->setTexture("Hero/hero01.png");
                        break;
                    case 1:
                        chessboardBattle[i][j].second = new HeroMagicTank(chessboard[i][j].second->isRed(), chessboard[i][j].second->getLevel());
                        chessboardBattle[i][j].second->setTexture("Hero/hero11.png");
                        break;
                    case 2:
                        chessboardBattle[i][j].second = new HeroPhysicalWarrior(chessboard[i][j].second->isRed(), chessboard[i][j].second->getLevel());
                        chessboardBattle[i][j].second->setTexture("Hero/hero21.png");
                        break;
                    case 3:
                        chessboardBattle[i][j].second = new HeroMagicalWarrior(chessboard[i][j].second->isRed(), chessboard[i][j].second->getLevel());
                        chessboardBattle[i][j].second->setTexture("Hero/hero31.png");
                        break;
                    case 4:
                        chessboardBattle[i][j].second = new HeroMarksman(chessboard[i][j].second->isRed(), chessboard[i][j].second->getLevel());
                        chessboardBattle[i][j].second->setTexture("Hero/hero41.png");
                        break;
                    case 5:
                        chessboardBattle[i][j].second = new HeroMage(chessboard[i][j].second->isRed(), chessboard[i][j].second->getLevel());
                        chessboardBattle[i][j].second->setTexture("Hero/hero51.png");
                        break;
                }
                
                chessboardBattle[i][j].second->setScale(1.5);
                chessboardBattle[i][j].second->setFlippedX(false);
                chessboardBattle[i][j].second->setFlippedY(false);
                put(chessboardBattle[i][j].second, j, i);
                this->addChild(chessboardBattle[i][j].second, 1);
            }
        } 

    auto visibleSize = Director::getInstance()->getVisibleSize(); //屏幕可见区域的大小
    Vec2 origin = Director::getInstance()->getVisibleOrigin(); //原点坐标   

    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/playingBGM.mp3", true); //播放背景音乐
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.6F); //设置背景音乐的音量

    /* 背景精灵 */
    auto background = Sprite::create("ChessBoard/playing.png");
    background->setContentSize(Size(visibleSize.width, visibleSize.height));
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(background, 0);

    /* 设置音效菜单项 */
    auto setMusic = MenuItemImage::create("Music/setting.png", "Music/setting.png",
        CC_CALLBACK_1(BattleScene::menuSetMusicCallback, this));
    setMusic->setScale(0.5);
    float x = origin.x + 11 * visibleSize.width / 12;
    float y = origin.y + 8 * visibleSize.height / 9;
    setMusic->setPosition(Vec2(x, y));

    /* 创建菜单 */
    auto menu = Menu::create(setMusic, nullptr); //添加菜单项
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    if (Battle()) { //胜利        
        auto label = Label::createWithTTF("Win!", "fonts/Marker Felt.ttf", 75); //创建标签
        label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 2 * visibleSize.height / 3)); //标签位置
        label->setTextColor(Color4B::BLACK); //设置颜色
        this->addChild(label, 1); //添加到场景中
    }
    else { //失败
        auto label = Label::createWithTTF("Lose...", "fonts/Marker Felt.ttf", 75); //创建标签
        label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 2 * visibleSize.height / 3)); //标签位置
        label->setTextColor(Color4B::BLACK); //设置颜色
        this->addChild(label, 1); //添加到场景中
    }
   
   

    Sleep(1000);
    Director::getInstance()->popScene();

    return true;
}

/* 英雄移动+对战 */
bool Battle() {
    vector<pair<int, Hero*>> heroVector; //存储英雄
    int myHero = 0; //我方英雄数
    int opHero = 0; //对方英雄数

    for (int i = 0; i < numRows; i++)
        for (int j = 0; j < numCols; j++) 
            if (chessboardBattle[i][j].second != nullptr) {
                put(chessboardBattle[i][j].second, i, j);
                heroVector.push_back(chessboardBattle[i][j]);

                if (chessboardBattle[i][j].second->isRed()) 
                    opHero++;
                else 
                    myHero++;
            }        
        
    while (opHero && myHero) 
        for (pair<int, Hero*> hero : heroVector)
            if (hero.second->isSurvival()) {
                bool battle = 0;
                int heroX = (hero.second->getPosition().x - 290) / 118.4;
                int heroY = (hero.second->getPosition().y - 295) / 71.2;

                //找到最近的攻击目标
                int targetX = -1;
                int targetY = -1;
                int minDistance = numRows * numRows + numCols * numCols;

                for (int i = 0; i < numRows; i++)
                    for (int j = 0; j < numCols; j++)
                        if (chessboardBattle[i][j].second != nullptr && chessboardBattle[i][j].second->isRed() != hero.second->isRed() && chessboardBattle[i][j].second->isSurvival()) {
                            int distance = (i - heroX) * (i - heroX) + (j - heroY) * (j - heroY);
                            if (distance < minDistance) {
                                minDistance = distance;
                                targetX = i;
                                targetY = j;
                            }
                            //对战
                            if (minDistance <= hero.second->getAttackDistance() * hero.second->getAttackDistance()) {
                                battle = 1;
                                Hero* hero1 = hero.second;
                                Hero* hero2 = chessboardBattle[targetX][targetY].second;

                                while (hero1->isSurvival() && hero2->isSurvival()) {
                                    //Sleep(2);
                                    int hurt1 = hero2->getAttack() - hero1->getDefect() / 10 + hero2->getAP() - hero1->getAPdefect() / 10;
                                    int hurt2 = hero1->getAttack() - hero2->getDefect() / 10 + hero1->getAP() - hero2->getAPdefect() / 10;

                                    hero1->changeHP(hurt1);
                                    hero2->changeHP(hurt2);

                                    if (hero1->getHP() <= 0) {
                                        hero1->dead();
                                        hero1->removeFromParent();
                                        if (hero1->isRed())
                                            opHero--;
                                        else
                                            myHero--;
                                    }
                                        
                                    if (hero2->getHP() <= 0) {
                                        hero2->dead();
                                        hero2->removeFromParent();
                                        if (hero1->isRed())
                                            opHero--;
                                        else
                                            myHero--;
                                    } 
                                }
                            }
                        }

                //移动英雄一格
                if (!battle && targetX != -1 && targetY != -1) {
                    chessboardBattle[heroX][heroY].second = nullptr;
                   
                    put(hero.second, targetX, targetY);
                }
            }
    
    return myHero;
}
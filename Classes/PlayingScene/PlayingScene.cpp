#include "SimpleAudioEngine.h"
#include "PlayingScene.h"
#include "../StartGame/StartGame.h"
#include "../SetMusic/SetMusicScene.h"
#include <vector>
#include <time.h>
#include <string>
#include <utility>
#include "../PopupLayer/PopupLayer.h"
USING_NS_CC;
using namespace CocosDenshion;
using namespace std;
using namespace ui;

Button* shopbutton;
Button* upbutton;
Button* rebutton;

Sprite* chooseground;

const int numRows = 6; //行
const int numCols = 6; //列
const float tileSize = 90.0F; //格子大小
int selectedHeroIndex = -1;

pair<int, Hero*> my_hero=make_pair(-1,nullptr);
pair<int, Hero*> heroCard[5] = { {-1, nullptr},{-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr} }; //卡牌
pair<int, Hero*> prepare[9] = { {-1, nullptr},{-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr} }; //备战

vector<vector<pair<int,Hero*>>> chessboard(numRows, vector<pair<int,Hero*>>(numCols,make_pair(-1, nullptr))); //棋盘数组

void findNearestHero(int i, int j, bool opponent, int& x, int& y);
pair<int, int> isWithinAttackRange(int x, int y, bool opponent);
void moveHero();

/* 随机生成牌库 初始化卡牌 */
void PlayingScene::randCard() {
    srand(time(0));

    for (int i = 0; i < 5; i++) 
        heroCard[i].first = rand() % 6; 
}

/* 创建一个Scene对象 */
Scene* PlayingScene::createScene() {
    return PlayingScene::create();
}

/* 点击后返回主界面 */
void PlayingScene::menuCloseCallback(Ref* pSender) {
    auto newScene = StartGameScene::create(); //主界面
    Director::getInstance()->replaceScene(newScene); //切换到主界面
}


//小小英雄移动
void PlayingScene::moveSpriteTo(Vec2 destination) {
    // 限制目标位置在边界内
    if ((destination.x > 200 && destination.x < 1050) && (destination.y > 250 && destination.y < 800))
    {
        // 计算精灵当前位置和目标位置的距离
        float distance = m_pSprite->getPosition().getDistance(destination);

        // 计算移动的时间，假设每秒移动300个像素
        float duration = distance / 300.0f;

        // 创建一个移动动作
        auto moveToAction = MoveTo::create(duration, destination);

        // 运行移动动作
        m_pSprite->runAction(moveToAction);
    }
}



//小小英雄读入鼠标
bool PlayingScene::onTouchBeganLITTLE(Touch* touch, Event* event)
{


    // 停止当前正在进行的移动动作
    m_pSprite->stopAllActions();

    // 获取鼠标点击的位置
    Vec2 touchLocation = touch->getLocation();

    // 调用移动函数，将精灵移动到鼠标点击的位置
    moveSpriteTo(touchLocation);

    return true;  // 返回true表示消耗了该事件
}


void PlayingScene::shoponButtonClicked(Ref* sender) {

    shopbutton->setEnabled(false);
    shopbutton->loadTextures("buttons/ShopSelected.png", "buttons/ShopSelected.png", "buttons/ShopSelected.png");
    auto popupLayer = PopupLayer::create();
    this->addChild(popupLayer);
}
void PlayingScene::onMouseDown(EventMouse* event)
{
    Vec2 mousePos = event->getLocation();
    mousePos.y = Director::getInstance()->getWinSize().height - mousePos.y;

    //备战区
    for (int i = 0; i < 9; i++) {
        if (mousePos.x >= 243.75 + 87.25 * i && mousePos.x <= 286.75 + 87.25 * i && mousePos.y >= 156 && mousePos.y <= 219&&prepare[i].first!=-1) {
            my_hero.first = prepare[i].first;
            my_hero.second = prepare[i].second;
            initialPosition = my_hero.second->getPosition();
            prepare[i].first = -1;
            prepare[i].second = nullptr;
            isDragging = true;
            chooseground->setVisible(true);
            break;
        }
    }
    //棋盘
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 6; j++) {
            if (mousePos.x >= 274 + 118.4 * j && mousePos.x <= 364 + 118.4 * j && mousePos.y >= 305+71.2*i && mousePos.y <= 352+71.2*i && chessboard[i][j].first != -1) {
                my_hero.first = chessboard[i][j].first;
                my_hero.second = chessboard[i][j].second;
                initialPosition = my_hero.second->getPosition();
                chessboard[i][j].first = -1;
                chessboard[i][j].second = nullptr;
                isDragging = true;
                chooseground->setVisible(true);
            }
        }
    }
}

void PlayingScene::onMouseMove(EventMouse* event)
{
    if (isDragging)
    {
        Vec2 mousePos = event->getLocation();
        mousePos.y = Director::getInstance()->getWinSize().height - mousePos.y;
        my_hero.second->setPosition(Vec2(mousePos.x-30, mousePos.y - 35));
    }
}

void PlayingScene::onMouseUp(EventMouse* event)
{
    if (isDragging)
    {
        isDragging = false;

        Vec2 mousePos = event->getLocation();
        mousePos.y = Director::getInstance()->getWinSize().height - mousePos.y;
        //备战区
        for (int i = 0; i < 9; i++) {
            if (mousePos.x >= 243.75 + 87.25 * i && mousePos.x <= 286.75 + 87.25 * i && mousePos.y >= 156 && mousePos.y <= 219 && prepare[i].first == -1) {
                prepare[i].first = my_hero.first;
                prepare[i].second= my_hero.second;
                prepare[i].second->setPosition(Vec2(240 + 87.25 * i, 150));
                my_hero.first = -1;
                my_hero.second = nullptr;
                chooseground->setVisible(false);
                break;
            }
        }
        if (my_hero.first == -1)
            return;
        //棋盘
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 6; j++) {
                if (mousePos.x >= 274 + 118.4 * j && mousePos.x <= 364 + 118.4 * j && mousePos.y >= 305 + 71.2 * i && mousePos.y <= 352 + 71.2 * i && chessboard[i][j].first == -1) {
                    chessboard[i][j].first = my_hero.first;
                    chessboard[i][j].second = my_hero.second;
                    chessboard[i][j].second->setPosition(Vec2(290 + 118.4 * j, 295 + 71.2 * i));
                    my_hero.first = -1;
                    my_hero.second = nullptr;
                    chooseground->setVisible(false);
                    break;
                }
            }
        }
        if (my_hero.first == -1)
            return;
        for (int i = 0; i < 9; i++) {
            if (initialPosition==Vec2(240 + 87.25 * i, 150)) {
                prepare[i].first = my_hero.first;
                prepare[i].second = my_hero.second;
                prepare[i].second->setPosition(initialPosition);
                my_hero.first = -1;
                my_hero.second = nullptr;
                chooseground->setVisible(false);
                break;
            }
        }
        if (my_hero.first == -1)
            return;
        //棋盘
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                if (initialPosition==Vec2(290+118.4*j,295+71.2*i)){
                    chessboard[i][j].first = my_hero.first;
                    chessboard[i][j].second = my_hero.second;
                    chessboard[i][j].second->setPosition(Vec2(initialPosition.x,initialPosition.y));
                    my_hero.first = -1;
                    my_hero.second = nullptr;
                    chooseground->setVisible(false);
                    break;
                }
            }
        }
    }
}
/*倒计时*/
void PlayingScene::updateProgressBar(float dt) {
    //计算剩余时间
    currentTime -= dt;
    if (currentTime < 0) {
        currentTime = 0;
    }

    //更新进度条
    float progress = currentTime / totalTime;
    timer->setPercentage(progress * 100);

    //更新时间标签
    int remainingTime = static_cast<int>(ceil(currentTime));
    timeLabel->setString(std::to_string(remainingTime));

    //检查是否时间已经用完
    if (currentTime <= 0) {
        unschedule(schedule_selector(PlayingScene::updateProgressBar));
        //moveHero(); //在这里添加时间到达的处理逻辑
    }
}

/* 点击后调节音效 */
void PlayingScene::menuSetMusicCallback(Ref* pSender) {
    auto newScene = SetMusicScene::create();
    Director::getInstance()->pushScene(newScene); //切换到调节音效场景 当前场景放入场景栈中
}

void PlayingScene::uponButtonClicked(Ref* sender) {
    // 触发英雄移动操作
    if (selectedHeroIndex != -1) {
        // 处理英雄移动逻辑

        selectedHeroIndex = -1; // 重置选中的英雄索引
    }
}

/* 初始化PlayingScene场景内容 */
bool PlayingScene::init() {
    if (!Scene::init()) //初始化
        return false; //初始化失败

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
    chooseground = Sprite::create("ChessBoard/choose.png");
    chooseground->setContentSize(Size(visibleSize.width, visibleSize.height));
    chooseground->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(chooseground, 0);
    chooseground->setVisible(false);
    randCard(); //初始化卡牌

    /* 退出本局游戏菜单项 */
    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
        CC_CALLBACK_1(PlayingScene::menuCloseCallback, this));
    float scale = 5.0;
    closeItem->setScale(4); //放大4倍
    float x = origin.x + visibleSize.width - closeItem->getContentSize().width * scale / 2;
    float y = origin.y + closeItem->getContentSize().height * scale / 2;
    closeItem->setPosition(Vec2(x, y)); //设置显示位置

    /* 设置音效菜单项 */
    auto setMusic = MenuItemImage::create("Music/setting.png", "Music/setting.png",
        CC_CALLBACK_1(PlayingScene::menuSetMusicCallback, this));
    setMusic->setScale(0.5);
    x = origin.x + 11 * visibleSize.width / 12;
    y = origin.y + 8 * visibleSize.height / 9;
    setMusic->setPosition(Vec2(x, y));

    /* 创建菜单 */
    auto menu = Menu::create(closeItem, setMusic, nullptr); //添加菜单项
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /* 创建按钮 */ 
    shopbutton = Button::create("buttons/ShopNormal.png", "buttons/ShopSelected.png");
    shopbutton->setPosition(Vec2(0.875 * visibleSize.width, 0.125 * visibleSize.height));
    shopbutton->addClickEventListener(CC_CALLBACK_1(PlayingScene::shoponButtonClicked, this));
    addChild(shopbutton);

    /* 创建按钮 */ 
    upbutton = Button::create("buttons/UpgradeNormal.png", "buttons/UpgradeSelected.png");
    upbutton->setPosition(Vec2(0.125 * visibleSize.width, 0.130 * visibleSize.height));
    upbutton->addClickEventListener(CC_CALLBACK_1(PlayingScene::uponButtonClicked, this));
    addChild(upbutton);

    /* 倒计时 */

    //进度条背景
    progressBackground = cocos2d::Sprite::create("Bar/countdownP.png");
    progressBackground->setPosition(visibleSize.width / 2, visibleSize.height / 40 * 39);
    addChild(progressBackground);

    //创建进度条
    timer = cocos2d::ProgressTimer::create(cocos2d::Sprite::create("Bar/countdown.png"));
    timer->setType(cocos2d::ProgressTimer::Type::BAR);
    timer->setMidpoint(cocos2d::Vec2(0, 0.5));
    timer->setBarChangeRate(cocos2d::Vec2(1, 0));
    timer->setPosition(visibleSize.width / 2, visibleSize.height / 40 * 39);
    addChild(timer);

    //创建时间标签
    timeLabel = cocos2d::Label::createWithTTF("", "fonts/arial.ttf", 24);
    timeLabel->setPosition(visibleSize.width * 0.325, visibleSize.height / 40 * 39);
    addChild(timeLabel);

    //初始化时间
    totalTime = 10.0f;
    currentTime = totalTime;

    //启动定时器
    schedule(schedule_selector(PlayingScene::updateProgressBar), 0.01f);

    //点击屏幕触发英雄移动
    auto touchlistener = EventListenerMouse::create();
    touchlistener->onMouseDown = CC_CALLBACK_1(PlayingScene::onMouseDown, this);
    touchlistener->onMouseMove = CC_CALLBACK_1(PlayingScene::onMouseMove, this);
    touchlistener->onMouseUp = CC_CALLBACK_1(PlayingScene::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistener, this);

    /* 创建小小英雄 */
    m_pSprite = CCSprite::create("ikun.png");

    // 放置精灵
    m_pSprite->setPosition(ccp(185, 276));

    addChild(m_pSprite,1);

    // 设置鼠标点击事件监听
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(PlayingScene::onTouchBeganLITTLE, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);



    return true;
}

/* 寻找距离最近的对方英雄 */
void findNearestHero(int i, int j, bool opponent, int& x, int& y) {
    int distance = numRows * numCols; //初始化距离为最大可能值
    x = -1;
    y = -1;

    for (int m = 0; m < numRows; m++)
        for (int n = 0; n < numCols; n++)
            if (chessboard[m][n].second->isRed() == opponent) {
                int tmp = abs((m - i) * (n - j)); //计算距离的平方
                if (distance > tmp) {
                    distance = tmp;
                    x = m;
                    y = n;
                }
            }
}

/* 判断是否在攻击范围内 */
pair<int, int> isWithinAttackRange(int x, int y, bool opponent) {
    int attackRange = chessboard[x][y].second->getAttackDistance(); //获取攻击范围

    for (int i = 0; i < numRows; i++)
        for (int j = 0; j < numCols; j++)
            if (chessboard[i][j].second->isRed() == opponent) { //该格子有对方英雄
                int distance = (x - i) + (y - j); //计算距离
                if (distance <= attackRange)
                    return { i,j }; //可以攻击，返回对方英雄坐标
            }

    return { -1, -1 }; //没有在攻击范围内的对方英雄
}

/* 英雄移动 */
void moveHero() {
    while (1) {
        bool noFight = true; //是否存在未对战的英雄的标志

        /* 遍历棋盘的每一个格子 */
        for (int i = 0; i < numRows; i++)
            for (int j = 0; j < numCols; j++) 
                if (chessboard[i][j].second!=nullptr) {
                    noFight = false; //存在未对战英雄
                    bool currentPlayer = chessboard[i][j].second->isRed(); //1对方 0己方

                    pair<int, int> pos = isWithinAttackRange(i, j, !currentPlayer);
                    if (pos.first != -1) {
                        Hero* h1 = chessboard[i][j].second;
                        Hero* h2 = chessboard[pos.first][pos.second].second;
                        //在攻击范围内，进行攻击
                        //实现攻击的逻辑，伤害计算、动画播放
                    }

                    int x, y;
                    findNearestHero(i, j, !currentPlayer, x, y); //寻找距离当前位置最近的对方英雄

                    int toX, toY; //移动后的位置
                    if (x == i)
                        toX = i;
                    else
                        toX = (x > i) ? i + 1 : i - 1;

                    if (y == j)
                        toY = j;
                    else
                        toY = (y > j) ? j + 1 : j - 1;

                    /* 移动英雄 */
                    chessboard[toX][toY].second = chessboard[i][j].second;
                    chessboard[i][j].second = nullptr;

                    pos = isWithinAttackRange(toX, toY, !currentPlayer);
                    if (pos.first != -1) {
                        Hero* h1 = chessboard[toX][toY].second;
                        Hero* h2 = chessboard[pos.first][pos.second].second;
                        //在攻击范围内，进行攻击
                        //实现攻击的逻辑，伤害计算、动画播放
                    }
                }      
            
        if (noFight)
            break;
    }
}
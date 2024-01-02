#include "SimpleAudioEngine.h"
#include "PlayingScene.h"
#include "StartGame.h"
#include "SetMusicScene.h"
#include <vector>
#include <time.h>
#include <string>
#include <utility>
#include "PopupLayer.h"
USING_NS_CC;
using namespace CocosDenshion;
using namespace std;
using namespace ui;

Button* shopbutton;
Button* upbutton;
Button* rebutton;

const int numRows = 6; //行
const int numCols = 6; //列
const float tileSize = 90.0F; //格子大小
int selectedHeroIndex = -1;

pair<int, Hero*> heroCard[5] = { {-1, nullptr},{-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr} }; //卡牌
pair<int, Hero*> prepare[9] = { {-1, nullptr},{-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr} }; //备战

vector<vector<Hero*>> chessboard(numRows, vector<Hero*>(numCols, nullptr)); //棋盘数组

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

void PlayingScene::shoponButtonClicked(Ref* sender) {

    shopbutton->setEnabled(false);
    shopbutton->loadTextures("buttons/ShopSelected.png", "buttons/ShopSelected.png", "buttons/ShopSelected.png");
    auto popupLayer = PopupLayer::create();
    this->addChild(popupLayer);
}

bool PlayingScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    Vec2 touchPos = touch->getLocation(); //获取触摸位置

    /* 实现英雄移动 */
    for (int i = 0; i < 9; i++) { //遍历备战区域
        if (selectedHeroIndex != -1) {
            /* 判断卡牌是否在棋盘范围内 */
            if (touchPos.x >= 310 && touchPos.x <= 310 + tileSize * 6 &&
                touchPos.y >= 210 && touchPos.y <= 210 + tileSize * 6) {
                //将选中的英雄移动到棋盘上的触摸位置
                //确定卡牌在棋盘上的位置
                int row = (touchPos.y - 210) / tileSize;
                int col = (touchPos.x - 310) / tileSize;
                //计算卡牌在6*6棋盘上的坐标
                float cardX = 310 + col * tileSize + tileSize / 2;
                float cardY = 210 + row * tileSize + tileSize / 2;
                prepare[selectedHeroIndex].second->setPosition(cardX, cardY);

                chessboard[row][col] = prepare[selectedHeroIndex].second; //添加到棋盘数组
                prepare[selectedHeroIndex].second->release(); //释放卡牌
            }
            else { //取消选中并恢复英雄的原始状态
                //prepare[selectedHeroIndex].second->setOutlineColor(Color4B::WHITE); //恢复轮廓颜色
                //prepare[selectedHeroIndex].second->setOutlineWidth(0); //恢复轮廓宽度
            }
            selectedHeroIndex = -1; // 重置选中的英雄索引
            return true;
        }

        if (prepare[i].first != -1 && prepare[i].second->getBoundingBox().containsPoint(touchPos)) {
            //触摸位置是否在备战区域的英雄上
            selectedHeroIndex = i; //设置选中的英雄索引
            prepare[i].second->retain();  //防止英雄被释放
            //prepare[i].second->setOutlineColor(Color4B::YELLOW); //将轮廓颜色改为黄色
            //prepare[i].second->setOutlineWidth(5); //增加轮廓宽度以提高可见性

            return true; //选中
        }
    }

    return false; //未选中
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
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(PlayingScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

/* 寻找距离最近的对方英雄 */
void findNearestHero(int i, int j, bool opponent, int& x, int& y) {
    int distance = numRows * numCols; //初始化距离为最大可能值
    x = -1;
    y = -1;

    for (int m = 0; m < numRows; m++)
        for (int n = 0; n < numCols; n++)
            if (chessboard[m][n]->isRed() == opponent) {
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
    int attackRange = chessboard[x][y]->getAttackDistance(); //获取攻击范围

    for (int i = 0; i < numRows; i++)
        for (int j = 0; j < numCols; j++)
            if (chessboard[i][j]->isRed() == opponent) { //该格子有对方英雄
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
                if (chessboard[i][j] != NULL&& chessboard[i][j]!=nullptr) {
                    noFight = false; //存在未对战英雄
                    bool currentPlayer = chessboard[i][j]->isRed(); //1对方 0己方

                    pair<int, int> pos = isWithinAttackRange(i, j, !currentPlayer);
                    if (pos.first != -1) {
                        Hero* h1 = chessboard[i][j];
                        Hero* h2 = chessboard[pos.first][pos.second];
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
                    chessboard[toX][toY] = chessboard[i][j];
                    chessboard[i][j] = nullptr;

                    pos = isWithinAttackRange(toX, toY, !currentPlayer);
                    if (pos.first != -1) {
                        Hero* h1 = chessboard[toX][toY];
                        Hero* h2 = chessboard[pos.first][pos.second];
                        //在攻击范围内，进行攻击
                        //实现攻击的逻辑，伤害计算、动画播放
                    }
                }      
            
        if (noFight)
            break;
    }
}
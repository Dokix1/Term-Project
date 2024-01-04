#include "SimpleAudioEngine.h"
#include "../PlayingScene/PlayingScene.h"
#include "../BattleScene/BattleScene.h"
#include "../SetMusic/SetMusicScene.h"
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
extern vector<vector<pair<int, Hero*>>> AIchessboard; //AI棋盘数组

extern int coinCount;

extern PlayingScene* playscene;

vector<vector<pair<int, Hero*>>> chessboardBattle(numRows, vector<pair<int, Hero*>>(numCols, make_pair(-1, nullptr))); //棋盘数组

extern void updateButtonState(Button* button);
extern void shopLabelState();

extern Button* upbutton;

void findNearestHero(int i, int j, bool opponent, int& x, int& y);
pair<int, int> isWithinAttackRange(int x, int y, bool opponent);

/* 创建一个Scene对象 */
Scene* BattleScene::createScene() {
    return BattleScene::create();
}

/* 点击后调节音效 */
void BattleScene::menuSetMusicCallback(Ref* pSender) {

    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Music/click.wav");
    auto newScene = SetMusicScene::create();
    Director::getInstance()->pushScene(newScene); //切换到调节音效场景 当前场景放入场景栈中
}

void BattleScene::releaseScene() {
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
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 6; j++) {
            if (i <= 2) {
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

                    //////////定义英雄 血条绑定
                    chessboardBattle[i][j].second->setFlippedX(false);
                    chessboardBattle[i][j].second->setFlippedY(false);

                    //己方血条
                    if (chessboardBattle[i][j].second->getLevel() == 1)
                    {
                        chessboardBattle[i][j].second->bloodbar = CCSprite::create("bloodbar/blood1.png");
                        chessboardBattle[i][j].second->bloodback = CCSprite::create("bloodbar/blood1back.png");
                    }
                    else if (chessboardBattle[i][j].second->getLevel() == 2)
                    {
                        chessboardBattle[i][j].second->bloodbar == CCSprite::create("bloodbar/blood2.png");
                        chessboardBattle[i][j].second->bloodback = CCSprite::create("bloodbar/blood1back.png");
                    }
                    else
                    {
                        chessboardBattle[i][j].second->bloodbar == CCSprite::create("bloodbar/blood3.png");
                        chessboardBattle[i][j].second->bloodback = CCSprite::create("bloodbar/blood1back.png");
                    }
                    chessboardBattle[i][j].second->bloodback->setAnchorPoint(Vec2(0.8F, 0.5F));  // 设置锚点
                    chessboardBattle[i][j].second->bloodbar->setAnchorPoint(Vec2(0.2F, 0.5F));  // 设置锚点
                    chessboardBattle[i][j].second->bloodback->setPosition(ccp(40, 70));
                    chessboardBattle[i][j].second->bloodback->setScale(0.1F);
                    chessboardBattle[i][j].second->bloodbar->setPosition(ccp(112, 107));

                    chessboardBattle[i][j].second->addChild(chessboardBattle[i][j].second->bloodback);
                    chessboardBattle[i][j].second->bloodback->addChild(chessboardBattle[i][j].second->bloodbar, 1);



                    chessboardBattle[i][j].second->setPosition(Vec2(290 + 118.4 * j, 295 + 71.2 * i));
                    chessboardBattle[i][j].second->setScale(0.9 + 0.3 * chessboardBattle[i][j].second->getLevel());
                    this->addChild(chessboardBattle[i][j].second, 0);
                    chessboardBattle[i][j].second->setOpacity(255);
                }
            }
            else {
                if (AIchessboard[i][j].first != -1) {
                    chessboardBattle[i][j].first = AIchessboard[i][j].first;
                    switch (AIchessboard[i][j].first) {
                    case 0:
                        chessboardBattle[i][j].second = new HeroPhysicalTank(AIchessboard[i][j].second->isRed(), AIchessboard[i][j].second->getLevel());
                        chessboardBattle[i][j].second->setTexture("Hero/hero01.png");
                        break;
                    case 1:
                        chessboardBattle[i][j].second = new HeroMagicTank(AIchessboard[i][j].second->isRed(), AIchessboard[i][j].second->getLevel());
                        chessboardBattle[i][j].second->setTexture("Hero/hero11.png");
                        break;
                    case 2:
                        chessboardBattle[i][j].second = new HeroPhysicalWarrior(AIchessboard[i][j].second->isRed(), AIchessboard[i][j].second->getLevel());
                        chessboardBattle[i][j].second->setTexture("Hero/hero21.png");
                        break;
                    case 3:
                        chessboardBattle[i][j].second = new HeroMagicalWarrior(AIchessboard[i][j].second->isRed(), AIchessboard[i][j].second->getLevel());
                        chessboardBattle[i][j].second->setTexture("Hero/hero31.png");
                        break;
                    case 4:
                        chessboardBattle[i][j].second = new HeroMarksman(AIchessboard[i][j].second->isRed(), AIchessboard[i][j].second->getLevel());
                        chessboardBattle[i][j].second->setTexture("Hero/hero41.png");
                        break;
                    case 5:
                        chessboardBattle[i][j].second = new HeroMage(AIchessboard[i][j].second->isRed(), AIchessboard[i][j].second->getLevel());
                        chessboardBattle[i][j].second->setTexture("Hero/hero51.png");
                        break;
                    }

                    //////////定义英雄 血条绑定
                    chessboardBattle[i][j].second->setFlippedX(false);
                    chessboardBattle[i][j].second->setFlippedY(false);

                    //敌方血条

                    if (chessboardBattle[i][j].second->getLevel() == 1)
                    {
                        chessboardBattle[i][j].second->bloodbar = CCSprite::create("bloodbar/enemyblood1.png");
                        chessboardBattle[i][j].second->bloodback = CCSprite::create("bloodbar/blood1back.png");
                    }
                    else if (chessboardBattle[i][j].second->getLevel() == 2)
                    {
                        chessboardBattle[i][j].second->bloodbar == CCSprite::create("bloodbar/enemyblood2.png");
                        chessboardBattle[i][j].second->bloodback = CCSprite::create("bloodbar/blood1back.png");
                    }
                    else
                    {
                        chessboardBattle[i][j].second->bloodbar == CCSprite::create("bloodbar/enemyblood3.png");
                        chessboardBattle[i][j].second->bloodback = CCSprite::create("bloodbar/blood1back.png");
                    }
                    chessboardBattle[i][j].second->bloodback->setAnchorPoint(Vec2(0.8F, 0.5F));  // 设置锚点
                    chessboardBattle[i][j].second->bloodbar->setAnchorPoint(Vec2(0.2F, 0.5F));  // 设置锚点
                    chessboardBattle[i][j].second->bloodback->setPosition(ccp(40, 70));
                    chessboardBattle[i][j].second->bloodback->setScale(0.1F);
                    chessboardBattle[i][j].second->bloodbar->setPosition(ccp(112, 107));

                    chessboardBattle[i][j].second->addChild(chessboardBattle[i][j].second->bloodback);
                    chessboardBattle[i][j].second->bloodback->addChild(chessboardBattle[i][j].second->bloodbar, 1);
                    
                    


                    chessboardBattle[i][j].second->setPosition(Vec2(290 + 118.4 * j, 295 + 71.2 * i));
                    chessboardBattle[i][j].second->setScale(0.9 + 0.3 * chessboardBattle[i][j].second->getLevel());
                    this->addChild(chessboardBattle[i][j].second, 0);
                    chessboardBattle[i][j].second->setOpacity(255);
                }
            }
            
         
           //Battle(); //对战

        }
    Battle(); //对战

    return true;
}

/* 英雄移动+对战 */
void BattleScene::Battle() {
    //点击屏幕触发英雄移动
    auto touchlistener = EventListenerMouse::create();


    /* 创建小小英雄+血条 */
    m_pSprite = CCSprite::create("LittleHero/ikun.png");
    m_pblood = CCSprite::create("bloodbar/littleblood.png");
    m_pbloodback = CCSprite::create("bloodbar/littlebloodback.png");

    m_penemy = CCSprite::create("LittleHero/enemy.png");
    m_penemyblood = CCSprite::create("bloodbar/enemyblood.png");
    m_penemybloodback = CCSprite::create("bloodbar/littlebloodback.png");

    // 放置小小英雄 我方
    m_pSprite->setPosition(ccp(185, 276));
    m_pbloodback->setAnchorPoint(Vec2(0.8, 0.5));  // 设置锚点
    m_pblood->setAnchorPoint(Vec2(0.2, 0.5));  // 设置锚点
    m_pbloodback->setPosition(ccp(120, 160));
    m_pbloodback->setScale(0.4);
    m_pblood->setPosition(ccp(72, 80));

    m_pSprite->addChild(m_pbloodback);
    m_pbloodback->addChild(m_pblood);
    addChild(m_pSprite, 0);

    //敌方
    m_penemy->setPosition(ccp(1020, 800));
    m_penemybloodback->setAnchorPoint(Vec2(0.8, 0.5));  // 设置锚点
    m_penemyblood->setAnchorPoint(Vec2(0.2, 0.5));  // 设置锚点
    m_penemybloodback->setPosition(ccp(130, 140));
    m_penemybloodback->setScale(0.4);
    m_penemyblood->setPosition(ccp(72, 80));

    m_penemy->addChild(m_penemybloodback);
    m_penemybloodback->addChild(m_penemyblood);
    addChild(m_penemy, 0);

    //血条初始化
   // m_penemyblood->setScaleX(enemyhero_current_blood / enemyhero_max_blood);

    //// 设置鼠标点击事件监听
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(BattleScene::onTouchBeganLITTLE, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    int myHero = 0; //我方英雄数
    int opHero = 0; //对方英雄数
    vector<Hero*>mHero, oHero;

    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            if (chessboardBattle[i][j].second != nullptr) {
                if (chessboardBattle[i][j].second->isRed()) {
                    oHero.push_back(chessboardBattle[i][j].second);
                    opHero++;
                }
                else {
                    mHero.push_back(chessboardBattle[i][j].second);
                    myHero++;
                }
            }
        }
    }

    while (opHero && myHero) {
        for (int i = 0; i < mHero.size(); i++) {
            if (mHero[i] == nullptr) {
                continue;
            }
            float mindistance = 999999;
            int min_no = -1;
            for (int j = 0; j < oHero.size(); j++) {
                if (oHero[j] == nullptr) {
                    continue;
                }
                float distance = mHero[i]->getPosition().distance(oHero[j]->getPosition());
                if (distance <= mindistance) {
                    mindistance = distance;
                    min_no = j;
                }
            }

            //if (mindistance <= mHero[i]->getAttackDistance()) {
            if (min_no != -1) {
                oHero[min_no]->changeHP((1 - oHero[min_no]->getDefect() / 200) * mHero[i]->getAttack() + (1 - oHero[min_no]->getAPdefect() / 200) * mHero[i]->getAP());
                oHero[min_no]->bloodbar->setScaleX(oHero[min_no]->getHP() / oHero[min_no]->getFullHP());
                if (oHero[min_no]->getHP() <= 0) {
                    opHero--;
                    oHero[min_no]->removeFromParent();
                    oHero[min_no]->release();
                    oHero[min_no] = nullptr;
                }

               
            }
            /* else {
                 float moveDistance = 100.0f; // 移动距离
                 Vec2 targetPosition = oHero[min_no]->getPosition(); // 目标位置为与当前我方英雄最近的对方英雄的位置
                 Vec2 direction = (targetPosition - mHero[i]->getPosition()).getNormalized(); // 移动方向向量

                 Vec2 destination = mHero[i]->getPosition() + direction * moveDistance; // 计算目标移动位置

                 // 创建 MoveTo 动作，使英雄向目标位置移动
                 auto moveTo = MoveTo::create(5.0f, destination);
                 mHero[i]->runAction(moveTo);

                 auto func = [i, targetPosition, destination, this, mHero](float dt) {
                     Vec2 currentPosition = mHero[i]->getPosition();
                     float distance = currentPosition.distance(destination);
                     if (distance <=5) {
                         mHero[i]->stopAllActions();
                         this->unschedule("check_distance");
                     }
                     };

                 this->schedule(func, "check_distance");
             }*/

             //else {
             //    
             //    auto moveTo = MoveTo::create(5.0f, Vec2(640,480)); // 创建 MoveBy 动作，使精灵向右移动 100 距离
             //    Vec2 orgin = mHero[i]->getPosition();
             //    mHero[i]->runAction(moveTo);
             //    auto func = std::bind([]( Vec2 orgin, Sprite* sprite) {
             //        auto currentPosition = sprite->getPosition();
             //        if (currentPosition.distance(orgin) >= 50) {
             //            sprite->stopAllActions();
             //        }
             //        }, orgin, mHero[i]);
             //    schedule(func, "check_distance");
             //}
        }

        for (int i = 0; i < oHero.size(); i++) {
            if (oHero[i] == nullptr) {
                continue;
            }
            float mindistance = 999999;
            int min_no = -1;
            for (int j = 0; j < mHero.size(); j++) {
                if (mHero[j] == nullptr) {
                    continue;
                }
                float distance = oHero[i]->getPosition().distance(mHero[j]->getPosition());
                if (distance <= mindistance) {
                    mindistance = distance;
                    min_no = j;
                }
            }
            //if (mindistance < oHero[i]->getAttackDistance()) {
            if (min_no != -1) {
                mHero[min_no]->changeHP((1 - mHero[min_no]->getDefect() / 200) * oHero[i]->getAttack() + (1 - mHero[min_no]->getAPdefect() / 200) * oHero[i]->getAP());
                
                if (mHero[min_no]->getHP() <= 0) {
                    myHero--;
                    mHero[min_no]->removeFromParent();
                    mHero[min_no]->release();
                    mHero[min_no] = nullptr;
                    mHero.erase(mHero.begin() + min_no);
                }

               
            }
            /* else {
                 float moveDistance = 100.0f; // 移动距离
                 Vec2 targetPosition = mHero[min_no]->getPosition(); // 目标位置为与当前我方英雄最近的对方英雄的位置
                 Vec2 direction = (targetPosition - oHero[i]->getPosition()).getNormalized(); // 移动方向向量

                 Vec2 destination = oHero[i]->getPosition() + direction * moveDistance; // 计算目标移动位置

                 // 创建 MoveTo 动作，使英雄向目标位置移动
                 auto moveTo = MoveTo::create(5.0f, destination);
                 oHero[i]->runAction(moveTo);

                 auto func = [i, targetPosition, destination, this, oHero](float dt) {
                     Vec2 currentPosition = oHero[i]->getPosition();
                     float distance = currentPosition.distance(destination);
                     if (distance <= 5) {
                         oHero[i]->stopAllActions();
                         this->unschedule("check_distance");
                     }
                     };

                 this->schedule(func, "check_distance");
             }*/
             /*else {
                 auto moveBy = MoveBy::create(1.0f, 100 * (mHero[min_no]->getPosition() - oHero[i]->getPosition()).getNormalized());
                 oHero[i]->runAction(moveBy);
             }*/
        }
    }
    auto visibleSize = Director::getInstance()->getVisibleSize(); //屏幕可见区域的大小
    Vec2 origin = Director::getInstance()->getVisibleOrigin(); //原点坐标   

    auto label = Label::createWithTTF("", "fonts/Marker Felt.ttf", 75); //创建标签
    label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 2 * visibleSize.height / 3)); //标签位置
    label->setTextColor(Color4B::BLACK); //设置颜色
    this->addChild(label, 1); //添加到场景中

    if (myHero) { //胜利        
        label->setString("Win!");
        enemyhero_current_blood -= 20;
        //m_penemyblood->setScaleX(enemyhero_current_blood / enemyhero_max_blood);
    }
    else { //失败
        label->setString("Lose.");
        littlehero_current_blood -= 20;
        //m_pblood->setScaleX(littlehero_current_blood / littlehero_max_blood);
    }

    this->scheduleOnce([&](float dt) {
        coinCount += 5;
        updateButtonState(upbutton);
        shopLabelState();
        // 重新开始倒计时
        playscene->currentTime = playscene->totalTime;
        playscene->schedule(schedule_selector(PlayingScene::updateProgressBar), 0.01f);
        Director::getInstance()->popScene();
        }, 5.0f, "delayedCallback");
}

//小小英雄移动
void BattleScene::moveSpriteTo(Vec2 destination) {

    // 限制目标位置在边界内
    if ((destination.x > 200 && destination.x < 1050) && (destination.y > 250 && destination.y < 800))
    {
        // 创建精灵，并设置位置
        auto click = CCSprite::create("SpecialEffect/click.png");
        click->setPosition(destination);
        // 将精灵添加到场景
        addChild(click, 0);
        auto delay = DelayTime::create(0.25f);
        auto removeAction = CallFunc::create([this, click]() {
            // 移除精灵
            click->removeFromParent();
            });
        click->runAction(Sequence::create(delay, removeAction, nullptr));
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
bool BattleScene::onTouchBeganLITTLE(Touch* touch, Event* event)
{
    // 停止当前正在进行的移动动作
    m_pSprite->stopAllActions();
    // 获取鼠标点击的位置
    Vec2 touchLocation = touch->getLocation();
    // 调用移动函数，将精灵移动到鼠标点击的位置
    moveSpriteTo(touchLocation);
    return true;  // 返回true表示消耗了该事件
}

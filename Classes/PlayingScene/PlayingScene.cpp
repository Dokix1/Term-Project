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

const int numRows = 6; //��
const int numCols = 6; //��
const float tileSize = 90.0F; //���Ӵ�С
int selectedHeroIndex = -1;

pair<int, Hero*> heroCard[5] = { {-1, nullptr},{-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr} }; //����
pair<int, Hero*> prepare[9] = { {-1, nullptr},{-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr} }; //��ս

vector<vector<Hero*>> chessboard(numRows, vector<Hero*>(numCols, nullptr)); //��������

void findNearestHero(int i, int j, bool opponent, int& x, int& y);
pair<int, int> isWithinAttackRange(int x, int y, bool opponent);
void moveHero();

/* ��������ƿ� ��ʼ������ */
void PlayingScene::randCard() {
    srand(time(0));

    for (int i = 0; i < 5; i++) 
        heroCard[i].first = rand() % 6; 
}

/* ����һ��Scene���� */
Scene* PlayingScene::createScene() {
    return PlayingScene::create();
}

/* ����󷵻������� */
void PlayingScene::menuCloseCallback(Ref* pSender) {
    auto newScene = StartGameScene::create(); //������
    Director::getInstance()->replaceScene(newScene); //�л���������
}

void PlayingScene::shoponButtonClicked(Ref* sender) {

    shopbutton->setEnabled(false);
    shopbutton->loadTextures("buttons/ShopSelected.png", "buttons/ShopSelected.png", "buttons/ShopSelected.png");
    auto popupLayer = PopupLayer::create();
    this->addChild(popupLayer);
}

bool PlayingScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    Vec2 touchPos = touch->getLocation(); //��ȡ����λ��

    /* ʵ��Ӣ���ƶ� */
    for (int i = 0; i < 9; i++) { //������ս����
        if (selectedHeroIndex != -1) {
            /* �жϿ����Ƿ������̷�Χ�� */
            if (touchPos.x >= 310 && touchPos.x <= 310 + tileSize * 6 &&
                touchPos.y >= 210 && touchPos.y <= 210 + tileSize * 6) {
                //��ѡ�е�Ӣ���ƶ��������ϵĴ���λ��
                //ȷ�������������ϵ�λ��
                int row = (touchPos.y - 210) / tileSize;
                int col = (touchPos.x - 310) / tileSize;
                //���㿨����6*6�����ϵ�����
                float cardX = 310 + col * tileSize + tileSize / 2;
                float cardY = 210 + row * tileSize + tileSize / 2;
                prepare[selectedHeroIndex].second->setPosition(cardX, cardY);

                chessboard[row][col] = prepare[selectedHeroIndex].second; //��ӵ���������
                prepare[selectedHeroIndex].second->release(); //�ͷſ���
            }
            else { //ȡ��ѡ�в��ָ�Ӣ�۵�ԭʼ״̬
                //prepare[selectedHeroIndex].second->setOutlineColor(Color4B::WHITE); //�ָ�������ɫ
                //prepare[selectedHeroIndex].second->setOutlineWidth(0); //�ָ��������
            }
            selectedHeroIndex = -1; // ����ѡ�е�Ӣ������
            return true;
        }

        if (prepare[i].first != -1 && prepare[i].second->getBoundingBox().containsPoint(touchPos)) {
            //����λ���Ƿ��ڱ�ս�����Ӣ����
            selectedHeroIndex = i; //����ѡ�е�Ӣ������
            prepare[i].second->retain();  //��ֹӢ�۱��ͷ�
            //prepare[i].second->setOutlineColor(Color4B::YELLOW); //��������ɫ��Ϊ��ɫ
            //prepare[i].second->setOutlineWidth(5); //���������������߿ɼ���

            return true; //ѡ��
        }
    }

    return false; //δѡ��
}

/*����ʱ*/
void PlayingScene::updateProgressBar(float dt) {
    //����ʣ��ʱ��
    currentTime -= dt;
    if (currentTime < 0) {
        currentTime = 0;
    }

    //���½�����
    float progress = currentTime / totalTime;
    timer->setPercentage(progress * 100);

    //����ʱ���ǩ
    int remainingTime = static_cast<int>(ceil(currentTime));
    timeLabel->setString(std::to_string(remainingTime));

    //����Ƿ�ʱ���Ѿ�����
    if (currentTime <= 0) {
        unschedule(schedule_selector(PlayingScene::updateProgressBar));
        //moveHero(); //���������ʱ�䵽��Ĵ����߼�
    }
}

/* ����������Ч */
void PlayingScene::menuSetMusicCallback(Ref* pSender) {
    auto newScene = SetMusicScene::create();
    Director::getInstance()->pushScene(newScene); //�л���������Ч���� ��ǰ�������볡��ջ��
}

void PlayingScene::uponButtonClicked(Ref* sender) {
    // ����Ӣ���ƶ�����
    if (selectedHeroIndex != -1) {
        // ����Ӣ���ƶ��߼�

        selectedHeroIndex = -1; // ����ѡ�е�Ӣ������
    }
}

/* ��ʼ��PlayingScene�������� */
bool PlayingScene::init() {
    if (!Scene::init()) //��ʼ��
        return false; //��ʼ��ʧ��

    auto visibleSize = Director::getInstance()->getVisibleSize(); //��Ļ�ɼ�����Ĵ�С
    Vec2 origin = Director::getInstance()->getVisibleOrigin(); //ԭ������   

    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/playingBGM.mp3", true); //���ű�������
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.6F); //���ñ������ֵ�����

    /* �������� */
    auto background = Sprite::create("ChessBoard/playing.png");
    background->setContentSize(Size(visibleSize.width, visibleSize.height));
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(background, 0);

    randCard(); //��ʼ������

    /* �˳�������Ϸ�˵��� */
    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
        CC_CALLBACK_1(PlayingScene::menuCloseCallback, this));
    float scale = 5.0;
    closeItem->setScale(4); //�Ŵ�4��
    float x = origin.x + visibleSize.width - closeItem->getContentSize().width * scale / 2;
    float y = origin.y + closeItem->getContentSize().height * scale / 2;
    closeItem->setPosition(Vec2(x, y)); //������ʾλ��

    /* ������Ч�˵��� */
    auto setMusic = MenuItemImage::create("Music/setting.png", "Music/setting.png",
        CC_CALLBACK_1(PlayingScene::menuSetMusicCallback, this));
    setMusic->setScale(0.5);
    x = origin.x + 11 * visibleSize.width / 12;
    y = origin.y + 8 * visibleSize.height / 9;
    setMusic->setPosition(Vec2(x, y));

    /* �����˵� */
    auto menu = Menu::create(closeItem, setMusic, nullptr); //��Ӳ˵���
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /* ������ť */ 
    shopbutton = Button::create("buttons/ShopNormal.png", "buttons/ShopSelected.png");
    shopbutton->setPosition(Vec2(0.875 * visibleSize.width, 0.125 * visibleSize.height));
    shopbutton->addClickEventListener(CC_CALLBACK_1(PlayingScene::shoponButtonClicked, this));
    addChild(shopbutton);

    /* ������ť */ 
    upbutton = Button::create("buttons/UpgradeNormal.png", "buttons/UpgradeSelected.png");
    upbutton->setPosition(Vec2(0.125 * visibleSize.width, 0.130 * visibleSize.height));
    upbutton->addClickEventListener(CC_CALLBACK_1(PlayingScene::uponButtonClicked, this));
    addChild(upbutton);

    /* ����ʱ */

    //����������
    progressBackground = cocos2d::Sprite::create("Bar/countdownP.png");
    progressBackground->setPosition(visibleSize.width / 2, visibleSize.height / 40 * 39);
    addChild(progressBackground);

    //����������
    timer = cocos2d::ProgressTimer::create(cocos2d::Sprite::create("Bar/countdown.png"));
    timer->setType(cocos2d::ProgressTimer::Type::BAR);
    timer->setMidpoint(cocos2d::Vec2(0, 0.5));
    timer->setBarChangeRate(cocos2d::Vec2(1, 0));
    timer->setPosition(visibleSize.width / 2, visibleSize.height / 40 * 39);
    addChild(timer);

    //����ʱ���ǩ
    timeLabel = cocos2d::Label::createWithTTF("", "fonts/arial.ttf", 24);
    timeLabel->setPosition(visibleSize.width * 0.325, visibleSize.height / 40 * 39);
    addChild(timeLabel);

    //��ʼ��ʱ��
    totalTime = 10.0f;
    currentTime = totalTime;

    //������ʱ��
    schedule(schedule_selector(PlayingScene::updateProgressBar), 0.01f);

    //�����Ļ����Ӣ���ƶ�
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(PlayingScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

/* Ѱ�Ҿ�������ĶԷ�Ӣ�� */
void findNearestHero(int i, int j, bool opponent, int& x, int& y) {
    int distance = numRows * numCols; //��ʼ������Ϊ������ֵ
    x = -1;
    y = -1;

    for (int m = 0; m < numRows; m++)
        for (int n = 0; n < numCols; n++)
            if (chessboard[m][n]->isRed() == opponent) {
                int tmp = abs((m - i) * (n - j)); //��������ƽ��
                if (distance > tmp) {
                    distance = tmp;
                    x = m;
                    y = n;
                }
            }
}

/* �ж��Ƿ��ڹ�����Χ�� */
pair<int, int> isWithinAttackRange(int x, int y, bool opponent) {
    int attackRange = chessboard[x][y]->getAttackDistance(); //��ȡ������Χ

    for (int i = 0; i < numRows; i++)
        for (int j = 0; j < numCols; j++)
            if (chessboard[i][j]->isRed() == opponent) { //�ø����жԷ�Ӣ��
                int distance = (x - i) + (y - j); //�������
                if (distance <= attackRange)
                    return { i,j }; //���Թ��������ضԷ�Ӣ������
            }

    return { -1, -1 }; //û���ڹ�����Χ�ڵĶԷ�Ӣ��
}

/* Ӣ���ƶ� */
void moveHero() {
    while (1) {
        bool noFight = true; //�Ƿ����δ��ս��Ӣ�۵ı�־

        /* �������̵�ÿһ������ */
        for (int i = 0; i < numRows; i++)
            for (int j = 0; j < numCols; j++) 
                if (chessboard[i][j] != NULL&& chessboard[i][j]!=nullptr) {
                    noFight = false; //����δ��սӢ��
                    bool currentPlayer = chessboard[i][j]->isRed(); //1�Է� 0����

                    pair<int, int> pos = isWithinAttackRange(i, j, !currentPlayer);
                    if (pos.first != -1) {
                        Hero* h1 = chessboard[i][j];
                        Hero* h2 = chessboard[pos.first][pos.second];
                        //�ڹ�����Χ�ڣ����й���
                        //ʵ�ֹ������߼����˺����㡢��������
                    }

                    int x, y;
                    findNearestHero(i, j, !currentPlayer, x, y); //Ѱ�Ҿ��뵱ǰλ������ĶԷ�Ӣ��

                    int toX, toY; //�ƶ����λ��
                    if (x == i)
                        toX = i;
                    else
                        toX = (x > i) ? i + 1 : i - 1;

                    if (y == j)
                        toY = j;
                    else
                        toY = (y > j) ? j + 1 : j - 1;

                    /* �ƶ�Ӣ�� */
                    chessboard[toX][toY] = chessboard[i][j];
                    chessboard[i][j] = nullptr;

                    pos = isWithinAttackRange(toX, toY, !currentPlayer);
                    if (pos.first != -1) {
                        Hero* h1 = chessboard[toX][toY];
                        Hero* h2 = chessboard[pos.first][pos.second];
                        //�ڹ�����Χ�ڣ����й���
                        //ʵ�ֹ������߼����˺����㡢��������
                    }
                }      
            
        if (noFight)
            break;
    }
}
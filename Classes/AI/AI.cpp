#include "SimpleAudioEngine.h"
#include "../PlayingScene/PlayingScene.h"
#include "../StartGame/StartGame.h"
#include "../SetMusic/SetMusicScene.h"
#include <vector>
#include <time.h>
#include <string>
#include <utility>
#include "../PopupLayer/PopupLayer.h"
#include "../AI/AI.h"
USING_NS_CC;
using namespace CocosDenshion;
using namespace std;
using namespace ui;

int countprepare = 0;
int countboard = 0;
int a = 0;

pair<int, Hero*> AImy_hero = make_pair(-1, nullptr);
pair<int, Hero*> AIheroCard[5] = { {-1, nullptr},{-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr} }; //卡牌
pair<int, Hero*> AIprepare[9] = { {-1, nullptr},{-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr}, {-1, nullptr} }; //备战

vector<vector<pair<int, Hero*>>> AIchessboard(6, vector<pair<int, Hero*>>(6, make_pair(-1, nullptr))); //棋盘数组

//升级
void AI::AIlevelup() {
    money -= (2 * AIlevel - 2);
    AIlevel++;
}

void AI::AICardsinit() {
    for (int i = 0; i < 5; i++)
        AIheroCard[i].first = rand() % 6;

    for (int i = 0; i < 5; i++)
        switch (AIheroCard[i].first) {
        case 0:
            AIheroCard[i].second = new HeroPhysicalTank(true, 1);
            break;
        case 1:
            AIheroCard[i].second = new HeroMagicTank(true, 1);
            break;
        case 2:
            AIheroCard[i].second = new HeroPhysicalWarrior(true, 1);
            break;
        case 3:
            AIheroCard[i].second = new HeroMagicalWarrior(true, 1);
            break;
        case 4:
            AIheroCard[i].second = new HeroMarksman(true, 1);
            break;
        case 5:
            AIheroCard[i].second = new HeroMage(true, 1);
            break;
        }
}

//升星
void AI::AIStartUp(int number) {
    int num = 0;
    bool first_hero = true;

    for (int start = 1; start < 3; start++) {
        num = 0;
        first_hero = true;
        for (int i = 0; i < 9; i++)
            if (AIprepare[i].first == number && AIprepare[i].second->getLevel() == start)
                num++;

        for (int i = 3; i < 6; i++)
            for (int j = 0; j < 6; j++)
                if (AIchessboard[i][j].first == number && AIchessboard[i][j].second->getLevel() == start)
                    num++;

        if (num == 3) {
            for (int i = 3; i < 6; i++)
                for (int j = 0; j < 6; j++)
                    if (AIchessboard[i][j].first == number && AIchessboard[i][j].second->getLevel() == start)
                        if (first_hero) {
                            AIchessboard[i][j].second->upLevel();
                            AIchessboard[i][j].second->setScale(0.9 + 0.3 * AIchessboard[i][j].second->getLevel());
                            first_hero = false;
                        }
                        else {
                            AIchessboard[i][j].first = -1;
                            population--;

                            AIchessboard[i][j].second->removeFromParent();
                            AIchessboard[i][j].second = nullptr;
                        }

            for (int i = 0; i < 9; i++)
                if (AIprepare[i].first == number && AIprepare[i].second->getLevel() == start)
                    if (first_hero) {
                        AIprepare[i].second->upLevel();
                        AIprepare[i].second->setScale(0.9 + 0.3 * AIprepare[i].second->getLevel());
                        first_hero = false;
                    }
                    else {
                        AIprepare[i].first = -1;
                        AIprepare[i].second->removeFromParent();
                        AIprepare[i].second = nullptr;
                    }
        }
    }
}


void AI::AIbettle() {

    AICardsinit();

    money += 5;

    population = 0;
    for (int i = 3; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (AIchessboard[i][j].first != -1)
            {
                population++;

            }
        }
    }


    if (population < AIlevel) {
        countprepare = 0;

        for (int i = 0; i < 5; i++) {
            countprepare = 0;
            for (int j = 3; j < 6 && countprepare == 0; j++)
                for (int p = 0; p < 6 && countprepare == 0; p++) {
                    if (AIheroCard[i].first != -1) {
                        if (AIheroCard[i].first == AIchessboard[j][p].first && AIheroCard[i].second->getCost() <= money)

                            for (int m = 0; m < 9 && countprepare == 0; m++)
                                if (AIprepare[m].first == -1) { //将卡牌移动到备战区域  
                                    AIprepare[m].first = AIheroCard[i].first;
                                    AIprepare[m].second = AIheroCard[i].second;
                                    AIheroCard[i].first = -1;
                                    AIheroCard[i].second = nullptr;

                                    money -= AIprepare[m].second->getCost();

                                    AIStartUp(AIprepare[m].first);

                                    countprepare++;
                                    break;
                                }
                    }


                }
        }
        for (int i = 0; i < 5; i++) {
            if (AIheroCard[i].first == -1)
                a++;
        }

        for (int i = 0; i < 5 && population + a < AIlevel; i++) {
            if (AIheroCard[i].first != -1 && AIheroCard[i].second->getCost() <= money)
            {
                for (int m = 0; m < 9 && countprepare == 0; m++)
                    if (AIprepare[m].first == -1) { //将卡牌移动到备战区域  
                        AIprepare[m].first = AIheroCard[i].first;
                        AIprepare[m].second = AIheroCard[i].second;
                        AIheroCard[i].first = -1;
                        AIheroCard[i].second = nullptr;

                        money -= AIprepare[m].second->getCost();

                        AIStartUp(AIprepare[m].first);

                        a++;
                        break;
                    }
            }

        }

    }
    else
        while (money >= (2 * AIlevel - 2))
            AIlevelup();


    population = 0;
    for (int i = 3; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (AIchessboard[i][j].first != -1)
            {
                population++;

            }
        }
    }


    for (int i = 0; i < 9; i++)
    {
        if (population < AIlevel + 3)
        {
            countboard = 0;
            if (AIprepare[i].first != -1 && countboard == 0)
            {
                if (AIprepare[i].second->getAttackDistance() < 400)
                {

                    for (int m = 3; m < 6 && countboard == 0; m++)
                    {
                        for (int n = 2; n < 6 && n >= 0 && countboard == 0;)
                        {

                            if (AIchessboard[m][n].first == -1)
                            {
                                AIchessboard[m][n].first = AIprepare[i].first;
                                AIchessboard[m][n].second = AIprepare[i].second;
                                AIprepare[i].first = -1;
                                AIprepare[i].second = nullptr;

                                population++;
                                countboard++;

                            }
                            if (n <= 1 && n >= 0)
                                n--;
                            else if (n == 5)
                                n = 1;
                            else if (n >= 2 && n <= 4)
                                n++;

                        }

                    }
                }
                else
                {
                    for (int m = 5; m >= 3 && countboard == 0; m--)
                    {
                        for (int n = 2; n < 6 && n >= 0 && countboard == 0;)
                        {

                            if (AIchessboard[m][n].first == -1)
                            {
                                AIchessboard[m][n].first = AIprepare[i].first;
                                AIchessboard[m][n].second = AIprepare[i].second;
                                AIprepare[i].first = -1;
                                AIprepare[i].second = nullptr;
                                population++;
                                countboard++;



                            }
                            if (n <= 1 && n >= 0) {
                                n--;
                            }

                            else if (n == 5) {
                                n = 1;
                            }
                            else if (n >= 2 && n <= 4) {
                                n++;
                            }
                        }

                    }

                }

            }
        }

    }





}







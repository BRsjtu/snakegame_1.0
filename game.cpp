#include <string>
#include <iostream>
#include <cmath>

// For terminal delay
#include <chrono>
#include <thread>

#include <fstream>
#include <algorithm>
#include <ctime>
#include "game.h"
clock_t start,now;
Game::Game()
{
    // Separate the screen to three windows
    this->mWindows.resize(3);
    initscr();
    // If there wasn't any key pressed don't wait for keypress
    nodelay(stdscr, true);
    // Turn on keypad control
    keypad(stdscr, true);
    // No echo for the key pressed
    noecho();
    // No cursor show
    curs_set(0);
    // Get screen and board parameters
    getmaxyx(stdscr, this->mScreenHeight, this->mScreenWidth);
    this->mGameBoardWidth = this->mScreenWidth - this->mInstructionWidth;
    this->mGameBoardHeight = this->mScreenHeight - this->mInformationHeight;

    this->createInformationBoard();
    this->createGameBoard();
    this->createInstructionBoard();

    // Initialize the leader board to be all zeros
    this->mLeaderBoard.assign(this->mNumLeaders, 0);
}

Game::~Game()
{
    for (int i = 0; i < this->mWindows.size(); i++)
    {
        delwin(this->mWindows[i]);
    }
    endwin();
}

void Game::createInformationBoard()
{
    int startY = 0;
    int startX = 0;
    this->mWindows[0] = newwin(this->mInformationHeight, this->mScreenWidth, startY, startX);
}

//void Game::renderInformationBoard() const
void Game::renderInformationBoard_classicMode() const
{
    mvwprintw(this->mWindows[0], 1, 1, "This is classic snakegame mode!");
    mvwprintw(this->mWindows[0], 2, 1, "Author: Lei Mao");
    mvwprintw(this->mWindows[0], 3, 1, "Website: https://github.com/leimao/");
    mvwprintw(this->mWindows[0], 4, 1, "Implemented using C++ and libncurses library.");
    wrefresh(this->mWindows[0]);
}
void Game::renderInformationBoard_propMode() const
{
    mvwprintw(this->mWindows[0], 1, 1, "This is prop mode!");
    mvwprintw(this->mWindows[0], 2, 1, "Author: Su Yingsheng");
    mvwprintw(this->mWindows[0], 3, 1, "Website: https://github.com/BRsjtu/snakegame_1.0");
    mvwprintw(this->mWindows[0], 4, 1, "You can touch different props to have corresponding ability.");
    wrefresh(this->mWindows[0]);
}
void Game::renderInformationBoard_survivalMode() const
{
    mvwprintw(this->mWindows[0], 1, 1, "This is survival mode!");
    mvwprintw(this->mWindows[0], 2, 1, "Author: Zeng Qiwen");
    mvwprintw(this->mWindows[0], 3, 1, "Website: https://github.com/BRsjtu/snakegame_1.0");
    mvwprintw(this->mWindows[0], 4, 1, "Try to keep your snake as shorter as possible.");
    wrefresh(this->mWindows[0]);
}

void Game::createGameBoard()
{
    int startY = this->mInformationHeight;
    int startX = 0;
    this->mWindows[1] = newwin(this->mScreenHeight - this->mInformationHeight, this->mScreenWidth - this->mInstructionWidth, startY, startX);
}

void Game::renderGameBoard() const
{
    wrefresh(this->mWindows[1]);
}

void Game::createInstructionBoard()
{
    int startY = this->mInformationHeight;
    int startX = this->mScreenWidth - this->mInstructionWidth;
    this->mWindows[2] = newwin(this->mScreenHeight - this->mInformationHeight, this->mInstructionWidth, startY, startX);
}

void Game::renderInstructionBoard_classicMode() const
{
    mvwprintw(this->mWindows[2], 1, 1, "Manual");

    mvwprintw(this->mWindows[2], 3, 1, "Up: W");
    mvwprintw(this->mWindows[2], 4, 1, "Down: S");
    mvwprintw(this->mWindows[2], 5, 1, "Left: A");
    mvwprintw(this->mWindows[2], 6, 1, "Right: D");

    mvwprintw(this->mWindows[2], 8, 1, "Difficulty");
    mvwprintw(this->mWindows[2], 11, 1, "Points");

    wrefresh(this->mWindows[2]);
}


void Game::renderInstructionBoard_propMode() const
{
    mvwprintw(this->mWindows[2], 1, 1, "Manual");

    mvwprintw(this->mWindows[2], 3, 1, "switch head and tail");
    mvwprintw(this->mWindows[2], 4, 1, "eat yourself");
    mvwprintw(this->mWindows[2], 5, 1, "dizzy");
    mvwprintw(this->mWindows[2], 6, 1, "more prop...");

    mvwprintw(this->mWindows[2], 8, 1, "Difficulty");
    mvwprintw(this->mWindows[2], 11, 1, "Points");

    wrefresh(this->mWindows[2]);
}


void Game::renderInstructionBoard_survivalMode() const
{
    mvwprintw(this->mWindows[2], 1, 1, "Manual");

    mvwprintw(this->mWindows[2], 3, 1, "length increasing");
    mvwprintw(this->mWindows[2], 4, 1, "food decrease length");
    mvwprintw(this->mWindows[2], 5, 1, "length to 2");
    mvwprintw(this->mWindows[2], 6, 1, "go into the door");

    mvwprintw(this->mWindows[2], 8, 1, "Difficulty");
    mvwprintw(this->mWindows[2], 11, 1, "Time");

    wrefresh(this->mWindows[2]);
}

void Game::renderLeaderBoard() const
{
    // If there is not too much space, skip rendering the leader board
    if (this->mScreenHeight - this->mInformationHeight - 14 - 2 < 3 * 2)
    {
        return;
    }
    mvwprintw(this->mWindows[2], 14, 1, "Leader Board");
    std::string pointString;
    std::string rank;
    for (int i = 0; i < std::min(this->mNumLeaders, this->mScreenHeight - this->mInformationHeight - 14 - 2); i++)
    {
        pointString = std::to_string(this->mLeaderBoard[i]);
        rank = "#" + std::to_string(i + 1) + ":";
        mvwprintw(this->mWindows[2], 14 + (i + 1), 1, rank.c_str());
        mvwprintw(this->mWindows[2], 14 + (i + 1), 5, pointString.c_str());
    }
    wrefresh(this->mWindows[2]);
}

bool Game::renderRestartMenu()
{
    WINDOW* menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = { "classic mode","prop mode","survival mode", "two-player mode","Quit"};

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, 1, "Your Final Score:");
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(menu, 2, 1, pointString.c_str());
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 0 + offset, 1, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 1 + offset, 1, menuItems[1].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 2 + offset, 1, menuItems[2].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 3 + offset, 1, menuItems[3].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 4 + offset, 1, menuItems[4].c_str());
    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch (key)
        {
        case 'W':
        case 'w':
        case KEY_UP:
        {
            mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
            index--;
            index = (index < 0) ? menuItems.size() - 1 : index;
            wattron(menu, A_STANDOUT);
            mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
            wattroff(menu, A_STANDOUT);
            break;
        }
        case 'S':
        case 's':
        case KEY_DOWN:
        {
            mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
            index++;
            index = (index > menuItems.size() - 1) ? 0 : index;
            wattron(menu, A_STANDOUT);
            mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
            wattroff(menu, A_STANDOUT);
            break;
        }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0)
    {
        this->setModeSelect(1);//classic mode
        return true;
    }
    else if (index == 1)
    {
        this->setModeSelect(2);//prop mode
        return true;
    }
    else if (index == 2)
    {
        this->setModeSelect(3);//survival mode
        return true;
    }
    else if (index == 3)
    {
        this->setModeSelect(4);//two-player mode
        return true;
    }
    else
    {
        return false;//quit
    }

}

void Game::renderPoints() const
{
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(this->mWindows[2], 12, 1, pointString.c_str());
    wrefresh(this->mWindows[2]);
}
void Game::renderSurvivalTime() const
{
    std::string pointString = std::to_string(this->survival_time);
    mvwprintw(this->mWindows[2], 12, 1, pointString.c_str());
    wrefresh(this->mWindows[2]);
}

void Game::renderDifficulty() const
{
    std::string difficultyString = std::to_string(this->mDifficulty);
    mvwprintw(this->mWindows[2], 9, 1, difficultyString.c_str());
    wrefresh(this->mWindows[2]);
}

void Game::initializeGame()
{
    this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));
    this->createRamdonFood();
    this->mPtrSnake->senseFood(this->mFood);
    this->mDifficulty = 0;
    this->mPoints = 0;
    this->mDelay = this->mBaseDelay;
}
void Game::initializeGame_propMode()
{
    this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));
    this->createRamdomFood_PorpMode();
    this->mPtrSnake->senseFood(this->mFood);
    this->createRamdomProp();
    this->mDifficulty = 0;
    this->mPoints = 0;
    this->mDelay = this->mBaseDelay;
}
void Game::initializeGame_survivalMode()
{
    this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength_survivalMode));
    this->createRamdonFood();
    this->mPtrSnake->senseFood(this->mFood);
    this->mDifficulty = 0;
    this->mPoints = 0;
    this->mDelay = this->mBaseDelay;
}
void Game::initializeGame_twoMode()
{
    this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength,1));
    this->nPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength,2));
    this->createRamdonFood();
    this->mPtrSnake->senseFood(this->mFood);
    this->nPtrSnake->senseFood(this->mFood);
    this->mDifficulty = 0;
    this->mPoints = 0;
    this->mDelay = this->mBaseDelay;
}
void Game::createRamdonFood()
{
    std::vector<SnakeBody> availableGrids;
    for (int i = 1; i < this->mGameBoardHeight - 1; i++)
    {
        for (int j = 1; j < this->mGameBoardWidth - 1; j++)
        {
            if (this->mPtrSnake->isPartOfSnake(j, i))
            {
                continue;
            }
            else
            {
                availableGrids.push_back(SnakeBody(j, i));
            }
        }
    }

    // Randomly select a grid that is not occupied by the snake
    int random_idx = std::rand() % availableGrids.size();
    this->mFood = availableGrids[random_idx];
}

void Game::renderFood() const
{
    mvwaddch(this->mWindows[1], this->mFood.getY(), this->mFood.getX(), this->mFoodSymbol);
    wrefresh(this->mWindows[1]);
}

void Game::renderSnake() const
{
    int snakeLength = this->mPtrSnake->getLength();
    std::vector<SnakeBody>& snake = this->mPtrSnake->getSnake();
    for (int i = 0; i < snakeLength; i++)
    {
        mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), this->mSnakeSymbol);
    }
    wrefresh(this->mWindows[1]);
}

void Game::rendernSnake() const
{
    int snakeLength = this->nPtrSnake->getLength();
    std::vector<SnakeBody>& snake = this->nPtrSnake->getSnake();
    for (int i = 0; i < snakeLength; i++)
    {
        mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), this->mSnakeSymbol);
    }
    wrefresh(this->mWindows[1]);
}

void Game::controlSnake() const
{
    int key;
    key = getch();
    switch (key)
    {
    case 'W':
    case 'w':
    case KEY_UP:
    {
        this->mPtrSnake->changeDirection(Direction::Up);
        break;
    }
    case 'S':
    case 's':
    case KEY_DOWN:
    {
        this->mPtrSnake->changeDirection(Direction::Down);
        break;
    }
    case 'A':
    case 'a':
    case KEY_LEFT:
    {
        this->mPtrSnake->changeDirection(Direction::Left);
        break;
    }
    case 'D':
    case 'd':
    case KEY_RIGHT:
    {
        this->mPtrSnake->changeDirection(Direction::Right);
        break;
    }
    default:
    {
        break;
    }
    }
}

void Game::controlmSnake() const
{
    int key;
    key = getch();
    switch (key)
    {
    case 'W':
    case 'w':
    {
        this->mPtrSnake->changeDirection(Direction::Up);
        break;
    }
    case 'S':
    case 's':
    {
        this->mPtrSnake->changeDirection(Direction::Down);
        break;
    }
    case 'A':
    case 'a':
    {
        this->mPtrSnake->changeDirection(Direction::Left);
        break;
    }
    case 'D':
    case 'd':
    {
        this->mPtrSnake->changeDirection(Direction::Right);
        break;
    }
    case KEY_UP:
    {
        this->nPtrSnake->changeDirection(Direction::Up);
        break;
    }
    case KEY_DOWN:
    {
        this->nPtrSnake->changeDirection(Direction::Down);
        break;
    }
    case KEY_LEFT:
    {
        this->nPtrSnake->changeDirection(Direction::Left);
        break;
    }
    case KEY_RIGHT:
    {
        this->nPtrSnake->changeDirection(Direction::Right);
        break;
    }
    default:
    {
        break;
    }
    }
}

void Game::controlnSnake() const
{
    int key;
    key = getch();
    switch (key)
    {
    case KEY_UP:
    {
        this->nPtrSnake->changeDirection(Direction::Up);
        break;
    }
    case KEY_DOWN:
    {
        this->nPtrSnake->changeDirection(Direction::Down);
        break;
    }
    case KEY_LEFT:
    {
        this->nPtrSnake->changeDirection(Direction::Left);
        break;
    }
    case KEY_RIGHT:
    {
        this->nPtrSnake->changeDirection(Direction::Right);
        break;
    }
    default:
    {
        break;
    }
    }
}
//void Game::renderBoards() const
void Game::renderBoards_classicMode() const
{
    for (int i = 0; i < this->mWindows.size(); i++)
    {
        werase(this->mWindows[i]);
    }
    //this->renderInformationBoard();
    this->renderInformationBoard_classicMode();
    this->renderGameBoard();
    this->renderInstructionBoard_classicMode();
    for (int i = 0; i < this->mWindows.size(); i++)
    {
        box(this->mWindows[i], 0, 0);
        wrefresh(this->mWindows[i]);
    }
    this->renderLeaderBoard();
}

void Game::renderBoards_propMode() const
{
    for (int i = 0; i < this->mWindows.size(); i++)
    {
        werase(this->mWindows[i]);
    }
    this->renderInformationBoard_propMode();
    this->renderGameBoard();
    this->renderInstructionBoard_propMode();
    for (int i = 0; i < this->mWindows.size(); i++)
    {
        box(this->mWindows[i], 0, 0);
        wrefresh(this->mWindows[i]);
    }
    this->renderLeaderBoard();
}

void Game::renderBoards_survivalMode() const
{
    for (int i = 0; i < this->mWindows.size(); i++)
    {
        werase(this->mWindows[i]);
    }
    this->renderInformationBoard_survivalMode();
    this->renderGameBoard();
    this->renderInstructionBoard_survivalMode();
    for (int i = 0; i < this->mWindows.size(); i++)
    {
        box(this->mWindows[i], 0, 0);
        wrefresh(this->mWindows[i]);
    }
    this->renderLeaderBoard();
}
void Game::renderBoards_twoMode() const
{
    for (int i = 0; i < this->mWindows.size(); i++)
    {
        werase(this->mWindows[i]);
    }
    this->renderInformationBoard_classicMode();//todo
    this->renderGameBoard();
    this->renderInstructionBoard_classicMode();//todo
    for (int i = 0; i < this->mWindows.size(); i++)
    {
        box(this->mWindows[i], 0, 0);
        wrefresh(this->mWindows[i]);
    }
    this->renderLeaderBoard();
}

void Game::adjustDelay()
{
    this->mDifficulty = this->mPoints / 5;
    if (mPoints % 5 == 0)
    {
        this->mDelay = this->mBaseDelay * pow(0.75, this->mDifficulty);
    }
}
void Game::adjustDelay_SurvivalMode(){
    this->mDifficulty = this->survival_time / 10; //该值需调试，10的话可能难度偏高（十秒加快一次）
    if(survival_time % 10 == 0){
        this->mDelay = this->mBaseDelay * pow(0.75,this->mDifficulty);
    }
}
void Game::runGame()
{
    bool moveSuccess;
    int key;
    while (true)
    {
        this->controlSnake();
        werase(this->mWindows[1]);
        box(this->mWindows[1], 0, 0);

        bool eatFood = this->mPtrSnake->moveFoward();
        bool collision = this->mPtrSnake->checkCollision();
        if (collision == true)
        {
            break;
        }
        this->renderSnake();
        if (eatFood == true)
        {
            this->mPoints += 1;
            this->createRamdonFood();
            this->mPtrSnake->senseFood(this->mFood);
            this->adjustDelay();
        }
        this->renderFood();
        this->renderDifficulty();
        this->renderPoints();

        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));

        refresh();
    }
}

void Game::runGame_propMode()
{
    bool moveSuccess;
    int key;
    while (true)
    {
        this->controlSnake();
        werase(this->mWindows[1]);
        box(this->mWindows[1], 0, 0);
        bool eatProp = this->mPtrSnake->moveFoward_PropMode();//检测是否能吃到道具
        bool eatFood = this->mPtrSnake->moveFoward();//检测是否能吃到食物
        //判断此时蛇能否吃掉自己
        if(this->mPtrSnake->getIfCanEatSelf())
        {
            if(this->mPtrSnake->checkCollision_AllowEatSelf())
            {
                break;
            }
        }
        else
        {
            if (this->mPtrSnake->checkCollision())
            {
                break;
            }
        }
        this->renderSnake();
        if (eatFood == true)
        {
            this->mPoints += 1;
            this->createRamdomFood_PorpMode();
            this->mPtrSnake->senseFood(this->mFood);
            this->adjustDelay();
        }
        if (eatProp == true)
        {
            this->mPoints += 1;
            this->createRamdomProp();
            this->adjustDelay();
        }
        this->renderFood();
        this->renderProp();
        this->renderDifficulty();
        this->renderPoints();
        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));

        refresh();
    }
}

void Game::runGame_survivalMode()
{
    start=clock();//计时开始
    int count_time=0;//用于计算的时间（int类型）
    while (true)
    {
        this->controlSnake();
        werase(this->mWindows[1]);
        box(this->mWindows[1], 0, 0);
        const int longer_time = 2;//蛇长度变长的时间
        const int food_time=4;//每隔4s产生一个道具
        now=clock();//获取实时时间
        double survived_time=(double)((now-start)/CLOCKS_PER_SEC-count_time);//生存的时间（double类型）
        if(survived_time>=1){
            count_time++;
        }
        this->survival_time = count_time;//survival_time是game类里面的时间
        if(count_time%longer_time==0){
            this->mPtrSnake->createNewHead();
        }
        bool eatFood = this->mPtrSnake->moveFoward_SurvivalMode();
        bool collision = this->mPtrSnake->checkCollision();
        if (collision == true)
        {
            break;
        }
        this->renderSnake();
        if (eatFood == true&&count_time%food_time==0)
        {
            this->createRamdonFood();
            this->mPtrSnake->senseFood(this->mFood);
            this->adjustDelay();
        }
        this->renderFood();
        this->renderDifficulty();
        this->renderSurvivalTime();
        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));

        refresh();
    }
}

void Game::runGame_twoMode()
{
    bool moveSuccess;
    int key;
    while (true)
    {
        this->controlmSnake();
        //this->controlnSnake();
        werase(this->mWindows[1]);
        box(this->mWindows[1], 0, 0);

        bool meatFood = this->mPtrSnake->moveFoward();
        bool neatFood = this->nPtrSnake->moveFoward();

        bool mcollision = this->mPtrSnake->checkCollision();
        bool ncollision = this->nPtrSnake->checkCollision();

        if (mcollision == true || ncollision == true)
        {
            break;
        }
        this->renderSnake();
        this->rendernSnake();
        if (meatFood == true|| neatFood == true)
        {
            this->mPoints += 1;
            this->createRamdonFood();
            this->mPtrSnake->senseFood(this->mFood);
            this->adjustDelay();
        }
        this->renderFood();
        this->renderDifficulty();
        this->renderPoints();

        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));

        refresh();
    }
}

void Game::startGame()
{
    refresh();
    bool choice;
    //int gamemode = this->modeSelect;//1:classic mode��2:prop mode��3:survival mode
    int gamemode = 1;
    while (true)
        //switch (gamemode)
    {
        //case 1://1:classic mode
        {
            switch (gamemode)
            {
            case 1://1:classic mode
            //while (true)
            {
                this->readLeaderBoard();
                this->renderBoards_classicMode();
                this->initializeGame();
                this->runGame();
                this->updateLeaderBoard();
                this->writeLeaderBoard();
                choice = this->renderRestartMenu();
                break;
            }

            case 2://2:prop mode
            {
                this->readLeaderBoard();
                this->renderBoards_propMode();
                this->initializeGame_propMode();
                this->runGame_propMode();
                this->updateLeaderBoard();
                this->writeLeaderBoard();
                choice = this->renderRestartMenu();
                break;
            }
            case 3://3:survival mode
            {
                this->readLeaderBoard();
                this->renderBoards_survivalMode();
                this->initializeGame_survivalMode();
                this->runGame_survivalMode();
                this->updateLeaderBoard();
                this->writeLeaderBoard();
                choice = this->renderRestartMenu();
                break;
            }
            case 4://4:two-player mode
            {
                this->readLeaderBoard();
                this->renderBoards_twoMode();
                this->initializeGame_twoMode();
                this->runGame_twoMode();
                this->updateLeaderBoard();
                this->writeLeaderBoard();
                choice = this->renderRestartMenu();
                break;
            }
            }
            gamemode = this->modeSelect;
            if (choice == false)
            {
                break;
            }
        }
    }
}
// https://en.cppreference.com/w/cpp/io/basic_fstream
bool Game::readLeaderBoard()
{
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.in);
    if (!fhand.is_open())
    {
        return false;
    }
    int temp;
    int i = 0;
    while ((!fhand.eof()) && (i < mNumLeaders))
    {
        fhand.read(reinterpret_cast<char*>(&temp), sizeof(temp));
        this->mLeaderBoard[i] = temp;
        i++;
    }
    fhand.close();
    return true;
}

bool Game::updateLeaderBoard()
{
    bool updated = false;
    int newScore = this->mPoints;
    for (int i = 0; i < this->mNumLeaders; i++)
    {
        if (this->mLeaderBoard[i] >= this->mPoints)
        {
            continue;
        }
        int oldScore = this->mLeaderBoard[i];
        this->mLeaderBoard[i] = newScore;
        newScore = oldScore;
        updated = true;
    }
    return updated;
}

bool Game::writeLeaderBoard()
{
    // trunc: clear the data file
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.trunc | fhand.out);
    if (!fhand.is_open())
    {
        return false;
    }
    for (int i = 0; i < this->mNumLeaders; i++)
    {
        fhand.write(reinterpret_cast<char*>(&this->mLeaderBoard[i]), sizeof(this->mLeaderBoard[i]));;
    }
    fhand.close();
    return true;
}

void Game::setModeSelect(int mode) {
    modeSelect = mode;
}

//Prop part
//随机创造道具的函数
void Game::createRamdomProp()
{
    std::vector<SnakeBody> availableGrids;
    for (int i = 1; i < this->mGameBoardHeight - 1; i++)
    {
        for (int j = 1; j < this->mGameBoardWidth - 1; j++)
        {
            if (this->mPtrSnake->isPartOfSnake(j, i) || this->mPtrSnake->isPartOfProp(j, i) || this->mFood == SnakeBody(j,i))
            {
                continue;
            }
            else
            {
                availableGrids.push_back(SnakeBody(j, i));
            }
        }
    }

    // Randomly select a grid that is not occupied by the snake
    int random_idx = std::rand() % availableGrids.size();//获取道具位置
    int random_prop = std::rand() % 3 ;//获取道具类型
    PropType newprop;
    switch (random_prop)
    {
    case 0:
        newprop = PropType::reserveSnake;
        break;
    case 1:
        newprop = PropType::decreaseSize;
        break;
    case 2:
        newprop = PropType::allowEatSelf;
        break;
    }
    this->mPtrSnake->setMyProp(SnakeBody(availableGrids[random_idx].getX(), availableGrids[random_idx].getY(), newprop));
}

//道具模式中创造食物
void Game::createRamdomFood_PorpMode()
{
    std::vector<SnakeBody> availableGrids;
    for (int i = 1; i < this->mGameBoardHeight - 1; i++)
    {
        for (int j = 1; j < this->mGameBoardWidth - 1; j++)
        {
            if (this->mPtrSnake->isPartOfSnake(j, i)|| this->mPtrSnake->isPartOfProp(j,i))
            {
                continue;
            }
            else
            {
                availableGrids.push_back(SnakeBody(j, i));
            }
        }
    }

    // Randomly select a grid that is not occupied by the snake
    int random_idx = std::rand() % availableGrids.size();
    this->mFood = availableGrids[random_idx];
}

void Game::renderProp() const
{
    std::vector<SnakeBody> prop = this->mPtrSnake->getMyProp();
    for(int i = 0; i < prop.size(); i++)
    {
        if(prop[i].getPropType() == PropType::reserveSnake)
        {
            mvwaddch(this->mWindows[1], prop[i].getY(), prop[i].getX(), this->mPropSymbolReserve);
        }
        if(prop[i].getPropType() == PropType::decreaseSize)
        {
            mvwaddch(this->mWindows[1], prop[i].getY(), prop[i].getX(), this->mPropSymbolDecrease);
        }
        if(prop[i].getPropType() == PropType::allowEatSelf)
        {
            mvwaddch(this->mWindows[1], prop[i].getY(), prop[i].getX(), this->mPropSymbolAllow);
        }
        
    }
    
    wrefresh(this->mWindows[1]);
}








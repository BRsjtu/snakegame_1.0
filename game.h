#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include <string>
#include <vector>
#include <memory>

#include "snake.h"


class Game
{
public:
    Game();
    ~Game();
    
		void createInformationBoard();
    void renderInformationBoard() const;

    void createGameBoard();
    void renderGameBoard() const;
    
		void createInstructionBoard();
    void renderInstructionBoard() const;
		
		void loadLeadBoard();
    void updateLeadBoard();
    bool readLeaderBoard();
    bool updateLeaderBoard();
    bool writeLeaderBoard();
    void renderLeaderBoard() const;
    
		void renderBoards() const;
    
		void initializeGame();
    void runGame();
    void renderPoints() const;
    void renderDifficulty() const;
    
		void createRamdonFood();
    void renderFood() const;
    void renderSnake() const;
    void controlSnake() const;
    
		void startGame();
    bool renderRestartMenu();//删除了const为了修改modeSelect
    void adjustDelay();
    //int getCurrentTime();//获取游戏进行的时间（单位：秒）
    void setModeSelect(int mode);//设置模式，修改modeSelect的值

    //prop
    void createRamdonProp();
    void selectProp(PropType prop);
    void ReserveSnake();
    void DecreaseSize();
    void AllowEatSelf();

private:
    // We need to have two windows
    // One is for game introduction
    // One is for game mWindows
    int mScreenWidth;
    int mScreenHeight;
    int mGameBoardWidth;
    int mGameBoardHeight;
    const int mInformationHeight = 6;
    const int mInstructionWidth = 18;
    std::vector<WINDOW *> mWindows;
    // Snake information
    const int mInitialSnakeLength = 2;
    const char mSnakeSymbol = '@';
    std::unique_ptr<Snake> mPtrSnake;
    // Food information
    SnakeBody mFood;
    const char mFoodSymbol = '#';
    int mPoints = 0;
    int mDifficulty = 0;
    int mBaseDelay = 100;
    int mDelay;
    const std::string mRecordBoardFilePath = "record.dat";
    std::vector<int> mLeaderBoard;
    const int mNumLeaders = 3;
    //int runTime;
    int modeSelect;//1:classic mode，2:prop mode，3:survival mode
    //prop
    std::vector<SnakeBody> foodAndProp;
    PropType mPropType;
    void Game::selectProp();
};

#endif

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
    //void renderInformationBoard() const;
    void renderInformationBoard_classicMode() const;

    void createGameBoard();
    void renderGameBoard() const;

	void createInstructionBoard();
    //void renderInstructionBoard() const;
    void renderInstructionBoard_classicMode() const;

		void loadLeadBoard();
    void updateLeadBoard();
    bool readLeaderBoard();
    bool updateLeaderBoard();
    bool writeLeaderBoard();
    void renderLeaderBoard() const;

		//void renderBoards() const;
    void renderBoards_classicMode() const;
    void renderBoards_propMode() const;
    void renderBoards_survivalMode() const;
		void initializeGame();
    void runGame();
    void runGame_propMode();
    void runGame_survivalMode();
    void renderPoints() const;
    void renderDifficulty() const;

		void createRamdonFood();
    void renderFood() const;
    void renderSnake() const;
    void controlSnake() const;

		void startGame();
    bool renderRestartMenu();//ɾ����constΪ���޸�modeSelect
    void adjustDelay();
    //int getCurrentTime();//��ȡ��Ϸ���е�ʱ�䣨��λ���룩
    void setModeSelect(int mode);//����ģʽ���޸�modeSelect��ֵ

    //prop
    void createRamdomProp();
    void selectProp(PropType prop);
    void adjustDelay_SurvivalMode;
private:
    // We need to have two windows
    // One is for game introduction
    // One is for game mWindows
    int survival_time;
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
    int modeSelect;//1:classic mode��2:prop mode��3:survival mode
    //prop
    PropType mPropType;
};

#endif

#ifndef SNAKE_H
#define SNAKE_H

#include <vector>

enum class Direction
{
    Up = 0,
    Down = 1,
    Left = 2,
    Right = 3,
};

enum class PropType
{
    reserveSnake = 0,
    decreaseSize = 1,
    allowEatSelf = 2,
};

class SnakeBody
{
public:
    SnakeBody();
    SnakeBody(int x, int y);
    SnakeBody(int x, int y, PropType prop);
    int getX() const;
    int getY() const;
    PropType getPropType() const;
    bool operator == (const SnakeBody& snakeBody);
private:
    int mX;
    int mY;
    PropType mPropType;
};

// Snake class should have no depency on the GUI library
class Snake
{
public:
    //Snake();
    Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength);
    // Set random seed
    void setRandomSeed();
    // Initialize snake
    void initializeSnake();
    // Check if the snake is on the coordinate
    // bool isSnakeOn(int x, int y);
    // Checking API for generating random food
    bool isPartOfSnake(int x, int y);
    void senseFood(SnakeBody food);
    // Check if hit wall
    bool hitWall();
    bool touchFood();
    bool hitSelf();
    bool checkCollision();
    bool changeDirection(Direction newDirection);
    std::vector<SnakeBody>& getSnake();
    int getLength();
    SnakeBody createNewHead();
    void selectProp();
    bool moveFoward();
    bool moveFoward_PropMode();
    bool moveFoward_SurvivalMode();
    //prop
    bool isPartOfProp(int x, int y);
    void setMyProp(SnakeBody prop);
    //void senseProp_PropMode(std::vector<SnakeBody> prop);
    bool touchProp_PropMode();
    void ReserveSnake();
    void DecreaseSize();
    void AllowEatSelf();
    bool checkCollision_AllowEatSelf();
    bool getIfCanEatSelf();
    void moveFoward_EatSelf();
    std::vector<SnakeBody> getMyProp () const;


private:
    const int mGameBoardWidth;
    const int mGameBoardHeight;
    // Snake information
    const int mInitialSnakeLength;
    Direction mDirection;
    SnakeBody mFood;
    std::vector<SnakeBody> mSnake;
    //Prop Part
    std::vector<SnakeBody> mProp;
    SnakeBody mTouchedProp;
    PropType mPropType;
    bool ifCanEatSelf = 0 ;
};

#endif

# ̰贪吃蛇的进阶功能实现

## 1. 道具模式
### 1.1 道具具体介绍  

我们设定道具模式中一共有三个类型的道具：

+ 第一个就是**reserveSnake**道具，作用是当蛇吃到该道具时会倒转整条蛇，使它沿着原本的方向往回走。这个道具的符号是$\mathbf{\$}$，吃到这个道具也会得到**1**分的加分。
+ 第二个就是**decreaseSize**道具，它的作用是当这只蛇吃到道具时会减少长度。这个道具的符号是$\mathbf{\%}$，同样地，吃到该道具也会得到**1**分的加分。
+ 第三个就是**allowEatSelf**道具，它的作用是改变蛇关于是否能吃自己的状态。当蛇现在可以吃自己时，若是它吃到了这一个道具，那就不能再吃自己了，而若是原本它此时不能吃自己，然后吃到了这一个道具，那么它就可以吃自己的身体了。蛇吃掉自己身体的效果是蛇将从那一截开始断开。这个道具的符号是$\mathbf{\&}$，它所拥有的分数也是**1**分。
+ 除此之外我们还保留了普通模式中**Food**，这样能够让小蛇不断增长了，食物也保留同样1分的加分。
  
### 1.2 道具模式框架  

&emsp;为使道具的选择更加简便，我们用**enum**枚举了一个名为**PropType**的数据类型，这就使得我们能更好的利用到**PropType**这一个数据类型。
&emsp;同时我们给**SnakeBody**重载了一个构造函数**SnakeBody(int x, int y, PropType prop);**，使得**SnakeBody**能够兼容道具这个数据类型。**SnakeBody**类中也有一个**private**权限的**mPropType**变量，类外则通过通过**getPropType**函数来获取**SnakeBody**的**mPropType**变量。
&emsp;在**Snake**类中，我们建立了**isPartOfProp**函数来判断某一个**SnakeBody**是否在**Snake**中的**mProp**内；建立了**setMyProp**函数来将随机得到的道具添加到**mProp**内，同时还有**getMyProp**函数提供获取**mProp**的接口；建立了**touchProp_PropMode**函数来判断下一步行动会不会触碰到已有的道具；建立了**moveFoward_PropMode**函数来实现吃道具这个行为；建立了**ReserveSnake**、**DecreaseSize**和**AllowEatSelf**三个函数来实现道具的功能，同时辅以**IfCanEatSelf**变量来判断该情况下蛇是否能吃掉自己，还有就是借助**getIfCanEatSelf**函数来判断蛇在此时是否可以吃掉自己的身体；还有用**checkCollision_AllowEatSelf**函数来规定在蛇可以吃掉自己时的碰撞情况；最后还有**moveFoward_EatSelf**函数来实现吃到自己的情况。
&emsp;在**Game**类中，我们用**createRamdomProp**函数来实现随机分配道具的地址这个功能，并且用**createRamdomFood_PorpMode**函数来实现在道具模式下随机分配食物的这个功能；还有**selectProp**函数则是在吃到道具时，判断道具的**PropType**类型决定接下来的操作；还有**renderProp**函数用来实现道具的图形在游戏窗口内的展示。
&emsp;在**startGame**函数中通过模式选择进入道具模式的代码模块,分别实现**readLeaderBoard**、**renderBoards_propMode**、**initializeGame_propMode**、**runGame_propMode**、**updateLeaderBoard**、**writeLeaderBoard**这六个函数。
&emsp;**readLeaderBoard**和**renderBoards_propMode**函数都只是针对性地对内容进行了一些修改，**initializeGame_propMode**和**runGame_propMode**函数则是运行道具模式的主体，**updateLeaderBoard**和**writeLeaderBoard**函数也是简单对排行榜进行更新，最后则是通过**renderRestartMenu**回到选择菜单。
&emsp;对道具模式运行主体函数**runGame_propMode**进行研究，**while**循环每执行一次都要检测一下是否吃到了食物，是否吃到了道具、在能吃自己的情况下是否吃到了自己，或者是否发生了碰撞导致这局游戏结束。其中**checkCollision_AllowEatSelf**函数就是为了判断在能吃自己的情况下是否吃发生了碰撞，而**checkCollision**则是要判断在不能吃自己的情况下是否发生了碰撞。在吃到食物的情况基本与普通模式相同，只是在创造新食物的时候需要用到**createRamdomFood_PorpMode**函数。吃到道具的情况也相差不大，基本上是吃到道具之后就**createRamdomProp**创建新道具。加分、还有调整难度所需要用到的**adjustDelay**函数与普通模式也完全相同。  

### 1.3 reserveSnake道具的实现  

&emsp;**reserveSnake**道具的实现主要依靠**Snake**类中的**ReserveSnake**函数，该函数通过将**this->Snake**这个**vector**中的多个**SnakeBody**倒转后就得到了新蛇，同时通过倒转后第一和第二个**SnakeBody**的位置就确定好了新蛇的**Direction**。  

### 1.4 decreaseSize道具的实现  

&emsp;**decreaseSize**道具的实现主要通过删除**Snake**类中**this->mSnake**的最后一个元素，即**this->mSnake.pop_back()**即可完成该道具的实现。  

### 1.5 allowEatSelf道具的实现

&emsp;在**Snake**类中，我们建立了**isPartOfProp**函数来判断某一个**SnakeBody**是否在**Snake**中的**mProp**内；建立了**setMyProp**函数来将随机得到的道具添加到**mProp**内，同时还有**getMyProp**函数提供获取**mProp**的接口；建立了**touchProp_PropMode**函数来判断下一步行动会不会触碰到已有的道具；建立了**ReserveSnake**、**DecreaseSize**和**AllowEatSelf**三个函数来实现道具的功能，同时辅以**IfCanEatSelf**变量来判断该情况下蛇是否能吃掉自己，还有就是借助**getIfCanEatSelf**函数来判断蛇在此时是否可以吃掉自己的身体；还有用**checkCollision_AllowEatSelf**函数来规定在蛇可以吃掉自己时的碰撞情况；最后还有**moveFoward_EatSelf**函数来实现吃到自己的情况。
&emsp;**allowEatSelf**道具的是实现主要是通过**IfCanEatSelf**变量的值来决定蛇是否可以吃自己的身体。（**IfCanEatSelf**初始值为0）在**selectProp**函数中选择**allowEatSelf**的**PropType**类型之后就可以进入**AllowEatSelf**函数中改变**IfCanEatSelf**的值。**runGame_propMode**函数中运行**moveFoward_PropMode**时，若是**IfCanEatSelf**值为1，即可以吃自己时，那么蛇就会通过**moveFoward_EatSelf**再次检测是否吃到了自己，这就实现了**allowEatSelf**道具的功能。

## 2. 生存模式
### 2.1 生存模式具体介绍

设计这个模式时，运用了反向思维。普通的贪吃蛇都是通过吃东西得分并且使自己变得更长。逆向思维后，我设计出蛇会随着时间变长，并且根据生存时间的得分的模式。但是很明显，单纯的随时间变长，然后失败是毫无乐趣的游戏体验。因此，我增加了一个道具，使得用户在碰到这个道具时可以使蛇的长度变短，并降低难度系数，使得玩家更有参与感。另外，随时间增加的不仅只有长度，还有难度系数（即蛇的速度）。所以，尽你所能，活久一点。

###2.2 生存模式框架

&emsp;生存模式一个重难点是计时器，为此我们在头文件加入了**ctime**，并使用了其中的**clock**函数，该函数能够一毫秒+1，除以1000后即是一秒加1的计时器。
&emsp
## 3. 双人模式
增加了一条蛇，分别用wasd和↑↓←→控制，增加游戏趣味性，目前还没有开发更多功能，可以考虑的是两条蛇的关系：竞争or合作，从而对应调用接口，希望通过细调参数（食物产生速度和个数）和分开显示各自的分数来使双人模式更有可玩性。
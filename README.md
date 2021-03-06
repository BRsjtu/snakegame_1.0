# ̰贪吃蛇的进阶功能实现

#1 整体框架
![代码框架](%E4%BB%A3%E7%A0%81%E6%A1%86%E6%9E%B6.png "title")


#2 三种进阶模式
![模式](模式.png "title")

## 2.1 道具模式
### 2.1.1 道具具体介绍  

我们设定道具模式中一共有三个类型的道具：

+ 第一个就是**reserveSnake**道具，作用是当蛇吃到该道具时会倒转整条蛇，使它沿着原本的方向往回走。这个道具的符号是$\mathbf{\$}$，吃到这个道具也会得到**1**分的加分。
+ 第二个就是**decreaseSize**道具，它的作用是当这只蛇吃到道具时会减少长度。这个道具的符号是$\mathbf{\%}$，同样地，吃到该道具也会得到**1**分的加分。(当蛇长度为1时，吃到**decreaseSize**道具，将不再起作用)
+ 第三个就是**allowEatSelf**道具，它的作用是改变蛇关于是否能吃自己的状态。当蛇现在可以吃自己时，若是它吃到了这一个道具，那就不能再吃自己了，而若是原本它此时不能吃自己，然后吃到了这一个道具，那么它就可以吃自己的身体了。蛇吃掉自己身体的效果是蛇将从那一截开始断开。这个道具的符号是$\mathbf{\&}$，它所拥有的分数也是**1**分。
+ 除此之外我们还保留了普通模式中**Food**，这样能够让小蛇不断增长了，食物也保留同样1分的加分。
  
### 2.1.2 道具模式框架  

&emsp;为使道具的选择更加简便，我们用**enum**枚举了一个名为**PropType**的数据类型，这就使得我们能更好的利用到**PropType**这一个数据类型。
&emsp;同时我们给**SnakeBody**重载了一个构造函数**SnakeBody(int x, int y, PropType prop);**，使得**SnakeBody**能够兼容道具这个数据类型。**SnakeBody**类中也有一个**private**权限的**mPropType**变量，类外则通过通过**getPropType**函数来获取**SnakeBody**的**mPropType**变量。
&emsp;在**Snake**类中，我们建立了**isPartOfProp**函数来判断某一个**SnakeBody**是否在**Snake**中的**mProp**内；建立了**setMyProp**函数来将随机得到的道具添加到**mProp**内，同时还有**getMyProp**函数提供获取**mProp**的接口；建立了**touchProp_PropMode**函数来判断下一步行动会不会触碰到已有的道具；建立了**moveFoward_PropMode**函数来实现吃道具这个行为；建立了**ReserveSnake**、**DecreaseSize**和**AllowEatSelf**三个函数来实现道具的功能，同时辅以**IfCanEatSelf**变量来判断该情况下蛇是否能吃掉自己，还有就是借助**getIfCanEatSelf**函数来判断蛇在此时是否可以吃掉自己的身体；还有用**checkCollision_AllowEatSelf**函数来规定在蛇可以吃掉自己时的碰撞情况；最后还有**moveFoward_EatSelf**函数来实现吃到自己的情况。
&emsp;在**Game**类中，我们用**createRamdomProp**函数来实现随机分配道具的地址这个功能，并且用**createRamdomFood_PorpMode**函数来实现在道具模式下随机分配食物的这个功能；还有**selectProp**函数则是在吃到道具时，判断道具的**PropType**类型决定接下来的操作；还有**renderProp**函数用来实现道具的图形在游戏窗口内的展示。
&emsp;在**startGame**函数中通过模式选择进入道具模式的代码模块,分别实现**readLeaderBoard**、**renderBoards_propMode**、**initializeGame_propMode**、**runGame_propMode**、**updateLeaderBoard**、**writeLeaderBoard**这六个函数。
&emsp;**readLeaderBoard**和**renderBoards_propMode**函数都只是针对性地对内容进行了一些修改，**initializeGame_propMode**和**runGame_propMode**函数则是运行道具模式的主体，**updateLeaderBoard**和**writeLeaderBoard**函数也是简单对排行榜进行更新，最后则是通过**renderRestartMenu**回到选择菜单。
&emsp;对道具模式运行主体函数**runGame_propMode**进行研究，**while**循环每执行一次都要检测一下是否吃到了食物，是否吃到了道具、在能吃自己的情况下是否吃到了自己，或者是否发生了碰撞导致这局游戏结束。其中**checkCollision_AllowEatSelf**函数就是为了判断在能吃自己的情况下是否吃发生了碰撞，而**checkCollision**则是要判断在不能吃自己的情况下是否发生了碰撞。在吃到食物的情况基本与普通模式相同，只是在创造新食物的时候需要用到**createRamdomFood_PorpMode**函数。吃到道具的情况也相差不大，基本上是吃到道具之后就**createRamdomProp**创建新道具。加分、还有调整难度所需要用到的**adjustDelay**函数与普通模式也完全相同。  

### 2.1.3 reserveSnake道具的实现  

&emsp;**reserveSnake**道具的实现主要依靠**Snake**类中的**ReserveSnake**函数，该函数通过将**this->Snake**这个**vector**中的多个**SnakeBody**倒转后就得到了新蛇，同时通过倒转后第一和第二个**SnakeBody**的位置就确定好了新蛇的**Direction**。  

### 2.1.4 decreaseSize道具的实现  

&emsp;**decreaseSize**道具的实现主要通过删除**Snake**类中**this->mSnake**的最后一个元素，即**this->mSnake.pop_back()**即可完成该道具的实现。  

### 2.1.5 allowEatSelf道具的实现

&emsp;在**Snake**类中，我们建立了**isPartOfProp**函数来判断某一个**SnakeBody**是否在**Snake**中的**mProp**内；建立了**setMyProp**函数来将随机得到的道具添加到**mProp**内，同时还有**getMyProp**函数提供获取**mProp**的接口；建立了**touchProp_PropMode**函数来判断下一步行动会不会触碰到已有的道具；建立了**ReserveSnake**、**DecreaseSize**和**AllowEatSelf**三个函数来实现道具的功能，同时辅以**IfCanEatSelf**变量来判断该情况下蛇是否能吃掉自己，还有就是借助**getIfCanEatSelf**函数来判断蛇在此时是否可以吃掉自己的身体；还有用**checkCollision_AllowEatSelf**函数来规定在蛇可以吃掉自己时的碰撞情况；最后还有**moveFoward_EatSelf**函数来实现吃到自己的情况。
&emsp;**allowEatSelf**道具的是实现主要是通过**IfCanEatSelf**变量的值来决定蛇是否可以吃自己的身体。（**IfCanEatSelf**初始值为0）在**selectProp**函数中选择**allowEatSelf**的**PropType**类型之后就可以进入**AllowEatSelf**函数中改变**IfCanEatSelf**的值。**runGame_propMode**函数中运行**moveFoward_PropMode**时，若是**IfCanEatSelf**值为1，即可以吃自己时，那么蛇就会通过**moveFoward_EatSelf**再次检测是否吃到了自己，这就实现了**allowEatSelf**道具的功能。


## 2.2 生存模式
### 2.2.1 生存模式思路介绍

设计这个模式时，运用了反向思维。普通的贪吃蛇都是通过吃东西得分并且使自己变得更长。逆向思维后，我设计出蛇会随着时间变长，并且根据生存时间的得分的模式。但是很明显，单纯的随时间变长，然后失败是毫无乐趣的游戏体验。因此，我增加了一个道具，使得用户在碰到这个道具时可以使蛇的长度变短，并降低难度系数，使得玩家更有参与感。另外，随时间增加的不仅只有长度，还有难度系数（即蛇的速度）。所以，尽你所能，活久一点。

### 2.2.2 生存模式各板块介绍

&emsp;生存模式一个重难点是计时器，为此我们在头文件加入了**ctime**，并使用了其中的**clock**函数，该函数能够一毫秒+1，除以1000后即是一秒加1的计时器，该计时器为定义在**Game**类中的函数**gettime**。
&emsp;我们设定每隔1s，我们的蛇都会增长一次，因此当计时器每加1的时候，我们都会调用一次**addHead**函数，**addHead**函数是我们另写的用于增长蛇长度的函数，其定义在**Snake**类中，由之前的函数修改即可得到。
&emsp；关于道具，我们仍旧使用了和classicmode相似的**food**以及其衍生函数，但是对于吃到道具后产生的效果，我们将原本的**moveForward**函数改写为**moveForward_SurvivalMode**函数，使得当蛇吃到道具时，其长度缩短2，而不是变长。
&emsp：关于难度的调整，我们修改了**adjustDelay**为**adjustDelay_SurvivalMode**函数，使得难度每10s增加一次（会随着游戏分数增加逐渐变快），并且当蛇吃到道具时，当前难度-1（不能为负数）。蛇吃到道具时，会使得**game**中**decrease_difficulty**这一变量加一，而**decrease_difficulty**这一变量是**adjustDelay_SurvivalMode**函数的参数，在之前**adjustDelay**这一函数基础上，将难度系数减去**decrease_difficulty**这一变量，即可实现降低难度。
&emsp：选择**gamemode**为**survivalmode**后，与**propmode**类似，将其中部分改为**survivalmode**对应的函数即可运行，如**runGame_SurvivalMode**,**adjustDelay_SurvivalMode**。
&emsp：承载游戏的主体函数是**runGame_SurvivalMode**函数，在**while(true)**每次计时以及对是否吃到食物，是否要增长蛇的长度等等。当**checkCollision**函数返回值为false，即蛇撞到了墙或者撞到了自己，游戏结束,输出分数。。
&emsp：关于游戏的得分，我们初始设定每隔五秒钟得一分，为了适应难度以及速度的变化，得分速度会越来越快，（与当前难度系数**mDifficulty**成正比）。而为了实现这一改动，我们编写了**renderTime**以及**renderRestart_survivalmode**等若干个函数，使得返回的分数与其他模式相匹配，不会出现过高分数的情况。。
&emsp：计时器介绍具体介绍：在**game**类中定义**int gettime**函数，该函数返回值每毫秒加1，而之后在下面定义了**count_time**变量为每秒加1的变量，初始值为0的**lasttime**变量，以及随时间变化的**now**变量。计时器原理是每当**now**减去**lasttime**大于0（因为二者均为**int**类型，所以该值最小为1，即时间过了1s）时，**count_time**加一，而之后通过**count_time%1==0**这一语句，使得后面的蛇长度函数**addNewHead**增长一秒触发一次。

## 2.3 双人模式
增加了一条蛇，分别用**wasd**和**↑↓←→**控制，增加游戏趣味性，目前还没有开发更多功能，可以考虑的是两条蛇的关系：**竞争or合作**，从而对应调用接口，希望通过细调参数（食物产生速度和个数）和分开显示各自的分数来使双人模式更有可玩性。


#3 team work
![版本控制](版本控制2.png "title")
![版本控制](版本控制1.png "title")

我们采用GitHub desktop作为我们的版本控制工具，先期学习使用**pull、push、merge**等基本操作，为后续小组三个成员之间连续紧密合作打下基础，避免出现两位成员各自写了一部分代码后还需要进行手动合并，甚至是解决**conflict**或者其他**bug**。

#4 总结
在此次程序设计大作业贪吃蛇进阶功能项目中，我们小组三人分工协作，沟通交流，其中开了**三次进度会议**，进行了**若干次技术交流**，队员各取所长,互帮互助，利用**github desktop**初步学习锻炼了团队协作完成代码项目的能力，并且初入了**linux命令行**进行代码开发的大门，我们认为这次合作项目使我们各方面能力得到了很大提高，也发挥了我们的创造力和想象力，体验很好。

受时间所限，功能实现方面还有更进阶的预想，但正如这个项目名称所言，**实现进阶功能**，我认为代码有限，功能无限，层层深入，**只有想不到没有做不到**，或许这就是作为开发者应有的目标。


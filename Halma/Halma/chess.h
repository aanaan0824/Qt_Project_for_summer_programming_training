#ifndef CHESS_H
#define CHESS_H

#include <iostream>

class Chess{
private:


public:
    std::pair<int,int>arrayInfo={-1,-1};
    int pos_x=-1;
    int pos_y=-1;
    int playerType=0;
    bool atOwn=false;
    bool checkedOwn=false;
    bool atOpposite=false;
    bool checkedOpposite=false;

    Chess();
    Chess(int x,int y);
    Chess(int pt,int x,int y,bool ow,bool op);
    void changePos(std::pair<int,int>);
    int getType();
    int getPosx();
    int getPosy();
    bool judgeOwn();
    bool judgeOpposite();
};

/*class Player{
public:

};*/

#endif // CHESS_H

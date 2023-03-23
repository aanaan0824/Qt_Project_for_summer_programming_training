#include "chess.h"

Chess::Chess(){}

Chess::Chess(int x,int y):pos_x(x),pos_y(y){}

Chess::Chess(int pt,int x,int y,bool ow,bool op):playerType(pt),pos_x(x),pos_y(y),atOwn(ow),atOpposite(op){}

void Chess::changePos(std::pair<int,int> pos){
    pos_x=pos.first;
    pos_y=pos.second;
}

bool Chess::judgeOwn(){
    if(playerType==1){
        if(arrayInfo.first>=13){
            atOwn=true;
            return true;
        }
        else{
            atOwn=false;
            return false;
        }
    }
    else if(playerType==2){
        if(arrayInfo.first<=3){
            atOwn=true;
            return true;
        }
        else{
            atOwn=false;
            return false;
        }
    }
    else{
        atOwn=false;
        return false;
    }
}

bool Chess::judgeOpposite(){
    if(playerType==2){
        if(arrayInfo.first>=13){
            atOpposite=true;
            return true;
        }
        else{
            atOpposite=false;
            return false;
        }
    }
    else if(playerType==1){
        if(arrayInfo.first<=3){
            atOpposite=true;
            return true;
        }
        else{
            atOpposite=false;
            return false;
        }
    }
    else{
        atOpposite=false;
        return false;
    }
}

int Chess::getType(){
    return playerType;
}

int Chess::getPosx(){
    return pos_x;
}

int Chess::getPosy(){
    return pos_y;
}


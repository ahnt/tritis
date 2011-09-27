//
//  main.cpp
//  tetris
//
//  Created by Arend Hintze on 21.09.11.
//  Copyright 2011 KGI. All rights reserved.
//

#include <iostream>
#include <vector>
#include <time.h>
#include <termios.h>
#include "game.h"

using namespace std;


int main (int argc, const char * argv[])
{
    srand((unsigned int)time(NULL));
    int input;
    char c;
    string log;
    
    log.clear();
    tetrisGame *game=new tetrisGame;
    game->reset();
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
    
    // Once the buffering is turned off, the rest is simple.
    do{
        log.append(game->mapIntoString());
        game->showMap();
        cout<<"------"<<endl;
        cout<<"a< >d s| w@"<<endl;
        c = cin.get();
        cout<<endl;
        switch(c){
            case 'a':input=1; break;
            case 'd':input=2; break;
            case 's':input=0; break;
            default:input=3;break;
        }
    }while(game->makeUpdate(input));
    game->showMap();
    log.append(game->mapIntoString());
    cout<<"-GAME-OVER-"<<endl;
    cout<<log.c_str()<<endl;
    return 0;
}

//
//  game.h
//  tritis
//
//  Created by Arend on 9/27/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef tritis_game_h
#define tritis_game_h

#include <vector>
#include <iostream>
#include <string.h>

using namespace std;

#define xDim 4
#define yDim 6

class tile{
public:
    unsigned char pattern[3][3];
    int x,y,orientation;
    bool setup(int type,int nx, int ny,vector<vector<unsigned char> > *map);
    bool applyAction(int action,vector<vector<unsigned char> > *map);
    void insertIntoMap(vector<vector<unsigned char> > *map);
    void removeFromMap(vector<vector<unsigned char> > *map);
};

class tetrisGame{
public:
    tile *currentTile;
    int mainCounter;
    int score;
    int nextTile;
    vector<vector<unsigned char> > map;    
    void reset(void);
    void setup(void);
    void showMap(void);
    bool makeUpdate(int action);
    void clearRows(void);
    string mapIntoString(void);
};



#endif

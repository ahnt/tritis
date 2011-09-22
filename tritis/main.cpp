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
};

int main (int argc, const char * argv[])
{
    srand((unsigned int)time(NULL));
    int input;
    char c;
    tetrisGame *game=new tetrisGame;
    game->reset();
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
    
    // Once the buffering is turned off, the rest is simple.
    do{
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
    cout<<"-GAME-OVER-"<<endl;
    
    return 0;
}

//*** tile
bool tile::setup(int type,int nx, int ny,vector<vector<unsigned char> > *map){
    int i,j;
    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
            pattern[i][j]=0;
    x=nx;
    y=ny;
    orientation=0;
    switch(type){
        case 0: //the line
            pattern[0][1]=1;
            pattern[1][1]=1;
            pattern[2][1]=1;
            break;
        case 1: //the L
            pattern[1][1]=1;
            pattern[1][2]=1;
            pattern[2][1]=1;
            break;
    }
    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
            if(((*map)[x+i][y+j]==1)&&(pattern[i][j]==1))
                return false;
    return true;
}

void tile::insertIntoMap(vector<vector<unsigned char> > *map){
    int i,j;
    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
            if(pattern[i][j]==1)
                (*map)[x+i][y+j]=(unsigned char)1;
}
void tile::removeFromMap(vector<vector<unsigned char> > *map){
    int i,j;
    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
            if(pattern[i][j]==1)
                (*map)[x+i][y+j]=(unsigned char)0;    
}

bool tile::applyAction(int action,vector<vector<unsigned char> > *map){
    int i,j;
    bool retBool=true;
    switch(action){
        case 0: //drop to the bottom
            while(applyAction(5,map)){};
            //add new tile
            //check for score
            break;
        case 1://simple move left
            for(i=0;i<3;i++)
                for(j=0;j<3;j++)
                    if((pattern[i][j]==1)&&((*map)[x+i-1][y+j]==1))
                        retBool=false;
            if(retBool)
                x-=1;
            break;
        case 2://simple move right
            for(i=0;i<3;i++)
                for(j=0;j<3;j++)
                    if((pattern[i][j]==1)&&((*map)[x+i+1][y+j]==1))
                        retBool=false;
            if(retBool)
                x+=1;
            break;
        case 3:
            for(i=0;i<3;i++)
                for(j=0;j<3;j++)
                    if((pattern[j][2-i]==1)&&((*map)[x+i][y+j]==1))
                        retBool=false;
            if(retBool){
                unsigned char buffer[3][3];
                for(i=0;i<3;i++)
                    for(j=0;j<3;j++)
                        buffer[i][j]=pattern[i][j];
                for(i=0;i<3;i++)
                    for(j=0;j<3;j++)
                        pattern[j][2-i]=buffer[i][j];
            }
            break;
        case 5://simple drop one step
            for(i=0;i<3;i++)
                for(j=0;j<3;j++)
                    if((pattern[i][j]==1)&&((*map)[x+i][y+j+1]==1)){
                        retBool=false;
                        //you tried dropping a tile which wasn't possible, this means you should get a new tile
                    }
            if(retBool)
                y++;
            break;
    }
    return retBool;
}


//*** tetrisGame
void tetrisGame::reset(void){
    int x,y;
    map.resize(xDim+2);
    for(x=0;x<xDim+2;x++){
        map[x].resize(yDim+2);
        for(y=0;y<yDim+2;y++)
            if((x==0)||(x==xDim+1)||(y==yDim+1))
                map[x][y]=1;
            else
                map[x][y]=0;
    }
    currentTile=new tile;
    currentTile->setup(rand()&1,1,0,&map);
    mainCounter=0;
    nextTile=rand()&1;
}

void tetrisGame::showMap(void){
    int x,y;
    currentTile->insertIntoMap(&map);
    for(y=0;y<yDim+2;y++){
        for(x=0;x<xDim+2;x++)
            cout<<(int)map[x][y];
        cout<<endl;
    }
    currentTile->removeFromMap(&map);
}
bool tetrisGame::makeUpdate(int action){
    bool b=true;
    bool cont=true;
    currentTile->applyAction(action, &map);
    mainCounter++;
    
    if((mainCounter&7)==7){
        //drop a tile every 7th tick
        b=currentTile->applyAction(5, &map);
    }
    if((action==0)||(b==false)) //drop a tile to the floor
    {
        currentTile->insertIntoMap(&map);
        clearRows();
        if(currentTile->setup(nextTile,1,0,&map)==false){
            //game over!
            cont=false;
        }
        nextTile=rand()&1;
        mainCounter=0;
    }
    return cont;
}

void tetrisGame::clearRows(void){
    int i,j,c,k;
    c=0;
    for(i=yDim;i>0;i--){
        do{
            k=0;
            for(j=1;j<5;j++)
                k+=map[j][i];
            if(k==4){
                c++;
                int x,y;
                for(x=1;x<5;x++)
                    for(y=i;y>1;y--)
                        map[x][y]=map[x][y-1];
            }
        } while(k==4);
    }
    switch(c){
        case 0: /* no line removed*/ break;
        case 1: score+=1; break;
        case 2: score+=4; break;
        case 3: score+=16; break;
    }
}

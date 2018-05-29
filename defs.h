//
// Created by Maksymilian Wojczuk on 29/05/2018.
//

#ifndef PROJECT_RURY_DEFS_H
#define PROJECT_RURY_DEFS_H


#include <vector>

using namespace std;

typedef struct loc {
    int x,y;
} Loc;

typedef enum {
    I1,I2,L1,L2,L3,L4,T1,T2,T3,T4,X,
    House, Source, Rock, Empty
} landMark;


int Xsize, Ysize;
int SrcsNum;
loc *Srcs;
int HsNum;
loc *Houses;
int BlNum;
loc *Blocks;
string pipeTypes;

landMark **land;

typedef struct {
    loc loc;
    landMark pipeType;
} PipePlacement;


typedef struct {
    bool success;
    vector<PipePlacement> result;
} Result;


#endif //PROJECT_RURY_DEFS_H

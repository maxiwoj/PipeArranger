//
// Created by Maksymilian Wojczuk on 29/05/2018.
//

#ifndef PROJECT_RURY_DEFS_H
#define PROJECT_RURY_DEFS_H


#include <vector>
#include <string>
#include <map>

using namespace std;

typedef struct loc {
    int x,y;
} Loc;

typedef struct {
    loc localisation;
    string landMark;
} PipePlacement;


typedef struct {
    bool success;
    vector<PipePlacement> pipesPlacements;
} Result;

typedef enum LandConnection {
    Empty, Extra, Full, Blocked
} LandConnection;

typedef struct Pipe {
    string name;
    LandConnection up, right, down, left;
} Pipe;

void initializeStaticStructures();
bool operator==(Loc loc1, Loc loc2);
int manhattanDistance(Loc loc1, Loc loc2);

extern int Xsize, Ysize;
extern int SrcsNum;
extern loc *Srcs;
extern int HsNum;
extern loc *Houses;
extern int BlNum;
extern loc *Blocks;
extern string pipeTypes;


extern Pipe I1, I2, L1, L2, L3, L4, T1, T2, T3, T4, X;
extern map<char, vector<Pipe>> pipes;


#endif //PROJECT_RURY_DEFS_H

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




int Xsize, Ysize;
int SrcsNum;
loc *Srcs;
int HsNum;
loc *Houses;
int BlNum;
loc *Blocks;
string pipeTypes;

typedef struct {
    loc loc;
    string landMark;
} PipePlacement;


typedef struct {
    bool success;
    vector<PipePlacement> result;
} Result;

typedef enum LandConnection {
    Empty, Extra, Full, Blocked
} LandConnection;

typedef struct {
    string name;
    LandConnection up, right, down, left;
} Pipe;

Pipe I1 = (Pipe) {"I1", Full, Blocked, Full, Blocked};
Pipe I2 = (Pipe) {"I2", Blocked, Full, Blocked, Full};
Pipe L1 = (Pipe) {"L1", Full, Full, Blocked, Blocked};
Pipe L2 = (Pipe) {"L2", Blocked, Full, Full, Blocked};
Pipe L3 = (Pipe) {"L3", Blocked, Blocked, Full, Full};
Pipe L4 = (Pipe) {"L4", Full, Blocked, Blocked, Full};
Pipe T1 = (Pipe) {"T1", Full, Full, Blocked, Full};
Pipe T2 = (Pipe) {"T2", Full, Full, Full, Blocked};
Pipe T3 = (Pipe) {"T3", Blocked, Full, Full, Full};
Pipe T4 = (Pipe) {"T4", Full, Blocked, Full, Full};
Pipe X = (Pipe) {"X", Full, Full, Full, Full, };

map<char, vector<Pipe>> pipes = {
        make_pair('I', (vector){I1, I2}),
        make_pair('L', (vector){L1, L2, L3, L4}),
        make_pair('T', (vector){T1, T2, T3, T4}),
        make_pair('X', (vector){X})
};


#endif //PROJECT_RURY_DEFS_H

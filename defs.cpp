//
// Created by Maksymilian Wojczuk on 13/06/2018.
//

#include "defs.h"

using namespace std;

void initializeStaticStructures() {
    I1 = (Pipe) {"I1", Full, Blocked, Full, Blocked};
    I2 = (Pipe) {"I2", Blocked, Full, Blocked, Full};
    L1 = (Pipe) {"L1", Full, Full, Blocked, Blocked};
    L2 = (Pipe) {"L2", Blocked, Full, Full, Blocked};
    L3 = (Pipe) {"L3", Blocked, Blocked, Full, Full};
    L4 = (Pipe) {"L4", Full, Blocked, Blocked, Full};
    T1 = (Pipe) {"T1", Full, Full, Blocked, Full};
    T2 = (Pipe) {"T2", Full, Full, Full, Blocked};
    T3 = (Pipe) {"T3", Blocked, Full, Full, Full};
    T4 = (Pipe) {"T4", Full, Blocked, Full, Full};
    X = (Pipe) {"X", Full, Full, Full, Full, };
    pipes = {
            make_pair('I', (vector<Pipe>){I1, I2}),
            make_pair('L', (vector<Pipe>){L1, L2, L3, L4}),
            make_pair('T', (vector<Pipe>){T1, T2, T3, T4}),
            make_pair('X', (vector<Pipe>){X})
    };
}

bool operator==(Loc loc1, Loc loc2) {
    return loc1.x == loc2.x && loc1.y == loc2.y;
}

int manhattanDistance(const Loc loc1, const Loc loc2) {
    return abs(loc1.x - loc2.x) + abs(loc1.y - loc2.y);
}

//
// Created by Maksymilian Wojczuk on 10/06/2018.
//

#ifndef PROJECT_RURY_LAND_H
#define PROJECT_RURY_LAND_H


#include "defs.h"

typedef struct FieldInfo {
    LandConnection up;
    LandConnection down;
    LandConnection left;
    LandConnection right;
    bool taken;
} FieldInfo;

class Land {
    int Xsize, Ysize;
    LandConnection **horizontalLandConnections;
    LandConnection **verticalLandConnections;

    bool **takenFields;

    void markFieldSingleType(Loc loc, LandConnection type, bool taken);

public:
    vector<char> nextPipes;
    vector<loc> freeSources;
    vector<loc> unsuppliedHouses;

    vector<loc> allSources;
    vector<loc> allHouses;

    vector<loc> currentFieldsLeaking;

    Land(int Xsize, int Ysize);
    void markFieldMultipleType(Loc loc, FieldInfo types);
    FieldInfo getFieldInfo(Loc loc);
    bool tryPlacePipe(Loc loc, Pipe pipe);

    bool checkBoundCompatibility(LandConnection connection, LandConnection landConnection);

    void reversePipePlacing(Loc loc, FieldInfo);


    LandConnection getFieldsUp(Loc loc);
    LandConnection getFieldsDown(Loc loc);
    LandConnection getFieldsLeft(Loc loc);
    LandConnection getFieldsRight(Loc loc);

    bool waterWillFlowThroughNewPipe(Loc loc, const Pipe &pipe);

    vector<Loc> getPossibleNeighbourFields(const loc &loc);

    void reverseNearbyFields(const Loc &fieldLoc, const LandConnection &connection);
};


#endif //PROJECT_RURY_LAND_H

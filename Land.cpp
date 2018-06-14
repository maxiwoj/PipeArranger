//
// Created by Maksymilian Wojczuk on 10/06/2018.
//

#include <iostream>
#include "Land.h"

using namespace std;

bool areNeighbours(Loc loc1, Loc loc2) ;

// initialize land
Land::Land(int Xsize, int Ysize) {
    this->Ysize = Ysize;
    this->Xsize = Xsize;

// Init structures representing connections (lines) between land fields
    this->horizontalLandConnections = new LandConnection*[this->Xsize + 1];
    for(int i = 0 ; i < this->Xsize + 1 ; i++) {
        this->horizontalLandConnections[i] = new LandConnection[this->Ysize];
        for(int j = 0 ; j < this->Ysize ; j++) {
            this->horizontalLandConnections[i][j] = Empty;
        }
    }
    this->verticalLandConnections = new LandConnection*[this->Xsize];
    for(int i = 0 ; i < this->Xsize ; i++) {
        this->verticalLandConnections[i] = new LandConnection[this->Ysize + 1];
        for(int j = 0 ; j < this->Ysize + 1; j++) {
            this->verticalLandConnections[i][j] = Empty;
        }
    }


//    Mark all fields with rocks, sources and houses as taken
    this->takenFields = new bool*[this->Xsize];
    for (int i = 0 ; i < this->Xsize ; i++) {
        this->takenFields[i] = new bool[this->Ysize];
        for (int j = 0 ; j < this->Ysize ; j++) {
            this->takenFields[i][j] = false;
        }
    }
    for (int i = 0 ; i < SrcsNum ; i++) {
        this->takenFields[Srcs[i].x][Srcs[i].y] = true;
        const Loc &srcLoc = (Loc){Srcs[i].x, Srcs[i].y};
        this->markFieldSingleType(srcLoc, Extra, true);
        this->freeSources.push_back(Srcs[i]);
    }
    for (int i = 0; i < HsNum; ++i) {
        this->takenFields[Houses[i].x][Houses[i].y] = true;
        const Loc &houseLoc = (Loc){Houses[i].x, Houses[i].y};
        this->markFieldSingleType(houseLoc, Extra, true);
        this->unsuppliedHouses.push_back(Houses[i]);
        for (int src = 0 ; src < SrcsNum ; src++) {
            if (areNeighbours(Srcs[src], houseLoc)) {
                this->markFieldSingleType(Srcs[src], Blocked, true);
                this->markFieldSingleType(houseLoc, Blocked, true);
                freeSources.erase(remove(freeSources.begin(), freeSources.end(), Srcs[src]), freeSources.end());
                unsuppliedHouses.pop_back();
                break;
            }
        }
    }
    for (int i = 0; i < BlNum; ++i) {
        this->takenFields[Blocks[i].x][Blocks[i].y] = true;
        this->markFieldSingleType((Loc){Blocks[i].x, Blocks[i].y}, Blocked, true);
    }

    for (auto &&pipeType :pipeTypes) {
        this->nextPipes.push_back(pipeType);
    }

    //    MARK BORDERS AS BLOCKED
    for(int y = 0 ; y < this->Ysize ; y++) {
        this->horizontalLandConnections[0][y] = Blocked;
        this->horizontalLandConnections[this->Xsize][y] = Blocked;
    }
    for(int x = 0 ; x < this->Xsize ; x++) {
        this->verticalLandConnections[x][0] = Blocked;
        this->verticalLandConnections[x][this->Ysize] = Blocked;
    }
    this->allSources = *new vector<Loc>(freeSources);
    this->allHouses = *new vector<Loc>(unsuppliedHouses);
}

void Land::markFieldSingleType(Loc loc, LandConnection type, bool taken) {
    auto fieldInfo = new FieldInfo;
    fieldInfo->taken = taken;
    fieldInfo->down = fieldInfo->left = fieldInfo->right = fieldInfo->up = type;
    this->markFieldMultipleType(loc, *fieldInfo);
}

void Land::markFieldMultipleType(Loc loc, FieldInfo types) {
    this->takenFields[loc.x][loc.y] = types.taken;
    this->verticalLandConnections[loc.x][loc.y] = loc.y != 0 ? types.up : Blocked;
    this->verticalLandConnections[loc.x][loc.y + 1] = loc.y + 1 != Ysize ? types.down : Blocked;
    this->horizontalLandConnections[loc.x][loc.y] = loc.x != 0 ? types.left : Blocked;
    this->horizontalLandConnections[loc.x + 1][loc.y] = loc.x + 1 != Xsize ? types.right : Blocked;
}

bool Land::tryPlacePipe(Loc loc, Pipe pipe) {
    if(takenFields[loc.x][loc.y]) return false;

//    Check bounds compatibility
    if(checkBoundCompatibility(this->getFieldsUp(loc), pipe.up) &&
            checkBoundCompatibility(this->getFieldsDown(loc), pipe.down) &&
            checkBoundCompatibility(this->getFieldsLeft(loc), pipe.left) &&
            checkBoundCompatibility(this->getFieldsRight(loc), pipe.right) &&
            waterWillFlowThroughNewPipe(loc, pipe)) {
        if (this->getFieldsUp(loc) == Extra && pipe.up == Full) {
            const Loc &locUp = (Loc) {loc.x, loc.y - 1};
            this->markFieldSingleType(locUp, Blocked, true);
            freeSources.erase(remove(this->freeSources.begin(), this->freeSources.end(), locUp), freeSources.end());
            unsuppliedHouses.erase(remove(this->unsuppliedHouses.begin(), this->unsuppliedHouses.end(), locUp), unsuppliedHouses.end());
            pipe.up = Extra;
        }
        if (this->getFieldsDown(loc) == Extra && pipe.down == Full) {
            const Loc &locDown = (Loc) {loc.x, loc.y + 1};
            this->markFieldSingleType(locDown, Blocked, true);
            freeSources.erase(remove(this->freeSources.begin(), this->freeSources.end(), locDown), freeSources.end());
            unsuppliedHouses.erase(remove(this->unsuppliedHouses.begin(), this->unsuppliedHouses.end(), locDown), unsuppliedHouses.end());
            pipe.down = Extra;
        }
        if (this->getFieldsLeft(loc) == Extra && pipe.left == Full) {
            const Loc &locLeft = (Loc) {loc.x - 1, loc.y};
            this->markFieldSingleType(locLeft, Blocked, true);
            freeSources.erase(remove(this->freeSources.begin(), this->freeSources.end(), locLeft), freeSources.end());
            unsuppliedHouses.erase(remove(this->unsuppliedHouses.begin(), this->unsuppliedHouses.end(), locLeft), unsuppliedHouses.end());
            pipe.left = Extra;
        }
        if(this->getFieldsRight(loc) == Extra && pipe.right == Full) {
            const Loc &locRight = (Loc) {loc.x + 1, loc.y};
            this->markFieldSingleType(locRight, Blocked, true);
            freeSources.erase(remove(this->freeSources.begin(), this->freeSources.end(), locRight), freeSources.end());
            unsuppliedHouses.erase(remove(this->unsuppliedHouses.begin(), this->unsuppliedHouses.end(), locRight), unsuppliedHouses.end());
            pipe.right = Extra;
        }

        auto newField = new FieldInfo;
        newField->taken = true;
        newField->right = pipe.right;
        newField->left = pipe.left;
        newField->up = pipe.up;
        newField->down = pipe.down;
        this->markFieldMultipleType(loc, *newField);
        this->currentFieldsLeaking.push_back(loc);

        return true;
    }
    return false;
}

LandConnection Land::getFieldsUp(Loc loc) {
    return this->verticalLandConnections[loc.x][loc.y];
}
LandConnection Land::getFieldsDown(Loc loc) {
    return this->verticalLandConnections[loc.x][loc.y + 1];
}
LandConnection Land::getFieldsLeft(Loc loc) {
    return this->horizontalLandConnections[loc.x][loc.y];
}
LandConnection Land::getFieldsRight(Loc loc) {
    return this->horizontalLandConnections[loc.x + 1][loc.y];
}

void Land::reversePipePlacing(Loc loc, FieldInfo field) {
    this->takenFields[loc.x][loc.y] = false;

    const Loc &locUp = (Loc) {loc.x, loc.y - 1};
    reverseNearbyFields(locUp, field.up);

    const Loc &locDown = (Loc) {loc.x, loc.y + 1};
    reverseNearbyFields(locDown, field.down);

    const Loc &locLeft = (Loc) {loc.x - 1, loc.y};
    reverseNearbyFields(locLeft, field.left);

    const Loc &locRight = (Loc) {loc.x + 1, loc.y};
    reverseNearbyFields(locRight, field.right);

    this->markFieldMultipleType(loc, field);
    this->currentFieldsLeaking.erase(remove(this->currentFieldsLeaking.begin(), this->currentFieldsLeaking.end(), loc), this->currentFieldsLeaking.end());
}

void Land::reverseNearbyFields(const Loc &fieldLoc, const LandConnection &connection) {
    if (connection == Extra) {
            //TODO: If not blocked from other blocks EDIT: This should not mess up since it would be taken... EDIT: Not if this was the end of the Land
            markFieldSingleType(fieldLoc, Extra, true);
            if(find(allHouses.begin(), allHouses.end(), fieldLoc) != allHouses.end()){
                unsuppliedHouses.push_back(fieldLoc);
            } else if(find(allSources.begin(), allSources.end(), fieldLoc) != allSources.end()) {
                freeSources.push_back(fieldLoc);
            }
        } else if (connection == Full) {
            currentFieldsLeaking.push_back(fieldLoc);
        }
}

FieldInfo Land::getFieldInfo(Loc loc) {
    auto fieldInfo = new FieldInfo;
    fieldInfo->right = this->horizontalLandConnections[loc.x + 1][loc.y];
    fieldInfo->left = this->horizontalLandConnections[loc.x][loc.y];
    fieldInfo->up = this->verticalLandConnections[loc.x][loc.y];
    fieldInfo->down = this->verticalLandConnections[loc.x][loc.y + 1];
    fieldInfo->taken = this->takenFields[loc.x][loc.y];
    return *fieldInfo;
}

bool Land::checkBoundCompatibility(LandConnection fieldConnection, LandConnection pipeConnection) {
    return fieldConnection == Empty || fieldConnection == pipeConnection || fieldConnection == Extra;
}

vector<Loc> Land::getPossibleNeighbourFields(const loc &loc) {
    auto *fields = new vector<Loc>();
    auto field = this->getFieldInfo(loc);
    if ((field.up == Extra || field.up == Full) && !this->takenFields[loc.x][loc.y - 1]){
        fields->push_back((Loc) {loc.x, loc.y - 1});
    }
    if ((field.down == Extra || field.down == Full) && !this->takenFields[loc.x][loc.y + 1]){
        fields->push_back((Loc) {loc.x, loc.y + 1});
    }
    if ((field.left == Extra || field.left == Full) && !this->takenFields[loc.x - 1][loc.y]){
        fields->push_back((Loc) {loc.x - 1, loc.y});
    }
    if ((field.right == Extra || field.right == Full) && !this->takenFields[loc.x + 1][loc.y]){
        fields->push_back((Loc) {loc.x + 1, loc.y});
    }
    if(fields->empty()){
        freeSources.erase(remove(freeSources.begin(), freeSources.end(), loc), freeSources.end());
        currentFieldsLeaking.erase(remove(currentFieldsLeaking.begin(), currentFieldsLeaking.end(), loc), currentFieldsLeaking.end());
    }
    return *fields;
}

bool Land::waterWillFlowThroughNewPipe(Loc loc, const Pipe &pipe) {
    return (pipe.up == Full && this->getFieldsUp(loc) != Empty) || (pipe.down == Full && this->getFieldsDown(loc) != Empty)
            || (pipe.right == Full && this->getFieldsRight(loc) != Empty) || (pipe.left == Full && this->getFieldsLeft(loc) != Empty);
}

void Land::printLand() {
    map<LandConnection, char> printMap = {
            make_pair(Empty, ' '),
            make_pair(Full, 'o'),
            make_pair(Extra, 'e'),
            make_pair(Blocked, 'x')
    };
    map<bool, char> printMapB = {
            make_pair(true, 'T'),
            make_pair(false, 'F')
    };

    for (int i = 0 ; i < Xsize ; i++){
        cout << "  " << printMap.at(this->getFieldsUp((Loc){i, 0})) << " ";
    }
    cout<<endl;
    for (int y = 0 ; y < Ysize ; y++){
        for (int x = 0 ; x < Xsize ; x++){
            cout << printMap.at(this->getFieldsLeft((Loc){x, y})) << " " << printMapB.at(this->takenFields[x][y]) << " ";
        }
        cout << printMap.at(this->getFieldsLeft((Loc){Xsize, y})) << endl;
        for (int x = 0 ; x < Xsize ; x++){
            cout << "  " << printMap.at(this->getFieldsDown((Loc){x, y})) << " ";
        }
        cout<<endl;
    }
    cout<<endl<<endl;
    for(int i = 0 ; i < 50 ; i ++) {
        cout<<"-";
    }
    cout<<endl<<endl;
}

bool areNeighbours(Loc loc1, Loc loc2) {
    return manhattanDistance(loc1, loc2) < 2;
}
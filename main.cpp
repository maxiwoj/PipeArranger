#include <iostream>
#include <fstream>
#include "Land.h"
#include <queue>

using namespace std;

void readInput(ifstream &inputFile);

Result findSolution(Land pLand);


vector<Loc> concatenate(const std::vector<Loc>& lhs, const std::vector<Loc>& rhs)
;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "You need to specify input!" << endl;
        return 1;
    }
    ifstream inputFile (argv[1]);
    readInput(inputFile);

    cout << "wymiary: " << Xsize << " " << Ysize << endl;
    cout << "liczba źródeł: " << SrcsNum << endl;
    cout << "liczba domow: " << HsNum << endl;
    cout << "liczba blokow: " << BlNum << endl;
    cout << "block types: " << pipeTypes << endl;

    auto land = new Land(Xsize, Ysize);

    Result solution = findSolution(*land);
    if(!solution.success) {
        cout << "Could not find solution!" << endl;
    }


    return 0;
}

Result findSolution(Land pLand) {
//    End conditions
    char nextPipe = pLand.nextPipes.front();
    pLand.nextPipes.erase(pLand.nextPipes.begin());
    auto possiblePipes = pipes.at(nextPipe);
    for (auto &&fieldLeaking : concatenate(pLand.currentFieldsLeaking, pLand.freeSources)) {
        vector<Loc> possiblefields = pLand.getPossibleNeighbourFields(fieldLeaking);
        for (auto possiblefieldLoc: possiblefields) {
            auto currentFieldSnapshot = pLand.getFieldInfo(possiblefieldLoc);
            for (auto &&possiblePipe : possiblePipes) {
                if(pLand.tryPlacePipe(possiblefieldLoc, possiblePipe)){
                    Result result = findSolution(pLand);
                    if(result.success) {
                        result.result.push_back((PipePlacement) {possiblefieldLoc, possiblePipe.name}); // TODO: Reverse in the end
                        return result;
                    } else {
                        pLand.reversePipePlacing(possiblefieldLoc, currentFieldSnapshot);
                    }
                }
            }
        }
    }
    Result result1 = Result();
    result1.success = false;
    return result1;

}

void readInput(ifstream &inputFile) {
    if(inputFile.is_open()) {
        inputFile >> Xsize;
        inputFile >> Ysize;
        inputFile >> SrcsNum;
        Srcs = new Loc[SrcsNum];
        for (int i = 0 ; i < SrcsNum ; i++) {
            inputFile >> Srcs[i].x >> Srcs[i].y;
        }
        inputFile >> HsNum;
        Houses = new Loc[HsNum];
        for (int i = 0 ; i < HsNum ; i++) {
            inputFile >> Houses[i].x >> Houses[i].y;
        }
        inputFile >> BlNum;
        Blocks = new Loc[BlNum];
        for (int i = 0 ; i < BlNum ; i++) {
            inputFile >> Blocks[i].x >> Blocks[i].y;
        }
        inputFile >> pipeTypes;
        inputFile.close();
    } else {
        cout << "Error opening file";
    }
}

// TODO: Go for checking for 2 vectors
std::vector<Loc> concatenate(const std::vector<Loc>& lhs, const std::vector<Loc>& rhs)
{
    auto result = lhs;
    std::copy( rhs.begin(), rhs.end(), std::back_inserter(result) );
    return result;
}
#include <iostream>
#include <queue>

#ifndef NDEBUG
#include <fstream>
#endif

#include <list>
#include "Land.h"
#include "defs.h"

using namespace std;

#ifndef NDEBUG
void readInputFromFile(ifstream &inputFile);
#endif

void readInput();

Result findSolution(Land &pLand);

vector<Loc> concatenate(const std::vector<Loc>& lhs, const std::vector<Loc>& rhs);


int Xsize, Ysize;
int SrcsNum;
loc *Srcs;
int HsNum;
loc *Houses;
int BlNum;
loc *Blocks;
string pipeTypes;

#ifndef NDEBUG
    list<PipePlacement> currPipes;
    string inPath;
    void printCurrentPipes() {
        ofstream outputFile("./judge/out");
        outputFile << currPipes.size() << endl;
        for (auto &pipePlacement: currPipes){
            outputFile << pipePlacement.localisation.x << " " << pipePlacement.localisation.y << " " << pipePlacement.landMark << endl;
        }
        cout << "Succesfully printed " << currPipes.size() << " pipes to file"<<endl;
    }
#endif

Pipe I1, I2, L1, L2, L3, L4, T1, T2, T3, T4, X;
map<char, vector<Pipe>> pipes;

int main(int argc, char* argv[]) {
    initializeStaticStructures();
#ifndef NDEBUG
    if (argc < 2) {
        cout << "You need to specify input!" << endl;
        return 1;
    }
    initializeStaticStructures();
    ifstream inputFile (argv[1]);
    readInputFromFile(inputFile);
#endif
#ifdef NDEBUG
    readInput();
#endif


#ifndef NDEBUG
    cout << "wymiary: " << Xsize << " " << Ysize << endl;
    cout << "liczba źródeł: " << SrcsNum << endl;
    cout << "liczba domow: " << HsNum << endl;
    cout << "liczba blokow: " << BlNum << endl;
    cout << "block types: " << pipeTypes << endl;

    currPipes = *new list<PipePlacement>();
    inPath = argv[1];
#endif

    Land land = Land(Xsize, Ysize);

    Result solution = findSolution(land);
    if(!solution.success) {
        cout << "Could not find solution!" << endl;
    } else {
        reverse(solution.pipesPlacements.begin(), solution.pipesPlacements.end());
        cout<<solution.pipesPlacements.size()<<endl;
        for (auto &pipePlacement: solution.pipesPlacements){
            cout << pipePlacement.localisation.x << " " << pipePlacement.localisation.y << " " << pipePlacement.landMark << endl;
        }
    }

    return 0;
}

Result findSolution(Land pLand) {
//    End conditions
    Result result1 = Result();

    const vector<Loc> possiblePlacementFields = concatenate(pLand.currentFieldsLeaking, pLand.freeSources);
    for(auto &field: possiblePlacementFields){
        pLand.getPossibleNeighbourFields(field);
    }
    if (pLand.unsuppliedHouses.empty() && pLand.currentFieldsLeaking.empty()){
        result1.success = true;
        return result1;
    }
    if (pLand.nextPipes.empty()){
        result1.success = false;
        return result1;
    }

    char nextPipe = pLand.nextPipes.front();
    pLand.nextPipes.erase(pLand.nextPipes.begin());
    auto possiblePipes = pipes.at(nextPipe);
    for (auto &&fieldLeaking : possiblePlacementFields) {
        vector<Loc> possiblefields = pLand.getPossibleNeighbourFields(fieldLeaking);
        for (auto possiblefieldLoc: possiblefields) {
            auto currentFieldSnapshot = pLand.getFieldInfo(possiblefieldLoc);
            for (auto &&possiblePipe : possiblePipes) {
                if(pLand.tryPlacePipe(possiblefieldLoc, possiblePipe)){

#ifndef NDEBUG
                    currPipes.push_back((PipePlacement){possiblefieldLoc, possiblePipe.name});
#endif

                    Result result = findSolution(pLand);
                    if(result.success) {
                        result.pipesPlacements.push_back((PipePlacement) {possiblefieldLoc, possiblePipe.name});
                        return result;
                    } else {
                        pLand.reversePipePlacing(possiblefieldLoc, currentFieldSnapshot);
#ifndef NDEBUG
                        currPipes.pop_back();
#endif
                    }
                }
            }
        }
    }
    pLand.nextPipes.insert(pLand.nextPipes.begin(), nextPipe);
    result1.success = false;
    return result1;

}

#ifndef NDEBUG
void readInputFromFile(ifstream &inputFile) {
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
#endif

void readInput() {
    cin >> Xsize;
    cin >> Ysize;
    cin >> SrcsNum;
    Srcs = new Loc[SrcsNum];
    for (int i = 0 ; i < SrcsNum ; i++) {
        cin >> Srcs[i].x >> Srcs[i].y;
    }
    cin >> HsNum;
    Houses = new Loc[HsNum];
    for (int i = 0 ; i < HsNum ; i++) {
        cin >> Houses[i].x >> Houses[i].y;
    }
    cin >> BlNum;
    Blocks = new Loc[BlNum];
    for (int i = 0 ; i < BlNum ; i++) {
        cin >> Blocks[i].x >> Blocks[i].y;
    }
    cin >> pipeTypes;

}

// TODO: Go for checking for 2 vectors
std::vector<Loc> concatenate(const vector<Loc>& lhs, const vector<Loc>& rhs)
{
    auto result = lhs;
    std::copy( rhs.begin(), rhs.end(), back_inserter(result) );
    return result;
}
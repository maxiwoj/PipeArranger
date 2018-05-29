#include <iostream>
#include <fstream>
#include "defs.h"
#include <queue>

using namespace std;

void readInput(ifstream &inputFile);

void initLand();

vector<loc> freeSources;
vector<loc> unsuppliedHouses;
vector<loc> currentFieldsPossible;
vector<char> nextPipes;


int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "You need to specify input!" << endl;
        return 1;
    }
    ifstream inputFile (argv[1]);
    readInput(inputFile);
    initLand();

    cout << "wymiary: " << Xsize << " " << Ysize << endl;
    cout << "liczba źródeł: " << SrcsNum << endl;
    cout << "liczba domow: " << HsNum << endl;
    cout << "liczba blokow: " << BlNum << endl;
    cout << "bkock types: " << pipeTypes << endl;

    return 0;
}

void initLand() {
    land = new landMark*[Xsize];
    for (int i = 0 ; i < Xsize ; i++) {
        land[i] = new landMark[Ysize];
        for (int j = 0 ; j < Ysize ; j++) {
            land[i][j] = Empty;
        }
    }
    for (int i = 0 ; i < SrcsNum ; i++) {
        land[Srcs[i].x][Srcs[i].y] = Source;
        freeSources.push_back(Srcs[i]);
        currentFieldsPossible.push_back(Srcs[i]);
    }
    for (int i = 0; i < HsNum; ++i) {
        land[Houses[i].x][Houses[i].y] = House;
        unsuppliedHouses.push_back(Houses[i]);
    }
    for (int i = 0; i < BlNum; ++i) {
        land[Blocks[i].x][Blocks[i].y] = Rock;
    }

    for (auto &&pipeType :pipeTypes) {
        nextPipes.push_back(pipeType);
    }
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

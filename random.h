/*** Created By killfat @2020.3***/
//#pragma once
#include <random>
#include <vector>
#include <chrono>
#include "game.h"
#include <algorithm>
#include <memory>

int randToIndex(int rand, std::vector<int>& vec);
int getSuitLength(int suit, int hand[]);
int getHCP(int hand[]);
class Deck {
private:
    int cards[52];
    const int left;
    int ind;
public:
    Deck();
    int extractCard(int card);
    int extractSuit(int suit);
    int dealCard();
    int getLeft();
    int reset();
};

class SideConstraint {
public:
    //int getSide();
    virtual bool check(int hand[][13]) = 0;
};

class SuitConstraint : public SideConstraint {
public:
    int side;
    int suit;
    int min;
    int max;
    bool check(int hand[][13]);
    int getSide();
    SuitConstraint(int sd, int st, int mi, int ma);
};

class HcpConstraint : public SideConstraint {
public:
    int side;
    int min;
    int max;
    bool check(int hand[][13]);
    int getSide();
    HcpConstraint(int sd, int mi, int ma);
};

class CompareConstraint : public SideConstraint {
public:
    int side1;
    int side2;
    int suit1;
    int suit2;
    int opt;  //1 gt, 0 eq, -1 lt
    int diff;
    bool check(int hand[][13]);
    int getSide();
    CompareConstraint(int s1, int s2, int st1, int st2, int op, int dif);
};

class AndConstraints : public SideConstraint {
public:
    std::vector<SideConstraint*> constraints;
    bool check(int hand[][13]);
    void appendConstraints(SideConstraint *s);
};

class OrConstraints : public SideConstraint {
public:
    std::vector<SideConstraint*> constraints;
    bool check(int hand[][13]);
    void appendConstraints(SideConstraint *s);
};

class NotConstraint : public SideConstraint {
public:
    SideConstraint* cons;
    bool check(int hand[][13]);
    NotConstraint(SideConstraint *con);
};

class AddConstraint : public SideConstraint{
public:
    int side1;
    int side2;
    int opt; //1 add lt, 0 eq
    int res;
    bool check(int hand[][13]);
    int getSide();
    AddConstraint(int s1, int s2, int op, int r);
};

class RandomGenerator {
private:
    std::default_random_engine *gen;

public:
    void getSeed();
    unsigned rand(int range);
    RandomGenerator() : gen(NULL) {}
    ~RandomGenerator();
};

class DealMaster {
private:
    int vacant[4];
    int tempCard[4][13];
    Deck dk;

    RandomGenerator ran;
    std::vector<SideConstraint*> constraints;
    std::vector<std::pair<int, int>> fixCards;
    std::vector<int> suitDistrib;
public:

    DealMaster();
    
    void genBoard(int num, std::vector<Board> &bd);
    void sort();
    //void AttachConstraint(int hand[][], Deal);
    void dealFix();
    void dealSuit(int suit);
    void dealRest();
    void dealReset();
    bool filterHand();
    int indexToBit(Board &bd);

    void addFix(int side, std::vector<int> card);
    void addSuit(int suit, std::vector<int> distrib);
    void addConstraint(SideConstraint *con);
    void watch(); //Debug Use
};

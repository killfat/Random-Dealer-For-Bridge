/*** Created By killfat @2020.3***/
#pragma once
#include <string>
#include <fstream>
#include "game.h"
#include <sstream>

static char vulRd[4][5] = { "None", "NS", "EW", "All" };
static char dealerRd[5] = "NESW";

class OutputInterface {

public:
    virtual int open(const char* filename) = 0;
    virtual int write(const std::string &text) = 0;
    virtual std::string writeBoard(Board &bd) = 0;
    virtual void close() = 0;
};

class PBNOutput : public OutputInterface {
private:
    std::fstream fs;
public:
    int open(const char* filename);
    int write(const std::string &text);
    std::string writeBoard(Board &bd);
    std::string writeDDTable(int tab[5][4]);
    void close();
};

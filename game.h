/*** Created By killfat @2020.3***/
#pragma once
#include <vector>
#include <string>

#define NIL -1

//class Game {
//    Dealer d;
//    std::vector<std::vector<Card>> Deck;
//    
//public:
//    Game() { Deck.resize(4); }
//    void deal() {
//	d.shuffle();
//	for (int i = 0; i < 52; i++) {
//	    Deck[i % 4].push_back(d.get()[i]);
//	}
//	for (int i = 0; i < 4; i++) {
//	    sort(Deck[i].begin(), Deck[i].end(), isgreater<int, int>);
//	}
//    }
//
//    void printable(const Card &c) {
//	int value = (c & 0x0F) + 2;
//	int suit = c >> 4;
//	switch (value) {
//	case 11:
//	    printf("J");
//	    break;
//	case 12:
//	    printf("Q");
//	    break;
//	case 13:
//	    printf("K");
//	    break;
//	case 14:
//	    printf("A");
//	    break;
//	default:
//	    printf("%d", value);
//	}
//	
//    }
//    void printHands() {
//	printf("         ");
//	for (Card &i : Deck[0]) {
//	    printable(i);
//	}
//	printf("\n");
//	for (Card &i : Deck[3]) {
//	    printable(i);
//	}
//	printf("     ");
//	for (Card &i : Deck[1]) {
//	    printable(i);
//	}
//	printf("\n         ");
//	for (Card &i : Deck[2]) {
//	    printable(i);
//	}
//
//    }
//};

static char cardRd[14] = "AKQJT98765432";
static int vulConstant[16] = { 0,1,2,3,1,2,3,0,2,3,0,1,1,0,2,3 };
class Board {
public:
    int no;
    int dealer;
    int vul;
    int hand[4][4] = {};
    //std::vector<int> handIndex[4];
    std::string name[4];
    std::string bidding;
    std::string comment;

    Board();
    std::string handToRd(const int direction);
};
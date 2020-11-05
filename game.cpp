/*** Created By killfat @2020.3***/
#include "game.h"

Board::Board()
{
    no = -1;
    dealer = -1;
    vul = -1;
}

std::string Board::handToRd(const int direction)
{
    std::string text;
    
    for (int s = 0; s < 4; s++) {
	for (int i = 0, mask = 0x4000; mask >= 4; i++, mask >>= 1) {
	    if (0 != (hand[direction][s] & mask))
		text.push_back(cardRd[i]);
	}
	text.push_back('.');
    }
    text.pop_back();
    return text;
}

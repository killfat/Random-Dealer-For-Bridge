/*** Created By killfat @2020.3***/
#include "output.h"

int PBNOutput::open(const char* filename)
{
    fs.open(filename, std::ios_base::out);

    return fs.fail();
}

int PBNOutput::write(const std::string &text)
{
    if (fs.is_open()) {
	fs << text;
	return 0;
    }
    else return 1;
}

std::string PBNOutput::writeBoard(Board & bd)
{
    std::stringstream s;
    if(bd.no != NIL)
	s << "[Board \"" << bd.no << "\"]\n";
    if(bd.dealer != NIL)
	s << "[Dealer \"" << dealerRd[bd.dealer] << "\"]\n";
    if (bd.vul != NIL)
	s << "[Vulnerable \"" << vulRd[bd.vul] << "\"]\n";

    s << "[Deal \"N:" << bd.handToRd(0)
	<< " " << bd.handToRd(1)
	<< " " << bd.handToRd(2)
	<< " " << bd.handToRd(3) << "\"]\n";
    
    //bidding not included
    return s.str();
}

std::string PBNOutput::writeDDTable(int tab[5][4])
{
    std::stringstream s;
    s << "[OptimumResultTable \"S;H;D;C;NT\"]\n";
    //s << "\"";
    for (int h = 0; h < 4; h++) {
	for (int d = 0; d < 5; d++)
	    s << tab[d][h] << " ";
	s << "\n";
    } 
    s << "\n";
    return s.str();
}

void PBNOutput::close()
{
    fs.close();
}

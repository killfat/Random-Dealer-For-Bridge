/*** Created By killfat @2020.3***/
#include "random.h"

Deck::Deck(): left(52)
{
    ind = 0;
    for (int i = 0; i < left; i++) {
	cards[i] = i;
    }
}

int Deck::extractCard(int card)
{
    int min = ind, max = left - 1, mid;
    int i = -1;
    while (min <= max) {
	mid = (min + max) / 2;
	if (card < cards[mid]) {
	    max = mid - 1;
	}
	else if (card > cards[mid]) {
	    min = mid + 1;
	}
	else {
	    i = mid;
	    break;
	}
    }

    if (i >= ind) {
	std::swap(cards[ind], cards[i]);
	ind++;
	return 0;
    }
    else return 10;
}

int Deck::extractSuit(int suit) //swap the suit to front
{
    int num = ind;
    for (int i = ind; i < left; i++) {
	if (cards[i] / 13 == suit) {
	    std::swap(cards[num], cards[i]);
	    num++;
	}
    }
    return num - ind;
}

int Deck::dealCard()
{
    return cards[ind++];
}

int Deck::getLeft()
{
    return left - ind;
}

int Deck::reset()
{
    for (int i = 0; i < left; i++) {
	cards[i] = i;
    }
    ind = 0;
    return 0;
}

DealMaster::DealMaster()
{
    for (int i = 0; i < 4; i++) {
	vacant[i] = 13;
    }
    ran.getSeed();
}

void DealMaster::genBoard(int num, std::vector<Board> &bd)
{
    int i = 0;
    while(i < num) {
	Board b;
	dealReset();
	dealFix();
	if(!suitDistrib.empty())
	    dealSuit(suitDistrib.back());
	dealRest();
	if (filterHand()) {
	    i++;
	    indexToBit(b);
	    b.no = i;
	    bd.push_back(b);
	}
    }
}

void DealMaster::sort()  //order cards big to small
{
    for (int i = 0; i < 4; i++) {
	std::sort(&tempCard[i][0], &tempCard[i][12]);
    }
}

void DealMaster::dealFix()
{
    int card, side;
    for (int i = 0; i < fixCards.size(); i++) {
	side = fixCards.at(i).first;
	card = fixCards.at(i).second;
	tempCard[side][13 - vacant[side]] = card;
	dk.extractCard(card);
	vacant[side]--;
    }
}

void DealMaster::dealSuit(int suit)
{
    int num = dk.extractSuit(suit);
    int r, place;
    std::vector<int> curDistrib = suitDistrib;
    while(num > 0) {
	r = ran.rand(num);
	place = randToIndex(r, curDistrib);
	tempCard[place][13 - vacant[place]] = dk.dealCard();
	vacant[place]--;
	curDistrib.at(place)--;
	num--;
    }
}

void DealMaster::dealRest()
{
    int r, place;
    while(dk.getLeft() > 0) {
	r = ran.rand(dk.getLeft());
	place = randToIndex(r, std::vector<int>(std::begin(vacant), std::end(vacant)));
	tempCard[place][13 - vacant[place]] = dk.dealCard();
	vacant[place]--;
    }
}

void DealMaster::dealReset()
{
    for (int i = 0; i < 4; i++)
	vacant[i] = 13;
    dk.reset();
}

bool DealMaster::filterHand()
{
    bool res = true;
    for (auto &c : constraints) {
	if (!c->check(tempCard)) {
	    res = false;
	    break;
	}
    }
    return res;
}

int DealMaster::indexToBit(Board &bd)  //must process on initialized array
{
    int card;
    for (int i = 0; i < 4; i++) {
	for (int c = 0; c < 13; c++) {
	    card = tempCard[i][c];
	    bd.hand[i][card / 13] |= 1 << ((12- card%13) + 2);
	}
    }
    return 1;
}

void DealMaster::addFix(int side, std::vector<int> card)
{
    for (int i = 0; i < card.size(); i++) {
	fixCards.push_back(std::pair<int, int>(side, card.at(i)));
    }
}

void DealMaster::addSuit(int suit, std::vector<int> distrib)
{
    suitDistrib = distrib;
    suitDistrib.push_back(suit);
}

void DealMaster::addConstraint(SideConstraint *con)
{
    constraints.push_back(con);
}

void DealMaster::watch()
{
    for (int j = 0; j < 13; j++) {
	printf("%d", tempCard[0][j]);
    }
}

int randToIndex(int rand, std::vector<int>& vec) {
    int index = -1;
    int sum = 0;
    for (int i = 0; i < vec.size(); i++) {
	sum += vec.at(i);
	if (rand < sum) {
	    index = i;
	    break;
	}
    }
    return index;
}

int getSuitLength(int suit, int hand[])
{
    int low = 13 * suit;
    int high = 13 * (suit + 1) - 1;
    int num = 0;
    for (int i = 0; i < 13; i++) {
	if (hand[i] >= low && hand[i] <= high)
	    num += 1;
    }
    return num;
}

int getHCP(int hand[])
{
    int hcp = 0;
    for (int i = 0; i < 13; i++) {
	int res = hand[i] % 13;
	switch (res) {
	case 0:
	    hcp += 4;
	    break;
	case 1:
	    hcp += 3;
	    break;
	case 2:
	    hcp += 2;
	    break;
	case 3:
	    hcp += 1;
	    break;
	}
    }
    return hcp;
}

void RandomGenerator::getSeed()
{
    if (gen != NULL) {
	delete gen;
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    gen = new std::default_random_engine(seed);
}

unsigned RandomGenerator::rand(int range)
{
    return (*gen)() % range;
}

RandomGenerator::~RandomGenerator()
{
    if (gen != NULL) {
	delete gen;
    }
}

bool HcpConstraint::check(int hand[][13])
{
    int hcp = getHCP(hand[side]);
    if (hcp >= min && hcp <= max)
	return true;
    else return false;
}

int HcpConstraint::getSide()
{
    return side;
}

HcpConstraint::HcpConstraint(int sd, int mi, int ma)
{
    side = sd;
    min = mi;
    max = ma;
}

bool SuitConstraint::check(int hand[][13])
{
    int num = getSuitLength(suit, hand[side]);
    if (num >= min && num <= max)
	return true;
    else return false;
}

int SuitConstraint::getSide()
{
    return side;
}

SuitConstraint::SuitConstraint(int sd, int st, int mi, int ma)
{
    side = sd;
    suit = st;
    min = mi;
    max = ma;
}

bool CompareConstraint::check(int hand[][13])
{
    int num1 = getSuitLength(suit1,hand[side1]);
    int num2 = getSuitLength(suit2,hand[side2]);
    if (opt == 0)
	return num1 == num2 + diff;
    else if (opt == 1)
	return num1 > num2 + diff;
    else
	return num1 < num2 + diff;
}

int CompareConstraint::getSide()
{
    return 0;
}

CompareConstraint::CompareConstraint(int s1, int s2, int st1, int st2, int op, int dif)
{
    side1 = s1;
    side2 = s2;
    suit1 = st1;
    suit2 = st2;
    opt = op;
    diff = dif;
}

bool AddConstraint::check(int hand[][13])
{
    if (opt == 0)
	return res == getHCP(hand[side1]) + getHCP(hand[side2]);
    if (opt == 1)
	return res < getHCP(hand[side1]) + getHCP(hand[side2]);
}

int AddConstraint::getSide()
{
    return 0;
}

AddConstraint::AddConstraint(int s1, int s2, int op, int r)
{
    side1 = s1;
    side2 = s2;
    opt = op;
    res = r;

}

bool OrConstraints::check(int hand[][13])
{
    bool res = false;
    for (auto&c : constraints) {
	if (c->check(hand))
	    res = true;
    }
    return res;
}

void OrConstraints::appendConstraints(SideConstraint * s)
{
    constraints.push_back(s);
}

bool AndConstraints::check(int hand[][13])
{
    bool res = true;
    for (auto&c : constraints) {
	if (!c->check(hand))
	    res = false;
    }
    return res;
}

void AndConstraints::appendConstraints(SideConstraint * s)
{
    constraints.push_back(s);
}

NotConstraint::NotConstraint(SideConstraint *con) :cons{ con } {
}

bool NotConstraint::check(int hand[][13])
{
    return !cons->check(hand);
}

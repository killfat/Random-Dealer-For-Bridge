/*** Created By killfat @2020.3***/
/*** Special Thanks to Bo Haglund***/
#include "output.h"
#include "random.h"
#include "dll.h"
#include <windows.h>
#include <array>
#include <bitset>
//extern "C" __declspec(dllimport) int __stdcall CalcAllTables(
//    struct ddTableDeals *dealsp,
//    int mode,
//    int trumpFilter[5],
//    struct ddTablesRes *resp,
//    struct allParResults *presp
//);
//
//extern "C" __declspec(dllimport) void __stdcall ErrorMessage(
//    int code,
//    char line[80]
//);

void test1() {
    Board b1;
    b1.dealer = 1;
    b1.no = 1;
    b1.hand[0][0] = 4;
    b1.hand[0][1] = 8;
    PBNOutput pbn;
    pbn.open("test.pbn");
    pbn.write(pbn.writeBoard(b1));
    pbn.close();
}

void test2() {
    DealMaster dm;

    int count6S = 0;
    int count6C = 0;
    int both = 0;
    int batch = 0;
    int n = 0;
    int lp;
    printf("Input batch size and loops:");
    scanf("%d %d", &batch, &lp);
    printf("Input declarer and suit and contract");

    HcpConstraint Ncon = HcpConstraint(0, 17, 19);
    SuitConstraint Ncon1 = SuitConstraint(0, 0, 2, 4);
    SuitConstraint Ncon2 = SuitConstraint(0, 1, 2, 4);
    SuitConstraint Ncon3 = SuitConstraint(0, 2, 2, 5);
    SuitConstraint Ncon4 = SuitConstraint(0, 3, 2, 5);
    HcpConstraint Scon = HcpConstraint(2, 4, 4);

    SuitConstraint Scon1 = SuitConstraint(2, 0, 5, 5);
    SuitConstraint Scon2 = SuitConstraint(2, 1, 3, 3);
    SuitConstraint Scon3 = SuitConstraint(2, 2, 3, 3);
    SuitConstraint Scon4 = SuitConstraint(2, 3, 2, 2);
    
    //CompareConstraint con3 = CompareConstraint(1, 1, 0, 1, -1, 0);
    //CompareConstraint con4 = CompareConstraint(1, 1, 1, 2, 1, -1);
    //CompareConstraint con5 = CompareConstraint(1, 1, 1, 3, 1, -1);

    dm.addConstraint(&Ncon);
    dm.addConstraint(&Ncon1);
    dm.addConstraint(&Ncon2);
    dm.addConstraint(&Ncon3);
    dm.addConstraint(&Ncon4);
    dm.addConstraint(&Scon);
    dm.addConstraint(&Scon1);
    dm.addConstraint(&Scon2);
    dm.addConstraint(&Scon3);
    dm.addConstraint(&Scon4);

    //dm.addFix(0, std::vector<int>({ 1, 5, 8, 10, 13, 19, 24, 28, 31, 41, 46, 48, 50 }));
    dm.addFix(2, std::vector<int>({ 4, 5, 10, 11, 12, 17, 24, 25, 30, 31, 38, 39, 51 }));
    //dm.addFix(0, std::vector<int>({ 7, 10, 14, 16, 23, 25, 27, 28, 36, 37, 38, 40, 44 }));

    //dm.addSuit(1, std::vector<int>({ 7,1,3 }));
    std::vector<Board> bd;
    std::vector<int> res;
    res.push_back(0);
    res.push_back(0);
    for (n = 0; n < lp; n++) {
	bd.clear();
	dm.genBoard(batch, bd);

	struct ddTablesRes ddRes;
	struct ddTableDeals ddDeals;
	ddDeals.noOfTables = batch;
	
	for (int i = 0; i < batch; i++) {
	    memcpy(ddDeals.deals[i].cards, bd[i].hand, 16 * sizeof(int));
	}
	int strain[5] = { FALSE,FALSE,FALSE,FALSE,FALSE };
	int code = CalcAllTables(&ddDeals, -1, strain, &ddRes, NULL);
	char errMsg[80];
	if (code != 1) {
	    ErrorMessage(code, errMsg);
	    printf("%s %d", errMsg, code);
	}
	/*for (int i = 0; i < batch; i++) {
	    for (int h = 0; h < 4; h++) {
		for (int d = 0; d < 5; d++)
		    printf("%d ", ddRes.results[i].resTable[d][h]);
		printf("\n");
	    }
	    printf("\n");
	}*/
	
	for (int i = 0; i < batch; i++) {
	    if (ddRes.results[i].resTable[0][0] >= 8)
		res[0]++;
	    if (ddRes.results[i].resTable[4][0] >= 7)
		res[1]++;
	}
	/*PBNOutput pbn;
	pbn.open("NEW.pbn");
	int x = 0;
	for (auto &b : bd) {
	    pbn.write(pbn.writeBoard(b));
	    pbn.write(pbn.writeDDTable(ddRes.results[x].resTable)); x++;
	}
	pbn.close();*/
	printf("Batch: %d/%d\n", n+1, lp);
    }
    for(auto &a : res)
	printf("%d ", a);
    bd.clear();
    dm.genBoard(16, bd);

    struct ddTablesRes ddRes;
    struct ddTableDeals ddDeals;
    ddDeals.noOfTables = 16;
    for (int i = 0; i < 16; i++) {
	memcpy(ddDeals.deals[i].cards, bd[i].hand, 16 * sizeof(int));
    }
    int strain[5] = { FALSE,FALSE,FALSE,FALSE,FALSE };
    int code = CalcAllTables(&ddDeals, -1, strain, &ddRes, NULL);
    PBNOutput pbn;
    pbn.open("NEW.pbn");
    int x = 0;
    for (auto &b : bd) {
	pbn.write(pbn.writeBoard(b));
	pbn.write(pbn.writeDDTable(ddRes.results[x].resTable)); x++;
    }
    pbn.close();
}
int main() {
    
    test2();
    system("pause");
    return 0;

}


#include <stdio.h>
#include "cluige.h"

//#include <windows.h>

int main()
{
    //TODO mettre des const aux params dans ttes les struct

    //SetConsoleOutputCP(CP_UTF8);

    //init
	cluigeInit();//makes all roots, set all functions pointers, etc.

	//here hardcode or parse my scene from file
	//...
	Node n;
	iCluige.iNode.initZero(&n);
	iCluige.iNode.addChild(&iCluige.publicRoot2D, &n);

	Node* pn;
	for(int i=0; i<3; i++)
    {
        pn = iCluige.iNode.newNode();
        iCluige.iNode.addChild(&n, pn);
        Node* pn01 = iCluige.iNode.newNode();
        iCluige.iNode.addChild(pn, pn01);
    }//test on a gamer pc = 2minutes for 123456 iterations

	Node* pn01 = iCluige.iNode.newNode();
	iCluige.iNode.addChild(pn, pn01);
	wprintf(L"my node name : %ls\n", pn01->name);

	wprintf(L"\ntree : \n");
	iCluige.iNode.printTreePretty(&iCluige.privateRoot2D);


    //game loop
	cluigeRun();

//	wchar_t* t = L"└  ├";//;L"azerty";//// u"\u251c";//iCluige.checkedMalloc(88 * sizeof(wchar_t));
//	wchar_t* l = L"";//      ░░░░▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓███";//;L"poiuytreza";////u"\u2514";//iCluige.checkedMalloc(88 * sizeof(wchar_t));
////  ├  └
////	wchar_t t[88];
////	wchar_t l[88];
////	wscanf("%ls", t);
////	wscanf("%ls", l);
////	t[1] = 0;
////	t[0] = 0x251c;
////	l[1] = 0;
////	l[0] = 0x2514;
////	wprintf(t);
//	wprintf(L"%ls\n", t);
////	wprintf(l);
//	wprintf(L"%ls\n", l);

	wprintf(L"Finishing...\n");
    return cluigeFinish();
}


/*  old tests

	float* pf = iCluige.checkedMalloc(sizeof(float));
	(*pf) = 3.141592;
	wprintf(L"test iCluige.checkedMalloc : %f\n", (*pf));


*/

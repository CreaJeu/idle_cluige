#include <stdio.h>
#include <cluige.h>
#include <assert.h>
//#include <float.h>
//#include <math.h>

//if need (for debug) of SetConsoleOutputCP(CP_UTF8);
//#include <windows.h>

//(probably bad) if really want to call directly curses lib
//#include <../PDCurses/curses.h>


//void debug_deque(const Deque* d)
//{
//	StringBuilder sb_dq;
//	int N = iCluige.iDeque.size(d);
//	int sizeDequeMsg = N * 8;//quick & dirty test
//	char* dqMsg = iCluige.iStringBuilder.string_alloc(&sb_dq, sizeDequeMsg);
//	iCluige.iStringBuilder.append(&sb_dq, "[ ");
//	for(int iDq=0; iDq < N; iDq++)
//	{
//		uint64_t val = iCluige.iDeque.at(d, iDq).ui64;
//		iCluige.iStringBuilder.append(&sb_dq, "%d ", val);//"%.1f "
//	}
//	iCluige.iStringBuilder.append(&sb_dq, "]");
//	free(dqMsg);
//}


int main()
{
	//SetConsoleOutputCP(CP_UTF8);

	//init
	cluige_init();//makes all roots, set all functions pointers, etc.

	// ... game specific here

	cluige_run();

	int finish = cluige_finish();
#ifndef IMPROBABLE_CONSTANT
	printf("\nThe End ! Press ENTER key to quit...\n");
	getchar();
#endif // IMPROBABLE_CONSTANT
//	utils_breakpoint_trick(NULL, true);
	return finish;
}


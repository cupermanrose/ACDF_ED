#include <init.h>
#include <ED.h>
#include <ACDF.h>
#include <Experiment.h>

using namespace std;

int main() {
	Init();
	//Init_BackSum();
	/*test();
	return 0;*/
	ECDF();
	return 0;

	/*SamplingCDF();
	ResultOut << "**********************************************************" << endl;*/
	for (int i = 0; i < DeltaNum; i++) {
		switch (dataset)
		{
		case 1: {
				int Op[16] = { 1,11,2,12,3,13,4,14,5,15,6,16,7,17,8,18 };
				vector<int> BoundList(Op, Op + 16);
				ACDF(BoundList, int(DeltaList[i] * DataSize));
				ResultOut << "**********************************************************" << endl;
				ACDF_Error(BoundList, int(DeltaList[i] * DataSize));
				ResultOut << "**********************************************************" << endl;
				break;
			}
		case 2: {
				int Op[2] = { /*1,11,2,12,*/3,13/*,4,14,5,15,6,16,7,17,8,18*/ };
				vector<int> BoundList(Op, Op + 2);
				refinetime = 0;
				ACDF(BoundList, int(DeltaList[i] * DataSize));
				ResultOut << "**********************************************************" << endl;
				ResultOut << refinetime << endl;
				system("pause");
				/*ACDF_Error(BoundList, int(DeltaList[i] * DataSize));
				ResultOut << "**********************************************************" << endl;*/
				break;
			}
		case 3: {
				int Op[14] = { 1,11,2,12,3,13,4,14,5,15,6,16,7,17/*,8,18*/ };
				vector<int> BoundList(Op, Op + 14);
				ACDF(BoundList, int(DeltaList[i] * DataSize));
				ResultOut << "**********************************************************" << endl;
				ACDF_Error(BoundList, int(DeltaList[i] * DataSize));
				ResultOut << "**********************************************************" << endl;
				break;
			}
		}	
	}
	/*Sampling_Delta();
	ResultOut << "**********************************************************" << endl;*/
	return 0;
}
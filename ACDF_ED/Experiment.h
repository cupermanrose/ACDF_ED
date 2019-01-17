#ifndef  _EXPERIMENT_H
#define _EXPERIMENT_H

#include <mytime.h>
#include <vector>
#include <ED.h>
#include <ACDF.h>
#include <algorithm>
#include <random>

using namespace std;

struct func {
	double x, y;
};

vector<func> efunc, afunc;
int Rand[DataSize];
vector<double> GLB, GUB;

void test() {
	double Ac[21];
	for (int i = 0; i < 21; i++) Ac[i] = 0;
	int cnt = 0;
	for (int i = 0; i < QuerySize; i++) {
		cout << i << endl;
		
		Init_ED_k();

		for (int j = 0; j < DataSize; j++) {
			double ed = ED(QueryObject[i].Points, DataObject[j].Points,j);
			if (ed < eps) continue;
			cnt++;

			Ac[1] = Ac[1] + LB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 1) / ed;
			Ac[2] = Ac[2] + LB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 2) / ed;
			Ac[3] = Ac[3] + LB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 4) / ed;
			Ac[4] = Ac[4] + LB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 8) / ed;
			Ac[5] = Ac[5] + LB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 16) / ed;
			Ac[6] = Ac[6] + LB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 32) / ed;
			Ac[7] = Ac[7] + LB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 64) / ed;
			Ac[8] = Ac[8] + LB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 128) / ed;
			//Ac[9] = Ac[9] + LB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 256) / ed;
			//Ac[10] = Ac[10] + LB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 512) / ed;
			Ac[11] = Ac[11] + UB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 1) / ed;
			Ac[12] = Ac[12] + UB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 2) / ed;
			Ac[13] = Ac[13] + UB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 4) / ed;
			Ac[14] = Ac[14] + UB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 8) / ed;
			Ac[15] = Ac[15] + UB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 16) / ed;
			Ac[16] = Ac[16] + UB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 32) / ed;
			Ac[17] = Ac[17] + UB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 64) / ed;
			Ac[18] = Ac[18] + UB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 128) / ed;
			//Ac[19] = Ac[19] + UB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 256) / ed;
			//Ac[20] = Ac[20] + UB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 512) / ed;
		}
	}

	for (int i = 0; i < 21; i++) {
		ResultOut << i << ": " << Ac[i] / (double)(cnt) << endl;
	}
}

double MaxError(vector<func>& G, vector<func>& F) {
	double MaxError = 0;
	int i, j;
	j = 0;
	for (i = 0; i < G.size(); i++) {
		while ((G[i].x > F[j].x) && (j < F.size() - 1)) j++;
		MaxError = max(MaxError, abs(G[i].y - F[j].y));
	}
	i = 0;
	for (j = 0; j < F.size(); j++) {
		while ((F[j].x > G[i].x) && (i < G.size() - 1)) i++;
		MaxError = max(MaxError, abs(F[j].y - G[i].y));
	}
	return MaxError / (double)DataSize;
}

void ProduceGfunc(vector<func>& G) {
	GLB.clear(); GUB.clear(); G.clear();
	for (int i = 0; i < DataSize; i++) {
		GLB.push_back(Object[i].LB);
		GUB.push_back(Object[i].UB);
	}
	sort(GLB.begin(), GLB.end());
	sort(GUB.begin(), GUB.end());
	int i = 0;
	int j = 0;
	func Temp;
	while ((i < DataSize) && (j < DataSize)) {
		//if (abs(GLB[i] - GUB[j]) < eps) {
		if (GLB[i] == GUB[j]) {
			Temp.x = GLB[i];
			Temp.y = (double)(i + j) / 2;
			i++; j++;
		}
		else if (GLB[i] < GUB[j]) {
			Temp.x = GLB[i];
			Temp.y = (double)(i + j) / 2;
			i++;
		}
		else {
			Temp.x = GUB[j];
			Temp.y = (double)(i + j) / 2;
			j++;
		}
		G.push_back(Temp);
	}
	while (i < DataSize) {
		Temp.x = GLB[i];
		Temp.y = (double)(i + j) / 2;
		i++;
		G.push_back(Temp);
	}
	while (j < DataSize) {
		Temp.x = GUB[j];
		Temp.y = (double)(i + j) / 2;
		j++;
		G.push_back(Temp);
	}
	return;
}

void Sampling_Delta() {
	init_time();
	double SampleTime[DeltaNum];
	int Exact[DeltaNum];
	for (int i = 0; i < DeltaNum; i++) {
		SampleTime[i] = 0;
		Exact[i] = 0;
	}

	for (int i = 0; i < QuerySize; i++) {
		cout << i << endl;
		ecdf.clear();
		for (int j = 0; j < DataSize; j++) {
			ecdf.push_back(ED(QueryObject[i].Points, DataObject[j].Points,j));
		}
		sort(ecdf.begin(), ecdf.end());
		efunc.clear();
		for (int j = 0; j < DataSize; j++) {
			func Temp;
			Temp.x = ecdf[j];
			Temp.y = j;
			efunc.push_back(Temp);
		}

		clock_t Stime = clock();

		scdf.clear();

		for (int j = 0; j < DataSize; j++) Rand[j] = j;

		std::mt19937_64 gen(std::random_device{}());

		for (int j = 0; j < DataSize; j++) {
			int a = gen() % DataSize;
			int b = gen() % DataSize;
			int c = Rand[a]; Rand[a] = Rand[b]; Rand[b] = c;
		}

		bool Pass[DeltaNum];
		for (int j = 0; j < DeltaNum; j++) Pass[j] = false;

		for (int SampleNum = 0; SampleNum < DataSize; SampleNum++) {

			scdf.push_back(ED(QueryObject[i].Points, DataObject[Rand[SampleNum]].Points, Rand[SampleNum]));
			sort(scdf.begin(), scdf.end());
			afunc.clear();
			for (int j = 0; j < scdf.size(); j++) {
				func Temp;
				Temp.x = scdf[j];
				Temp.y = (double)(j + 1)*(DataSize / (double)scdf.size());
				afunc.push_back(Temp);
			}
			double TempError = MaxError(afunc, efunc);
			for (int j = 0; j < DeltaNum; j++) {
				if ((!Pass[j]) && (TempError < DeltaList[j])) {
					Pass[j] = true;
					SampleTime[j] = SampleTime[j] + (double)(clock() - Stime) / CLOCKS_PER_SEC;
					Exact[j] = Exact[j] + SampleNum;
				}
			}
		}
	}

	for (int i = 0; i < DeltaNum; i++) {
		ResultOut << DeltaList[i] << " Average exact distance: " << Exact[i] / QuerySize << endl;
		ResultOut << DeltaList[i] << " Average Time: " << SampleTime[i] / QuerySize << endl;
	}
	out_time("Sampling:");
}

void ACDF_Error(vector<int>& BoundList, int Delta) {
	init_time();
	int ed = 0;

	clock_t Stime;
	double Ave_MaxError = 0;
	double Max_MaxError = 0;
	double Min_MaxError = INFINITY;
	double ACDF_time = 0;

	for (int i = 0; i < QuerySize; i++) {
		cout << i << endl;

		// ECDF
		ecdf.clear();
		for (int j = 0; j < DataSize; j++) {
			ecdf.push_back(ED(QueryObject[i].Points, DataObject[j].Points, j));
		}
		sort(ecdf.begin(), ecdf.end());
		efunc.clear();
		for (int j = 0; j < DataSize; j++) {
			func Temp;
			Temp.x = ecdf[j];
			Temp.y = j;
			efunc.push_back(Temp);
		}


		Init_ED_k();
		Stime = clock();
		bool PassVerification = false;

		// Init LB and UB
		for (int j = 0; j < DataSize; j++) {
			Object[j].LB = 0;
			Object[j].UB = INFINITY;
			Object[j].number = j;
			ToUpdate[j] = true; // all objects need update at first
		}

		for (int k = 0; k < BoundList.size(); k++) {
			int cnt = 0; // update number
			for (int j = 0; j < DataSize; j++) {
				if (ToUpdate[j]) {
					cnt++;
					double LB = 0;
					double UB = INFINITY;
					switch (BoundList[k]) {
					case 1: LB = LB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 1); break;
					case 2: LB = LB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 2); break;
					case 3: LB = LB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 4); break;
					case 4: LB = LB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 8); break;
					case 5: LB = LB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 16); break;
					case 6: LB = LB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 32); break;
					case 7: LB = LB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 64); break;
					case 8: LB = LB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 128); break;
					case 9: LB = LB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 256); break;
					case 10: LB = LB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 512); break;
					case 11: UB = UB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 1); break;
					case 12: UB = UB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 2); break;
					case 13: UB = UB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 4); break;
					case 14: UB = UB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 8); break;
					case 15: UB = UB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 16); break;
					case 16: UB = UB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 32); break;
					case 17: UB = UB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 64); break;
					case 18: UB = UB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 128); break;
					case 19: UB = UB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 256); break;
					case 20: UB = UB_Delta(QueryObject[i].Points, i, DataObject[j].Points, j, 512); break;
					}
					Object[j].LB = max(Object[j].LB, LB);
					Object[j].UB = min(Object[j].UB, UB);
					ToUpdate[j] = false;
				}
			}
			//cout << BoundList[k] << ' ' << cnt << endl;
			if (BoundRefinement(Delta)) { ResultOut << k << endl; PassVerification = true; break; }
		}

		if (PassVerification) continue;
		if (OptimalRefinement(Delta)) continue;
		cout << "Final" << endl;
		int cnt = 0;
		for (int j = 0; j < DataSize; j++) {
			if (ToUpdate[j]) {
				ed++;
				cnt++;
				//Object[j].LB = Object[j].UB = ED(QueryObject[i].Points, DataObject[j].Points,j);
			}
		}

		if (!PassVerification) {
			if (!OptimalRefinement(Delta)) {
				for (int j = 0; j < DataSize; j++) {
					if (ToUpdate[j]) {
						ed++;
						Object[j].LB = Object[j].UB = ED(QueryObject[i].Points, DataObject[j].Points, j);
					}
				}
			}
		}
		cout << cnt << endl;

		ProduceGfunc(afunc);

		double TempError = MaxError(afunc, efunc);

		Max_MaxError = max(Max_MaxError, TempError);
		Min_MaxError = min(Min_MaxError, TempError);
		Ave_MaxError = Ave_MaxError + TempError;
		ACDF_time = ACDF_time + (double)(clock() - Stime) / CLOCKS_PER_SEC;
	}

	ResultOut << "ErrorDelta " << Delta << ": " << endl;
	ResultOut << "Average MaxError: " << Ave_MaxError / QuerySize * 100 << endl;
	ResultOut << "Maximum MaxError: " << Max_MaxError * 100 << endl;
	ResultOut << "Minimum MaxError: " << Min_MaxError * 100 << endl;
	ResultOut << "Average Time: " << ACDF_time / QuerySize << endl;
	out_time("ACDF_multibound");
	ResultOut << "ED: " << (double)ed / QuerySize << endl;
	return;

}

void SamplingCDF() {
	init_time();
	double SampleTime[SampleTest], Ave_MaxError[SampleTest], Max_MaxError[SampleTest], Min_MaxError[SampleTest];
	for (int i = 0; i < SampleTest; i++) {
		SampleTime[i] = 0;
		Ave_MaxError[i] = 0;
		Max_MaxError[i] = 0;
		Min_MaxError[i] = INFINITY;
	}

	for (int i = 0; i < QuerySize; i++) {
		cout << i << endl;

		//ECDF
		ecdf.clear();
		for (int j = 0; j < DataSize; j++) {
			ecdf.push_back(ED(QueryObject[i].Points, DataObject[j].Points,j));
		}
		sort(ecdf.begin(), ecdf.end());
		efunc.clear();
		for (int j = 0; j < DataSize; j++) {
			func Temp;
			Temp.x = ecdf[j];
			Temp.y = j;
			efunc.push_back(Temp);
		}

		for (int k = 0; k < SampleTest; k++) {
			clock_t Stime = clock();
			int SampleNum = DataSize*SamplingRatio[k];
			scdf.clear();
			srand((int)time(NULL));
			for (int jj = 0; jj < SampleNum; jj++) {
				int j = rand() % DataSize;
				scdf.push_back(ED(QueryObject[i].Points, DataObject[j].Points, j));
			}
			sort(scdf.begin(), scdf.end());
			afunc.clear();
			for (int j = 0; j < SampleNum; j++) {
				func Temp;
				Temp.x = scdf[j];
				Temp.y = (double)(j + 1)*(DataSize / (double)SampleNum);
				afunc.push_back(Temp);
			}

			double TempError = MaxError(afunc, efunc);
			Max_MaxError[k] = max(Max_MaxError[k], TempError);
			Min_MaxError[k] = min(Min_MaxError[k], TempError);
			Ave_MaxError[k] = Ave_MaxError[k] + TempError;
			SampleTime[k] = SampleTime[k] + (double)(clock() - Stime) / CLOCKS_PER_SEC;
		}
	}

	for (int i = 0; i < SampleTest; i++) {
		ResultOut << "SamplingRatio " << SamplingRatio[i] << ": " << endl;
		ResultOut << "Average MaxError: " << Ave_MaxError[i] / QuerySize * 100 << endl;
		ResultOut << "Maximum MaxError: " << Max_MaxError[i] * 100 << endl;
		ResultOut << "Minimum MaxError: " << Min_MaxError[i] * 100 << endl;
		ResultOut << "Average Time: " << SampleTime[i] / QuerySize << endl;
	}
	out_time("Sampling:");
}
#endif // ! _EXPERIMENT_H
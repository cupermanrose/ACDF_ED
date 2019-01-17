#ifndef _ED_H
#define _ED_H

#include <init.h>
#include <algorithm>

using namespace std;

struct ED_update{
	int ID,k; // the first k element(0,k-1) of (q,object[ID]) has been computed
	double value; // sum of the first k dist()
};

ED_update ED_k[DataSize];
double BackSum[DataSize][dimension],Q_BackSum[QuerySize][dimension];
double EDwithO[DataSize][MaxRand], EDwithQ[QuerySize][MaxRand];
int RandID[MaxRand];

void Init_MetricBound() {
	init_time();
	srand((int)time(NULL));
	for (int i = 0; i < MaxRand; i++) {
		RandID[i] = rand() % DataSize;
	}
	for (int i = 0; i < MaxRand; i++) {
		for (int j = 0; j < DataSize; j++) {
			EDwithO[j][i] = ED(DataObject[j].Points, DataObject[RandID[i]].Points, RandID[i]);
		}
		for (int j = 0; j < QuerySize; j++) {
			EDwithQ[j][i] = ED(QueryObject[j].Points, DataObject[RandID[i]].Points, RandID[i]);
		}
	}
	out_time("MetricBound Init:");
	return;
}

void Init_BackSum() {
	for (int i = 0; i < DataSize; i++) {
		for (int j = dimension - 1; j >= 0; j--) {
			if (j == dimension - 1) {
				BackSum[i][j] = DataObject[i].Points[j] * DataObject[i].Points[j];
				continue;
			}
			BackSum[i][j] = BackSum[i][j + 1] + DataObject[i].Points[j] * DataObject[i].Points[j];
		}
		for (int j = 0; j < dimension; j++) BackSum[i][j] = sqrt(BackSum[i][j]);
	}

	for (int i = 0; i < QuerySize; i++) {
		for (int j = dimension - 1; j >= 0; j--) {
			if (j == dimension - 1) {
				Q_BackSum[i][j] = QueryObject[i].Points[j] * QueryObject[i].Points[j];
				continue;
			}
			Q_BackSum[i][j] = Q_BackSum[i][j + 1] + QueryObject[i].Points[j] * QueryObject[i].Points[j];
		}
		for (int j = 0; j < dimension; j++) Q_BackSum[i][j] = sqrt(Q_BackSum[i][j]);
	}

	return;
}

void Init_ED_k() {
	for (int i = 0; i < DataSize; i++) {
		ED_k[i].ID = i;
		ED_k[i].k = 0;
		ED_k[i].value = 0;
	}
	return;
}

double dist(double a, double b) {
	return ((a - b)*(a - b));
}

double ED(vector<double>& Q, vector<double>& T,int t) {
	double res = 0;
	for (int i = 0; i < dimension; i++) {
		res = res + dist(Q[i], T[i]);
	}
	/*ED_k[t].k = dimension;
	ED_k[t].value = res;*/
	return sqrt(res);
}

double ED_FromK(vector<double>& Q, vector<double>& T, int t) {
	double res = ED_k[t].value;
	int K = ED_k[t].k;
	for (int i = K; i < dimension; i++) {
		res = res + dist(Q[i], T[i]);
	}
	ED_k[t].k = dimension;
	ED_k[t].value = res;
	return sqrt(res);
}

double LB_Delta(vector<double>& Q, int q, vector<double>& T, int t, int End) {
	double res = ED_k[t].value;
	int K = ED_k[t].k;
	for (int i = K; i < End; i++) {
		res = res + dist(Q[i], T[i]);
	}
	ED_k[t].k = max(K, End);
	ED_k[t].value = res;
	res = res + (Q_BackSum[q][End] - BackSum[t][End])*(Q_BackSum[q][End] - BackSum[t][End]);
	return sqrt(res);
}

double UB_Delta(vector<double>& Q, int q, vector<double>& T, int t, int End) {
	double res = ED_k[t].value;
	int K = ED_k[t].k;
	for (int i = K; i < End; i++) {
		res = res + dist(Q[i], T[i]);
	}
	ED_k[t].k = max(K, End);
	ED_k[t].value = res;
	res = res + (Q_BackSum[q][End] + BackSum[t][End])*(Q_BackSum[q][End] + BackSum[t][End]);
	return sqrt(res);
}

//double LB_metric(int q, int o) {
//	double LB = INFINITY;
//	for (int i = 0; i < RandID; i++) {
//		
//	}
//}
//
//double UB_metric() {
//
//}

#endif // !_ED_H
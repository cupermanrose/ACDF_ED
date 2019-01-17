#ifndef _ACDF_H
#define _ACDF_H

#include <init.h>
#include <ED.h>
#include <algorithm>

/*BoundMatch{
0:	ED
1: LB_Delta(1)		2: LB_Delta(2)		3: LB_Delta(4)		4: LB_Delta(8)		5: LB_Delta(16)
6: LB_Delta(32)		7: LB_Delta(64)		8: LB_Delta(128)	9: LB_Delta(256)	10: LB_Delta(512)
11: UB_Delta(1)		12: UB_Delta(2)		13: UB_Delta(4)		14: UB_Delta(8)		15: UB_Delta(16)
16: UB_Delta(32)	17: UB_Delta(64)	18: UB_Delta(128)	19: UB_Delta(256)	20: UB_Delta(512)
}*/

const int BoundType[21] = { 0,
							1,1,1,1,1,
							1,1,1,1,1,
							2,2,2,2,2,
							2,2,2,2,2
};

using namespace std;

struct Bound {
	double value;
	int number;
	bool type; // 0=LB,1=UB
};

struct LUB {
	double LB, UB;
	int number;
};

LUB Object[DataSize], TempObject[DataSize];
bool ToUpdate[DataSize], TempToUpdate[DataSize];

bool BoundCompare(Bound i, Bound j) {
	return(i.value < j.value);
}
bool LUBCompare(LUB i, LUB j) {
	return(i.LB < j.LB);
}
bool OnlyUBCompare(LUB i, LUB j) { // MAX-heap
	return(i.UB < j.UB);
}

bool BoundRefinement(int Delta) {
	clock_t Stime = clock();
	vector<Bound> Sub;
	vector<LUB> LBandUB, OnlyUB;

	Sub.clear(); LBandUB.clear();
	OnlyUB.clear(); // not heap in this function

	for (int j = 0; j < DataSize; j++) {
		ToUpdate[j] = false;

		Bound Temp;
		LUB TempLUB; TempLUB.number = j;

		Temp.value = Object[j].LB; Temp.number = j; Temp.type = 0;
		TempLUB.LB = Temp.value;
		Sub.push_back(Temp);

		Temp.value = Object[j].UB; Temp.number = j; Temp.type = 1;
		TempLUB.UB = Temp.value;
		Sub.push_back(Temp);

		LBandUB.push_back(TempLUB);
	}

	sort(Sub.begin(), Sub.end(), BoundCompare);
	sort(LBandUB.begin(), LBandUB.end(), LUBCompare); // sort all intervals

	int GLB, GUB; // the value of two function at this position (Sub[j])
	int LUB_i = 0;
	bool flag = true;
	GLB = GUB = 0;
	for (int j = 0; j < Sub.size(); j++) {
		if (Sub[j].type == 0) { GLB++; }
		else { GUB++; }

		while ((LUB_i < DataSize) && (Sub[j].value + eps > LBandUB[LUB_i].LB)) {
			OnlyUB.push_back(LBandUB[LUB_i]);
			LUB_i++;
		}
		if (GLB >= GUB + 2 * Delta) {
			flag = false;

			for (int k = 0; k < OnlyUB.size(); k++) {
				if (OnlyUB[k].UB + eps >= Sub[j].value) {
					ToUpdate[OnlyUB[k].number] = true; // all objects cover this interval need updating
				}
			}

			OnlyUB.clear();
		}
	}
	refinetime = refinetime + (double)(clock() - Stime) / CLOCKS_PER_SEC;
	return flag;
}

bool OptimalRefinement(int Delta) {
	clock_t Stime = clock();
	vector<Bound> Sub;
	vector<LUB> LBandUB, OnlyUB;

	Sub.clear(); LBandUB.clear();
	OnlyUB.clear(); // MAX-heap

	for (int j = 0; j < DataSize; j++) {
		ToUpdate[j] = false;
		Bound Temp;
		LUB TempLUB; TempLUB.number = j;

		Temp.value = Object[j].LB; Temp.number = j; Temp.type = 0;
		TempLUB.LB = Temp.value;
		Sub.push_back(Temp);

		Temp.value = Object[j].UB; Temp.number = j; Temp.type = 1;
		TempLUB.UB = Temp.value;
		Sub.push_back(Temp);

		LBandUB.push_back(TempLUB);
	}

	sort(Sub.begin(), Sub.end(), BoundCompare);
	sort(LBandUB.begin(), LBandUB.end(), LUBCompare); // sort all intervals
	make_heap(OnlyUB.begin(), OnlyUB.end(), OnlyUBCompare); // empty maxheap at first

	int GLB, GUB; // the value of two function at this position (Sub[j])
	int LUB_i = 0;
	bool flag = true;
	GLB = GUB = 0;

	vector<int> Subfunc;
	Subfunc.clear();
	for (int j = 0; j < Sub.size(); j++) {
		if (Sub[j].type == 0) { GLB++; }
		else { GUB++; }
		Subfunc.push_back(GLB - GUB);
	}

	int SubCount = 0;

	for (int j = 0; j < Sub.size(); j++) {

		Subfunc[j] = Subfunc[j] + SubCount;
		if (ToUpdate[Sub[j].number]) SubCount++; // the influence from Sub[j].number is end
		if (ToUpdate[Sub[j].number]) continue; // have been updated and cannot contribute to GLB-GUB
		while ((LUB_i < DataSize) && (Sub[j].value + eps > LBandUB[LUB_i].LB)) {
			OnlyUB.push_back(LBandUB[LUB_i]);
			push_heap(OnlyUB.begin(), OnlyUB.end(), OnlyUBCompare);
			LUB_i++;
		}

		if (Subfunc[j] <= 2 * Delta) continue;

		flag = false;

		while (Subfunc[j] > 2 * Delta) {
			int NowID = OnlyUB.front().number;
			ToUpdate[NowID] = true;// the max upper bound need to update
			pop_heap(OnlyUB.begin(), OnlyUB.end(), OnlyUBCompare);
			OnlyUB.pop_back();

			Subfunc[j]--;
			SubCount--; // [Object[NowID].LB,Object[NowID].UB] --;
		}
	}
	refinetime = refinetime + (double)(clock() - Stime) / CLOCKS_PER_SEC;
	return flag;
}

void ECDF() {
	init_time();
	double edtime = 0, sorttime = 0;
	for (int i = 0; i < QuerySize; i++) {
		cout << i << endl;
		clock_t Stime = clock();
		ecdf.clear();
		for (int j = 0; j < DataSize; j++) {
			ecdf.push_back(ED(QueryObject[i].Points, DataObject[j].Points, j));
		}
		edtime = edtime + (double)(clock() - Stime) / CLOCKS_PER_SEC;
		Stime = clock();
		sort(ecdf.begin(), ecdf.end());
		sorttime = sorttime + (double)(clock() - Stime) / CLOCKS_PER_SEC;
		/*for (int j = 0; j < DataSize; j++) {
			ResultOut << ecdf[j] << ' ' << j << endl;
		}*/
	}
	ResultOut << edtime << endl;
	ResultOut << sorttime << endl;
	return;
	out_time("ECDF");
	return;
}

void ACDF(vector<int>& BoundList, int Delta) {
	init_time();
	int ed = 0;
	for (int i = 0; i < QuerySize; i++) {
		cout << i << endl;
		Init_ED_k();
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
			cout << BoundList[k] << ' ' << cnt << endl;
			if (BoundRefinement(Delta)) { /*ResultOut << k << endl;*/ PassVerification = true; break; }
		}

		if (PassVerification) continue;
		if (OptimalRefinement(Delta)) continue;
		cout << "Final" << endl;
		int cnt = 0;
		for (int j = 0; j < DataSize; j++) {
			if (ToUpdate[j]) {
				ed++;
				cnt++;
				Object[j].LB = Object[j].UB = ED(QueryObject[i].Points, DataObject[j].Points,j);
			}
		}
		cout << cnt << endl;
	}
	
	ResultOut << "ED: " << (double)ed / QuerySize << endl;
	out_time("ACDF");
	return;

}

#endif // !_ACDF_H


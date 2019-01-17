#ifndef _INIT_H
#define _INIT_H

#include <mytime.h>
#include <vector>

double refinetime;

struct object {
	int ID;
	vector<double> Points;
};

object DataObject[DataSize], QueryObject[QuerySize];

vector<double> ecdf, acdf, scdf;

void DataLoading(string filename) {
	fstream fData(filename.c_str(), std::fstream::in);
	for (int i = 0; i < DataSize; i++) {
		object A;
		A.ID = i;
		A.Points.clear();
		for (int j = 0; j < dimension; j++) {
			double x;
			fData >> x;
			A.Points.push_back(x);
		}
		DataObject[i] = A;
	}
	fData.close();
	for (int i = 0; i < QuerySize; i++) {
		QueryObject[i] = DataObject[i*(DataSize / QuerySize)];
	}
	return;
}

void Init() {
	init_time();
	DataLoading(DataFile);
	out_time("Init");
	cout << "init!" << endl;
	return;
}

#endif // !_INIT_H


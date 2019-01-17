#ifndef _PARAMETER_H
#define _PARAMETER_H

#include <string>
#include <fstream>

using namespace std;

const int MaxRand = 100;
const int DeltaNum = 10;
const double DeltaList[10] = { 0.005,0.01,0.015,0.02,0.03,0.04,0.05,0.06,0.07,0.08 };
const int SampleTest = 10;
const double SamplingRatio[10] = { 0.01,0.05,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8 };


fstream ResultOut("G:\\work\\DFD_convoy\\experimence_results\\similarity_search\\OSM_FULL\\test.txt", ios::out);

const double eps = 1.0E-8;


//CM 55
//const int QuerySize = 10;
//const int DataSize = 269648;
//const int dimension = 225;
//string DataFile = "G:\\work\\DFD_convoy\\dataset\\NUS\\Low_Level_Features\\Normalized_CM55.dat";
//const int dataset = 1;

//CORR
const int QuerySize = 10;
const int DataSize = 269648;
const int dimension = 144;
string DataFile = "G:\\work\\DFD_convoy\\dataset\\NUS\\Low_Level_Features\\Normalized_CORR.dat";
const int dataset = 2;

//WT
//const int QuerySize = 10;
//const int DataSize = 20000;// 269648;
//const int dimension = 128;
//string DataFile = "G:\\work\\DFD_convoy\\dataset\\NUS\\Low_Level_Features\\Normalized_CORR.dat";
//const int dataset = 3;

#endif // !_PARAMETER_H


/****************************************************************************
  FileName     [modem.h]
  PackageName  [ MODEM ]
  Synopsis     [ COMMLAB ]
  Author       [ Juei-Yang (Tony) Hsu ]
****************************************************************************/
#ifndef MODEM_H
#define MODEM_H
#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
#include <string>
#include <bitset>
#include "util.h"
using namespace std;
class Modem{
public:
	Modem(string _modu_type,double _Eb){
		Eb = _Eb;
		modu_type = _modu_type;
		if (!(_modu_type.compare("BPSK"))) amp=sqrt(_Eb);
		else if(!(_modu_type.compare("8PSK"))) amp=sqrt(_Eb*3);
		else amp=sqrt(_Eb*4/10);
	}
	vector<tuple<double,double> > modulation(bitset<8*2*BUFSIZE> bits);
	bitset<8*2*BUFSIZE> demodulation(vector<tuple<double,double> >symbols);
	tuple<double,double> grayMapper(unsigned long bit_char);
	string md_detector(tuple<double,double> symbol);

private:
	double Eb;
	double amp;
	string modu_type;
};
#endif

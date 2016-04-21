/****************************************************************************
  FileName     [modem.h]
  PackageName  [ MODEM ]
  Synopsis     [ COMMLAB ]
  Author       [ Juei-Yang (Tony) Hsu ]
****************************************************************************/
#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
#include <string>
#include <bitset>

#include "util.h"
using namespace std;

#define BUFSIZE (15)
/*Modulation
 **arguments**
 * bits:transmitted bits
 * modu_type:modulation method [BPSK,8PSK,16QAM]
 * amp:modulation amplitude

 **return**
 * symbol's vector (each element is a tuple)
 */
vector<tuple<double,double> > modulation(bitset<8*BUFSIZE> bits,string modu_type,double Es){
	vector<tuple<double,double> > symbols;
	string bits_str = bits.to_string();
	if(!(modu_type.compare("BPSK"))){
		double amp = Es;
		for(size_t i=0;i<bits.size();++i){
			if(bits_str[i] == '1') symbols.push_back(make_tuple(amp,0));
			else symbols.push_back(make_tuple(-1*amp,0));
		}
	}
	else if(!(modu_type.compare("8PSK"))){
		double amp=Es;
		for(size_t i=0;i<bits.size();i+=3){
			bitset<8> bits_char(bits_str.substr(i,3));
			symbols.push_back(grayMapper(bits_char.to_ulong(),modu_type,amp));
		}
	}
	else if(!(modu_type.compare("16QAM"))){
		double amp = sqrt(Es/10);
		for(size_t i=0;i<bits.size();i+=4){
			bitset<8> bits_char(bits_str.substr(i,4));
			symbols.push_back(grayMapper(bits_char.to_ulong(),modu_type,amp));
		}
	}
	else{
		cerr << "Invalid modulation type (" << modu_type << ")" << endl;
		exit(1);
	}
	return symbols;
}

/*deModulation 
 **arguments**
 * symbols:received symbols
 * modu_type:modulation method [BPSK,8PSK,16QAM]
 * amp:modulation amplitude

 **return**
 * symbol's vector (each element is a tuple)
 */
//bitset<8*BUFSIZE*2> demodulation(vector<tuple<double,double> >symbols,string modu_type,double amp){
bitset<8*BUFSIZE> demodulation(vector<tuple<double,double> >symbols,string modu_type,double Es){
	string bits_str;
	double amp = modu_type.compare("16QAM")?Es:sqrt(Es/10);
	for(size_t i=0;i<symbols.size();++i){
		bits_str += md_detector(symbols[i],modu_type,amp);
	}
	return bitset<8*BUFSIZE>(bits_str);
}

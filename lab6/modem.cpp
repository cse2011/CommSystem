#include "modem.h"
using namespace std;

vector<tuple<double,double> > Modem::modulation(bitset<8*2*BUFSIZE> bits){
	vector<tuple<double,double> > symbols;
	string bits_str = bits.to_string();
	if(!(this->modu_type.compare("BPSK"))){
		for(size_t i=0;i<bits.size();++i){
			if(bits_str[i] == '1') symbols.push_back(make_tuple(amp,0));
			else symbols.push_back(make_tuple(-1*amp,0));
		}
	}
	else if(!(this->modu_type.compare("8PSK"))){
		for(size_t i=0;i<bits.size();i+=3){
			bitset<8> bits_char(bits_str.substr(i,3));
			symbols.push_back(grayMapper(bits_char.to_ulong()));
		}
	}
	else{
		for(size_t i=0;i<bits.size();i+=4){ 
			bitset<8> bits_char(bits_str.substr(i,4));
			symbols.push_back(grayMapper(bits_char.to_ulong()));
		}
	}
	return symbols;
}

bitset<8*2*BUFSIZE> Modem::demodulation(vector<tuple<double,double> >symbols){
	string bits_str;
	for(size_t i=0;i<symbols.size();++i){
		bits_str += md_detector(symbols[i]);
	}
	return bitset<8*2*BUFSIZE>(bits_str);
}

tuple<double,double> Modem::grayMapper(unsigned long bit_char){
	if(!(this->modu_type.compare("8PSK"))){
		if(bit_char == 0) return make_tuple(amp,0);
		else if(bit_char == 1) return make_tuple(amp*SQRT2,amp*SQRT2);
		else if(bit_char == 3) return make_tuple(0,amp);
		else if(bit_char == 2) return make_tuple(-1*amp*SQRT2,amp*SQRT2);
		else if(bit_char == 6) return make_tuple(-1*amp,0);
		else if(bit_char == 7) return make_tuple(-1*amp*SQRT2,-1*amp*SQRT2);
		else if(bit_char == 5) return make_tuple(0,-1*amp);
		else return make_tuple(amp*SQRT2,-1*amp*SQRT2);
	}
	else{ // 16QAM
		if(bit_char==0) return make_tuple(-3*amp,-3*amp);
		else if(bit_char == 1) return make_tuple(-3*amp,-1*amp);
		else if(bit_char == 2) return make_tuple(-3*amp,3*amp);
		else if(bit_char == 3) return make_tuple(-3*amp,amp);
		else if(bit_char == 4) return make_tuple(-1*amp,-3*amp);
		else if(bit_char == 5) return make_tuple(-1*amp,-1*amp);
		else if(bit_char == 6) return make_tuple(-1*amp,3*amp);
		else if(bit_char == 7) return make_tuple(-1*amp,amp);
		else if(bit_char == 8) return make_tuple(3*amp,-3*amp);
		else if(bit_char == 9) return make_tuple(3*amp,-1*amp);
		else if(bit_char == 10) return make_tuple(3*amp,3*amp);
		else if(bit_char == 11) return make_tuple(3*amp,amp);
		else if(bit_char == 12) return make_tuple(amp,-3*amp);
		else if(bit_char == 13) return make_tuple(amp,-1*amp);
		else if(bit_char == 14) return make_tuple(amp,3*amp);
		else return make_tuple(amp,amp);
	}

}
string Modem::md_detector(tuple<double,double> symbol){
	double x,y;
	tie(x,y) = symbol;
	if(!(this->modu_type.compare("BPSK"))){
		if(x>=0) return to_string(1);
		else return to_string(0);
	}
	else if(!(this->modu_type.compare("8PSK"))){
		double p = atan2(y,x);
        if(p >= -PI/8 && p < PI/8) return string("000");
        else if(p>= PI/8 && p< 3*PI/8) return string("001");
        else if(p>= 3*PI/8 && p< 5*PI/8) return string("011");
        else if(p>= 5*PI/8 && p< 7*PI/8) return string("010");
        else if(p>= 7*PI/8 || p< -7*PI/8) return string("110");
        else if(p>= -7*PI/8 && p< -5*PI/8) return string("111");
        else if(p>= -5*PI/8 && p< -3*PI/8) return string("101");
        else return string("100");
	}
	else{ //16QAM
		if (x<=-2*amp){
			if(y <= -2*amp) return string("0000");
			else if(y<=0) return string("0001");
			else if(y<=2*amp) return string("0011");
			else return string("0010");
		}
		else if(x<=0){
			if(y <= -2*amp) return string("0100");
			else if(y<=0) return string("0101");
			else if(y<=2*amp) return string("0111");
			else return string("0110");
		}
		else if(x<=2*amp){
			if(y <= -2*amp) return string("1100");
			else if(y<=0) return string("1101");
			else if(y<=2*amp) return string("1111");
			else return string("1110");
		}
		else{ // x>2*amp
			if(y <= -2*amp) return string("1000");
			else if(y<=0) return string("1001");
			else if(y<=2*amp) return string("1011");
			else return string("1010");
		}
	}
}

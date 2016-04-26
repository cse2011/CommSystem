#ifndef CONVOLUTION_H
#define CONVOLUTION_H
/****************************************************************************
  FileName     [convolution.h]
  PackageName  [ CONV ]
  Synopsis     [ COMMLAB  ]
  Author       [ Juei-Yang (Tony) Hsu ]
****************************************************************************/

#include <iostream>
#include <bitset>
#include <string>
#include <limits>
#include "util.h"
#include <algorithm>

const double POS_INF = numeric_limits<double>::infinity();

using namespace std;

class Delta{
public:
	Delta(){
		prev_state_ptr = NULL;
		cost = POS_INF;
		state = 123;
	}
	size_t state;
	double cost;
	Delta* prev_state_ptr;

	bool operator<(const Delta&rhs){return this->cost<rhs.cost;}
};
class Transit{
public:
	//Transit(){
		//choice = -1;
		//output = bitset<2>o;
	//}
	Transit(int _choice,string _output){
		choice = _choice;
		output = bitset<2>(_output);
	}
	int choice;
	bitset<2> output;
};
//class Decision
//{
//public:
	//Decision(size_t _next_state,string _output){
		//next_state = _next_state;
		//output = _output;
	//}
	//size_t next_state;
	//string output;
//};
class Convolution{
public:
	Convolution():
		transit_map{
			{Transit(0,"00"),Transit(-1,"00"),Transit(1,"11"),Transit(-1,"00")},
			{Transit(0,"11"),Transit(-1,"00"),Transit(1,"01"),Transit(-1,"00")},
			{Transit(-1,"00"),Transit(-1,"00"),Transit(0,"01"),Transit(1,"10")},
			{Transit(-1,"00"),Transit(-1,"00"),Transit(0,"10"),Transit(1,"01")}
		},
		decision_map{{0,2},{0,2},{1,3},{1,3}}{}
	bitset<8*2*BUFSIZE> encode(bitset<8*BUFSIZE> info_bits);
	bitset<8*BUFSIZE> decode(bitset<8*2*BUFSIZE> coded_bits);

		//this->transit_map[0]={Transit(0,"00"),Transit(-1,"00"),Transit(1,"11"),Transit(-1,"00")};
		//this->transit_map[1]={Transit(0,"11"),Transit(-1,"00"),Transit(1,"01"),Transit(-1,"00")};
		//this->transit_map[2]={Transit(-1,"00"),Transit(-1,"00"),Transit(0,"01"),Transit(1,"10")};
		//this->transit_map[3]={Transit(-1,"00"),Transit(-1,"00"),Transit(0,"10"),Transit(1,"01")};
		
		//this->decision_map[0]={0,2};
		//this->decision_map[1]={0,2};
		//this->decision_map[2]={1,3};
		//this->decision_map[3]={1,3};
	Transit transit_map[4][4];
	size_t decision_map[4][2];
};
#endif

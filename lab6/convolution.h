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

#define BUFSIZE (15)
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
	Transit(int _choice,string _output){
		choice = _choice;
		output = bitset<2>(_output);
	}
	int choice;
	bitset<2> output;
}
Transit transit_map[4][4];
transit_map[0]={Transit(0,"00"),Transit(-1,"00"),Transit(1,"11"),Transit(-1,"00")};
transit_map[1]={Transit(0,"11"),Transit(-1,"00"),Transit(1,"01"),Transit(-1,"00")};
transit_map[2]={Transit(-1,"00"),Transit(-1,"00"),Transit(0,"01"),Transit(1,"10")};
transit_map[3]={Transit(-1,"00"),Transit(-1,"00"),Transit(0,"10"),Transit(1,"01")};
bitset<8*2*BUFSIZE> conv_encoder(bitset<8*BUFSIZE> info_bits){
	string coded_bits;
}
bitset<8*BUFSIZE> conv_decoder(bitset<8*2*BUFSIZE> coded_bits){
	string info_bits;
	string coded_bits_str = coded_bits.to_string();
	
	vector<vector<Delta> > deltas(8*BUFSIZE + 1);
	vector<Delta> delta(4);

	//base case
	delta[0].cost = 0;
	delta[0].state = 0;
	deltas[0] = delta;
	delta.clear();
	for(size_t i=0;i<4;++i){
		bitset<2> d(coded_bits.substr(0,2));
		if (transit_map[0][i].choice != -1){
			delta[i].cost = hamming_dis(d,transit_map[0][i].output);
			delta[i].state = i;
			delta[i].prev_state_ptr = &deltas[0][0];
		}
	}
	deltas[1] = delta;

	//recursive case
	for(size_t i=2;i<BUFSIZE*8+1;++i){
		delta.clear();
		bitset<2> d(coded_bits.substr(2*(i-1),2));
		for(size_t cur_state=0;cur_state<4;++cur_state){
			vector<double> costs(4);
			for(size_t from_state=0;from_state<4;++from_state){
				int choice = transit_map[0][i].choice;
				if (choice != -1){
				costs[from_state] = deltas[i-1][from_state].cost + hamming_dis(d,transit_map[from_state][cur_state]);
				}
			}
			delta[cur_state].cost = *(min_element(costs.begin(),consts.end()));
			delta[cur_state].state = cur_state;
			delta[cur_state].prev_state_ptr = &(deltas[i-1][distance(costs.begin(),min_element(costs.begin(),consts.end()))]);
		}
		deltas[i] = delta;
	}
	
	//backtrace
	Delta psi = *(min_element(deltas[8*BUFSIZE+1].begin(),deltas[8*BUFSIZE+1].end()));
	for(size_t i=120;i>=0;--i){
		if(psi.prev_state_ptr != NULL){
			size_t next_state = psi.state;
			psi = *(psi.prev_state_ptr);
			info_bits += to_string(transit_map[psi.state][next_state]);
		}
	}
	info_bits = reverse(info_bits);
}
#endif

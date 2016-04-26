#include "convolution.h"
using namespace std;

bitset<8*BUFSIZE> Convolution::decode(bitset<8*2*BUFSIZE> coded_bits){
	string info_bits;
	string coded_bits_str = coded_bits.to_string();
	
	vector<vector<Delta> > deltas(8*BUFSIZE + 1);
	vector<Delta> delta(4);

	//base case
	delta[0].cost = 0;
	delta[0].state = 0;
	deltas[0] = delta;
	delta.clear();
	delta.resize(4);
	bitset<2> d(coded_bits_str.substr(0,2));
	for(size_t i=0;i<4;++i){
		if (transit_map[0][i].choice != -1){
			delta[i].cost = hamming_distance(d,transit_map[0][i].output);
			delta[i].state = i;
			delta[i].prev_state_ptr = &deltas[0][0];
		}
	}
	deltas[1] = delta;

	//recursive case
	for(size_t i=2;i<BUFSIZE*8+1;++i){
		delta.clear();
		delta.resize(4);
		bitset<2> d(coded_bits_str.substr(2*(i-1),2));
		for(size_t cur_state=0;cur_state<4;++cur_state){
			vector<double> costs(4);
			for(size_t from_state=0;from_state<4;++from_state){
				int choice = transit_map[from_state][cur_state].choice;
				if (choice != -1){
				costs[from_state] = deltas[i-1][from_state].cost + hamming_distance(d,transit_map[from_state][cur_state].output);
				}
			}
			delta[cur_state].cost = *(min_element(costs.begin(),costs.end()));
			delta[cur_state].state = cur_state;
			delta[cur_state].prev_state_ptr = &(deltas[i-1][distance(costs.begin(),min_element(costs.begin(),costs.end()))]);
		}
		deltas[i] = delta;
	}
	
	//backtrace
	Delta psi = *(min_element(deltas[8*BUFSIZE].begin(),deltas[8*BUFSIZE].end()));
	for(int i=8*BUFSIZE;i>=0;--i){
		if(psi.prev_state_ptr != NULL){
			size_t next_state = psi.state;
			psi = *(psi.prev_state_ptr);
			info_bits += transit_map[psi.state][next_state].output.to_string();
		}
	}
	reverse(info_bits.begin(),info_bits.end());

	return bitset<8*BUFSIZE>(info_bits);
}

bitset<8*2*BUFSIZE> Convolution::encode(bitset<8*BUFSIZE> info_bits){
	string coded_bits;
	size_t cur_state = 0;
	size_t next_state = 0;

	for(size_t i=0;i<info_bits.size();++i){
		next_state = decision_map[cur_state][info_bits[i]];
		coded_bits += transit_map[cur_state][next_state].output.to_string();
	}

	return bitset<8*2*BUFSIZE>(coded_bits);
}

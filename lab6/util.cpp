#include "util.h"
using namespace std;

double hamming_distance(bitset<2> c,bitset<2> d){
	return double((c^d).count());
}
double calculate_ber(bitset<BUFSIZE*8> rx,bitset<BUFSIZE*8> tx){
	//calulating hamming distance
	size_t hamming_distance = (tx ^ rx).count();
	return double(hamming_distance)/tx.size();
}

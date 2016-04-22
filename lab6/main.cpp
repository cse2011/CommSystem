#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <bitset>
#include <string>
#include <tuple>
#include <random>

#include "modem.h"
#include "util.h"
using namespace std;

template< size_t size>
bitset<size> random_bitset( double p){

	bitset<size> bits;
	random_device rd;
	mt19937 gen(rd());
	bernoulli_distribution d(p);

	for( int n = 0; n < size; ++n) {
		bits[n] = d(gen);
	}
	return bits;
}
int main(int argc,const char* argv[])
{
	if(argc ==5){
		string modu_type = string(argv[1]);
		double Eb = atof(argv[2]);
		double N0 = atof(argv[3]);
		//double real_N0 = modu_type.compare("BPSK")?N0:2*N0;
		double real_N0 = N0;
		size_t NUM_PACKET = atoi(argv[4]);

		default_random_engine generator;
		normal_distribution<double> dist(0,sqrt(real_N0/2));

		bool is2D = modu_type.compare("BPSK");
		double ber_sum = 0;
		size_t num_diff_packet = 0;
		Modem modemer(modu_type,Eb);

		for(size_t i=0;i<NUM_PACKET;++i){
			bitset<8*BUFSIZE> tx_info_bits = random_bitset<120>(0.5);
			//bitset<8*2*BUFSIZE> tx_coded_bits;
			bitset<8*BUFSIZE> tx_coded_bits(tx_info_bits);
			//tx_coded_bits = conv_encoder(tx_info_bits);

			vector<tuple<double,double> > tx_symbols;
			tx_symbols = modemer.modulation(tx_coded_bits);

			vector<tuple<double,double> > rx_symbols;
			rx_symbols.resize(tx_symbols.size());

			//add noise
			for(size_t i=0;i<tx_symbols.size();++i){
				double x,y;
				tie(x,y) = tx_symbols[i];
				if(is2D){
					x += dist(generator);
					y += dist(generator);
				}
				else{
					x += dist(generator);
				}
				rx_symbols[i] = make_tuple(x,y);
			}
			//bitset<8*BUFSIZE*2> rx_coded_bits= demodulation(rx_symbols,modu_type);
			bitset<8*BUFSIZE> rx_coded_bits= modemer.demodulation(rx_symbols);

			//bitset<8*BUFSIZE> rx_info_bits = conv_decoder(rx_coded_bits);
			bitset<8*BUFSIZE> rx_info_bits(rx_coded_bits);
			ber_sum += calculate_ber(rx_info_bits,tx_info_bits);
			if (rx_info_bits != tx_info_bits) ++num_diff_packet;
		}
		//calculate
		cout << "=====STATISTICS=====" << endl;
		cout << "N0 : " << N0 <<endl;
		cout << "Eb : " << Eb << endl;
		cout << "NUMBER OF PACKETS : " << NUM_PACKET << endl;
		cout << "PER : " << double(num_diff_packet)/ NUM_PACKET << endl;
		cout << "BER : " << ber_sum/NUM_PACKET << endl;
	}
	else{
		cerr << "Usage : ./audio <MODEM> <Eb> <N0> <#PACKET>" << endl;
		exit(1);
	}
}

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <bitset>
#include <string>
#include <tuple>
using namespace std;
#define BUFSIZE (15)
// WAVE PCM soundfile format
typedef struct header_file
{
    char chunk_id[4];
    int chunk_size;
    char format[4];
    char subchunk1_id[4];
    int subchunk1_size;
    short int audio_format;
    short int num_channels;
    int sample_rate;
    int byte_rate;
    short int block_align;
    short int bits_per_sample;
    char subchunk2_id[4];
    int subchun2_size;
} header;

typedef struct header_file* header_p;

int main(int argc,const char* argv[])
{
	FILE * infile = fopen("input.wav","rb");
	FILE * outfile = fopen("output.wav","wb");

	string modu_type = string(argv[1]);
	size_t num_per_symbol;
	bool is2D = modu_type.compare("BPSK");
	int count = 0;
	unsigned char rx_buff8[BUFSIZE],tx_buff8[BUFSIZE];
	header_p meta = (header_p)malloc(sizeof(header));
	int nb;

	if (infile)
	{
		fread(meta, 1, sizeof(header), infile);
		fwrite(meta,1, sizeof(*meta), outfile);
		while (!feof(infile))
		{
			nb = fread(tx_buff8,1,BUFSIZE,infile);
			count++;

			//collect the packet (120 bits)
			string data_str;
			for(size_t i=0;i<nb;++i){
				bitset<8> tmp(tx_buff8[i]);
				data_str += tmp.to_string();
			}
			bitset<8*BUFSIZE> tx_info_bits(data_str);
			string rx_data_str = tx_info_bits.to_string();
			for(size_t i=0;i<nb;++i){
				bitset<8> tmp(rx_data_str.substr(8*i,8));
				rx_buff8[i] = static_cast<unsigned char>(tmp.to_ulong());
			}
			
			//send it to the convolution encoder
			bitset<8*BUFSIZE*2> tx_coded_bits;
			tx_coded_bits = conv_encoder(tx_info_bits);

			vector<tuple<double x,double y> > tx_symbols;
			tx_symbols = modulation(tx_coded_bits,modu_type);

			vector<tuple<double x,double y> > rx_symbols;
			rx_symbols = add_noise(tx_symbols);
			
			bitset<8*BUFSIZE*2> rx_coded_bits= demodulation(rx_symbols,modu_type);

			bitset<8*BUFSIZE> rx_info_bits = conv_decoder(rx_coded_bits);

			
			string rx_data_str = rx_info_bits.to_string();
			for(size_t i=0;i<nb;++i){
				bitset<8> tmp(rx_data_str.substr(8*i,8));
				rx_buff8[i] = static_cast<unsigned char>(tmp.to_ulong());
			}
			fwrite(rx_buff8,1,nb,outfile);
		}
		cout << " Number of frames in the input wave file are " <<count << endl;
	}
	return 0;
}

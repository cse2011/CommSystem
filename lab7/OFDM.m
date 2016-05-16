clear all;
load('LTI');

N = 128000 / 2; % Total number of symbols
Nc = 128; % OFDM Symbol Length (number of parallel bitstreams)
h_k = zeros(Nc,1);

% Build Impulse Response of LTI channel in freq domain
for k=0:Nc-1
    for l=0:length(h)-1
        h_k(k+1) = h_k(k+1) + h(l+1)*exp(-i*2*pi*k*l/Nc);
    end
end

L = 40; % Cyclic Prefix Length
K = N / Nc; % Number of OFDM Symbols

M = 8;
SNR = 10;

if M == 8
    SNR  = SNR * 3;
elseif M == 16
    SNR = SNR * 4 /sqrt(10);
end

% generate random symbols for transmit
tx_data = randi([0 M-1],N,1);
tx_bit_stream = de2bi(tx_data);
tx_symbols = pskmod(tx_data, M, 0); %BPSK (2) and 8PSK (8)
%tx_symbols = qammod(tx_data, M); %16QAM (16)
rx_symbols = [];


for i=1:K
    
    % Nc-point IDFT
    tx_idft_time_symbols = IDFT(tx_symbols((i-1)*Nc+1:i*Nc),Nc);
 
    %Cyclic Prefix
    tx_cyclic_prefix_symbols = zeros(Nc + L-1, 1);
    tx_cyclic_prefix_symbols(1:L-1) = tx_idft_time_symbols(end-L+2:end);
    tx_cyclic_prefix_symbols(L:end) = tx_idft_time_symbols;
    
    %LTI channel
    rx_cyclic_prefix_symbols = LTIchannel(tx_cyclic_prefix_symbols,h,SNR);
    
    %Cyclic Prefix Removal
    rx_idft_time_symbols = rx_cyclic_prefix_symbols(L:end);
    
    %Nc-point DFT
    rx_symbols = [rx_symbols;DFT(rx_idft_time_symbols,Nc) ./ h_k];
end

rx_data = pskdemod(rx_symbols,M,0);
%rx_data = qamdemod(rx_symbols,M);
rx_bit_stream = de2bi(rx_data);

%disp(sum(xor(rx_bit_stream,tx_bit_stream)));
BER = sum(xor(rx_bit_stream,tx_bit_stream))/length(tx_bit_stream); 
disp(BER);
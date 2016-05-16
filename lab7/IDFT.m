function [ time_symbols ] = IDFT( freq_symbols, Nc )
    u_n = zeros(Nc,1);
    for n=0:Nc-1
        for k=0:Nc-1
            u_n(n+1) = u_n(n+1) + freq_symbols(k+1) * exp(j*2*pi*k*n/Nc);
        end
        u_n(n+1) = u_n(n+1) / sqrt(Nc);
    end
    time_symbols = u_n;
end
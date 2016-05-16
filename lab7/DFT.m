function [ freq_symbols ] = DFT( time_symbols,Nc )
    y_k = zeros(Nc,1);
    for k=0:Nc-1
        for n=0:Nc-1
            y_k(k+1) = y_k(k+1) + time_symbols(n+1) * exp(-j*2*pi*n*k/Nc);
        end
        y_k(k+1) = y_k(k+1) / sqrt(Nc);
    end
    freq_symbols = y_k;
end


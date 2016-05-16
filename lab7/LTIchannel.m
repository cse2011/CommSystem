function [ lti_symbols ] = LTIchannel( symbols,h,SNR )
    ret = zeros(length(symbols),1);
    
    %ret = conv(symbols,h);
    for n=1:length(symbols)
         for l=0:length(h)-1
             if( n-l > 0) 
                 ret(n)  = ret(n) + h(l+1) * symbols(n-l); %h(l+1) because of 1-based script...= =
             else
                 break
             end 
         end
        ret(n) = ret(n) + sqrt(1/(2*SNR))*(randn+1i*randn);
    end
    lti_symbols = ret;
end
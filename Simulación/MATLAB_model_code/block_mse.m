function [timeMSE]=block_mse(e,blockSize)
    N=length(e);
    nSamples=floor(N/blockSize);
    timeMSE=zeros(nSamples,1);
    
    for i=1:nSamples
        timeMSE(i)=0;
        for j=1:blockSize
            timeMSE(i)=timeMSE(i)+e(((i-1)*blockSize)+j)^2;
        end
        timeMSE(i)=timeMSE(i)/blockSize;
    end
end

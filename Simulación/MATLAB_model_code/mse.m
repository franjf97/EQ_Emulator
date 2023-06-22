function [timeMSE]=mse(error)
    N=length(error);
    timeMSE=zeros(N,1);
    timeMSE(1)=error(1)^2;

    for i = 2:N
        timeMSE(i)=timeMSE(i-1)+(1/i)*((error(i)^2)-timeMSE(i-1));
    end
end
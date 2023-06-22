clear;
clc;
%%
figure
hold on
load('u5.mat')
plot(t,timeMSE,'DisplayName',['\mu=' num2str(mu)])
load('u4.mat')
plot(t,timeMSE,'DisplayName',['\mu=' num2str(mu)])
load('u3.mat')
plot(t,timeMSE,'DisplayName',['\mu=' num2str(mu)])
load('u2.mat')
plot(t,timeMSE,'DisplayName',['\mu=' num2str(mu)])
load('u1.mat')
plot(t,timeMSE,'DisplayName',['\mu=' num2str(mu)])
grid on
legend
title('Variación de \mu')
xlabel('t [s]')
ylabel('MSE')
clear;
clc;
%%
Fs=48000;
[RI_amp,~]=audioread('Amplificador completo.wav');
RI_amp=RI_amp(47974:end,1);
[imp_amp,t_amp] = impz(RI_amp,1,length(RI_amp),Fs);
[h_amp,f_amp]=freqz(RI_amp,1,10000,Fs);
mag_amp=10*log10(abs(h_amp));
figure
plot(t_amp,imp_amp),grid on, title 'Respuesta al impulso sistema desconocido'
xlabel('Time (s)')
ylabel('Magnitude')
figure
freqz(RI_amp,1,10000,Fs), title 'Respuesta en frecuencia Sistema desconocido'
%%
[RI_mod,~]=audioread('Amplificador emulado.wav');
RI_mod=RI_mod(47974:end,1);
%%
scale_factor=max(abs(RI_amp))/max(abs(RI_mod));
RI_mod=-scale_factor*RI_mod;
[imp_mod,t_mod] = impz(RI_mod,1,length(RI_mod),Fs);
[h_mod,f_mod]=freqz(RI_mod,1,10000,Fs);
mag_mod=10*log10(abs(h_mod));
figure
plot(t_mod,imp_mod),grid on, title 'Respuesta al impulso modelo'
xlabel('Time (s)')
ylabel('Magnitude')
figure
freqz(RI_mod,1,10000,Fs), title 'Respuesta en frecuencia modelo'
%%
figure
plot(t_amp,imp_amp)
hold on
plot(t_mod,imp_mod,'-o')
grid on
legend('Amplificador','Emulación')
title('Respuesta al impulso Amplificador vs Emulación')
xlabel('Time (s)')
ylabel('Magnitude')

figure
plot(f_amp,mag_amp)
hold on
plot(f_mod,mag_mod)
grid on
legend('Amplificador','Emulación')
title('Respuesta en frecuencia Amplificador vs Emulación')
xlabel('Frequency (Hz)')
ylabel('Magnitude')
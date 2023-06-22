clear;
clc;
%%
Fs=48000;
RI_folder="RI's/SACCO BODY IRs";
[RI,~]=audioread(RI_folder+'/'+'ACC LAVA NYLON 01 MONO.wav');
RI=RI(:,1);
numerator=RI;
denominator=1;
[imp_orig,t_orig] = impz(numerator,denominator,length(numerator),Fs);
figure
plot(t_orig,imp_orig),grid on
[h_orig,f_orig]=freqz(numerator,denominator,10000,Fs);
mag_orig=10*log10(abs(h_orig));
disp(length(imp_orig)/Fs)
H=fft(RI);
H=H(1:length(H)/2);
H_mag=10*log10(abs(H));
f=0:(Fs/(2*length(H))):(Fs/2)-(Fs/(2*length(H)));
figure
plot(f,H_mag), grid on
xlabel('Frequency (Hz)')
ylabel('Magnitude (dB)')
%%
fcL=18;
fcH=295;
w1=[ones(1,fcL) zeros(1,length(H)-fcL)];
w2=[zeros(1,fcL) ones(1,fcH-fcL) zeros(1,length(H)-fcH)];
w3=[zeros(1,fcH) ones(1,length(H)-fcH)];
figure
semilogx(f,H_mag), grid on
hold on
semilogx(f,10*log10(w1))
hold on
semilogx(f,10*log10(w2))
hold on
semilogx(f,10*log10(w3))
xlabel('Frequency (Hz)')
ylabel('Magnitude (dB)')
legend('Respuesta','Zona de baja frecuencia','Zona de media frecuencia','Zona de alta frecuencia')
%%
H1=H.*w1';
H2=H.*w2';
H3=H.*w3';
figure
semilogx(f,10*log10(abs(H1)))
hold on
semilogx(f,10*log10(abs(H2)))
hold on
semilogx(f,10*log10(abs(H3)))
grid on
xlabel('Frequency (Hz)')
ylabel('Magnitude (dB)')
legend('Zona de baja frecuencia','Zona de media frecuencia','Zona de alta frecuencia')
%%
h=ifft(H);
h1=ifft(H1);
h2=ifft(H2);
h3=ifft(H3);
t=0:(2/Fs):((2/Fs)*length(real(h)))-(2/Fs);
figure
plot(t,real(h))
hold on
plot(t,real(h1))
hold on
plot(t,real(h2))
hold on
plot(t,real(h3))
xlabel('Time (s)')
ylabel('Magnitude')
grid on
legend('Respuesta total','Respuesta debida a bajas frecuencias','Respuesta debida a medias frecuencias','Respuesta debida a altas frecuencias')
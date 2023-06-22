clear;
clc;
%% PARAMETROS
unknown_system=0;  %0:real RI; 1:fixed_FIR; 2:fixed_IIR; 3:analog_filter
Fs=48000; %Frecuencia de muestreo
if unknown_system==0
    RI_folder="RI's/Ampeg_V4B_Marshall_4x12";
    RI_filename='Ampeg V4-B_marshal_sm7.wav';
end

%Parametros del filtro adaptativo
mu = 0.0001; %Coeficiente de convergencia
order=128;  %Orden deseado del filtro adaptativo
lms = dsp.LMSFilter(order,'StepSize',mu,'Method','Normalized LMS'); %Objeto del filtro (tipo NMLS)

%Parametros de simulacion
trainSeconds=100; %Segundos que el sistema estará en fase de adaptación
%% Sistema desconocido
switch unknown_system
    case 0 %real_RI
        [RI,~]=audioread(RI_folder+'/'+RI_filename);
        RI=RI(:,1);
        numerator=RI;
        denominator=1;
        [imp_orig,t_orig] = impz(numerator,denominator,length(numerator),Fs);
        [h_orig,f_orig]=freqz(numerator,denominator,10000,Fs);
        mag_orig=10*log10(abs(h_orig));
    case 1 %fixed_FIR
        n=200; %Orden del filtro FIR
        Fc=1000; %Frecuencia de corte
        Wn=Fc/(Fs/2); %Frecuencia de corte normalizada
        numerator=fir1(n,Wn,'low'); %Coeficientes FIR
        denominator=1; %Denominador unitario (por ser FIR)
        [imp_orig,t_orig] = impz(numerator,denominator,length(numerator),Fs);
        [h_orig,f_orig]=freqz(numerator,denominator,10000,Fs);
        mag_orig=10*log10(abs(h_orig));
    case 2 %fixed_IIR
        n=5; %Orden del filtro IIR
        Fc=1000; %Frecuencia de corte
        Wn=Fc/(Fs/2); %Frecuencia de corte normalizada
        [numerator,denominator] = butter(n,Wn); %Diseño filtro butterworth
        [imp_orig,t_orig] = impz(numerator,denominator,1000,Fs);
        [h_orig,f_orig]=freqz(numerator,denominator,10000,Fs);
        mag_orig=10*log10(abs(h_orig));
    case 3 %analog_filter
        n=5; %Orden del filtro
        Fc=1000; %Frecuencia de corte
        Wc = 2*pi*Fc; %Frecuencia de corte angular
        [numerator,denominator] = butter(n,Wc,'s');
        sys = tf(numerator,denominator); %Función de transferencia del filtro
        [imp_orig_un,t_orig]=impulse(sys);
        figure
        plot(t_orig,imp_orig_un),grid on, title 'Respuesta al impulso sistema desconocido'
        xlabel('Time (s)')
        ylabel('Magnitude')
        W=0:((pi*Fs)/10000):(pi*Fs);
        f_orig=W/(2*pi);
        h=freqs(numerator,denominator,W);
        mag_orig = 10*log10(abs(h));
        phase = angle(h);
        phasedeg = phase*180/pi;
        figure
        subplot(2,1,1)
        plot(f_orig,mag_orig), grid on, title 'Respuesta en frecuenciaistema desconocido'
        xlabel('Frequency (Hz)')
        ylabel('Magnitude')
        subplot(2,1,2)
        plot(W/(2*pi),phasedeg), grid on
        xlabel('Frequency (Hz)')
        ylabel('Phase (degrees)')
    otherwise
        disp('Error')
        return
end

if unknown_system~=3
    figure
    plot(t_orig,imp_orig),grid on, title 'Respuesta al impulso sistema desconocido'
    xlabel('Time (s)')
    ylabel('Magnitude')
    figure
    freqz(numerator,denominator,10000,Fs), title 'Respuesta en frecuencia Sistema desconocido'
end
%% ADAPTACIÓN
%Señal de entrada (Ruido blanco)
x = randn(trainSeconds*Fs,1); %Ruido blanco

%Señal deseada (salida del sistema desconocido)
if unknown_system~=3
    d = filter(numerator,denominator,x);
else
    t=0:(1/Fs):((1/Fs)*length(x))-(1/Fs); %Vector de tiempo
    d=lsim(sys,x,t);
end

%Salida del filtro adaptativo (y:salida; e:error; w:coeficientes; d:deseada)
[y,e,w] = lms(x,d); %Entrenamiento del filtro adaptativo
t=0:(1/Fs):((1/Fs)*length(y))-(1/Fs); %Vector de tiempo
figure
plot(t,[d,y,e])
title('Etapa de adaptacion')
legend('Deseada','Salida','Error')
xlabel('t [s]')
ylabel('Valor')
figure
freqz(w,1,10000,Fs), title 'Filtro adaptado'
figure
stem(w), title 'Coeficientes finales', grid on

%Error cuadrático medio
timeMSE=mse(e); %Cálculo de la variación del error cuadrático medio
t=0:(1/Fs):((1/Fs)*length(y))-(1/Fs); %Vector de tiempo
figure
plot(t,timeMSE), title 'Error cuadratico medio', grid on
xlabel('t [s]')
ylabel('MSE')

%MSE calculado en bloque
blockSize=512;
timeMSE_block=block_mse(e,blockSize); %Cálculo de la variación del error cuadrático medio por bloques (Similar a la implementación práctica)
t=0:(blockSize/Fs):((blockSize/Fs)*length(timeMSE_block))-(blockSize/Fs); %Vector de tiempo
figure
plot(t,timeMSE_block), title 'Error cuadratico medio estimado', grid on
xlabel('t [s]')
ylabel('MSE')

%% Comparacion de filtros
[imp_model,t_model] = impz(w,1,length(w),Fs);
[h_model,f_model]=freqz(w,1,10000,Fs);
mag_model=10*log10(abs(h_model));

if unknown_system==3
    scale_value=max(abs(imp_model))/max(abs(imp_orig_un));
    imp_orig=scale_value*imp_orig_un;
end

figure
plot(t_orig,imp_orig)
hold on
plot(t_model,imp_model,'-o')
grid on
legend('Sistema original','Modelo obtenido')
title('Respuesta al impulso sistema vs modelo')
xlabel('Time (s)')
ylabel('Magnitude')

figure
plot(f_orig,mag_orig)
hold on
plot(f_model,mag_model)
grid on
legend('Sistema original','Modelo obtenido')
title('Comparativa en frecuencia sistema vs modelo')
xlabel('Frequency (Hz)')
ylabel('Magnitude')

%% Algoritmo de ventaneo (no implementado)
[~,I]=max(abs(w));
wind1=ones(I,1);
%wind2=blackman(2*(order-I));
wind2=hamming(2*(order-I));
wind2=wind2(order-I+1:2*(order-I));
wind_as=[wind1; wind2];
w_windowed=w.*wind_as;

figure
stem(wind_as),title 'Ventana aplicada', grid on
figure
stem(w_windowed), title 'Coeficientes con ventaneo', grid on

[imp_model_wind,t_model_wind] = impz(w_windowed,1,length(w_windowed),Fs);
[h_model_wind,f_model_wind]=freqz(w_windowed,1,10000,Fs);
mag_model_wind=10*log10(abs(h_model_wind));

figure
plot(t_orig,imp_orig)
hold on
plot(t_model,imp_model,'-o')
hold on
plot(t_model_wind,imp_model_wind,'-*')
grid on
legend('Sistema original','Modelo obtenido','Modelo con ventaneo')
title('Respuesta al impulso sistema vs modelo')
xlabel('Time (s)')
ylabel('Magnitude')

figure
plot(f_orig,mag_orig)
hold on
plot(f_model,mag_model)
grid on
plot(f_model_wind,mag_model_wind)
grid on
legend('Sistema original','Modelo obtenido','Modelo con ventaneo')
title('Comparativa en frecuencia sistema vs modelo')
xlabel('Frequency (Hz)')
ylabel('Magnitude')

%% FILTRADO
%% Señal de prueba original
[audio,~]=audioread('prueba.wav');
audio1=audio(:,1);
t=0:(1/Fs):((1/Fs)*length(audio1))-(1/Fs);
figure
plot(t,audio1),grid on, title 'Señal de prueba original'
xlabel('t (s)')
sound(audio1,Fs);
%% Señal pasada por el sistema desconocido
if unknown_system~=3
    salidaSist = filter(numerator,denominator,audio1); 
else
    salidaSist=lsim(sys,audio1,t);
end
t=0:(1/Fs):((1/Fs)*length(salidaSist))-(1/Fs);
figure
plot(t,salidaSist),grid on, title 'Señal de salida (sistema desconocido)'
xlabel('t (s)')
valormax=max(abs(salidaSist));
salidaSistA=salidaSist/valormax;
sound(salidaSistA,Fs);
%% Señal modelo
salidaModelo = filter(w,1,audio1); 
t=0:(1/Fs):((1/Fs)*length(salidaModelo))-(1/Fs);
figure
plot(t,salidaModelo),grid on, title 'Señal de salida (modelo)'
xlabel('t (s)')
valormax=max(abs(salidaModelo));
salidaModeloA=salidaModelo/valormax;
sound(salidaModeloA,Fs);
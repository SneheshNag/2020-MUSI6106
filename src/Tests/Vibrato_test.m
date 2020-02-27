
[x, fs] = audioread("MajorTom16.wav");
[audio_cpp, fs] = audioread("MajorTom16_vibrato.wav");

delay = fs*0.1;
width = fs*0.05;
mod_freq = 10.0/fs;

audio_mat = vibrato(x,fs,modfreq,width); % vibrato.m MATLAB
sound(y_mat, fs);

MSE = immse(audio_cpp, audio_mat); % mean square error

figure(1);
hold on
subplot(3,1,1);
plot(audio_mat);
title('MATLAB');

subplot(3,1,2);
plot(audio_cpp);
title('C++');

subplot(3,1,3);
plot(audio_cpp - audio_mat);
title('Difference');


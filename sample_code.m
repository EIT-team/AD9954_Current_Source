clear all
fclose(instrfind)
CS = serial('COM21','BaudRate',115200);

fopen(CS);
pause(1);
%%
freq = 5000;
inj_time = 1;
freq_string = ['frequency ' num2str(freq)];

fwrite(CS,freq_string)
pause(0.1)
fwrite(CS,'stim 1')

pause(inj_time)
fwrite(CS,'stim 0')
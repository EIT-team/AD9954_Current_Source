% Create serial port object
CS = serial('COM21','BaudRate',115200);
fopen(CS);
pause(2); %Need a pause to allow time for the port to open 

% Define frequency (Hz) and injection time (s)
freq = 5000;
inj_time = 1;

freq_string = ['frequency ' num2str(freq)];

fwrite(CS,freq_string)
pause(0.1)

fwrite(CS,'stim 1')
pause(inj_time)
fwrite(CS,'stim 0')
% Requires ScouseTom library in Matlab!

% this just cleans up any old arduino connections
fclose(instrfind)

%get example ExpSetup with all of the variables 
load('ExpSetupExample.mat','ExpSetup');

%change the variables that are important
ExpSetup.Amp=1; % THIS IS NOT USED! Setting it low to avoid warnings later
ExpSetup.Freq =100; %freq in hz
ExpSetup.MeasurementTime = 500; %time for each injection pair in ms
ExpSetup.Repeats = 10; % number of times to repeat protocol below before stopping
ExpSetup.Elec_num = 32; % number of electrodes connected > 32 will make the system use two boards
ExpSetup.Protocol = [1 6; 4 18; 21 28]; %order of injection pairs Nx2 left column is source+, and left sink -

%% initialise connection with arduino

%should only get black text fininshing with "initialisation finished"
[Ard]=ScouseTom_Init('COM3');
%communication errors - check COM port string is correct. reconnect arduino
%to computer. Does it work in the Arduino IDE serial monitor?

%send the settings to the arduino, should be all black text "Ardunio is
%checking settings...Settings OK! Ready to inject!!!" 
[Ard,ExpSetup]=ScouseTom_SendSettings(Ard,ExpSetup);

%% Do injection

%  [Ard,ExpSetup]=ScouseTom_Start(Ard,ExpSetup,1);
[Ard,ExpSetup]=ScouseTom_Start(Ard,ExpSetup);

%% Do contact check

[Ard]=ScouseTom_ContactCheck(Ard,ExpSetup);

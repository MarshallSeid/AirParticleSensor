#include <SPI.h>
#include <String.h>
/* Author: Marshall Seid and Hung Ngyuyen 
 * Prototyping Lab Calit2
 * Date: May 30th, 2015
 * Sources of help: Frank Cardone
 */
 
//Constants for Command
const int sampleSeconds = 10;
const int ESC = 27;
const int OPERATE_MODE = 1; //Stop:0 Start:1 Starting:2 Count:3
const int _C = 67;
const int _N = 78;
const int _O = 79;
const int _R = 82;
const int _S = 83;
const int _T = 84;
const int CR = 13;
const int LF = 10;
const int PARSE_SIZE = 20;
const int ZERO = 0;

//LED Transfer Constants
const int slaveSelectPin = 10; 
const int clockPin = 13;
const int numOfLEDs = 288; //Based on 2 LED Strings (Large/Small particles)

const int BLUE_BIG = 255;
const int GREEN_BIG = 0;
const int RED_BIG = 0;
const int INTENSITY_BIG= 12;

const int BLUE_SMALL = 0;
const int GREEN_SMALL = 0;
const int RED_SMALL = 0;
const int INTENSITY_SMALL = 0;
const int INTENSITY = 2;

byte setting[] = {ESC, _R, _S, CR}; //<ESC> RS <CR>
byte getSampleTime[] = {ESC, _S, _T, CR}; //<ESC>ST<CR>
byte setSampleTime[] = {ESC, _S, _T, 49, 48, 48, CR}; //<ESC>ST d<CR> 1 (49), 0 (48)
byte setNumSamples[] = {ESC, _S, _N, 50, 48, 48, CR}; //Set to 200
byte setOpMode[] = {ESC, _S, _O, OPERATE_MODE, CR};
byte setReportType[] = {ESC, _S, _R, 16, CR}; //0, 0 Raw counts, report counts when sample is completed
byte reportSamples[] = {ESC, _R, _C, CR};
char output1_c[PARSE_SIZE]; //FTS
char output2_c[PARSE_SIZE];
char filter[PARSE_SIZE];
char buf[PARSE_SIZE];
char output1_toI[PARSE_SIZE];
char output2_toI[PARSE_SIZE];

int currLeds;
void setup() {
  /* Setting up LED */
   Serial.begin(9600); //Set Baud rate at 9600   **Need this for serial transfer
  // set the slaveSelectPin as an output:
  pinMode (slaveSelectPin, OUTPUT); 
  //initialize SPI;
  SPI.begin();
  pinMode (clockPin, OUTPUT);
  SPI.setDataMode(SPI_MODE3);
  
 
  while (!Serial) Serial.print("Serial isn't working");
  Serial.write(setSampleTime, sizeof(setSampleTime)); //Set Sample time
  Serial.write(setNumSamples, sizeof(setNumSamples)); //Set number of samples to infinity
  Serial.println("Set Num samples");
  //Setting report type to 0, 0 and seeing if theres a change in the report settings
  Serial.write(setReportType, sizeof(setReportType));
  Serial.println("Set report type to 0: Report Raw Counts, 0: Report counts when sample is completed");
  while(Serial.available() > 0) byte b = Serial.read();
  //Serial.write(setting, sizeof(setting)); //Print out report settings once
  Serial.write(getSampleTime, sizeof(getSampleTime));
  Serial.println("Finished reporting settings");
  } 
  
void loop() {
  Serial.println();
  memset(output1_c, 0, sizeof(output1_c)); //Reset buffers each loop
  memset(output2_c, 0, sizeof(output2_c));
  memset(filter, 0, sizeof(filter));
  Serial.write(reportSamples, sizeof(reportSamples)); //Write the samples

  Serial.readBytesUntil(' ', filter, 20); //Filters out RC of resulting command
  Serial.readBytesUntil(',', output1_c, 20); //Output1 holds large particle count
  Serial.readBytesUntil('\n', output2_c, 20); //Output2 holds small particle count
 
  while(Serial.available() > 0 ) byte a = Serial.read(); //Flush buffer
  
  int output1 = atoi(output1_c); //Convert char arrays to ints
  int output2 = atoi(output2_c);
  Serial.print("*");
  Serial.print(output1); //Output1 
  Serial.print("@");
  Serial.print(output2); //Output2
  Serial.print("%");
  
   /* Using LED's */
  digitalWrite(slaveSelectPin, LOW);
  //Start frame: 4 bytes of 0 
  for (int start = 0; start < 4; start++) SPI.transfer(0);
  //Standard Input for RGB + Intensity
      for (int l = 0; l < numOfLEDs; l++) {
         int randomRed = 0;
         int randomGreen=255;
         int randomBlue = 0;
         if (l < output1) {
         SPI.transfer(randomRed);
         SPI.transfer(randomBlue);
         SPI.transfer(randomGreen);
         SPI.transfer(INTENSITY);
         }
         else {
            SPI.transfer(0);
         SPI.transfer(255);
         SPI.transfer(0);
         SPI.transfer(20);
         }
       }     
   //End Frame - # of LEDs /2: bytes of 0 
   for (int endF = 0; endF < numOfLEDs/2; endF++) SPI.transfer(0); 
   // take the SS pin high to de-select the chip:
   digitalWrite(slaveSelectPin, HIGH); 

}



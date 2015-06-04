/* Author: Marshall Seid
 * Prototyping Lab Calit2
 * Date: May 30th, 2015
 * Sources of help: Frank Cardone
 */
const int sampleSeconds = 100;
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

byte setting[] = {ESC, _R, _S, CR}; //<ESC> RS <CR>
byte getSampleTime[] = {ESC, _S, _T, CR}; //<ESC>ST<CR>
byte setSampleTime[] = {ESC, _S, _T, sampleSeconds, CR}; //<ESC>ST d<CR>
byte setNumSamples[] = {ESC, _S, _N, 0, CR}; //Set to 0
byte setOpMode[] = {ESC, _S, _O, OPERATE_MODE, CR};
byte setReportType[] = {ESC, _S, _R, ZERO, CR}; //0, 0 Raw counts, report counts when sample is completed
byte reportSamples[] = {ESC, _R, _C, CR};
char output1_c[PARSE_SIZE]; //FTS
char output2_c[PARSE_SIZE];
char buf[PARSE_SIZE];
void setup() {
  
  Serial.begin(9600); //Set Baud rate at 9600    
  while (!Serial) Serial.print("Serial isn't working");
  delay(10000);
  //Setting report type to 0, 0 and seeing if theres a change in the report settings
  Serial.write(setReportType, sizeof(setReportType));
  Serial.println("Set report type to 0: Report Raw Counts, 0: Report counts when sample is completed");
  while(Serial.available() >0) byte b = Serial.read();
  Serial.write(setting, sizeof(setting)); //Print out report settings once
  while(Serial.available() > 0) {     
     Serial.readBytesUntil('\n', buf, 15);
     for (int i = 0; i < 20; i++) Serial.print(buf[i]);
     Serial.print('\n');
  }
  Serial.println("Finished reporting settings");
  Serial.write(setSampleTime, sizeof(setSampleTime));
  Serial.println("Set Sample time to 10 seconds");
  } 
void loop() {
  Serial.println("LOOP:");
  memset(output1_c, 0, sizeof(output1_c));
  Serial.write(reportSamples, sizeof(reportSamples)); //Write the samples
  Serial.readBytesUntil('\n', output1_c, 20);
  
/* Serial.readBytesUntil(',', output1_c, 20);
  long toReturn = atol(output1_c);
  Serial.readBytesUntil('\n', output2_c, 20);
  long toReturn2 = atol(output2_c); 
  Serial.print(toReturn);
  Serial.print(toReturn2);
  */
  int index2 = 0;
  while(Serial.available() > 0 ) { 
   byte a = Serial.read();
   if (a != '\n' && a != '\r') {
     output2_c[index2] = a;
    index2++;
   }
  }
  Serial.print(' ');
  for (int i = 0; i < 20; i++) Serial.print(output1_c[i]);
  Serial.print(' ');
  for (int j = 0; j < 20; j++) Serial.print(output2_c[j]);
  Serial.print(' ');
  delay(2000);
}

/* int getSamples(): This method will get the samples from the particle sensor
 *              and convert them into data usable for SPI transfer to LED string
long getSamples() {
 Serial.write(reportSamples, sizeof(reportSamples)); //Write the samples
 char output1_c[PARSE_SIZE]; //FTS
 char output2_c[PARSE_SIZE];
 char inChar; //Store char
 Serial.readBytesUntil(',', output1_c, 20);
 long toReturn = atol(output1_c);
 Serial.readBytesUntil('\n', output2_c, 20);
 long toReturn2 = atol(output2_c);
 return toReturn;
 }      


/*static int parse_counts(char *input, unsigned int *output1, unsigned int *output2)
{
  char output1_c[20];
  char output2_c[20];
  for (int i = 0; i < 20; i++) {
    if (input[i] = ',') {
      output1_c[i] = '\0';
    } 
  } 
  return 0;
}
//Helper method to convert string to longs
long stringToLong(String s) {
    char arr[12];
    s.toCharArray(arr, sizeof(arr));
    return atol(arr);
}*/

/*
 * Sketch Creation Date:  June 14, 2017
 * Created By:  Tom Gambill
 * This sketch was created for the purppose of testing my specific 
 * Infineon Motor Driver SHield TLE94112EL V1.3 for Arduino
 * for evaluation for possible defects due to a narrow
 * voltage operating range of approximately 5.0 to 6.1 Volts.
 * 
 * This sketch Activates motors connected between OUT1-2 and Out3-4 for 1 reapeating cycle
 * which performs the following:
 * 1.  Activates motors for 2 seconds in one direction
 * 2.  Brakes motors to ground for 300ms
 * 3.  Activates motors for 2 seconds in the opposite direction
 * 4.  Brakes motors again for 300ms
 * 
 * 5.  The Open Load (LE) and Overcurrent (VS_OV) for Out1-4 registers are read back 
 *     during each activation/deactivation; and 
 *     The Open Load (LE) and Overcurrent (VS_OV) registers for Out1-4 are cleared at the end
 *     of each cycle.
 *     
 * Data for the motor type, power supply used, voltage, and current will be provided separately
 * accompanying the documentation of the test so as to keep this remarks section generic 
 * for potential future use.
 */

 #include <SPI.h> // include SPI library

 const int EN = 8;                       // Set pin 8 to control Enable pin of TLE94112
 const int CSN = 10;                     // Set pin 10 (CSN1) to control CSN of TLE94112 (default)

const byte HB_ACT_1_CTRL = B0000011;    // Control Register for Activation/Deactivation of 
                                        // OUT1-2 and OUT3-4

//Addresses of the Status Registers Used
const byte SYS_DIAG_1 = B00011011;      // Global Status Register
//BitMapping:  D7=SPI_ERR D6=LE D5=VS_UV D4=VS_OV D3=NPOR D2=TSD D1=TPW D0=0(reserved)

const byte SYS_DIAG_2 = B01011011;      // Overcurrent HB1-4
//BitMappping:  D7=HB4_HS_OC D6=HB4_LS_OC D5=HB3_HS-OC D4=HB3-LS-OC D3=HB2_HS-OC D2=HB2_LS-OC D1= HB1_HS_C D0=HB1_LS_OC

const byte SYS_DIAG_5 = B00000111;      // Open load HB1-4
//BitMapping:  D7=HB4_HS-OL D6=HB4_LS_OL D5=HB3_HS_OL D4=HB3_LS_OL D3=HB2_HS_OL D2=HB2_LS_OL D1=HB1_HS_OL D0=HB1_LS_OL

const byte WRITE = 0b10000000; // Mask used for data byte to WRITE commands to control registers in Activating functions
const byte CLEAR = 0b10000000; // Mask used for data byte CLEAR commands (functions) in status registers 


void setup() {
  // Setup the parameters for SPI communication, set CSN and EN pins as outputs and set EN to High  
  Serial.begin(9600);                     // sets the baud rate for communication with the computer to 9600 bauds
  SPI.begin();                            // Initializes the SPI bus
  SPI.setBitOrder(LSBFIRST);              // Least significant bit is sent first 
  SPI.setClockDivider(SPI_CLOCK_DIV16);   // SPI clock frequency set to 1 MHz
  SPI.setDataMode(SPI_MODE1);             // Set SPI clock polarity and clock phase; CPOL = 0, CPHA = 1
  pinMode(CSN, OUTPUT);                   // sets CSN as output
  pinMode(EN, OUTPUT);                    // sets EN as output
  digitalWrite(EN, HIGH);                 // TLE94112 set in active mode (ENABLE pin = high)
}


void loop() {
  /*NOTE:  to make deugging easier, several Serial Print commands will be
    used to document commands throughput this program*/

    Serial.println();
    Serial.println("Activate Motors in one direction--HS4, LS3, HS2, LS1 Enabled");
    writeRegister(HB_ACT_1_CTRL, 0b10011001); // HB4_HS_EN, 0, 0, HB3_LS_EN, HB2_HS_EN, 0, 0, HB1_LS_EN

    Serial.println();
    Serial.println("Read Over Current Register SYS_DIAG_2");
    readRegister(SYS_DIAG_2);  //Read Diagnosytic Register for Over Current Condition
   
    Serial.println();
    Serial.println("Read Open Load Register SYS_DIAG_5");
    readRegister(SYS_DIAG_5);  //Read Diagnosytic Register for Open Load Condition
    
    Serial.println();
    Serial.println("Delay--allow motors to run--for 2 seconds (2000ms)");
    delay(2000);  //Allow motors to run for 2 seconds

    Serial.println("Brake for 300ms");
    Serial.println();
    writeRegister(HB_ACT_1_CTRL, 0b01010101); // ENable LS 4, 3, 2, 1, to brake motors to ground.
    delay(300);  //Hold brake for 300 milliseconds

    Serial.println();
    Serial.println("Read Over Current Register SYS_DIAG_2");
    readRegister(SYS_DIAG_2);  //Read Diagnosytic Register for Over Current Condition
    
    Serial.println();
    Serial.println("Read Open Load Register SYS_DIAG_5");
    readRegister(SYS_DIAG_5);  //Read Diagnosytic Register for Open Load Condition
  
    Serial.println(); 
    Serial.println("Activate Motors in OPPOSITE direction--LS4, HS3, LS2, HS1 Enabled");
    writeRegister(HB_ACT_1_CTRL, 0b01100110); //0, HB4_LS_EN, HB3_HS_EN, 0, 0, HB2_LS_EN, HB1_HS_EN, 0
    
    
    Serial.println();  
    Serial.println("Read Over Current Register SYS_DIAG_2");
    readRegister(SYS_DIAG_2);  //Read Diagnosytic Register for Over Current Condition
   

    Serial.println(); 
    Serial.println("Read Open Load Register SYS_DIAG_5");
    readRegister(SYS_DIAG_5);  //Read Diagnosytic Register for Open Load Condition
    
    Serial.println();
    Serial.println("Delay--allow motors to run--for 2 seconds (2000ms)");
    delay(2000);  //Allow motors to run for 2 seconds

    Serial.println("Brake for 300ms");
    Serial.println();
    writeRegister(HB_ACT_1_CTRL, 0b01010101); // ENable LS 4, 3, 2, 1, to brake motors to ground.
    delay(300);  //Hold brake for 300 milliseconds

    Serial.println();
    Serial.println("Read Over Current Register SYS_DIAG_2");
    readRegister(SYS_DIAG_2);  //Read Diagnosytic Register for Over Current Condition
    
    Serial.println();
    Serial.println("Read Open Load Register SYS_DIAG_5");
    readRegister(SYS_DIAG_5);  //Read Diagnosytic Register for Open Load Condition

     Serial.println();
    Serial.println("Clear Over Current and Over Load Registers");
    Serial.println("SYS_DIAG_2 and SYS_DIAG_5");
    
    clearRegister(SYS_DIAG_2);
    clearRegister(SYS_DIAG_5);
    //end of cycle--repeat
    }
    int writeRegister(byte address, byte data)
{
/*   This function sends SPI commands to the address  "address" with the data "data"
 *   The data sent by the TLE94112EL to the Arduino Uno (SDO) and the data sent by the Arduino Uno to the TLE94112EL (SDI) are reported in the
 *   Serial Monitor (CTRL + SHIFT + M)
 */
  address = address | WRITE ;
  digitalWrite(CSN, LOW);               // take the CSN pin low to select the chip:
  byte byte0 = SPI.transfer(address);   // send address byte to the TLE94112 and store the received byte to byte 0
  byte byte1 = SPI.transfer(data);      // send data byte to the TLE94112 and store the received byte to byte 1
  digitalWrite(CSN, HIGH);              // take the CSN pin high to terminate the SPI frame
  //Serial.println();                     // insert a carriage return to the serial monitor

  Serial.print("SDI:\t");
  printByte(address);                   // write address to the serial monitor
  printByte(data);                      // write data to the serial monitor
  Serial.print("SDO:\t");
  printByte(byte0);                     // write byte0 (Global Satus Register of the TLE94112EL) to the serial monitor
  printByte(byte1);                     // write byte1 (Data Byte sent by the TLE9412EL) to the serial monitor
  Serial.println();
  int result = ( ((int)byte0 << 8) | (int)byte1 ); // concatenates byte0 and byte1 in result

  return (result);
}

int readRegister(byte address)          // This function reads a register and returns the data sent by the TLE94112EL. Data sent and received by the TLE94112EL are displayed on the serial monitor
{
  digitalWrite(CSN, LOW);               // take the CSN pin low to select the chip:
  byte byte0 = SPI.transfer(address);   // send Low Byte
  byte byte1 = SPI.transfer(0);         // send High Byte
  digitalWrite(CSN, HIGH);              // take the CSN pin high to de-select the chip:
  //Serial.println();
  
  Serial.print("SDI:\t");
  printByte(address);                   // write address to the serial monitor
  printByte(0);                         // write data to the serial monitor
  Serial.print("SDO:\t");
  printByte(byte0);                     // write byte0 (Global Satus Register of the TLE94112EL) to the serial monitor
  printByte(byte1);                     // write byte1 (Data Byte sent by the TLE9412EL) to the serial monitor
  Serial.println();
  int result = ( ((int)byte0 << 8) | (int)byte1 ); // concatenates byte0 and byte1 in result
  return (result);
}

int clearRegister(int address)          // This function clears a register and returns the data sent by the TLE94112EL. Data sent and received by the TLE94112EL are displayed on the serial monitor
{
  address = address | CLEAR ;
  digitalWrite(CSN, LOW);               // take the CSN pin low to select the chip:
  byte byte0 = SPI.transfer(address);
  byte byte1 = SPI.transfer(0);
  digitalWrite(CSN, HIGH);              // take the CSN pin high to de-select the chip:
  //Serial.println();
  
  Serial.print("SDI:\t");
  printByte(address);                   // write address to the serial monitor
  printByte(0);                         // write data to the serial monitor
  Serial.print("SDO:\t");               
  printByte(byte0);                     // write byte0 (Global Satus Register of the TLE94112EL) to the serial monitor
  printByte(byte1);                     // write byte1 (Data Byte sent by the TLE9412EL) to the serial monitor
  Serial.println();
  int result = ( ((int)byte0 << 0) | (int)byte1 ); // concatenates byte0 and byte1 in result
  return (result);
}

void printByte(byte printByte)          // This function writes to the serial monitor the value of the byte printByte in binary form 
{
  for (int i = 7; i >= 0; i--)
  {
    Serial.print(bitRead(printByte, i));
  }
  Serial.print("\t");
}

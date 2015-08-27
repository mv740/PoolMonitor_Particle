// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_ST7735/Adafruit_ST7735.h"

// Use this include for the Web IDE:
#include "OneWire/OneWire.h"



// https://github.com/Hotaman/OneWireSpark
// https://github.com/adafruit/Adafruit-ST7735-Library
//
/*


Colors : some are inverse while using a different lcd ( ebay imitation ) 
ST7735_BLACK     0x0000
ST7735_BLUE      0x001F
ST7735_RED       0xF800
ST7735_GREEN     0x07E0
ST7735_CYAN      0x07FF
ST7735_MAGENTA   0xF81F
ST7735_YELLOW    0xFFE0 
ST7735_WHITE     0xFFFF


*/


#define cs   A2
#define dc   D6
#define rst  D2 

Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst); // hardware spi

// char array to print to the screen
char sensorPrintout[6];
char celsiusPrint[6];
char fahrenheitPrint[6];
float celsius, fahrenheit;
//for spark Cloud
double sparkCelcius;

// Define the pins we will use
int ow = D0;    // put the onewire bus on D0

OneWire  ds(ow);  // a 1 - 4.7K resistor to 3.3v is necessary

void setup(void) {
    
    //screen
	tft.initG();
     
	tft.fillScreen(ST7735_BLACK);

    tft.setFont(ARIAL_8);
    tft.setTextColor(ST7735_BLUE); //red and blue are inverse in some imitation lcd 
    tft.setTextSize(2);
    
    tft.println("temperature"); //Temperature Weather 
    tft.drawFastHLine( 0, 25, tft.width()-1, ST7735_WHITE );

    
    //cloud variable
    Spark.variable("Temperature", &sparkCelcius, DOUBLE);

    
    //for debugging
    Serial.begin(57600);  // local hardware test only
    
   
    
}



void loop(void) {
    
    
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
 
  
  /*
  if ( !ds.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
  
  Serial.print("ROM =");
  for( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
  Serial.println();
 
  // the first ROM byte indicates which chip
  // includes debug output of chip type
  switch (addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    case 0x26:
      Serial.println("  Chip = DS2438");
      type_s = 2;
      break;
    default:
      Serial.println("Device is not a DS18x20/DS1822/DS2438 device. Skipping...");
      return;
  } 

    */
    // addr = 
    ds.search(addr);
    type_s =0;
    
  ds.reset();
  ds.select(addr);      // Just do one at a time for testing
                        // change to skip if you already have a list of addresses
                        // then loop through them below for reading
                        
  ds.write(0x44);        // start conversion, with parasite power on at the end
  
  delay(25);     // maybe 750ms is enough, maybe not, I'm shooting for 1 reading per second
                    // prob should set to min reliable and check the timer for 1 second intervals
                    // but that's a little fancy for test code
             
				// delay must be around 800ms-1000ms so it doesn't give error like -0.006 every 10 reading
				// in this project, I use a big delay between print and delete (output to screen -> line 232)
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE, 0);         // Read Scratchpad 0

  Serial.print("  Data = ");
  Serial.print(present, HEX);
  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(" CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    if (type_s==1) {    // DS18S20
      raw = raw << 3; // 9 bit resolution default
      if (data[7] == 0x10) {
        // "count remain" gives full 12 bit resolution
        raw = (raw & 0xFFF0) + 12 - data[6];
      }
      celsius = (float)raw / 16.0;
    }else{ // type_s==2 for DS2438
      if (data[2] > 127) data[2]=0;
      data[1] = data[1] >> 3;
      celsius = (float)data[2] + ((float)data[1] * .03125);
    }
  } else {  // DS18B20 and DS1822
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
    celsius = (float)raw / 16.0;
    
    //for spark cloud
    sparkCelcius = (double)celsius;
  }
  fahrenheit = celsius * 1.8 + 32.0;
  // end of test code
  
  // debug output
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");
  
  //test screen
  // Read the value of the sensor on A0
  String TempC = String(celsius);
  String TempF = String(fahrenheit);

  // convert the reading to a char array
  TempC.toCharArray(celsiusPrint, 6);
  TempF.toCharArray(fahrenheitPrint, 6);

  // set the font color
  //tft.stroke(255, 255, 255);
  // print the sensor value
  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(10, 50);
  tft.println(celsiusPrint);
  tft.setCursor(80, 50);
  tft.println("C");
  tft.setCursor(10, 100);
  tft.println(fahrenheitPrint);
  tft.setCursor(80,100);
  tft.println("F");
  // wait for a moment
  delay(1000);
  // erase the text you just wrote
  //tft.stroke(0, 0, 0);
    tft.setTextColor(ST7735_BLACK);
    tft.setCursor(10, 50);
    tft.println(celsiusPrint);
    tft.setCursor(10, 100);
    tft.println(fahrenheitPrint);
  
}

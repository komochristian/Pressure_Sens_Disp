#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Declaration for second display
#define SCREEN_ADDRESS2 0x3C
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


String current_pressure = "";
String last_pressure = "";
String current_pressure2 = "";
String last_pressure2 = "";


void setup() {
  //  Initialize Serial port baudrate
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);

  //  SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  //  Initialize display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.display();

  //  SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  //  Initialize display
  if(!display2.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS2)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display2.clearDisplay();
  display2.display();

  //  Initialize pressure units
  // Serial1.write("@253RSD!OFF;FF");
  Serial1.write("@253U!TORR;FF");
  Serial2.write("@253U!TORR;FF");
}

//  Take returned pressure sensor text and trim out data readings only
String parse_string( String data ) {
  int ack_index = data.indexOf("ACK");
  int semicolon_index = data.indexOf(';');
  if (ack_index != -1 && semicolon_index != -1) {
    return data.substring(ack_index + 3, semicolon_index);
  } else {
    return "Invalid";
  }
}

void loop() {
  //  Request pressure data from both sensors
  Serial1.write("@253PR4?;FF");
  Serial2.write("@253PR4?;FF");

  //  Read if pressure sensor 1 is transmitting pressure data
  if (Serial1.available() > 0) {
    current_pressure = parse_string(Serial1.readString());
    last_pressure = current_pressure;
  } 
  else {
    current_pressure = last_pressure;
  }

  //  Read if pressure sensor 2 is transmitting pressure data
  if (Serial2.available() > 0) {
    current_pressure2 = parse_string(Serial2.readString());
    last_pressure2 = current_pressure2;
  } 
  else {
    current_pressure2 = last_pressure2;
  }

  //  Display the pressure of sensor 1 on display 1
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Pressure: " + current_pressure + "  torr");
  display.display();
  Serial.println(current_pressure);

  //  Display the pressure of sensor 2 on display 2
  display2.clearDisplay();
  display2.setTextSize(2);
  display2.setTextColor(WHITE);
  display2.setCursor(0,0);
  display2.println("Pressure: " + current_pressure2 + "  torr");
  display2.display();
  Serial.println(current_pressure2);

  //Update every quarter second
  delay(250);
}

//  Analog test code for MKS Pressure
// read = analogRead(testPort);
  // // Serial.println(read);
  // volt = (read/1023)*10;
  // Serial.println(volt);
  // double torr = pow(10, (volt-6));
  // // Serial.println(torr);
  // delay(1000);

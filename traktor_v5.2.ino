/*
/* pekatonix@gmail.com
Termometru + voltmetru baterie tractor v.1.3
Termometru cu NTC 10K
parametrii termistor (valori din datasheet):
 * RT0: 10 000 Ω
 * B: 3977 K +- 0.75%
 * T0:  25 C
 * +- 5
 * display TFT 1.8" with driver ST7735
 Targu Mures, 26.01.2022

Display color TFT 1.8" SPI driver ST7735:
 * LED =   3.3V  // 15Ω resistor to 5V
 * SCK =   13
 * SDA =   11
 * A0 =    8
 * RESET = 9
 * CS =    10
 * GND =   GND
 * VCC =   5V
*/


#include "Ucglib.h"  // Include Ucglib library to drive the display

#define RT0 10000   // Ω
#define B 3977      // K
#define VCC 5    //Tensiune alimentare 5V
#define R 10000  //R=10KΩ

int V_alarma = 2;  // LED rosu pentru V < 12V || V => 13.9V
int V_normal = 3;  // LED verde pentru V => 12V || V <13.9V
int T_alarma = 4;  // LED rosu pentru TX>92 C

float RT, VR, ln, TX, T0, VRT; // variabile termometru

const int voltageSensorPin = A0;          // sensor tesiune la A0
float vIn;                                // measured voltage (3.3V = max. 16.5V, 5V = max 25V)
float vOut;
float voltageSensorVal;                   // value on pin A0 (0 - 1023)
const float factor = 5.05;               // factorul de corectie senzor/shield temperatura
const float vCC = 5.00;                   // Arduino input voltage (measurable by voltmeter)

// Create display and define the pins:
Ucglib_ST7735_18x128x160_HWSPI ucg(8, 10, 9);  // (A0=8, CS=10, RESET=9)
// The rest of the pins are pre-selected as the default hardware SPI for Arduino Uno (SCK=13 and SDA=11)

//int Variable1;  // Create a variable to have something dynamic to show on the display

void setup(void)  // Start of setup
{
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
	
  T0 = 25 + 273.15; // Temperatura T0 din datasheet, conversie din Celsius in Kelvin

  // Display setup:
  
  // Select a type of text background:
  //ucg.begin(UCG_FONT_MODE_TRANSPARENT);  // It doesn't overwrite background, so it's a mess for text that changes
  ucg.begin(UCG_FONT_MODE_SOLID);  // It writes a background for the text. This is the recommended option
  
  ucg.clearScreen();  // Clear the screen

  // Set display orientation:
  ucg.setRotate90();  // Put 90, 180 or 270, or comment to leave default

  // Set text orientation:
  // It only alters the text, not the entire display
  //ucg.setPrintDir(0);  // 0, 1, 2 or 3 to change orientation of text. If not specify, orientation is 0

  //************* My favorite fonts:
  // Font name: fontgroupx11:
  //ucg.setFont(ucg_font_4x6_mr);
  //ucg.setFont(ucg_font_5x7_mr);
  //ucg.setFont(ucg_font_5x8_mr);
  //ucg.setFont(ucg_font_6x10_mr);
  //ucg.setFont(ucg_font_6x12_mr);
  //ucg.setFont(ucg_font_6x13_mr);
  //ucg.setFont(ucg_font_7x13_mr);
  //ucg.setFont(ucg_font_7x14_mr);
  //ucg.setFont(ucg_font_7x14B_mr);
  //ucg.setFont(ucg_font_8x13_mr);
  //ucg.setFont(ucg_font_8x13B_mr);
  //ucg.setFont(ucg_font_8x13O_mr);
  //ucg.setFont(ucg_font_9x15_mr);
  //ucg.setFont(ucg_font_9x15B_mr);
  //ucg.setFont(ucg_font_9x18_mr);
  //ucg.setFont(ucg_font_9x18B_mr);
  //ucg.setFont(ucg_font_10x20_mr);  // This is the biggest size for this font

  // Font name: fontgroupadobex11:
  //ucg.setFont(ucg_font_courR08_mr);
  //ucg.setFont(ucg_font_courB08_mr);
  //ucg.setFont(ucg_font_courR10_mr);
  //ucg.setFont(ucg_font_courR12_mr);
  //ucg.setFont(ucg_font_courB12_mr);
  //ucg.setFont(ucg_font_courR14_mr);
  //ucg.setFont(ucg_font_courB14_mr);
  //ucg.setFont(ucg_font_courR18_mr);
  //ucg.setFont(ucg_font_courB18_mr);
  //ucg.setFont(ucg_font_courR24_mr);
  //ucg.setFont(ucg_font_courB24_mr);
  //ucg.setFont(ucg_font_courR24_mr);
  //ucg.setFont(ucg_font_courB24_mr);  // This is the biggest size for this font

  // Font name: fontgroupprofont
  //ucg.setFont(ucg_font_profont10_mr);
  //ucg.setFont(ucg_font_profont11_mr);
  //ucg.setFont(ucg_font_profont12_mr);
  //ucg.setFont(ucg_font_profont15_mr);
  //ucg.setFont(ucg_font_profont17_mr);
  //ucg.setFont(ucg_font_profont22_mr);
  //ucg.setFont(ucg_font_profont29_mr);

  // Font name: fontgroupinconsolata:
  //ucg.setFont(ucg_font_inr16_mr);  // This is the smallest size for this font
  //ucg.setFont(ucg_font_inb16_mr);
  //ucg.setFont(ucg_font_inr19_mr);
  //ucg.setFont(ucg_font_inb19_mr);
  //ucg.setFont(ucg_font_inr21_mr);
  //ucg.setFont(ucg_font_inb21_mr);
  //ucg.setFont(ucg_font_inr24_mr);
  //ucg.setFont(ucg_font_inb24_mr);
  //ucg.setFont(ucg_font_inr27_mr);
  //ucg.setFont(ucg_font_inb27_mr);
  //ucg.setFont(ucg_font_inr30_mr);
  //ucg.setFont(ucg_font_inb30_mr);
  //ucg.setFont(ucg_font_inr33_mr);
  //ucg.setFont(ucg_font_inb33_mr);
  //ucg.setFont(ucg_font_inr38_mr);
  //ucg.setFont(ucg_font_inb38_mr);
  //ucg.setFont(ucg_font_inr42_mr);
  //ucg.setFont(ucg_font_inb42_mr);
  //ucg.setFont(ucg_font_inr46_mr);
  //ucg.setFont(ucg_font_inb46_mr);
  //ucg.setFont(ucg_font_inr49_mr);
  //ucg.setFont(ucg_font_inb49_mr);
  //ucg.setFont(ucg_font_inr53_mr);
  //ucg.setFont(ucg_font_inb53_mr);
  //ucg.setFont(ucg_font_inr57_mr);
  //ucg.setFont(ucg_font_inb57_mr);
  //*************************************

  // Welcome screen
  ucg.setFont(ucg_font_courB24_mr);  // Set font
  ucg.setColor(0, 0, 0, 250);  // Set color (0,R,G,B)
  ucg.setColor(1, 0, 0, 0);  // Set color of text background (1,R,G,B)
  ucg.setPrintPos(4,30);  // Set position (x,y)
  ucg.print("LEVI");  // Print text or value

  ucg.setFont(ucg_font_courB18_mr);  // Set font
  ucg.setColor(0, 0, 0, 250);  // Set color (0,R,G,B)
  ucg.setColor(1, 0, 0, 0);  // Set color of text background (1,R,G,B)
  ucg.setPrintPos(4,60);  // Set position (x,y)
  ucg.print("traktora");  // Print text or value

  // Write to the display the text "Viz ho:":
   ucg.setFont(ucg_font_courR14_mr);  // Set font
  ucg.setColor(0, 255, 255, 0);  // Set color (0,R,G,B)
  ucg.setPrintPos(4,100);  // Set position (x,y)
  ucg.print("by Pekatonix");  // Print text or value
  delay (2000);
  ucg.clearScreen();  // Clear the screen

  // Write to the display the text "Akkumulator:":
  ucg.setFont(ucg_font_courB14_mr); // Set font
  ucg.setColor(0, 255, 255, 0);  // Set color (0,R,G,B)
  ucg.setColor(1, 0, 0, 0);  // Set color of text background (1,R,G,B)
  ucg.setPrintPos(4,20);  // Set position (x,y)
  ucg.print("Akkumulator:");  // Print text or value

  // Write to the display the text "Viz ho:":
  ucg.setColor(0, 255, 255, 0);  // Set color (0,R,G,B)
  ucg.setPrintPos(4,80);  // Set position (x,y)
  ucg.print("Viz ho:");  // Print text or value


  // Draw rounded rectangle for "Akkumulator:":
   ucg.setColor(0, 255, 255, 255);  // Set color (0, R,G,B)
   ucg.drawRFrame(4, 24, 154, 34, 2);  // Start from top-left pixel (x,y,wigth,height,radius)

  // Draw filled rounded rectangle for "Akkumulator:":
  //ucg.setColor(255, 255, 255);  // Set color (0,R,G,B)
  //ucg.drawRBox(70, 24, 70, 34, 2);  // Start from top-left pixel (x,y,wigth,height,radius)

  // Draw rounded rectangle for "Viz ho:":
  ucg.setColor(0, 255, 255, 255);  // Set color (0,R,G,B)
  ucg.drawRFrame(4, 88, 154, 34, 2);  // Start from top-left pixel (x,y,wigth,height,radius)

  // Draw filled rounded rectangle for "Viz ho:":
  //ucg.setColor(255, 255, 255);  // Set color (0,R,G,B)
  //ucg.drawRBox(70, 86, 70, 32, 2);  // Start from top-left pixel (x,y,wigth,height,radius)

}  // End of setup


void loop(void)  // Start of loop
{
	// Voltmetru
voltageSensorVal = analogRead(voltageSensorPin);    // read the current sensor value (0 - 1023) 
  vOut = (voltageSensorVal / 1024) * vCC;             // convert the value to the real voltage on the analog pin
  vIn =  vOut * factor;                               // convert the voltage on the source by multiplying with the facto
  delay(1000);
	
// Write to the display the voltage with left text alignment:
  ucg.setFont(ucg_font_inb19_mr);  // Set font
  ucg.setColor(0, 255, 0, 0);  // Set color (0,R,G,B)
  ucg.setColor(1, 0, 0, 0);  // Set color of text background (1,R,G,B)
  ucg.setPrintPos(39,49);  // Set position (x,y)
  ucg.print(vIn);  // Print text or value
 // ucg.setPrintPos(100,45);  // Set position (x,y)
  ucg.print("V ");  // Print text or value

    // Termometru
  VRT = analogRead(A3);              // Acquisition analog value of VRT
  VRT = (5.00 / 1023.00) * VRT;      // Conversie in volti
  VR = VCC - VRT;
  RT = VRT / (VR / R);               // Resistanta divizorului de tensiune

  ln = log(RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0))); // Temperatura termistor

  TX = TX - 273.15;                 // Conversie in Celsius

// Write to the display the water temperature with left text alignment:
  ucg.setFont(ucg_font_inb19_mr);  // Set font
  ucg.setColor(0, 0, 255, 255);  // Set color (0,R,G,B)
  ucg.setColor(1, 0, 0, 0);  // Set color of text background (1,R,G,B)
  ucg.setPrintPos(33,114);  // Set position (x,y)
  ucg.print(TX);  // Print text or value
 // ucg.setPrintPos(100,45);  // Set position (x,y)
  ucg.print("C");  // Print text or value

  //Alarmne:
  if ((vIn < 12) || (vIn > 14)) {
	  digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
  }
  else {
	  digitalWrite(2, LOW);
   digitalWrite(3, HIGH);
  }

    if (TX > 90) {
    digitalWrite(4, HIGH);
  }
  else {
    digitalWrite(4, LOW);
  }
}  // End of loop

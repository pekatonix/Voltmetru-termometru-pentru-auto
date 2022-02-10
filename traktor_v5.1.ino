/* pekatonix@gmail.com
Termometru + voltmetru baterie tractor v.1.3
Display Nokia 5110
Termometru cu NTC 10K
parametrii termistor (valori din datasheet):
 * RT0: 10 000 Ω
 * B: 3977 K +- 0.75%
 * T0:  25 C
 * +- 5
 * display Nokia 5110
 Targu Mures, 26.01.2022
*/

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 7, 6);

#define RT0 10000   // Ω
#define B 3977      // K
#define VCC 5    //Tensiune alimentare 5V
#define R 10000  //R=10KΩ

static const byte DEGREES_CHAR = 1;  // A custom "degrees" symbol...
static const byte degrees_glyph[] = { 0x00, 0x07, 0x05, 0x07, 0x00 };

float RT, VR, ln, TX, T0, VRT; // variabile termometru

const int voltageSensorPin = A0;          // sensor tesiune la A0
float vIn;                                // measured voltage (3.3V = max. 16.5V, 5V = max 25V)
float vOut;
float voltageSensorVal;                   // value on pin A0 (0 - 1023)
const float factor = 5.05;               // factorul de corectie senzor/shield temperatura
const float vCC = 5.00;                   // Arduino input voltage (measurable by voltmeter)

void setup()   {
  Serial.begin(9600);

  display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(30);
  display.clearDisplay();

  // text display tests
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Levi's traktor");
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.println("by Pekatonix");
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  // Voltmetru
voltageSensorVal = analogRead(voltageSensorPin);    // read the current sensor value (0 - 1023) 
  vOut = (voltageSensorVal / 1024) * vCC;             // convert the value to the real voltage on the analog pin
  vIn =  vOut * factor;                               // convert the voltage on the source by multiplying with the factor

 // delay(1000);
 
// partea de afisare voltmetru 
   display.setTextSize(1);
   display.setTextColor(WHITE, BLACK);
   display.setCursor(0,0);
   display.println("Akkumulator:");
   display.setTextColor(BLACK);
   display.setCursor(40,12);
   display.print(vIn);
   display.print("V");

  
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.setCursor(0,22);
  display.println("Viz homersek:");
  display.setTextColor(BLACK);
  display.setCursor(40,32);
  display.print(TX);
  display.setCursor(66,30);
  display.print("o");
   display.setCursor(72,32);
  display.print("C");
  display.display();
  delay(2000);
  display.clearDisplay();
}

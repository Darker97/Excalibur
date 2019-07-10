#include <Adafruit_NeoPixel.h>

//Threads
#include <ThreadController.h>
#include <StaticThreadController.h>
#include <Thread.h>

#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

#include <Wire.h>

#if SOFTWARE_SERIAL_AVAILABLE
#include <SoftwareSerial.h>
#endif

#define NEOPIXELS_PIN 12
#define FACTORYRESET_ENABLE 1
#define MINIMUM_FIRMWARE_VERSION "0.6.6"
#define MODE_LED_BEHAVIOUR "MODE"
#define BLUEFRUIT_HWSERIAL_NAME Serial1

#define SAMPLE_WINDOW   10  // Sample window for average level
#define PEAK_HANG       24  // Time of pause before peak dot falls
#define PEAK_FALL        4  // Rate of falling peak dot

#define INPUT_FLOOR     56  // Lower range of mic sensitivity in dB SPL
#define INPUT_CEILING  110  // Upper range of mic sensitivity in db SPL

Adafruit_BluefruitLE_UART ble(Serial1, BLUEFRUIT_UART_MODE_PIN);

Adafruit_NeoPixel pixels(60, NEOPIXELS_PIN, NEO_GRB + NEO_KHZ800);

/*=========================================================================*/
// Public Variables
int Modus_Auswahl = 0;
int Brems_auswahl = false;

//int Sound_Lautstärke = 0;

int SleepModus_Bool = false;

//Anfang SOundSensor STuff
byte peak = 16;        // Peak level of column; used for falling dots
unsigned int sample;
byte dotCount = 0;     //Frame counter for peak dot
byte dotHangCount = 0; //Frame counter for holding peak dot

float mapf(float x, float in_min, float in_max, float out_min, float out_max);
//Ende SOund Sensor STuff
/*=========================================================================*/
//Deffinieren von Werten
#define MAX_TEMPERATUR 50

/*=========================================================================*/
// A small helper
void error(const __FlashStringHelper *err)
{
  Serial.println(err);
  while (1)
    ;
}

/*=========================================================================*/
//Kleine Sichereheitsroutine die die Temperatur checkt und das board in den Sleep Modus schaltet, sollte es zu heiß werden.
void Sicherheit_Temp()
{
  int Temp_Temperatur = CircuitPlayground.temperature();
  if (Temp_Temperatur >= MAX_TEMPERATUR)
  {
    Serial.println("ZU HEIß");
    SleepModus();
  }
}

/*=========================================================================*/
//Sleep Modi an

//TODO: CPU zum schlafen bringen
void SleepModus()
{
  Serial.println("gut Nacht :)");
  //detachInterrupt(BlIncoming, Bluetooth_Input, RISING);
  detachInterrupt(CircuitPlayground.leftButton(), Moduswechsel, RISING);
  detachInterrupt(Brems_Interrupt, Brems_Interrupt, RISING);

  Modus_Auswahl = 0;
  //set_sleep_Mode(SLEEP_MODE_PWR_DOWN);
  //sleep_Cpu();
}

void SleepModusAUS()
{
  //sleep_disable();
  setup();
  Serial.println("Bin wach :D");
}
/*=========================================================================*/
//abgeschlossen
void Moduswechsel()
{
  //ALle Pixels clearen
  pixels.clear();
  CircuitPlayground.clearPixels();
  Modus_Auswahl++;
  if (Modus_Auswahl == 5)
  {
    Modus_Auswahl = 0;
  }
}

/*
void LICHTER_AN(int[][] LICHTER)
{
  for (int i = 0; i < LICHTER.length; i++)
  {
    pixels.setPixelColor(LICHTER[i][0], LICHTER[i][1], LICHTER[i][2], LICHTER[i][3])
  }
}

//Notwendig?
void LICHTER_AUS()
{
  pixels.clear();
  CircuitPlayground.clearPixels();
}
*/

/*=========================================================================*/
//Sende Daten ans Handy
/*void Bluetooth_out(char[] output)
{
  Serial.print(output);
}*/

//Nicht implementiert
void Vibrieren()
{
  //VIBRIERE
}
/*=========================================================================*/

//Funktionen für CircuitPlaygroundNeopixels

//um alle Pixel auf eine Farbe zu setzten ähnlich wie fill für Circuit
void setpixCircuit(int red, int green, int blue)
{
  for (int p = 0; p < 10; p++)
  {
    CircuitPlayground.setPixelColor(p, red, green, blue);
  }
}

void colorWipeCircuit(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < 10; i++)
  {
    CircuitPlayground.setPixelColor(i, c);
    pixels.show();
    delay(wait);
  }
}
/*=========================================================================*/
//Funktionen für die NeoPixels

//Funktionen für SoundSensor NeoPixel 
//Used to draw a line between two points of a given color
void drawLine(uint8_t from, uint8_t to, uint32_t c) {
  uint8_t fromTemp;
  if (from > to) {
    fromTemp = from;
    from = to;
    to = fromTemp;
  }
  for(int i=from; i<=to; i++){
    pixels.setPixelColor(i, c);
  }
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void soundSensor() {
  // diese Func könnte man auch in die Loop schreiben? 
  float peakToPeak = 0;   // peak-to-peak level
  unsigned int c, y;

  //get peak sound pressure level over the sample window
  peakToPeak = CircuitPlayground.mic.soundPressureLevel(SAMPLE_WINDOW);

  //limit to the floor value
  peakToPeak = max(INPUT_FLOOR, peakToPeak);
 
  // Serial.println(peakToPeak);
  for(int i = 0; i <= 59; i++) {
    pixels.setPixelColor(i,Wheel(map(i,0,59,30,150)));
    setpixCircuit(0,255,0);
  }
  c = mapf(peakToPeak, INPUT_FLOOR, INPUT_CEILING, 60, 0);

  // Turn off pixels that are below volume threshold.
  if(c < peak) {
    peak = c;        // Keep dot on top
    dotHangCount = 0;    // make the dot hang before falling
  }
  if (c <= 60) { // Fill partial column with off pixels
    drawLine(60, 60-c, pixels.Color(0, 0, 0));
  }

    // Set the peak dot to match the rainbow gradient
  y = 60 - peak;
  pixels.setPixelColor(y-1,Wheel(map(y,0,59,30,150)));
  pixels.show();

  // Frame based peak dot animation
  if(dotHangCount > PEAK_HANG) { //Peak pause length
    if(++dotCount >= PEAK_FALL) { //Fall rate 
      peak++;
      dotCount = 0;
    }
  } 
  else {
    dotHangCount++; 
  }
}

//normales ColorWipe
void colorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < pixels.numPixels(); i++)
  {
    pixels.setPixelColor(i, c);
    pixels.show();
    delay(wait);
  }
}
/*
//TODO durch fill ersetzten?
//um alle Pixel auf eine Farbe zu setzten ähnlich wie fill
void setpix(int red, int green, int blue)
{
  for (int p = 0; p < pixels.numPixels(); p++)
  {
    pixels.setPixelColor(p, pixels.Color(red, green, blue));
  }
  pixels.show();
} */

//Regenbogen wird über ColorWipe immer wieder an und aus gemacht
void blink(int wait)
{
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256)
  {
    for (int i = 0; i < pixels.numPixels(); i++)
    { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      for (int p = 0; p < pixels.numPixels(); p++)
      {
        pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
      }
    }
    pixels.show(); // Update strip with new contents
    delay(wait);   // Pause for a moment
    for (int i = 0; i < pixels.numPixels(); i++)
    { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      //int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      for (int p = 0; p < pixels.numPixels(); p++)
      {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      }
      pixels.show();
      delay(10);
    }
    // delay(5);
  }
}

void buntBlink(int r, int g, int b)
{
  for (int i = 5; i >= 0; i--)
  {

    if (r > 255)
    {
      r = r - 5;
    }
    else
    {
      r = r + 5;
    }

    if (g > 255)
    {
      g = g - 5;
    }
    else
    {
      g = g + 5;
    }

    if (b > 255)
    {
      b = b - 5;
    }
    else
    {
      b = b + 5;
    }

    pixels.fill(pixels.Color(r, g, b));
    
    delay(100);
    pixels.clear();
    delay(100);
  }
}

//TODO
//kann man den Endpunkt und Anfangspunkt von colorWipe definieren
void colorWipeDefine(uint32_t c, uint8_t wait, int begin, int end)
{
  for (uint16_t i = begin; i < end; i++)
  {
    pixels.setPixelColor(i, c);
    pixels.show();
    delay(wait);
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait)
{
  int firstPixelHue = 0; // First pixel starts at red (hue 0)
  for (int a = 0; a < 30; a++)
  { // Repeat 30 times...
    for (int b = 0; b < 3; b++)
    {                 //  'b' counts from 0 to 2...
      pixels.clear(); //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for (int c = b; c < pixels.numPixels(); c += 3)
      {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int hue = firstPixelHue + c * 65536L / pixels.numPixels();
        uint32_t color = pixels.gamma32(pixels.ColorHSV(hue)); // hue -> RGB
        pixels.setPixelColor(c, color);                        // Set pixel 'c' to value 'color'
      }
      pixels.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

void partyParty()
{
  blink(10);
  theaterChaseRainbow(50);
  //colorWipeCircuit(); ?
  buntBlink(255, 0, 125);
  buntBlink(125, 0, 255);
  blink(10);
  buntBlink(0, 255, 125);
  theaterChaseRainbow(50);
  buntBlink(0, 125, 255);
  buntBlink(125, 255, 0);
  blink(10);
  buntBlink(255, 125, 0);
}

/*=========================================================================*/
bool getUserInput(char buffer[], uint8_t maxSize)
{
  // timeout in 100 milliseconds
  TimeoutTimer timeout(100);

  memset(buffer, 0, maxSize);
  while( (!Serial.available()) && !timeout.expired() ) { delay(1); }

  if ( timeout.expired() ) return false;

  delay(2);
  uint8_t count=0;
  do
  {
    count += Serial.readBytes(buffer+count, maxSize);
    delay(2);
  } while( (count < maxSize) && (Serial.available()) );

  return true;
}


//Auswerten von Signalen vom Handy
//Fertig
void Bluetooth_Input(char[] Input)
{

  //ALle Pixels clearen
  pixels.clear();
  CircuitPlayground.clearPixels();

  if (Input.count == 1)
  {
    Modus_Auswahl = Input[0];
  }

  if (Input.count == 2)
  {

    uint32_t FARBE;
    switch (Input[1])
    {
    //Rot
    case 1:
      FARBE = pixels.Color(255, 0, 0);
    //Grün
    case 2:
      FARBE = pixels.Color(0, 255, 0);
    //Blau
    case 3:
      FARBE = pixels.Color(0, 0, 255);
    //Gelb
    case 4:
      FARBE = pixels.Color(255, 128, 0);
    //Pink
    case 5:
      FARBE = pixels.Color(220, 118, 255);
    //weiß
    default:
      FARBE = pixels.Color(255, 255, 255);
    }
    switch (Input[0])
    {
    case 1:
      pixels.fill(FARBE, 0, 10);
    case 2:
      pixels.fill(FARBE, 10, 10);
    case 3:
      pixels.fill(FARBE, 20, 10);
    default:
      pixels.clear();
    }
  }
  else
  {
    if(Input[0] == -1) 
      {pixels.clear();
      CircuitPlayground.clearPixels();
      }
    //else if (Input[0] == -2) Bluetooth_out(Modus_Auswahl);
    /*else if (Input[0] == -3)
        Vibrieren();*/
    else Modus_Auswahl = Input[0];
  }
}
/*=========================================================================*/
void Bluetooth_Setup(){
    while (!Serial)
    ; // required for Flora & Micro
  delay(500);

  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit Command <-> Data Mode Example"));
  Serial.println(F("------------------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if (!ble.begin(VERBOSE_MODE))
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println(F("OK!"));

  if (FACTORYRESET_ENABLE)
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if (!ble.factoryReset())
    {
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in UART mode"));
  Serial.println(F("Then Enter characters to send to Bluefruit"));
  Serial.println();

  ble.verbose(false); // debug info is a little annoying after this point!

  /* Wait for connection */
  while (!ble.isConnected())
  {
    delay(500);
  }

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    Serial.println(F("******************************"));
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
    Serial.println(F("******************************"));
  }


}

void NeoPixel_Setup(){
    //setup for NeoPixels

  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif
  // END of Trinket-specific code.
  //  CircuitPlayground.clear();

  CircuitPlayground.begin();

  //neo pixel initalisieren und alle auf aus und Helligkeit einstellen
  pixels.clear();
  pixels.begin();

  pixels.show();
  pixels.setBrightness(50); //Helligkeit
}

//TODO
void setup(void)
{
  attachInterrupt(CircuitPlayground.slideSwitch(), SleepModus, RISING);
  attachInterrupt(CircuitPlayground.slideSwitch(), SleepModusAUS, FALLING);
  attachInterrupt(CircuitPlayground.leftButton(), Moduswechsel, RISING);

  /*=========================================================================*/
  NeoPixel_Setup();
  Bluetooth_Setup();

  Thread myThread = Thread();
  myThread.onRun(loop_bluetooth);
  myThread.enabled = true;
}
/*=========================================================================*/
//TODO
void loop_bluetooth(){
  while (true){
        // Check for user input
      char inputs[BUFSIZE+1];

      if ( getUserInput(inputs, BUFSIZE) )
      {
        // Send characters to Bluefruit
        Serial.print("[Send] ");
        Serial.println(inputs);
        Bluetooth_Input(inputs);
      }

      // Check for incoming characters from Bluefruit
      ble.println("AT+BLEUARTRX");
      ble.readline();
      if (strcmp(ble.buffer, "OK") == 0) {
        // no data
        return;
      }
      // Some data was found, its in the buffer
      Serial.print(F("[Recv] ")); Serial.println(ble.buffer);
      ble.waitForOK();
  }
}

void loop(void)
{

  Sicherheit_Temp();
  switch (Modus_Auswahl)
  {
    //Standart
  case 0:

    //Bremse
  case 1:
    //TODO Positionen prüfen
    pixels.fill(pixels.Color(50, 50, 50), 0, 10); //hier ist jeweils die Frage ob die Position stimmt und wo fängt der Alg anzuzählen
    pixels.fill(pixels.Color(50, 50, 50), 20, 10);
    pixels.fill(pixels.Color(50, 50, 50), 30, 10);
    pixels.fill(pixels.Color(50, 50, 50), 50, 10);
    pixels.show();
    //allgemeine Lichter rechts und Links

    //Bremse
    int temp_Bremse = CircuitPlayground.motionZ();
      if (temp_Bremse < 0)
      {
        Brems_auswahl = true;
      }
      else
      {
        Brems_auswahl = false;
      }


    if (Brems_auswahl){
      //LICHTER_AN();
      pixels.fill(pixels.Color(50, 50, 50), 0, 10); //hier ist jeweils die Frage ob die Position stimmt und wo fängt der Alg anzuzählen
      pixels.fill(pixels.Color(50, 0, 0), 10, 10);
      pixels.fill(pixels.Color(50, 50, 50), 20, 10);
      pixels.fill(pixels.Color(50, 50, 50), 30, 10);
      pixels.fill(pixels.Color(50, 0, 0), 40, 10);
      pixels.fill(pixels.Color(50, 50, 50), 50, 10);
      pixels.show();}
    else pixels.clear();

  // Heiligenschein
  case 2:
    colorWipe(pixels.Color(255, 128, 0), 50);
    setpixCircuit(255, 128, 0);

  //Party
  case 3:
    partyParty();

  //Sound Farbe im Zufall => rand()
  case 4:
    soundSensor();
  }
}

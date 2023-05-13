#include <FastLED.h>
#define LED_PIN 2
#define NUM_LEDS 450
#define SPEED 10
#define PAUSE 500

CRGB leds[NUM_LEDS];
int maxbright = 10;
int inByte = ‘n’;

void setup() {
FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
FastLED.setMaxPowerInVoltsAndMilliamps(5, 20000);
FastLED.clear();
FastLED.show();
Serial.begin(115200);
prompt();
}

int incr(int curval, int begval, int endval, int increment)
{
int incrval = curval;

if (begval <= endval)
{
incrval = curval + increment;
if (incrval > endval) incrval = endval;
}
else
{
incrval = curval - increment;
if (incrval < endval) incrval = endval;
}
return(incrval);
}

void ramp(int begred, // Beginning red intensity
int endred, // Ending red intensity
int beggreen, // Beginning green intensity
int endgreen, // Ending green intensity
int begblue,
int endblue,
int begbright, // Beginning brightness
int endbright, // Ending brightness
int increment, // Amount to add to colors and brightness each cycle
int interval, // Milliseconds to wait after each cycle
int endpause) // Milliseconds to wait after end of sequence
{
int red = begred;
int green = beggreen;
int blue = begblue;
int bright = begbright;
CRGB cval;

do {
cval = CRGB(red, green, blue);
for (int i=0; i<NUM_LEDS; i++) { leds[i] = cval; }
FastLED.setBrightness(bright);
FastLED.show();
delay (interval);
red = incr(red, begred, endred, increment);
green = incr(green, beggreen, endgreen, increment);
blue = incr(blue, begblue, endblue, increment);
bright = incr(bright, begbright, endbright, increment);
} while (red != endred || green != endgreen || blue != endblue);

delay (endpause);
}

void scroll(int begred, // Beginning red intensity
int endred, // Ending red intensity
int beggreen, // Beginning green intensity
int endgreen, // Ending green intensity
int begblue,
int endblue,
int begbright, // Beginning brightness
int endbright, // Ending brightness
int increment, // Amount to add to colors and brightness each cycle
int interval, // Milliseconds to wait after each cycle
int numsamecolor,
int endpause) // Milliseconds to wait after end of sequence
{
int red = begred;
int green = beggreen;
int blue = begblue;
int bright = begbright;
CRGB cval;
CRGB tmpleds[NUM_LEDS];

for (int k=0,j=0; k < NUM_LEDS; k++,j++){
for (int i=0; i < NUM_LEDS - 1; i++){ leds[i] = leds[i + 1]; }
cval = CRGB(red, green, blue);
leds[NUM_LEDS - 1] = cval;
if (j >= numsamecolor){
red = incr(red, begred, endred, increment);
green = incr(green, beggreen, endgreen, increment);
blue = incr(blue, begblue, endblue, increment);
j = 0;
}
FastLED.setBrightness(bright);
FastLED.show();
delay (interval);
}

delay (endpause);
}

void prompt(){
Serial.println(“”);
Serial.println(“a - WHITE”);
Serial.println(“b - RED”);
Serial.println(“c - GREEN”);
Serial.println(“d - BLUE”);
Serial.println(“e - RED/GREEN”);
Serial.println(“f - RED/BLUE”);
Serial.println(“g - GREEN/BLUE”);
Serial.println(“h - RED to GREEN”);
Serial.println(“i - RED to BLUE”);
Serial.println(“j - GREEN to BLUE”);
Serial.println(“k - RED to GREEN to BLUE”);
Serial.println(“l - SWIPE RED to GREEN to RED”);
Serial.println(“m - SWIPE GREEN to BLUE to GREEN”);
Serial.println(“n - SWIPE RED to GREEN to BLUE to YELLOW to RED”);
Serial.println(“o - CYCLE COLORS THRU BLACK”);
Serial.print(“>”);
}

void loop() {

if (Serial.available()) {
inByte = Serial.read();
Serial.write(inByte);
prompt();
}
switch (inByte) {
case ‘a’: // white
ramp(255, 255, 255, 255, 255, 255, 255, 255, 1, 10, 1000);
break;
case ‘b’: // red
ramp(255, 255, 000, 000, 000, 000, 255, 255, 1, 10, 1000);
break;
case ‘c’: // green
ramp(000, 000, 255, 255, 000, 000, 255, 255, 1, 10, 1000);
break;
case ‘d’: // blue
ramp(000, 000, 000, 000, 255, 255, 255, 255, 1, 10, 1000);
break;
case ‘e’: // red/green
ramp(255, 255, 255, 255, 000, 000, 255, 255, 1, 10, 1000);
break;
case ‘f’: // red/blue
ramp(255, 255, 000, 000, 255, 255, 255, 255, 1, 10, 1000);
break;
case ‘g’: // green/blue
ramp(000, 000, 255, 255, 255, 255, 255, 255, 1, 10, 1000);
break;
case ‘h’: // red to green
ramp(255, 000, 000, 255, 000, 000, 255, 255, 1, 5, 1);
ramp(000, 255, 255, 000, 000, 000, 255, 255, 1, 5, 1);
break;
case ‘i’: // red to blue
ramp(255, 000, 000, 000, 000, 255, 255, 255, 1, 5, 1);
ramp(000, 255, 000, 000, 255, 000, 255, 255, 1, 5, 1);
break;
case ‘j’: // green to blue
ramp(000, 000, 255, 000, 000, 255, 255, 255, 1, 5, 1);
ramp(000, 000, 000, 255, 255, 000, 255, 255, 1, 5, 1);
break;
case ‘k’: // red to green to blue
ramp(255, 000, 000, 255, 000, 000, 255, 255, 1, 5, 1);
ramp(000, 000, 255, 000, 000, 255, 255, 255, 1, 5, 1);
ramp(000, 255, 255, 000, 255, 000, 255, 255, 1, 5, 1);
break;
case ‘l’: // swipe red to green to red
scroll(255, 000, 000, 255, 000, 000, 255, 255, 1, 10, 1, 1);
scroll(000, 255, 255, 000, 000, 000, 255, 255, 1, 10, 1, 1);
break;
case ‘m’: // swipe green to blue to green
scroll(000, 000, 255, 000, 000, 255, 255, 255, 1, 10, 1, 1);
scroll(000, 000, 000, 255, 255, 000, 255, 255, 1, 10, 1, 1);
break;
case ‘n’: // swipe red to green to blue to yellow to red
scroll(255, 000, 000, 255, 000, 000, 255, 255, 1, 10, 1, 1); // red to green
scroll(000, 000, 255, 000, 000, 255, 255, 255, 1, 10, 1, 1); // green to blue
scroll(000, 255, 000, 255, 255, 000, 255, 255, 1, 10, 1, 1); // blue to yellow
scroll(255, 255, 255, 000, 000, 000, 255, 255, 1, 10, 1, 1); // yellow to red
break;
case ‘o’: // cycle colors thru black
ramp(000, 255, 000, 255, 000, 255, 000, 255, 1, 5, 1); // black to white
ramp(255, 000, 255, 000, 255, 000, 255, 000, 1, 5, 1); // white to black
ramp(000, 255, 000, 000, 000, 000, 000, 255, 1, 5, 1); // black to red
ramp(255, 000, 000, 000, 000, 000, 255, 000, 1, 5, 1); // red to black
ramp(000, 000, 000, 255, 000, 000, 000, 255, 1, 5, 1); // black to green
ramp(000, 000, 255, 000, 000, 000, 255, 000, 1, 5, 1); // green to black
ramp(000, 000, 000, 000, 000, 255, 000, 255, 1, 5, 1); // black to blue
ramp(000, 000, 000, 000, 255, 000, 255, 000, 1, 5, 1); // blue to black
break;
}
}

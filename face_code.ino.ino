#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.fillCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_HEIGHT/2, WHITE);
  display.fillCircle(SCREEN_WIDTH/2, (SCREEN_HEIGHT/2)-(SCREEN_HEIGHT/12), SCREEN_HEIGHT/2+(SCREEN_HEIGHT/32), BLACK);
  display.fillCircle((SCREEN_WIDTH/2)-(SCREEN_WIDTH / 6), 20, SCREEN_HEIGHT / 8, WHITE);
  display.fillCircle((SCREEN_WIDTH/2)+(SCREEN_WIDTH / 6), 20, SCREEN_HEIGHT / 8, WHITE);
  display.display();
}

void loop() {
  // put your main code here, to run repeatedly:

}

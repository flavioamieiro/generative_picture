#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>  

#define uS_TO_S_FACTOR 1000000ULL
#define TIME_TO_SLEEP 5
#define W 800
#define H 480

RTC_DATA_ATTR bool first_boot = true;

GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display(GxEPD2_750_T7(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));

void setup() {
  Serial.begin(115200);
  Serial.println("Starting setup...");
  Serial.println(first_boot);
  if (first_boot == true) {
    first_boot = false;
    onBoot();
  } else {
    draw();
  } 
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Sleeping...");
  esp_deep_sleep_start();
}


void loop() {}

void onBoot() {
  display.init(115200, true, 2, false);
  display.setRotation(0);
  display.setFullWindow();
  display.clearScreen();
  display.powerOff();
}

void draw() {
  display.init(115200, false, 2, false);
  Serial.println("Draw.");
  
  int w = 32;
  int h = 32;
  
  int x = random(0, (display.width() - w));
  int y = random(0, (display.height() - h));

  display.setRotation(0);
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE);
  display.fillRect(x, y, w, h, GxEPD_BLACK);
  display.displayWindow(x, y, w, h);
  display.powerOff();
}

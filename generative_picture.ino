/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <GxEPD2_BW.h>

#define W 480
#define H 800
#define BUFF_SIZE (W * H) / 8

#define SCALE 2

uint8_t buff[BUFF_SIZE] = {};

GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display(GxEPD2_750_T7(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));

void setup() {
  clearDisplay();
  initBuff();
}


void loop() {
  draw();
  delay(100);
}

void clearDisplay() {
  display.init(0, true, 2, false);
  display.setRotation(3);
  display.setFullWindow();
  display.clearScreen();
}

void draw() {
  updateBuff();
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE);
  for (int x=0; x<W; x++){
    for (int y=0; y<H; y++) {
      uint16_t color = getPixel(x, y) ? GxEPD_BLACK : GxEPD_WHITE;
      display.drawPixel(x, y, color);
    }
  }
  display.displayWindow(0, 0, display.width(), display.height());
}

int getXYindex(int x, int y) {
  if (x >= W || y >= H) {
    return -1;
  }
  return x + (W * y);
}

bool getPixel(int x, int y) {
  int idx = getXYindex(x, y);
  if (idx == -1) {
    return false;
  };
  int byte_address = idx / 8;
  int bit_pos_MSB_first = 7 - (idx % 8);

  return ((buff[byte_address] & (0x01 << bit_pos_MSB_first)) != 0x00);

}

void setPixel(int x, int y, bool filled) {
  int idx = getXYindex(x, y);
  if (idx == -1) {
    return;
  };
  int byte_address = idx / 8;
  int bit_pos_MSB_first = 7 - (idx % 8);

  if (filled) {
    buff[byte_address] |= (0x01 << bit_pos_MSB_first);
  } else {
    buff[byte_address] &= ~(0x01 << bit_pos_MSB_first);
  };
}

void initBuff() {
  int rx = 40;
  int ry = 200;
  int rw = 400;
  int rh = 400;
  for(int x=0; x<W; x++) {
    for (int y=0; y<H; y++) {
       if (((x == rx || x == rx+rw) && y >= ry && y <= ry+rh) ||
              ((y == ry || y == ry+rh) && x >= rx && x <= rx+rw)) {
                setPixel(x, y, true);
      }
    }
  }
}

void updateBuff() {
    float scl = SCALE;
    float newWSize = W*(1/scl);
    float newHSize = H*(1/scl);
    for (float x=0; x<W; x+=scl) {
      for (float y=0; y<H; y+=scl) {
        float new_x = ((W - newWSize)/2) + (x/scl);
        float new_y = ((H - newHSize)/2) + (y/scl);
        if (getPixel(x, y)) {
            setPixel((int)new_x, (int)new_y, true);
        }
      }
    }
}

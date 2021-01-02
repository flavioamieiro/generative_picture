#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>  

#define uS_TO_S_FACTOR 1000000ULL
#define TIME_TO_SLEEP 5

#define W 800
#define H 480

#define BOARD_WIDTH 40
#define BOARD_HEIGHT 40
#define BOARD_SIZE BOARD_WIDTH * BOARD_HEIGHT

#define CELL_W 800 / BOARD_WIDTH
#define CELL_H 480 / BOARD_HEIGHT


RTC_DATA_ATTR int boot_count = 0;
RTC_DATA_ATTR bool board[BOARD_SIZE] = {};

GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display(GxEPD2_750_T7(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));

void setup() {
  if (boot_count == 0) {
    initializeBoard();
  }

  if (boot_count % 100 == 0) {
    clearDisplay();
  }

  draw();

  boot_count++;
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  display.powerOff();
  esp_deep_sleep_start();
}


void loop() {}

void clearDisplay() {
  display.init(0, true, 2, false);
  display.setRotation(0);
  display.setFullWindow();
  display.clearScreen();
}

void draw() {
  display.init(0, false, 2, false);

  display.setRotation(0);
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE);
  for (int x=0; x<BOARD_WIDTH; x++){
    for (int y=0; y<BOARD_HEIGHT; y++) {
      uint16_t color = getCell(x, y) ? GxEPD_BLACK : GxEPD_WHITE;
      display.fillRect(x*CELL_W, y*CELL_H, CELL_W, CELL_H, color);
    }
  }

  display.setOldBuffer();

  updateBoard();

  display.fillScreen(GxEPD_WHITE);
  for (int x=0; x<BOARD_WIDTH; x++){
    for (int y=0; y<BOARD_HEIGHT; y++) {
      uint16_t color = getCell(x, y) ? GxEPD_BLACK : GxEPD_WHITE;
      display.fillRect(x*CELL_W, y*CELL_H, CELL_W, CELL_H, color);
    }
  }
  display.displayWindow(0, 0, display.width(), display.height());
}

bool getCell(int x, int y) {
  return board[x + (BOARD_WIDTH * y)];
}

void setCell(int x, int y, bool alive) {
  board[x + (BOARD_WIDTH * y)] = alive;
}

int getLivingNeighbors(int x, int y) {
    int n = 0;
    for (int i=-1; i<2; i++) {
      for (int j=-1; j<2; j++) {
        if (i == 0 && j == 0) {
          // this cell;
          continue;
        } else {
          int nx = x+i;
          int ny = y+j;
          if (nx > 0 && nx < BOARD_WIDTH && ny > 0 && ny < BOARD_WIDTH) {
            int nidx = nx + (BOARD_WIDTH * ny);
            if (board[nidx]) {
              n++;
            }
          }
        }
      }
    }
    return n;
}

void updateBoard() {
  bool new_board[BOARD_SIZE] = {};
  for (int x=0; x<BOARD_WIDTH; x++) {
    for (int y=0; y<BOARD_WIDTH; y++) {
      int idx = x + (BOARD_WIDTH * y);
      int neighbors_alive = getLivingNeighbors(x, y);
      if (neighbors_alive < 2 || neighbors_alive > 3) {
        new_board[idx] = false;
      } else if (neighbors_alive == 3) {
        new_board[idx] = true;
      } else {
        new_board[idx] = getCell(x, y);
      }
    }
  }

  for (int i=0; i<BOARD_SIZE; i++) {
    board[i] = new_board[i];
  }
}

void initializeBoard() {
  for (int i=0; i<BOARD_SIZE; i++) {
    board[i] = false;
  }

  for (int i=0; i<300; i++) {
    int x = random(0, BOARD_WIDTH);
    int y = random(0, BOARD_HEIGHT);
    setCell(x, y, true);
  }
}

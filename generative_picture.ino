#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>  

#define uS_TO_S_FACTOR 1000000ULL
#define TIME_TO_SLEEP 5
#define W 800
#define H 480
#define BOARD_WIDTH 40
#define BOARD_HEIGHT 40
#define CELL_W 800 / BOARD_WIDTH
#define CELL_H 480 / BOARD_HEIGHT

#define BOARD_SIZE BOARD_WIDTH * BOARD_HEIGHT

RTC_DATA_ATTR bool first_boot = true;
RTC_DATA_ATTR bool board[BOARD_SIZE] = {};

GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display(GxEPD2_750_T7(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4));

void setup() {
  Serial.begin(115200);
  Serial.println("Starting setup...");
  if (first_boot == true) {
    first_boot = false;
    onBoot();
  } else {
    updateBoard();
    draw();
  }
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Sleeping...");
  esp_deep_sleep_start();
}


void loop() {}

void onBoot() {
  initializeBoard();
  display.init(115200, true, 2, false);
  display.setRotation(0);
  display.setFullWindow();
  display.clearScreen();
  display.powerOff();
}

void draw() {
  display.init(115200, false, 2, false);

  display.setRotation(0);
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE);
  for (int x=0; x<BOARD_WIDTH; x++){
    for (int y=0; y<BOARD_HEIGHT; y++) {
      uint16_t color = getCell(x, y) ? GxEPD_BLACK : GxEPD_WHITE;
      display.fillRect(x*CELL_W, y*CELL_H, CELL_W, CELL_H, color);
    }
  }
  display.displayWindow(0, 0, display.width(), display.height());
  display.powerOff();
}

int getXYBufferAddress(int x, int y) {
  if (x >= BOARD_WIDTH || y >= BOARD_HEIGHT) {
    return -1;
  }
  return x + (BOARD_WIDTH * y);
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

  setCell(11, 10, true);
  setCell(12, 11, true);
  setCell(10, 12, true);
  setCell(11, 12, true);
  setCell(12, 12, true);
}

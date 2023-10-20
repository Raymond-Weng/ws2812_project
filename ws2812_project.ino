#include <Adafruit_NeoPixel.h>

#define SERIAL_BAUD 9600   // baud rate
#define NUM_LEDS_TOTAL 90  // 總LED數量
#define LED_PIN 9         // 連接第一個LED的腳位
#define BUTTON_PINS 2      // 連接按鈕的腳位
#define LIGHT_PER_ROW 5   // 每行上的按鈕數量
#define BREAK_BETWEEN_ROWS 2    //行與行之間不亮的燈泡數量

int delayTime[9] = {900, 800, 700, 600, 500, 400, 300, 200, 100};  //每段燈泡出現的時間差（毫秒）

Adafruit_NeoPixel leds(NUM_LEDS_TOTAL, LED_PIN, NEO_GRB + NEO_KHZ800);  //  定義ws2812燈條

bool last_status;
bool status;
int unit = 0;
long last_update;
bool light;

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println("start!");
  leds.begin();
  leds.show();
  pinMode(BUTTON_PINS, INPUT_PULLUP);
}

void loop() {

  if (ifBotton()){ //  按鈕功能函數
    if(light){
      setunitColor(0, 0, 0);  //  暗
      unit = 0;
      light = false;
    }else{
      light = true;
      unit++;
      last_update = millis();
    }
  }

  if(light && unit - 1 < sizeof(delayTime) / sizeof(delayTime[0]) && millis() - last_update > delayTime[unit - 1]){
    unit++;
    last_update = millis();
  }

  setunitColor(255, 255, 71);  //  香蕉黃

  Serial.println(status);
  delay(100);  //防死機
}

void setunitColor(int red, int green, int blue) {  // 設定LED顏色
  for (int i = 0; i < unit * 7;) {
    for(int r = 0; r < LIGHT_PER_ROW; r++){
      leds.setPixelColor(i + r, red, green, blue);
    }
    i += LIGHT_PER_ROW;
    for(int r = 0; r < BREAK_BETWEEN_ROWS; r++){
      leds.setPixelColor(i + r, 0, 0, 0);
    }
    i += BREAK_BETWEEN_ROWS;
  }
  leds.show();
}

bool ifBotton() {                      // 檢查按鈕狀態
  status = !digitalRead(BUTTON_PINS);  // 取得按鈕狀態

  if (!last_status && status){
    last_status = status;                // 紀錄按鈕的狀態
    return true;
  }
  else{
    last_status = status;                // 紀錄按鈕的狀態
    return false;
  }
}

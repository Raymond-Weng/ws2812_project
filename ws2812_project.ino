#include <Adafruit_NeoPixel.h>

#define SERIAL_BAUD 9600   // baud rate
#define NUM_LEDS_TOTAL 90  // 總LED數量
#define LED_PIN 9         // 連接第一個LED的腳位
#define LED_PIN2 11         // 連接第一個LED的腳位
#define BUTTON_PINS 2      // 連接按鈕的腳位
#define LIGHT_PER_ROW 5   // 每行上的按鈕數量
#define BREAK_BETWEEN_ROWS 3    //行與行之間不亮的燈泡數量

int delayTime[9] = {400, 300, 200, 200, 200, 100, 100, 800, 1000};  //每段燈泡出現的時間差（毫秒）
int delayTime2[3] = {1100, 1100, 1100};  //每段燈泡出現的時間差（毫秒）

Adafruit_NeoPixel leds(NUM_LEDS_TOTAL, LED_PIN, NEO_GRB + NEO_KHZ800);  //  定義ws2812燈條
Adafruit_NeoPixel leds2(NUM_LEDS_TOTAL, LED_PIN2, NEO_GRB + NEO_KHZ800);  //  定義ws2812燈條

bool last_status;
bool status;
int unit = 0;
int unit2 = 0;
long last_update;
long last_update2;
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
      unit2 = 0;
      light = false;
    }else{
      light = true;
      unit++;
      unit2++;
      last_update = millis();
      last_update2 = last_update;
    }
  }

  if(light && unit - 1 < sizeof(delayTime) / sizeof(delayTime[0]) && millis() - last_update > delayTime[unit - 1]){
    unit++;
    last_update = millis();
  }

  if(light && unit2 - 1 < sizeof(delayTime2) / sizeof(delayTime2[0]) && millis() - last_update2 > delayTime2[unit2 - 1]){
    unit2++;
    last_update2 = millis();
  }

  setunitColor(255, 255, 71);  //  香蕉黃

  Serial.println(unit2);
  delay(100);  //防死機
}

void setunitColor(int red, int green, int blue) {  // 設定LED顏色
  int skipped_lights = 0; //被跳過的燈泡數量
  for (int i = 0; i < unit * (LIGHT_PER_ROW + BREAK_BETWEEN_ROWS) + skipped_lights;) {
    if(30 - (i % 30) < LIGHT_PER_ROW){ //跳過接縫處燈泡
      // for(int r = 0; r < 30 - (i % 30); r++){ //不亮的燈泡
      //   leds.setPixelColor(i + r, 0, 0, 0);
      // }
      skipped_lights += 30 - (i % 30);
      i += skipped_lights;
    }
    for(int r = 0; r < LIGHT_PER_ROW; r++){
      leds.setPixelColor(i + r, red, green, blue);
    }
    i += LIGHT_PER_ROW;
    // for(int r = 0; r < BREAK_BETWEEN_ROWS; r++){ //不亮的燈泡
    //   leds.setPixelColor(i + r, 0, 0, 0);
    // }
    i += BREAK_BETWEEN_ROWS;
  }
  leds.show();

  int skipped_lights2 = 0; //被跳過的燈泡數量
  for (int i = 0; i < unit2 * (LIGHT_PER_ROW + BREAK_BETWEEN_ROWS) + skipped_lights2;) {
    if(30 - (i % 30) < LIGHT_PER_ROW){ //跳過接縫處燈泡
      // for(int r = 0; r < 30 - (i % 30); r++){ //不亮的燈泡
      //   leds2.setPixelColor(i + r, 0, 0, 0);
      // }
      skipped_lights2 += 30 - (i % 30);
      i += skipped_lights2;
    }
    for(int r = 0; r < LIGHT_PER_ROW; r++){
      leds2.setPixelColor(i + r, red, green, blue);
    }
    i += LIGHT_PER_ROW;
    // for(int r = 0; r < BREAK_BETWEEN_ROWS; r++){ //不亮的燈泡
    //   leds2.setPixelColor(i + r, 0, 0, 0);
    // }
    i += BREAK_BETWEEN_ROWS;
  }
  leds2.show();
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

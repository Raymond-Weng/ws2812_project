#include <Adafruit_NeoPixel.h>

#define SERIAL_BAUD 9600   // baud rate
#define NUM_LEDS_TOTAL 90  // 總LED數量
#define LED_PIN 9         // 連接第一個LED的腳位
#define LED_PIN2 11         // 連接第一個LED的腳位
#define LIGHT_PER_ROW 5   // 每行上的按鈕數量
#define BREAK_BETWEEN_ROWS 3    //行與行之間不亮的燈泡數量

int delayTime[9] = 400;  //每段燈泡出現的時間差（毫秒）
int delayTime2[3] = 400;  //每段燈泡出現的時間差（毫秒）
int R = random(0,255);
int G = random(0,255);
int B = random(0,255);

Adafruit_NeoPixel leds(NUM_LEDS_TOTAL, LED_PIN, NEO_GRB + NEO_KHZ800);  //  定義ws2812燈條
Adafruit_NeoPixel leds2(NUM_LEDS_TOTAL, LED_PIN2, NEO_GRB + NEO_KHZ800);  //  定義ws2812燈條

bool last_status;
bool status;
int unit = 0;
int unit2 = 0;
long last_update;
long last_update2;
bool light = true;
bool raise = true;

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println("start!");
  leds.begin();
  leds.show();
  leds2.begin();
  leds2.show();
  pinMode(INPUT_PULLUP);
}

void loop() {
  if(raise){
    unit++;
    unit2++;
    last_update=millis();
    last_update2=millis();
  }

  if(!raise){
    unit--;
    unit2--;
    last_update=millis();
    last_update2=millis();
  }

  if(raise){
    if(unit==9 && unit2==9){
      raise = false;
    }
  }else{
    if(unit==0 && unit2==0){
      raise = true;
    }
  }

  setunitColor(R,G,B);  // 隨機顏色

  Serial.println(unit, unit2);
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

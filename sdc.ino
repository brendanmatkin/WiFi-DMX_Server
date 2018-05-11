
float avg_bright[MAX_FIXTURES];
uint32_t animTimer[MAX_FIXTURES];
//uint32_t frameCounter[MAX_FIXTURES];
enum Animation {
  UP,
  DOWN,
  HOLD
} anim[MAX_FIXTURES];

//void sdcZygotes() {
//  
//  for (int i = 0; i < currentFixtures; i++) {
//    frameCounter[i]++;
//    uint8_t bright;
//    switch (anim[i]) {
//      case UP: 
//        bright = ease8InOutCubic((frameCounter[i]*DMX_PERIOD/sdc_speed) + sdc_delay*i);
//        if (bright == 255) {
//          anim[i] = DOWN;
//          frameCounter[i] = 0;
//        }
//        break;
//      case DOWN: 
//        bright = 255 - ease8InOutCubic((frameCounter[i]*DMX_PERIOD/sdc_speed) + sdc_delay*i);
//        animTimer[i] = millis();
//        if (bright == 0) anim[i] = HOLD;
//        break;
//      case HOLD:
//        bright = 0;
//        if (millis() - animTimer[i] > (200 + sdc_speed*5)) {
//          anim[i] = UP;
//          frameCounter[i] = 0;
//        }
//        break;
//      default:
//        break;
//    }
//    
//    float coef = 0.1;
//    avg_bright[i] = coef *bright + (1.0-coef)*avg_bright[i];
//    
//    leds[i].v = avg_bright[i];
//    CRGB _temp = leds[i];
//    if (i == 0) {
//      //Serial.println(avg_bright[i]);
//      Serial.println(bright);
//      Serial.println(anim[i]);
//    }
//    //setZygoteDMX(i, scale8_video(leds[i].r, bright), scale8_video(leds[i].g, bright), scale8_video(leds[i].b, bright), whiteLeds[i].r);
//    setZygoteDMX( i, _temp.r, _temp.g, _temp.b, whiteLeds[i].r);
//  }
//}

uint32_t frameCounter;
void sdcZygotes() {
  frameCounter++;
  for (int i = 0; i < currentFixtures; i++) {
    uint8_t _bright = sin8((frameCounter*DMX_PERIOD/sdc_speed) + sdc_delay*i);
    //float bright = (exp(sin(frameCounter*DMX_PERIOD/6)) - 0.36787944)*108.0;
    int bright = map(_bright, 1, 255, -20, 255);
    bright = constrain(bright, 0, 255);
    float coef = 0.25;
    avg_bright[i] = coef *bright + (1.0-coef)*avg_bright[i];
    leds[i].v = avg_bright[i];
    leds[i].h+=hueCycleSpeed;
    CRGB _temp = leds[i];
    if (i == 0) {
      Serial.printf ("%d, %.2f\n", bright, avg_bright[i]);
    }
    //setZygoteDMX(i, scale8_video(leds[i].r, bright), scale8_video(leds[i].g, bright), scale8_video(leds[i].b, bright), whiteLeds[i].r);
    setZygoteDMX( i, _temp.r, _temp.g, _temp.b, whiteLeds[i].r);
  }
}

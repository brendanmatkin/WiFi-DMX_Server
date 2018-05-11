unsigned long frameCounter = 0;
void sdcZygotes() {
  frameCounter++;
  for (int i = 0; i < currentFixtures; i++) {
    uint8_t bright = cubicwave8(frameCounter * DMX_PERIOD / 50.0f);
    //uint8_t hue = leds[i].getHue();
    //leds[i] = CHSV(200, 255, bright);
    CRGB _temp = leds[i];
    if (i == 0) Serial.println(bright);
    //setZygoteDMX(i, scale8_video(leds[i].r, bright), scale8_video(leds[i].g, bright), scale8_video(leds[i].b, bright), whiteLeds[i].r);
    setZygoteDMX( i, _temp.r, _temp.g, _temp.b, whiteLeds[i].r);
  }
}


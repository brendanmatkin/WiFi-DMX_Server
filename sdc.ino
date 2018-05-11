unsigned long frameCounter = 0;

void sdcZygotes() {
  frameCounter++;
  //uint8_t bright = quadwave8((float)frameCounter * DMX_PERIOD / 150.0f);
  uint8_t bright = sin8(frameCounter*DMX_PERIOD/sdc_speed);
  for (int i = 0; i < currentFixtures; i++) {
    
    //float bright = (exp(sin(frameCounter*DMX_PERIOD/6)) - 0.36787944)*108.0;
    //uint8_t hue = leds[i].getHue();
    //leds[i] = CHSV(200, 255, bright);
    leds[i].v = bright;
    CRGB _temp = leds[i];
    //if (i == 0) Serial.println(bright);
    //setZygoteDMX(i, scale8_video(leds[i].r, bright), scale8_video(leds[i].g, bright), scale8_video(leds[i].b, bright), whiteLeds[i].r);
    setZygoteDMX( i, _temp.r, _temp.g, _temp.b, whiteLeds[i].r);
  }
}


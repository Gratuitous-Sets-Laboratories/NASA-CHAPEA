void aimAnimate(uint32_t color){
  int pixel = lockedX * 8;
  if (lockedX % 2){
    int flip = (lockedY + 1) * -1;
    pixel += 8;
    pixel += flip;
  }
  else{
    pixel += lockedY;
  }
  for (int p = 8; p < 264; p++){
    neoPixel.setPixelColor(p,0);
  }
  neoPixel.setPixelColor(pixel,color);
  neoPixel.show();
}

void scanAnimate(){
  for (int p = 0; p < 8; p++){
    neoPixel.setPixelColor(p,0,200,200);
    neoPixel.setPixelColor(p-1,0);
    neoPixel.show();
    delay(100);
  }
  for (int p = 0; p < 8; p++){
    neoPixel.setPixelColor(p,0);
    neoPixel.show();
  }
  for (int p = 7; p >= 0; p--){
    neoPixel.setPixelColor(p,0,200,200);
    neoPixel.setPixelColor(p+1,0);
    neoPixel.show();
    delay(100);
  }
    for (int p = 0; p < 8; p++){
    neoPixel.setPixelColor(p,0);
    neoPixel.show();
  }
}

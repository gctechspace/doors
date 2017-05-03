
void showColour(byte colour){
  if (!DEBUG){
    switch (colour) {
      case OFF:    
        pixels.setPixelColor(0, 0x000000); 
        pixels.setPixelColor(1, 0x000000);
        break;
      case RED:   
        pixels.setPixelColor(0, 0xFF0000);
        pixels.setPixelColor(1, 0xFF0000);
        //pixels.setPixelColor(0, pixels.Color(255,0,0)); 
        break;
      case ORANGE:   
        pixels.setPixelColor(0, 0xFF8000);
        pixels.setPixelColor(1, 0xFF8000);
        //pixels.setPixelColor(0, pixels.Color(255,128,0));
        break;
      case YELLOW:   
        pixels.setPixelColor(0, 0xFFFF00);
        pixels.setPixelColor(1, 0xFFFF00);
        //pixels.setPixelColor(0, pixels.Color(255,255,0));
        break;
      case GREEN:    
        pixels.setPixelColor(0, 0x00FF00); 
        pixels.setPixelColor(1, 0x00FF00);
        //pixels.setPixelColor(0, pixels.Color(0,255,0)); 
        break;
      case LBLUE:   
        pixels.setPixelColor(0, 0x00FFFF);
        pixels.setPixelColor(1, 0x00FFFF);
        //pixels.setPixelColor(0, pixels.Color(0,255,255)); 
        break;
      case BLUE:   
        pixels.setPixelColor(0, 0x0000FF);
        pixels.setPixelColor(1, 0x0000FF);
        //pixels.setPixelColor(0, pixels.Color(0,0,255));
        break;
      case PURPLE:   
        pixels.setPixelColor(0, 0x7F00FF);
        pixels.setPixelColor(1, 0x7F00FF);
        //pixels.setPixelColor(0, pixels.Color(127,0,255));
        break;
      case PINK:   
        pixels.setPixelColor(0, 0xFF00FF);
        pixels.setPixelColor(1, 0xFF00FF);
        //pixels.setPixelColor(0, pixels.Color(255,0,255));
        break;
      case WHITE:   
        pixels.setPixelColor(0, 0x7F7F7F);
        pixels.setPixelColor(1, 0x7F7F7F);
        //pixels.setPixelColor(0, pixels.Color(127,127,127));
        break;
    }
    pixels.show(); // This sends the updated pixel color to the hardware.
  }
}


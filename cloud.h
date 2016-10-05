const uint8_t maxCommandLength = 63; // set by Particle: https://docs.particle.io/reference/firmware/photon/#particle-function-

int power = 1;
int brightness = 32;
int backgroundBrightness = brightness / 2;
int patternIndex = 0;
int autoplay = 1;
int rotation = 2;
char text[maxCommandLength] = "SmartMatrix Library";
int scroll = 1;
int scrollMode = 0;
int scrollSpeed = 0;
int scrollFont = 0;
int scrollTop = 0;
int scrollR = 255;
int scrollG = 255;
int scrollB = 255;

int getIntValue(String data, int min, int max) {
  int value = data.toInt();
  if(value < min)
    value = min;
  else if (value > max)
    value = max;
  return value;
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1  };
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex) {
      found++;
      strIndex[0] = strIndex[1]+1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

int setPower(String command) {
  power = getIntValue(command, 0, 1);
  return power;
}

int setBrightness(String command) {
  brightness = getIntValue(command, 0, 255);
  matrix.setBrightness(brightness);
  return brightness;
}

int setBackgroundBrightness(String command) {
  backgroundBrightness = getIntValue(command, 0, 255);
  backgroundLayer.setBrightness(backgroundBrightness);
  return brightness;
}

int setPatternIndex(String command) {
  patternIndex = getIntValue(command, 0, patternCount - 1);
  return patternIndex;
}

int setAutoplay(String command) {
  autoplay = getIntValue(command, 0, 1);
  return autoplay;
}

int setRotation(String command) {
  rotation = getIntValue(command, 0, 3);

  switch(rotation) {
    case 0:
      matrix.setRotation(rotation0);
      break;

    case 1:
      matrix.setRotation(rotation90);
      break;

    case 2:
      matrix.setRotation(rotation180);
      break;

    case 3:
      matrix.setRotation(rotation270);
      break;
  }

  return rotation;
}

int setText(String command) {
  command.toCharArray(text, maxCommandLength);
  scrollingLayer.update(text);
  return 0;
}

int setScroll(String command) {
  if(command == "0") {
    scrollingLayer.stop();
    return 0;
  } else {
    scrollingLayer.start(text, -1);
    return 1;
  }
}

int setScrollMode(String command) {
  scrollMode = getIntValue(command, 0, 5);

  switch(scrollMode) {
    case 0:
      scrollingLayer.setMode(wrapForward);
      break;

    case 1:
      scrollingLayer.setMode(bounceForward);
      break;

    case 2:
      scrollingLayer.setMode(bounceReverse);
      break;

    case 3:
      scrollingLayer.setMode(stopped);
      break;

    case 4:
      scrollingLayer.setMode(off);
      break;

    case 5:
      scrollingLayer.setMode(wrapForwardFromLeft);
      break;
  }

  return scrollMode;
}

int setScrollColor(String command) {
  String r = getValue(command, ',', 0);
  String g = getValue(command, ',', 1);
  String b = getValue(command, ',', 2);

  scrollR = getIntValue(r, 0, 255);
  scrollG = getIntValue(g, 0, 255);
  scrollB = getIntValue(b, 0, 255);

  scrollingLayer.setColor({scrollR, scrollG, scrollB});

  return 0;
}

int setScrollSpeed(String command) {
  scrollSpeed = command.toInt();
  scrollingLayer.setSpeed(scrollSpeed);
  return scrollSpeed;
}

int setScrollFont(String command) {
  scrollFont = getIntValue(command, 0, 5);

  switch(scrollMode) {
    case 0:
      scrollingLayer.setFont(font3x5);
      break;

    case 1:
      scrollingLayer.setFont(font5x7);
      break;

    case 2:
      scrollingLayer.setFont(font6x10);
      break;

    case 3:
      scrollingLayer.setFont(font8x13);
      break;

    case 4:
      scrollingLayer.setFont(gohufont11);
      break;

    case 5:
      scrollingLayer.setFont(gohufont11b);
      break;
  }

  return scrollFont;
}

int setScrollOffsetFromTop(String command) {
  scrollTop = command.toInt();
  scrollingLayer.setOffsetFromTop(scrollTop);
  return scrollTop;
}

void setupCloudVariables() {
  // https://docs.particle.io/reference/firmware/photon/#particle-variable-
  // Up to 20 cloud variables may be registered and each variable name is limited to a maximum of 12 characters.
  // There are three supported data types:
  // * INT
  // * DOUBLE
  // * STRING (maximum string length is 622 bytes)

  Particle.variable("power", power);
  Particle.variable("brightness", brightness);
  Particle.variable("backBright", backgroundBrightness);
  Particle.variable("patternIndex", patternIndex);
  Particle.variable("autoplay", autoplay);
  Particle.variable("rotation", rotation);

  // scrolling text variables
  Particle.variable("text", text);
  Particle.variable("scroll", scroll);
  Particle.variable("scrollMode", scrollMode);
  Particle.variable("scrollSpeed", scrollSpeed);
  Particle.variable("scrollFont", scrollFont);
  Particle.variable("scrollTop", scrollTop);
  Particle.variable("scrollR", scrollR);
  Particle.variable("scrollG", scrollR);
  Particle.variable("scrollB", scrollR);
}

void setupCloudFunctions() {
  // https://docs.particle.io/reference/firmware/photon/#particle-function-
  // Up to 15 cloud functions may be registered and each function name is limited to a maximum of 12 characters.
  // A cloud function is set up to take one argument of the String datatype.
  // This argument length is limited to a max of 63 characters.

  Particle.function("power", setPower);
  Particle.function("brightness", setBrightness);
  Particle.function("bkgrndBrtns", setBackgroundBrightness);
  Particle.function("pattern", setPatternIndex);
  Particle.function("autoplay", setAutoplay);
  Particle.function("rotation", setRotation);

  // scrolling text functions
  Particle.function("text", setText);
  Particle.function("scroll", setScroll);
  Particle.function("scrollMode", setScrollMode);
  Particle.function("scrollColor", setScrollColor);
  Particle.function("scrollSpeed", setScrollSpeed);
  Particle.function("scrollFont", setScrollFont);
  Particle.function("scrollTop", setScrollOffsetFromTop);
}

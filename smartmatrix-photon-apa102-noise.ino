/*
  Connect APA102 Data Pin to D2
  Connect APA102 Clock Pin to D4
*/

#include "application.h"

#include "SmartMatrix3_Photon_Apa102.h"
#include "FastLEDSmartMatrix.h"
FASTLED_USING_NAMESPACE;

#define COLOR_DEPTH 24                  // known working: 24, 48 - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24
const uint8_t kMatrixWidth = 16;        // known working: 16, 32, 48, 64
const uint8_t kMatrixHeight = 16;       // known working: 32, 64, 96, 128
const uint8_t kRefreshDepth = 36;       // known working: 24, 36, 48
const uint8_t kDmaBufferRows = 4;       // known working: 2-4, use 2 to save memory, more to keep from dropping frames and automatically lowering refresh rate
const uint8_t kPanelType = 0;   // use SMARTMATRIX_HUB75_16ROW_MOD8SCAN for common 16x32 panels
const uint8_t kMatrixOptions = (SMARTMATRIX_OPTIONS_NONE);      // see http://docs.pixelmatix.com/SmartMatrix for options
const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);
const uint8_t kScrollingLayerOptions = (SM_SCROLLING_OPTIONS_NONE);

SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);
SMARTMATRIX_ALLOCATE_SCROLLING_LAYER(scrollingLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kScrollingLayerOptions);

rgb24 *buffer;

const uint16_t NUM_LEDS = kMatrixWidth * kMatrixHeight;

const uint8_t scale = 256 / kMatrixWidth;

uint16_t noisex;
uint16_t noisey;
uint16_t noisez;

int noisespeedx;
int noisespeedy;
int noisespeedz;

uint16_t noisescale;

CRGBPalette16 palette;

#include "patterns.h"

typedef void (*Pattern)();
typedef Pattern PatternList[];

const PatternList patterns = {
  rainbowNoise,
  fireNoise,
  fireNoise2,
  lavaNoise,
  rainbowStripeNoise,
  partyNoise,
  forestNoise,
  cloudNoise,
  oceanNoise
};

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
const uint8_t patternCount = ARRAY_SIZE(patterns);

#include "cloud.h"

uint16_t XY(uint8_t x, uint8_t y) {
  return kMatrixWidth * y + x;
}

void setup() {
  matrix.addLayer(&backgroundLayer);
  matrix.addLayer(&scrollingLayer);
  matrix.setRefreshRate(30);
  matrix.begin();
  matrix.setRotation(rotation180);
  matrix.setBrightness(brightness);

  backgroundLayer.setBrightness(backgroundBrightness);
  backgroundLayer.enableColorCorrection(true);

  scrollingLayer.setMode(wrapForward);
  scrollingLayer.setColor({0xff, 0xff, 0xff});
  scrollingLayer.setSpeed(35);
  scrollingLayer.setFont(gohufont11b);
  scrollingLayer.setOffsetFromTop((kMatrixHeight/2) - 5);
  scrollingLayer.start("SmartMatrix Library", -1);

  setupCloudVariables();
  setupCloudFunctions();
}

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  patternIndex = (patternIndex + 1) % patternCount;
}

void loop() {
  EVERY_N_MILLISECONDS(1000/120) {

    buffer = backgroundLayer.backBuffer();

    if(power != 0) {
      patterns[patternIndex]();
    }
    else {
      backgroundLayer.fillScreen({0, 0, 0});
      scrollingLayer.setColor({0, 0, 0});
      scrollingLayer.stop();
    }

    backgroundLayer.swapBuffers(true);
  }

  if(autoplay != 0) {
    EVERY_N_SECONDS( 10 ) {
      nextPattern();  // change patterns periodically
    }
  }
}

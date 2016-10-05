void drawNoise()
{
  for (uint8_t x = 0; x < kMatrixWidth; x++) {
    for (uint8_t y = 0; y < kMatrixHeight; y++) {
      uint16_t xoffset = noisescale * x;
      uint16_t yoffset = noisescale * y;

      uint8_t index = inoise8(x + xoffset + noisex, y + yoffset + noisey, noisez);

      // The range of the inoise8 function is roughly 16-238.
      // These two operations expand those values out to roughly 0..255
      // You can comment them out if you want the raw noise data.
      index = qsub8(index, 16);
      index = qadd8(index, scale8(index, 39));

      buffer[XY(x, y)] = ColorFromPalette(palette, index);
    }
  }

  noisex += noisespeedx;
  noisey += noisespeedy;
  noisez += noisespeedz;
}

void fireNoise() {
  noisespeedx = 0;
  noisespeedy = 64;
  noisespeedz = 0;
  noisescale = 64;
  palette = HeatColors_p;
  drawNoise();
}

void fireNoise2() {
  noisespeedx = 0;
  noisespeedy = 192;
  noisespeedz = 4;
  noisescale = 128;
  palette = HeatColors_p;
  drawNoise();
}

void lavaNoise() {
  noisespeedx = 0;
  noisespeedy = -3;
  noisespeedz = 2;
  noisescale = 8;
  palette = LavaColors_p;
  drawNoise();
}

void rainbowNoise() {
  noisespeedx = 8;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 16;
  palette = RainbowColors_p;
  drawNoise();
}

void rainbowStripeNoise() {
  noisespeedx = 2;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 4;
  palette = RainbowStripeColors_p;
  drawNoise();
}

void partyNoise() {
  noisespeedx = 0;
  noisespeedy = 8;
  noisespeedz = 1;
  noisescale = 16;
  palette = PartyColors_p;
  drawNoise();
}

void forestNoise() {
  noisespeedx = 2;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 4;
  palette = ForestColors_p;
  drawNoise();
}

void cloudNoise() {
  noisespeedx = 0;
  noisespeedy = 4;
  noisespeedz = 0;
  noisescale = 16;
  palette = CloudColors_p;
  drawNoise();
}

void oceanNoise() {
  noisespeedx = 0;
  noisespeedy = 0;
  noisespeedz = 16;
  noisescale = 16;
  palette = OceanColors_p;
  drawNoise();
}

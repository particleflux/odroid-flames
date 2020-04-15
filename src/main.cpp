#include <Arduino.h>
#include <odroid_go.h>

// TFT_WIDTH and HEIGHT are swapped (rotated display)
#define DISPLAY_WIDTH TFT_HEIGHT
#define DISPLAY_HEIGHT TFT_WIDTH

#define FIRE_HEIGHT 80
#define FIRE_WIDTH DISPLAY_WIDTH

#define FIRE_BOTTOM (DISPLAY_HEIGHT - 1)
#define NUM_PIXELS (FIRE_WIDTH * FIRE_HEIGHT)

#define DEMO_TEXT "ODROID-GO flames demo"

// 16 bit rgb  - 565
// but for pushRect() they need to be in BIG endian
static uint16_t colors[] = {
        0x2000,
        0x2018,
        0x6028,
        0x6040,
        0xa050,
        0xe060,
        0xe070,
        0x2089,
        0x6099,
        0xe0a9,
        0x20ba,
        0x20c2,
        0x60da,
        0xa0da,
        0xa0da,
        0xe0d2,
        0x21d3,
        0x61cb,
        0xa1cb,
        0xe1cb,
        0x22cc,
        0x22c4,
        0x62c4,
        0xa3c4,
        0xe3bc,
        0xe3bc,
        0x24bd,
        0x24bd,
        0x65bd,
        0x65b5,
        0xa5b5,
        0xa6b5,
        0x6dce,
        0xf3de,
        0x78ef,
        0xffff,
};

static int numColors = sizeof(colors) / sizeof(colors[0]);

uint8_t firePixels[FIRE_WIDTH * FIRE_HEIGHT];
uint16_t firePixels2[FIRE_WIDTH * FIRE_HEIGHT];

void updateFire();
void spreadFire(int src);

void setup() {
    GO.begin();


    for (int x = 0; x < FIRE_WIDTH; ++x) {
        firePixels[x] = numColors - 1;
        GO.lcd.drawPixel(x, FIRE_BOTTOM, colors[numColors - 1]);
    }

    GO.lcd.setTextSize(2);
    GO.lcd.setTextColor(ORANGE);
    int textWidth = GO.lcd.textWidth(DEMO_TEXT);
    GO.lcd.drawString(DEMO_TEXT, (DISPLAY_WIDTH - textWidth) / 2, DISPLAY_HEIGHT / 2 - 8);
}

void loop() {
    updateFire();
    delay(0);
}


void updateFire() {
    int i;

    for (i = FIRE_WIDTH; i < FIRE_WIDTH * FIRE_HEIGHT; ++i) {
        spreadFire(i);
    }

    for (i = 0; i < FIRE_WIDTH * FIRE_HEIGHT; ++i) {
        firePixels2[i] = colors[firePixels[i]];
    }

    GO.lcd.pushRect(0, 0, FIRE_WIDTH, FIRE_HEIGHT, (uint16_t *) firePixels2);

    for (i = 0; i < FIRE_WIDTH * FIRE_HEIGHT; ++i) {
        firePixels2[NUM_PIXELS - i - 1] = colors[firePixels[i]];
    }

    GO.lcd.pushRect(0, FIRE_BOTTOM - FIRE_HEIGHT, FIRE_WIDTH, FIRE_HEIGHT, (uint16_t *) firePixels2);
}

void spreadFire(int src) {
    int rnd = random(-2, 2);

    firePixels[src - rnd] = firePixels[src - FIRE_WIDTH] > 0 ? firePixels[src - FIRE_WIDTH] - (rnd & 1) : 0;
}

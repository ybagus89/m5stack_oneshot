#include <Arduino.h>
#include <M5Stack.h>

/*
  OneShot125 with a 1kHz frequency range:
    - Min ~125µs
    - Neutral = 190µs  (user request)
    - Max ~250µs

  We'll define thrusterValue in [-127..0..+127]:
    - -127 => ~125µs
    -   0 =>  190µs  (neutral)
    - +127 => ~250µs

  This code sets up pin 26 for ESC output at 1kHz.  The user can press:
    - BtnA to increase thrusterValue by 5 (towards +127 => forward)
    - BtnB to decrease thrusterValue by 5 (towards -127 => reverse)
    - BtnC to set thrusterValue = 0 (190µs => neutral)
*/

static const int ESC_PIN        = 26;
static const int ONESHOT_FREQ   = 1000;  // 1kHz
static const int ONESHOT_RES    = 8;     // 8-bit resolution (0..255)
static const int THRUSTER_MIN   = -127;
static const int THRUSTER_MAX   =  127;

// The user requests that the neutral pulse is 190µs
static const float NEUTRAL_US   = 190.0f;
// The valid range is from 125µs to 250µs
static const float MIN_US       = 125.0f;
static const float MAX_US       = 250.0f;

int thrusterValue = 0;  // range [-127..127]

// Map thrusterValue to 125..250µs with neutral at 190µs.
int mapThrusterToMicros(int value) {
  // fraction in [-1..+1]
  float fraction = float(value) / 127.0f;

  // If fraction is positive, we scale from NEUTRAL_US => MAX_US
  //   rangeUp = MAX_US - NEUTRAL_US
  // If fraction is negative, we scale from NEUTRAL_US => MIN_US
  //   rangeDown = NEUTRAL_US - MIN_US
  float rangeUp   = (MAX_US - NEUTRAL_US);  // ~60
  float rangeDown = (NEUTRAL_US - MIN_US);  // ~65
  
  float pulseUs;
  
  if (fraction >= 0.0f) {
    // fraction in [0..1]
    pulseUs = NEUTRAL_US + fraction * rangeUp;
  } else {
    // fraction in [-1..0)
    pulseUs = NEUTRAL_US + fraction * rangeDown;
  }

  // Constrain for safety
  if (pulseUs < MIN_US)  pulseUs = MIN_US;
  if (pulseUs > MAX_US)  pulseUs = MAX_US;

  return int(pulseUs);
}

void setup() {
  // Initialize M5Stack
  M5.begin(true, false, true, true);
  M5.Power.begin();
  M5.Lcd.setBrightness(255);

  // Clear screen, set text color
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.setTextSize(2);

  // Configure channel 0 for 1kHz OneShot
  ledcSetup(0, ONESHOT_FREQ, ONESHOT_RES);
  ledcAttachPin(ESC_PIN, 0);

  // Start at neutral
  thrusterValue = 0;
}

void loop() {
  M5.update();

  // Forward
  if (M5.BtnA.wasPressed()) {
    thrusterValue = min(thrusterValue + 5, THRUSTER_MAX);
  }

  // Reverse
  if (M5.BtnB.wasPressed()) {
    thrusterValue = max(thrusterValue - 5, THRUSTER_MIN);
  }

  // Neutral
  if (M5.BtnC.wasPressed()) {
    thrusterValue = 0;
  }

  // Convert thrusterValue to microseconds (125..250) with neutral at 190
  int pulseMicros = mapThrusterToMicros(thrusterValue);

  // At 1kHz, period is 1000µs. Map pulseMicros to an 8-bit duty cycle:
  // ratio = pulseMicros / 1000 => [0.125..0.250], ~ [125..250].
  // dutyByte = ratio * 255
  float ratio    = float(pulseMicros) / 1000.0f;
  int   dutyByte = int(ratio * 255.0f);

  // Write to ESC
  ledcWrite(0, dutyByte);

  // Display info
  M5.Lcd.clearDisplay();
  M5.Lcd.setCursor(0, 20);

  M5.Lcd.printf("Thruster [-127..127]: %d\n", thrusterValue);
  M5.Lcd.printf("Pulse us: %d\n", pulseMicros);
  M5.Lcd.printf("Neutral: %.1f\n", NEUTRAL_US);
  M5.Lcd.printf("Duty: %d  (max 255)\n", dutyByte);

  delay(100);
}

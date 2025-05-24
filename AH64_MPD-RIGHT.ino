#define DCSBIOS_DEFAULT_SERIAL
#include "DcsBios.h"


// #########################################################################################
// ****************************
// ***** AH64D MPD RIGHT ******
// *****   by MilKris    ******
// **************************** 

// ONLY WORKS WITH ARDUINO MEGA 2560 OR A COMPATIBLE BOARD

// Check the wiring diagram in the build guide!

// NOTE: If Pin 13 is LOW, the program starts in Pilot MPD mode.
//       If Pin 13 is HIGH, it starts in CPG MPD mode.
//       The mode must be selected before powering the Arduino.
//       You need to restart the Arduino if you change the mode.
//       Unfortunately, DCS-BIOS cannot handle this dynamically.
//       Since I fly the Apache exclusively from the CPG position, 
//       this solution works fine for me.
//       Don’t like it? Create an Arduino USB joystick device or use a Leo Bodnar BBI-64
// #########################################################################################


// Pin to switch between CPG and PLT mode.
const int selectorPin = 13;

// --- Pilot-Configuration ---
class PilotMPD {
public:
  DcsBios::Switch2Pos ac{"PLT_MPD_R_AC", 38};
  DcsBios::Switch2Pos ast{"PLT_MPD_R_AST", 47};
  DcsBios::Switch2Pos com{"PLT_MPD_R_COM", 39};
  DcsBios::Switch2Pos fcr{"PLT_MPD_R_FCR", 29};
  DcsBios::Switch2Pos tsd{"PLT_MPD_R_TSD", 31};
  DcsBios::Switch2Pos vid{"PLT_MPD_R_VID", 40};
  DcsBios::Switch2Pos wpn{"PLT_MPD_R_WPN", 30};

  DcsBios::RotaryEncoder brt{"PLT_MPD_R_BRT", "-1600", "+1600", 8, 9};
  DcsBios::RotaryEncoder video{"PLT_MPD_R_VIDEO", "-1600", "+1600", 10, 11};

  const byte modePins[3] = {2, 3, 4};
  DcsBios::SwitchMultiPos mode{"PLT_MPD_R_MODE", modePins, 3};

  DcsBios::Switch2Pos b[6] = {
    {"PLT_MPD_R_B1", 32}, {"PLT_MPD_R_B2", 33}, {"PLT_MPD_R_B3", 34},
    {"PLT_MPD_R_B4", 35}, {"PLT_MPD_R_B5", 36}, {"PLT_MPD_R_B6", 37}
  };

  DcsBios::Switch2Pos l[6] = {
    {"PLT_MPD_R_L1", 23}, {"PLT_MPD_R_L2", 24}, {"PLT_MPD_R_L3", 25},
    {"PLT_MPD_R_L4", 26}, {"PLT_MPD_R_L5", 27}, {"PLT_MPD_R_L6", 28}
  };

  DcsBios::Switch2Pos r[6] = {
    {"PLT_MPD_R_R1", 46}, {"PLT_MPD_R_R2", 45}, {"PLT_MPD_R_R3", 44},
    {"PLT_MPD_R_R4", 43}, {"PLT_MPD_R_R5", 42}, {"PLT_MPD_R_R6", 41}
  };

  DcsBios::Switch2Pos t[6] = {
    {"PLT_MPD_R_T1", 53}, {"PLT_MPD_R_T2", 52}, {"PLT_MPD_R_T3", 51},
    {"PLT_MPD_R_T4", 50}, {"PLT_MPD_R_T5", 49}, {"PLT_MPD_R_T6", 48}
  };
};

// --- CPG-Configuration ---
class CpgMPD {
public:
  DcsBios::Switch2Pos ac{"CPG_MPD_R_AC", 38};
  DcsBios::Switch2Pos ast{"CPG_MPD_R_AST", 47};
  DcsBios::Switch2Pos com{"CPG_MPD_R_COM", 39};
  DcsBios::Switch2Pos fcr{"CPG_MPD_R_FCR", 29};
  DcsBios::Switch2Pos tsd{"CPG_MPD_R_TSD", 31};
  DcsBios::Switch2Pos vid{"CPG_MPD_R_VID", 40};
  DcsBios::Switch2Pos wpn{"CPG_MPD_R_WPN", 30};

  DcsBios::RotaryEncoder brt{"CPG_MPD_R_BRT", "-1600", "+1600", 8, 9};
  DcsBios::RotaryEncoder video{"CPG_MPD_R_VIDEO", "-1600", "+1600", 10, 11};

  const byte modePins[3] = {2, 3, 4};
  DcsBios::SwitchMultiPos mode{"CPG_MPD_R_MODE", modePins, 3};

  DcsBios::Switch2Pos b[6] = {
    {"CPG_MPD_R_B1", 32}, {"CPG_MPD_R_B2", 33}, {"CPG_MPD_R_B3", 34},
    {"CPG_MPD_R_B4", 35}, {"CPG_MPD_R_B5", 36}, {"CPG_MPD_R_B6", 37}
  };

  DcsBios::Switch2Pos l[6] = {
    {"CPG_MPD_R_L1", 23}, {"CPG_MPD_R_L2", 24}, {"CPG_MPD_R_L3", 25},
    {"CPG_MPD_R_L4", 26}, {"CPG_MPD_R_L5", 27}, {"CPG_MPD_R_L6", 28}
  };

  DcsBios::Switch2Pos r[6] = {
    {"CPG_MPD_R_R1", 46}, {"CPG_MPD_R_R2", 45}, {"CPG_MPD_R_R3", 44},
    {"CPG_MPD_R_R4", 43}, {"CPG_MPD_R_R5", 42}, {"CPG_MPD_R_R6", 41}
  };

  DcsBios::Switch2Pos t[6] = {
    {"CPG_MPD_R_T1", 53}, {"CPG_MPD_R_T2", 52}, {"CPG_MPD_R_T3", 51},
    {"CPG_MPD_R_T4", 50}, {"CPG_MPD_R_T5", 49}, {"CPG_MPD_R_T6", 48}
  };
};

// ----------------
PilotMPD* pilot = nullptr;
CpgMPD* cpg = nullptr;

void setup() {
  pinMode(selectorPin, INPUT_PULLUP);  // LOW = Pilot, HIGH = CPG
  bool isPilot = digitalRead(selectorPin);

  if (isPilot) {
    pilot = new PilotMPD();
  } else {
    cpg = new CpgMPD();
  }
  delay(500);
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}

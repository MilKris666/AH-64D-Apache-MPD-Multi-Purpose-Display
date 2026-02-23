// #########################################################################################
// ****************************
// ***** AH64D MPD LEFT  ******
// *****   Auto Seat     ******
// *****   by MilKris    ******
// **************************** 
//
// ONLY WORKS WITH ARDUINO MEGA 2560
//
// This sketch automatically detects whether you are in the PLT or CP/G seat and switches automatically.
//
// SEE THE WIRING-DIAGRAM IN THE HOW-TO.PDF!!!
//
// NOTE: 
// #########################################################################################

#define DCSBIOS_IRQ_SERIAL
#include <DcsBios.h>
#include <Arduino.h>

// =========================================================
// Seat state + safe switching
// =========================================================
volatile uint8_t status = 0;           // 0 = PLT, 1 = CPG
volatile bool gSeatChanged = true;     // true -> Set the layout once at startup
volatile bool g_bRun = true;

// Seat Position Callback (arrives here as 0/1 due to mask/shift in the IntegerBuffer)
void onSeatPositionChange(unsigned int newValueSeat) {
  uint8_t s = (uint8_t)newValueSeat;  // 0 or 1
  if (s != status) {
    status = s;
    gSeatChanged = true;
  }
}
DcsBios::IntegerBuffer seatPositionBuffer(0x8750, 0x0100, 8, onSeatPositionChange);

// =========================================================
// Controls
// =========================================================
// 
// NUMBER_OF_CONTROLS must exactly correspond to the number of switches, encoders, etc.
#define NUMBER_OF_CONTROLS 34

DcsBios::Switch2Pos switch2_01("UNDEF", 38);
DcsBios::Switch2Pos switch2_02("UNDEF", 47);
DcsBios::Switch2Pos switch2_03("UNDEF", 39);
DcsBios::Switch2Pos switch2_04("UNDEF", 29);
DcsBios::Switch2Pos switch2_05("UNDEF", 31);
DcsBios::Switch2Pos switch2_06("UNDEF", 40);
DcsBios::Switch2Pos switch2_07("UNDEF", 30);

DcsBios::Switch2Pos switch2_08("UNDEF", 32);
DcsBios::Switch2Pos switch2_09("UNDEF", 33);
DcsBios::Switch2Pos switch2_10("UNDEF", 34);
DcsBios::Switch2Pos switch2_11("UNDEF", 35);
DcsBios::Switch2Pos switch2_12("UNDEF", 36);
DcsBios::Switch2Pos switch2_13("UNDEF", 37);

DcsBios::Switch2Pos switch2_14("UNDEF", 23);
DcsBios::Switch2Pos switch2_15("UNDEF", 24);
DcsBios::Switch2Pos switch2_16("UNDEF", 25);
DcsBios::Switch2Pos switch2_17("UNDEF", 26);
DcsBios::Switch2Pos switch2_18("UNDEF", 27);
DcsBios::Switch2Pos switch2_19("UNDEF", 28);

DcsBios::Switch2Pos switch2_20("UNDEF", 46);
DcsBios::Switch2Pos switch2_21("UNDEF", 45);
DcsBios::Switch2Pos switch2_22("UNDEF", 44);
DcsBios::Switch2Pos switch2_23("UNDEF", 43);
DcsBios::Switch2Pos switch2_24("UNDEF", 42);
DcsBios::Switch2Pos switch2_25("UNDEF", 41);

DcsBios::Switch2Pos switch2_26("UNDEF", 53);
DcsBios::Switch2Pos switch2_27("UNDEF", 52);
DcsBios::Switch2Pos switch2_28("UNDEF", 51);
DcsBios::Switch2Pos switch2_29("UNDEF", 50);
DcsBios::Switch2Pos switch2_30("UNDEF", 49);
DcsBios::Switch2Pos switch2_31("UNDEF", 48);


DcsBios::RotaryEncoder encoder_01({"UNDEF", "-1600", "+1600", 8, 9});
DcsBios::RotaryEncoder encoder_02({"UNDEF", "-1600", "+1600", 10, 11});

const byte modePins[3] = {2, 3, 4};
DcsBios::SwitchMultiPos mode(modePins, 3, "UNDEF"); 

// =========================================================
// Layout struct
// =========================================================
struct _controllayout_type {
  const char* cntrl_name[NUMBER_OF_CONTROLS];

  _controllayout_type(const char* const _name[NUMBER_OF_CONTROLS]) {
    for (int i = 0; i < NUMBER_OF_CONTROLS; i++) {
      cntrl_name[i] = _name[i];
    }
  }

  void MakeCurrent() {
    // The order must match the arrays.
    switch2_01.SetControl(cntrl_name[0]);
    switch2_02.SetControl(cntrl_name[1]);
    switch2_03.SetControl(cntrl_name[2]);
    switch2_04.SetControl(cntrl_name[3]);
    switch2_05.SetControl(cntrl_name[4]);
    switch2_06.SetControl(cntrl_name[5]);
    switch2_07.SetControl(cntrl_name[6]);
    encoder_01.SetControl(cntrl_name[7]);
    encoder_02.SetControl(cntrl_name[8]);
    mode.SetControl(cntrl_name[9]); 

    switch2_08.SetControl(cntrl_name[10]);
    switch2_09.SetControl(cntrl_name[11]);
    switch2_10.SetControl(cntrl_name[12]);
    switch2_11.SetControl(cntrl_name[13]);
    switch2_12.SetControl(cntrl_name[14]);
    switch2_13.SetControl(cntrl_name[15]);

    switch2_14.SetControl(cntrl_name[16]);
    switch2_15.SetControl(cntrl_name[17]);
    switch2_16.SetControl(cntrl_name[18]);
    switch2_17.SetControl(cntrl_name[19]);
    switch2_18.SetControl(cntrl_name[20]);
    switch2_19.SetControl(cntrl_name[21]);

    switch2_20.SetControl(cntrl_name[22]);
    switch2_21.SetControl(cntrl_name[23]);
    switch2_22.SetControl(cntrl_name[24]);
    switch2_23.SetControl(cntrl_name[25]);
    switch2_24.SetControl(cntrl_name[26]);
    switch2_25.SetControl(cntrl_name[27]);

    switch2_26.SetControl(cntrl_name[28]);
    switch2_27.SetControl(cntrl_name[29]);
    switch2_28.SetControl(cntrl_name[30]);
    switch2_29.SetControl(cntrl_name[31]);
    switch2_30.SetControl(cntrl_name[32]);
    switch2_31.SetControl(cntrl_name[33]);
  }
};

// =========================================================
// Control names
// =========================================================
const char* const controlnames_PLT[NUMBER_OF_CONTROLS] = {
  "PLT_MPD_L_AC",
  "PLT_MPD_L_AST",
  "PLT_MPD_L_COM",
  "PLT_MPD_L_FCR",
  "PLT_MPD_L_TSD",
  "PLT_MPD_L_VID",
  "PLT_MPD_L_WPN",
  "PLT_MPD_L_BRT",
  "PLT_MPD_L_VIDEO",
  "PLT_MPD_L_MODE", 

  "PLT_MPD_L_B1",
  "PLT_MPD_L_B2",
  "PLT_MPD_L_B3",
  "PLT_MPD_L_B4",
  "PLT_MPD_L_B5",
  "PLT_MPD_L_B6",

  "PLT_MPD_L_L1",
  "PLT_MPD_L_L2",
  "PLT_MPD_L_L3",
  "PLT_MPD_L_L4",
  "PLT_MPD_L_L5",
  "PLT_MPD_L_L6",

  "PLT_MPD_L_R1",
  "PLT_MPD_L_R2",
  "PLT_MPD_L_R3",
  "PLT_MPD_L_R4",
  "PLT_MPD_L_R5",
  "PLT_MPD_L_R6",

  "PLT_MPD_L_T1",
  "PLT_MPD_L_T2",
  "PLT_MPD_L_T3",
  "PLT_MPD_L_T4",
  "PLT_MPD_L_T5",
  "PLT_MPD_L_T6",
};
_controllayout_type cl_PLT(controlnames_PLT);

const char* const controlnames_CPG[NUMBER_OF_CONTROLS] = {
  "CPG_MPD_L_AC",
  "CPG_MPD_L_AST",
  "CPG_MPD_L_COM",
  "CPG_MPD_L_FCR",
  "CPG_MPD_L_TSD",
  "CPG_MPD_L_VID",
  "CPG_MPD_L_WPN",
  "CPG_MPD_L_BRT",
  "CPG_MPD_L_VIDEO",
  "CPG_MPD_L_MODE", 
  
  "CPG_MPD_L_B1",
  "CPG_MPD_L_B2",
  "CPG_MPD_L_B3",
  "CPG_MPD_L_B4",
  "CPG_MPD_L_B5",
  "CPG_MPD_L_B6",

  "CPG_MPD_L_L1",
  "CPG_MPD_L_L2",
  "CPG_MPD_L_L3",
  "CPG_MPD_L_L4",
  "CPG_MPD_L_L5",
  "CPG_MPD_L_L6",

  "CPG_MPD_L_R1",
  "CPG_MPD_L_R2",
  "CPG_MPD_L_R3",
  "CPG_MPD_L_R4",
  "CPG_MPD_L_R5",
  "CPG_MPD_L_R6",

  "CPG_MPD_L_T1",
  "CPG_MPD_L_T2",
  "CPG_MPD_L_T3",
  "CPG_MPD_L_T4",
  "CPG_MPD_L_T5",
  "CPG_MPD_L_T6",
};
_controllayout_type cl_CPG(controlnames_CPG);

// =========================================================
// Apply seat layout (called from loop when change detected)
// =========================================================
void applySeatLayout() {
  g_bRun = false;                 // kurz DcsBios::loop pausieren
  if (status == 0) cl_PLT.MakeCurrent();
  else             cl_CPG.MakeCurrent();
  g_bRun = true;
}

// =========================================================
// Arduino
// =========================================================
void setup() {
  DcsBios::setup();
  // The layout is initially set in the loop() by gSeatChanged=true
}

void loop() {
  // Handle a clean Seat exchange (outside of IST/Callback)
  if (gSeatChanged) {
    gSeatChanged = false;
    applySeatLayout();
  }

  if (g_bRun) {
    DcsBios::loop();
  }
}

#include "audio_definition.h"
#include "def.h"
#include <AT42QT2120.h>
#include <Arduino.h>
#include <Audio.h>
#include <LittleFS.h>
#include <SPI.h>
#include <Wire.h>
#include <button_matrix.h>
#include <debouncer.h>
#include <harp.h>
#include <potentiometer.h>
#include <vector>
#include <USBHost_t36.h>  // Teensy USB Host library
#include <MIDI.h>
bool debug = true;
enum SignalType_enum
{
  LoopStartStopSignal = 0,
  LoopRecordSignal = 1,
  LoopClearSignal = 2
};

uint32_t upLastPressTime;
uint32_t downLastPressTime;
bool upPressed = false;
bool downPressed = false;
bool listenShift = false;
typedef struct _MidiEvent {
  uint32_t timeStamp;
  uint8_t status;
  uint8_t data1;
  uint8_t data2;
  bool record = true;
} midiEvent;
std::vector<midiEvent> LooperSignalNotes; 
bool isLooperToBeRecording = false;
bool isLooperToBeStarting = false;
bool isLooperActive = false; //looper is playing
bool isLooperRecording = false; //looper is recording
bool isLooperClearing = false;//
uint32_t curLooperTick = 0; //current time in terms of tick
uint32_t curLooperCnt = 0;

std::vector<midiEvent> looperBuffer;
std::vector<midiEvent> looperRecordBuffer;

void addMidiLooperEvent(midiEvent m, uint8_t channel, bool isSignal = false)
{
  switch (channel)
  {
    case HARP_CHANNEL:
      channel = HARP_LOOPER_CHANNEL;
      break;
    case CHORD_CHANNEL:
      channel = CHORD_LOOPER_CHANNEL;
      break;
    case EXTERNAL_CHANNEL:
      channel = EXTERNAL_LOOPER_CHANNEL;
      break;
    default:
      channel = EXTERNAL_CHORD_LOOPER_CHANNEL;
      break;
  }
  m.status = m.status | channel;
  
  if (!isSignal)
  {
    if (looperBuffer.size() + looperRecordBuffer.size() > MAX_LOOPER_EVENTS)
    {
      return; //do nothing
    }
    looperRecordBuffer.reserve(1);
    looperRecordBuffer.push_back(m);
  }
  else
  {
    LooperSignalNotes.reserve(1);
    LooperSignalNotes.push_back(m);
  }
  
}

bool pedalUsed = false;
bool externalUseFixedChannel = true;

int volumeOffset = 0;
void handleExtUSBNoteOn(byte channel, byte note, byte velocity);
void handleExtUSBNoteOff(byte channel, byte note, byte velocity);

//>>SOFWTARE VERSION 
int version_ID=0007; //to be read 00.03, stored at adress 7 in memory
//>>BUTTON ARRAYS<<
debouncer harp_array[12];
debouncer chord_matrix_array[22];

//>>HARDWARE SETUP<<
USBHost myusb;
MIDIDevice midi1(myusb);
harp harp_sensor;
button_matrix chord_matrix(SHIFT_DATA_PIN, SHIFT_STORAGE_CLOCK_PIN, SHIFT_CLOCK_PIN, READ_MATRIX_1_PIN, READ_MATRIX_2_PIN, READ_MATRIX_3_PIN);
debouncer hold_button;
debouncer up_button;
debouncer down_button;
debouncer LBO_flag;
bool flag_save_needed=false; //to know if we need to save the preset
potentiometer chord_pot(POT_CHORD_PIN);
potentiometer harp_pot(POT_HARP_PIN);
potentiometer mod_pot(POT_MOD_PIN);
LittleFS_Program myfs; // to save the settings
float color_led_blink_val = 1.0;
bool led_blinking_flag = false;
float led_attenuation = 0.0; 
//>>CHORD DEFINITION<<
//for each chord, we first have the 4 notes of the chord, then decoration that might be used in specific modes
uint8_t major[7] = {0, 4, 7, 12, 2, 5, 9};  // After the four notes of the chord (fundamental, third, fifth of seven, and octave of fifth, the next notes are the second fourth and sixth)
uint8_t minor[7] = {0, 3, 7, 12, 1, 5, 8};
uint8_t maj_sixth[7] = {0, 4, 7, 9, 2, 5, 12};
uint8_t min_sixth[7] = {0, 3, 7, 9, 1, 5, 12};
uint8_t seventh[7] = {0, 4, 10, 7, 2, 5, 9};
uint8_t maj_seventh[7] = {0, 4, 11, 7, 2, 5, 9};
uint8_t min_seventh[7] = {0, 3, 10, 7, 1, 5, 8};
uint8_t aug[7] = {0, 4, 8, 12, 2, 5, 9};
uint8_t dim[7] = {0, 3, 6, 12, 2, 5, 9};
uint8_t full_dim[7] = {0, 3, 6, 9, 2, 5, 12};
uint8_t key_signature_selection = 0; // 0=C, 1=G, 2=D, 3=A, 4=E, 5=B, 6=F, 7=Bb, 8=Eb, 9=Ab, 10=Db, 11=Gb
enum KeySig { // Enums for KeySigs
  KEY_SIG_C, KEY_SIG_G, KEY_SIG_D, KEY_SIG_A, KEY_SIG_E, KEY_SIG_B,
  KEY_SIG_F, KEY_SIG_Bb, KEY_SIG_Eb, KEY_SIG_Ab, KEY_SIG_Db, KEY_SIG_Gb
};
enum Button { // Button enum in hardware order: B, E, A, D, G, C, F
  BTN_B, BTN_E, BTN_A, BTN_D, BTN_G, BTN_C, BTN_F
};
enum FrameShift { //Enums for chord frame shifts
  FRAMESHIFT_0, FRAMESHIFT_1,FRAMESHIFT_2,FRAMESHIFT_3,FRAMESHIFT_4,FRAMESHIFT_5,FRAMESHIFT_6
};
const int8_t base_notes[7] = {11, 4, 9, 2, 7, 0, 5}; // Base note offsets for buttons in key of C (relative to C4 = MIDI 60), in hardware order B, E, A, D, G, C, F
const int8_t key_offsets[12] = {0, 7, 2, 9, 4, 11, 5, 10, 3, 8, 1, 6}; // Circle of fifths: semitone offset for each key’s root note relative to C: C, G, D, A, E, B, F, Bb, Eb, Ab, Db, Gb
const int8_t key_signatures[12] = {0, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 6}; // Number of sharps or flats for each key: Sharps for C, G, D, A, E, B; flats for F, Bb, Eb, Ab, Db, Gb
const int8_t sharp_notes[6][6] = { // Notes affected by sharps in each key, in hardware order (B, E, A, D, G, C, F)
  {BTN_F},          // 1 sharp: F#
  {BTN_F, BTN_C},   // 2 sharps: F#, C#
  {BTN_F, BTN_C, BTN_G}, // 3 sharps: F#, C#, G#
  {BTN_F, BTN_C, BTN_G, BTN_D}, // 4 sharps: F#, C#, G#, D#
  {BTN_F, BTN_C, BTN_G, BTN_D, BTN_A}, // 5 sharps: F#, C#, G#, D#, A#
  {BTN_F, BTN_C, BTN_G, BTN_D, BTN_A, BTN_E} // 6 sharps: F#, C#, G#, D#, A#, E#
};
const int8_t flat_notes[6][6] = { // Notes affected by flats in each key, in hardware order (B, E, A, D, G, C, F)
  {BTN_B},          // 1 flat: Bb
  {BTN_B, BTN_E},   // 2 flats: Bb, Eb
  {BTN_B, BTN_E, BTN_A}, // 3 flats: Bb, Eb, Ab
  {BTN_B, BTN_E, BTN_A, BTN_D}, // 4 flats: Bb, Eb, Ab, Db
  {BTN_B, BTN_E, BTN_A, BTN_D, BTN_G}, // 5 flats: Bb, Eb, Ab, Db, Gb
  {BTN_B, BTN_E, BTN_A, BTN_D, BTN_G, BTN_C} // 6 flats: Bb, Eb, Ab, Db, Gb, Cb
};

float c_frequency = 130.81;                      // for C3
uint8_t chord_octave_change=4;
uint8_t harp_octave_change=4;
uint8_t chord_frame_shift=0;
uint8_t transpose_semitones=0;                       // to use to transpose the instrument, number of semitones
uint8_t (*current_chord)[7] = &major;            // the array holding the current chord
uint8_t current_chord_notes[7];                  // the array for the note calculation within the chord, calculate 7 of them for the arpeggiator mode
uint8_t current_applied_chord_notes[7];          // the array for the note calculation within the chord
uint8_t current_harp_notes[12];                  // the array for the note calculation within the string

//>>SWITCHING LOGIC GLOBAL VARIABLES<<
int8_t current_line = -1;      // holds the current selected line of button, -1 if nothing is on
int8_t fundamental = 0;        // holds the value of the last selected line, hence the fundamental
uint8_t slash_value = 0;       // stores the "slash", ie when a different alternative note is selected
bool slash_chord = false;      // flag for when a slashed chord is currently activated
bool button_pushed = false;    // flag for when any button has been pushed during the main loop
bool trigger_chord = false;    // flag to trigger the enveloppe of the chord
bool sharp_active = false;     // flag for when the sharp is active
bool flat_button_modifier= false; //flag to set the modifier to flat instead of sharp
bool continuous_chord = false; // wether the chord is held continuously. Controlled by the "hold" button
bool rythm_mode = false;
bool barry_harris_mode = false;
IntervalTimer note_timer[4]; // timers for delayed chord enveloppe
bool inhibit_button=false;

//>>SWITCHING LOGIC PARAMETERS<<
uint8_t note_slash_level = 0;     // the level we are replacing in the chord when slashing (usually the fundamental)
bool retrigger_chord = true;      // wether or not to retrigger the enveloppe when the chord is switched within current line (including when selecting slash chord)
bool change_held_strings = false; // to control wether hold strings change with chord:
bool chromatic_harp_mode = false; // to switch the harp to chromatic mode
//>>SYSEX PARAMETERS<<
// SYSEX midi message are used to control up to 256 synthesis parameters.
const uint16_t parameter_size = 256;
const uint8_t preset_number = 12;
int16_t default_bank_sysex_parameters[preset_number][parameter_size] = {
  {0,0,50,50,512,512,512,0,0,0,192,100,49,100,184,100,157,100,0,0,0,0,0,0,43,0,50,37,38,67,0,0,0,0,0,0,0,0,0,0,0,16,0,8,8,12,42,1171,1,423,20,70,3,35,83,59,2658,1,0,0,0,0,0,0,0,1,1,1,100,1,1,0,1,1,1,1,14,0,0,70,0,0,0,100,0,6,0,0,755,195,23,61,29,0,0,0,0,162,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,13,8,100,16,0,200,0,0,50,0,50,18,32,50,0,0,10,66,353,65,995,1,569,16,141,32,83,28,48,54,1,0,0,0,56,0,389,0,20,0,0,0,0,1,1,1,0,1,1,0,1,1,1,1,0,0,0,70,0,0,0,100,0,64,0,0,80,16,4,94,753,474,70,5,100,100,100,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,6,6,32,0,6,0,16,0,6,6,32,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,194,100,85,100,60,100,61,100,0,0,10,1,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,4,25,8,3,29,18,65,488,3,159,25,70,5,18,4,26,6,1,77,0,587,32,0,390,0,76,1,1,100,1,1,68,17,14,22,20,0,340,1682,70,48,0,0,100,18,54,0,0,800,70,57,100,100,0,0,0,0,199,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,7,0,100,0,0,100,6,8,200,2,50,36,75,50,28,0,3,1,1,80,1218,2,1659,38,114,19,8,32,80,1,1,0,30,0,0,0,0,0,0,0,24,0,3,1,1,1,100,1,1,100,1,1,1,1,31,0,0,70,0,0,0,100,0,33,2,1,162,16,4,100,100,1436,118,100,50,32,107,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,13,0,4,0,7,0,0,2,13,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,192,100,49,100,184,100,157,100,0,0,30,0,1,1,87,33,62,12,80,67,0,0,0,0,0,0,0,0,0,0,0,6,3,8,8,12,42,1855,1,42,20,217,3,35,83,59,2658,1,185,0,282,14,0,247,11,1,1,1,100,1,1,0,1,1,1,1,14,0,0,70,0,0,0,100,0,23,0,0,755,195,82,61,29,0,0,0,0,162,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,13,0,100,23,8,200,0,0,50,0,50,18,32,50,0,0,10,66,353,44,1452,1,569,16,141,32,83,28,48,54,1,0,698,82,56,0,579,0,28,0,0,0,0,1,1,1,0,1,1,100,1,1,1,1,0,0,0,70,0,0,0,100,0,100,0,0,80,16,4,94,753,474,70,5,100,100,100,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,6,6,32,0,6,0,16,0,6,6,32,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,196,76,92,100,184,100,85,100,0,0,60,0,1,0,14,0,0,0,42,46,0,0,0,0,0,0,0,0,0,0,3,10,8,11,42,30,61,2137,1,106,22,140,4,35,83,24,1956,1,139,0,282,0,0,247,10,1,1,1,100,1,1,0,1,1,1,1,0,0,0,70,0,0,0,100,0,57,0,1,858,70,66,100,100,0,0,0,0,127,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,17,8,200,11,0,134,10,0,100,7,50,59,15,50,0,15,10,10,45,61,1489,1,652,16,84,32,21,15,33,19,1,0,490,0,109,0,252,0,8,0,244,1,49,1,1,1,15,1,1,100,1,1,1,1,26,479,1931,70,0,50,0,100,40,78,21,0,162,16,4,100,1000,639,140,100,47,100,85,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,65,0,12,0,0,12,0,0,65,0,6,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,146,100,76,100,85,79,184,100,0,0,110,0,0,0,57,0,0,0,67,58,0,0,0,0,0,0,0,0,0,0,0,6,8,8,65,12,38,1855,5,57,24,152,3,35,83,59,2658,6,137,0,640,8,0,247,11,1,1,1,100,1,1,131,4,116,3,1,11,424,1360,70,46,0,0,100,60,85,0,0,996,125,82,61,71,0,0,0,0,90,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0,100,23,0,200,15,8,100,0,50,0,18,50,0,0,2,293,17,44,1012,1,10,33,141,32,83,28,67,239,1,0,0,0,287,0,579,0,0,0,366,0,35,5,52,12,0,1,1,100,1,1,1,1,16,0,0,70,0,0,0,100,0,50,0,0,184,16,4,100,100,657,70,86,100,100,95,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,2,4,2,15,0,2,2,6,2,6,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,192,60,47,56,159,100,157,100,0,0,138,1,0,0,0,0,0,0,27,74,0,0,0,0,0,0,0,0,0,0,4,14,11,3,29,18,65,1102,3,302,11,70,5,18,4,26,6,1,38,0,516,16,0,0,0,76,1,1,100,1,1,68,17,14,22,20,0,340,1682,70,48,0,0,100,18,58,0,0,800,70,43,100,100,0,0,0,0,200,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,10,11,100,9,0,200,4,8,200,0,50,36,75,50,0,0,3,1,1,80,1855,2,769,15,114,19,8,32,80,1,1,0,30,0,11,0,467,0,35,0,24,0,3,1,1,1,100,1,1,100,1,1,1,1,0,341,2164,70,54,0,0,100,36,42,0,1,80,16,4,100,20,2101,70,100,6,22,55,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,4,8,17,0,12,0,1,2,4,8,16,6,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,192,100,61,100,130,100,83,100,0,0,175,0,0,0,0,0,0,0,0,35,0,0,0,0,0,0,0,0,0,0,0,12,9,11,1,1,70,2141,1,383,17,302,101,103,34,30,1,12,0,0,367,17,0,363,5,1,1,1,86,1,1,0,1,1,1,1,8,0,0,70,0,0,0,100,0,33,50,0,5000,70,100,0,0,0,0,0,0,90,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,9,9,200,13,6,99,6,0,50,27,50,50,50,50,0,0,345,1,1,80,800,32,2200,0,70,1,1,1,100,1,1,0,30,0,0,0,166,0,6,0,0,0,0,1,1,1,0,1,1,100,1,1,1,1,0,0,0,70,0,0,0,100,0,59,100,0,80,16,4,100,100,2366,70,100,40,23,57,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,4,4,16,16,12,0,1,6,8,6,0,6,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,146,100,76,100,85,79,184,100,0,0,220,0,0,0,0,0,0,86,55,63,0,0,0,0,0,0,0,0,0,0,0,6,8,8,65,12,38,1855,5,57,24,152,3,35,83,59,2658,6,137,0,640,8,0,247,11,1,1,1,100,1,1,131,4,116,3,1,11,424,1360,70,46,0,0,100,60,84,0,0,996,125,82,61,71,0,0,0,0,90,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,0,100,23,0,200,15,8,100,0,50,0,18,50,0,0,2,293,17,44,1012,1,10,33,141,32,83,28,67,239,1,0,0,0,287,0,579,0,0,0,366,0,35,5,52,12,0,1,1,100,1,1,1,1,16,0,0,70,0,0,0,100,0,50,0,0,184,16,4,100,100,657,70,86,100,100,95,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,2,4,2,15,0,2,2,6,2,6,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,196,100,47,56,159,100,157,100,0,0,253,1,0,0,0,0,0,0,0,59,0,0,0,0,0,0,0,0,0,0,4,15,11,3,29,18,65,1102,3,302,11,70,5,18,4,26,6,1,38,0,516,16,0,0,12,76,1,22,100,1,1,68,17,14,22,20,0,340,1682,70,48,0,0,100,18,58,0,0,800,70,43,100,100,0,0,0,0,158,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,10,0,100,9,0,200,4,8,200,0,50,36,75,50,0,0,3,1,1,80,1855,2,769,15,114,19,8,32,80,1,1,0,30,0,11,0,467,0,22,0,24,0,3,1,1,1,100,1,1,100,1,1,1,1,0,341,2164,70,54,0,0,100,36,42,0,1,80,16,4,100,20,532,70,100,6,82,96,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,4,8,17,0,12,0,1,2,4,8,16,6,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,149,69,76,100,85,79,184,100,0,0,266,0,0,0,0,0,0,0,65,70,0,0,0,0,0,0,0,0,0,0,0,5,0,8,65,12,38,1855,5,57,24,152,3,35,83,59,2658,6,137,0,640,8,0,247,11,1,1,1,100,1,1,131,4,116,3,1,0,424,1360,70,46,0,0,100,60,81,0,0,996,125,82,61,71,0,0,0,0,108,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,20,12,100,23,0,200,15,8,100,0,50,45,17,0,0,0,2,293,17,44,1012,1,10,33,141,32,83,28,67,239,1,0,0,0,287,0,579,0,0,0,366,0,35,5,52,12,0,1,1,100,1,1,1,1,16,0,0,70,0,0,0,100,0,50,0,0,184,16,4,100,100,857,70,42,100,100,158,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,2,4,2,15,0,2,2,6,2,6,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,192,55,61,100,184,100,85,100,0,0,310,0,0,0,86,0,56,87,37,60,0,0,0,0,0,0,0,0,0,0,4,15,12,36,42,30,59,1410,1,30,33,140,4,35,83,24,615,1,94,0,282,15,0,247,0,1,1,1,100,1,1,0,1,1,1,1,0,0,0,70,0,0,0,100,0,40,0,1,858,70,46,100,82,0,0,0,0,124,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,16,8,200,18,0,134,21,0,200,2,50,59,15,50,18,15,10,66,43,35,711,1,271,16,84,32,83,733,31,54,1,0,698,0,220,0,252,0,8,0,244,1,49,1,1,1,15,1,1,100,1,1,1,1,26,479,1931,70,0,50,0,100,34,75,0,0,162,16,4,100,1000,1889,116,49,100,42,129,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,65,0,12,0,0,12,0,0,65,0,6,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,50,50,512,512,512,0,0,0,194,100,85,100,60,100,61,100,0,0,340,1,0,0,0,0,0,0,0,62,0,0,0,0,0,0,0,0,0,0,4,25,3,3,29,18,65,488,3,159,25,70,5,18,4,26,40,1,77,0,587,32,0,715,11,76,1,1,100,1,1,68,17,14,22,20,17,340,1682,70,48,0,0,100,18,54,0,0,800,70,57,100,100,0,0,0,0,199,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,1,100,0,0,100,6,8,200,2,50,36,75,50,28,0,3,1,1,80,1218,2,706,38,114,19,8,32,80,1,1,0,30,0,0,0,0,0,0,0,24,0,3,1,1,1,100,1,1,100,1,1,1,1,31,0,0,70,0,0,0,100,0,33,2,1,162,16,4,100,100,678,118,100,50,32,168,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,13,0,4,0,7,0,0,2,13,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
}; 
int16_t current_sysex_parameters[parameter_size] = {0,0,50,50,512,512,512,1,0,0,192,100,49,100,184,100,157,100,0,0,0,0,0,0,0,0,0,0,0,67,0,0,0,0,0,0,0,0,0,0,0,16,0,8,8,12,42,1171,1,423,20,70,3,35,83,59,2658,1,0,0,0,0,0,0,0,1,1,1,100,1,1,0,1,1,1,1,14,0,0,70,0,0,0,100,0,6,0,0,755,195,23,61,29,0,0,0,0,162,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,13,8,100,16,0,200,0,0,50,0,50,18,32,50,0,0,10,66,353,65,995,1,569,16,141,32,83,28,48,54,1,0,0,0,56,0,389,0,20,0,0,0,0,1,1,1,0,1,1,0,1,1,1,1,0,0,0,70,0,0,0,100,0,38,0,0,80,16,4,94,753,474,70,5,100,100,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,6,6,32,0,6,0,16,0,6,6,32,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const char *bank_name[preset_number] = {"a.txt", "b.txt", "c.txt", "d.txt", "e.txt", "f.txt", "g.txt", "h.txt", "i.txt", "j.txt", "k.txt", "l.txt"};
int8_t current_bank_number = 0;
float bank_led_hue = 0;
// Reserved SYSEX adresses
// =0 is for the control command
// 1 is the bank ID
// >2 and <10 are protected
int8_t harp_volume_sysex = 2;
int8_t chord_volume_sysex = 3;
int8_t chord_pot_alternate_storage = 4;
int8_t harp_pot_alternate_storage = 5;
int8_t mod_pot_alternate_storage = 6;

// >10 and <21 are limited access, for example the potentiometer settings (we don't want a pot to control another pot sysex adress or range)
int8_t chord_pot_alternate_control = 10;
int8_t chord_pot_alternate_range = 11;
int8_t harp_pot_alternate_control = 12;
int8_t harp_pot_alternate_range = 13;
int8_t mod_pot_main_control = 14;
int8_t mod_pot_main_range = 15;
int8_t mod_pot_alternate_control = 16;
int8_t mod_pot_alternate_range = 17;
// 21-39 are global parameters (switching logic, global reverb etc.)
// 40-119 are harp parameters
// 120-219 are chord parameters
// 220-235 are rythm patterns
bool sysex_controler_connected=false; //bool to remember if there is a controller that is connected to avoid saving any change

//>>AUDIO OBJECT ARRAYS<<
// for the strings
AudioSynthWaveformModulated *string_waveform_array[12] = {&waveform_string_1, &waveform_string_2, &waveform_string_3, &waveform_string_4, &waveform_string_5, &waveform_string_6, &waveform_string_7, &waveform_string_8, &waveform_string_9, &waveform_string_10, &waveform_string_11, &waveform_string_12};
AudioEffectEnvelope *string_enveloppe_array[12] = {&envelope_string_1, &envelope_string_2, &envelope_string_3, &envelope_string_4, &envelope_string_5, &envelope_string_6, &envelope_string_7, &envelope_string_8, &envelope_string_9, &envelope_string_10, &envelope_string_11, &envelope_string_12};
AudioEffectEnvelope *string_enveloppe_filter_array[12] = {&envelope_filter_1, &envelope_filter_2, &envelope_filter_3, &envelope_filter_4, &envelope_filter_5, &envelope_filter_6, &envelope_filter_7, &envelope_filter_8, &envelope_filter_9, &envelope_filter_10, &envelope_filter_11, &envelope_filter_12};
AudioMixer4 *string_mixer_array[3] = {&string_mix_1, &string_mix_2, &string_mix_3};
AudioFilterStateVariable *string_filter_array[12] = {&filter_string_1, &filter_string_2, &filter_string_3, &filter_string_4, &filter_string_5, &filter_string_6, &filter_string_7, &filter_string_8, &filter_string_9, &filter_string_10, &filter_string_11, &filter_string_12};
AudioSynthWaveform *string_transient_waveform_array[12] = {&waveform_transient_1, &waveform_transient_2, &waveform_transient_3, &waveform_transient_4, &waveform_transient_5, &waveform_transient_6, &waveform_transient_7, &waveform_transient_8, &waveform_transient_9, &waveform_transient_10, &waveform_transient_11, &waveform_transient_12};
AudioEffectEnvelope *string_transient_envelope_array[12] = {&envelope_transient_1, &envelope_transient_2, &envelope_transient_3, &envelope_transient_4, &envelope_transient_5, &envelope_transient_6, &envelope_transient_7, &envelope_transient_8, &envelope_transient_9, &envelope_transient_10, &envelope_transient_11, &envelope_transient_12};
AudioMixer4 *transient_mixer_array[3] = {&transient_mix_1, &transient_mix_2, &transient_mix_3};
// for the chord
AudioEffectEnvelope *chord_vibrato_envelope_array[4] = {&voice1_vibrato_envelope, &voice2_vibrato_envelope, &voice3_vibrato_envelope, &voice4_vibrato_envelope};
AudioEffectEnvelope *chord_vibrato_dc_envelope_array[4] = {&voice1_vibrato_dc_envelope, &voice2_vibrato_dc_envelope, &voice3_vibrato_dc_envelope, &voice4_vibrato_dc_envelope};
AudioMixer4 *chord_vibrato_mixer_array[4] = {&voice1_vibrato_mixer, &voice2_vibrato_mixer, &voice3_vibrato_mixer, &voice4_vibrato_mixer};
AudioSynthWaveformModulated *chord_osc_1_array[4] = {&voice1_osc1, &voice2_osc1, &voice3_osc1, &voice4_osc1};
AudioSynthWaveformModulated *chord_osc_2_array[4] = {&voice1_osc2, &voice2_osc2, &voice3_osc2, &voice4_osc2};
AudioSynthWaveformModulated *chord_osc_3_array[4] = {&voice1_osc3, &voice2_osc3, &voice3_osc3, &voice4_osc3};
AudioSynthWaveformDc *chord_freq_dc_array[4]= {&voice1_frequency_dc, &voice2_frequency_dc, &voice3_frequency_dc, &voice4_frequency_dc};
AudioSynthNoiseWhite *chord_noise_array[4] = {&voice1_noise, &voice2_noise, &voice3_noise, &voice4_noise};
AudioMixer4 *chord_voice_mixer_array[4] = {&voice1_mixer, &voice2_mixer, &voice3_mixer, &voice4_mixer};
AudioFilterStateVariable *chord_voice_filter_array[4] = {&voice1_filter, &voice2_filter, &voice3_filter, &voice4_filter};
AudioEffectEnvelope *chord_envelope_filter_array[4] = {&voice1_envelope_filter, &voice2_envelope_filter, &voice3_envelope_filter, &voice4_envelope_filter};
AudioEffectMultiply *chord_tremolo_mult_array[4] = {&voice1_tremolo_mult, &voice2_tremolo_mult, &voice3_tremolo_mult, &voice4_tremolo_mult};
AudioEffectEnvelope *chord_envelope_array[4] = {&voice1_envelope, &voice2_envelope, &voice3_envelope, &voice4_envelope};

//clones
AudioSynthWaveformModulated *string_waveform_array1[12] = {&waveform_string_18, &waveform_string_19, &waveform_string_20, &waveform_string_17, &waveform_string_21, &waveform_string_24, &waveform_string_22, &waveform_string_23, &waveform_string_13, &waveform_string_16, &waveform_string_14, &waveform_string_15};
AudioEffectEnvelope *string_enveloppe_array1[12] = {&envelope_string_18, &envelope_string_19, &envelope_string_20, &envelope_string_17, &envelope_string_21, &envelope_string_24, &envelope_string_22, &envelope_string_23, &envelope_string_13, &envelope_string_16, &envelope_string_14, &envelope_string_15};
AudioEffectEnvelope *string_enveloppe_filter_array1[12] = {&envelope_filter_18, &envelope_filter_19, &envelope_filter_20, &envelope_filter_17, &envelope_filter_21, &envelope_filter_24, &envelope_filter_22, &envelope_filter_23, &envelope_filter_13, &envelope_filter_16, &envelope_filter_14, &envelope_filter_15};
AudioMixer4 *string_mixer_array1[3] = {&string_mix_5, &string_mix_6, &string_mix_4};
AudioFilterStateVariable *string_filter_array1[12] = {&filter_string_18, &filter_string_19, &filter_string_20, &filter_string_17, &filter_string_21, &filter_string_24, &filter_string_22, &filter_string_23, &filter_string_13, &filter_string_16, &filter_string_14, &filter_string_15};
AudioSynthWaveform *string_transient_waveform_array1[12] = {&waveform_transient_18, &waveform_transient_19, &waveform_transient_20, &waveform_transient_17, &waveform_transient_15, &waveform_transient_16, &waveform_transient_13, &waveform_transient_14, &waveform_transient_22, &waveform_transient_21, &waveform_transient_23, &waveform_transient_24};
AudioEffectEnvelope *string_transient_envelope_array1[12] = {&envelope_transient_18, &envelope_transient_19, &envelope_transient_20, &envelope_transient_17, &envelope_transient_15, &envelope_transient_16, &envelope_transient_13, &envelope_transient_14, &envelope_transient_22, &envelope_transient_21, &envelope_transient_23, &envelope_transient_24};
AudioMixer4 *transient_mixer_array1[3] = {&transient_mix_5, &transient_mix_4, &transient_mix_6};
// for the chord
AudioEffectEnvelope *chord_vibrato_envelope_array1[4] = {&voice1_vibrato_envelope1, &voice2_vibrato_envelope1, &voice3_vibrato_envelope1, &voice4_vibrato_envelope1};
AudioEffectEnvelope *chord_vibrato_dc_envelope_array1[4] = {&voice1_vibrato_dc_envelope1, &voice2_vibrato_dc_envelope1, &voice3_vibrato_dc_envelope1, &voice4_vibrato_dc_envelope1};
AudioMixer4 *chord_vibrato_mixer_array1[4] = {&voice1_vibrato_mixer1, &voice2_vibrato_mixer1, &voice3_vibrato_mixer1, &voice4_vibrato_mixer1};
AudioSynthWaveformModulated *chord_osc_1_array1[4] = {&voice_osc12, &voice2_osc5, &voice3_osc5, &voice4_osc6};
AudioSynthWaveformModulated *chord_osc_2_array1[4] = {&voice1_osc5, &voice_osc23, &voice3_osc4, &voice4_osc5};
AudioSynthWaveformModulated *chord_osc_3_array1[4] = {&voice1_osc4, &voice2_osc4, &voice_osc34, &voice4_osc4};
AudioSynthWaveformDc *chord_freq_dc_array1[4]= {&voice1_frequency_dc1, &voice2_frequency_dc1, &voice3_frequency_dc1, &voice4_frequency_dc1};
AudioSynthNoiseWhite *chord_noise_array1[4] = {&voice1_noise1, &voice2_noise1, &voice3_noise1, &voice4_noise1};
AudioMixer4 *chord_voice_mixer_array1[4] = {&voice1_mixer1, &voice2_mixer1, &voice3_mixer1, &voice4_mixer1};
AudioFilterStateVariable *chord_voice_filter_array1[4] = {&voice1_filter1, &voice2_filter1, &voice3_filter1, &voice4_filter1};
AudioEffectEnvelope *chord_envelope_filter_array1[4] = {&voice1_envelope_filter1, &voice2_envelope_filter1, &voice3_envelope_filter1, &voice4_envelope_filter1};
AudioEffectMultiply *chord_tremolo_mult_array1[4] = {&voice1_tremolo_mult1, &voice2_tremolo_mult1, &voice3_tremolo_mult1, &voice4_tremolo_mult1};
AudioEffectEnvelope *chord_envelope_array1[4] = {&voice1_envelope1, &voice2_envelope1, &voice3_envelope1, &voice4_envelope1};
#include <Arduino.h>
#include <malloc.h>

extern "C" char _stext;  // Start of program code
extern "C" char _etext;  // End of program code
extern "C" char _sbss;   // Start of BSS (zero-initialized vars)
extern "C" char _ebss;   // End of BSS
extern "C" char _sdata;  // Start of data section
extern "C" char _edata;  // End of data section

void printMemoryUsage() {
  struct mallinfo mi = mallinfo();

  // Heap information
  uint32_t heapUsed = mi.uordblks;

  // Stack information
  char stackTop;
  uint32_t stackUsed = (uint32_t)&stackTop - (uint32_t)&_ebss;

  // Teensy 4.0 has 512 KB tightly coupled RAM (DTCM + ITCM)
  constexpr uint32_t totalRAM = 512 * 1024;

  uint32_t totalUsed = heapUsed + stackUsed;
  uint32_t totalFree = totalRAM - totalUsed;

  Serial.println("\n===== MEMORY USAGE (Teensy 4.x) =====");
  Serial.printf("Code (.text):  %lu bytes\n", (uint32_t)(&_etext - &_stext));
  Serial.printf("Data (.data):  %lu bytes\n", (uint32_t)(&_edata - &_sdata));
  Serial.printf("BSS (.bss):    %lu bytes\n", (uint32_t)(&_ebss - &_sbss));
  Serial.printf("Heap Used:     %lu bytes\n", heapUsed);
  Serial.printf("Stack Used:    %lu bytes\n", stackUsed);
  Serial.printf("--------------------------------------\n");
  Serial.printf("Total Used:    %lu bytes\n", totalUsed);
  Serial.printf("Free RAM:      %lu bytes\n", totalFree);
  Serial.printf("======================================\n\n");
}


bool memShown = false;
//>>SYNTHESIS VARIABLE<<
// waveshaper shape
float wave_shape[257] = {};
float ws_sin_param = 1;
// waveform array 
int8_t waveform_array[12] = {
    0, //WAVEFORM_SINE
    1, //WAVEFORM_SAWTOOTH
    2, //WAVEFORM_SQUARE
    3, //WAVEFORM_TRIANGLE
    12, //WAVEFORM_BANDLIMIT_PULSE
    5, //WAVEFORM_PULSE
    6, //WAVEFORM_SAWTOOTH_REVERSE
    7, //WAVEFORM_SAMPLE_HOLD 
    8, //WAVEFORM_TRIANGLE_VARIABLE
    9, //WAVEFORM_BANDLIMIT_SAWTOOTH
    10,//WAVEFORM_BANDLIMIT_SAWTOOTH_REVERSE
    11, //WAVEFORM_BANDLIMIT_SQUARE
}; 
// shuffling arrays and index for the harp
int8_t harp_shuffling_array[7][12] = {
    //each number indicates the note for the string 0-6 are taken within the chord pattern. 
    //the /10 number indicates the octave
    {0, 1, 2, 10, 11, 12, 20, 21, 22, 30, 31, 32},
    {4, 1, 0, 2, 14, 11, 10, 12, 24, 21, 20, 22}, //add the seconds
    {5, 2, 0, 1, 15, 12, 10, 11, 25, 22, 20, 21}, //add the fourth
    {6, 2, 0, 1, 16, 12, 10, 11, 26, 22, 20, 21}, //add the sixth
    {0, 1, 2, 3, 10, 11, 12, 13, 20, 21, 22, 23}, //replaced octave by barry_harris shuffling array 
    {0, 4, 1, 5, 2, 6, 10, 14, 11, 15, 12, 16}, //chromatic
    {0, 10, 20, 1, 11, 21, 2, 12, 22, 3, 13, 23}}; //special array for keymaster/barry_harris combo
int8_t harp_shuffling_selection = 0;
int8_t transient_note_level=0; //level of the note of the transient in the scale;
int8_t chord_shuffling_array[6][7] = {
    //each number indicates the note for the voice 0-6 are taken within the chord pattern. In normal mode, only 0-3 is used, and 4-6 is available in rythm mode 
    //the /10 number indicates the octave
    {0, 1, 2, 3, 4, 5, 6}, //normal 
    {10, 11, 12, 13, 14, 15, 16},//one octave up with chromatics
    {10, 11, 12, 13, 0, 2, 3},//one octave up with low chord notes
    {10, 11, 12, 13, 2, 5, 6},//one octave up with low fifth and low chromatics
    {10, 11, 12, 13, 2, 15, 16},//one octave up with low fifth and high chromatics
    {20, 21, 22, 23, 24, 25, 26}};//two octave up
int8_t chord_shuffling_selection = 0;
// retrigger release for chord delayed note

int chord_retrigger_release=0;
int glide_length=0;
// strings filter parameters
float string_filter_keytrack = 0;
int string_filter_base_freq = 0;
// lfo for chord parameters
float chord_vibrato_base_freq = 0;
float chord_vibrato_keytrack = 0;
float chord_tremolo_base_freq = 0;
float chord_tremolo_keytrack = 0;
float chord_filter_base_freq = 0;
float chord_filter_keytrack = 0;
// frequency mutlipliers for chord
float osc_1_freq_multiplier = 1;
float osc_2_freq_multiplier = 1;
float osc_3_freq_multiplier = 1;
// string delay_parameter
u_int32_t inter_string_delay = 30000;
u_int32_t random_delay = 10000;
// pan for audio output 
float pan=1;
float reverb_dry_proportion=0.6; //to avoid drop in volume in full reverb, keep some part of the dry signal in

//>>AUTO RYTHM<<
u_int8_t rythm_pattern[16] = {};
float rythm_bpm = 80;
u_int8_t rythm_current_step = 0;
u_int16_t note_pushed_duration = 30;
float shuffle = 1;
u_int32_t long_timer_period = shuffle * (60 * 1000 * 1000) / (2 * rythm_bpm);
u_int32_t short_timer_period = 2 * (60 * 1000 * 1000) / (2 * rythm_bpm) - long_timer_period;
bool current_long_period = true;
bool rythm_timer_running = false;
IntervalTimer rythm_timer;       // that gives the general rythm
IntervalTimer note_off_timer[4]; // timers for delayed chord enveloppe
IntervalTimer led_timer;
IntervalTimer color_led_blink_timer;
elapsedMillis note_off_timing[4];
elapsedMicros last_midi_clock_in;
int midi_clock_current_step=0;

uint8_t rythm_limit_change_to_every = 2; // when we allow the chord change
elapsedMillis since_last_button_push;
elapsedMillis last_key_change;

uint8_t rythm_freeze_current_chord_notes[7]; // this array is needed because we need to handle the situation when a
uint8_t rythm_loop_length = 16;
u_int8_t current_selected_voice=0; //increment at each voice steal to rotate amongst voices;

//-->>MIDI PARAMETERS
uint8_t chord_port=0;
uint8_t chord_attack_velocity=127;
uint8_t chord_attack_velocity_PedalUsed=90;
uint8_t chord_attack_velocity_noPedal=127;
uint8_t chord_release_velocity=20;
uint8_t chord_started_notes[4]={0,0,0,0};                   
uint8_t harp_port=1;
uint8_t harp_attack_velocity=127; 
uint8_t harp_attack_velocity_PedalUsed=90; 
uint8_t harp_attack_velocity_noPedal=127; 
uint8_t harp_release_velocity=20;
uint8_t harp_started_notes[12]={0,0,0,0,0,0,0,0,0,0,0,0};    
uint8_t midi_base_note=48; // for C3
uint8_t midi_base_note_transposed=midi_base_note; //to handle note transposition

//-->>FUNCTION THAT NEED ANNOUNCING
void save_config(int bank_number, bool default_save);
void load_config(int bank_number);
void recalculate_timer();
uint8_t calculate_note_harp(uint8_t string, bool slashed, bool sharp);
uint8_t calculate_note_chord(uint8_t voice, bool slashed, bool sharp);
void set_chord_voice_frequency(uint8_t i, uint16_t current_note);
void calculate_ws_array();
void rythm_tick_function();

//-->>LED HSV CALCULATION
// function to calculate led RGB value, thank you SO
void set_led_color(float h, float s, float v) {
  double hh, p, q, t, ff;
  long i;
  double r, g, b;
  if (s <= 0.0) {
    r = v;
    g = v;
    b = v;
    analogWrite(R_LED_PIN, 0);
    analogWrite(G_LED_PIN, 0);
    analogWrite(B_LED_PIN, 0);
    return;
  }
  hh = h;
  if (hh >= 360.0)
    hh = 0.0;
  hh /= 60.0;
  i = (long)hh;
  ff = hh - i;
  p = v * (1.0 - s);
  q = v * (1.0 - (s * ff));
  t = v * (1.0 - (s * (1.0 - ff)));
  switch (i) {
  case 0:
    r = v;
    g = t;
    b = p;
    break;
  case 1:
    r = q;
    g = v;
    b = p;
    break;
  case 2:
    r = p;
    g = v;
    b = t;
    break;
  case 3:
    r = p;
    g = q;
    b = v;
    break;
  case 4:
    r = t;
    g = p;
    b = v;
    break;
  case 5:
  default:
    r = v;
    g = p;
    b = q;
    break;
  }
  analogWrite(R_LED_PIN, r * 200);
  analogWrite(G_LED_PIN, g * 115);
  analogWrite(B_LED_PIN, b * 70);
  return;
}
void set_harp_voice_frequency(uint8_t i, uint16_t current_note);
//-->>UTILITIES FOR SYSEX HANDLING
void control_command(uint8_t command, uint8_t parameter) {
  switch (command) {
  case 0: // SIGNAL TO SEND BACK ALL DATA
    Serial.println("Reporting all data");
    int8_t midi_data_array[parameter_size * 2];
    for (int i = 0; i < parameter_size; i++) {
      midi_data_array[2 * i] = current_sysex_parameters[i] % 128;
      midi_data_array[2 * i + 1] = current_sysex_parameters[i] / 128;
    }
    usbMIDI.sendSysEx(parameter_size * 2, (const uint8_t *)&midi_data_array,0);
    break;
  case 1: // SIGNAL TO WIPE MEMORY
    Serial.println("Wiping memory");
    digitalWrite(_MUTE_PIN, LOW); // muting the DAC
    myfs.quickFormat();
    current_bank_number = 0;
    load_config(current_bank_number);
    digitalWrite(_MUTE_PIN, HIGH); // unmuting the DAC
    break;
  case 2: // saving bank
    Serial.print("Saving to bank: ");
    Serial.println(parameter);
    save_config(parameter, false);
    break;
  case 3: // setting bank to default
    Serial.print("Saving to bank: ");
    Serial.println(parameter);
    current_bank_number = parameter;
    save_config(parameter, true);
    break;

  default:
    break;
  }
}
const float bendRangeSemitones = 2.0f;
// the autogenerated code (see ./generator for the script)
#include <sysex_handler.h>
void processMIDI(void) {
  byte type;
  type = usbMIDI.getType();
  if (type == usbMIDI.SystemExclusive && usbMIDI.getSysExArrayLength() == 6) 
  {
    sysex_controler_connected=true; //we can say for sure a controller is connected
    const byte *data = usbMIDI.getSysExArray();
    int adress = data[1] + 128 * data[2];
    if (adress == 0) { // it is a control command
      control_command(data[3], data[4]);
    } else {
      Serial.print("Received instruction on address:");
      Serial.print(adress);
      int value = data[3] + 128 * data[4];
      Serial.print(" with value:");
      Serial.println(value);
      current_sysex_parameters[adress] = value;
      apply_audio_parameter(adress, value);
    }
  }
  if(type==usbMIDI.Start && rythm_mode)
  {
    rythm_current_step=0;
    midi_clock_current_step=0;
    rythm_tick_function();
    Serial.println("Start received");
    rythm_timer.end();
  }
  if(type==usbMIDI.Stop && rythm_mode)
  {
    rythm_timer.begin(rythm_tick_function, short_timer_period);
  }


  if(type==usbMIDI.Clock && rythm_mode)
  {
    //here we want half of the cycle to be synced with the midi clock, and half with the calculated internal clock, so we can still have shuffle
    //recalculate the BPM
    rythm_bpm=(rythm_bpm*10+(1000*1000*60/last_midi_clock_in)/24)/11.0;
    last_midi_clock_in=0;
    midi_clock_current_step+=1;
    recalculate_timer();   
    //once every two beat, we sync
    if(midi_clock_current_step==24){
      rythm_timer.begin(rythm_tick_function, short_timer_period);
      rythm_tick_function();
      midi_clock_current_step=0;
    }
    //We disable the timer to avoid having it trigger the tick too early when we arrive at the sync beat 
    if(midi_clock_current_step>18){
      rythm_timer.end();
    }
  }
  if(type==usbMIDI.NoteOn)
    {
      int channel = usbMIDI.getChannel();
      int note = usbMIDI.getData1();
      int velocity = usbMIDI.getData2();
      Serial.printf("Note On: ch=%d, note=%d, vel=%d\n", channel, note, velocity);
        //int c = externalUseFixedChannel ? EXTERNAL_CHANNEL : channel;
        note += 1;
        
        // Map the incoming note to a harp voice or chord voice
        //int harp_index = (note - midi_base_note_transposed) % 12; // assuming 12 harp zones
        int harp_index = note % 12; // assuming 12 harp zones
        int oldAttack = harp_attack_velocity;
        if (harp_index >= 0 && harp_index < 12) {
          current_harp_notes[harp_index] = note;
          set_harp_voice_frequency(harp_index, note);
          
          AudioNoInterrupts();
          harp_attack_velocity = velocity;
          envelope_string_vibrato_lfo1.noteOn();
          envelope_string_vibrato_dc1.noteOn();
          string_enveloppe_filter_array1[harp_index]->noteOn();
          string_enveloppe_array1[harp_index]->noteOn();
          string_transient_envelope_array1[harp_index]->noteOn();
          AudioInterrupts();
          harp_attack_velocity = oldAttack;
        }
    }
    if(type==usbMIDI.NoteOff) 
    {
      int channel = usbMIDI.getChannel();
      int note = usbMIDI.getData1();
      int velocity = usbMIDI.getData2();
      Serial.printf("Note Off: ch=%d, note=%d, vel=%d\n", channel, note, velocity);
        //int c = externalUseFixedChannel ? EXTERNAL_CHANNEL : channel;
        note += 1;
        
        // Map the incoming note to a harp voice or chord voice
        //int harp_index = (note - midi_base_note_transposed) % 12; // assuming 12 harp zones
        int harp_index = note % 12; // assuming 12 harp zones
        
        if (harp_index >= 0 && harp_index < 12) {
          current_harp_notes[harp_index] = note;
          set_harp_voice_frequency(harp_index, note);
          
          AudioNoInterrupts();
          string_enveloppe_array1[harp_index]->noteOff();
          string_transient_envelope_array1[harp_index]->noteOff();
          string_enveloppe_filter_array1[harp_index]->noteOff();
          AudioInterrupts();
        }
    }
    if (type == usbMIDI.ControlChange && usbMIDI.getChannel() == 1)
    {

      int d2 = usbMIDI.getData2();
      int channel = usbMIDI.getChannel();
      chord_attack_velocity = chord_attack_velocity_PedalUsed;
      harp_attack_velocity = harp_attack_velocity_PedalUsed; 
      volumeOffset = d2;
        if (volumeOffset > 127)
        {
          volumeOffset = 127;
        }
        if (volumeOffset < 0)
        {
          volumeOffset = 0;
        }
        
        int clamped = chord_attack_velocity_PedalUsed + floor(volumeOffset/127.0);
        
        float vel_gain = clamped;
        Serial.printf("Velocity Change: ch=%d, raw=%d value=%d gain = %f\n", channel, volumeOffset, clamped, vel_gain);

        AudioNoInterrupts();
        for (int i = 0; i < 12; i++)
        {
          string_waveform_array1[i]->amplitude(0.5 + vel_gain*0.25); //too loud
        }

        for (int i = 0; i < 4; i++)
        {
          chord_voice_mixer_array1[i]->gain(0, 0.7 + vel_gain*0.7);
          chord_voice_mixer_array1[i]->gain(1, 0.7 + vel_gain*0.7);
          chord_voice_mixer_array1[i]->gain(2, 0.7 + vel_gain*0.7);
        }
        Serial.printf("Velocity Change: ch=%d, raw=%d act value=%d\n", channel, volumeOffset, chord_attack_velocity);
        AudioInterrupts();
      
      
    }
    if (type == usbMIDI.PitchBend  && (usbMIDI.getChannel() == 1 || usbMIDI.getChannel() == 2))
    {
        // ---------- PITCH BEND ----------
        int d1 = usbMIDI.getData1();
        int d2 = usbMIDI.getData2();
        int channel = usbMIDI.getChannel();
        // 14-bit raw value from LSB/MSB
        int raw14 = (d2 << 7) | d1;        // 0 .. 16383
        int signed14 = raw14 - 8192;      // -8192 .. 8191

        int mapped = 0; // will hold -8191..8191 (or 0..8191 for channel 2)

        if (channel == 2) {
          if (signed14 <= 0) mapped = 0;
          else mapped = constrain(signed14, 0, 8191);
        } 
        else if (channel == 3) {
          // channel 3 = only downward bend: -8191 .. 0
          if (signed14 <= 0) mapped = 0;
          else mapped = constrain(signed14, 0, 8191) * -1;
        } 
        else {
          // fallback: full signed range (if you ever want other channels to do full-range)
          mapped = constrain(signed14, -8191, 8191);
        }

        // Convert mapped (-8191..8191 or 0..8191) into a semitone offset and then a freq multiplier
        float bendSemis = ( (float)mapped / 8192.0f ) * bendRangeSemitones; // e.g. -2..0 or 0..+2
        float bendRatio = powf(2.0f, bendSemis / 12.0f);

        // --- apply to audio oscillators safely ---
        // NOTE: these sizeof expressions work if these arrays are real static arrays visible here.
        // If they are pointers, replace with your constants (e.g. NUM_CHORD_NOTES, NUM_CHORD_OSC).
        const int numChordNotes = sizeof(current_chord_notes) / sizeof(current_chord_notes[0]);
        const int numChordOsc   = sizeof(chord_osc_1_array)    / sizeof(chord_osc_1_array[0]);
        const int applyVoices = min(numChordNotes, numChordOsc);
        Serial.printf("Pitch Bend: ch=%d, raw=%d value=%d\n", channel,raw14, mapped);
        AudioNoInterrupts();
        for (int i = 0; i < applyVoices; i++) {
          // only modify voices that are active / started
          if (i < (int)(sizeof(chord_started_notes) / sizeof(chord_started_notes[0])) && chord_started_notes[i] != 0) {
            // compute the same base frequency your set_chord_voice_frequency uses
            float baseFreq = pow(2.0f, chord_octave_change) * c_frequency / 8.0f
                            * pow(2.0f, (current_chord_notes[i] + transpose_semitones) / 12.0f);

            chord_osc_1_array1[i]->frequency(osc_1_freq_multiplier * baseFreq * bendRatio);
            chord_osc_2_array1[i]->frequency(osc_2_freq_multiplier * baseFreq * bendRatio);
            chord_osc_3_array1[i]->frequency(osc_3_freq_multiplier * baseFreq * bendRatio);
          }
        }
      
        const int numHarpNotes = (int)(sizeof(current_harp_notes) / sizeof(current_harp_notes[0]));
        const int numStringWave = (int)(sizeof(string_waveform_array) / sizeof(string_waveform_array[0]));
        const int applyHarpVoices = min(numHarpNotes, numStringWave);

        const int numHarpStarted = (int)(sizeof(harp_started_notes) / sizeof(harp_started_notes[0]));

        for (int i = 0; i < applyHarpVoices; i++) {
          if (i < numHarpStarted && harp_started_notes[i] != 0) {
            // compute the same base frequencies as set_harp_voice_frequency, then apply bendRatio
            float note_freq = pow(2.0f, harp_octave_change) * c_frequency / 4.0f
                              * pow(2.0f, (current_harp_notes[i] + transpose_semitones) / 12.0f);
            float transient_freq = 64.0f * c_frequency / 4.0f
                                    * pow(2.0f, ((current_harp_notes[i] + transpose_semitones) % 12 + transient_note_level) / 12.0f);

            // apply bend
            string_waveform_array1[i]->frequency(note_freq * bendRatio);
            string_transient_waveform_array1[i]->frequency(transient_freq * bendRatio);
            // keep filter base freq but keytrack over the bent note freq
            string_filter_array1[i]->frequency(string_filter_base_freq + (note_freq * bendRatio) * string_filter_keytrack);
          }
        }
        
        AudioInterrupts();
      
    }
}

void processMIDI_orig(void) {
  byte type;
  type = usbMIDI.getType();
  if (type == usbMIDI.SystemExclusive && usbMIDI.getSysExArrayLength() == 6) 
  {
    sysex_controler_connected=true; //we can say for sure a controller is connected
    const byte *data = usbMIDI.getSysExArray();
    int adress = data[1] + 128 * data[2];
    if (adress == 0) { // it is a control command
      control_command(data[3], data[4]);
    } else {
      Serial.print("Received instruction on address:");
      Serial.print(adress);
      int value = data[3] + 128 * data[4];
      Serial.print(" with value:");
      Serial.println(value);
      current_sysex_parameters[adress] = value;
      apply_audio_parameter(adress, value);
    }
  }
  if(type==usbMIDI.Start && rythm_mode)
  {
    rythm_current_step=0;
    midi_clock_current_step=0;
    rythm_tick_function();
    Serial.println("Start received");
    rythm_timer.end();
  }
  if(type==usbMIDI.Stop && rythm_mode)
  {
    rythm_timer.begin(rythm_tick_function, short_timer_period);
  }


  if(type==usbMIDI.Clock && rythm_mode)
  {
    //here we want half of the cycle to be synced with the midi clock, and half with the calculated internal clock, so we can still have shuffle
    //recalculate the BPM
    rythm_bpm=(rythm_bpm*10+(1000*1000*60/last_midi_clock_in)/24)/11.0;
    last_midi_clock_in=0;
    midi_clock_current_step+=1;
    recalculate_timer();   
    //once every two beat, we sync
    if(midi_clock_current_step==24){
      rythm_timer.begin(rythm_tick_function, short_timer_period);
      rythm_tick_function();
      midi_clock_current_step=0;
    }
    //We disable the timer to avoid having it trigger the tick too early when we arrive at the sync beat 
    if(midi_clock_current_step>18){
      rythm_timer.end();
    }
  }
  if(type==usbMIDI.NoteOn)
    {
      int channel = usbMIDI.getChannel();
      int note = usbMIDI.getData1();
      int velocity = usbMIDI.getData2();
      Serial.printf("Note On: ch=%d, note=%d, vel=%d\n", channel, note, velocity);
        //int c = externalUseFixedChannel ? EXTERNAL_CHANNEL : channel;
        note += 1;
        
        // Map the incoming note to a harp voice or chord voice
        //int harp_index = (note - midi_base_note_transposed) % 12; // assuming 12 harp zones
        int harp_index = note % 12; // assuming 12 harp zones
        int oldAttack = harp_attack_velocity;
        if (harp_index >= 0 && harp_index < 12) {
          current_harp_notes[harp_index] = note;
          set_harp_voice_frequency(harp_index, note);
          
          AudioNoInterrupts();
          harp_attack_velocity = velocity;
          envelope_string_vibrato_lfo.noteOn();
          envelope_string_vibrato_dc.noteOn();
          string_enveloppe_filter_array[harp_index]->noteOn();
          string_enveloppe_array[harp_index]->noteOn();
          string_transient_envelope_array[harp_index]->noteOn();
          AudioInterrupts();
          harp_attack_velocity = oldAttack;
        }
    }
    if(type==usbMIDI.NoteOff) 
    {
      int channel = usbMIDI.getChannel();
      int note = usbMIDI.getData1();
      int velocity = usbMIDI.getData2();
      Serial.printf("Note Off: ch=%d, note=%d, vel=%d\n", channel, note, velocity);
        //int c = externalUseFixedChannel ? EXTERNAL_CHANNEL : channel;
        note += 1;
        
        // Map the incoming note to a harp voice or chord voice
        //int harp_index = (note - midi_base_note_transposed) % 12; // assuming 12 harp zones
        int harp_index = note % 12; // assuming 12 harp zones
        
        if (harp_index >= 0 && harp_index < 12) {
          current_harp_notes[harp_index] = note;
          set_harp_voice_frequency(harp_index, note);
          
          AudioNoInterrupts();
          string_enveloppe_array[harp_index]->noteOff();
          string_transient_envelope_array[harp_index]->noteOff();
          string_enveloppe_filter_array[harp_index]->noteOff();
          AudioInterrupts();
        }
    }
    if (type == usbMIDI.ControlChange && usbMIDI.getChannel() == 1)
    {

      int d2 = usbMIDI.getData2();
      int channel = usbMIDI.getChannel();
      chord_attack_velocity = chord_attack_velocity_PedalUsed;
      harp_attack_velocity = harp_attack_velocity_PedalUsed; 
      volumeOffset = d2;
        if (volumeOffset > 127)
        {
          volumeOffset = 127;
        }
        if (volumeOffset < 0)
        {
          volumeOffset = 0;
        }
        
        int clamped = chord_attack_velocity_PedalUsed + floor(volumeOffset/127.0);
        
        float vel_gain = clamped;
        Serial.printf("Velocity Change: ch=%d, raw=%d value=%d gain = %f\n", channel, volumeOffset, clamped, vel_gain);

        AudioNoInterrupts();
        for (int i = 0; i < 12; i++)
        {
          string_waveform_array[i]->amplitude(0.5 + vel_gain*0.25); //too loud
        }

        for (int i = 0; i < 4; i++)
        {
          chord_voice_mixer_array[i]->gain(0, 0.7 + vel_gain*0.7);
          chord_voice_mixer_array[i]->gain(1, 0.7 + vel_gain*0.7);
          chord_voice_mixer_array[i]->gain(2, 0.7 + vel_gain*0.7);
        }
        Serial.printf("Velocity Change: ch=%d, raw=%d act value=%d\n", channel, volumeOffset, chord_attack_velocity);
        AudioInterrupts();
      
      
    }
    if (type == usbMIDI.PitchBend  && (usbMIDI.getChannel() == 1 || usbMIDI.getChannel() == 2))
    {
        // ---------- PITCH BEND ----------
        int d1 = usbMIDI.getData1();
        int d2 = usbMIDI.getData2();
        int channel = usbMIDI.getChannel();
        // 14-bit raw value from LSB/MSB
        int raw14 = (d2 << 7) | d1;        // 0 .. 16383
        int signed14 = raw14 - 8192;      // -8192 .. 8191

        int mapped = 0; // will hold -8191..8191 (or 0..8191 for channel 2)

        if (channel == 2) {
          if (signed14 <= 0) mapped = 0;
          else mapped = constrain(signed14, 0, 8191);
        } 
        else if (channel == 3) {
          // channel 3 = only downward bend: -8191 .. 0
          if (signed14 <= 0) mapped = 0;
          else mapped = constrain(signed14, 0, 8191) * -1;
        } 
        else {
          // fallback: full signed range (if you ever want other channels to do full-range)
          mapped = constrain(signed14, -8191, 8191);
        }

        // Convert mapped (-8191..8191 or 0..8191) into a semitone offset and then a freq multiplier
        float bendSemis = ( (float)mapped / 8192.0f ) * bendRangeSemitones; // e.g. -2..0 or 0..+2
        float bendRatio = powf(2.0f, bendSemis / 12.0f);

        // --- apply to audio oscillators safely ---
        // NOTE: these sizeof expressions work if these arrays are real static arrays visible here.
        // If they are pointers, replace with your constants (e.g. NUM_CHORD_NOTES, NUM_CHORD_OSC).
        const int numChordNotes = sizeof(current_chord_notes) / sizeof(current_chord_notes[0]);
        const int numChordOsc   = sizeof(chord_osc_1_array)    / sizeof(chord_osc_1_array[0]);
        const int applyVoices = min(numChordNotes, numChordOsc);
        Serial.printf("Pitch Bend: ch=%d, raw=%d value=%d\n", channel,raw14, mapped);
        AudioNoInterrupts();
        for (int i = 0; i < applyVoices; i++) {
          // only modify voices that are active / started
          if (i < (int)(sizeof(chord_started_notes) / sizeof(chord_started_notes[0])) && chord_started_notes[i] != 0) {
            // compute the same base frequency your set_chord_voice_frequency uses
            float baseFreq = pow(2.0f, chord_octave_change) * c_frequency / 8.0f
                            * pow(2.0f, (current_chord_notes[i] + transpose_semitones) / 12.0f);

            chord_osc_1_array[i]->frequency(osc_1_freq_multiplier * baseFreq * bendRatio);
            chord_osc_2_array[i]->frequency(osc_2_freq_multiplier * baseFreq * bendRatio);
            chord_osc_3_array[i]->frequency(osc_3_freq_multiplier * baseFreq * bendRatio);
          }
        }
      
        const int numHarpNotes = (int)(sizeof(current_harp_notes) / sizeof(current_harp_notes[0]));
        const int numStringWave = (int)(sizeof(string_waveform_array) / sizeof(string_waveform_array[0]));
        const int applyHarpVoices = min(numHarpNotes, numStringWave);

        const int numHarpStarted = (int)(sizeof(harp_started_notes) / sizeof(harp_started_notes[0]));

        for (int i = 0; i < applyHarpVoices; i++) {
          if (i < numHarpStarted && harp_started_notes[i] != 0) {
            // compute the same base frequencies as set_harp_voice_frequency, then apply bendRatio
            float note_freq = pow(2.0f, harp_octave_change) * c_frequency / 4.0f
                              * pow(2.0f, (current_harp_notes[i] + transpose_semitones) / 12.0f);
            float transient_freq = 64.0f * c_frequency / 4.0f
                                    * pow(2.0f, ((current_harp_notes[i] + transpose_semitones) % 12 + transient_note_level) / 12.0f);

            // apply bend
            string_waveform_array[i]->frequency(note_freq * bendRatio);
            string_transient_waveform_array[i]->frequency(transient_freq * bendRatio);
            // keep filter base freq but keytrack over the bent note freq
            string_filter_array[i]->frequency(string_filter_base_freq + (note_freq * bendRatio) * string_filter_keytrack);
          }
        }
        
        AudioInterrupts();
      
    }
}

//-->>TIMER FUNCTIONS
// function to handle the delayed chord activation
void play_single_note(int i, IntervalTimer *timer) {
  timer->end();
  set_chord_voice_frequency(i, current_applied_chord_notes[i]);
  chord_vibrato_envelope_array1[i]->noteOn();
  chord_vibrato_dc_envelope_array1[i]->noteOn();
  chord_envelope_array1[i]->noteOn();
  chord_envelope_filter_array1[i]->noteOn();
  if(chord_started_notes[i]!=0){
    if (isLooperRecording)
    {
      midiEvent m;
      m.timeStamp = curLooperTick;
      m.status = MIDI_OFF;
      m.data1 = chord_started_notes[i];
      m.data2 = chord_release_velocity;      
      addMidiLooperEvent(m, CHORD_CHANNEL);
    }
    usbMIDI.sendNoteOff(chord_started_notes[i],chord_release_velocity,CHORD_CHANNEL,chord_port);
    chord_started_notes[i]=0;}
    if (isLooperRecording)
    {
      midiEvent m;
      m.timeStamp = curLooperTick;
      m.status = MIDI_ON;
      m.data1 = midi_base_note_transposed+ current_applied_chord_notes[i];
      m.data2 = chord_attack_velocity+volumeOffset;      
      addMidiLooperEvent(m, CHORD_CHANNEL);
    }
  usbMIDI.sendNoteOn(midi_base_note_transposed+ current_applied_chord_notes[i],chord_attack_velocity+volumeOffset,CHORD_CHANNEL,chord_port);
  chord_started_notes[i]=midi_base_note_transposed+ current_applied_chord_notes[i];
}

void play_single_note_orig(int i, IntervalTimer *timer) {
  timer->end();
  set_chord_voice_frequency(i, current_applied_chord_notes[i]);
  chord_vibrato_envelope_array[i]->noteOn();
  chord_vibrato_dc_envelope_array[i]->noteOn();
  chord_envelope_array[i]->noteOn();
  chord_envelope_filter_array[i]->noteOn();
  if(chord_started_notes[i]!=0){
    if (isLooperRecording)
    {
      midiEvent m;
      m.timeStamp = curLooperTick;
      m.status = MIDI_OFF;
      m.data1 = chord_started_notes[i];
      m.data2 = chord_release_velocity;      
      addMidiLooperEvent(m, CHORD_CHANNEL);
    }
    usbMIDI.sendNoteOff(chord_started_notes[i],chord_release_velocity,CHORD_CHANNEL,chord_port);
    chord_started_notes[i]=0;}
    if (isLooperRecording)
    {
      midiEvent m;
      m.timeStamp = curLooperTick;
      m.status = MIDI_ON;
      m.data1 = midi_base_note_transposed+ current_applied_chord_notes[i];
      m.data2 = chord_attack_velocity+volumeOffset;      
      addMidiLooperEvent(m, CHORD_CHANNEL);
    }
  usbMIDI.sendNoteOn(midi_base_note_transposed+ current_applied_chord_notes[i],chord_attack_velocity+volumeOffset,CHORD_CHANNEL,chord_port);
  chord_started_notes[i]=midi_base_note_transposed+ current_applied_chord_notes[i];
}

void play_note_selected_duration(int i,int current_note){
  chord_vibrato_envelope_array1[i]->noteOn();
  chord_vibrato_dc_envelope_array1[i]->noteOn();
  chord_envelope_array1[i]->noteOn();
  chord_envelope_filter_array1[i]->noteOn();
  note_off_timing[i]=0;
  if(chord_started_notes[i]!=0){
    if (isLooperRecording)
    {
      midiEvent m;
      m.timeStamp = curLooperTick;
      m.status = MIDI_OFF;
      m.data1 = chord_started_notes[i];
      m.data2 = chord_release_velocity;      
      addMidiLooperEvent(m, CHORD_CHANNEL);
    }
    usbMIDI.sendNoteOff(chord_started_notes[i],chord_release_velocity,CHORD_CHANNEL,chord_port);
    chord_started_notes[i]=0;}
    if (isLooperRecording)
    {
      midiEvent m;
      m.timeStamp = curLooperTick;
      m.status = MIDI_ON;
      m.data1 = midi_base_note_transposed+current_note;
      m.data2 = chord_attack_velocity+volumeOffset;      
      addMidiLooperEvent(m, CHORD_CHANNEL);
    }
  usbMIDI.sendNoteOn(midi_base_note_transposed+current_note,chord_attack_velocity+volumeOffset,CHORD_CHANNEL,chord_port);
  chord_started_notes[i]=midi_base_note_transposed+current_note;
}

void play_note_selected_duration_orig(int i,int current_note){
  chord_vibrato_envelope_array[i]->noteOn();
  chord_vibrato_dc_envelope_array[i]->noteOn();
  chord_envelope_array[i]->noteOn();
  chord_envelope_filter_array[i]->noteOn();
  note_off_timing[i]=0;
  if(chord_started_notes[i]!=0){
    if (isLooperRecording)
    {
      midiEvent m;
      m.timeStamp = curLooperTick;
      m.status = MIDI_OFF;
      m.data1 = chord_started_notes[i];
      m.data2 = chord_release_velocity;      
      addMidiLooperEvent(m, CHORD_CHANNEL);
    }
    usbMIDI.sendNoteOff(chord_started_notes[i],chord_release_velocity,CHORD_CHANNEL,chord_port);
    chord_started_notes[i]=0;}
    if (isLooperRecording)
    {
      midiEvent m;
      m.timeStamp = curLooperTick;
      m.status = MIDI_ON;
      m.data1 = midi_base_note_transposed+current_note;
      m.data2 = chord_attack_velocity+volumeOffset;      
      addMidiLooperEvent(m, CHORD_CHANNEL);
    }
  usbMIDI.sendNoteOn(midi_base_note_transposed+current_note,chord_attack_velocity+volumeOffset,CHORD_CHANNEL,chord_port);
  chord_started_notes[i]=midi_base_note_transposed+current_note;
}

void turn_off_led(IntervalTimer *timer) {
  timer->end();
  analogWrite(RYTHM_LED_PIN, 0);
}

//-->>AUDIO HELPER FUNCTIONS
// calculationg the ws array
void calculate_ws_array() {
  for (int i = 0; i < 257; i++) {
    float current_x = (i / 256.0 - 0.5) * 2.0 * PI;
    wave_shape[i] = sin(current_x);
    for (int j = 0; j < ws_sin_param; j++) {
      wave_shape[i] = sin(wave_shape[i] * PI);
    }
  }
}
// setting the pad_frequency

// setting the pad_frequency
void set_chord_voice_frequency(uint8_t i, uint16_t current_note) {
  float note_freq = pow(2,chord_octave_change)*c_frequency/8 * pow(2, (current_note+transpose_semitones) / 12.0); //down one octave to let more possibilities with the shuffling array
  if(glide_length>0){
        //ok so first we need to set the "middle note". Keep in mind that the signal will be +/-1 and will go +/- 1 octave
    //let's do a trick to select a middle note: get the level (relative to the C) and the note and do a modulo 
    int note_level=12*chord_octave_change-3*12+current_note+transpose_semitones;
    int base_octave =chord_octave_change-2+(chord_shuffling_array[chord_shuffling_selection][i])/12;
    int middle_note=base_octave*12+transpose_semitones; 
    int note_delta=note_level-middle_note;
    float middle_freq=c_frequency*pow(2,middle_note/12.0);

    AudioNoInterrupts();
    chords_vibrato_lfo1.frequency(chord_vibrato_base_freq + chord_vibrato_keytrack * current_chord_notes[0]);
    chords_tremolo_lfo1.frequency(chord_tremolo_base_freq + chord_tremolo_keytrack * current_chord_notes[0]);
    // hord_vibrato_lfo_array[i]->frequency(chord_vibrato_base_freq);
    // chord_tremolo_lfo_array[i]->frequency(chord_tremolo_base_freq);
    chord_voice_filter_array1[i]->frequency(note_freq * chord_filter_keytrack + chord_filter_base_freq);
    chord_osc_1_array1[i]->frequency(osc_1_freq_multiplier * middle_freq);
    chord_osc_2_array1[i]->frequency(osc_2_freq_multiplier * middle_freq);
    chord_osc_3_array1[i]->frequency(osc_3_freq_multiplier * middle_freq);
    chord_freq_dc_array1[i]->amplitude(note_delta/24.0,glide_length);
    // chord_voice_filter_array[i]->frequency(1*freq);
    AudioInterrupts();
  }else{
    float note_freq = pow(2,chord_octave_change)*c_frequency/8 * pow(2, (current_note+transpose_semitones) / 12.0); //down one octave to let more possibilities with the shuffling array
    AudioNoInterrupts();
    chords_vibrato_lfo1.frequency(chord_vibrato_base_freq + chord_vibrato_keytrack * current_chord_notes[0]);
    chords_tremolo_lfo1.frequency(chord_tremolo_base_freq + chord_tremolo_keytrack * current_chord_notes[0]);
    // hord_vibrato_lfo_array[i]->frequency(chord_vibrato_base_freq);
    // chord_tremolo_lfo_array[i]->frequency(chord_tremolo_base_freq);
    chord_voice_filter_array1[i]->frequency(note_freq * chord_filter_keytrack + chord_filter_base_freq);
    chord_osc_1_array1[i]->frequency(osc_1_freq_multiplier * note_freq);
    chord_osc_2_array1[i]->frequency(osc_2_freq_multiplier * note_freq);
    chord_osc_3_array1[i]->frequency(osc_3_freq_multiplier * note_freq);
    chord_freq_dc_array1[i]->amplitude(0,0);
    // chord_voice_filter_array[i]->frequency(1*freq);
    AudioInterrupts();
  }

  if(chord_started_notes[i]!=0 && chord_started_notes[i]!=midi_base_note_transposed+current_note){
    //we need to change the note without triggering the change, ie a pitch bend
    if (isLooperRecording)
    {
      midiEvent m;
      m.timeStamp = curLooperTick;
      m.status = MIDI_OFF;
      m.data1 = chord_started_notes[i];
      m.data2 = chord_release_velocity;      
      addMidiLooperEvent(m, CHORD_CHANNEL);
    }
    usbMIDI.sendNoteOff(chord_started_notes[i],chord_release_velocity,CHORD_CHANNEL,chord_port);
    chord_started_notes[i]=0;
    if (isLooperRecording)
    {
      midiEvent m;
      m.timeStamp = curLooperTick;
      m.status = MIDI_OFF;
      m.data1 = midi_base_note_transposed+current_note;
      m.data2 = chord_attack_velocity+volumeOffset;      
      addMidiLooperEvent(m, CHORD_CHANNEL);
    }
    usbMIDI.sendNoteOn(midi_base_note_transposed+current_note,chord_attack_velocity+volumeOffset,CHORD_CHANNEL,chord_port);
    chord_started_notes[i]=midi_base_note_transposed+ current_note;
  }
}

void set_chord_voice_frequency_orig(uint8_t i, uint16_t current_note) {
  float note_freq = pow(2,chord_octave_change)*c_frequency/8 * pow(2, (current_note+transpose_semitones) / 12.0); //down one octave to let more possibilities with the shuffling array
  if(glide_length>0){
        //ok so first we need to set the "middle note". Keep in mind that the signal will be +/-1 and will go +/- 1 octave
    //let's do a trick to select a middle note: get the level (relative to the C) and the note and do a modulo 
    int note_level=12*chord_octave_change-3*12+current_note+transpose_semitones;
    int base_octave =chord_octave_change-2+(chord_shuffling_array[chord_shuffling_selection][i])/12;
    int middle_note=base_octave*12+transpose_semitones; 
    int note_delta=note_level-middle_note;
    float middle_freq=c_frequency*pow(2,middle_note/12.0);

    AudioNoInterrupts();
    chords_vibrato_lfo.frequency(chord_vibrato_base_freq + chord_vibrato_keytrack * current_chord_notes[0]);
    chords_tremolo_lfo.frequency(chord_tremolo_base_freq + chord_tremolo_keytrack * current_chord_notes[0]);
    // hord_vibrato_lfo_array[i]->frequency(chord_vibrato_base_freq);
    // chord_tremolo_lfo_array[i]->frequency(chord_tremolo_base_freq);
    chord_voice_filter_array[i]->frequency(note_freq * chord_filter_keytrack + chord_filter_base_freq);
    chord_osc_1_array[i]->frequency(osc_1_freq_multiplier * middle_freq);
    chord_osc_2_array[i]->frequency(osc_2_freq_multiplier * middle_freq);
    chord_osc_3_array[i]->frequency(osc_3_freq_multiplier * middle_freq);
    chord_freq_dc_array[i]->amplitude(note_delta/24.0,glide_length);
    // chord_voice_filter_array[i]->frequency(1*freq);
    AudioInterrupts();
  }else{
    float note_freq = pow(2,chord_octave_change)*c_frequency/8 * pow(2, (current_note+transpose_semitones) / 12.0); //down one octave to let more possibilities with the shuffling array
    AudioNoInterrupts();
    chords_vibrato_lfo.frequency(chord_vibrato_base_freq + chord_vibrato_keytrack * current_chord_notes[0]);
    chords_tremolo_lfo.frequency(chord_tremolo_base_freq + chord_tremolo_keytrack * current_chord_notes[0]);
    // hord_vibrato_lfo_array[i]->frequency(chord_vibrato_base_freq);
    // chord_tremolo_lfo_array[i]->frequency(chord_tremolo_base_freq);
    chord_voice_filter_array[i]->frequency(note_freq * chord_filter_keytrack + chord_filter_base_freq);
    chord_osc_1_array[i]->frequency(osc_1_freq_multiplier * note_freq);
    chord_osc_2_array[i]->frequency(osc_2_freq_multiplier * note_freq);
    chord_osc_3_array[i]->frequency(osc_3_freq_multiplier * note_freq);
    chord_freq_dc_array[i]->amplitude(0,0);
    // chord_voice_filter_array[i]->frequency(1*freq);
    AudioInterrupts();
  }

  if(chord_started_notes[i]!=0 && chord_started_notes[i]!=midi_base_note_transposed+current_note){
    //we need to change the note without triggering the change, ie a pitch bend
    if (isLooperRecording)
    {
      midiEvent m;
      m.timeStamp = curLooperTick;
      m.status = MIDI_OFF;
      m.data1 = chord_started_notes[i];
      m.data2 = chord_release_velocity;      
      addMidiLooperEvent(m, CHORD_CHANNEL);
    }
    usbMIDI.sendNoteOff(chord_started_notes[i],chord_release_velocity,CHORD_CHANNEL,chord_port);
    chord_started_notes[i]=0;
    if (isLooperRecording)
    {
      midiEvent m;
      m.timeStamp = curLooperTick;
      m.status = MIDI_OFF;
      m.data1 = midi_base_note_transposed+current_note;
      m.data2 = chord_attack_velocity+volumeOffset;      
      addMidiLooperEvent(m, CHORD_CHANNEL);
    }
    usbMIDI.sendNoteOn(midi_base_note_transposed+current_note,chord_attack_velocity+volumeOffset,CHORD_CHANNEL,chord_port);
    chord_started_notes[i]=midi_base_note_transposed+ current_note;
  }
}

void set_harp_voice_frequency(uint8_t i, uint16_t current_note) {
  float note_freq =  pow(2,harp_octave_change)*c_frequency/4 * pow(2, (current_note+transpose_semitones) / 12.0);
  float transient_freq =  64.0*c_frequency/4 *pow(2, ((current_note+transpose_semitones)%12+transient_note_level) / 12.0);
  AudioNoInterrupts();
  string_waveform_array1[i]->frequency(note_freq);
  string_transient_waveform_array1[i]->frequency(transient_freq);
  string_filter_array1[i]->frequency(string_filter_base_freq + note_freq * string_filter_keytrack);
  // string_vibrato_1.offset(0);
  AudioInterrupts();
}

// setting the harp
void set_harp_voice_frequency_orig(uint8_t i, uint16_t current_note) {
  float note_freq =  pow(2,harp_octave_change)*c_frequency/4 * pow(2, (current_note+transpose_semitones) / 12.0);
  float transient_freq =  64.0*c_frequency/4 *pow(2, ((current_note+transpose_semitones)%12+transient_note_level) / 12.0);
  AudioNoInterrupts();
  string_waveform_array[i]->frequency(note_freq);
  string_transient_waveform_array[i]->frequency(transient_freq);
  string_filter_array[i]->frequency(string_filter_base_freq + note_freq * string_filter_keytrack);
  // string_vibrato_1.offset(0);
  AudioInterrupts();
}
// Function to compute MIDI note offset dynamically with circular frame shift
int8_t get_root_button(uint8_t key, uint8_t shift, uint8_t button) { 
  int8_t note = base_notes[button]; // Start with base note in C (e.g., B = 11, E = 4, ..., F = 5)
  // Apply circular frame shift: move notes C, D, E, F, G, A, B up an octave based on shift
  // Map button to musical note index (C=0, D=1, E=2, F=3, G=4, A=5, B=6)
  int8_t musical_index;
  switch (button) {
    case BTN_B: musical_index = 6; break; // B
    case BTN_E: musical_index = 2; break; // E
    case BTN_A: musical_index = 5; break; // A
    case BTN_D: musical_index = 1; break; // D
    case BTN_G: musical_index = 4; break; // G
    case BTN_C: musical_index = 0; break; // C
    case BTN_F: musical_index = 3; break; // F
    default: musical_index = 0; // Should not happen
  }
  if (musical_index < shift) {
    note += 12; // Move up one octave if the note is shifted "on top"
  }
  int8_t num_accidentals = key_signatures[key];   // Apply key signature (sharps or flats)
  if (key <= KEY_SIG_B) { // Sharp keys (C, G, D, A, E, B)
    for (int i = 0; i < num_accidentals; i++) {
      if (button == sharp_notes[num_accidentals - 1][i]) {
        note += 1; // Add sharp
      }
    }
  } else { // Flat keys (F, Bb, Eb, Ab, Db, Gb)
    for (int i = 0; i < num_accidentals; i++) {
      if (button == flat_notes[num_accidentals - 1][i]) {
        note -= 1; // Add flat
      }
    }
  }

  return note; //No need to constrain here
}
// function to calculate the frequency of individual chord notes
uint8_t calculate_note_chord(uint8_t voice, bool slashed, bool sharp) {
  uint8_t note = 0;
  uint8_t level = chord_shuffling_array[chord_shuffling_selection][voice];
  if (slashed && level % 10 == note_slash_level) {
    if (!flat_button_modifier) {
      note = (12 * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, slash_value) + sharp * 1.0);
    } else {
      note = (12 * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, slash_value) - sharp * 1.0);
    }
  } else {
    if (!flat_button_modifier) {
      note = (12 * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, fundamental) + sharp * 1.0 + (*current_chord)[level % 10]);
    } else {
      note = (12 * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, fundamental) - sharp * 1.0 + (*current_chord)[level % 10]);
    }
  }
  return note;
}
// function to calculate the level of individual harp touch
uint8_t calculate_note_harp(uint8_t string, bool slashed, bool sharp) {
  if (!chromatic_harp_mode) {
    uint8_t note = 0;
    uint8_t level = harp_shuffling_array[harp_shuffling_selection][string];
    if (slashed && level % 10 == note_slash_level) {
      if (!flat_button_modifier) {
        note = (12 * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, slash_value) + sharp * 1.0);
      } else {
        note = (12 * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, slash_value) - sharp * 1.0);
      }
    } else {
      if (!flat_button_modifier) {
        note = (12 * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, fundamental) + sharp * 1.0 + (*current_chord)[level % 10]);
      } else {
        note = (12 * int(level / 10) + get_root_button(key_signature_selection, chord_frame_shift, fundamental) - sharp * 1.0 + (*current_chord)[level % 10]);
      }
    }
    return note;
  } else {
    return string + 24; // Chromatic mode
  }
}
//-->>RYTHM MODE UTILITIES
void rythm_tick_function() {
  //this function seems a bit long for a timed one. Maybe try to offload some logic somewhere else? 
  if (rythm_current_step % rythm_limit_change_to_every == 0) {
    for (int i = 0; i < 7; i++) {
      rythm_freeze_current_chord_notes[i] = current_applied_chord_notes[i];
    }
  }
  // handling the led pattern
  uint8_t active_modulus = 1;
  uint8_t possible_pattern[4] = {3, 2};
  for (uint8_t i = 0; i < sizeof(possible_pattern) / sizeof(uint8_t); i++) {
    if (rythm_loop_length % possible_pattern[i] == 0) {
      active_modulus = possible_pattern[i];
      break;
    }
  }
  analogWrite(RYTHM_LED_PIN, (220 * (rythm_current_step % rythm_limit_change_to_every == 0) + 15) * (rythm_current_step % active_modulus == 0));
  led_timer.priority(255);
  led_timer.begin([] { turn_off_led(&led_timer); }, 200000); 
  if (current_long_period) {
    rythm_timer.update(short_timer_period);
    current_long_period = false;
  } else {
    rythm_timer.update(long_timer_period);
    current_long_period = true;
  }
  u_int8_t result;
  result = rythm_pattern[rythm_current_step];
  for (int i = 6; i >= 0; i--) {
    if (result & (1 << i)) {
      int current_voice=0;
      if(i<4){
        current_voice=i;
      }else{
        current_voice=i-3;
      }
      set_chord_voice_frequency(current_voice, rythm_freeze_current_chord_notes[i]);
      play_note_selected_duration(current_voice, rythm_freeze_current_chord_notes[i]);
    }
  }
  rythm_current_step = (rythm_current_step + 1) % rythm_loop_length;
}

void recalculate_timer() {
  long_timer_period = shuffle * (60 * 1000 * 1000) / (2 * rythm_bpm);
  short_timer_period = 2 * (60 * 1000 * 1000) / (2 * rythm_bpm) - long_timer_period;
}

//--->>FILE HANDLING UTILITIES
String serialize(int16_t data_array[], u_int16_t array_size) {
  String dataString = "0,";
  dataString += String(current_bank_number); // to save the number of the bank for the online display
  dataString += ",";
  for (u_int16_t i = 2; i < array_size; i++) {
    dataString += String(data_array[i]);
    dataString += ",";
  }
  return dataString;
}

void deserialize(String input, int16_t data_array[]) {
  int len = input.length() + 1;
  char string[len];
  char *p;
  input.toCharArray(string, len);
  p = strtok(string, ",");
  int i = 0;
  while (p && i < parameter_size) {
    data_array[i] = atoi(p);
    p = strtok(NULL, ",");
    i++;
  }
}

void save_config(int bank_number, bool default_save) {
  if (bank_number < 0 || bank_number >= preset_number) {
    Serial.printf("Error: Invalid bank_number %d in save_config\n", bank_number);
    return;
  }
  digitalWrite(_MUTE_PIN, LOW); // muting the DAC
  current_bank_number=bank_number; //save to correctly write in the memory 
  AudioNoInterrupts();
  // myfs.quickFormat();  // performs a quick format of the created di
  myfs.remove(bank_name[bank_number]);
  File dataFile = myfs.open(bank_name[bank_number], FILE_WRITE);

  if (default_save) {
    // if we need to put the default in memory
    Serial.println("Writing the default file");
    Serial.println(bank_name[bank_number]);
    String return_data = serialize(default_bank_sysex_parameters[bank_number], parameter_size);
    dataFile.println(return_data);
  } else {
    Serial.println("Saving current settings");
    for (u_int16_t i = 0; i < parameter_size; i++) {
          Serial.println(current_sysex_parameters[i]);
    }
    dataFile.println(serialize(current_sysex_parameters, parameter_size));
  }
  Serial.print("Saved preset: ");
  Serial.println(dataFile.name());
  dataFile.close();

  load_config(current_bank_number); //we do a full reload to initialise values
  
  // add something to set config_bit in the parameters to zero
  AudioInterrupts();
  digitalWrite(_MUTE_PIN, HIGH); // unmuting the DAC
}

void load_config(int bank_number) {
  if (bank_number < 0 || bank_number >= preset_number) {
    Serial.printf("Error: Invalid bank_number %d in save_config\n", bank_number);
    return;
  }
  //digitalWrite(_MUTE_PIN, LOW); // muting the DAC
  //Turn off chords notes
  for (int i = 0; i < 4; i++) {
    chord_vibrato_envelope_array[i]->noteOff();
    chord_vibrato_dc_envelope_array[i]->noteOff();
    chord_envelope_array[i]->noteOff();
    chord_envelope_filter_array[i]->noteOff();
  }
  trigger_chord = true; //to be ready to retrigger if needed

  File entry = myfs.open(bank_name[bank_number]);
  if (entry) {
    String data_string = "";
    while (entry.available()) {
      data_string += char(entry.read());
    }
    deserialize(data_string, current_sysex_parameters);
    Serial.print("Loaded preset: ");
    Serial.println(entry.name());
    entry.close();
  } else {
    entry.close();
    Serial.print("No preset, writing factory default");
    save_config(bank_number, true); // reboot with default value
  }
  // Loading the potentiometer
  chord_pot.setup(chord_volume_sysex, 100, current_sysex_parameters[chord_pot_alternate_control], current_sysex_parameters[chord_pot_alternate_range], current_sysex_parameters,current_sysex_parameters[chord_pot_alternate_storage],apply_audio_parameter,chord_pot_alternate_storage);
  harp_pot.setup(harp_volume_sysex, 100, current_sysex_parameters[harp_pot_alternate_control], current_sysex_parameters[harp_pot_alternate_range], current_sysex_parameters,current_sysex_parameters[harp_pot_alternate_storage],apply_audio_parameter,harp_pot_alternate_storage);
  mod_pot.setup(current_sysex_parameters[mod_pot_main_control], current_sysex_parameters[mod_pot_main_range], current_sysex_parameters[mod_pot_alternate_control], current_sysex_parameters[mod_pot_alternate_range], current_sysex_parameters,current_sysex_parameters[mod_pot_alternate_storage],apply_audio_parameter,mod_pot_alternate_storage);
  Serial.println("pot setup done");
  for (int i = 1; i < parameter_size; i++) {
    apply_audio_parameter(i, current_sysex_parameters[i]);
  }
  control_command(0, 0); // tell itself to update the remote controller if present
  chord_pot.force_update();
  harp_pot.force_update();
  mod_pot.force_update();
  flag_save_needed=false;
  //digitalWrite(_MUTE_PIN, HIGH); // unmuting the DAC
}
unsigned int computeTickInterval(int bpm) {
  // 60,000,000 microseconds per minute / (BPM * 192 ticks per quarter note)
  return ((60000 / bpm) / (QUARTERNOTETICKS * 1.0)) * 1000;
}
bool sendClockTick = false;
void clockISR() {
  sendClockTick = true;
}

void tickISR() {
  sendClockTick = true;
}
IntervalTimer tickTimer;
void setup() {
  Serial.begin(9600);
  Serial.println("Initialising audio parameters");
  AudioMemory(1200);
  //>>STATIC AUDIO PARAMETERS
  // the waveshaper
  calculate_ws_array();
  chord_waveshape.shape(wave_shape, 257);
  string_waveshape.shape(wave_shape, 257);
  //clones
  chord_waveshape1.shape(wave_shape, 257);
  string_waveshape1.shape(wave_shape, 257);
  //the base DC value for strings
  filter_dc.amplitude(1);
  filter_dc1.amplitude(1);
  // the delay passthrough
  string_delay_mix.gain(0, 1);
  chord_delay_mix.gain(0, 1);

  string_delay_mix1.gain(0, 1);
  chord_delay_mix1.gain(0, 1);
  // simple mixers
  string_vibrato_mixer.gain(0,0.5);
  string_vibrato_mixer.gain(1,0.5);
  string_vibrato_mixer1.gain(0,0.5);
  string_vibrato_mixer1.gain(1,0.5);
  envelope_string_vibrato_dc.sustain(0);
  for (int i = 0; i < 3; i++) {
    string_mixer_array[i]->gain(0, 1);
    string_mixer_array[i]->gain(1, 1);
    string_mixer_array[i]->gain(2, 1);
    string_mixer_array[i]->gain(3, 1);
    transient_mixer_array[i]->gain(0, 1);
    transient_mixer_array[i]->gain(1, 1);
    transient_mixer_array[i]->gain(2, 1);
    transient_mixer_array[i]->gain(3, 1);
    //clones
    string_mixer_array1[i]->gain(0, 1);
    string_mixer_array1[i]->gain(1, 1);
    string_mixer_array1[i]->gain(2, 1);
    string_mixer_array1[i]->gain(3, 1);
    transient_mixer_array1[i]->gain(0, 1);
    transient_mixer_array1[i]->gain(1, 1);
    transient_mixer_array1[i]->gain(2, 1);
    transient_mixer_array1[i]->gain(3, 1);
  }
  for (int i = 0; i < 4; i++) {
    chord_voice_mixer_array[i]->gain(0, 1);
    chord_voice_mixer_array[i]->gain(1, 1);
    chord_voice_mixer_array[i]->gain(2, 1);
    chord_noise_array[i]->amplitude(0.5);
    //we hardcode the frequency modulation. Now intensity of the effect will be depending on the mixer gain 
    chord_osc_1_array[i]->frequencyModulation(2);
    chord_osc_2_array[i]->frequencyModulation(2);
    chord_osc_3_array[i]->frequencyModulation(2);
    //Now the max value of the vibrato is 0.25 for each component and we add 0.5 for pitch selection. With the multiplication by freqmodulation of 2, we maintain the rate we had before. 
    chord_vibrato_mixer_array[i]->gain(1,0.5); 

    chord_vibrato_dc_envelope_array[i]->sustain(0); //for the pitch bend no need for sustain
    transient_full_mix.gain(i, 1);
    all_string_mix.gain(i, 1);
//clones
    chord_voice_mixer_array1[i]->gain(0, 1);
    chord_voice_mixer_array1[i]->gain(1, 1);
    chord_voice_mixer_array1[i]->gain(2, 1);
    chord_noise_array1[i]->amplitude(0.5);
    //we hardcode the frequency modulation. Now intensity of the effect will be depending on the mixer gain 
    chord_osc_1_array1[i]->frequencyModulation(2);
    chord_osc_2_array1[i]->frequencyModulation(2);
    chord_osc_3_array1[i]->frequencyModulation(2);
    //Now the max value of the vibrato is 0.25 for each component and we add 0.5 for pitch selection. With the multiplication by freqmodulation of 2, we maintain the rate we had before. 
    chord_vibrato_mixer_array1[i]->gain(1,0.5); 
    chord_vibrato_dc_envelope_array1[i]->sustain(0); //for the pitch bend no need for sustain
    transient_full_mix1.gain(i, 1);
    all_string_mix1.gain(i, 1);
  }
  for(int i=0;i<12;i++){
    string_transient_envelope_array[i]->sustain(0);//don't need sustain for the transient
    string_transient_envelope_array1[i]->sustain(0);//don't need sustain for the transient
  }
  all_string_mix.gain(3,0.02); //for the transient
  all_string_mix1.gain(3,0.02); //for the transient

  // initialising the rest of the hardware
  chord_matrix.setup();
  harp_sensor.setup();
  harp_sensor.recalibrate();
  pinMode(BATT_LBO_PIN, INPUT);
  pinMode(DOWN_PGM_PIN, INPUT);
  pinMode(UP_PGM_PIN, INPUT);
  pinMode(HOLD_BUTTON_PIN, INPUT);
  if (continuous_chord) {
    analogWrite(RYTHM_LED_PIN, 255);
  }
  // loading the preset
  Serial.println("Initialising filesystem");
  if (!myfs.begin(1024 * 1024)) { // Need to check that size
    Serial.printf("Error starting %s\n", "Program flash DISK");
    while (1) {
      set_led_color(0, 1.0, 1.0); // turn red light
    }
  }
  Serial.println("Loading the preset");
  load_config(current_bank_number);
  // initializing the strings
  for (int i = 0; i < 12; i++) {
    current_harp_notes[i] = calculate_note_harp(i, slash_chord, sharp_active);
  }
  //Checking the battery 
  LBO_flag.set(digitalRead(BATT_LBO_PIN));
  uint8_t LBO_value = LBO_flag.read_value();
  if (LBO_value == 0) {
    led_blinking_flag=true;
  }
  myusb.begin();
  midi1.setHandleNoteOn(handleExtUSBNoteOn);
  midi1.setHandleNoteOff(handleExtUSBNoteOff);
  tickTimer.begin(clockISR, computeTickInterval(127));  // in microseconds
  Serial.println("Initialisation complete");
  digitalWrite(_MUTE_PIN, HIGH);
  
}
AudioSynthWaveform testTone;
AudioConnection* testPatch = nullptr;

void handle_chords_button() {
  int sharp_transition = chord_matrix_array[0].read_transition();
  if (sharp_transition > 1 && current_line != -1) {
    button_pushed = true;
  }
  bool oldSharp_active = sharp_active;
  sharp_active = chord_matrix_array[0].read_value();
  
  if (sharp_active && !oldSharp_active)
  {
    Serial.printf("Sharp pressed\n");
    listenShift = true;
    if (!testPatch) {
      testPatch = new AudioConnection(testTone, 0, string_filter_mixer, 0);
    }
    testTone.begin(WAVEFORM_SINE);
    testTone.frequency(440);
    testTone.amplitude(0.5);

    // Temporarily wire into mixer input to test:

    
  }
  else if (!sharp_active && listenShift)
  {
    Serial.printf("Sharp unpressed\n");
    listenShift = false;
  }
  for (int i = 1; i < 22; i++) {
    int value = chord_matrix_array[i].read_transition();
    if (value > 1 && !inhibit_button) {
      button_pushed = true;
      Serial.print("Button pushed: ");
      Serial.println(i);
      if (current_line == -1) {
        current_line = (i - 1) / 3;
        if (!continuous_chord) {
          trigger_chord = true;
        }
      }
    }
  }
}

void handle_harp() {
  harp_sensor.update(harp_array);
  for (int i = 0; i < 12; i++) {
    int value = harp_array[i].read_transition();
    if (value == 2) {
      set_harp_voice_frequency(i, current_harp_notes[i]);
      AudioNoInterrupts();
      envelope_string_vibrato_lfo1.noteOn();
      envelope_string_vibrato_dc1.noteOn();
      string_enveloppe_filter_array1[i]->noteOn();
      string_enveloppe_array1[i]->noteOn();
      string_transient_envelope_array1[i]->noteOn();
      AudioInterrupts();
      if (harp_started_notes[i] != 0) {
        usbMIDI.sendNoteOff(harp_started_notes[i], harp_release_velocity, HARP_CHANNEL, harp_port);
        if (isLooperRecording)
        {
          midiEvent m;
          m.timeStamp = curLooperTick;
          m.status = MIDI_OFF;
          m.data1 = harp_started_notes[i];
          m.data2 = harp_release_velocity;      
          addMidiLooperEvent(m, HARP_CHANNEL);
        }
      }
      if (isLooperRecording)
      {
        midiEvent m;
        m.timeStamp = curLooperTick;
        m.status = MIDI_ON;
        m.data1 = midi_base_note_transposed + current_harp_notes[i];
        m.data2 = harp_attack_velocity+volumeOffset;      
        addMidiLooperEvent(m, HARP_CHANNEL);
      }
      usbMIDI.sendNoteOn(midi_base_note_transposed + current_harp_notes[i], harp_attack_velocity+volumeOffset, HARP_CHANNEL, harp_port);
      harp_started_notes[i] = midi_base_note_transposed + current_harp_notes[i];
    } else if (value == 1) {
      AudioNoInterrupts();
      string_enveloppe_array1[i]->noteOff();
      string_transient_envelope_array1[i]->noteOff();
      string_enveloppe_filter_array1[i]->noteOff();
      AudioInterrupts();
      if (harp_started_notes[i] != 0) {
        if (isLooperRecording)
          {
            midiEvent m;
            m.timeStamp = curLooperTick;
            m.status = MIDI_OFF;
            m.data1 = harp_started_notes[i];
            m.data2 = harp_release_velocity;      
            addMidiLooperEvent(m, HARP_CHANNEL);
          }
        usbMIDI.sendNoteOff(harp_started_notes[i], harp_release_velocity, HARP_CHANNEL, harp_port);
        harp_started_notes[i] = 0;
      }
    }
  }
}

void handle_harp_orig() {
  harp_sensor.update(harp_array);
  for (int i = 0; i < 12; i++) {
    int value = harp_array[i].read_transition();
    if (value == 2) {
      set_harp_voice_frequency(i, current_harp_notes[i]);
      AudioNoInterrupts();
      envelope_string_vibrato_lfo.noteOn();
      envelope_string_vibrato_dc.noteOn();
      string_enveloppe_filter_array[i]->noteOn();
      string_enveloppe_array[i]->noteOn();
      string_transient_envelope_array[i]->noteOn();
      AudioInterrupts();
      if (harp_started_notes[i] != 0) {
        usbMIDI.sendNoteOff(harp_started_notes[i], harp_release_velocity, HARP_CHANNEL, harp_port);
        if (isLooperRecording)
        {
          midiEvent m;
          m.timeStamp = curLooperTick;
          m.status = MIDI_OFF;
          m.data1 = harp_started_notes[i];
          m.data2 = harp_release_velocity;      
          addMidiLooperEvent(m, HARP_CHANNEL);
        }
      }
      if (isLooperRecording)
      {
        midiEvent m;
        m.timeStamp = curLooperTick;
        m.status = MIDI_ON;
        m.data1 = midi_base_note_transposed + current_harp_notes[i];
        m.data2 = harp_attack_velocity+volumeOffset;      
        addMidiLooperEvent(m, HARP_CHANNEL);
      }
      usbMIDI.sendNoteOn(midi_base_note_transposed + current_harp_notes[i], harp_attack_velocity+volumeOffset, HARP_CHANNEL, harp_port);
      harp_started_notes[i] = midi_base_note_transposed + current_harp_notes[i];
    } else if (value == 1) {
      AudioNoInterrupts();
      string_enveloppe_array[i]->noteOff();
      string_transient_envelope_array[i]->noteOff();
      string_enveloppe_filter_array[i]->noteOff();
      AudioInterrupts();
      if (harp_started_notes[i] != 0) {
        if (isLooperRecording)
          {
            midiEvent m;
            m.timeStamp = curLooperTick;
            m.status = MIDI_OFF;
            m.data1 = harp_started_notes[i];
            m.data2 = harp_release_velocity;      
            addMidiLooperEvent(m, HARP_CHANNEL);
          }
        usbMIDI.sendNoteOff(harp_started_notes[i], harp_release_velocity, HARP_CHANNEL, harp_port);
        harp_started_notes[i] = 0;
      }
    }
  }
}

void handle_chord_type(bool button_maj, bool button_min, bool button_seventh) {
  if (!(button_maj || button_min || button_seventh)) {
    current_line = -1;
    return;
  }
  if (button_maj && !button_min && !button_seventh) {
    current_chord = barry_harris_mode ? &maj_sixth : &major;
  } else if (!button_maj && button_min && !button_seventh) {
    current_chord = barry_harris_mode ? &min_sixth : &minor;
  } else if (!button_maj && !button_min && button_seventh) {
    current_chord = &seventh;
  } else if (button_maj && !button_min && button_seventh) {
    current_chord = &maj_seventh;
  } else if (!button_maj && button_min && button_seventh) {
    current_chord = &min_seventh;
  } else if (button_maj && button_min && !button_seventh) {
    current_chord = barry_harris_mode ? &full_dim : &dim;
  } else if (button_maj && button_min && button_seventh) {
    current_chord = &aug;
  }
}

void detect_slash() {
  slash_chord = false;
  for (int i = 1; i < 22; i++) {
    if (chord_matrix_array[i].read_value()) {
      int slash_line = (i - 1) / 3;
      if (slash_line != current_line) {
        slash_chord = true;
        slash_value = slash_line;
      }
    }
  }
}

void update_chord_notes() {
  if (button_pushed) {
    for (int i = 0; i < 7; i++) {
      current_chord_notes[i] = calculate_note_chord(i, slash_chord, sharp_active);
    }
    Serial.println("Updating frequencies");
    if (!rythm_mode && !trigger_chord && !retrigger_chord) {
      for (int i = 0; i < 4; i++) {
        set_chord_voice_frequency(i, current_chord_notes[i]);
      }
    } else {
      for (int i = 0; i < 7; i++) {
        current_applied_chord_notes[i] = current_chord_notes[i];
      }
    }
  }
}

void update_harp_notes() {
  if (button_pushed) {
    for (int i = 0; i < 12; i++) {
      current_harp_notes[i] = calculate_note_harp(i, slash_chord, sharp_active);
      if (change_held_strings && harp_started_notes[i] != 0) {
        if (isLooperRecording)
        {
          midiEvent m;
          m.timeStamp = curLooperTick;
          m.status = MIDI_OFF;
          m.data1 = harp_started_notes[i];
          m.data2 = harp_release_velocity;      
          addMidiLooperEvent(m, HARP_CHANNEL);
        }
        usbMIDI.sendNoteOff(harp_started_notes[i], harp_release_velocity, HARP_CHANNEL, harp_port);
        if (isLooperRecording)
        {
          midiEvent m;
          m.timeStamp = curLooperTick;
          m.status = MIDI_ON;
          m.data1 = midi_base_note_transposed + current_harp_notes[i];
          m.data2 = harp_attack_velocity+volumeOffset;      
          addMidiLooperEvent(m, HARP_CHANNEL);
        }        
        usbMIDI.sendNoteOn(midi_base_note_transposed + current_harp_notes[i], harp_attack_velocity+volumeOffset, HARP_CHANNEL, harp_port);
        harp_started_notes[i] = midi_base_note_transposed + current_harp_notes[i];
        if (string_enveloppe_array[i]->isSustain()) {
          set_harp_voice_frequency(i, current_harp_notes[i]);
        }
      }
    }
  }
}

void stop_chord_notes() {
  AudioNoInterrupts();
  for (int i = 0; i < 4; i++) {
    if (chord_envelope_array1[i]->isSustain()) {
      chord_vibrato_envelope_array1[i]->noteOff();
      chord_vibrato_dc_envelope_array1[i]->noteOff();
      chord_envelope_array1[i]->noteOff();
      chord_envelope_filter_array1[i]->noteOff();
      if (chord_started_notes[i] != 0) {
        if (isLooperRecording)
        {
          midiEvent m;
          m.timeStamp = curLooperTick;
          m.status = MIDI_OFF;
          m.data1 = chord_started_notes[i];
          m.data2 = chord_release_velocity;      
          addMidiLooperEvent(m, CHORD_CHANNEL);
        }       
        usbMIDI.sendNoteOff(chord_started_notes[i], chord_release_velocity, CHORD_CHANNEL, chord_port);
        chord_started_notes[i] = 0;
      }
    }
  }
  AudioInterrupts();
}

void stop_chord_notes_orig() {
  AudioNoInterrupts();
  for (int i = 0; i < 4; i++) {
    if (chord_envelope_array[i]->isSustain()) {
      chord_vibrato_envelope_array[i]->noteOff();
      chord_vibrato_dc_envelope_array[i]->noteOff();
      chord_envelope_array[i]->noteOff();
      chord_envelope_filter_array[i]->noteOff();
      if (chord_started_notes[i] != 0) {
        if (isLooperRecording)
        {
          midiEvent m;
          m.timeStamp = curLooperTick;
          m.status = MIDI_OFF;
          m.data1 = chord_started_notes[i];
          m.data2 = chord_release_velocity;      
          addMidiLooperEvent(m, CHORD_CHANNEL);
        }       
        usbMIDI.sendNoteOff(chord_started_notes[i], chord_release_velocity, CHORD_CHANNEL, chord_port);
        chord_started_notes[i] = 0;
      }
    }
  }
  AudioInterrupts();
}

void handle_rhythm_mode() {
  for (int i = 0; i < 4; i++) {
    if (note_off_timing[i] > note_pushed_duration && chord_envelope_array1[i]->isSustain()) {
      chord_vibrato_envelope_array1[i]->noteOff();
      chord_vibrato_dc_envelope_array1[i]->noteOff();
      chord_envelope_array1[i]->noteOff();
      chord_envelope_filter_array1[i]->noteOff();
      if (chord_started_notes[i] != 0) {
        if (isLooperRecording)
        {
          midiEvent m;
          m.timeStamp = curLooperTick;
          m.status = MIDI_OFF;
          m.data1 = chord_started_notes[i];
          m.data2 = chord_release_velocity;      
          addMidiLooperEvent(m, CHORD_CHANNEL);
        }               
        usbMIDI.sendNoteOff(chord_started_notes[i], chord_release_velocity, CHORD_CHANNEL, chord_port);
        chord_started_notes[i] = 0;
      }
    }
  }
}

void handle_rhythm_mode_orig() {
  for (int i = 0; i < 4; i++) {
    if (note_off_timing[i] > note_pushed_duration && chord_envelope_array[i]->isSustain()) {
      chord_vibrato_envelope_array[i]->noteOff();
      chord_vibrato_dc_envelope_array[i]->noteOff();
      chord_envelope_array[i]->noteOff();
      chord_envelope_filter_array[i]->noteOff();
      if (chord_started_notes[i] != 0) {
        if (isLooperRecording)
        {
          midiEvent m;
          m.timeStamp = curLooperTick;
          m.status = MIDI_OFF;
          m.data1 = chord_started_notes[i];
          m.data2 = chord_release_velocity;      
          addMidiLooperEvent(m, CHORD_CHANNEL);
        }               
        usbMIDI.sendNoteOff(chord_started_notes[i], chord_release_velocity, CHORD_CHANNEL, chord_port);
        chord_started_notes[i] = 0;
      }
    }
  }
}

void handle_continuous_mode() {
  bool one_button_active = false;
  int line_accumulator[3] = {0, 0, 0};
  for (int i = 1; i < 22; i++) {
    bool active = chord_matrix_array[i].read_value();
    one_button_active |= active;
    if (active) {
      line_accumulator[i % 3]++;
    }
  }
  if (line_accumulator[0] > 2 || line_accumulator[1] > 2 || line_accumulator[2] > 2) {
    current_line = -1;
    inhibit_button = true;
  }
  if (!one_button_active) {
    inhibit_button = false;
    stop_chord_notes();
  }
}

void generateLooperSignal(SignalType_enum mode)
{
  
  LooperSignalNotes.clear();
  //looperRecordCnt = 0;
  midiEvent m;
  if (mode == LoopStartStopSignal) //start/stop
  {
    m.record = false;
    m.timeStamp = 0;
    m.status = MIDI_ON;
    m.data1 = 60;
    m.data2 = 127;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);

    m.timeStamp = 192*2; // half
    m.status = MIDI_OFF | HARP_CHANNEL;
    m.data1 = 60;
    m.data2 = 127;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);
    
    //m.timeStamp += 192*2; // half
    m.status = MIDI_ON | HARP_CHANNEL;
    m.data1 = 60;
    m.data2 = 127;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);

    m.timeStamp += 192*2; // half
    m.status = MIDI_OFF  | HARP_CHANNEL;
    m.data1 = 60;
    m.data2 = 127;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);

    //m.timeStamp += 192*2; // half
    m.status = MIDI_ON  | HARP_CHANNEL;
    m.data1 = 60;
    m.data2 = 127;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);

    m.timeStamp += 192*2; // half
    m.status = MIDI_OFF | HARP_CHANNEL;
    m.data1 = 60;
    m.data2 = 127;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);

    m.status = MIDI_ON | HARP_CHANNEL;
    m.data1 = 72;
    m.data2 = 127;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);

    m.timeStamp += 192; // quarter
    m.status = MIDI_OFF | HARP_CHANNEL;
    m.data1 = 72;
    m.data2 = 0;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);
  }
  else if (mode == LoopClearSignal)
  {
    m.record = false;
    m.timeStamp = 0;
    m.status = MIDI_ON;
    m.data1 = 60;
    m.data2 = 127;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);

    m.record = false;
    m.timeStamp = 0;
    m.status = MIDI_ON;
    m.data1 = 61;
    m.data2 = 127;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);

    m.record = false;
    m.timeStamp = 0;
    m.status = MIDI_ON;
    m.data1 = 62;
    m.data2 = 127;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);

    m.timeStamp = 192*2; // half
    m.status = MIDI_OFF | HARP_CHANNEL;
    m.data1 = 60;
    m.data2 = 127;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);
    
    m.timeStamp = 192*2; // half
    m.status = MIDI_OFF | HARP_CHANNEL;
    m.data1 = 61;
    m.data2 = 127;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);

    m.timeStamp = 192*2; // half
    m.status = MIDI_OFF | HARP_CHANNEL;
    m.data1 = 62;
    m.data2 = 127;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);

  }
  else //record
  {
    m.record = false;
    m.timeStamp = 0;
    m.status = MIDI_ON;
    m.data1 = 72;
    m.data2 = 127;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);

    m.timeStamp = 192*2; // half
    m.status = MIDI_OFF | HARP_CHANNEL;
    m.data1 = 72;
    m.data2 = 127;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);
    
    //m.timeStamp += 192*2; // half
    m.status = MIDI_ON | HARP_CHANNEL;
    m.data1 = 60;
    m.data2 = 127;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);

    m.timeStamp += 192*2; // half
    m.status = MIDI_OFF  | HARP_CHANNEL;
    m.data1 = 60;
    m.data2 = 127;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);

    //m.timeStamp += 192*2; // half
    m.status = MIDI_ON  | HARP_CHANNEL;
    m.data1 = 72;
    m.data2 = 127;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);

    m.timeStamp += 192*2; // half
    m.status = MIDI_OFF | HARP_CHANNEL;
    m.data1 = 72;
    m.data2 = 127;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);

    m.status = MIDI_ON | HARP_CHANNEL;
    m.data1 = 60;
    m.data2 = 127;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);

    m.timeStamp += 192; // quarter
    m.status = MIDI_OFF | HARP_CHANNEL;
    m.data1 = 60;
    m.data2 = 0;
    addMidiLooperEvent(m, KEYBOARD_CHANNEL, true);
  }
}

void transferRecording()
{
  int32_t offset = 0;
  if (looperBuffer.size() == 0 && looperRecordBuffer.size() > 0)
  {
    Serial.printf("Correcting time\n");
    uint32_t tRef = looperRecordBuffer[0].timeStamp;
    offset = LOOPER_MIN_TIME - tRef;
    //get offset
  }
  looperBuffer.reserve(looperBuffer.size() + looperRecordBuffer.size());
  Serial.printf("Transferring %d data to total %d offset = %d\n", looperRecordBuffer.size(), looperBuffer.size()+looperRecordBuffer.size(), offset);
  //uint32_t old;
  //todo delete after transferring
  for (int i = 0; i < (int)looperRecordBuffer.size(); i++)
  {
      //old = looperRecordBuffer[i].timeStamp;

      // Adjust timestamp before transferring
      looperRecordBuffer[i].timeStamp = (uint32_t)(looperRecordBuffer[i].timeStamp + offset);

      // Move the element into looperBuffer (no copy if using std::move)
      looperBuffer.push_back(std::move(looperRecordBuffer[i]));

      //Serial.printf("old %u vs new timestamp is %u\n", old, looperBuffer.back().timeStamp);
  }

  isLooperRecording = false;
  looperRecordBuffer.clear();
  looperRecordBuffer.shrink_to_fit();
  printMemoryUsage();
}


void loopDoStartStop()
{
  
  isLooperToBeStarting = !isLooperActive;
  Serial.printf("isLooperToBeStarting=%d\n", isLooperToBeStarting);
  curLooperTick = 0;
  curLooperCnt = 0;
  if (isLooperToBeStarting)
  {
    isLooperToBeRecording = false;
    if (isLooperRecording)
    {
      transferRecording(); //consider it that the user just created a shortcut
    }
    generateLooperSignal(LoopStartStopSignal); 

  }
  else
  {
    isLooperActive = false;
  }
}
void loopDoRecord()
{
    curLooperCnt = 0;
    curLooperTick = 0;
    looperRecordBuffer.clear(); //reset record pointer
    isLooperToBeStarting = false;
    isLooperToBeRecording = true;
    generateLooperSignal(LoopRecordSignal); 
}

void resetLooper()
{
  isLooperClearing = false;
  isLooperToBeRecording = false;
  isLooperToBeStarting = false;
  isLooperActive = false;
  isLooperRecording = false;
  curLooperTick = 0;
  looperBuffer.clear();
  looperRecordBuffer.clear();
}

void loopDoClear()
{
  resetLooper();
  generateLooperSignal(LoopClearSignal);
}
void handle_hold_button() {
  uint8_t hold_transition = hold_button.read_transition();
  if (hold_transition == 2) {
    if (!rythm_mode) {
      Serial.println("Switching mode");
      continuous_chord = !continuous_chord;
      analogWrite(RYTHM_LED_PIN, 255 * continuous_chord);
      if (current_line == -1) {
        trigger_chord = true;
      }
    } else {
      if (since_last_button_push > 100 && since_last_button_push < 2000) {
        rythm_bpm = (rythm_bpm * 5.0 + 60 * 1000 / since_last_button_push) / 6.0;
        Serial.print("Updating the BPM to: ");
        Serial.println(rythm_bpm);
        recalculate_timer();
        rythm_timer.update(current_long_period ? long_timer_period : short_timer_period);
      }
    }
    since_last_button_push = 0;
  } else if (hold_transition == 1 && since_last_button_push > 800) {
    Serial.println("Long push, switching rhythm mode");
    rythm_mode = !rythm_mode;
    continuous_chord = false;
    analogWrite(RYTHM_LED_PIN, 255 * continuous_chord);
    if (rythm_mode) {
      rythm_current_step = 0;
      Serial.println("Starting rhythm timers");
      rythm_timer.priority(254);
      rythm_timer.begin(rythm_tick_function, short_timer_period);
      rythm_timer_running = true;
      rythm_timer.update(long_timer_period);
      current_long_period = true;
    } else {
      Serial.println("Stopping rhythm timers");
      rythm_timer.end();
      rythm_timer_running = false;
    }
  }
}

void handle_preset_change() {
  uint8_t trans;
  if (sharp_active && listenShift)
  {
    //check how long up or down has been pressed
    trans = up_button.read_transition();
    if (trans == 2)
    {
      //start count for up
      Serial.printf("Up pressed\n");
      upLastPressTime = millis();
      upPressed = true;
    }
    else if (trans == 1)
    {
      Serial.printf("Up unpressed\n");
      upPressed = false;
    }
    else if (upPressed && trans == 0) // no transition
    {
      //determine if hold is enough
      if (millis() - upLastPressTime > LOOPER_BUTTON_HOLD_TIME)
      {
        listenShift = false;
        if (downPressed && millis() - downLastPressTime > LOOPER_BUTTON_SUB_HOLD_TIME)
        {
          
          Serial.printf("I should do Looper Clear here\n");
          downPressed = false;          
          loopDoClear();
        }
        else
        {
          Serial.printf("I should do Looper Start/Stop here\n");
          loopDoStartStop();
        }
        upPressed = false;
      }
    }
    else
    {

    }
    trans = down_button.read_transition();
    if (trans == 2)
    {
      Serial.printf("Down pressed\n");
      //start count for down
      downLastPressTime = millis();
      downPressed = true;
    }
    else if (trans == 1)
    {
      Serial.printf("Down unpressed\n");
      downPressed = false;
    }
    else if (downPressed && trans == 0)
    {
      if (millis() - downLastPressTime > LOOPER_BUTTON_HOLD_TIME)
      {
        listenShift = false;
        if (upPressed && millis() - upLastPressTime > LOOPER_BUTTON_SUB_HOLD_TIME)
        {
          Serial.printf("I should do Looper Clear here\n");
          upPressed = false;
          loopDoClear();
        }
        else
        {
          Serial.printf("I should do Looper Record here\n");
          loopDoRecord();
        }
        downPressed = false;
      }
    }
    else
    {

    }
    
  }
  else
  {
    if (up_button.read_transition() > 1) {
      Serial.println("Switching to next preset");
      if (!sysex_controler_connected && flag_save_needed) {
        save_config(current_bank_number, false);
      }
      current_bank_number = (current_bank_number + 1) % 12;
      load_config(current_bank_number);
    }
    if (down_button.read_transition() > 1) {
      Serial.println("Switching to last preset");
      if (!sysex_controler_connected && flag_save_needed) {
        save_config(current_bank_number, false);
      }
      current_bank_number = (current_bank_number - 1);
      if (current_bank_number == -1) {
        current_bank_number = 11;
      }
      load_config(current_bank_number);
    }
  }
}

void handle_low_battery() {
  uint8_t LBO_transition = LBO_flag.read_transition();
  if (LBO_transition == 1) {
    led_blinking_flag = true;
  } else if (LBO_transition == 2) {
    led_blinking_flag = false;
    set_led_color(bank_led_hue, 1.0, 1 - led_attenuation);
  }
  if (led_blinking_flag) {
    set_led_color(bank_led_hue, 1.0, 0.6 + 0.4 * sin(color_led_blink_val));
    color_led_blink_val += 0.005;
  }
}

void trigger_chord_notes() {
  if ((trigger_chord || (button_pushed && retrigger_chord)) && !rythm_mode) {
    Serial.println("Triggering chord notes");
    for (int i = 0; i < 4; i++) {
      note_timer[i].priority(253);
    }
    note_timer[0].begin([] { play_single_note(0, &note_timer[0]); }, 10+chord_retrigger_release*1000);          // those allow for delayed triggering
    note_timer[1].begin([] { play_single_note(1, &note_timer[1]); }, 10 +chord_retrigger_release*1000+ inter_string_delay + random(random_delay));
    note_timer[2].begin([] { play_single_note(2, &note_timer[2]); }, 10 + chord_retrigger_release*1000+inter_string_delay * 2 + random(random_delay));
    note_timer[3].begin([] { play_single_note(3, &note_timer[3]); }, 10 + chord_retrigger_release*1000+inter_string_delay * 3 + random(random_delay));
    trigger_chord = false;
  }
  button_pushed = false;
}

void handleExtUSBNoteOn(byte channel, byte note, byte velocity) {
  Serial.printf("EXT Note On: ch=%d, note=%d, vel=%d\n", channel, note, velocity);
  int c = externalUseFixedChannel ? EXTERNAL_CHANNEL : channel;
  note += 1;
  usbMIDI.sendNoteOn(note, velocity, c);
   // Map the incoming note to a harp voice or chord voice
  //int harp_index = (note - midi_base_note_transposed) % 12; // assuming 12 harp zones
  int harp_index = note % 12; // assuming 12 harp zones
  int oldAttack = harp_attack_velocity;
  if (harp_index >= 0 && harp_index < 12) {
    current_harp_notes[harp_index] = note;
    set_harp_voice_frequency(harp_index, note);
    
    AudioNoInterrupts();
    harp_attack_velocity = velocity;
    envelope_string_vibrato_lfo.noteOn();
    envelope_string_vibrato_dc.noteOn();
    string_enveloppe_filter_array[harp_index]->noteOn();
    string_enveloppe_array[harp_index]->noteOn();
    string_transient_envelope_array[harp_index]->noteOn();
    harp_attack_velocity = oldAttack;
    AudioInterrupts();
  }
}

void handleExtUSBNoteOff(byte channel, byte note, byte velocity) {
  Serial.printf("EXT Note Off: ch=%d, note=%d, vel=%d\n", channel, note, velocity);
  int c = externalUseFixedChannel ? EXTERNAL_CHANNEL : channel;
  note += 1;
  usbMIDI.sendNoteOff(note, velocity, c);
    //int harp_index = (note - midi_base_note_transposed) % 12;
    int harp_index = note % 12;
  
  if (harp_index >= 0 && harp_index < 12) {
    AudioNoInterrupts();
    envelope_string_vibrato_lfo.noteOff();
    envelope_string_vibrato_dc.noteOff();
    string_enveloppe_array[harp_index]->noteOff();
    string_transient_envelope_array[harp_index]->noteOff();
    string_enveloppe_filter_array[harp_index]->noteOff();
    AudioInterrupts();
  }
}


void HandleUSBHost()
{
  bool pedalChanged = false;
  bool pedalVel = false;
  while (midi1.read()) {
    byte type = midi1.getType();
    byte channel = midi1.getChannel();
    byte d1 = midi1.getData1();
    byte d2 = midi1.getData2();
    //int mult = 1;

    if (channel == 1 && type == midi::ControlChange) {
      pedalChanged = true;
      pedalVel = true;
      
      
    } else if (channel == 2) {
      //mult = -1;
      pedalChanged = true;
    }

      // ---------- PITCH BEND ----------
    if (type == midi::PitchBend) {
      // 14-bit raw value from LSB/MSB
      int raw14 = (d2 << 7) | d1;        // 0 .. 16383
      int signed14 = raw14 - 8192;      // -8192 .. 8191

      int mapped = 0; // will hold -8191..8191 (or 0..8191 for channel 2)

      if (channel == 2) {
        // channel 2 = only upward bend:  0 .. +8191
        if (signed14 <= 0) mapped = 0;
        else mapped = constrain(signed14, 0, 8191);
      } 
      else if (channel == 3) {
        // channel 3 = only downward bend: -8191 .. 0
        if (signed14 <= 0) mapped = 0;
        else mapped = constrain(signed14, 0, 8191) * -1;
      } 
      else {
        // fallback: full signed range (if you ever want other channels to do full-range)
        mapped = constrain(signed14, -8191, 8191);
      }

      // Send the mapped pitch-bend to MIDI (library accepts -8192..8191 style numbers)
      midiEvent m;
      m.data1 = d1;
      m.data2 = d2;
      m.status = channel == 2?MIDI_PITCH_UP:MIDI_PITCH_DOWN;
      addMidiLooperEvent(m, CHORD_CHANNEL);
      usbMIDI.sendPitchBend(mapped, CHORD_CHANNEL);

      // Convert mapped (-8191..8191 or 0..8191) into a semitone offset and then a freq multiplier
      float bendSemis = ( (float)mapped / 8192.0f ) * bendRangeSemitones; // e.g. -2..0 or 0..+2
      float bendRatio = powf(2.0f, bendSemis / 12.0f);

      // --- apply to audio oscillators safely ---
      // NOTE: these sizeof expressions work if these arrays are real static arrays visible here.
      // If they are pointers, replace with your constants (e.g. NUM_CHORD_NOTES, NUM_CHORD_OSC).
      const int numChordNotes = sizeof(current_chord_notes) / sizeof(current_chord_notes[0]);
      const int numChordOsc   = sizeof(chord_osc_1_array)    / sizeof(chord_osc_1_array[0]);
      const int applyVoices = min(numChordNotes, numChordOsc);
      Serial.printf("Pitch Bend: ch=%d, raw=%d value=%d\n", channel,raw14, mapped);
      AudioNoInterrupts();
      for (int i = 0; i < applyVoices; i++) {
        // only modify voices that are active / started
        if (i < (int)(sizeof(chord_started_notes) / sizeof(chord_started_notes[0])) && chord_started_notes[i] != 0) {
          // compute the same base frequency your set_chord_voice_frequency uses
          float baseFreq = pow(2.0f, chord_octave_change) * c_frequency / 8.0f
                           * pow(2.0f, (current_chord_notes[i] + transpose_semitones) / 12.0f);

          chord_osc_1_array[i]->frequency(osc_1_freq_multiplier * baseFreq * bendRatio);
          chord_osc_2_array[i]->frequency(osc_2_freq_multiplier * baseFreq * bendRatio);
          chord_osc_3_array[i]->frequency(osc_3_freq_multiplier * baseFreq * bendRatio);
        }
      }
      
        const int numHarpNotes = (int)(sizeof(current_harp_notes) / sizeof(current_harp_notes[0]));
        const int numStringWave = (int)(sizeof(string_waveform_array) / sizeof(string_waveform_array[0]));
        const int applyHarpVoices = min(numHarpNotes, numStringWave);

        const int numHarpStarted = (int)(sizeof(harp_started_notes) / sizeof(harp_started_notes[0]));

        for (int i = 0; i < applyHarpVoices; i++) {
          if (i < numHarpStarted && harp_started_notes[i] != 0) {
            // compute the same base frequencies as set_harp_voice_frequency, then apply bendRatio
            float note_freq = pow(2.0f, harp_octave_change) * c_frequency / 4.0f
                              * pow(2.0f, (current_harp_notes[i] + transpose_semitones) / 12.0f);
            float transient_freq = 64.0f * c_frequency / 4.0f
                                    * pow(2.0f, ((current_harp_notes[i] + transpose_semitones) % 12 + transient_note_level) / 12.0f);

            // apply bend
            string_waveform_array[i]->frequency(note_freq * bendRatio);
            string_transient_waveform_array[i]->frequency(transient_freq * bendRatio);
            // keep filter base freq but keytrack over the bent note freq
            string_filter_array[i]->frequency(string_filter_base_freq + (note_freq * bendRatio) * string_filter_keytrack);
          }
        }
      
      AudioInterrupts();
    }
    if (pedalChanged && pedalVel)
    {
        chord_attack_velocity = chord_attack_velocity_PedalUsed;
        harp_attack_velocity = harp_attack_velocity_PedalUsed; 
    }

    if (pedalVel) //adjust volume depending on pedal velocity
    {

      volumeOffset = floor(d2 - 90); //0-37
      if (volumeOffset > 37)
      {
        volumeOffset = 37;
      }
      if (volumeOffset < 0)
      {
        volumeOffset = 0;
      }
      
      int clamped = chord_attack_velocity_PedalUsed + volumeOffset;
      
      if (clamped > 127)
      {
        clamped = 127;
      }
      clamped -=90;
      if (clamped < 0)
      {
        clamped = 0;
      }


      float vel_gain = clamped / 37.0f;
      Serial.printf("Velocity Change: ch=%d, raw=%d value=%d gain = %f\n", channel, volumeOffset, clamped, vel_gain);

      AudioNoInterrupts();
      for (int i = 0; i < 12; i++)
      {
        string_waveform_array[i]->amplitude(0.5 + vel_gain*0.25); //too loud
      }

      for (int i = 0; i < 4; i++)
      {
        chord_voice_mixer_array1[i]->gain(0, 0.7 + vel_gain*0.7);
        chord_voice_mixer_array1[i]->gain(1, 0.7 + vel_gain*0.7);
        chord_voice_mixer_array1[i]->gain(2, 0.7 + vel_gain*0.7);
      }
       Serial.printf("Velocity Change: ch=%d, raw=%d act value=%d\n", channel, volumeOffset, chord_attack_velocity);
       AudioInterrupts();
    }
    
  }
  
}




template<typename SerialType>
bool decodeCmd(SerialType& serialPort, String cmd, std::vector<String>* params) {
  char buffer[64];
  //bool bTemp = false;
  if (debug) {
    Serial.printf("Serial command received is %s\n", cmd.c_str());
  }
  if (cmd == "LPSW")  //Looper start/stop write
  {
    if (params->size() < 1)  //missing parameter
    {
      serialPort.write("ER00\r\n");
      return true;
    }
    if (atoi(params->at(0).c_str()) < 0 || atoi(params->at(0).c_str()) > 1 )
    {
      serialPort.write("ER01\r\n");
      return true;
    }
    curLooperTick = 0;
    //LooperNotes.clear();
    isLooperToBeStarting = atoi(params->at(0).c_str()) == 1;
    if (isLooperToBeStarting == 1)
    {
      isLooperToBeRecording = false;
      if (isLooperRecording)
      {
        transferRecording(); //consider it that the user just created a shortcut
      }
      generateLooperSignal(LoopStartStopSignal);

    }
    else
    {
      //noteAllOff();
      isLooperActive = false;
    }
    serialPort.write("OK00\r\n");
  } 
  else if (cmd == "LPSR")  //Looper start/stop read
  {
    snprintf(buffer, sizeof(buffer), "OK00,%d\r\n", isLooperActive);
    serialPort.write(buffer);
  }

  else if (cmd == "LPRW")  //Looper record start/stop write
  {
    if (params->size() < 1)  //missing parameter
    {
      serialPort.write("ER00\r\n");
      return true;
    }
    if (atoi(params->at(0).c_str()) < 0 || atoi(params->at(0).c_str()) > 1 )
    {
      serialPort.write("ER01\r\n");
      return true;
    }
    //LooperNotes.clear();
    bool isLooperToBeRecordingNew = atoi(params->at(0).c_str()) == 1;
    curLooperTick = 0;
    Serial.printf("isLooperToBeRecordingNew = %d looperRecordBuffer.size() = %d\n", isLooperToBeRecordingNew?1:0, looperRecordBuffer.size());
    if (isLooperToBeRecordingNew) //stopped->start or start->start
    {
      looperRecordBuffer.clear(); //reset record pointer
      isLooperToBeStarting = false;
      generateLooperSignal(LoopRecordSignal);
    }
    else
    {
      if (looperRecordBuffer.size() > 0)
      { 
        //to prevent them overlapping right away
        transferRecording();
      }
      
    }
    isLooperToBeRecording = isLooperToBeRecordingNew;

    serialPort.write("OK00\r\n");
  } 

  else if (cmd == "LPRR")  //Looper record start/stop read
  {
    snprintf(buffer, sizeof(buffer), "OK00,%d\r\n", isLooperRecording);
    serialPort.write(buffer);
  }

  else if (cmd == "LPCL")  //Looper Clear
  {
    //noteAllOff();
    resetLooper();
    isLooperClearing = true;
    generateLooperSignal(LoopClearSignal);
    serialPort.write("OK00\r\n");
  } 
  return true;
}

void advanceLooperSignal() 
{
  int channel = 0;
  for (size_t i = 0; i < LooperSignalNotes.size();) {
    if (LooperSignalNotes[i].timeStamp == curLooperTick) 
    {
      auto &m = LooperSignalNotes[i];
      channel = m.status & 0xf;
      if ((m.status & 0xf0) == MIDI_ON) 
      {
        usbMIDI.sendNoteOn(m.data1, m.data2, channel);
      }
      else if ((m.status & 0xf0) == MIDI_OFF) 
      {
        usbMIDI.sendNoteOff(m.data1, m.data2, channel);
      }
      else
      {

      }
      LooperSignalNotes.erase(LooperSignalNotes.begin() + i);
    }
    else 
    {
      i++;
    }

  }
  if (LooperSignalNotes.size() == 0)
  {
    Serial.printf("Post LooperSignal\n");
    curLooperCnt = -1; //need to overflow due to loop behavior 
    isLooperActive = false;
    isLooperRecording = false;
    if (isLooperClearing)
    {
      isLooperClearing = false;
    }
    if (isLooperToBeRecording)
    {
      isLooperRecording = true;
      Serial.printf("Looper is Recording officially\n");
    }
    else if (isLooperToBeStarting)
    {
      Serial.printf("Looper is starting officially\n");
      isLooperActive = true;
    }
    curLooperTick = 0;
    curLooperCnt = 0;
  }
}

void advanceLooper() {
  int channel = 0;
  for (size_t i = 0; i < looperBuffer.size();i++) {
    if (looperBuffer[i].timeStamp == curLooperTick) 
    {
      curLooperCnt++;
      auto &m = looperBuffer[i];
      channel = m.status & 0xf;
      if ((m.status & 0xf0) == MIDI_ON) 
      {
        usbMIDI.sendNoteOn(m.data1, m.data2, channel);
        int harp_index = m.data1 % 12; // assuming 12 harp zones
        int oldAttack = harp_attack_velocity;
        if (harp_index >= 0 && harp_index < 12) {
          current_harp_notes[harp_index] = m.data1 - 24;
          set_harp_voice_frequency(harp_index, m.data1- 24);
          
          AudioNoInterrupts();
          harp_attack_velocity = m.data2;
          envelope_string_vibrato_lfo.noteOn();
          envelope_string_vibrato_dc.noteOn();
          string_enveloppe_filter_array[harp_index]->noteOn();
          string_enveloppe_array[harp_index]->noteOn();
          string_transient_envelope_array[harp_index]->noteOn();
          AudioInterrupts();
          harp_attack_velocity = oldAttack;
        }

      }
      else if ((m.status & 0xf0) == MIDI_OFF) 
      {
        usbMIDI.sendNoteOff(m.data1, m.data2, channel);
        int harp_index = m.data1 % 12; // assuming 12 harp zones
        
        if (harp_index >= 0 && harp_index < 12) {
          current_harp_notes[harp_index] = m.data1- 24;
          set_harp_voice_frequency(harp_index, m.data1- 24);
          
          AudioNoInterrupts();
          string_enveloppe_array[harp_index]->noteOff();
          string_transient_envelope_array[harp_index]->noteOff();
          string_enveloppe_filter_array[harp_index]->noteOff();
          AudioInterrupts();
        }
      }
      
      else if ((m.status & 0xf0) == MIDI_PITCH_UP || (m.status & 0xf0) == MIDI_PITCH_DOWN) 
      {
        int value = ((m.data2 << 7) | m.data1) - 8192;
        value = constrain(value + 512, -8192, 8191);
        int mult = 1;
        if (m.status == MIDI_PITCH_DOWN)
        {
          mult = -1;
        }
        usbMIDI.sendPitchBend(value * mult, channel);
      }

    }
  }
}


// Interrupt Service Routine: set flag to send clock


int tickCount = 0;
void onTick64() {
  tickCount++;

  
  if (LooperSignalNotes.size() > 0)
  {
    //Serial.printf("LooperSignalNotes %d vs %d\n", LooperSignalNotes.size(), curLooperTick);
    advanceLooperSignal();
    curLooperTick++;
  }
  //loop for handling playback
  else if (isLooperActive || isLooperRecording || isLooperClearing)
  {
    //Serial.printf("7 curLooperTick %d, curLooperCnt %d, looperBuffer.size() %d\n",curLooperTick, curLooperCnt,looperBuffer.size() );
    if (looperBuffer.size() > 0 && curLooperCnt < looperBuffer.size())
    {
      //Serial.printf("Looper looperCnt = %d, curLooperCnt = %d\n", looperCnt, curLooperCnt);

      if (LooperSignalNotes.size() == 0 && looperBuffer.size() > 0)
      {
        advanceLooper();
      }
    }
    curLooperTick++;
    bool shouldReset = false;
    if (LooperSignalNotes.size() != 0)
    {

    }
    else
    {

      if (curLooperTick > MAX_LOOPER_TIME)
      {
        shouldReset = true;
        curLooperCnt = 0;
      } 
      else if (isLooperRecording)
      {
        //Serial.printf("3 curLooperCnt %d, looperCnt %d\n",curLooperCnt,looperCnt );
      }
      else if (isLooperActive && looperBuffer.size() == 0) //handle case where record->record is done
      {
        Serial.printf("2 curLooperCnt %d, looperCnt %d\n",curLooperCnt,looperBuffer.size() );
        //do nothing
      }
      else if (isLooperActive && curLooperCnt >= looperBuffer.size())
      {
        Serial.printf("curLooperCnt %d, looperCnt %d\n",curLooperCnt,looperBuffer.size() );
        shouldReset = true;
        curLooperCnt= 0;
      }
    }
    if (shouldReset)
    {
      Serial.printf("5 curLooperTick %d, looperCnt %d\n",curLooperCnt,looperBuffer.size() );
      curLooperTick = 0;
    }
  }
  // Optional: wrap counter
  if (tickCount >= 4800) tickCount = 0;
}
#define MAX_BUFFER_SIZE 32
char dataBuffer4[MAX_BUFFER_SIZE + 1];  // +1 for null terminator

uint8_t bufferLen4 = 0;
template<typename SerialType>
void checkSerialCmd(SerialType& serialPort, char* buffer, uint8_t& bufferLen, int isBT) {
  std::vector<String> params;
  while (serialPort.available()) {
    char c = serialPort.read();

    // Prevent buffer overflow
    if (bufferLen < 255) {
      buffer[bufferLen++] = c;
    }

    // Look for command terminator
    if (c == '\n' && bufferLen >= 2 && buffer[bufferLen - 2] == '\r') {
      buffer[bufferLen] = '\0';  // Null-terminate the string

      // Optionally: remove trailing \r\n for cleaner parsing
      buffer[bufferLen - 2] = '\0';

      // --- Parse Command ---
      char* cmd = strtok(buffer, ",");
      if (cmd && strlen(cmd) == 4) {
        if (debug) {
          serialPort.printf("Received %d:CMD: %s", isBT, cmd);
        }

        //int paramIndex = 1;
        char* token;
        while ((token = strtok(NULL, ",")) != NULL) {
          //serialPort.printf("Param %d: %s\n", paramIndex++, token);
          if (debug) {
            serialPort.printf(",%s", token);
          }
          params.push_back(String(token));
        }
        if (debug)
          serialPort.printf("\n");
        // Respond with OK

      } else {
        

        
        if (isBT == 0)
        {
          Serial.printf("I strangely got %s\n", buffer);
          serialPort.write("HA? ER98\r\n");
        }
      }
      if (!decodeCmd(serialPort, String(cmd), &params)) 
      {
        serialPort.write("ER99\r\n");
        Serial.printf("DecodeCmd failed for %s\n",cmd);
      }
      // Reset buffer for next command
      bufferLen = 0;
    }
  }
}
void loop() {
  if (sendClockTick) {
    sendClockTick = false;
    onTick64();
  }
  checkSerialCmd(Serial, dataBuffer4, bufferLen4, 0);  //handling of USB serial
  
  // Process incoming MIDI messages
  if (usbMIDI.read()) {
    processMIDI();
  }
  myusb.Task();          // process USB host events
  HandleUSBHost();  // handle messages

  // Check sysex controller connection
  if (sysex_controler_connected && (USB1_PORTSC1, 7)) {
    sysex_controler_connected = false;
  }

  // Update debouncers
  hold_button.set(digitalRead(HOLD_BUTTON_PIN));
  up_button.set(digitalRead(UP_PGM_PIN));
  down_button.set(digitalRead(DOWN_PGM_PIN));
  LBO_flag.set(digitalRead(BATT_LBO_PIN));
  chord_matrix.update(chord_matrix_array);

  // Handle low battery indicator
  handle_low_battery();

  // Handle hold button for mode switching and rhythm
  handle_hold_button();

  // Handle preset changes
  handle_preset_change();

  // Handle rhythm mode note-off timing
  if (rythm_mode) {
    handle_rhythm_mode();
  }

  // Handle potentiometer updates
  bool alternate = chord_matrix_array[0].read_value();
  flag_save_needed |= chord_pot.update_parameter(alternate);
  flag_save_needed |= harp_pot.update_parameter(alternate);
  flag_save_needed |= mod_pot.update_parameter(alternate);

  // Handle continuous mode logic
  if (!continuous_chord && !rythm_mode) {
    handle_continuous_mode();
  }

  // Handle chord logic
  if (current_line >= 0) {
    fundamental = current_line;
    detect_slash();
    bool button_maj = chord_matrix_array[1 + current_line * 3].read_value();
    bool button_min = chord_matrix_array[2 + current_line * 3].read_value();
    bool button_seventh = chord_matrix_array[3 + current_line * 3].read_value();
    handle_chord_type(button_maj, button_min, button_seventh);
    update_chord_notes(); // Replaced updateNotes() with update_chord_notes()
    update_harp_notes();  // Added call to update_harp_notes()
    trigger_chord_notes();

  }

  // Handle chord button transitions
  handle_chords_button();

  // Handle harp functions
  handle_harp();
  if (!memShown)
  {
    memShown = true;
    printMemoryUsage();
  }

}

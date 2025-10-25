#define KEYBOARD_CHANNEL 1
#define QUARTERNOTETICKS 192
#define MIDI_ON 0x90
#define MIDI_OFF 0x80
#define MIDI_PITCH_UP 0xE0
#define MIDI_PITCH_DOWN 0xD0 //not really pitch bend but I won't use 0xD0
#define MAX_LOOPER_EVENTS 32000
#define MAX_LOOPER_TIME 2000000
#define LOOPER_MIN_TIME 1
#define HARP_CHANNEL 1
#define CHORD_CHANNEL 2
#define EXTERNAL_CHANNEL 3
#define EXTERNAL_CHORD_CHANNEL 4
#define HARP_LOOPER_CHANNEL 11
#define CHORD_LOOPER_CHANNEL 12
#define EXTERNAL_LOOPER_CHANNEL 13
#define EXTERNAL_CHORD_LOOPER_CHANNEL 14
//Audio Hardware
#define _MUTE_PIN 3

//Leds
#define RYTHM_LED_PIN 12
#define R_LED_PIN 0
#define G_LED_PIN 1
#define B_LED_PIN 2

//Shift register
#define SHIFT_DATA_PIN 13
#define SHIFT_STORAGE_CLOCK_PIN 14
#define SHIFT_CLOCK_PIN 15

//Buttons
#define READ_MATRIX_1_PIN 6
#define READ_MATRIX_2_PIN 5
#define READ_MATRIX_3_PIN 4

#define DOWN_PGM_PIN 8
#define UP_PGM_PIN 9

#define HOLD_BUTTON_PIN 11

//Potentiometers
#define POT_CHORD_PIN 16
#define POT_HARP_PIN 17
#define POT_MOD_PIN 22

//Battery management
#define BATT_LBO_PIN 10

//Capacitive touch
#define CAP_SDA_PIN 18
#define CAP_SDL_PIN 19

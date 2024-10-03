
#include "cmsis_os.h"
#include "usbd_midi.h"

extern float roll_deg, pitch_deg;  // Roll and pitch angles, range -90 to 90 degrees
extern USBD_HandleTypeDef hUsbDeviceFS;

uint8_t midi_tx[8] = {0x09, 0xb0, 11u, 0, 0x09, 0xb0, 1u, 0};
uint16_t success = 0;
uint8_t velocity;
uint8_t pitch_bend;

static float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static int beatToMillis(int bpm, float beat) { return (60000 / bpm) * beat; }

static void pushBuffer() {
  while (USBD_MIDI_GetState(&hUsbDeviceFS) != MIDI_IDLE) {
    osDelay(10);
  }
  USBD_MIDI_SendReport(&hUsbDeviceFS, midi_tx, 4);
}

static void note_on(uint8_t buf[4], uint8_t note, uint8_t velocity) {
  buf[0] = 0x09;
  buf[1] = 0x90;
  buf[2] = note;
  buf[3] = velocity;
  pushBuffer();
}

static void note_off(uint8_t buf[4], uint8_t note, uint8_t velocity) {
  buf[0] = 0x09;
  buf[1] = 0x80;
  buf[2] = note;
  buf[3] = velocity;
  pushBuffer();
}

void midiTask(void *pv_arg) {
  while (1) {
    osDelay(10);
    // expression cc 11
    midi_tx[3] = abs((roll_deg / 60.f) * 127);
    // modulation cc 1
    midi_tx[7] = map(pitch_deg, -10, 10, 0, 127);

    // the lick :)
    // HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
    // note_on(midi_tx, 0x3a, 77);
    // osDelay(beatToMillis(120, 0.25));
    // note_off(midi_tx, 0x3a, 77);

    // note_on(midi_tx, 0x3c, 77);
    // osDelay(beatToMillis(120, 0.25));
    // note_off(midi_tx, 0x3c, 77);

    // HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
    // note_on(midi_tx, 0x3d, 77);
    // osDelay(beatToMillis(120, 0.25));
    // note_off(midi_tx, 0x3d, 77);

    // note_on(midi_tx, 0x3f, 77);
    // osDelay(beatToMillis(120, 0.25));
    // note_off(midi_tx, 0x3f, 77);

    // HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
    // note_on(midi_tx, 0x3c, 77);
    // osDelay(beatToMillis(120, 0.5));
    // note_off(midi_tx, 0x3c, 77);

    // HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
    // note_on(midi_tx, 0x38, 77);
    // osDelay(beatToMillis(120, 0.25));
    // note_off(midi_tx, 0x38, 77);

    // note_on(midi_tx, 0x3a, 77);
    // osDelay(beatToMillis(120, 0.25));
    // note_off(midi_tx, 0x3a, 77);

    // while (USBD_MIDI_GetState(&hUsbDeviceFS) != MIDI_IDLE) {
    //     osDelay(10);
    // }
    // USBD_MIDI_SendReport(&hUsbDeviceFS, midi_tx, 8);
    // ++success;
  }
}

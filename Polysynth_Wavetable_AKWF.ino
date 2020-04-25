/* Play notes when your computer sends USB MIDI messages.

   To use this example, you must run software on your computer which
   sends MIDI.  Tools > USB Type must be set to MIDI when uploading.

   Single cycle waveforms provided by Adventure Kid, available here: https://www.adventurekid.se/akrt/waveforms/

   encoding to soundfont courtesy of 12abb17
*/

//smaller samples, to fit in Teensy 3.2 memory
//CrystalPiano3_samples.h
//#include "CrystalPiano3_samples.h"
#include "AdventureKid.h"
//#include "ElectricPiano0_samples.h"
//#include "MusicBox_samples.h"
//#include "Ocarina_samples.h"
//#include "Recorder_samples.h"
//#include "SynthStrings1_samples.h"
//#include "Vibraphone0_samples.h"
//#include "Seashore_samples.h"
//#include "AcouPiano3_samples.h"
//#include "Bird_samples.h"

#include <Bounce.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

//#define DEBUG_ALLOC

const int TOTAL_VOICES = 64;
const int TOTAL_MIXERS = 21;
const int SECONDARY_MIXERS = 4;

AudioControlSGTL5000 sgtl5000_1;
AudioSynthWavetable wavetable[TOTAL_VOICES];
AudioMixer4 mixer[TOTAL_MIXERS];
AudioOutputI2S i2s1;
AudioConnection patchCord[] = {
  {wavetable[0], 0, mixer[0], 0}, {wavetable[1], 0, mixer[0], 1}, {wavetable[2], 0, mixer[0],  2}, {wavetable[3], 0, mixer[0],  3}, {mixer[0], 0, mixer[TOTAL_MIXERS - 2], 0},
  {wavetable[4], 0, mixer[1], 0}, {wavetable[5], 0, mixer[1], 1}, {wavetable[6], 0, mixer[1],  2}, {wavetable[7], 0, mixer[1],  3}, {mixer[1], 0, mixer[TOTAL_MIXERS - 2], 1},
  {wavetable[8], 0, mixer[2], 0}, {wavetable[9], 0, mixer[2], 1}, {wavetable[10], 0, mixer[2],  2}, {wavetable[11], 0, mixer[2],  3}, {mixer[2], 0, mixer[TOTAL_MIXERS - 2], 2},
  {wavetable[12], 0, mixer[3], 0}, {wavetable[13], 0, mixer[3], 1}, {wavetable[14], 0, mixer[3],  2}, {wavetable[15], 0, mixer[3],  3}, {mixer[3], 0, mixer[TOTAL_MIXERS - 2], 3},
  {wavetable[16], 0, mixer[4], 0}, {wavetable[17], 0, mixer[4], 1}, {wavetable[18], 0, mixer[4],  2}, {wavetable[19], 0, mixer[4],  3}, {mixer[4], 0, mixer[TOTAL_MIXERS - 3], 0},
  {wavetable[20], 0, mixer[5], 0}, {wavetable[21], 0, mixer[5], 1}, {wavetable[22], 0, mixer[5],  2}, {wavetable[23], 0, mixer[5],  3}, {mixer[5], 0, mixer[TOTAL_MIXERS - 3], 1},
  {wavetable[24], 0, mixer[6], 0}, {wavetable[25], 0, mixer[6], 1}, {wavetable[26], 0, mixer[6],  2}, {wavetable[27], 0, mixer[6],  3}, {mixer[6], 0, mixer[TOTAL_MIXERS - 3], 2},
  {wavetable[28], 0, mixer[7], 0}, {wavetable[29], 0, mixer[7], 1}, {wavetable[30], 0, mixer[7],  2}, {wavetable[31], 0, mixer[7],  3}, {mixer[7], 0, mixer[TOTAL_MIXERS - 3], 3},
  {wavetable[32], 0, mixer[8], 0}, {wavetable[33], 0, mixer[8], 1}, {wavetable[34], 0, mixer[8],  2}, {wavetable[35], 0, mixer[8],  3}, {mixer[8], 0, mixer[TOTAL_MIXERS - 4], 0},
  {wavetable[36], 0, mixer[9], 0}, {wavetable[37], 0, mixer[9], 1}, {wavetable[38], 0, mixer[9],  2}, {wavetable[39], 0, mixer[9],  3}, {mixer[9], 0, mixer[TOTAL_MIXERS - 4], 1},
  {wavetable[40], 0, mixer[10], 0}, {wavetable[41], 0, mixer[10], 1}, {wavetable[42], 0, mixer[10], 2}, {wavetable[43], 0, mixer[10], 3}, {mixer[10], 0, mixer[TOTAL_MIXERS - 4], 2},
  {wavetable[44], 0, mixer[11], 0}, {wavetable[45], 0, mixer[11], 1}, {wavetable[46], 0, mixer[11], 2}, {wavetable[47], 0, mixer[11], 3}, {mixer[11], 0, mixer[TOTAL_MIXERS - 4], 3},
  {wavetable[48], 0, mixer[12], 0}, {wavetable[49], 0, mixer[12], 1}, {wavetable[50], 0, mixer[12], 2}, {wavetable[51], 0, mixer[12], 3}, {mixer[12], 0, mixer[TOTAL_MIXERS - 5], 0},
  {wavetable[52], 0, mixer[13], 0}, {wavetable[53], 0, mixer[13], 1}, {wavetable[54], 0, mixer[13], 2}, {wavetable[55], 0, mixer[13], 3}, {mixer[13], 0, mixer[TOTAL_MIXERS - 5], 1},
  {wavetable[56], 0, mixer[14], 0}, {wavetable[57], 0, mixer[14], 1}, {wavetable[58], 0, mixer[14], 2}, {wavetable[59], 0, mixer[14], 3}, {mixer[14], 0, mixer[TOTAL_MIXERS - 5], 2},
  {wavetable[60], 0, mixer[15], 0}, {wavetable[61], 0, mixer[15], 1}, {wavetable[62], 0, mixer[15], 2}, {wavetable[63], 0, mixer[15], 3}, {mixer[15], 0, mixer[TOTAL_MIXERS - 5], 3},
  {mixer[TOTAL_MIXERS - 2], 0, mixer[TOTAL_MIXERS - 1], 0},
  {mixer[TOTAL_MIXERS - 3], 0, mixer[TOTAL_MIXERS - 1], 1},
  {mixer[TOTAL_MIXERS - 4], 0, mixer[TOTAL_MIXERS - 1], 2},
  {mixer[TOTAL_MIXERS - 5], 0, mixer[TOTAL_MIXERS - 1], 3},
  {mixer[TOTAL_MIXERS - 1], 0, i2s1, 0},
  {mixer[TOTAL_MIXERS - 1], 0, i2s1, 1},
};

AudioSynthWavetable::instrument_data adventureKid1[] =
{ AKWF_0001, AKWF_0002, AKWF_0003, AKWF_0004, AKWF_0005, AKWF_0006, AKWF_0007, AKWF_0008, AKWF_0009, AKWF_0010, AKWF_0011, AKWF_0012,
  AKWF_0013, AKWF_0014, AKWF_0015, AKWF_0016, AKWF_0017, AKWF_0018, AKWF_0020, AKWF_0021, AKWF_0022, AKWF_0023, AKWF_0024, AKWF_0025, AKWF_0026, AKWF_0027, AKWF_0028, AKWF_0029, AKWF_0030,
  AKWF_0031, AKWF_0032, AKWF_0033, AKWF_0034, AKWF_0035, AKWF_0036, AKWF_0037, AKWF_0038, AKWF_0039, AKWF_0040, AKWF_0041, AKWF_0042, AKWF_0043, AKWF_0044, AKWF_0045, AKWF_0046, AKWF_0047,
  AKWF_0048, AKWF_0049, AKWF_0050, AKWF_0051, AKWF_0052, AKWF_0053, AKWF_0054, AKWF_0055, AKWF_0056, AKWF_0057, AKWF_0058, AKWF_0059, AKWF_0060, AKWF_0061, AKWF_0062, AKWF_0063, AKWF_0064,
  AKWF_0065, AKWF_0066, AKWF_0067, AKWF_0068, AKWF_0069, AKWF_0070, AKWF_0071, AKWF_0072, AKWF_0073, AKWF_0074, AKWF_0075, AKWF_0076, AKWF_0077, AKWF_0078, AKWF_0079, AKWF_0080, AKWF_0081,
  AKWF_0082, AKWF_0083, AKWF_0084, AKWF_0085, AKWF_0086, AKWF_0087, AKWF_0088, AKWF_0089, AKWF_0090, AKWF_0091, AKWF_0092, AKWF_0093, AKWF_0094, AKWF_0095, AKWF_0096, AKWF_0097, AKWF_0098,
  AKWF_0099, AKWF_0100, AKWF_0102, AKWF_0103, AKWF_0104, AKWF_0105, AKWF_0106, AKWF_0107, AKWF_0108, AKWF_0109, AKWF_0110, AKWF_0111, AKWF_0112, AKWF_0113, AKWF_0114, AKWF_0115, AKWF_0116,
  AKWF_0117, AKWF_0118, AKWF_0119, AKWF_0120, AKWF_0121, AKWF_0122, AKWF_0123, AKWF_0124, AKWF_0125, AKWF_0126, AKWF_0127
  };
Bounce buttons[] = { {0, 15}, {1, 15}, {2, 15}, };
const int TOTAL_BUTTONS = sizeof(buttons) / sizeof(Bounce);
static int CURRENT_INSTRUMENT = 01;

void guitarHeroMode();
void printVoices();
void setVolume() {
  sgtl5000_1.volume(0.8 * (analogRead(PIN_A2) - 1) / 1022.0);
}

struct voice_t {
  int wavetable_id;
  byte channel;
  byte note;
};
voice_t voices[TOTAL_VOICES];

IntervalTimer midiMapTimer;
IntervalTimer guitarHeroTimer;
IntervalTimer volumeTimer;

void setup() {
  Serial.begin(115200);

  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);

  AudioMemory(120);

  sgtl5000_1.enable();
  sgtl5000_1.volume(.58);
  sgtl5000_1.audioPostProcessorEnable();
  sgtl5000_1.autoVolumeControl(0, 0, 1, -24.0, .01, .02);
  //sgtl5000_1.autoVolumeEnable();
  //sgtl5000_1.dacVolumeRamp();
  sgtl5000_1.adcHighPassFilterDisable();

  for (int i = 0; i < TOTAL_VOICES; ++i) {
    wavetable[i].setInstrument(adventureKid1[CURRENT_INSTRUMENT]);
    wavetable[i].amplitude(.14);
    voices[i].wavetable_id = i;
    voices[i].channel = voices[i].note = 0xFF;
  }

  for (int i = 0; i < TOTAL_MIXERS - 1; ++i)
    for (int j = 0; j < 4; ++j)
      mixer[i].gain(j, .0995);//0.14925);
  for (int i = 0; i < 4; ++i)
    mixer[TOTAL_MIXERS - 1].gain(i, i < SECONDARY_MIXERS ? 75.0 / SECONDARY_MIXERS : 0.0);

  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleControlChange(OnControlChange);
  //volumeTimer.begin(setVolume, 100000);
  //guitarHeroTimer.begin(guitarHeroMode, 1000000 / 120);
  //midiMapTimer.begin(printVoices, 5000);

  delay(2000);
}

void loop() {
  usbMIDI.read();
  //for (int i = 0; i < TOTAL_BUTTONS; ++i) buttons[i].update();
  //if (buttons[0].fallingEdge()) AudioSynthWavetable::print_performance();
  //if (buttons[1].risingEdge()) {
  //  midiMapTimer.end();
  //  Serial.print('\n');
  //}
  //if (buttons[1].fallingEdge()) midiMapTimer.begin(printVoices, 5000);
  //if (buttons[2].risingEdge()) guitarHeroTimer.end();
  //if (buttons[2].fallingEdge())
  //  guitarHeroTimer.begin(guitarHeroMode, 1000000/60);
}

int allocateVoice(byte channel, byte note);
int findVoice(byte channel, byte note);
void freeVoices();

int used_voices = 0;
int stopped_voices = 0;
int evict_voice = 0;
int notes_played = 0;

void OnPress(int key)
{
  Serial.print("key '");
  Serial.print((char)key);
  Serial.print("'  ");
  Serial.println(key);
  //Serial.print("key ");
  //Serial.print((char)keyboard1.getKey());
  //Serial.print("  ");
  //Serial.print((char)keyboard2.getKey());
  //Serial.println();
}

void OnControlChange(byte channel, byte control, byte value)
{
  Serial.print("Control Change, ch=");
  Serial.print(channel);
  Serial.print(", control=");
  Serial.print(control);
  Serial.print(", value=");
  Serial.print(value);
  Serial.println();

  switch (control) {
    case 70:
      CURRENT_INSTRUMENT = value;
      for (int i = 0; i < TOTAL_VOICES; ++i) {
        wavetable[i].setInstrument(adventureKid1[CURRENT_INSTRUMENT]);
        wavetable[i].amplitude(.03);
        voices[i].wavetable_id = i;
        voices[i].channel = voices[i].note = 0xFF;
      }
      break;
  }
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  notes_played++;
#ifdef DEBUG_ALLOC
  //Serial.printf("**** NoteOn: channel==%hhu,note==%hhu ****\n", channel, note);
  printVoices();
#endif //DEBUG_ALLOC
  freeVoices();
  int wavetable_id = allocateVoice(channel, note);
  switch (channel) {
    case 1:
      wavetable[wavetable_id].setInstrument(adventureKid1[CURRENT_INSTRUMENT]);
      break;
    /*
      case 2:
        wavetable[wavetable_id].setInstrument(Vibraphone0);
        break;
      case 3:
        wavetable[wavetable_id].setInstrument(MusicBox);
        break;
      case 4:
        wavetable[wavetable_id].setInstrument(AcouPiano3);
        break;
      case 5:
        wavetable[wavetable_id].setInstrument(SynthStrings1);
        break;
      case 6:
        wavetable[wavetable_id].setInstrument(Ocarina);
        break;
      case 7:
        wavetable[wavetable_id].setInstrument(Seashore);
        break;
      case 8:
        wavetable[wavetable_id].setInstrument(Recorder);
        break;
      case 9:
        wavetable[wavetable_id].setInstrument(SynthStrings1);
        break;
      case 10:
        wavetable[wavetable_id].setInstrument(ElectricPiano0);
        break;
      case 11:
        wavetable[wavetable_id].setInstrument(ElectricPiano0);
        break;
    */
    default:
      wavetable[wavetable_id].setInstrument(adventureKid1[CURRENT_INSTRUMENT]);
      break;
  }
  wavetable[wavetable_id].playNote(note, velocity);
  //wavetable[wavetable_id+1].playNote(note, velocity);
#ifdef DEBUG_ALLOC
  printVoices();
#endif //DEBUG_ALLOC
}

void OnNoteOff(byte channel, byte note, byte velocity) {
#ifdef DEBUG_ALLOC
  //Serial.printf("\n**** NoteOff: channel==%hhu,note==%hhu ****", channel, note);
  printVoices();
#endif //DEBUG_ALLOC
  int wavetable_id = findVoice(channel, note);
  if (wavetable_id != TOTAL_VOICES)
    wavetable[wavetable_id].stop();
#ifdef DEBUG_ALLOC
  printVoices();
#endif //DEBUG_ALLOC
}

int allocateVoice(byte channel, byte note) {
  int i;
  int nonfree_voices = stopped_voices + used_voices;
  if (nonfree_voices < TOTAL_VOICES) {
    for (i = nonfree_voices; i < TOTAL_VOICES && voices[i].channel != channel; ++i);
    if (i < TOTAL_VOICES) {
      voice_t temp = voices[i];
      voices[i] = voices[nonfree_voices];
      voices[nonfree_voices] = temp;
    }
    i = nonfree_voices;
    used_voices++;
  }
  else {
    if (stopped_voices) {
      i = evict_voice % stopped_voices;
      voice_t temp = voices[i];
      stopped_voices--;
      voices[i] = voices[stopped_voices];
      voices[stopped_voices] = temp;
      used_voices++;
      i = stopped_voices;
    }
    else
      i = evict_voice;
  }

  voices[i].channel = channel;
  voices[i].note = note;

  evict_voice++;
  evict_voice %= TOTAL_VOICES;

  return voices[i].wavetable_id;
}

int findVoice(byte channel, byte note) {
  int i;
  //find match
  int nonfree_voices = stopped_voices + used_voices;
  for (i = stopped_voices; i < nonfree_voices && !(voices[i].channel == channel && voices[i].note == note); ++i);
  //return TOTAL_VOICES if no match
  if (i == (nonfree_voices)) return TOTAL_VOICES;

  voice_t temp = voices[i];
  voices[i] = voices[stopped_voices];
  voices[stopped_voices] = temp;
  --used_voices;

  return voices[stopped_voices++].wavetable_id;
}

void freeVoices() {
  for (int i = 0; i < stopped_voices; i++)
    if (wavetable[voices[i].wavetable_id].isPlaying() == false) {
      voice_t temp = voices[i];
      --stopped_voices;
      voices[i] = voices[stopped_voices];
      int nonfree_voices = stopped_voices + used_voices;
      voices[stopped_voices] = voices[nonfree_voices];
      voices[nonfree_voices] = temp;
    }
}

void guitarHeroMode() { // now unicorn friendly
  const int RESET = 4;
  const int MIDI_NOTES = 128;
  static char line[MIDI_NOTES + 1] = { 0 };
  static int accumulated = 0;
  if (!accumulated) {
    for (int i = 0; i < MIDI_NOTES; ++i) line[i] = '-';
    ++accumulated;
  }
  for (int i = stopped_voices; i < used_voices + stopped_voices; ++i) line[voices[i].note] = '*';
  if (accumulated == RESET) {
    Serial.println(line);
    accumulated = 0;
  }
  else {
    ++accumulated;
  }
}

const char* note_map[] = {
  "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
};

void printVoices() {
  static int last_notes_played = notes_played;
  if (last_notes_played == notes_played)
    return;
  last_notes_played = notes_played;
  int usage = AudioProcessorUsage();
  Serial.printf("\nCPU:%03i voices:%02i CPU/Voice:%02i evict:%02i", usage, used_voices, usage / used_voices, evict_voice);
  for (int i = 0; i < used_voices; ++i)
    Serial.printf(" %02hhu %-2s", voices[i].channel, note_map[voices[i].note % 12]);

}

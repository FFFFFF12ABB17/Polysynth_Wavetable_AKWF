#include "Ocarina_samples.h"

static const uint32_t sample_0_Ocarina_OcarinaGb2[128] = {
0xff7cff71,0x008dffdd,0x02970179,0x04eb03ca,0x06c505ef,0x079a074b,0x06f3077d,0x04d3061e,
0x00e10321,0xfae0fe20,0xf3fdf75f,0xedf3f0c8,0xea3debba,0xe94be968,0xeb29e9e7,0xefe9ed34,
0xf748f357,0x0094fb9a,0x0c630659,0x1801127c,0x1f9e1c4f,0x229f21dc,0x20f62265,0x1af91e6e,
0x123016d7,0x04f20c22,0xf42afcf7,0xe28deb16,0xd462dadd,0xcd4fd00e,0xcc9ccc23,0xd228ce88,
0xddbad75f,0xecf2e523,0x005df5f4,0x16c30b43,0x2bde21f0,0x39c433ca,0x3f113dd1,0x3c183e88,
0x31773763,0x20e42a06,0x0a1815b6,0xef25fdb2,0xd1b4e01a,0xbb48c554,0xaf78b3b1,0xaec6ad96,
0xb81fb2a6,0xc91dbfd6,0xe17cd48a,0xff02efb4,0x206d0fbd,0x3f893093,0x53b44aff,0x5bb45991,
0x57485ad8,0x476b508d,0x2fd33be8,0x11fe2208,0xec26fff3,0xc3ead782,0xa34eb26a,0x92ef98de,
0x91b3907c,0x9d979654,0xb44aa76c,0xd4acc3bc,0xf992e64e,0x24d20e74,0x4fa93af5,0x6afa5f76,
0x753772bd,0x6d8872f2,0x595d64c2,0x3d6d4c8f,0x187a2c15,0xecf60343,0xbe23d4fa,0x9a77aa7a,
0x882b8f0b,0x873685c5,0x95568cbb,0xaf53a113,0xd17dbf86,0xf919e4fa,0x24b50e9a,0x4ee83a57,
0x6aba5eef,0x750d7265,0x6dbc72fc,0x596764f3,0x3d484c55,0x188c2c12,0xed04035a,0xbe23d503,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,

};

static const AudioSynthWavetable::sample_data Ocarina_samples[1] = {
	{
		(int16_t*)sample_0_Ocarina_OcarinaGb2, // sample
		true, // LOOP
		8, // LENGTH_BITS
		(1 << (32 - 8)) * WAVETABLE_CENTS_SHIFT(32) * 44100.0 / WAVETABLE_NOTE_TO_FREQUENCY(88) / AUDIO_SAMPLE_RATE_EXACT + 0.5, // PER_HERTZ_PHASE_INCREMENT
		((uint32_t)191 - 1) << (32 - 8), // MAX_PHASE
		((uint32_t)190 - 1) << (32 - 8), // LOOP_PHASE_END
		(((uint32_t)190 - 1) << (32 - 8)) - (((uint32_t)156 - 1) << (32 - 8)), // LOOP_PHASE_LENGTH
		uint16_t(UINT16_MAX * WAVETABLE_DECIBEL_SHIFT(-3.0)), // INITIAL_ATTENUATION_SCALAR
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // DELAY_COUNT
		uint32_t(7.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // ATTACK_COUNT
		uint32_t(6.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // HOLD_COUNT
		uint32_t(1739.09 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // DECAY_COUNT
		uint32_t(318.09 * AudioSynthWavetable::SAMPLES_PER_MSEC / AudioSynthWavetable::ENVELOPE_PERIOD + 0.5), // RELEASE_COUNT
		int32_t((1.0 - WAVETABLE_DECIBEL_SHIFT(-2.3)) * AudioSynthWavetable::UNITY_GAIN), // SUSTAIN_MULT
		uint32_t(0.00 * AudioSynthWavetable::SAMPLES_PER_MSEC / (2 * AudioSynthWavetable::LFO_PERIOD)), // VIBRATO_DELAY
		uint32_t(4.3 * AudioSynthWavetable::LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // VIBRATO_INCREMENT
		(WAVETABLE_CENTS_SHIFT(0) - 1.0) * 4, // VIBRATO_PITCH_COEFFICIENT_INITIAL
		(1.0 - WAVETABLE_CENTS_SHIFT(0)) * 4, // VIBRATO_COEFFICIENT_SECONDARY
		uint32_t(15.01 * AudioSynthWavetable::SAMPLES_PER_MSEC / (2 * AudioSynthWavetable::LFO_PERIOD)), // MODULATION_DELAY
		uint32_t(4.3 * AudioSynthWavetable::LFO_PERIOD * (UINT32_MAX / AUDIO_SAMPLE_RATE_EXACT)), // MODULATION_INCREMENT
		(WAVETABLE_CENTS_SHIFT(0) - 1.0) * 4, // MODULATION_PITCH_COEFFICIENT_INITIAL
		(1.0 - WAVETABLE_CENTS_SHIFT(0)) * 4, // MODULATION_PITCH_COEFFICIENT_SECOND
		int32_t(UINT16_MAX * (WAVETABLE_DECIBEL_SHIFT(0) - 1.0)) * 4, // MODULATION_AMPLITUDE_INITIAL_GAIN
		int32_t(UINT16_MAX * (1.0 - WAVETABLE_DECIBEL_SHIFT(0))) * 4, // MODULATION_AMPLITUDE_FINAL_GAIN
	},
};

static const uint8_t Ocarina_ranges[] = {127, };

const AudioSynthWavetable::instrument_data Ocarina = {1, Ocarina_ranges, Ocarina_samples };

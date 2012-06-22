typedef struct krad_mixer_St krad_mixer_t;
typedef struct krad_mixer_portgroup_St krad_mixer_portgroup_t;
typedef struct krad_mixer_portgroup_St krad_mixer_mixbus_t;
typedef struct krad_mixer_crossfade_group_St krad_mixer_crossfade_group_t;

#define KRAD_MIXER_MAX_PORTGROUPS 20
#define KRAD_MIXER_MAX_CHANNELS 8
#define KRAD_MIXER_DEFAULT_SAMPLE_RATE 48000
#define KRAD_MIXER_DEFAULT_TICKER_PERIOD 512

#include "krad_radio.h"

#ifndef KRAD_MIXER_H
#define KRAD_MIXER_H

#include "hardlimiter.h"



typedef enum {
	OUTPUT,
	INPUT,
	MIX,
} krad_mixer_portgroup_direction_t;

typedef enum {
	KRAD_TONE,
	KRAD_AUDIO, /* i.e local audio i/o */
	KRAD_LINK, /* i.e. remote audio i/o */
	MIXBUS,	/* i.e. mixer internal i/o */
} krad_mixer_portgroup_io_t;

typedef enum {
	NIL,
	MONO,
	STEREO,
	THREE,
	QUAD,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
} channels_t;

struct krad_mixer_crossfade_group_St {

	krad_mixer_portgroup_t *portgroup[2];
	float fade;

};

struct krad_mixer_portgroup_St {
	
	char sysname[256];
	krad_mixer_portgroup_direction_t direction;
	krad_mixer_portgroup_io_t io_type;
	void *io_ptr;
	channels_t channels;
	krad_mixer_mixbus_t *mixbus;
	krad_mixer_crossfade_group_t *crossfade_group;
	
	float volume[KRAD_MIXER_MAX_CHANNELS];
	float volume_actual[KRAD_MIXER_MAX_CHANNELS];
	float new_volume_actual[KRAD_MIXER_MAX_CHANNELS];
	int last_sign[KRAD_MIXER_MAX_CHANNELS];

	float peak[KRAD_MIXER_MAX_CHANNELS];
	float *samples[KRAD_MIXER_MAX_CHANNELS];

	int active;
	
	krad_mixer_t *krad_mixer;
	krad_tags_t *krad_tags;

	krad_xmms_t *krad_xmms;

};


struct krad_mixer_St {

	krad_audio_t *krad_audio;

	krad_audio_api_t pusher;
	krad_ticker_t *krad_ticker;
	int ticker_running;
	int ticker_period;
	pthread_t ticker_thread;
		
	char *name;
	int sample_rate;
    
	krad_mixer_mixbus_t *master_mix;

	krad_mixer_portgroup_t *tone_port;
	char *push_tone;	
	char push_tone_value[64];
    
	krad_mixer_portgroup_t *portgroup[KRAD_MIXER_MAX_PORTGROUPS];
	krad_mixer_crossfade_group_t *crossfade_group;

	krad_ipc_server_t *krad_ipc;

};


void krad_mixer_bind_portgroup_xmms2 (krad_mixer_t *krad_mixer, char *portgroupname, char *ipc_path);
void krad_mixer_unbind_portgroup_xmms2 (krad_mixer_t *krad_mixer, char *portgroupname);
		

void *krad_mixer_ticker_thread (void *arg);
void krad_mixer_start_ticker (krad_mixer_t *krad_mixer);
void krad_mixer_stop_ticker (krad_mixer_t *krad_mixer);

krad_audio_api_t krad_mixer_get_pusher (krad_mixer_t *krad_mixer);
int krad_mixer_has_pusher (krad_mixer_t *krad_mixer);
void krad_mixer_set_pusher (krad_mixer_t *krad_mixer, krad_audio_api_t pusher);
void krad_mixer_unset_pusher (krad_mixer_t *krad_mixer);

int krad_mixer_get_sample_rate ();
void krad_mixer_set_sample_rate ();

int krad_mixer_process (uint32_t nframes, krad_mixer_t *krad_mixer);
krad_mixer_t *krad_mixer_create (char *name);
void krad_mixer_destroy (krad_mixer_t *krad_mixer);
void krad_mixer_set_ipc (krad_mixer_t *krad_mixer, krad_ipc_server_t *krad_ipc);

int krad_mixer_handler ( krad_mixer_t *krad_mixer, krad_ipc_server_t *krad_ipc );

krad_mixer_portgroup_t *krad_mixer_portgroup_create (krad_mixer_t *krad_mixer, char *sysname, int direction, int channels, 
													 krad_mixer_mixbus_t *mixbus, krad_mixer_portgroup_io_t io_type, void *io_ptr, krad_audio_api_t api);
void krad_mixer_portgroup_destroy (krad_mixer_t *krad_mixer, krad_mixer_portgroup_t *portgroup);
krad_mixer_portgroup_t *krad_mixer_get_portgroup_from_sysname (krad_mixer_t *krad_mixer, char *sysname);

void krad_mixer_crossfade_group_create (krad_mixer_t *krad_mixer, krad_mixer_portgroup_t *portgroup1, krad_mixer_portgroup_t *portgroup2);
void krad_mixer_crossfade_group_destroy (krad_mixer_t *krad_mixer, krad_mixer_crossfade_group_t *crossfade_group);
void crossfade_group_set_crossfade (krad_mixer_crossfade_group_t *crossfade_group, float value);

void portgroup_update_volume (krad_mixer_portgroup_t *portgroup);
void portgroup_set_channel_volume (krad_mixer_portgroup_t *portgroup, int channel, float value);
void portgroup_set_volume (krad_mixer_portgroup_t *portgroup, float value);
void portgroup_set_crossfade (krad_mixer_portgroup_t *portgroup, float value);

char *krad_mixer_channel_number_to_string (int channel);
void krad_mixer_portgroup_compute_channel_peak (krad_mixer_portgroup_t *portgroup, int channel, uint32_t nframes);
void krad_mixer_portgroup_compute_peaks (krad_mixer_portgroup_t *portgroup, uint32_t nframes);
float krad_mixer_portgroup_read_peak (krad_mixer_portgroup_t *portgroup);
float krad_mixer_portgroup_read_channel_peak (krad_mixer_portgroup_t *portgroup, int channel);
float krad_mixer_peak_scale (float value);
//int krad_mixer_jack_xrun_callback (void *arg);
#endif

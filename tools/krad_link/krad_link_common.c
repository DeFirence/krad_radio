#include "krad_link_common.h"

/* The lot of this should be generated by a script once my cone shaped hat is removed */

char *krad_opus_bandwidth_to_string (int bandwidth) {

	switch ( bandwidth ) {
		case OPUS_AUTO:
			return "OPUS_AUTO";
		case OPUS_BANDWIDTH_NARROWBAND:
			return "OPUS_BANDWIDTH_NARROWBAND";
		case OPUS_BANDWIDTH_MEDIUMBAND:
			return "OPUS_BANDWIDTH_MEDIUMBAND";
		case OPUS_BANDWIDTH_WIDEBAND:
			return "OPUS_BANDWIDTH_WIDEBAND";			
		case OPUS_BANDWIDTH_SUPERWIDEBAND:
			return "OPUS_BANDWIDTH_SUPERWIDEBAND";
		case OPUS_BANDWIDTH_FULLBAND:
			return "OPUS_BANDWIDTH_FULLBAND";
		default:
			return "OPUS_AUTO";
	}		
}

char *krad_opus_signal_to_string (int signal) {

	switch ( signal ) {
		case OPUS_AUTO:
			return "OPUS_AUTO";
		case OPUS_SIGNAL_VOICE:
			return "OPUS_SIGNAL_VOICE";
		case OPUS_SIGNAL_MUSIC:
			return "OPUS_SIGNAL_MUSIC";
		default:
			return "OPUS_AUTO";
	}		
}

int krad_opus_string_to_signal (char *string) {

	if ((strlen(string) == 17) && (strncmp(string, "OPUS_SIGNAL_VOICE", 17) == 0)) {
		return OPUS_SIGNAL_VOICE;
	}

	if ((strlen(string) == 17) && (strncmp(string, "OPUS_SIGNAL_MUSIC", 17) == 0)) {
		return OPUS_SIGNAL_MUSIC;
	}

	return OPUS_AUTO;
}

int krad_opus_string_to_bandwidth (char *string) {

	if ((strlen(string) == 25) && (strncmp(string, "OPUS_BANDWIDTH_NARROWBAND", 25) == 0)) {
		return OPUS_BANDWIDTH_NARROWBAND;
	}

	if ((strlen(string) == 25) && (strncmp(string, "OPUS_BANDWIDTH_MEDIUMBAND", 25) == 0)) {
		return OPUS_BANDWIDTH_MEDIUMBAND;
	}
	
	if ((strlen(string) == 23) && (strncmp(string, "OPUS_BANDWIDTH_WIDEBAND", 23) == 0)) {
		return OPUS_BANDWIDTH_WIDEBAND;
	}
	
	if ((strlen(string) == 28) && (strncmp(string, "OPUS_BANDWIDTH_SUPERWIDEBAND", 28) == 0)) {
		return OPUS_BANDWIDTH_SUPERWIDEBAND;
	}
	
	if ((strlen(string) == 23) && (strncmp(string, "OPUS_BANDWIDTH_FULLBAND", 23) == 0)) {
		return OPUS_BANDWIDTH_FULLBAND;
	}			

	return OPUS_AUTO;
}

char *krad_codec_to_string (krad_codec_t codec) {

	switch (codec) {
		case VORBIS:
			return "Vorbis";
		case FLAC:
			return "FLAC";
		case OPUS:
			return "Opus";
		case VP8:
			return "VP8";
		case THEORA:
			return "Theora";
		case DIRAC:
			return "Dirac";
		case MJPEG:
			return "Mjpeg";
		case PNG:
			return "PNG";
		case HEXON:
			return "Hexon";
		case DAALA:
			return "Daala";			
		default:
			return "No Codec";
	}
}

krad_codec_t krad_string_to_codec (char *string) {

	if (strcmp(string, "Vorbis") == 0) {
		return VORBIS;
	}

	if (strcmp(string, "FLAC") == 0) {
		return FLAC;
	}

	if (strcmp(string, "Opus") == 0) {
		return OPUS;
	}

	if (strcmp(string, "VP8") == 0) {
		return VP8;
	}

	if (strcmp(string, "VP8") == 0) {
		return TRANSMIT;
	}
	
	if (strcmp(string, "Theora") == 0) {
		return THEORA;
	}

	if (strcmp(string, "Dirac") == 0) {
		return DIRAC;
	}
	
	if (strcmp(string, "Mjpeg") == 0) {
		return MJPEG;
	}
	
	if (strcmp(string, "PNG") == 0) {
		return PNG;
	}
	
	if (strcmp(string, "Hexon") == 0) {
		return HEXON;
	}
	
	if (strcmp(string, "Daala") == 0) {
		return DAALA;
	}

	return NOCODEC;

}

krad_link_av_mode_t krad_link_string_to_av_mode (char *string) {

	if ((strcmp(string, "av") == 0) || (strcmp(string, "audiovideo") == 0) || (strcmp(string, "audio and video") == 0)) {
		return AUDIO_AND_VIDEO;
	}

	if ((strcmp(string, "video") == 0) || (strcmp(string, "videoonly") == 0) || (strcmp(string, "video only") == 0)) {
		return VIDEO_ONLY;
	}

	return AUDIO_ONLY;

}

char *krad_link_av_mode_to_string (krad_link_av_mode_t av_mode) {

	switch (av_mode) {
		case AUDIO_ONLY:
			return "audio only";
		case VIDEO_ONLY:
			return "video only";
		case AUDIO_AND_VIDEO:
			return "audio and video";
		default:
			return "Unknown";
	}
}

char *krad_link_operation_mode_to_string (krad_link_operation_mode_t operation_mode) {

	switch (operation_mode) {
		case CAPTURE:
			return "capture";
		case RECEIVE:
			return "receive";
		case RECORD:
			return "record";
		case TRANSMIT:
			return "transmit";
		case PLAYBACK:
			return "playback";
		case FAILURE:
			return "failure";
		default:
			return "Unknown";
	}
}

krad_link_operation_mode_t krad_link_string_to_operation_mode (char *string) {

	if (strcmp(string, "capture") == 0) {
		return CAPTURE;
	}

	if (strcmp(string, "receive") == 0) {
		return RECEIVE;
	}

	if (strcmp(string, "record") == 0) {
		return RECORD;
	}

	if (strcmp(string, "transmit") == 0) {
		return TRANSMIT;
	}

	if (strcmp(string, "playback") == 0) {
		return PLAYBACK;
	}

	return FAILURE;

}

krad_link_video_source_t krad_link_string_to_video_source (char *string) {

	if (strcmp(string, "test") == 0) {
		return TEST;
	}

	if ((strcmp(string, "x11") == 0) || (strcmp(string, "X11") == 0)) {
		return X11;
	}
	
	if (strcmp(string, "decklink") == 0) {
		return DECKLINK;
	}
	
	if ((strcmp(string, "V4L2") == 0) || (strcmp(string, "v4l2") == 0) || (strcmp(string, "v4l") == 0)) {
		return V4L2;
	}	

	return NOVIDEO;

}

char *krad_link_video_source_to_string (krad_link_video_source_t video_source) {

	switch (video_source) {
		case TEST:
			return "test";
		case V4L2:
			return "V4L2";
		case DECKLINK:
			return "decklink";
		case X11:
			return "X11";
		case NOVIDEO:
			return "novideo";
		default:
			return "Unknown";
	}
}

krad_link_transport_mode_t krad_link_string_to_transport_mode (char *string) {

	if ((strcmp(string, "UDP") == 0) || (strcmp(string, "udp") == 0)) {
		return UDP;
	}
	
	if ((strcmp(string, "TCP") == 0) || (strcmp(string, "tcp") == 0)) {
		return TCP;
	}
	
	if ((strcmp(string, "filesystem") == 0) || (strcmp(string, "FILESYSTEM") == 0)) {
		return FILESYSTEM;
	}	

	return FAIL;

}

char *krad_link_transport_mode_to_string (krad_link_transport_mode_t transport_mode) {

	switch (transport_mode) {
		case TCP:
			return "TCP";
		case UDP:
			return "UDP";
		case FILESYSTEM:
			return "filesystem";
		case FAIL:
			return "failure";
		default:
			return "Unknown";
	}
}
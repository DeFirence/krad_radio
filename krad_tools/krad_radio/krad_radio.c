#include "krad_radio.h"

int do_shutdown;
int verbose;

void krad_radio_daemonize () {

	pid_t pid, sid;

	close (STDIN_FILENO);
	close (STDOUT_FILENO);
	close (STDERR_FILENO);

	pid = fork();

	if (pid < 0) {
		exit (EXIT_FAILURE);
	}

	if (pid > 0) {
		exit (EXIT_SUCCESS);
	}
	
	umask(0);
 
	sid = setsid();
	
	if (sid < 0) {
		exit(EXIT_FAILURE);
	}

	if ((chdir("/")) < 0) {
		exit(EXIT_FAILURE);
	}
        
}

int krad_radio_valid_sysname (char *sysname) {
	
	int i = 0;
	char j;
	
	char requirements[] = "Sysnames must be atleast 4 numbers or lowercase letters, start with a letter, and be no longer than 40 characters.";
	
	
	if (strlen(sysname) < 4) {
		fprintf (stderr, "Sysname Name %s is invalid, too short!\n", sysname);
		fprintf (stderr, "%s\n", requirements);
		return 0;
	}
	
	if (strlen(sysname) > 40) {
		fprintf (stderr, "Sysname %s is invalid, too long!\n", sysname);
		fprintf (stderr, "%s\n", requirements);
		return 0;
	}
	
	j = sysname[i];
	if (!((isalpha (j)) && (islower (j)))) {
		fprintf (stderr, "Sysname %s is invalid, must start with a lowercase letter!\n", sysname);
		fprintf (stderr, "%s\n", requirements);
		return 0;
	}
	i++;

	while (sysname[i]) {
		j = sysname[i];
			if (!isalnum (j)) {
				fprintf (stderr, "Sysname %s is invalid, alphanumeric only!\n", sysname);
				fprintf (stderr, "%s\n", requirements);
				return 0;
			}
			if (isalpha (j)) {
				if (!islower (j)) {
					fprintf (stderr, "Sysname %s is invalid lowercase letters only!\n", sysname);
					fprintf (stderr, "%s\n", requirements);
					return 0;
				}
			}
		i++;
	}

	return 1;

}

void krad_radio_destroy (krad_radio_t *krad_radio) {

	krad_http_server_destroy (krad_radio->krad_http);
	krad_websocket_server_destroy (krad_radio->krad_websocket);
	krad_ipc_server_destroy (krad_radio->krad_ipc);
	krad_linker_destroy (krad_radio->krad_linker);
	krad_mixer_destroy (krad_radio->krad_mixer);
	krad_tags_destroy (krad_radio->krad_tags);	
	free (krad_radio->sysname);
	free (krad_radio);

}

krad_radio_t *krad_radio_create (char *sysname) {

	krad_radio_t *krad_radio = calloc(1, sizeof(krad_radio_t));


	if (!krad_radio_valid_sysname(sysname)) {
		return NULL;
	}
	krad_radio->sysname = strdup (sysname);
	
	krad_radio->krad_tags = krad_tags_create ();
	
	if (krad_radio->krad_tags == NULL) {
		krad_radio_destroy (krad_radio);
		return NULL;
	}
	
	krad_radio->krad_mixer = krad_mixer_create (krad_radio);
	
	if (krad_radio->krad_mixer == NULL) {
		krad_radio_destroy (krad_radio);
		return NULL;
	}
	
	krad_radio->krad_linker = krad_linker_create (krad_radio);
	
	if (krad_radio->krad_linker == NULL) {
		krad_radio_destroy (krad_radio);
		return NULL;
	}	
	
	krad_radio->krad_ipc = krad_ipc_server ( sysname, krad_radio_handler, krad_radio );
	
	if (krad_radio->krad_ipc == NULL) {
		krad_radio_destroy (krad_radio);
		return NULL;
	}
	
	/* fixme - should be added after the fact via ipc command */
	
		krad_radio->krad_websocket = krad_websocket_server_create (sysname, 46001);
	
		if (krad_radio->krad_websocket == NULL) {
			krad_radio_destroy (krad_radio);
			return NULL;
		}	
	
		krad_radio->krad_http = krad_http_server_create (46000);
	
		if (krad_radio->krad_http == NULL) {
			krad_radio_destroy (krad_radio);
			return NULL;
		}	
	
	/* end fixme */
	
	return krad_radio;

}


void krad_radio (char *sysname) {

	krad_radio_t *krad_radio_station;

	do_shutdown = 0;
	verbose = 1;

	krad_radio_station = krad_radio_create (sysname);

	if (krad_radio_station != NULL) {
	
		printf("Krad Radio Station %s Daemonizing..\n", krad_radio_station->sysname);
		//krad_radio_daemonize ();

		krad_radio_run ( krad_radio_station );

		krad_radio_destroy (krad_radio_station);

	}
}


void krad_radio_run (krad_radio_t *krad_radio) {

	while (1) {
		sleep (25);
	}
}


int krad_radio_handler ( void *output, int *output_len, void *ptr ) {

	krad_radio_t *krad_radio_station = (krad_radio_t *)ptr;
	
	uint32_t command;
	uint64_t ebml_data_size;
	uint64_t element;
	uint64_t response;
//	uint64_t broadcast;
//	uint64_t number;
	
	char tag_name_actual[256];
	char tag_value_actual[1024];
	
	tag_name_actual[0] = '\0';
	tag_value_actual[0] = '\0';
	
	char *tag_name = tag_name_actual;
	char *tag_value = tag_value_actual;
	
	int i;
	
	i = 0;
	command = 0;
	ebml_data_size = 0;
	element = 0;
	response = 0;

	//printf("handler! \n");
	
	krad_ipc_server_read_command ( krad_radio_station->krad_ipc, &command, &ebml_data_size);

	switch ( command ) {
	
		case EBML_ID_KRAD_RADIO_CMD:
			return krad_radio_handler ( output, output_len, ptr );
		case EBML_ID_KRAD_MIXER_CMD:
			printf("got a krad MIXER cmd\n");
			return krad_mixer_handler ( krad_radio_station->krad_mixer, krad_radio_station->krad_ipc );
		case EBML_ID_KRAD_LINK_CMD:
			printf("got a krad link cmd\n");
			return krad_linker_handler ( krad_radio_station->krad_linker, krad_radio_station->krad_ipc );
			return 0;
		case EBML_ID_KRAD_RADIO_CMD_LIST_TAGS:
			printf("get tags list\n");
			krad_ipc_server_response_start ( krad_radio_station->krad_ipc, EBML_ID_KRAD_RADIO_MSG, &response);
			krad_ipc_server_response_list_start ( krad_radio_station->krad_ipc, EBML_ID_KRAD_RADIO_TAG_LIST, &element);
			
			while (krad_tags_get_next_tag ( krad_radio_station->krad_tags, &i, &tag_name, &tag_value)) {
				krad_ipc_server_response_add_tag ( krad_radio_station->krad_ipc, tag_name, tag_value);
				printf("got tag %d: %s to %s\n", i, tag_name, tag_value);
			}
			
			krad_ipc_server_response_list_finish ( krad_radio_station->krad_ipc, element);
			krad_ipc_server_response_finish ( krad_radio_station->krad_ipc, response);
			return 1;
			break;
		case EBML_ID_KRAD_RADIO_CMD_SET_TAG:
			
			krad_ipc_server_read_tag ( krad_radio_station->krad_ipc, &tag_name, &tag_value );
			
			krad_tags_set_tag ( krad_radio_station->krad_tags, tag_name, tag_value);

			printf("set tag %s to %s\n", tag_name, tag_value);
			

			*output_len = 666;
			return 2;
			break;	
		case EBML_ID_KRAD_RADIO_CMD_GET_TAG:
			krad_ipc_server_read_tag ( krad_radio_station->krad_ipc, &tag_name, &tag_value );
			
			tag_value = krad_tags_get_tag (krad_radio_station->krad_tags, tag_name);
			
			printf("get tag %s its %s\n", tag_name, tag_value);
			
			krad_ipc_server_response_start ( krad_radio_station->krad_ipc, EBML_ID_KRAD_RADIO_MSG, &response);
			krad_ipc_server_response_add_tag ( krad_radio_station->krad_ipc, tag_name, tag_value);
			krad_ipc_server_response_finish ( krad_radio_station->krad_ipc, response);
			

			return 1;
			break;
			
		default:
			printf("unknown command! %u\n", command);
			return 0;
			break;
	}

	return 0;

}
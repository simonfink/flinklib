#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <stdbool.h>

#include <flinklib.h>

#define DEFAULT_DEV "/dev/flink0"

int main(int argc, char* argv[]) {
	flink_dev*    dev;
	flink_subdev* subdev;
	char*         dev_name = DEFAULT_DEV;
	uint8_t       subdevice_id = 0;
	int           error = 0,e;
	char          str[29];
	
	// Error message if long dashes (en dash) are used
	int i;
	for (i=0; i < argc; i++) {
		 if ((argv[i][0] == 226) && (argv[i][1] == 128) && (argv[i][2] == 147)) {
			fprintf(stderr, "Error: Invalid arguments. En dashes are used.\n");
			return -1;
		 }
	}

	/* Compute command line arguments */
	int c;
	while((c = getopt(argc, argv, "d:s:c:rwhl")) != -1) {
		switch(c) {
			case 'd': // device file
				dev_name = optarg;
				break;
			case 's': // subdevice id
				subdevice_id = atoi(optarg);
				break;
			case '?':
				if(optopt == 'd' || optopt == 's' || optopt == 'c') fprintf(stderr, "Option -%c requires an argument.\n", optopt);
				else if(isprint(optopt)) fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
				return -1;
			default:
				abort();
		}
	}

	// Open flink device
	dev = flink_open(dev_name);
	if(dev == NULL) {
		fprintf(stderr, "Failed to open device %s!\n", dev_name);
		return -1;
	}

	// Get a pointer to the choosen subdevice
	subdev = flink_get_subdevice_by_id(dev, subdevice_id);
	if(subdev == NULL) {
		fprintf(stderr, "Illegal subdevice id %d!\n", subdevice_id);
		return -1;
	}

	// Read description
	printf("Reading info device: subdevice %u\n", subdevice_id);
	error = flink_info_get_description(subdev, str);
	if(error != 0) {
		printf("Reading description failed!\n");
		return -1;
	} else {
		printf("Description: %s\n", str);
	}

	// Close flink device
	flink_close(dev);

    return EXIT_SUCCESS;
}

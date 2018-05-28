#include <stdio.h>
#include <string.h>
#include "command.h"
#include "coder.h"

int main(int argc, char * argv[]) { 
	if(argc < 3) {
		printf("\n\tUsage:\n");
		printf("\t $ ./coder encode <in-file-name> <out-file-name>\n"
			   "\t $ ./coder decode <in-file-name> <out-file-name>\n\n");
	    return 0;
	}
	if(strcmp(argv[1], "encode") == 0) {
		encode_file(argv[2], argv[3]);
		return 0;
	}
	if(strcmp(argv[1], "decode") == 0) {
		decode_file(argv[2], argv[3]);
		return 0;
	}

	return 0;
}
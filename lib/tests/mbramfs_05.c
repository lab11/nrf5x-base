
#include <stdio.h>

int main (int argc, char** argv) {
	char* fname = argv[1];

	FILE* f;
	char mydata_start[10] = "abcDEFghi";
	char mydata_end[10];
	int num;

	f = fopen(fname, "w");
	fwrite(mydata_start, 1, 10, f);
	fclose(f);

	remove(fname);

	f = fopen(fname, "r");
	if (f == NULL) {
		printf("could not open file\n");
	} else {
		num = fread(mydata_end, 1, 10, f);

		printf("Read %i bytes: \n", num);
		for (int i=0; i<num; i++) {
			printf("%c\n", mydata_end[i]);
		}
	}


	// Do this to make the build system happy...
	f = fopen(fname, "w");
	fwrite(mydata_start, 1, 10, f);
	fclose(f);

	return 0;
}


#include <stdio.h>

int main (int argc, char** argv) {
	char* fname = argv[1];

	FILE* f;
	char mydata_start[10] = "abcDEFghi";
	char mydata_end[10];
	int num;

	f = fopen(fname, "w");
	fwrite(mydata_start, 1, 4, f);
	fclose(f);

	f = fopen(fname, "wa");
	fwrite(mydata_start, 1, 4, f);
	fclose(f);

	f = fopen(fname, "r");
	num = fread(mydata_end, 1, 15, f);

	printf("Read %i bytes: \n", num);
	for (int i=0; i<num; i++) {
		printf("%c\n", mydata_end[i]);
	}

	return 0;
}

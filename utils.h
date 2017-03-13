#ifndef UTILS_H_
#define UTILS_H_

#include <string.h>

FILE *open_file(char *file_name)
{
	FILE *fd = fopen(file_name, "a+");
	
	if (!fd) {
		fprintf(stderr, "File not found");
		exit(-1);
	}
	return fd;
}

void print_result(char *result, char *output_file)
{
	if (output_file == NULL)
		printf("%s\n", result);
	else {
		FILE *fd;
		
		fd = open_file(output_file);
		fprintf(fd, "%s\n", result);
		fclose(fd);
	}
}

char *copy_string(char *str)
{
	char *tmp = (char *)malloc((strlen(str) + 1) * sizeof(char));
	
	strcpy(tmp, str);
	return tmp;
}

int check_if_string_is_number(char *number)
{
	if (atoi(number) == 0) {
		fprintf(stderr, "The index is not a number\n");
		return 0;
	}
	return 1;
}
#endif

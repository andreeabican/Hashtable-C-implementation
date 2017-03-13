#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashbucket.h"
#include "utils.h"
#include "bucket.h"

#define MAX_CMD_SIZE 20000 

int hashtable_dimension;
struct Bucket *hashtable;

void initialize_hashtable(struct Bucket **hash, int dimension)
{
	int i;

	*hash = (struct Bucket *)calloc(dimension, sizeof(struct Bucket));
	for (i = 0; i < dimension; i++)
		initialize_bucket(&(*hash)[i]);
}

void free_hashtable(struct Bucket **hash, int dimension)
{
	int i;
	
	for (i = 0; i < dimension; i++)
		free_bucket(&(*hash)[i]);
	free(*hash);
}

char *extract_cmd_param(char *command, int param_index)
{
	char const delim[6] = "\n \r";
	int i;
	char *param;
	char *tmp = (char *)calloc((strlen(command) + 1), sizeof(char));
	
	strcpy(tmp, command);
	param = strtok(tmp, delim);
	for (i = 0; i < param_index; i++)
		param = strtok(NULL, delim);
	return param;
}

void execute_add_cmd(char *command)
{
	char *word = extract_cmd_param(command, 1);
	unsigned int word_hashcode = hash(word, hashtable_dimension);
	
	if (find_in_bucket(hashtable[word_hashcode], word) == 0)
		add_in_bucket(&hashtable[word_hashcode], word);
	else
		fprintf(stderr, "Add command error: Word already exists\n");
}

void execute_remove_cmd(char *command)
{
	char *word = extract_cmd_param(command, 1);
	unsigned int index = hash(word, hashtable_dimension);
	
	if (find_in_bucket(hashtable[index], word) == 1)
		remove_from_bucket(&hashtable[index], word);
	else
		fprintf(stderr, "Remove command error: Word doesn't exist\n");
}

void execute_find_cmd(char *command)
{
	char *word = extract_cmd_param(command, 1);
	char *output_file = extract_cmd_param(command, 2);
	unsigned int hashcode = hash(word, hashtable_dimension);
	int result = find_in_bucket(hashtable[hashcode], word);
	
	if (result == 1)
		print_result("True", output_file);
	else
		print_result("False", output_file);
}

void execute_print_bucket_cmd(char *command)
{
	char *index_bucket = extract_cmd_param(command, 1);
	char *output_file = extract_cmd_param(command, 2);
	
	if (check_if_string_is_number(index_bucket) != 0) {
		int index = atoi(index_bucket);
		char *output;
		
		if (index < hashtable_dimension) {
			output = get_words_in_bucket(hashtable[index], 1);
			print_result(output, output_file);
			free(output);
		} else
			fprintf(stderr, "Print bucket cmd error: big index\n");
	}
}

void execute_print_cmd(char *command)
{
	char *output_file = extract_cmd_param(command, 1);
	int i, contor = 0;
	
	for (i = 0; i < hashtable_dimension; i++) {
		char *current_bucket = get_words_in_bucket(hashtable[i], 0);
		
		if (strlen(current_bucket) != 0)
			print_result(current_bucket, output_file);
		free(current_bucket);
	}
}

struct Bucket *copy_hashtable(int dim)
{
	struct Bucket *tmp_hashtable;
	int i, j;
	
	initialize_hashtable(&tmp_hashtable, dim);
	for (i = 0; i < hashtable_dimension; i++) {
		for (j = 0; j < hashtable[i].dimension; j++) {
			unsigned int index = hash(hashtable[i].words[j], dim);
			char *word = hashtable[i].words[j];
			
			if (find_in_bucket(tmp_hashtable[index], word) == 0)
				add_in_bucket(&tmp_hashtable[index], word);
		}
	}
	return tmp_hashtable;
}

void execute_resize_cmd(char *command)
{
	char *word = extract_cmd_param(command, 1);
	struct Bucket *tmp_hashtable;
	int tmp_dimension;
	
	if (strcmp(word, "double") == 0) {
		tmp_dimension = hashtable_dimension * 2;
		tmp_hashtable = copy_hashtable(tmp_dimension);
	} else if (strcmp(word, "halve") == 0) {
		tmp_dimension = hashtable_dimension / 2;
		tmp_hashtable = copy_hashtable(tmp_dimension);
	}
	free_hashtable(&hashtable, hashtable_dimension);
	hashtable = tmp_hashtable;	
	hashtable_dimension = tmp_dimension;
}

void execute_clear_command(char *command)
{
	free_hashtable(&hashtable, hashtable_dimension);
	initialize_hashtable(&hashtable, hashtable_dimension);
}

void execute_cmd(char *command)
{
	if (strncmp(command, "add ", 4) == 0)
		execute_add_cmd(command);
	else if (strncmp(command, "remove ", 7) == 0)
		execute_remove_cmd(command);
	else if (strncmp(command, "find ", 5) == 0)
		execute_find_cmd(command);
	else if (strncmp(command, "print_bucket ", 13) == 0)
		execute_print_bucket_cmd(command);
	else if (strncmp(command, "print", 5) == 0)
		execute_print_cmd(command);
	else if (strncmp(command, "resize ", 7) == 0)
		execute_resize_cmd(command);
	else if (strncmp(command, "clear", 5) == 0)
		execute_clear_command(command);
	else if (strncmp(command, "\n", 1) == 0) {	
	} else {
		fprintf(stderr, "Wrong command\n");
		exit(-1);
	}
}

void read_input_from_stdin(int param_necesar)
{
	char *command = (char *)calloc(MAX_CMD_SIZE, sizeof(char));
	
	fgets(command, MAX_CMD_SIZE, stdin);
	while (!feof(stdin)) {
		execute_cmd(command);
		fgets(command, MAX_CMD_SIZE, stdin);
	}
	free(command);
}

void read_commands_from_file(char *file_name)
{
	FILE *fd = open_file(file_name);
	char *command = (char *)calloc(MAX_CMD_SIZE, sizeof(char));
	
	while (fgets(command, MAX_CMD_SIZE, fd))
		execute_cmd(command);
	free(command);
	fclose(fd);
}

void parse_input_files_from_cmd_line_args(int argc, char *argv[])
{
	int i;
	
	for (i = 2; i < argc; i++)
		read_commands_from_file(argv[i]);
}

void parse_cmd_line_args(int argc, char *argv[])
{
	if (argc == 1) {
		fprintf(stderr, "Command line argumets error:\n");
		fprintf(stderr, "Too few arguments\n");
		exit(-1);
	}
	if (check_if_string_is_number(argv[1]) != 0) {
		hashtable_dimension = atoi(argv[1]);
		initialize_hashtable(&hashtable, hashtable_dimension);
	} else
		exit(-1);
	if (argc == 2)
		read_input_from_stdin(1);
	else
		parse_input_files_from_cmd_line_args(argc, argv);
}

int main(int argc, char *argv[])
{
	parse_cmd_line_args(argc, argv);
}

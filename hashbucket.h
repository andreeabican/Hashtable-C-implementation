#ifndef HASHBUCKET_H_
#define HASHBUCKET_H_
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "bucket.h"

#define MAX_WORD_SIZE 20000

void initialize_bucket(struct Bucket *bucket)
{
	int i;
	
	bucket->dimension = 0;
	bucket->words = (char **)calloc(bucket->dimension, sizeof(char *));
	for (i = 0; i < bucket->dimension; i++)
		bucket->words[i] = (char *)calloc(MAX_WORD_SIZE, sizeof(char));
}

void free_bucket(struct Bucket *bucket)
{
	int i;
	
	for (i = 0; i < bucket->dimension; i++)
		free(bucket->words[i]);
	free(bucket->words);
}

void add_in_bucket(struct Bucket *bucket, char *word)
{
	int new_dimension = (bucket->dimension + 1) * sizeof(char *);
	int bucket_dim = bucket->dimension;
	int len = strlen(word) + 1;
	
	bucket->words = (char **)realloc(bucket->words, new_dimension);
	bucket->words[bucket_dim] = (char *)calloc(len, sizeof(char));
	strcpy(bucket->words[bucket->dimension], word);
	bucket->dimension++;
}

int find_in_bucket(struct Bucket bucket, char *word)
{
	int i;
	
	for (i = 0; i < bucket.dimension; i++)
		if (strcmp(bucket.words[i], word) == 0)
			return 1;
	return 0;
}

int get_bucket_size(struct Bucket bucket)
{
	int i, size = 0;
	
	for (i = 0; i < bucket.dimension; i++)
		size += strlen(bucket.words[i]) + 1;
	return size;
}

char *get_words_in_bucket(struct Bucket bucket, int command)
{
	int i;
	int size = get_bucket_size(bucket);
	char *output = (char *)calloc(size + 1, sizeof(char));
	
	for (i = 0; i < bucket.dimension; i++) {
		strcat(output, bucket.words[i]);
		strcat(output, " ");
	}
	return output;
}

int get_index_in_bucket(struct Bucket bucket, char *word)
{
	int i;
	
	for (i = 0; i < bucket.dimension; i++)
		if (strcmp(bucket.words[i], word) == 0)
			return i;
	return -1;
}

void remove_from_bucket(struct Bucket *bucket, char *word)
{
	int i;
	int index = get_index_in_bucket(*bucket, word);
	
	for (i = index; i < bucket->dimension-1; i++)
		strcpy(bucket->words[i], bucket->words[i+1]);
	free(bucket->words[bucket->dimension-1]);
	bucket->dimension--;
}

#endif

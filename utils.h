// Copyright Toma Ariciu 312CAb 2023-2024
#pragma once

#include <stdio.h>
#define MAX_LENGTH 1024

typedef struct selection {
	int x1, y1, x2, y2;
} selection_t;

void swap(int *x, int *y);
int read_ascii(FILE *in);
void print_ascii(FILE *out, int x);
int read_binary(FILE *in);
void print_binary(FILE *out, int x);
int clamp(int x, int mini, int maxi);

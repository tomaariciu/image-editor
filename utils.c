// Copyright Toma Ariciu 312CAb 2023-2024
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

// Swap cu xor
void swap(int *x, int *y)
{
	(*x) = (*x) ^ (*y);
	(*y) = (*y) ^ (*x);
	(*x) = (*x) ^ (*y);
}

// Citirea unui numar in format ascii
int read_ascii(FILE *in)
{
	int x;
	fscanf(in, "%d", &x);
	return x;
}

// Afisarea unui numar in format ascii
void print_ascii(FILE *out, int x)
{
	fprintf(out, "%d ", x);
}

// Citirea unui numar in format binar
int read_binary(FILE *in)
{
	unsigned char x;
	fread(&x, sizeof(unsigned char), 1, in);
	int aux = x;
	return aux;
}

// Afisarea unui numar in format binar
void print_binary(FILE *out, int x)
{
	unsigned char aux = x;
	fwrite(&aux, sizeof(unsigned char), 1, out);
}

int clamp(int x, int mini, int maxi)
{
	if (x < mini)
		return mini;
	if (x > maxi)
		return maxi;
	return x;
}

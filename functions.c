// Copyright Toma Ariciu 312CAb 2023-2024
#include <string.h>
#include <math.h>
#include "functions.h"

// Returnez in array-ul word primul cuvant/numar neparsat inca
void get_word(char str[MAX_LENGTH], char word[MAX_LENGTH], int *pos)
{
	memset(word, 0, MAX_LENGTH * sizeof(word[0]));
	int word_pos = 0, n = strlen(str);
	while ((*pos) < n && (str[*pos] == ' ' || str[*pos] == '\n'))
		(*pos)++;
	while ((*pos) < n && str[*pos] != ' ' && str[*pos] != '\n') {
		word[word_pos] = str[*pos];
		word_pos++;
		(*pos)++;
	}
	while ((*pos) < n && (str[*pos] == ' ' || str[*pos] == '\n'))
		(*pos)++;
}

// Transform un cuvant intr-un numar
int get_number(char str[MAX_LENGTH], int *pos, int *nr)
{
	char aux[MAX_LENGTH];
	int x = 0, sgn = 1;
	if ((*pos) == (int)strlen(str))
		return -1;
	get_word(str, aux, pos);
	for (int i = 0; i < (int)strlen(aux); i++) {
		if (aux[i] == '-') {
			sgn = -1;
			continue;
		}
		if (aux[i] < '0' || aux[i] > '9')
			return -1;
		x = x * 10 + aux[i] - '0';
	}
	*nr = sgn * x;
	return 0;
}

// Sar peste comentarii, verificand primul caracter al liniei citite
void skip_comments(FILE *in, fpos_t *pos)
{
	char line[MAX_LENGTH];
	do {
		fgetpos(in, pos);
		fgets(line, MAX_LENGTH, in);
	} while (line[0] == '#');
	// Linia curenta nu este un comentariu, asa ca mut cursorul
	// Pentru ca aceasta sa fie citita din nou
	fsetpos(in, pos);
}

// Construiesc nucleul de kernel in functie de parametrul dat
void apply_initialize(char parameter[MAX_LENGTH], int *valid_parameter,
					  int kernel[3][3], int *div)
{
	if (strcmp(parameter, "EDGE") == 0) {
		*valid_parameter = 1;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
				kernel[i][j] = -1;
		}
		kernel[1][1] = 8;
	}
	if (strcmp(parameter, "SHARPEN") == 0) {
		*valid_parameter = 1;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (i % 2 == 0 && j % 2 == 0)
					kernel[i][j] = 0;
				else
					kernel[i][j] = -1;
			}
		}
		kernel[1][1] = 5;
	}
	if (strcmp(parameter, "BLUR") == 0) {
		*valid_parameter = 1;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
				kernel[i][j] = 1;
		}
		*div = 9;
	}
	if (strcmp(parameter, "GAUSSIAN_BLUR") == 0) {
		*valid_parameter = 1;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (i % 2 == 0 && j % 2 == 0)
					kernel[i][j] = 1;
				else
					kernel[i][j] = 2;
			}
		}
		kernel[1][1] = 4;
		*div = 16;
	}
}

// Aplic kernelul blocului de 3x3 din imaginea initiala
// Si returnez noua valoare pe care o va avea pixelul
int apply_matrix(int mat[3][3], int kernel[3][3], int div)
{
	int ans = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			ans += kernel[i][j] * mat[i][j];
	}
	ans = round(1.0 * ans / (1.0 * div));
	return ans;
}

// Resetez selectia la intreaga imagine
void reset_selection_to_image(image_t *img, selection_t *sel)
{
	sel->x1 = 0;
	sel->y1 = 0;
	sel->x2 = img->n;
	sel->y2 = img->m;
}

// Resetez selectia la starea default
void reset_selection(selection_t *sel)
{
	sel->x1 = 0;
	sel->y1 = 0;
	sel->x2 = 0;
	sel->y2 = 0;
}

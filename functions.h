// Copyright Toma Ariciu 312CAb 2023-2024
#pragma once
#include "utils.h"
#include "image.h"

void get_word(char str[MAX_LENGTH], char word[MAX_LENGTH], int *pos);
int get_number(char str[MAX_LENGTH], int *pos, int *nr);
void skip_comments(FILE *in, fpos_t *pos);
void apply_initialize(char parameter[MAX_LENGTH], int *valid_parameter,
					  int kernel[3][3], int *div);
int apply_matrix(int mat[3][3], int kernel[3][3], int div);
void reset_selection_to_image(image_t *img, selection_t *sel);
void reset_selection(selection_t *sel);

// Copyright Toma Ariciu 312CAb 2023-2024
#pragma once
#include "functions.h"

void load_command(char line[MAX_LENGTH], int *pos, image_t *img,
				  selection_t *sel, int *loaded);
void select_command(char line[MAX_LENGTH], int *pos, image_t *img,
					selection_t *sel);
void rotate_command(char line[MAX_LENGTH], int *pos, image_t *img,
					selection_t *sel);
void equalize_command(image_t *img, selection_t *sel);
void crop_command(image_t *img, selection_t *sel);
void apply_command(char line[MAX_LENGTH], int *pos, image_t *img,
				   selection_t *sel);
void histogram_command(char line[MAX_LENGTH], int *pos, image_t *img,
					   selection_t *sel);
void save_command(char line[MAX_LENGTH], int *pos, image_t *img);
void exit_command(image_t *img, selection_t *sel);

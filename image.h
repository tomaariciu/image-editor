// Copyright Toma Ariciu 312CAb 2023-2024
#pragma once
#include <stdio.h>

typedef struct pixel {
	int v[3];
} pixel_t;

typedef struct image {
	int n, m, type, max_val;
	pixel_t **mat;
} image_t;

int create_image(image_t *img);
void delete_matrix(image_t *img);
void delete_image(image_t *img);
int copy_image(image_t *img, image_t *new_img);
void read_image(FILE *in, image_t *img);
void print_image(FILE *out, image_t *img, int type);

// Copyright Toma Ariciu 312CAb 2023-2024
#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "utils.h"

// Aloc o matrice cu dimensiunile imaginii
int create_image(image_t *img)
{
	img->mat = (pixel_t **)malloc(img->n * sizeof(pixel_t *));
	if (!img->mat)
		return -1;
	for (int i = 0; i < img->n; i++) {
		img->mat[i] = (pixel_t *)malloc(img->m * sizeof(pixel_t));
		if (!img->mat[i]) {
			for (int j = 0; j < i; j++)
				free(img->mat[j]);
			return -1;
		}
	}
	return 0;
}

// Dealoc doar matricea de pixeli a imaginii
void delete_matrix(image_t *img)
{
	for (int i = 0; i < img->n; i++)
		free(img->mat[i]);
	free(img->mat);
}

// Dealoc toata memoria folosita pentru imagine
void delete_image(image_t *img)
{
	delete_matrix(img);
	free(img);
}

// Copiez imaginea curenta in una noua
int copy_image(image_t *img, image_t *new_img)
{
	new_img->n = img->n;
	new_img->m = img->m;
	new_img->type = img->type;
	new_img->max_val = img->max_val;
	if (create_image(new_img) != 0)
		return -1;
	for (int i = 0; i < new_img->n; i++) {
		for (int j = 0; j < new_img->m; j++)
			new_img->mat[i][j] = img->mat[i][j];
	}
	return 0;
}

// Citesc imaginea din fisier, in functie de tipul acesteia
void read_image(FILE *in, image_t *img)
{
	for (int i = 0; i < img->n; i++) {
		for (int j = 0; j < img->m; j++) {
			if (img->type == 2)
				img->mat[i][j].v[0] = read_ascii(in);

			if (img->type == 3)
				for (int c = 0; c < 3; c++)
					img->mat[i][j].v[c] = read_ascii(in);

			if (img->type == 5)
				img->mat[i][j].v[0] = read_binary(in);

			if (img->type == 6)
				for (int c = 0; c < 3; c++)
					img->mat[i][j].v[c] = read_binary(in);
		}
	}
}

// Afisez imaginea intr-un fisier, in functie de tipul in care vreau sa fie
// salvata
void print_image(FILE *out, image_t *img, int type)
{
	for (int i = 0; i < img->n; i++) {
		for (int j = 0; j < img->m; j++) {
			if (type == 2)
				print_ascii(out, img->mat[i][j].v[0]);

			if (type == 3)
				for (int c = 0; c < 3; c++)
					print_ascii(out, img->mat[i][j].v[c]);

			if (type == 5)
				print_binary(out, img->mat[i][j].v[0]);

			if (type == 6)
				for (int c = 0; c < 3; c++)
					print_binary(out, img->mat[i][j].v[c]);
		}
		if (type <= 3)
			fprintf(out, "\n");
	}
}

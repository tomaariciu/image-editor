// Copyright Toma Ariciu 312CAb 2023-2024
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "commands.h"

// Incarc in memorie imaginea data
void load_command(char line[MAX_LENGTH], int *pos, image_t *img,
				  selection_t *sel, int *loaded)
{
	char filename[MAX_LENGTH], magic[MAX_LENGTH];
	get_word(line, filename, pos);
	FILE *in = fopen(filename, "r");
	fpos_t file_pos;
	// Dealoc imaginea incarcata, daca aceasta exista
	if ((*loaded)) {
		delete_matrix(img);
		reset_selection(sel);
	}
	(*loaded) = 0;
	if (!in) {
		printf("Failed to load %s\n", filename);
		return;
	}
	// Sar peste comentarii si citesc imaginea
	skip_comments(in, &file_pos);
	fscanf(in, "%s\n", magic);
	img->type = magic[1] - '0';
	skip_comments(in, &file_pos);
	// Dimensiunile sunt date in format "coloane linii"
	// Asta imi afecteaza toate citirile/afisarile de coordonate
	fscanf(in, "%d %d\n", &img->m, &img->n);
	skip_comments(in, &file_pos);
	fscanf(in, "%d\n", &img->max_val);
	skip_comments(in, &file_pos);
	if (create_image(img) != 0) {
		free(img);
		free(sel);
		printf("Memory allocation failed\n");
		exit(-1);
	}
	read_image(in, img);
	reset_selection_to_image(img, sel);
	(*loaded) = 1;
	printf("Loaded %s\n", filename);
	fclose(in);
}

// Parsez parametrii comenzii de select si schimb selectia in cea data
void select_command(char line[MAX_LENGTH], int *pos, image_t *img,
					selection_t *sel)
{
	char str[MAX_LENGTH];
	int aux_pos = *pos, v[4], error = 0;
	get_word(line, str, pos);
	if (strcmp(str, "ALL") == 0) {
		reset_selection_to_image(img, sel);
		printf("Selected ALL\n");
		return;
	}
	*pos = aux_pos;
	// Verific validitatea tuturor parametrilor
	for (int i = 0; i < 4; i++)
		error |= get_number(line, pos, &v[i]);
	error |= ((*pos) != (int)strlen(line));
	if (error) {
		printf("Invalid command\n");
		return;
	}
	if (v[0] > v[2])
		swap(&v[0], &v[2]);
	if (v[1] > v[3])
		swap(&v[1], &v[3]);
	if (v[0] < 0 || v[1] < 0 || v[3] > img->n || v[2] > img->m ||
		v[0] == v[2] || v[1] == v[3]) {
		printf("Invalid set of coordinates\n");
		return;
	}
	sel->x1 = v[1];
	sel->y1 = v[0];
	sel->x2 = v[3];
	sel->y2 = v[2];
	printf("Selected %d %d %d %d\n", sel->y1, sel->x1, sel->y2, sel->x2);
}

// Rotesc selectia curenta cu unghiul dat
void rotate_command(char line[MAX_LENGTH], int *pos, image_t *img,
					selection_t *sel)
{
	int angle = 0;
	if (get_number(line, pos, &angle)) {
		printf("Invalid command\n");
		return;
	}
	// Dupa 4 rotatii, matricea se intoarce in starea initiala
	// De aceea, numarul de rotatii necesar este intotdeauna mai mic decat 4
	int rotations = (angle / 90) % 4;
	int square = ((sel->x2 - sel->x1) == (sel->y2 - sel->y1));
	int full = (sel->x1 == 0 && sel->x2 == img->n &&
				sel->y1 == 0 && sel->y2 == img->m);
	image_t *aux = (image_t *)malloc(sizeof(image_t));
	if (!aux) {
		exit_command(img, sel);
		printf("Memory allocation failed\n");
		exit(-1);
	}
	if (angle % 90 != 0) {
		printf("Unsupported rotation angle\n");
		free(aux);
		return;
	}
	if (!full && !square) {
		printf("The selection must be square\n");
		free(aux);
		return;
	}
	// O rotatie la stanga este echivalenta cu 3 la dreapta
	if (rotations < 0)
		rotations += 4;
	if (full) {
		for (int cnt = 0; cnt < rotations; cnt++) {
			if (copy_image(img, aux) != 0) {
				free(aux);
				exit_command(img, sel);
				printf("Memory allocation failed\n");
				exit(-1);
			}
			delete_matrix(img);
			img->m = aux->n;
			img->n = aux->m;
			img->type = aux->type;
			img->max_val = aux->max_val;
			if (create_image(img) != 0) {
				delete_image(aux);
				free(img);
				free(sel);
				printf("Memory allocation failed\n");
				exit(-1);
			}
			for (int i = 0; i < aux->n; i++) {
				for (int j = 0; j < aux->m; j++)
					img->mat[j][aux->n - i - 1] = aux->mat[i][j];
			}
			reset_selection_to_image(img, sel);
			delete_matrix(aux);
		}
	} else {
		for (int cnt = 0; cnt < rotations; cnt++) {
			if (copy_image(img, aux) != 0) {
				free(aux);
				exit_command(img, sel);
				printf("Memory allocation failed\n");
				exit(-1);
			}
			for (int i = sel->x1; i < sel->x2; i++) {
				for (int j = sel->y1; j < sel->y2; j++) {
					int i2 = j - sel->y1 + sel->x1;
					int j2 = sel->y2 + sel->x1 - i - 1;
					img->mat[i2][j2] = aux->mat[i][j];
				}
			}
			delete_matrix(aux);
		}
	}
	printf("Rotated %d\n", angle);
	free(aux);
}

// Fac egalizarea imaginii
void equalize_command(image_t *img, selection_t *sel)
{
	int *frecv = (int *)malloc((img->max_val + 1) * sizeof(int));
	if (!frecv) {
		exit_command(img, sel);
		printf("Memory allocation failed\n");
		exit(-1);
	}
	if (img->type == 3 || img->type == 6) {
		printf("Black and white image needed\n");
		free(frecv);
		return;
	}
	// Calculez frecventele
	for (int i = 0; i <= img->max_val; i++)
		frecv[i] = 0;
	for (int i = 0; i < img->n; i++) {
		for (int j = 0; j < img->m; j++)
			frecv[img->mat[i][j].v[0]]++;
	}
	// Calculez sumele partiale
	for (int i = 1; i <= img->max_val; i++)
		frecv[i] += frecv[i - 1];
	// Schimb fiecare pixel conform formulei
	for (int i = 0; i < img->n; i++) {
		for (int j = 0; j < img->m; j++) {
			int val = round((double)255 * frecv[img->mat[i][j].v[0]] /
							((double)img->n * img->m));
			img->mat[i][j].v[0] = clamp(val, 0, img->max_val);
		}
	}
	free(frecv);
	printf("Equalize done\n");
}

// Reduc imaginea la selectia curenta
void crop_command(image_t *img, selection_t *sel)
{
	image_t *aux = (image_t *)malloc(sizeof(image_t));
	if (!aux) {
		exit_command(img, sel);
		printf("Memory allocation failed\n");
		exit(-1);
	}
	// Copiez imaginea initiala in una auxiliara
	if (copy_image(img, aux) != 0) {
		free(aux);
		exit_command(img, sel);
		printf("Memory allocation failed\n");
		exit(-1);
	}
	delete_matrix(img);
	img->n = sel->x2 - sel->x1;
	img->m = sel->y2 - sel->y1;
	img->type = aux->type;
	img->max_val = aux->max_val;
	if (create_image(img) != 0) {
		delete_image(aux);
		free(img);
		free(sel);
		printf("Memory allocation failed\n");
		exit(-1);
	}
	// Pastrez in noua imagine doar selectia curenta si sterg auxiliara
	for (int i = 0; i < img->n; i++) {
		for (int j = 0; j < img->m; j++)
			img->mat[i][j] = aux->mat[i + sel->x1][j + sel->y1];
	}
	delete_image(aux);
	reset_selection_to_image(img, sel);
	printf("Image cropped\n");
}

// Aplic un nucleu de kernel selectiei curente
void apply_command(char line[MAX_LENGTH], int *pos, image_t *img,
				   selection_t *sel)
{
	char parameter[MAX_LENGTH];
	if (*pos == (int)strlen(line)) {
		printf("Invalid command\n");
		return;
	}
	int valid_parameter = 0, kernel[3][3], div = 1, mat[3][3];
	int x1 = sel->x1, y1 = sel->y1, x2 = sel->x2, y2 = sel->y2;
	image_t *aux = (image_t *)malloc(sizeof(image_t));
	if (!aux) {
		exit_command(img, sel);
		printf("Memory allocation failed\n");
		exit(-1);
	}
	get_word(line, parameter, pos);
	apply_initialize(parameter, &valid_parameter, kernel, &div);
	if (!valid_parameter) {
		printf("APPLY parameter invalid\n");
		free(aux);
		return;
	}
	if (img->type == 2 || img->type == 5) {
		printf("Easy, Charlie Chaplin\n");
		free(aux);
		return;
	}
	// Pixelii de pe marginea imaginii nu pot sa fie modificati
	if (x1 == 0)
		x1++;
	if (y1 == 0)
		y1++;
	if (x2 == img->n)
		x2--;
	if (y2 == img->m)
		y2--;
	if (copy_image(img, aux) != 0) {
		free(aux);
		exit_command(img, sel);
		printf("Memory allocation failed\n");
		exit(-1);
	}
	for (int i = x1; i < x2; i++) {
		for (int j = y1; j < y2; j++) {
			for (int c = 0; c < 3; c++) {
				// Extrag blocul de 3x3 din jurul pixelului curent
				for (int x = 0; x < 3; x++)
					for (int y = 0; y < 3; y++)
						mat[x][y] = aux->mat[i + x - 1][j + y - 1].v[c];

				int val = apply_matrix(mat, kernel, div);
				img->mat[i][j].v[c] = clamp(val, 0, img->max_val);
			}
		}
	}
	delete_image(aux);
	printf("APPLY %s done\n", parameter);
}

// Calculez histograma imaginii
void histogram_command(char line[MAX_LENGTH], int *pos, image_t *img,
					   selection_t *sel)
{
	int x = 0, y = 0, error = 0;
	// Verific validitatea parametrilor
	error |= get_number(line, pos, &x);
	error |= get_number(line, pos, &y);
	error |= ((*pos) != (int)strlen(line));
	if (error) {
		printf("Invalid command\n");
		return;
	}
	if ((y & (y - 1)) != 0 || y == 0) {
		printf("Invalid set of parameters\n");
		return;
	}
	if (img->type == 3 || img->type == 6) {
		printf("Black and white image needed\n");
		return;
	}
	// sz este latimea unui interval
	int sz = (img->max_val + 1) / y, max_frecv = 0;
	int *frecv = (int *)malloc(y * sizeof(int));
	if (!frecv) {
		exit_command(img, sel);
		printf("Memory allocation failed\n");
		exit(-1);
	}
	// Calculez vectorul de frecventa
	for (int i = 0; i < y; i++)
		frecv[i] = 0;
	for (int i = 0; i < img->n; i++) {
		for (int j = 0; j < img->m; j++) {
			int val = img->mat[i][j].v[0] / sz;
			frecv[val]++;
			if (frecv[val] > max_frecv)
				max_frecv = frecv[val];
		}
	}
	// Afisez histograma conform formulei date
	for (int i = 0; i < y; i++) {
		int val = (x * frecv[i] / max_frecv);
		printf("%d	|	", val);
		for (int j = 0; j < val; j++)
			printf("*");
		printf("\n");
	}
	free(frecv);
}

// Salvez imaginea aflata in memorie in fisierul dat
// Determin tipul imaginii salvate in functie de parametrul comenzii
void save_command(char line[MAX_LENGTH], int *pos, image_t *img)
{
	char filename[MAX_LENGTH], str[MAX_LENGTH];
	int ascii = 0;
	get_word(line, filename, pos);
	get_word(line, str, pos);
	ascii = (!strcmp(str, "ascii"));
	int type = img->type;
	if (ascii && type > 3)
		type -= 3;
	if (!ascii && type <= 3)
		type += 3;
	FILE *out = fopen(filename, "w");
	fprintf(out, "P%d\n%d %d\n%d\n", type, img->m, img->n, img->max_val);
	print_image(out, img, type);
	printf("Saved %s\n", filename);
	fclose(out);
}

// Dealoc memoria pentru a putea opri programul
void exit_command(image_t *img, selection_t *sel)
{
	delete_image(img);
	free(sel);
}

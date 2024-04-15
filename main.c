// Copyright Toma Ariciu 312CAb 2023-2024
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

int main(void)
{
	// Declar obiectele cu care lucreaza programul: imaginea si selectia
	image_t *img = (image_t *)malloc(sizeof(image_t));
	if (!img) {
		printf("Memory allocation failed\n");
		exit(-1);
	}
	selection_t *sel = (selection_t *)malloc(sizeof(selection_t));
	if (!sel) {
		printf("Memory allocation failed\n");
		free(img);
		exit(-1);
	}
	char command[MAX_LENGTH], line[MAX_LENGTH];
	int pos = 0;
	int loaded = 0;
	do {
		// Parsez linia si extrag comanda
		// pos reprezinta indexul primului caracter neparsat
		pos = 0;
		fgets(line, MAX_LENGTH, stdin);
		get_word(line, command, &pos);
		if (strcmp(command, "LOAD") == 0) {
			load_command(line, &pos, img, sel, &loaded);
			continue;
		}
		if (!loaded) {
			printf("No image loaded\n");
			if (strcmp(command, "EXIT") == 0) {
				free(img);
				free(sel);
			}
			continue;
		}
		if (strcmp(command, "SELECT") == 0) {
			select_command(line, &pos, img, sel);
			continue;
		}
		if (strcmp(command, "ROTATE") == 0) {
			rotate_command(line, &pos, img, sel);
			continue;
		}
		if (strcmp(command, "EQUALIZE") == 0) {
			equalize_command(img, sel);
			continue;
		}
		if (strcmp(command, "CROP") == 0) {
			crop_command(img, sel);
			continue;
		}
		if (strcmp(command, "APPLY") == 0) {
			apply_command(line, &pos, img, sel);
			continue;
		}
		if (strcmp(command, "HISTOGRAM") == 0) {
			histogram_command(line, &pos, img, sel);
			continue;
		}
		if (strcmp(command, "SAVE") == 0) {
			save_command(line, &pos, img);
			continue;
		}
		if (strcmp(command, "EXIT") == 0)
			exit_command(img, sel);
		else
			printf("Invalid command\n");
	} while (strcmp(command, "EXIT") != 0);
	return 0;
}

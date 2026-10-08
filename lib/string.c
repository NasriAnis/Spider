void memcpy(char *source, char *dest, int nbytes) {
	for (int i = 0; i < nbytes; i++) {
		*(dest + i) = *(source + i);
	}
}
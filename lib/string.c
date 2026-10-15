void memcpy(char *source, char *dest, int nbytes) {
	for (int i = 0; i < nbytes; i++) {
		*(dest + i) = *(source + i);
	}
}

int strlen(char s[]) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

void append(char s[], char n) {
    int len = strlen(s);
    s[len] = n;
    s[len+1] = '\0';
}
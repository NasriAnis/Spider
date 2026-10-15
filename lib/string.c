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

int tokenize(char *line, char **argv, int max_args) {
    int argc = 0;
    char *p = line;

    while (*p != '\0' && argc < max_args) {
        /* skip spaces before the next token */
        while (*p == ' ' || *p == '\t')
            p++;
        if (*p == '\0')
            break;

        argv[argc++] = p;

        while (*p != '\0' && *p != ' ' && *p != '\t')
            p++;

        if (*p != '\0')
            *p++ = '\0';
    }
    return argc;
}

int strcmp(const char *s1, const char *s2) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}
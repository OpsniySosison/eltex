#include "buffer.h"

static int ResizeLenStrBuff(BUFF* buff) {
    if (buff->size < 1 || buff->lenght < 1 || !buff->str) {
        fprintf(stderr, "Resize Buffer error, buffer broken\n");
        if (buff->str) {
            free(buff->str);
        }
        if (buff->sizes_of_prev_strs) {
            free(buff->sizes_of_prev_strs);
        }
        free(buff);
        exit(EXIT_FAILURE);
    }

    if (buff->size == 0) {
        buff->size++;
    }

    buff->size = buff->size << 1;
    // buff->str = (char*)realloc(buff->str, sizeof(char) * buff->size);

    char* temp = (char*)malloc(sizeof(char) * buff->size);

    if (!strcat(temp, buff->str)) {
        fprintf(stderr, "Resize Buffer error, strings not connection\n");
        if (buff->str) {
            free(buff->str);
        }
        if (buff->sizes_of_prev_strs) {
            free(buff->sizes_of_prev_strs);
        }
        free(buff);
        exit(EXIT_FAILURE);
    }

    free(buff->str);
    buff->str = NULL;
    buff->str = temp;
    
    if (!buff->str) {
        fprintf(stderr,"Resize buffer error, memory not allocation\n");
        free(buff->str);
        exit(EXIT_FAILURE);
    }

    return NORMAL;
}

static int ResizeNumStrBuff(BUFF* buff) {
    if (buff->num_of_str < 1 || !buff->sizes_of_prev_strs) {
        fprintf(stderr, "Resize Buffer error, buffer broken\n");
        if (buff->str) {
            free(buff->str);
        }
        if (buff->sizes_of_prev_strs) {
            free(buff->sizes_of_prev_strs);
        }
        free(buff);
        exit(EXIT_FAILURE);
    }

    buff->num_of_str++;

    buff->sizes_of_prev_strs = (unsigned int*)realloc(buff->sizes_of_prev_strs, sizeof(unsigned int) * buff->num_of_str);

    if (!buff->sizes_of_prev_strs) {
        fprintf(stderr,"Resize buffer error, memory not allocation\n");
        free(buff->str);
        exit(EXIT_FAILURE);
    }

    return NORMAL;
}

int BufferInit(FILE* file, BUFF* buff) {
    if (fseek(file, 0, SEEK_END)) {
        fprintf(stderr, "Fseek error\n"); 
        return BAD;
    }

    unsigned int size = ftell(file) / sizeof(char);
    
    buff->num_of_str = 1;
    buff->size = size;
    buff->lenght = size;
    buff->str = (char*)malloc(sizeof(char) * size);
    buff->sizes_of_prev_strs = (unsigned int*)malloc(sizeof(unsigned int) * buff->num_of_str);

    fseek(file, 0, SEEK_SET);
    char* temp = (char*)malloc(sizeof(char) * size);
    while (fgets(temp, size + 1, file)) {
        if (!strcat(buff->str, temp)) {
            fprintf(stderr, "Init Buffer error, strings not connection\n");
            if (buff->str) {
                free(buff->str);
            }
            if (buff->sizes_of_prev_strs) {
                free(buff->sizes_of_prev_strs);
            }
            free(buff);
            exit(EXIT_FAILURE);
        }
        buff->sizes_of_prev_strs[buff->num_of_str - 1] = strlen(temp);
        buff->num_of_str++;
        buff->sizes_of_prev_strs = (unsigned int*)realloc(buff->sizes_of_prev_strs, sizeof(unsigned int) * buff->num_of_str);
    }

    if (ferror(file)) {
        fprintf(stderr, "File is broken\n");
        exit(EXIT_FAILURE);
    }

    free(temp);

    return NORMAL;
}

int WriteToBuffer(CURS* cursor, BUFF* buff, char sym, unsigned int offset) {
    if (offset > buff->lenght || offset < 0) {
        fprintf(stderr, "Offset is too large of small\n");
        return BAD;
    }

    if (sym == 0x0a) {
        ResizeNumStrBuff(buff);
    }

    if (offset == buff->lenght) {
        buff->str[offset] = sym;
        if (buff->size - 1 < offset + 1) {
            ResizeLenStrBuff(buff);
        }
        buff->str[offset + 1] = '\0';
        buff->lenght++;
        if (sym == 0x0a) {       
            cursor->x = 0; 
            cursor->y++;
        }
        else { 
            cursor->x++;
        }
        return NORMAL;
    }

    buff->str[offset] = sym;
    if (sym == 0x0a) {       
        cursor->x = 0; 
        cursor->y++;
    }
    else { 
        cursor->x++;
    }
    return NORMAL;
}

int UploadToFile(BUFF* buff, FILE* file) {
    fseek(file, 0, SEEK_SET);
    fwrite((char*)buff->str, sizeof(char), buff->lenght - 1, file);
    return NORMAL;
}

int BufferDestructor(BUFF* buff) {
    free(buff->str);
    free(buff->sizes_of_prev_strs);
    free(buff);
    return NORMAL;
}

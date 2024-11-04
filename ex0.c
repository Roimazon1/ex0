#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 100

typedef struct {
    int _num_of_lines;
    char **_string;
} Text;

FILE *open_file();
void fill_struct(Text *text, FILE *file);
void check_allocation(void *ptr);
void copy_rows(Text *text, FILE *file);
void free_matrix(Text *text);
void quick_sort(Text *text, int low, int high);
int partition(Text *text, int low, int high);
int get_first_number(const char *line);
void print(Text *text);

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Error\n");
        return 1;
    }
    Text *text = malloc(sizeof(Text));
    
    FILE *file = open_file();

    fill_struct(text, file);
    quick_sort(text, 0, text->_num_of_lines - 1);

    print(text);

    free_matrix(text);
    fclose(file);
    return 0;
}

void fill_struct(Text *text, FILE *file) {
    if (fscanf(file, "%d\n", &text->_num_of_lines) != 1) {  
        printf("Error reading number\n");
        text->_num_of_lines = 0;
        return;
    }

    text->_string = (char**)malloc(text->_num_of_lines * sizeof(char *)); 
    check_allocation(text->_string);
    copy_rows(text, file);    
}

void copy_rows(Text *text, FILE *file) {
    char line[MAX_STR_LEN];
    //fgetc(file);  // דילוג על תו חדש, אם קיים

    for (int i = 0; i < text->_num_of_lines; i++) {
        
        if (fgets(line, MAX_STR_LEN, file) != NULL) {
            text->_string[i] = (char*)malloc((strlen(line) + 1) * sizeof(char));
            check_allocation(text->_string[i]);
            size_t length=strlen(line);
            if (length > 0 && line[length - 1] != '\n') 
            {
                line[length] = '\n';
                line[length + 1] = '\0';
            }
            strcpy(text->_string[i], line);
        
        } else {
            break;  // יציאה אם לא הצלחנו לקרוא שורה
        }
    }
}

void quick_sort(Text *text, int low, int high) {
    if (low < high) {
        int pi = partition(text, low, high);
        quick_sort(text, low, pi - 1);  
        quick_sort(text, pi + 1, high);
    }
}

int partition(Text *text, int low, int high) {
    int pivot = get_first_number(text->_string[high]);
    int i = (low - 1); 

    for (int j = low; j < high; j++) {
        if (get_first_number(text->_string[j]) <= pivot) {
            i++;
            char *temp = text->_string[i];
            text->_string[i] = text->_string[j];
            text->_string[j] = temp;
        }
    }

    char *temp = text->_string[i + 1];
    text->_string[i + 1] = text->_string[high];
    text->_string[high] = temp;

    return i + 1;
}

int get_first_number(const char *line) {
    int num;
    sscanf(line, "%d", &num);
    return num;
}

void check_allocation(void *ptr) {
    if (ptr == NULL) {
        printf("Cannot allocate\n");
        exit(1);
    }
}

void free_matrix(Text *text) {
    for (int i = 0; i < text->_num_of_lines; i++) {
        free(text->_string[i]);
    }
    free(text->_string);
    free(text);    
}

FILE *open_file() {
    FILE *fp = fopen("ex0t.txt", "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    return fp;
}

void print(Text *text)
{
    printf("%d\n", text->_num_of_lines);
    for (int i = 0; i < text->_num_of_lines; i++) 
    {
        printf("%s", text->_string[i]);
    }
}
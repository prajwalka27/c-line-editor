#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 500
#define MAX_LENGTH 256

char doc[MAX_LINES][MAX_LENGTH];
int total_lines = 0;

void print_doc() {
    if (total_lines == 0) {
        printf("[Document is empty]\n");
        return;
    }
    for (int i = 0; i < total_lines; i++) {
        printf("%d: %s\n", i + 1, doc[i]);
    }
}

void insert_line(int line_num, const char *text) {
    if (line_num < 1 || line_num > total_lines + 1 || total_lines >= MAX_LINES) {
        printf("Error: Invalid line number or document full.\n");
        return;
    }
    int idx = line_num - 1;
    for (int i = total_lines; i > idx; i--) {
        strcpy(doc[i], doc[i - 1]);
    }
    strncpy(doc[idx], text, MAX_LENGTH - 1);
    doc[idx][MAX_LENGTH - 1] = '\0';
    total_lines++;
    printf("Line %d inserted.\n", line_num);
}

void delete_line(int line_num) {
    if (line_num < 1 || line_num > total_lines) {
        printf("Error: Line %d does not exist.\n", line_num);
        return;
    }
    int idx = line_num - 1;
    for (int i = idx; i < total_lines - 1; i++) {
        strcpy(doc[i], doc[i + 1]);
    }
    total_lines--;
    printf("Line %d deleted.\n", line_num);
}

int main() {
    char command[16];
    printf("Line Editor Ready. Commands: I, D, P, Q\n");

    while (1) {
        printf("> ");
        if (scanf("%15s", command) != 1) break;

        if (strcmp(command, "P") == 0 || strcmp(command, "p") == 0) {
            print_doc();
        } else if (strcmp(command, "I") == 0 || strcmp(command, "i") == 0) {
            int line_num;
            char buffer[MAX_LENGTH];
            scanf("%d", &line_num);
            getchar(); // Consume leading whitespace/newline
            if (fgets(buffer, sizeof(buffer), stdin)) {
                buffer[strcspn(buffer, "\r\n")] = '\0'; // Strip newline
                insert_line(line_num, buffer);
            }
        } else if (strcmp(command, "D") == 0 || strcmp(command, "d") == 0) {
            int line_num;
            scanf("%d", &line_num);
            delete_line(line_num);
        } else if (strcmp(command, "Q") == 0 || strcmp(command, "q") == 0) {
            printf("Exiting editor.\n");
            break;
        } else {
            printf("Unknown command. Type P, I, D, or Q.\n");
            while (getchar() != '\n'); // Flush input line
        }
    }
    return 0;
}
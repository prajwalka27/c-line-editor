#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 500
#define MAX_LENGTH 256

char doc[MAX_LINES][MAX_LENGTH];
int total_lines = 0;

// --- UNDO SYSTEM GLOBALS ---
char backup_doc[MAX_LINES][MAX_LENGTH];
int backup_lines = 0;

// Saves the state before any modification
void save_state() {
    backup_lines = total_lines;
    for (int i = 0; i < total_lines; i++) {
        strcpy(backup_doc[i], doc[i]);
    }
}

// Reverts to the last saved state
void undo_action() {
    total_lines = backup_lines;
    for (int i = 0; i < total_lines; i++) {
        strcpy(doc[i], backup_doc[i]);
    }
    printf("Undo successful. Restored previous state.\n");
}
// ---------------------------

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
    save_state(); // Save before modifying
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
    save_state(); // Save before modifying
    int idx = line_num - 1;
    for (int i = idx; i < total_lines - 1; i++) {
        strcpy(doc[i], doc[i + 1]);
    }
    total_lines--;
    printf("Line %d deleted.\n", line_num);
}

// --- NEW BONUS FEATURES ---

void save_file(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: Could not open file for writing.\n");
        return;
    }
    for (int i = 0; i < total_lines; i++) {
        fprintf(fp, "%s\n", doc[i]);
    }
    fclose(fp);
    printf("Document saved to %s.\n", filename);
}

void load_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Could not open file %s.\n", filename);
        return;
    }
    save_state(); // Save in case they want to undo the load
    total_lines = 0;
    char buffer[MAX_LENGTH];
    while (fgets(buffer, sizeof(buffer), fp) && total_lines < MAX_LINES) {
        buffer[strcspn(buffer, "\r\n")] = '\0'; // Strip newline
        strcpy(doc[total_lines++], buffer);
    }
    fclose(fp);
    printf("Document loaded from %s.\n", filename);
}

void search_doc(const char *query) {
    int found = 0;
    for (int i = 0; i < total_lines; i++) {
        if (strstr(doc[i], query) != NULL) {
            printf("Found on Line %d: %s\n", i + 1, doc[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("Search query '%s' not found in the document.\n", query);
    }
}

void replace_in_doc(const char *old_word, const char *new_word) {
    save_state(); // Save before modifying
    int count = 0;
    for (int i = 0; i < total_lines; i++) {
        char *pos = strstr(doc[i], old_word);
        if (pos != NULL) { // Replaces the FIRST occurrence on the line
            char buffer[MAX_LENGTH];
            strncpy(buffer, doc[i], pos - doc[i]);
            buffer[pos - doc[i]] = '\0';
            strcat(buffer, new_word);
            strcat(buffer, pos + strlen(old_word));
            strcpy(doc[i], buffer);
            count++;
        }
    }
    printf("Replaced %d occurrence(s) of '%s' with '%s'.\n", count, old_word, new_word);
}

// --------------------------

int main() {
    char command[16];
    printf("Line Editor Ready. Commands: I, D, P, S, L, F, R, U, Q\n");

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
                buffer[strcspn(buffer, "\r\n")] = '\0';
                insert_line(line_num, buffer);
            }
        } else if (strcmp(command, "D") == 0 || strcmp(command, "d") == 0) {
            int line_num;
            scanf("%d", &line_num);
            delete_line(line_num);
        } else if (strcmp(command, "S") == 0 || strcmp(command, "s") == 0) {
            char filename[MAX_LENGTH];
            scanf("%s", filename);
            save_file(filename);
        } else if (strcmp(command, "L") == 0 || strcmp(command, "l") == 0) {
            char filename[MAX_LENGTH];
            scanf("%s", filename);
            load_file(filename);
        } else if (strcmp(command, "F") == 0 || strcmp(command, "f") == 0) {
            char query[MAX_LENGTH];
            getchar(); // Consume leading whitespace
            if (fgets(query, sizeof(query), stdin)) {
                query[strcspn(query, "\r\n")] = '\0';
                search_doc(query);
            }
        } else if (strcmp(command, "R") == 0 || strcmp(command, "r") == 0) {
            char old_word[64], new_word[64];
            scanf("%63s %63s", old_word, new_word);
            replace_in_doc(old_word, new_word);
        } else if (strcmp(command, "U") == 0 || strcmp(command, "u") == 0) {
            undo_action();
        } else if (strcmp(command, "Q") == 0 || strcmp(command, "q") == 0) {
            printf("Exiting editor.\n");
            break;
        } else {
            printf("Unknown command. Type P, I, D, S, L, F, R, U, or Q.\n");
            while (getchar() != '\n'); // Flush input line
        }
    }
    return 0;
}
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 5

void create_key_table(const char *key, char table[TABLE_SIZE][TABLE_SIZE]) {
    int used[26] = {0};  // Track letters used in table, excluding 'Q'
    int index = 0;

    // Add key characters to table
    for (int i = 0; key[i] != '\0'; i++) {
        char ch = tolower(key[i]);
        if (ch == 'q' || ch == ' ') continue;  // Skip 'q' and spaces
        if (!used[ch - 'a']) {
            table[index / TABLE_SIZE][index % TABLE_SIZE] = ch;
            used[ch - 'a'] = 1;
            index++;
        }
    }

    // Add remaining alphabet characters to table
    for (char ch = 'a'; ch <= 'z'; ch++) {
        if (ch == 'q' || used[ch - 'a']) continue;  // Skip 'q' and used letters
        table[index / TABLE_SIZE][index % TABLE_SIZE] = ch;
        index++;
    }
}

void preprocess_plaintext(const char *text, char *processed_text) {
    int len = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == ' ') continue;  // Ignore spaces
        processed_text[len++] = tolower(text[i]);
    }
    processed_text[len] = '\0';

    // Insert 'x' if there are duplicate letters in digraphs
    char temp[2000] = {0};
    int j = 0;
    for (int i = 0; i < len; i += 2) {
        temp[j++] = processed_text[i];
        if (i + 1 < len && processed_text[i] == processed_text[i + 1]) {
            temp[j++] = 'x';  // Insert 'x' if letters are the same
            temp[j++] = processed_text[i + 1];
        } else if (i + 1 < len) {
            temp[j++] = processed_text[i + 1];
        } else {
            temp[j++] = 'x';  // Pad with 'x' if only one letter remains
        }
    }
    temp[j] = '\0';
    strcpy(processed_text, temp);
}

void find_position(char ch, char table[TABLE_SIZE][TABLE_SIZE], int *row, int *col) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        for (int j = 0; j < TABLE_SIZE; j++) {
            if (table[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

void encrypt_digraph(char a, char b, char table[TABLE_SIZE][TABLE_SIZE], char *encrypted_pair) {
    int row_a, col_a, row_b, col_b;
    find_position(a, table, &row_a, &col_a);
    find_position(b, table, &row_b, &col_b);

    if (row_a == row_b) {
        // Same row
        encrypted_pair[0] = table[row_a][(col_a + 1) % TABLE_SIZE];
        encrypted_pair[1] = table[row_b][(col_b + 1) % TABLE_SIZE];
    } else if (col_a == col_b) {
        // Same column
        encrypted_pair[0] = table[(row_a + 1) % TABLE_SIZE][col_a];
        encrypted_pair[1] = table[(row_b + 1) % TABLE_SIZE][col_b];
    } else {
        // Rectangle rule
        encrypted_pair[0] = table[row_a][col_b];
        encrypted_pair[1] = table[row_b][col_a];
    }
}

void playfair_encrypt(const char *key, const char *plaintext, char *encrypted_text) {
    char table[TABLE_SIZE][TABLE_SIZE];
    create_key_table(key, table);

    char processed_text[2000];
    preprocess_plaintext(plaintext, processed_text);

    int len = strlen(processed_text);
    char encrypted_pair[2];
    int k = 0;

    // Encrypt each digraph
    for (int i = 0; i < len; i += 2) {
        encrypt_digraph(processed_text[i], processed_text[i + 1], table, encrypted_pair);
        encrypted_text[k++] = toupper(encrypted_pair[0]);
        encrypted_text[k++] = toupper(encrypted_pair[1]);
    }
    encrypted_text[k] = '\0';
}

int main() {
    char key[1001], plaintext[1001];
    char encrypted_text[2000];

    // Read input key and plaintext
    printf("input: ");
    fgets(key, sizeof(key), stdin);
    fgets(plaintext, sizeof(plaintext), stdin);

    // Remove newline characters from fgets input
    key[strcspn(key, "\n")] = '\0';
    plaintext[strcspn(plaintext, "\n")] = '\0';

    playfair_encrypt(key, plaintext, encrypted_text);
    printf("%s\n", encrypted_text);

    return 0;
}

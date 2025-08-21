#include <stdio.h>
#include <string.h>
#include <ctype.h>

// ===== Structures =====
struct Token {
    char type[20];
    char value[100];
};

struct Symbol {
    char name[100];
    char type[20];
    char scope[20];
};

// ===== Rule: SSSD (3 letters + 1 digit, length = 4) =====
int isValidVariableName(const char *var) {
    if (strlen(var) != 4) return 0; // Must be exactly 4 characters
    for (int i = 0; i < 3; i++) {
        if (!isalpha((unsigned char)var[i])) return 0; // First 3 must be letters
    }
    if (!isdigit((unsigned char)var[3])) return 0; // Last must be digit
    return 1; // Valid
}

// ===== Tokenization & Symbol Table Builder =====
void tokenize_and_build_symbol_table(const char *input, struct Token tokens[], int *token_count,
                                     struct Symbol symbol_table[], int *symbol_count) {
    int i = 0, token_idx = 0, symbol_idx = 0;
    char word[100];
    int word_idx;

    while (input[i]) {
        // Skip whitespace
        if (isspace((unsigned char)input[i])) {
            i++;
            continue;
        }

        // Handle symbols
        if (strchr("{};(),[]", input[i])) {
            word_idx = 0;
            word[word_idx++] = input[i++];
            word[word_idx] = '\0';
            strcpy(tokens[token_idx].type, "SYMBOL");
            strcpy(tokens[token_idx].value, word);
            token_idx++;
            continue;
        }

        // Handle strings (including single-quoted characters)
        if (input[i] == '"' || input[i] == '\'') {
            char quote = input[i];
            word_idx = 0;
            word[word_idx++] = input[i++];
            while (input[i] && input[i] != quote) {
                word[word_idx++] = input[i++];
            }
            if (input[i] == quote) {
                word[word_idx++] = input[i++];
            }
            word[word_idx] = '\0';
            strcpy(tokens[token_idx].type, "STRING");
            strcpy(tokens[token_idx].value, word);
            token_idx++;
            continue;
        }

        // Handle keywords & identifiers
        if (isalpha((unsigned char)input[i])) {
            word_idx = 0;
            while (isalnum((unsigned char)input[i]) || input[i] == '_') {
                word[word_idx++] = input[i++];
            }
            word[word_idx] = '\0';

            if (!strcmp(word, "int") || !strcmp(word, "float") ||
                !strcmp(word, "char") || !strcmp(word, "double")) {
                strcpy(tokens[token_idx].type, "KEYWORD");
            } else {
                strcpy(tokens[token_idx].type, "IDENTIFIER");
            }
            strcpy(tokens[token_idx].value, word);
            token_idx++;
            continue;
        }

        // Handle numbers
        if (isdigit((unsigned char)input[i])) {
            word_idx = 0;
            while (isdigit((unsigned char)input[i])) {
                word[word_idx++] = input[i++];
            }
            word[word_idx] = '\0';
            strcpy(tokens[token_idx].type, "NUMBER");
            strcpy(tokens[token_idx].value, word);
            token_idx++;
            continue;
        }

        i++; // Skip unrecognized characters
    }

    // Build symbol table from tokens
    for (int j = 0; j < token_idx; j++) {
        if (strcmp(tokens[j].type, "KEYWORD") == 0 &&
            (strcmp(tokens[j].value, "int") == 0 ||
             strcmp(tokens[j].value, "float") == 0 ||
             strcmp(tokens[j].value, "char") == 0 ||
             strcmp(tokens[j].value, "double") == 0)) {

            if (j + 1 < token_idx && strcmp(tokens[j + 1].type, "IDENTIFIER") == 0 &&
                isValidVariableName(tokens[j + 1].value)) { // Apply SSSD rule

                int k = j + 1;
                // Handle arrays
                if (k + 1 < token_idx && strcmp(tokens[k + 1].type, "SYMBOL") == 0 &&
                    strcmp(tokens[k + 1].value, "[") == 0) {
                    strcpy(symbol_table[symbol_idx].name, tokens[k].value);
                    sprintf(symbol_table[symbol_idx].type, "%s[]", tokens[j].value);
                    strcpy(symbol_table[symbol_idx].scope, "global");
                    symbol_idx++;
                } else {
                    strcpy(symbol_table[symbol_idx].name, tokens[k].value);
                    strcpy(symbol_table[symbol_idx].type, tokens[j].value);
                    strcpy(symbol_table[symbol_idx].scope, "global");
                    symbol_idx++;
                }
            }
        }
    }

    *token_count = token_idx;
    *symbol_count = symbol_idx;
}

// ===== Main Function =====
int main() {
    // Open input file
    FILE *input = fopen("input.txt", "r");
    if (!input) {
        printf("Error: Cannot open input.txt\n");
        return 1;
    }

    // Read input file into buffer
    char buffer[10000];
    int buffer_idx = 0;
    char c;
    while ((c = fgetc(input)) != EOF) {
        buffer[buffer_idx++] = c;
    }
    buffer[buffer_idx] = '\0';
    fclose(input);

    // Tokenize and build symbol table
    struct Token tokens[1000];
    struct Symbol symbol_table[100];
    int token_count = 0, symbol_count = 0;
    tokenize_and_build_symbol_table(buffer, tokens, &token_count, symbol_table, &symbol_count);

    // Write to output file
    FILE *output = fopen("output.txt", "w");
    if (!output) {
        printf("Error: Cannot open output.txt\n");
        return 1;
    }

    fprintf(output, "=== Tokens ===\n");
    for (int i = 0; i < token_count; i++) {
        fprintf(output, "%s: %s\n", tokens[i].type, tokens[i].value);
    }

    fprintf(output, "\n=== Symbol Table ===\n");
    for (int i = 0; i < symbol_count; i++) {
        fprintf(output, "Name: %s, Type: %s, Scope: %s\n",
                symbol_table[i].name, symbol_table[i].type, symbol_table[i].scope);
    }

    fclose(output);
    printf("Output written to output.txt\n");
    return 0;
}

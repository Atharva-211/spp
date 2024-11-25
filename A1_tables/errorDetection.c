#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SYMBOLS 100
#define MAX_LITERALS 100
#define MAX_POOLS 10
#define MAX_CODE_LINES 100
#define MAX_MNEMONICS 10

// Data Structures
typedef struct {
    char symbol[10];
    int address;
} Symbol;

typedef struct {
    char literal[10];
    int address;
} Literal;

typedef struct {
    int start_idx;
} Pool;

typedef struct {
    char mnemonic[10];
    int code;
} Mnemonic;

// Global Tables
Symbol symtab[MAX_SYMBOLS];
int symtab_count = 0;

Literal littab[MAX_LITERALS];
int littab_count = 0;

Pool pooltab[MAX_POOLS];
int pooltab_count = 1;

Mnemonic mnemonics[MAX_MNEMONICS] = {
    {"MOV", 1}, {"ADD", 2}, {"SUB", 3}, {"MUL", 4}, {"DIV", 5},
    {"DC", 6}, {"DS", 7}, {"START", 8}, {"END", 9}, {"LTORG", 10}
};

char intermediate_code[MAX_CODE_LINES][50];
int intermediate_count = 0;

int location_counter = 0;

char errors[MAX_CODE_LINES][100];
int error_count = 0;

// Helper function to check if a mnemonic is valid
int is_valid_mnemonic(const char *mnemonic) {
    for (int i = 0; i < MAX_MNEMONICS; i++) {
        if (strcmp(mnemonics[i].mnemonic, mnemonic) == 0) {
            return 1; // Valid mnemonic
        }
    }
    return 0; // Invalid mnemonic
}

// Function to add a symbol to the symbol table
void add_symbol(char *symbol, int address) {
    strcpy(symtab[symtab_count].symbol, symbol);
    symtab[symtab_count].address = address;
    symtab_count++;
}

// Function to add a literal to the literal table
void add_literal(char *literal) {
    strcpy(littab[littab_count].literal, literal);
    littab[littab_count].address = -1; // Address will be assigned during LTORG or END
    littab_count++;
}

// Function to handle symbol table logic
void process_symbol(char *symbol, int address) {
    for (int i = 0; i < symtab_count; i++) {
        if (strcmp(symtab[i].symbol, symbol) == 0) {
            return; // Symbol already exists, no need to add
        }
    }
    strcpy(symtab[symtab_count].symbol, symbol);
    symtab[symtab_count].address = address;
    symtab_count++;
}

// Modified process_line function to handle symbols
void process_line(char *line, int line_number) {
    char token1[10], token2[10], token3[10];
    sscanf(line, "%s %s %s", token1, token2, token3);

    if (is_valid_mnemonic(token1)) {
        if (strcmp(token1, "START") == 0) {
            location_counter = atoi(token2);
        } else if (strcmp(token1, "END") == 0 || strcmp(token1, "LTORG") == 0) {
            for (int i = pooltab[pooltab_count - 1].start_idx; i < littab_count; i++) {
                if (littab[i].address == -1) {
                    littab[i].address = location_counter;
                    sprintf(intermediate_code[intermediate_count++], "%d (LIT, %s)", location_counter, littab[i].literal);
                    location_counter++;
                }
            }
            if (strcmp(token1, "LTORG") == 0) {
                pooltab[pooltab_count++].start_idx = littab_count;
            }
        } else if (strcmp(token2, "DC") == 0 || strcmp(token2, "DS") == 0) {
            process_symbol(token1, location_counter); // Add symbol to symbol table
            sprintf(intermediate_code[intermediate_count++], "%d (%s, %s)", location_counter, token2, token3);
            location_counter += (strcmp(token2, "DS") == 0) ? atoi(token3) : 1;
        } else if (strcmp(token1, "MOV") == 0) {
            if (token3[0] == '=') {
                add_literal(token3);
            }
            sprintf(intermediate_code[intermediate_count++], "%d (%s, %s, %s)", location_counter, token1, token2, token3);
            location_counter++;
        }
    } else if (!is_valid_mnemonic(token1) && strlen(token2) > 0) {
        process_symbol(token1, location_counter); // Handle symbols defined without DC/DS
    } else {
        sprintf(errors[error_count++], "Line %d: Invalid instruction: %s", line_number, token1);
    }
}


// Function to display tables and intermediate code
void display(FILE *output) {
    fprintf(output, "\nSymbol Table:\n");
    for (int i = 0; i < symtab_count; i++) {
        fprintf(output, "%s\t%d\n", symtab[i].symbol, symtab[i].address);
    }

    fprintf(output, "\nLiteral Table:\n");
    for (int i = 0; i < littab_count; i++) {
        fprintf(output, "%s\t%d\n", littab[i].literal, littab[i].address);
    }

    fprintf(output, "\nPool Table:\n");
    for (int i = 0; i < pooltab_count; i++) {
        fprintf(output, "%d\n", pooltab[i].start_idx);
    }

    fprintf(output, "\nMnemonic Table:\n");
    for (int i = 0; i < MAX_MNEMONICS; i++) {
        fprintf(output, "%s\t%d\n", mnemonics[i].mnemonic, mnemonics[i].code);
    }

    fprintf(output, "\nIntermediate Code:\n");
    for (int i = 0; i < intermediate_count; i++) {
        fprintf(output, "%s\n", intermediate_code[i]);
    }

    fprintf(output, "\nErrors:\n");
    for (int i = 0; i < error_count; i++) {
        fprintf(output, "%s\n", errors[i]);
    }
}

int main() {
    // Open the input file
    FILE *input = fopen("D:\\SEM5\\FINAL Z\\FINAL Z\\2\\02\\02input.asm", "r");
    if (input == NULL) {
        printf("Error: Unable to open input.asm\n");
        return 1;
    }

    // Open the output file
    FILE *output = fopen("D:\\SEM5\\FINAL Z\\FINAL Z\\2\\02\\output.txt", "w");
    if (output == NULL) {
        printf("Error: Unable to create output.txt\n");
        fclose(input);
        return 1;
    }

    // Process each line in the input file
    char line[50];
    int line_number = 1;
    while (fgets(line, sizeof(line), input)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        process_line(line, line_number++);
    }

    // Display the tables and intermediate code
    display(output);

    // Close files
    fclose(input);
    fclose(output);

    return 0;
}

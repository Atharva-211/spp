// Input is intermediate code machine code is generated from the input intermediate code

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

// Mnemonic Opcode Table (Using struct for mapping mnemonic to opcode)
struct MOTEntry {
    char mnemonic[MAX];
    int opcode;
};

struct MOTEntry mot[] = {
    {"LDA", 0x00}, {"ADD", 0x01}, {"STA", 0x02}, {"HLT", 0xFF},
    {"DC", -1}, {"DS", -2}  // DC and DS are directives, not instructions
};
int motSize = sizeof(mot) / sizeof(mot[0]);

// Symbol Table
struct SymbolEntry {
    char symbol[MAX];
    int address;
};

struct SymbolEntry symbolTable[] = {
    {"VALUE", 1005}, {"SUM", 1008}, {"RESULT", 1011}  // Example addresses
};
int symbolTableSize = sizeof(symbolTable) / sizeof(symbolTable[0]);

// Intermediate Code
char* intermediateCode[] = {
    "[1005] DC 05",
    "[1008] DC 10",
    "[1011] LDA VALUE",
    "[1014] ADD SUM",
    "[1017] STA RESULT",
    "[1020] HLT"
};
int intermediateCodeSize = sizeof(intermediateCode) / sizeof(intermediateCode[0]);

// Function to get opcode from MOT
int getOpcode(char* mnemonic) {
    for (int i = 0; i < motSize; i++) {
        if (strcmp(mot[i].mnemonic, mnemonic) == 0) {
            return mot[i].opcode;
        }
    }
    return -1;  // Return -1 if mnemonic not found
}

// Function to get symbol address from Symbol Table
int getAddress(char* symbol) {
    for (int i = 0; i < symbolTableSize; i++) {
        if (strcmp(symbolTable[i].symbol, symbol) == 0) {
            return symbolTable[i].address;
        }
    }
    return -1;  // Return -1 if symbol not found
}

void pass2() {
    char machineCode[MAX][MAX];
    int machineCodeIndex = 0;

    for (int i = 0; i < intermediateCodeSize; i++) {
        char address[MAX], mnemonic[MAX], operand[MAX];
        sscanf(intermediateCode[i], "%*s %s %s", mnemonic, operand);

        if (strcmp(mnemonic, "DC") == 0 || strcmp(mnemonic, "DS") == 0 || mnemonic[0] == '\0') {
            // Handle Directives or empty lines (i.e., no machine code)
            strcpy(machineCode[machineCodeIndex++], "No machine code");
            continue;
        }

        // Retrieve the opcode from MOT
        int opcode = getOpcode(mnemonic);

        // Resolve the address of the operand
        int operandAddress = getAddress(operand);

        // Generate machine code line with two columns: opcode and address
        sprintf(machineCode[machineCodeIndex], "%04X %04X", opcode, operandAddress);
        machineCodeIndex++;
    }

    // Output the Machine Code
    printf("Machine Code (from Pass 2):\n");
    for (int i = 0; i < machineCodeIndex; i++) {
        if (strcmp(machineCode[i], "No machine code") == 0) {
            printf("%s\n", machineCode[i]);
        } else {
            printf("Opcode: %.4s Address: %.4s\n", machineCode[i], machineCode[i] + 5);
        }
    }
}

int main() {
    // Output the Intermediate Code
    printf("Intermediate Code (from Pass 1):\n");
    for (int i = 0; i < intermediateCodeSize; i++) {
        printf("%s\n", intermediateCode[i]);
    }
    printf("\n");

    // Run Pass 2 to generate Machine Code
    pass2();

    return 0;
}
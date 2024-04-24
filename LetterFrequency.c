#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef struct {
    char character;
    int frequency;
} CharFreq;


void combineFiles(const char *destination, const char *sourceDir) {
    char searchPath[256];
    snprintf(searchPath, sizeof(searchPath), "%s\\*.txt", sourceDir);

    WIN32_FIND_DATA fd;
    HANDLE hFind = FindFirstFile(searchPath, &fd);
    if (hFind == INVALID_HANDLE_VALUE) {
        perror("Failed to read directory");
        return;
    }

    FILE *destFile = fopen(destination, "w");
    if (destFile == NULL) {
        perror("Failed to open destination file");
        return;
    }

    do {
        char filePath[256];
        snprintf(filePath, sizeof(filePath), "%s\\%s", sourceDir, fd.cFileName);

        FILE *srcFile = fopen(filePath, "r");
        if (srcFile == NULL) {
            perror("Failed to open source file");
            continue;
        }

        int ch;
        while ((ch = fgetc(srcFile)) != EOF) {
            fputc(ch, destFile);
        }
        fclose(srcFile);
    } while (FindNextFile(hFind, &fd) != 0);

    fclose(destFile);
    FindClose(hFind);
}

void combineFilesForBooks(const char *destination, char *sources[], int num_sources) {
    FILE *destFile = fopen(destination, "w");
    if (destFile == NULL) {
        perror("Failed to open destination file");
        return;
    }

    for (int i = 0; i < num_sources; i++) {
        FILE *srcFile = fopen(sources[i], "r");
        if (srcFile == NULL) {
            perror("Failed to open source file");
            continue;
        }

        int ch;
        while ((ch = fgetc(srcFile)) != EOF) {
            fputc(ch, destFile);
        }
        fclose(srcFile);
    }

    fclose(destFile);
}

void countCharacterFrequencies(const char *filename, CharFreq frequencies[95]){
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    for (int i = 0; i < 95; i++) {
        frequencies[i].character = i + 32;
        frequencies[i].frequency = 0;
    }

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch >= 32 && ch <= 126) {
            frequencies[ch - 32].frequency++;
        }
    }

    fclose(file);
}

int compareFrequencies(const void *a, const void *b) {
    const CharFreq *cf1 = (const CharFreq *)a;
    const CharFreq *cf2 = (const CharFreq *)b;
    return cf2->frequency - cf1->frequency;
}

void sortFrequencies(CharFreq frequencies[], int size) {
    qsort(frequencies, size, sizeof(CharFreq), compareFrequencies);
}

char decryptCharacter(char c, int key) {
    int shifted = c - key;
    if (shifted < 32) {
        shifted += 94;
    }
    return (char)shifted;
}

int main() {
    printf("Setting up Frequency Attack now:\n");

    char *ciphertextFiles[] = {"C:\\Users\\Owen_\\CLionProjects\\C291-Spring-24\\Cryptography\\Data\\Data\\lesMis-FULL.txt", "C:\\Users\\Owen_\\CLionProjects\\C291-Spring-24\\Cryptography\\Data\\Data\\warAndPeace-FULL.txt", "C:\\Users\\Owen_\\CLionProjects\\C291-Spring-24\\Cryptography\\Data\\Data\\crime_and_punishment_FULL.txt"};
    int num_ciphertextFiles = sizeof(ciphertextFiles) / sizeof(ciphertextFiles[0]);
    combineFilesForBooks("C1.txt", ciphertextFiles, num_ciphertextFiles);

    combineFiles("P1.txt", "C:\\Users\\Owen_\\CLionProjects\\C291-Spring-24\\Cryptography\\Data\\Data\\Collection1");

    CharFreq c1Frequencies[95] = {0};
    countCharacterFrequencies("C1.txt", c1Frequencies);
    sortFrequencies(c1Frequencies, 95);

    CharFreq p1Frequencies[95] = {0};
    countCharacterFrequencies("P1.txt", p1Frequencies);
    sortFrequencies(p1Frequencies, 95);

    //c1:  : 1221940
    //e: 745253
    //t: 521590
    //a: 471163
    //o: 444069
    //n: 409914
    //i: 388374
    //h: 387345
    //s: 369296
    //r: 337209

    //p1: Y: 516966
    //@: 313017
    //O: 219635
    //<: 199253
    //J: 191277
    //I: 180576
    //D: 166326
    //C: 163047
    //N: 159905
    //M: 145389

    char ci = c1Frequencies[0].character;  // Most frequent character in C1
    char pi = p1Frequencies[0].character;  // Standard frequent character in plaintext (E works here, but how?) -37 is the key
    int key = pi-ci;                     // Estimate the key based on the most frequent character in C1

    printf("Estimated key: %d\n", key);

    // Decrypt and print
    FILE *file = fopen("P1.txt", "r");
    if (file == NULL) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch >= 32 && ch <= 126) {
            char decryptedChar = decryptCharacter(ch, key);
            putchar(decryptedChar);
        } else {
            putchar(ch);
        }
    }

    fclose(file);
    printf("\nDecryption complete.\n");

    return EXIT_SUCCESS;
}

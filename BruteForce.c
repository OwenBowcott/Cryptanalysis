#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


#define BUFFER_SIZE 1024  // Define a buffer size for reading files
int correctKey = -1; // Global variable to hold the correct key

void decryptFile(const char* filePath);
void decryptRest(const char* filePath);

int main(void) {
    DIR *dir;
    struct dirent *ent;
    char directoryPath[256];

    printf("Enter the directory path containing text files:\n");
    scanf("%255s", directoryPath); // Read directory path from user
    getchar(); // Consume the newline left by scanf

    printf("Attempting to open directory: '%s'\n", directoryPath);
    dir = opendir(directoryPath);
    if (dir == NULL) {
        perror("Unable to read directory");
        return EXIT_FAILURE;
    }

    printf("Beginning Brute Force Attack on files in %s\n", directoryPath);

    while ((ent = readdir(dir)) != NULL) {
        if (strstr(ent->d_name, ".txt") != NULL) { // Check if txt
            if(correctKey == -1){
                char fullPath[512];
                snprintf(fullPath, sizeof(fullPath), "%s/%s", directoryPath, ent->d_name); // Construct full path to file
                decryptFile(fullPath);
            }
            else{
                char fullPath[512];
                snprintf(fullPath, sizeof(fullPath), "%s/%s", directoryPath, ent->d_name); // Construct full path to file
                decryptRest(fullPath);
            }
        }
    }

    closedir(dir);
    return EXIT_SUCCESS;
}

void decryptFile(const char* filePath) {
    FILE *file = fopen(filePath, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    char ciphertext[BUFFER_SIZE];
    while (fgets(ciphertext, BUFFER_SIZE, file)) { // Read each line from the file
        if (correctKey == -1) { // Only run decryption loop if key not yet found
            for (int key = 1; key <= 95; key++) { // Test all printable ASCII keys
                char decrypted[BUFFER_SIZE];
                int i = 0;
                while (ciphertext[i] != '\0' && ciphertext[i] != '\n') {
                    char shifted = ciphertext[i] - key;
                    if (shifted < 32) {
                        shifted += 96; // Wrap around within the printable ASCII range
                    }
                    decrypted[i] = shifted;
                    i++;
                }
                decrypted[i] = '\0'; // Null-terminate the decrypted string
                printf("Key: %d, Decrypted Message: %s\n", key, decrypted);

                char response[10];
                printf("Is the message readable (yes/no)? ");
                fgets(response, 10, stdin); // Read user response
                if (strstr(response, "yes") != NULL) {
                    correctKey = key; // Set correct key
                    printf("Correct key found: %d\n", correctKey);
                    printf("Printing entire collection using key: %d\n", correctKey);
                    break;
                }
            }
        } else { // If key is found, decrypt using the correct key only
            char decrypted[BUFFER_SIZE];
            int i = 0;
            while (ciphertext[i] != '\0' && ciphertext[i] != '\n') {
                char shifted = ciphertext[i] - correctKey;
                if (shifted < 32) {
                    shifted += 96; // Wrap around within the printable ASCII range
                }
                decrypted[i] = shifted;
                i++;
            }
            decrypted[i] = '\0'; // Null-terminate the decrypted string
            printf("%s\n", decrypted);
        }
    }
    fclose(file);
}

void decryptRest(const char* filePath) {
    FILE *file = fopen(filePath, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }
    char ciphertext[BUFFER_SIZE];
    while (fgets(ciphertext, BUFFER_SIZE, file)) { // Read each line from the file
            char decrypted[BUFFER_SIZE];
            int i = 0;
            while (ciphertext[i] != '\0' && ciphertext[i] != '\n') {
                char shifted = ciphertext[i] - correctKey;
                if (shifted < 32) {
                    shifted += 96; // Wrap around within the printable ASCII range
                }
                decrypted[i] = shifted;
                i++;
            }
            decrypted[i] = '\0'; // Null-terminate the decrypted string
            printf("%s\n", decrypted);
        }
    fclose(file);
}

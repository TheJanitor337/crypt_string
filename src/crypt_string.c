/**
 * @file crypt_string.c
 *
 * @brief C file for crypt_string functions.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "../include/crypt_utils.h"

#define MENU_CHOICE_ENCRYPT 1
#define MENU_CHOICE_VERIFY 2
#define MENU_CHOICE_EXIT 3

/**
 * @brief Reads a line of input with validation.
 *
 * This function reads a line of input from the standard input (stdin) and validates it.
 *
 * @param buffer The buffer to store the input.
 * @param buffer_size The size of the buffer.
 * @return true if input is read and validated successfully, false otherwise.
 */
bool read_input(char *buffer, size_t buffer_size)
{
    if (fgets(buffer, buffer_size, stdin) == NULL)
    {
        // Error reading input
        return false;
    }

    // Remove the newline character if present
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
    {
        buffer[len - 1] = '\0';
    }

    return true;
}

/**
 * @brief Handles encryption and user interaction.
 */
void encrypt_and_prompt()
{
    char generatedSalt[MAX_SALT_LENGTH];
    char hashed[MAX_STRING_LENGTH];

    // Use getpass to securely input the password
    char *plainText = getpass("Enter the string to encrypt: ");

    generate_salt(generatedSalt, sizeof(generatedSalt));

    if (!encrypt_string(plainText, generatedSalt, hashed))
    {
        printf("Encryption failed.\n");
        return;
    }

    printf("Plaintext: %s\n", plainText);
    printf("Salt: %s\n", generatedSalt);
    printf("Hashed: %s\n", hashed);
}

/**
 * @brief Handles decryption and user interaction.
 */
void verify_and_prompt()
{
    char cipherText[MAX_STRING_LENGTH];
    char userSalt[MAX_SALT_LENGTH];
    char plainText[MAX_SALT_LENGTH];

    printf("Enter the string to validate: ");
    if (!read_input(plainText, sizeof(plainText)))
    {
        printf("Error reading input.\n");
        return;
    }

    printf("Enter the hash: ");
    if (!read_input(cipherText, sizeof(cipherText)))
    {
        printf("Error reading input.\n");
        return;
    }

    printf("Enter the salt: ");
    if (!read_input(userSalt, sizeof(userSalt)))
    {
        printf("Error reading input.\n");
        return;
    }

    bool isValid = verify_hash(plainText, cipherText, userSalt);

    if (isValid)
    {
        printf("Ciphertext matches the salt.\n");
    }
    else
    {
        printf("Ciphertext does not match the salt.\n");
    }
}

int main()
{
    int choice;

    while (1)
    {
        printf("Choose an option:\n");
        printf("%d. Encrypt a string\n", MENU_CHOICE_ENCRYPT);
        printf("%d. Verify encryption\n", MENU_CHOICE_VERIFY);
        printf("%d. Exit\n", MENU_CHOICE_EXIT);
        printf("Enter %d, %d, or %d: ", MENU_CHOICE_ENCRYPT, MENU_CHOICE_VERIFY, MENU_CHOICE_EXIT);

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a valid menu choice.\n");
            // Clear the input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            continue;
        }

        // Consume the newline character
        getchar();

        switch (choice)
        {
        case MENU_CHOICE_ENCRYPT:
            encrypt_and_prompt();
            break;
        case MENU_CHOICE_VERIFY:
            verify_and_prompt();
            break;
        case MENU_CHOICE_EXIT:
            printf("Exiting the program.\n");
            return 0;
        default:
            printf("Invalid choice. Please enter %d, %d, or %d.\n", MENU_CHOICE_ENCRYPT, MENU_CHOICE_VERIFY, MENU_CHOICE_EXIT);
        }
    }

    return 0;
}

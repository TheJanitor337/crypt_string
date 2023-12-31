/**
 * @file crypt_utils.c
 *
 * @brief C file for crypt_utils functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "../include/crypt_utils.h"

/**
 * @brief Generates a random salt.
 *
 * This function generates a random salt using the specified character set and
 * null-terminates it.
 *
 * @param salt The buffer to store the generated salt.
 * @param salt_size The size of the salt buffer.
 */
void generate_salt(char salt[], size_t salt_size)
{
  const char charset[] = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  const int charset_size = sizeof(charset) - 1;

  srand((unsigned int)time(NULL));

  for (size_t i = 0; i < salt_size - 1; ++i)
  {
    salt[i] = charset[rand() % charset_size];
  }
  salt[salt_size - 1] = '\0'; // Null-terminate the salt
}

/**
 * @brief Encrypts a string using crypt encryption.
 *
 * This function encrypts a plaintext string using crypt encryption and stores
 * the result in the provided buffer.
 *
 * @param plainText The plaintext string to encrypt.
 * @param salt The salt to use for encryption.
 * @param hashed The buffer to store the encrypted string.
 * @return true if encryption is successful, false otherwise.
 */
bool encrypt_string(const char *plainText, const char *salt, char hashed[])
{
  // Hash the plaintext password using DES
  char *result = crypt(plainText, salt);

  if (result != NULL)
  {
    strcpy(hashed, result);
    return true;
  }
  else
  {
    perror("Crypt error");
    return false;
  }
}

/**
 * @brief Verifies a plaintext string against a DES hash.
 *
 * This function verifies whether a plaintext string matches a DES hash
 * when using a provided salt.
 *
 * @param plainText The plaintext string to verify.
 * @param crypt_hash The DES hash to compare with.
 * @param salt The salt used for encryption.
 * @return true if the plaintext matches the hash, false otherwise.
 */
bool verify_hash(const char *plainText, const char *crypt_hash, const char *salt)
{
  char expected_hash[MAX_STRING_LENGTH];

  if (encrypt_string(plainText, salt, expected_hash))
  {
    // Compare the expected hash with the provided crypt hash
    return (strcmp(expected_hash, crypt_hash) == 0);
  }
  else
  {
    return false;
  }
}

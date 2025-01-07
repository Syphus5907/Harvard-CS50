#include <stdio.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>

#define AES_KEYLEN 256  // Key length for AES (256 bits)

// Error handling function
void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}

// Function to encrypt the plaintext
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    // Create and initialize the context
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    // Initialize the encryption operation with AES-256-CBC
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    // Provide the message to be encrypted and obtain the encrypted output
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    // Finalize the encryption
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

// Function to decrypt the ciphertext
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;

    // Create and initialize the context
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    // Initialize the decryption operation with AES-256-CBC
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    // Provide the ciphertext to be decrypted and obtain the plaintext output
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    // Finalize the decryption
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
    plaintext_len += len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

int main(void) {
    // Set up the key and IV (Initialization Vector)
    unsigned char key[AES_KEYLEN/8];
    unsigned char iv[16];  // AES block size is 16 bytes

    // Fill the key and IV with random bytes
    if (!RAND_bytes(key, sizeof(key)) || !RAND_bytes(iv, sizeof(iv))) {
        fprintf(stderr, "Error generating random key/IV\n");
        return 1;
    }

    // Message to encrypt
    unsigned char *plaintext = (unsigned char *)"This is the secret message!";

    // Buffer for ciphertext
    unsigned char ciphertext[128];

    // Buffer for decrypted text
    unsigned char decryptedtext[128];

    int decryptedtext_len, ciphertext_len;

    // Encrypt the plaintext
    ciphertext_len = encrypt(plaintext, strlen((char *)plaintext), key, iv, ciphertext);

    // Print ciphertext as hex
    printf("Ciphertext is:\n");
    for(int i = 0; i < ciphertext_len; i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    // Decrypt the ciphertext
    decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv, decryptedtext);

    // Add a NULL terminator to the decrypted text
    decryptedtext[decryptedtext_len] = '\0';

    // Show the decrypted text
    printf("Decrypted text is: %s\n", decryptedtext);

    return 0;
}

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Constantes Globales ---
const char B64_TABLE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// --- Prototipos de Funciones ---
// Codificación
char *base64_encode(const char *input);
void text_to_bits(const char *input, int *bit_array);
void char_to_8bits(char c, int *bits);

// Decodificación
char *base64_decode(char *input);
void b64_to_bits(const char *input, int *bit_array);
void b64_char_to_6bits(char c, int *bits);
int get_b64_index(char c);
char *sanitize_b64(char *input);

int main(int argc, char *argv[])
{
    if (argc != 2 || (strcmp(argv[1], "encode") != 0 && strcmp(argv[1], "decode") != 0))
    {
        printf("USAGE: encoder50 [encode/decode]\n");
        return 1;
    }

    printf("%s\n", "                             __          __________ ");
    printf("%s\n", "  ___  ____  _________  ____╱ ╱__  _____╱ ____╱ __ ╲");
    printf("%s\n", " ╱ _ ╲╱ __ ╲╱ ___╱ __ ╲╱ __  ╱ _ ╲╱ ___╱___ ╲╱ ╱ ╱ ╱");
    printf("%s\n", "╱  __╱ ╱ ╱ ╱ ╱__╱ ╱_╱ ╱ ╱_╱ ╱  __╱ ╱  ____╱ ╱ ╱_╱ ╱ ");
    printf("%s\n", "╲___╱_╱ ╱_╱╲___╱╲____╱╲__,_╱╲___╱_╱  ╱_____╱╲____╱  ");
    printf("\n");

    char *input = get_string("Input:  ");

    if (strcmp(argv[1], "encode") == 0)
    {
        char *encoded = base64_encode(input);
        if (encoded == NULL)
            return 1;
        printf("Output: %s\n", encoded);
        free(encoded);
    }
    else if (strcmp(argv[1], "decode") == 0)
    {
        char *decoded = base64_decode(input);
        if (decoded == NULL)
            return 1;
        printf("Output: %s\n", decoded);
        free(decoded);
    }

    return 0;
}

// --- Codificación ---

char *base64_encode(const char *input)
{
    int len = strlen(input);
    int total_bits = len * 8;

    // Redondeamos al siguiente múltiplo de 6 para el bucle
    int padding_bits = (6 - (total_bits % 6)) % 6;
    int bit_array_size = total_bits + padding_bits;

    // Creamos una lista vacía de la longitud de la cadena * 8 para almacenar el binario de toda la
    // cadena
    int *bit_array = calloc(bit_array_size, sizeof(int));
    if (bit_array == NULL)
        return NULL;

    // Almacenar toda la entrada en una lista con el binario de cada caracter
    text_to_bits(input, bit_array);

    // Calcula la longitud de la futura cadena en chars, con padding incluido
    int output_len = ((4 * strlen(input) / 3) + 3) & ~3;

    char *output = calloc(output_len + 1, sizeof(char));
    if (output == NULL)
    {
        free(bit_array);
        return NULL;
    }
    // Cogemos los bits de 6 en 6 y los juntamos para hacer un caracter de la tabla
    for (int i = 0; i < bit_array_size; i += 6)
    {
        // M -> a -> n
        int index = 0;
        for (int j = 0; j < 6; j++)
        {
            // Desplazamos el índice a la izquierda y añadimos el nuevo bit, que estará a la
            // derecha.
            index = (index << 1) | bit_array[i + j];
        }
        // Usamos el índice para obtener el caracter de la tabla y lo añadimos a la salida
        output[i / 6] = B64_TABLE[index];
    }

    // Añadir padding '='
    if (len % 3 == 1)
    {
        output[output_len - 1] = '=';
        output[output_len - 2] = '=';
    }
    else if (len % 3 == 2)
    {
        output[output_len - 1] = '=';
    }

    free(bit_array);
    return output;
}

void text_to_bits(const char *input, int *bit_array)
{
    for (int i = 0, len = strlen(input); i < len; i++)
    {

        int bits[8];
        char_to_8bits(input[i], bits);

        // Almacenamos los bits en entrada_binario
        for (int j = 0; j < 8; j++)
        {
            // M = 77 = 01001101
            // 0->1->0->0->1->1->0->1
            bit_array[i * 8 + j] = bits[j];
        }
    }
}

void char_to_8bits(char c, int *bits)
{
    for (int i = 0; i < 8; i++)
    {
        // Desplazamos el bit que nos interesa a la posición de la derecha
        // y aplicamos una máscara con 1 para saber si es 0 o 1.

        bits[i] = (c >> (7 - i)) & 1;
    }
}

// --- Decodificación ---

char *base64_decode(char *input)
{
    char *clean_input = sanitize_b64(input);
    int len = strlen(clean_input);
    int total_bits = len * 6;
    int output_bytes = total_bits / 8;

    int *bit_array = calloc(total_bits, sizeof(int));
    if (bit_array == NULL)
        return NULL;

    b64_to_bits(clean_input, bit_array);

    char *output = calloc(output_bytes + 1, sizeof(char));
    if (output == NULL)
    {
        free(bit_array);
        return NULL;
    }

    // Cogemos los bits de 8 en 8 y los juntamos para hacer un caracter
    for (int i = 0; i + 8 <= total_bits; i += 8)
    {
        int ascii_value = 0;
        for (int j = 0; j < 8; j++)
        {
            // Desplazamos el índice a la izquierda y añadimos el nuevo bit, que estará a la
            // derecha.
            ascii_value = (ascii_value << 1) | bit_array[i + j];
        }
        // Usamos el índice para obtener el caracter y lo añadimos a la salida
        output[i / 8] = (char) ascii_value;
    }
    free(bit_array);
    return output;
}

int get_b64_index(char c)
{
    for (int i = 0; i < 64; i++)
    {
        if (B64_TABLE[i] == c)
            return i;
    }
    return -1;
}

void b64_char_to_6bits(char c, int *bits)
{
    int index = get_b64_index(c);
    for (int i = 0; i < 6; i++)
    {
        bits[i] = (index >> (5 - i)) & 1;
    }
}

void b64_to_bits(const char *input, int *bit_array)
{
    for (int i = 0, len = strlen(input); i < len; i++)
    {
        int bits[6];
        b64_char_to_6bits(input[i], bits);
        for (int j = 0; j < 6; j++)
        {
            bit_array[i * 6 + j] = bits[j];
        }
    }
}

char *sanitize_b64(char *entrada)
{
    for (int i = 0, len = strlen(entrada); i < len; i++)
    {
        if (entrada[i] == '=')
        {
            entrada[i] = '\0';
            break;
        }
    }
    return entrada;
}
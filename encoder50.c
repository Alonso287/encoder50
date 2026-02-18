// Ejemplo usado todo el tiempo: "Man"

#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bit(char c, int bits[8]);
void binario(char *entrada, int entrada_binario[strlen(entrada) * 8]);
char *a_base64(char *entrada);

int main(void) {
    // Obtener el string
    char *entrada = get_string("Entrada: ");

    char *salida = a_base64(entrada);
    if (salida == NULL) {
        return 1;
    }

    printf("Salida: %s\n", salida);

    free(salida);

    return 0;
}


char *a_base64(char *entrada) {

    // Tabla de caracteres Base64
    const char tabla_base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    // Creamos una lista vacía de la longitud de la cadena * 8 para almacenar el binario de toda la cadena
    int entrada_binario[strlen(entrada) * 8];

    // Almacenar toda la entrada en una lista con el binario de cada caracter
    binario(entrada, entrada_binario);

    // Crear una variable para almacenar la salida
    int len_salida = ((4 * strlen(entrada) / 3) + 3) & ~3;

    printf("%d\n", len_salida);

    char *salida = malloc(len_salida + 1);
    if (salida == NULL) {
        return NULL;
    }

    salida[len_salida + 1] = '\0';

    // Cogemos los bits de 6 en 6 y los juntamos para hacer un caracter de la tabla
    for (int i = 0, len = strlen(entrada) * 8; i < len; i += 6) {
        // M -> a -> n 
        int indice = 0;
        for (int j = 0; j < 6; j++) {
            // Desplazamos el índice a la izquierda y añadimos el nuevo bit, que estará a la derecha.
            indice = (indice << 1) | entrada_binario[i + j];
        }
        // Usamos el índice para obtener el caracter de la tabla y lo añadimos a la salida
        salida[i / 6] = tabla_base64[indice];
    }
    return salida;
}


void bit(char c, int bits[8]) {
    // Como una función no puede devolver directamente una lista,
    // lo que haremos será recibir el char y una lista vacía,
    // y rellenar la lista.
    for (int i = 0; i < 8; i++) {
        // Desplazamos el bit que nos interesa a la posición de la derecha
        // y aplicamos una máscara con 1 para saber si es 0 o 1.

        // M = 77 = 01001101
        // 0->1->0->0->1->1->0->1
        int bit = (c >> (7 - i)) & 1;

        bits[i] = bit;
    }
}


void binario(char *entrada, int entrada_binario[strlen(entrada) * 8]) {
    // Recorremos cada letra de la entrada
    // Orden: M->a->n
    for (int i = 0, len = strlen(entrada); i < len; i++) {
        // Creamos una lista vacía y la rellenamos
        // Con los bits del char
        int bits[8];
        bit(entrada[i], bits);

        // Almacenamos los bits en entrada_binario
        for (int j = 0; j < 8; j++) {
            // M = 77 = 01001101
            // 0->1->0->0->1->1->0->1
            entrada_binario[i * 8 + j] = bits[j];
        }
    }
}
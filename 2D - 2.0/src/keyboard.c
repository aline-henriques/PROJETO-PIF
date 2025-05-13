/**
 * keyboard.c
 * Adaptado para Windows
 */

#include <conio.h>
#include "keyboard.h"

void keyboardInit() {
    // Não é necessário nada no Windows
}

void keyboardDestroy() {
    // Também não é necessário nada no Windows
}

int keyhit() {
    return _kbhit(); // verifica se alguma tecla foi pressionada
}

int readch() {
    return _getch(); // lê a tecla pressionada
}

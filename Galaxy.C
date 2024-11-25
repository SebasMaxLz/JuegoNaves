#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>  // Para usar rand()
#include <time.h>
#include <stdbool.h> //para el bool

#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80

// Estructuras para representar la nave, los asteroides y las balas
typedef struct {
    int x, y;
    int corazones;
    int vidas;
} Nave;

typedef struct {
    int x, y;
} Asteroide;

typedef struct {
    int x, y;
} Bala;

void gotxy(int x, int y) {    //Función para posicionarme en consola
    HANDLE hCON;
    hCON = GetStdHandle(STD_OUTPUT_HANDLE);  // Toma control de la salida de la consola
    COORD coor;
    coor.X = x;
    coor.Y = y;
    SetConsoleCursorPosition(hCON, coor);
}

void OcultarCursor(){
    HANDLE hCON;
    hCON = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO Cu;
    Cu.dwSize = 1;
    Cu.bVisible = false;
    SetConsoleCursorInfo(hCON, &Cu);
}

void pintar_limites(){
    for(int i = 2; i < 78; i++) {
        gotxy(i, 3); printf("%c", 205);
        gotxy(i, 33); printf("%c", 205);
    }
    for(int i = 4; i < 33; i++) {
        gotxy(2, i); printf("%c", 186);
        gotxy(77, i); printf("%c", 186);
    }
    gotxy(2, 3); printf("%c", 201);
    gotxy(2, 33); printf("%c", 200);
    gotxy(77, 3); printf("%c", 187);
    gotxy(77, 33); printf("%c", 188);
}

void pintar_nave(Nave *nave) {
    gotxy(nave->x, nave->y); printf("  %c", 30);
    gotxy(nave->x, nave->y + 1); printf(" %c%c%c", 40, 207, 41);
    gotxy(nave->x, nave->y + 2); printf("%c%c %c%c", 30, 190, 190, 30);
}

void borrar_nave(Nave *nave) {
    gotxy(nave->x, nave->y);     printf("        ");
    gotxy(nave->x, nave->y + 1); printf("        ");
    gotxy(nave->x, nave->y + 2); printf("        ");
}

void mover_nave(Nave *nave) {
    if (kbhit()) {
        char tecla = getch();
        borrar_nave(nave);
        if (tecla == ARRIBA && nave->y > 4) nave->y--;
        if (tecla == DERECHA && nave->x + 6 < 77) nave->x++;
        if (tecla == IZQUIERDA && nave->x > 3) nave->x--;
        if (tecla == ABAJO && nave->y + 3 < 33) nave->y++;
        pintar_nave(nave);
    }
}

void pintar_corazones(Nave *nave) {
    gotxy(50, 2); printf("vidas %d", nave->vidas);
    gotxy(64, 2); printf("Salud");
    gotxy(70, 2); printf("     ");
    for (int i = 0; i < nave->corazones; i++) {
        gotxy(70 + i, 2); printf("%c", 3);
    }
}

void morir(Nave *nave) {
    if (nave->corazones == 0) {
        borrar_nave(nave);
        gotxy(nave->x, nave->y);     printf("   **   ");
        gotxy(nave->x, nave->y + 1); printf("  ****  ");
        gotxy(nave->x, nave->y + 2); printf("   **   ");
        Sleep(200);
        borrar_nave(nave);
        gotxy(nave->x, nave->y);     printf(" * ** * ");
        gotxy(nave->x, nave->y + 1); printf("  ****  ");
        gotxy(nave->x, nave->y + 2); printf(" * ** * ");
        Sleep(200);
        borrar_nave(nave);
        nave->vidas--;
        nave->corazones = 3;
        pintar_corazones(nave);
        pintar_nave(nave);
    }
}

void pintar_asteroide(Asteroide *asteroide) {
    gotxy(asteroide->x, asteroide->y); printf("%c", 184);
}

void mover_asteroide(Asteroide *asteroide) {
    gotxy(asteroide->x, asteroide->y); printf(" ");  // Borra la posición anterior del asteroide
    asteroide->y++;  // Mueve el asteroide hacia abajo
    if (asteroide->y > 32) {
        asteroide->x = rand() % 71 + 4;  // Cambia la posición horizontal aleatoriamente
        asteroide->y = 4;  // Vuelve al principio (en la parte superior)
    }
    pintar_asteroide(asteroide);  // Dibuja el asteroide en la nueva posición
}

void choque_asteroide(Asteroide *asteroide, Nave *nave) {
    if (asteroide->x >= nave->x && asteroide->x < nave->x + 6 && asteroide->y >= nave->y && asteroide->y <= nave->y + 2) {
        nave->corazones--;
        borrar_nave(nave);
        pintar_nave(nave);
        pintar_corazones(nave);
        asteroide->x = rand() % 71 + 3;
        asteroide->y = 4;
    }
}

void pintar_bala(Bala *bala) {
    gotxy(bala->x, bala->y); printf("|");
}

void mover_bala(Bala *bala) {
    gotxy(bala->x, bala->y); printf(" ");
    if (bala->y > 4) bala->y--;
    pintar_bala(bala);
}

int main() {
    bool perdiste = false;
    int puntos = 0;
    int velocidad = 10;
    int gravedad = 9;
    Nave nave = {37, 30, 3, 3};

    OcultarCursor();
    pintar_limites();
    pintar_nave(&nave);
    pintar_corazones(&nave);

    Asteroide asteroides[5];
    for (int i = 0; i < 5; i++) {
        asteroides[i].x = rand() % 75 + 3;
        asteroides[i].y = rand() % 5 + 4;
    }

    Bala balas[100];
    int cantidad_balas = 0;

    while (!perdiste) {
        gotxy(2, 2); printf("Puntos %d", puntos);
        gotxy(12, 2); printf("Gravedad %d", gravedad);
        gotxy(26, 2); printf("Velocidad %d", velocidad);
        velocidad++;
        if (kbhit()) {
            char tecla = getch();
            if (tecla == 'a') {  // CREO LAS BALAS
                balas[cantidad_balas].x = nave.x + 2;
                balas[cantidad_balas].y = nave.y - 1;
                cantidad_balas++;
            }
        }

        for (int i = 0; i < cantidad_balas; i++) {
            mover_bala(&balas[i]);
            if (balas[i].y == 4) {
                gotxy(balas[i].x, balas[i].y); printf(" ");
                cantidad_balas--;
                for (int j = i; j < cantidad_balas; j++) {
                    balas[j] = balas[j + 1];
                }
                i--;
            }
        }

        for (int i = 0; i < 5; i++) {
            mover_asteroide(&asteroides[i]);
            choque_asteroide(&asteroides[i], &nave);
        }

        for (int i = 0; i < 5; i++) { // controlamos los asteroides
            for (int j = 0; j < cantidad_balas; j++) { // controlamos las balas
                if (asteroides[i].x == balas[j].x && asteroides[i].y == balas[j].y) {
                    gotxy(balas[j].x, balas[j].y); printf(" ");
                    cantidad_balas--;
                    for (int k = j; k < cantidad_balas; k++) {
                        balas[k] = balas[k + 1];
                    }
                    j--;

                    // Creo un nuevo asteroide
                    asteroides[i].x = rand() % 74 + 3;
                    asteroides[i].y = 4;

                    puntos += 5;
                    if (puntos > 30) gravedad += 10;
                }
            }
        }

        if (nave.vidas == 0) perdiste = true;
        morir(&nave);
        mover_nave(&nave);
        Sleep(30);  // Pausa para controlar la velocidad de actualización
    }

    system("cls");

    gotxy(20, 14); printf("Tus puntos son %d", puntos);
    gotxy(20, 16); printf("Alcanzaste una Velocidad de %d km/h", velocidad);

    getchar();
    return 0;
}
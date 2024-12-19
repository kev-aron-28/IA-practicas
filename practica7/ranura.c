#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define MAXLEN 500

// Compilación: gcc ranura.c -o r -lncurses
void init_colors() {
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);   // Títulos
    init_pair(2, COLOR_WHITE, COLOR_BLUE);   // Fondo de entrada
    init_pair(3, COLOR_GREEN, COLOR_BLACK);  // Encendido
    init_pair(4, COLOR_RED, COLOR_BLACK);    // Apagado
    init_pair(5, COLOR_YELLOW, COLOR_BLACK); // Mensajes de error
}

// Dibuja un recuadro con caracteres simples
void draw_box(int y, int x, int color, const char* label) {
    attron(COLOR_PAIR(color));
    mvprintw(y - 1, x - 4, "+------+");
    mvprintw(y, x - 4, "| %s |", label);
    mvprintw(y + 1, x - 4, "+------+");
    attroff(COLOR_PAIR(color));
}

int main(int argc, char const *argv[])
{
    char input[MAXLEN] = {0};
    char dispositivo[50], ubicacion[50], hora[50];
    char comando[10];
    int ch, pos = 0;

    initscr();
    cbreak();
    noecho();
    curs_set(1);
    init_colors();

    attron(COLOR_PAIR(1));
    box(stdscr, 0, 0);
    mvprintw(1, 2, "Sistema de Control de Dispositivos");
    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(1));
    mvprintw(3, 2, "Ingresa la cadena en el formato:");
<<<<<<< HEAD
    mvprintw(4, 4, "[Enciende/Apaga] el [Dispositivo] en [Ubicación] a las [Hora]");
=======
    mvprintw(4, 4, "reserva un vuelo de [Origen] a [Destino] el [Fecha]");
>>>>>>> 8c01dea (chore(shared): user has now associated each module it can access)
    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(2));
    mvprintw(6, 2, "Input: ");
    attroff(COLOR_PAIR(2));
    refresh();

    move(6, 9);

    while ((ch = getch()) != '\n') {
        if (ch == KEY_BACKSPACE || ch == 127) {
            if (pos > 0) {
                pos--;
                input[pos] = '\0';
                mvaddch(6, 9 + pos, ' ');
                move(6, 9 + pos);
            }
        } else if (pos < MAXLEN - 1) {
            input[pos++] = ch;
            addch(ch);
        }
        refresh();
    }
    input[pos] = '\0';

    clear();
    attron(COLOR_PAIR(1));
    box(stdscr, 0, 0);
    mvprintw(1, 2, "Resultados de Control");
    attroff(COLOR_PAIR(1));

<<<<<<< HEAD
    if (sscanf(input, "%9s el %49s en %49s a las %49[^\n]", comando, dispositivo, ubicacion, hora) == 4) {
        if (strncasecmp(comando, "enciende", 8) == 0) {
            attron(COLOR_PAIR(3));
            mvprintw(3, 2, "Acción: Encender");
            mvprintw(4, 2, "Dispositivo: %s", dispositivo);
            mvprintw(5, 2, "Ubicación: %s", ubicacion);
            mvprintw(6, 2, "Hora: %s", hora);
            attroff(COLOR_PAIR(3));

        } else if (strncasecmp(comando, "apaga", 5) == 0) {
            attron(COLOR_PAIR(4));
            mvprintw(3, 2, "Acción: Apagar");
            mvprintw(4, 2, "Dispositivo: %s", dispositivo);
            mvprintw(5, 2, "Ubicación: %s", ubicacion);
            mvprintw(6, 2, "Hora: %s", hora);
            attroff(COLOR_PAIR(4));

        } else {
            attron(COLOR_PAIR(5));
            mvprintw(3, 2, "Error: Comando no reconocido.");
            attroff(COLOR_PAIR(5));
        }
    } else {
        attron(COLOR_PAIR(5));
=======
    if (sscanf(input, "reserva un vuelo de %49s a %49s el %49[^\n]", origen, destino, fecha) == 3)
    {
        attron(COLOR_PAIR(3));
        mvprintw(3, 2, "Origen: %s", origen);
        mvprintw(4, 2, "Destino: %s", destino);
        mvprintw(5, 2, "Fecha: %s", fecha);
        attroff(COLOR_PAIR(3));
    } 
    else if (sscanf(input, "reserva un vuelo desde %49s a %49s el %49[^\n]", origen, destino, fecha) == 3) {
        attron(COLOR_PAIR(3));
        mvprintw(3, 2, "Origen: %s", origen);
        mvprintw(4, 2, "Destino: %s", destino);
        mvprintw(5, 2, "Fecha: %s", fecha);
        attroff(COLOR_PAIR(3));
    }
    else
    {
        attron(COLOR_PAIR(4));
>>>>>>> 8c01dea (chore(shared): user has now associated each module it can access)
        mvprintw(3, 2, "Error: No se pudo analizar correctamente la cadena.");
        attroff(COLOR_PAIR(5));
    }

    attron(COLOR_PAIR(1));
    mvprintw(15, 2, "Presiona cualquier tecla para salir...");
    attroff(COLOR_PAIR(1));
    refresh();

    getch();

    endwin();
    return 0;
}

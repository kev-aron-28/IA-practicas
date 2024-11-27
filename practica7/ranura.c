#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define MAXLEN 500


// comp: gcc ranura.c -o r -lncurses 
void init_colors() {
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);   // Títulos
    init_pair(2, COLOR_WHITE, COLOR_BLUE);   // Fondo de entrada
    init_pair(3, COLOR_GREEN, COLOR_BLACK);  // Resultados
    init_pair(4, COLOR_RED, COLOR_BLACK);    // Mensajes de error
}

int main(int argc, char const *argv[])
{
    char input[MAXLEN] = {0};
    char origen[50], destino[50], fecha[50];
    int ch, pos = 0;

    initscr();
    cbreak();
    noecho();
    curs_set(1);
    init_colors();

    attron(COLOR_PAIR(1));
    box(stdscr, 0, 0);
    mvprintw(1, 2, "Sistema de Reservas de Vuelo");
    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(1));
    mvprintw(3, 2, "Ingresa la cadena en el formato:");
    mvprintw(4, 4, "Reserva un vuelo de [Origen] a [Destino] el [Fecha]");
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
    mvprintw(1, 2, "Resultados de la Reserva");
    attroff(COLOR_PAIR(1));

    if (sscanf(input, "reserva un vuelo de %49s a %49s el %49[^\n]", origen, destino, fecha) == 3)
    {
        attron(COLOR_PAIR(3));
        mvprintw(3, 2, "Origen: %s", origen);
        mvprintw(4, 2, "Destino: %s", destino);
        mvprintw(5, 2, "Fecha: %s", fecha);
        attroff(COLOR_PAIR(3));
    }
    else
    {
        attron(COLOR_PAIR(4));
        mvprintw(3, 2, "Error: No se pudo analizar correctamente la cadena.");
        attroff(COLOR_PAIR(4));
    }

    attron(COLOR_PAIR(1));
    mvprintw(7, 2, "Presiona cualquier tecla para salir...");
    attroff(COLOR_PAIR(1));
    refresh();

    getch();

    endwin();
    return 0;
}

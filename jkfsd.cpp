// Woorr1.cpp: главный файл проекта.

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <malloc.h>
#include <clocale>

using namespace std;
using namespace System;
using namespace System::IO;

#define ENTER 13
#define ESC 27
#define UP 72
#define DOWN 80

const char dan[7][80] = {
    "У кого NEW машина?                                      ",
    "Введите силь матора, чтоб узнать информацию             ",
    "Одинаковая силь, но разные машины                       ",
    "Алфавитный список всех покупателей?                     ",
    "Диаграмма. Процентное соотношение покупателей с машинами",
    "                                                        ",
    "Выход                                                   "
};

const char BlankLine[] = "";

struct z {
    char name[15];
    char core[10];
    int video;
    char ozu[4];
    long price;
    char date[10];
};

struct sp {
    char fio[20];
    char core[10];
    long price;
    struct sp* sled;
    struct sp* pred;
};

int menu(int n, const char dan[7][80]);
void maxim(struct z* clients, int NC);
void first(struct z* clients, int NC);
void kolvo(struct z* clients, int NC);
void alfalist(struct z* clients, int NC, struct sp* &spisok);
void diagram(struct z* clients, int NC, struct sp* &spisok);
void vstavka(struct z* clients, int NC, struct sp* &spisok, char* fio);
void writeToFile(struct z* clients, int NC, const char* filename);

int main(array<System::String ^> ^args) {
    int i, n, NC;
    FILE *in;
    struct z *clients;
    struct sp* spisok = NULL;
    
    setlocale(LC_CTYPE, "Russian");
    Console::CursorVisible::set(false);
    Console::BufferHeight = Console::WindowHeight;
    Console::BufferWidth = Console::WindowWidth; 

    if ((in = fopen("aslbek1_6.txt", "r")) == NULL) {
        printf("\nФайл не открыт !");
        _getch();
        exit(1);
    }
    
    fscanf(in, "%d", &NC);
    clients = (struct z*)malloc(NC * sizeof(struct z));

    for (i = 0; i < NC; i++) {
        fscanf(in, "%s%s%ld%s%ld%s", clients[i].name, clients[i].core, &clients[i].video, clients[i].ozu, &clients[i].price, clients[i].date);
    }
    
    fclose(in);

    for (i = 0; i < NC; i++) {
        printf("\n%-16s %-12s %-8ld %-8s %5ld %-16s", clients[i].name, clients[i].core, clients[i].video, clients[i].ozu, clients[i].price, clients[i].date);
    }
    
    _getch();

    while (true) {
        Console::ForegroundColor = ConsoleColor::Gray;
        Console::BackgroundColor = ConsoleColor::Black;
        Console::Clear();
        Console::ForegroundColor = ConsoleColor::Black;
        Console::BackgroundColor = ConsoleColor::Gray;
        Console::CursorLeft = 10;
        Console::CursorTop = 4;
        printf(BlankLine);
        for (i = 0; i < 7; i++) {
            Console::CursorLeft = 10;
            Console::CursorTop = i + 5;
            printf(" %s ", dan[i]);
        }
        Console::CursorLeft = 10;
        Console::CursorTop = 11;
        printf(BlankLine);

        n = menu(7, dan);
        switch (n) {
            case 1: maxim(clients, NC); break;
            case 2: first(clients, NC); break;
            case 3: kolvo(clients, NC); break;
            case 4: alfalist(clients, NC, spisok); break;
            case 5: diagram(clients, NC, spisok); break;
            case 6: kolvo(clients, NC); break;
            case 7: 
                writeToFile(clients, NC, "aslbek1_6.txt"); 
                free(clients);
                exit(0);
        }
    }

    free(clients);
    return 0;
}

int menu(int n, const char dan[7][80]) {
    int y1 = 0, y2 = n - 1;
    char c = 1;
    while (c != ESC) {
        switch (c) {
            case DOWN: y2 = y1; y1++; break;
            case UP: y2 = y1; y1--; break;
            case ENTER: return y1 + 1;
        }
        if (y1 > n - 1) { y2 = n - 1; y1 = 0; }
        if (y1 < 0) { y2 = 0; y1 = n - 1; }
        Console::ForegroundColor = ConsoleColor::Black;
        Console::BackgroundColor = ConsoleColor::White;
        Console::CursorLeft = 11;
        Console::CursorTop = y1 + 5;
        printf("%s", dan[y1]);
        Console::ForegroundColor = ConsoleColor::Black;
        Console::BackgroundColor = ConsoleColor::Gray;
        Console::CursorLeft = 11;
        Console::CursorTop = y2 + 5;
        printf("%s", dan[y2]);
        c = getch();
    }
    exit(0);
}

void maxim(struct z* clients, int NC) {
    int i = 0;
    struct z best;
    strcpy(best.name, clients[0].name);
    best.video = clients[0].video;
    for (i = 1; i < NC; i++) {
        if (clients[i].video > best.video) {
            strcpy(best.name, clients[i].name);
            best.video = clients[i].video;
        }
    }

    Console::ForegroundColor = ConsoleColor::Yellow;
    Console::BackgroundColor = ConsoleColor::Black;
    Console::CursorLeft = 10;
    Console::CursorTop = 15;
    printf("Самую NEW машина имеет покупатель %s с машиной %d", best.name, best.video);
    getch();
}

void first(struct z* clients, int NC) {
    int inputMemory;
    Console::ForegroundColor = ConsoleColor::Red;
    Console::CursorLeft = 10;
    Console::CursorTop = 15;
    printf("Введите силь матор: ");
    scanf("%d", &inputMemory);
    for (int i = 0; i < NC; i++) {
        if (atoi(clients[i].ozu) == inputMemory) {
            Console::CursorLeft = 10;
            Console::CursorTop++;
            printf("Клиент %s имеет силь двигатель %s", clients[i].name, clients[i].ozu);
        }
    }
    getch();
}

void vstavka(struct z* clients, int NC, struct sp* &spisok, char* fio) {
    int i;
    struct sp* nov, * nt, * z = 0;
    for (nt = spisok; nt != 0 && strcmp(nt->fio, fio) < 0; z = nt, nt = nt->sled);
    if (nt && strcmp(nt->fio, fio) == 0) return;
    nov = (struct sp*)malloc(sizeof(struct sp));
    strcpy(nov->fio, fio);
    for (i = 0; i < NC; i++) {
        if (strcmp(clients[i].name, fio) == 0) {
            strcpy(nov->core, clients[i].core);
            break;
        }
    }
    nov->pred = z;
    nov->sled = nt;
    if (!z) spisok = nov;
    else z->sled = nov;
    if (nt) nt->pred = nov;
}

void diagram(struct z* clients, int NC, struct sp* &spisok) {
    struct sp* nt;
    int len, i, NColor = 0, sum = 0;
    char str1[100];
    char str2[100];
    char str3[100];
    ConsoleColor Color;
    Console::BackgroundColor = ConsoleColor::Black;
    Console::Clear();
    for (i = 0; i < NC; i++)
        sum += clients[i].price;
    if (!spisok)
        for (i = 0; i < NC; i++)
            vstavka(clients, NC, spisok, clients[i].name); // Вставка имен покупателей
    Color = ConsoleColor::Black;
    for (nt = spisok, i = 0; nt != 0; nt = nt->sled, i++) {
        sprintf(str1, "%s", clients[i].name);
        sprintf(str3, "%s", clients[i].core);
        sprintf(str2, "%3.1f%%", (clients[i].price * 100. / sum));
        Console::ForegroundColor = ConsoleColor::Cyan;
        Console::BackgroundColor = ConsoleColor::Black;
        Console::CursorLeft = 5;
        Console::CursorTop = i + 1;
        printf("%s", str1);
        Console::CursorLeft = 35;
        printf("%s", str3);
        Console::CursorLeft = 55;
        printf("%s", str2);
        Console::BackgroundColor = ++Color;
        NColor++;
        Console::CursorLeft = 60;
        for (len = 0; len < clients[i].price * 100. / sum + 5; len++)
            printf(" ");
        if (NColor == 14) // выделяем на диаграмму 14 цветов
        {
            Color = ConsoleColor::Black;
            NColor = 0;
        }
    }
    getch();
}

void alfalist(struct z* clients, int NC, struct sp* &spisok) {
    int i, n = 0;
    struct sp* nt;
    struct sp* z;
    Console::ForegroundColor = ConsoleColor::White;
    Console::BackgroundColor = ConsoleColor::Black;
    Console::Clear();
    if (!spisok) {
        for (i = 0; i < NC; i++) {
            vstavka(clients, NC, spisok, clients[i].name);
        }
    }

    Console::Clear();
    printf("\n Алфавитный список покупателей  ");
    printf("\t\t Алфавитный список покупателей в обратном порядке");
    printf("\n ===================================\t");
    printf("\t========================================================\n");
    for (nt = spisok; nt != 0; nt = nt->sled) {
        printf("\n %-20s %s", nt->fio, nt->core);
    }

    for (nt = spisok, z = 0; nt != 0; z = nt, nt = nt->sled);
    for (nt = z; nt != 0; nt = nt->pred) {
        Console::CursorLeft = 48;
        Console::CursorTop = 4 + n;
        printf(" %-20s %s", nt->fio, nt->core);
        n += 1;
    }
    _getch();
    Console::Clear();
}

void kolvo(struct z* clients, int NC) {
    printf("Hello kolvo");
}

void writeToFile(struct z* clients, int NC, const char* filename) {
    FILE* out = fopen(filename, "w");
    if (out == NULL) {
        printf("\nНе удалось открыть файл для записи!");
        return;
    }

    fprintf(out, "%d\n", NC);
    for (int i = 0; i < NC; i++) {
        fprintf(out, "%s %s %ld %s %ld %s\n", clients[i].name, clients[i].core, clients[i].video, clients[i].ozu, clients[i].price, clients[i].date);
    }

    fclose(out);
}

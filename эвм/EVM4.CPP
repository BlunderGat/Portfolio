#include "iostream.h"
#include "conio.h"
#include "dos.h"

const int C = 261;  // Нота C (До)
const int D = 294;  // Нота D (Ре)
const int E = 329;  // Нота E (Ми)
const int F = 349;  // Нота F (Фа)
const int G = 392;  // Нота G (Соль)
const int A = 440;  // Нота A (Ля)
const int B = 493;  // Нота B (Си)

// Длительности нот (в миллисекундах)
const int quarterNote = 400;   // Четвертная нота
const int halfNote = 800;      // Половинная нота

void playMelody() {
    // Проигрываем мелодию
    sound(C);
    delay(quarterNote);
    nosound(); // Останавливаем звук после каждой ноты

    sound(D);
    delay(quarterNote);
    nosound();

    sound(E);
    delay(quarterNote);
    nosound();

    sound(F);
    delay(quarterNote);
    nosound();

    sound(G);
    delay(halfNote);
    nosound();

    sound(A);
    delay(quarterNote);
    nosound();

    sound(B);
    delay(halfNote);
    nosound();
}

void drawBorders() {
    // Нарисовать вертикальные границы '|'
    for (int i = 5; i <= 15; i++) {
        gotoxy(10, i);  // Левая граница
        cout << "|";
        gotoxy(70, i);  // Правая граница
        cout << "|";
    }

    // Нарисовать горизонтальные границы '_'
    for (i = 10; i <= 70; i++) {
        gotoxy(i, 5);  // Верхняя граница
        cout << "_";
        gotoxy(i, 15); // Нижняя граница
        cout << "_";
    }
}

int main() {
    int a = 0;
    int x = 45, y = 10;

    clrscr();

    // Нарисовать рамку
    drawBorders();

    // Позиционировать символ
    gotoxy(x, y);
    cout << "*";

    while (a != 27) {  // 27 — это код клавиши "Esc" для выхода из цикла
        a = getch();
        gotoxy(x, y);  // Стереть старый символ
        cout << " ";

        switch (a) {
        case 72:  // Вверх (код стрелки вверх)
	    if (y > 6) y--;  // Ограничение сверху (не ниже рамки)
	    else {
		sound(500);
		delay(150);
		nosound();
		y = 14;  // Вернуться в нижнюю часть
	    }
	    break;
	case 80:  // Вниз (код стрелки вниз)
	    if (y < 14) y++;  // Ограничение снизу
            else {
                sound(500);
                delay(150);
                nosound();
                playMelody();
		y = 6;  // Вернуться вверх
            }
            break;
        case 75:  // Влево (код стрелки влево)
            if (x > 11) x--;  // Ограничение слева (не заходить за границу)
            break;
        case 77:  // Вправо (код стрелки вправо)
            if (x < 69) x++;  // Ограничение справа
            break;
        }

        gotoxy(x, y);  // Отобразить символ на новой позиции
        cout << "*";
    }

    return 0;
}

// Simple, transparent recoil helper for benign/local use only.
// Toggles: F5 = ON/OFF. Activate by holding LEFT + RIGHT mouse buttons.

#include <windows.h>
#include <stdio.h>
#include <conio.h>

double vertical_recoil = 5.0;    // 1..10
double horizontal_recoil = 2.0;  // 1..10
bool active = false;

// set console text color (Windows)
void setColor(WORD color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Print menu/status
void printStatus() {
    system("cls");
    printf("=============================\n");
    printf("        VORTEX RECOIL        \n");
    printf("=============================\n");
    printf("[1] Vertical recoil (1-10): %.0f\n", vertical_recoil);
    printf("[2] Horizontal recoil (1-10): %.0f\n", horizontal_recoil);
    printf("[F5] Script status: ");
    if (active) {
        setColor(10); // green
        printf("ON\n");
    } else {
        setColor(12); // red
        printf("OFF\n");
    }
    setColor(7); // reset
    printf("[ESC] Exit\n");
    printf("=============================\n");
    printf("Hold LEFT + RIGHT mouse buttons to apply movement\n");
    printf("Press a number to change a value\n");
}

// Move mouse relatively using SendInput (preferred over mouse_event)
void moveMouseRelative(int dx, int dy) {
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.dx = dx;
    input.mi.dy = dy;
    SendInput(1, &input, sizeof(input));
}

int main() {
    printStatus();

    while (true) {
        // non-blocking keyboard handling
        if (_kbhit()) {
            int ch = _getch();

            if (ch == 27) { // ESC
                break;
            } else if (ch == '1') {
                printf("Enter vertical recoil (1-10): ");
                double v;
                if (scanf_s("%lf", &v) == 1) {
                    if (v < 1.0) v = 1.0;
                    if (v > 10.0) v = 10.0;
                    vertical_recoil = v;
                }
                printStatus();
            } else if (ch == '2') {
                printf("Enter horizontal recoil (1-10): ");
                double h;
                if (scanf_s("%lf", &h) == 1) {
                    if (h < 1.0) h = 1.0;
                    if (h > 10.0) h = 10.0;
                    horizontal_recoil = h;
                }
                printStatus();
            } else if (ch == 0 || ch == 224) {
                // special key - read next code
                int code = _getch();
                // If F5 was pressed via _getch mapping it may show different values,
                // so we also handle toggling via GetAsyncKeyState below.
                (void)code;
            }
        }

        // allow toggling by pressing F5 even if not captured by _getch
        if (GetAsyncKeyState(VK_F5) & 1) {
            active = !active;
            printStatus();
        }

        // If active and both mouse buttons are held, move the mouse
        if (active &&
            (GetAsyncKeyState(VK_LBUTTON) & 0x8000) &&
            (GetAsyncKeyState(VK_RBUTTON) & 0x8000)) {

            int dx = (int)horizontal_recoil; // horizontal pixels per tick
            int dy = (int)vertical_recoil;   // vertical pixels per tick (positive = down)
            moveMouseRelative(dx, dy);
            // very small sleep to avoid pegging CPU, movement is still frequent
            Sleep(1);
            continue; // quick loop while firing
        }

        Sleep(8); // idle sleep to reduce CPU use when not active
    }

    return 0;
}

#include "mbed.h" 
#include "Blue_ESC.h"

DigitalOut myled(LED1);
//Blue_ESC blue(A1, 9);

int main() {
    while(1) {
        myled = 1;
        wait(0.2);
        myled = 0;
        wait(0.2);
    }
}

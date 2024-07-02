#include <stdio.h>

void deposit(int* balance, int amount) {
    // before dereferencing, make sure balance is not a null pointer
    if (!balance) return;

    *balance += amount;
}

void withdraw(int* balance, int amount) {
    if (balance == NULL) {
        return;
    }
    if (*balance >= amount) {
        *balance -= amount;
    }
}

int main() {
    int balance = 100;
    deposit(&balance, 50);

    // does not increment because you are passing by value, making a copy of arguments to pass into balance(), need to use pass by reference
    printf("%d\n", balance);



    return 0;
}
#include <stdio.h>

void increaseBalance(int *balance, int amount) {
    *balance += amount;
}

void decreaseBalance(int *balance, int amount) {
    if (!balance) {
        return;
    }
    *balance -= amount;
}

typedef struct person {
	char* name;
	int age;
} person_t;

// person_t *create_person(char* name, int age) {
	
// }

int main(void) {
    int bal = 100;

    decreaseBalance(&bal, 15);
    increaseBalance(&bal, 200);
    decreaseBalance(NULL, 30);

    printf("%d", bal);

    return 0;
}

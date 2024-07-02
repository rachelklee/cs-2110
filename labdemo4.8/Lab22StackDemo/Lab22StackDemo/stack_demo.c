// READ THE HEADER FILE FOR MORE DOCUMENTATION
#include "stack_demo.h"

/**
 * You will implement a stack that will be used to manage cars. To  implement this stack, you will use a array of pointers. 
 * That means each array element contains a pointer to an object of type car_t.
 * In order to help you manage the data structure, we have provided two global pointers. There is capacity
 * for the size of the backing array and numCars for the number of items held in the stack. You will be given
 * responsibility to change these variables as needed.
 * 
 * Note that dynamic memory allocation is required for this implementation. It’s unknown if you will have to
 * increase the size of the backing array at compilation.
 * 
 * For this stack, you shall be writing two functions
 * stack: pop_stack, and destroy_stack
 * 
 */


/**
 * \brief Variables used to track the stack.
 *
 * The two integers (capacity and numCars) represent the current 
 * maximum capacity of cars the stack can handle and the number of cars on the stack so far.
 * 
 * The pointer (stack_arr) points to the bottom of the stack, or in other words, the first element in the backing array.
 *
 * Initially, capacity is set to 5, numCars is set to 0, and stack_arr is
 * pointing the first open spot at the bottom of the stack. The stack is initially
 * empty so stack_arr is originally pointing to garbage data.
 *
 * \property extern int capacity
 * \property extern int numCars
 * \property extern struct car_t **stack_arr
 */
int capacity = 5;
int numCars = 0;
struct car_t **stack_arr;


/**
 * \brief Provided function to initialize the stack
*/
void initialize_stack(void) {
    stack_arr = (struct car_t **) malloc(sizeof(struct car_t*) * capacity);
}

/**
 * \brief Add cars to the top of the stack
 *
 * This function will be called by client code to add a car to the top of
 * the stack. The caller will supply the question and category of the car to add.
 *
 * This function should allocate a car on the heap. No deep-copying is necessary as the car struct
 * doesn't contain any pointers.
 * Make sure that all members of the [car_t] are set!
 *
 * If the stack is full, double the capacity of the stack and then add the new car to the stack.
 * 
 * Finally, insert the car onto the stack with the help of the [stack_arr] pointer.
 * 
 * This function should return `SUCCESS` if the car was added successfully.
 * If it fails, it should return `FAILURE` and leave the list unchanged. It
 * should fail if and only if:
 * * `malloc` or `realloc` fails,
 * * the car has no wheels
 *
 * \param[in] wheels The number of cars
 * \param[in] color The color for the car
 * \return Whether the car was successfully added
 */
int stack_push(int wheels, enum color_t color) {


    // Check that the stack is not full
    if(capacity >= numCars) {
        struct car_t **tmp = realloc(stack_arr, (sizeof(struct car_t *) * capacity * 2));
        if(tmp == NULL) {
            return FAILURE;
        }
        stack_arr = tmp;
        capacity *= 2;
    }

    // Create a new car on the heap
    // Die on failure
    struct car_t *new_car = malloc(sizeof(struct car_t));
    if (new_car == NULL) {
        return FAILURE;
    }

    // Set all the fields appropriately
    new_car->wheels = wheels;
    new_car->color = color;

    // Set the new empty space to the new car
    stack_arr[numCars] = new_car;
    //Update size of stack
    numCars++;
    return SUCCESS;
}

/**
 * \brief Pop a car from the stack
 *
 * This function will be called by client code to remove a car from the
 * top of the stack. It will return whether a car was removed successfully,
 * and the car removed in that case.
 *
 * The way this function returns the car is using the data out technique.
 * This is to get around the limitation that functions may only have one return
 * value. As such, the caller will pass in a pointer where the car
 * should be stored. Then this function will store the returned car at that
 * pointer. Independently, it returns whether it succeeded via the normal path.
 * 
 * Finally, set the pointer of the car being popped to 'NULL' and update numCars.
 *
 * If this function succeeds, it should return `SUCCESS` and modify `*data` to
 * be pointing to the removed car. If it fails, it should return `FAILURE`
 * and leave both the stack and `*data` unchanged. It should fail if and only if:
 * * [data] is `NULL`, or
 * * the stack is empty.
 *
 * \param[out] data Where to put the removed car
 * \return Whether a car was successfully removed
 */
int stack_pop(struct car_t *data) {
    // TODO

    // check precondidionts
    if (data == NULL) {
        return FAILURE;
    }
    if (numCars == 0) {
        return FAILURE;
    }
    
    // decrement numCars
    numCars--;

    // pop and save last element
    struct car_t* removeData = stack_arr[numCars];
    data = removeData;

    // free backing for last element
    free(removeData);

    return SUCCESS;
}

/**
 * \brief Destroy the whole stack
 *
 * This function will be called by client code to free the whole stack. This involves
 * freeing all elements associated with the stack: every pointer in the stack and every car
 * associated with the stack. Finally, you will set numCars to 0.
 * 
 * When this function succeeds, it should return `SUCCESS`.
 */
int destroy_stack(void) {
    // 
    
    while (numCars > 0) {
        numCars--;
        struct car_t* removedData = stack_arr(numCars);
        free(removedData);
    }

    return SUCCESS;
}

int main() {
    return 0;
}
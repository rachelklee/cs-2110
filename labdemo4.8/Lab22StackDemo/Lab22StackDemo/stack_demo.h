/**
 * \mainpage Stack Demo
 *
 * TODO: Write prompt
 */

#ifndef SD_H
#define SD_H

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>


/**
 * \brief Marks unused variables
 */
#define UNUSED(x) (void)(x)

/**
 * The return values for your functions
 * \property SUCCESS
 * \property FAILURE
 */
#define SUCCESS (0)
#define FAILURE (1)


/**
 * \brief All the colors a car is offered in
 * \see car_t
 */
enum color_t {
    RED, GREEN, BLUE, PURPLE, GREY, YELLOW, ORANGE
};

/**
 * \brief Car to be stored on the stack
 *
 * This type represents a car. A car has two properties: number of wheels and color
 */
struct car_t {
    int wheels;              ///< Number of Wheels in car
    enum color_t color;     ///< Color of car
};

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
extern int capacity;
extern int numCars;
extern struct car_t **stack_arr;

/**
 * \brief Provided function to initialize the stack
*/
void initialize_stack(void);

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
extern int stack_push(int wheels, enum color_t color);

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
extern int stack_pop(struct car_t *data);

/**
 * \brief Destroy the whole stack
 *
 * This function will be called by client code to free the whole stack. This involves
 * freeing all elements associated with the stack: every pointer in the stack and every car
 * associated with the stack. Finally, you will set numCars to 0.
 * 
 * When this function succeeds, it should return `SUCCESS`.
 */
extern int destroy_stack(void);

#endif // #ifndef SD_H

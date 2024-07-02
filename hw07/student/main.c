/**
 * @file main.c
 * @author YOUR NAME HERE
 * @brief testing and debugging functions written in my_string.c, oh_queue.c
 * @date 2023-03-xx
 */

// You may add and remove includes as necessary to help facilitate your testing
#include <stdio.h>
#include "my_string.c"
#include "pkmn_gym.c"

/** main
 *
 * @brief used for testing and debugging functions written in my_string.c and hw7.c
 *
 * @param void
 * @return 0 on success
 */
int main(void)
{

    // first trainer
    char *species1[] = {"pikachu", "pikachu", "charmander"};
    int levels1[] = {1, 1, 1};
    int numPokemon1 = 3;
    struct Pokemon party1[3];

    register_trainer("firstT");
    catch_pokemon(&gym.trainers[0], "pikachu", 1);
    catch_pokemon(&gym.trainers[0], "pikachu", 1);
    catch_pokemon(&gym.trainers[0], "charmander", 1);

    register_trainer("secondT");
    catch_pokemon(&gym.trainers[1], "pikachu", 1);
    catch_pokemon(&gym.trainers[1], "pikachu", 1);
    catch_pokemon(&gym.trainers[1], "pikachu", 1);

    count_species("pikachu");

    // // register_trainer("ollie");
    // register_trainer("bolobao");
    // register_trainer("hiesman");


    // // catch_pokemon()

    // struct Trainer hiesman;
    // struct Trainer test2;
    
    
    // catch_pokemon(&hiesman, "sloth", 5);
    // // catch_pokemon(&test2, "test2", 2);

    // // unregister_trainer(NULL);
    // // unregister_trainer("hiesman");

    // ------------------

    // char *richardsPokemonSpecies[] = {"pikachu", "squirtle"};
    // int richardsPokemonLevels[] = {5, 10};
    // int numPokemon = 2;
    // int numWins = 0;

    // struct Trainer richard;
    // struct Trainer test_richard;
    // struct Trainer bob;
    // struct Trainer testtrainer;


    // register_trainer("richard");
    // register_trainer("bob");
    // register_trainer("test_richard");
    // register_trainer("testtrainer");

    // // make two identical trainers
    // // construct_trainer("Richard", richardsPokemonSpecies, richardsPokemonLevels, numPokemon, numWins, &richard);
    // // construct_trainer("Richard", richardsPokemonSpecies, richardsPokemonLevels, numPokemon, numWins, &test_richard);

    // // try to catch a new pokemon
    // char *newPokemonSpecies = "snivy";
    // int newPokemonLvl = 15;

    // catch_pokemon(&gym.trainers[0], *richardsPokemonSpecies, *richardsPokemonLevels);
    // catch_pokemon(&gym.trainers[0], *((richardsPokemonSpecies) + 1), *((richardsPokemonLevels) + 1));

    // catch_pokemon(&gym.trainers[0], newPokemonSpecies, newPokemonLvl);

    // catch_pokemon(&gym.trainers[1], newPokemonSpecies, newPokemonLvl);

    // catch_pokemon(&gym.trainers[2], newPokemonSpecies, newPokemonLvl);

    // count_species("testSpecies");


    
    return 0;
}

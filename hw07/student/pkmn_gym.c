/**
 * @file pkmn_gym.c
 * @author YOUR NAME HERE
 *
 * @date 2024-03-xx
 */

// DO NOT MODIFY THE INCLUDE(S) LIST
#include "pkmn_gym.h"

// DO NOT MODIFY THE GLOBAL VARIABLE(S) LIST
struct Gym gym;

// ALWAYS check the validity of the passed in arguments
// (i.e. not NULL, in the correct range, not too long)

// If any of the parameters are invalid, the function should
// do nothing and return FAILURE.



/** register_trainer
 * @brief Add a new trainer to the gym with the provided parameters.
 * @param name the name of the trainer
 * @return FAILURE if parameters invalid or the gym is full, SUCCESS otherwise
*/
int register_trainer(const char *name) {
    // UNUSED_PARAM(name);

    if (name == NULL) {
        return FAILURE;
    }

    if (gym.num_trainers >= MAX_TRAINER_LENGTH) {
        return FAILURE;
    }

    if ((my_strlen(name) + 1) >= MAX_NAME_LENGTH) {
        return FAILURE;
    }

    my_strncpy(gym.trainers[gym.num_trainers].name, name, MAX_NAME_LENGTH - 1);
    gym.trainers[gym.num_trainers].name[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination

    // Initialize the new trainer's party size and number of wins
    gym.trainers[gym.num_trainers].party_size = 0;
    gym.trainers[gym.num_trainers].num_wins = 0;

    // Increment the number of trainers in the gym
    gym.num_trainers++;

    return SUCCESS;
}

/** unregister_trainer
 * @brief Removes a trainer from the gym.
 * @param name the name of the trainer
 * @return FAILURE if parameters invalid or the trainer is not found, SUCCESS otherwise
*/
int unregister_trainer(const char *name) {
    // UNUSED_PARAM(name);

    if (name == NULL) {
        return FAILURE;
    }

    if ((my_strlen(name) + 1) >= MAX_NAME_LENGTH) {
        return FAILURE;
    }

    int nameLen = (my_strlen(name) + 1);
    for (int i = 0; i < gym.num_trainers; ++i) {
        if (my_strncmp(gym.trainers[i].name, name, nameLen) == 0) {
            int j = 0;
            if (gym.num_trainers == 1) {
                my_strncpy(gym.trainers[0].name, "", 1);
                gym.num_trainers = 0;
                return SUCCESS;
            }
            for (j = i + 1; j <= gym.num_trainers; ++j) {
                gym.trainers[j - 1] = gym.trainers[j]; 
            }
            my_strncpy(gym.trainers[j].name, "", 1);
            --gym.num_trainers;
            return SUCCESS;
        }
    }

    return FAILURE;
}

/** catch_pokemon
 * @brief Creates a new Pokémon and adds it to
 * the trainer's party.
 * @param trainer pointer to the trainer who caught this Pokémon
 * @param species the species of the Pokémon caught
 * @param level the level of the Pokémon caught
 * @return FAILURE if parameters invalid or the trainer's party is full, SUCCESS otherwise
*/
int catch_pokemon(struct Trainer *trainer, const char *species, int level) {
    // UNUSED_PARAM(trainer);
    // UNUSED_PARAM(species);
    // UNUSED_PARAM(level);

    if (trainer == NULL || species == NULL) {
        return FAILURE;
    }

    if (trainer->party_size >= MAX_PARTY_LENGTH) {
        return FAILURE;
    }

    if (my_strlen(species) + 1 >= MAX_NAME_LENGTH) {
        return FAILURE;
    }

    if (level < 1 || level > 100) {
        return FAILURE;
    }

    struct Pokemon caughtPokemon;
    my_strncpy(caughtPokemon.species, species, (my_strlen(species) + 1));
    caughtPokemon.level = level;
    trainer->party[trainer->party_size] = caughtPokemon;
    ++(trainer->party_size);
    return SUCCESS;
}

/** release_pokemon
 * @brief Removes a Pokémon from a trainer's party.
 * @param trainer pointer to the trainer who is releasing a Pokémon
 * @param species the species of the Pokémon to release
 * @return FAILURE if parameters invalid or the Pokémon is not found in the trainer's party, SUCCESS otherwise
*/
int release_pokemon(struct Trainer *trainer, const char *species) {
    // UNUSED_PARAM(trainer);
    // UNUSED_PARAM(species);

    if (trainer == NULL) {
        return FAILURE;
    }

    if (species == NULL) {
        return FAILURE;
    }

    if ((my_strlen(species) + 1) >= MAX_NAME_LENGTH) {
        return FAILURE;
    }

    for (int i = 0; i < trainer->party_size; ++i) {
        if (my_strncmp(species, trainer->party[i].species, my_strlen((species) + 1)) == 0) {
            // for (i = i + 1; i < trainer->party_size; ++i) {
            //     trainer->party[i] = trainer->party[i + 1];
            // }

            while (i + 1 < trainer->party_size) {
                trainer->party[i] = trainer->party[i + 1];
                ++i;
            }
            --(trainer->party_size);
            return SUCCESS;
        }
    }
    
    return FAILURE;

    // --(trainer->party_size);
    // return SUCCESS;

    // for (int i = 0; i < gym.num_trainers; ++i) {
    //     if (my_strncmp(trainer->name, gym.trainers[i].name, my_strlen(trainer->name)) == 0) {
    //         for (int j = 0; j < gym.trainers[i].party_size; ++j) {
    //             if (my_strncmp(species, gym.trainers[i].party[j].species, my_strlen(species)) == 0) {
    //                 my_strncpy(gym.trainers[i].party[j].species, "", 1);
    //                 --(gym.trainers[i].party_size);
    //                 return SUCCESS;
    //             }
    //         }
    //     }
    // }

    // return FAILURE;
    
}

/** count_species
 * @brief Count the total number of Pokémon of a given species
 * out of the parties of every trainer in this gym.
 * @param species the name of the species to search for
 * @return FAILURE if parameters invalid, otherwise the number of Pokémon in the gym of the given species
*/
int count_species(const char *species) {
    // UNUSED_PARAM(species);

    if (species == NULL) {
        return FAILURE;
    }
    if ((my_strlen(species) + 1) > MAX_NAME_LENGTH) {
        return FAILURE;
    }
    
    int count = 0;
    for (int i = 0; i < gym.num_trainers; ++i) {
        for (int j = 0; j < gym.trainers[i].party_size; ++j) {
            if (my_strncmp(species, gym.trainers[i].party[j].species, (my_strlen(species) + 1)) == 0) {
                ++count;
            }
        }
    }

    return count;
}

/** trade_pokemon
 * @brief Trade two Pokémon between two trainers,
 * swapping their positions in their parties.
 * @param t0 pointer to the first trainer in the trade
 * @param party_index_0 the index of the Pokémon to trade in the first trainer's party
 * @param t1 pointer to the second trainer in the trade
 * @param party_index_1 the index of the Pokémon to trade in the second trainer's party
 * @return FAILURE if 
 *                  parameters invalid,
 *                  both Trainers are the same,
 *                  party_index_0 or party_index_1 do not point to pokemon,
 *         SUCCESS otherwise
*/
int trade_pokemon(struct Trainer *t0, int party_index_0, struct Trainer *t1, int party_index_1) {
    // UNUSED_PARAM(t0);
    // UNUSED_PARAM(party_index_0);
    // UNUSED_PARAM(t1);
    // UNUSED_PARAM(party_index_1);

    if (t0 == NULL) {
        return FAILURE;
    }
    if (t1 == NULL) {
        return FAILURE;
    }
    if (party_index_0 < 0 || party_index_1 < 0) {
        return FAILURE;
    }
    if (t0 == t1) {
        return FAILURE;
    }

    int t0idx = 0;
    int t1idx = 0;
    for (int i = 0; i < gym.num_trainers; ++i) {
        if (my_strncmp(t0->name, gym.trainers[i].name, (my_strlen(t0->name) + 1)) == 0) {
            t0idx = i;
            if (party_index_0 >= gym.trainers[i].party_size) {
                return FAILURE;
            }
        }

        if (my_strncmp(t1->name, gym.trainers[i].name, (my_strlen(t1->name) + 1)) == 0) {
            t1idx = i;
            if (party_index_1 >= gym.trainers[i].party_size) {
                return FAILURE;
            }
        }
    }

    struct Pokemon t0pokemon = gym.trainers[t0idx].party[party_index_0];
    struct Pokemon t1pokemon = gym.trainers[t1idx].party[party_index_1];

    gym.trainers[t0idx].party[party_index_0] = t1pokemon;
    gym.trainers[t1idx].party[party_index_1] = t0pokemon;

    return SUCCESS;
}

/** battle_trainer
 * @brief Start a battle between two trainers.
 * 
 * The num_wins variable of the winner of this battle should
 * also be incremented to reflect the trainer's win.
 * 
 * @param challenger pointer to the trainer who initiated the battle
 * @param opponent pointer to the trainer who was initiated by the challenger
 * @return FAILURE if parameters invalid, 0 if the challenger wins, 1 if the opponent wins
*/
int battle_trainer(struct Trainer *challenger, struct Trainer *opponent) {
    UNUSED_PARAM(challenger);
    UNUSED_PARAM(opponent);


    
    if (challenger == NULL) {
        return FAILURE;
    }
    if (opponent == NULL) {
        return FAILURE;
    }

    int cWins = 0;
    int oWins = 0;

    int maxArray = 0;
    if (challenger->party_size >= opponent->party_size) {
        maxArray = challenger->party_size;
    } else if (challenger->party_size < opponent->party_size) {
        maxArray = opponent->party_size;
    }

    for (int i = 0; i < maxArray; ++i) {
        if (challenger->party[i].species == NULL) {
            ++oWins;
            // ++(opponent->num_wins);
        } else if (opponent->party[i].species == NULL) {
            ++cWins;
            // ++(challenger->num_wins);
        }

        if (opponent->party[i].level > challenger->party[i].level) {
            ++oWins;
            // ++(opponent->num_wins);
        } else if (opponent->party[i].level < challenger->party[i].level) {
            ++cWins;
            // ++(challenger->num_wins);
        } else {
            continue;
        }
    }
    
    if (cWins > oWins) {
        ++(challenger->num_wins);
        return 0;
    }
    ++(opponent->num_wins);
    return 1;
    

    

    // int cIdx = 0;
    // int oIdx = 0;


    // for (int i = 0; i < gym.num_trainers; ++i) {
    //     // get index for each trainer in gym.trainers[]
    //     if (my_strncmp(challenger->name, gym.trainers[i].name, (my_strlen(challenger->name) + 1)) == 0) {
    //         cIdx = i;
    //     }

    //     if (my_strncmp(opponent->name, gym.trainers[i].name, (my_strlen(opponent->name) + 1)) == 0) {
    //         oIdx = i;
    //     }
    // }

    // // get maxPokemon
    // int maxPokemon = 0;
    // if (gym.trainers[cIdx].party_size >= gym.trainers[oIdx].party_size) {
    //     maxPokemon = gym.trainers[cIdx].party_size;
    // } else {
    //     maxPokemon = gym.trainers[oIdx].party_size;
    // }

    // for (int i = 0; i < maxPokemon; ++i) {
    //     // make sure pokemon exists at index for both trainers, if not automatic loss if no pokemon
    //     if (gym.trainers[cIdx].party[i].species == NULL) {
    //         ++oWins;
    //         ++(gym.trainers[oIdx].num_wins);
    //     } 
    //     if (gym.trainers[oIdx].party[i].species == NULL) {
    //         ++cWins;
    //         ++(gym.trainers[cIdx].num_wins);
    //     }

    //     // compare if pokemon exist at both indices exist
    //     if (gym.trainers[cIdx].party[i].level > gym.trainers[oIdx].party[i].level) {
    //         ++cWins;
    //         ++(gym.trainers[cIdx].num_wins);
    //     } else if (gym.trainers[cIdx].party[i].level < gym.trainers[oIdx].party[i].level) {
    //         ++oWins;
    //         ++(gym.trainers[oIdx].num_wins);   
    //     } else {
    //         continue;
    //     }
    // }

    
}

/** find_champion
 * @brief Find the CHAMPION!!! (i.e., the trainer in this gym with the most wins.)
 * If several trainers have the maximum number of wins, pick the first in the gym trainer array with
 *  the maximum wins.
 * @return a pointer to the Trainer with the most wins, NULL is the gym has no Trainers in it
*/
struct Trainer *find_champion(void) {
    if (gym.num_trainers == 0) {
        return NULL;
    }

    int winCount = gym.trainers[0].num_wins;
    
    struct Trainer *mostWins = &(gym.trainers[0]);

    for (int i = 1; i < gym.num_trainers; ++i) {
        if (gym.trainers[i].num_wins > winCount) {
            mostWins = &(gym.trainers[i]);
            winCount = gym.trainers[i].num_wins;
        }
    }

    return mostWins;
}
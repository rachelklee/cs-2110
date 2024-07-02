/**
 * CS 2110 - Spring 2024 - Homework #9
 *
 * @author Rachel Lee
 *
 * slack.c: Complete the functions!
 */

/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!-IMPORTANT-!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * For any function that must use malloc, if malloc returns NULL, the function
 * itself should return NULL if needs to return a pointer (or return FAILURE if
 * the function returns a int).
 */

// Do not add ANY additional includes!!!
#include "slack.h"

/* You should NOT have any global variables. */

/** createChannel
 *
 * Creates a new channel. 
 * Initially: 
 *      - The LinkedList should have a size of 0.
 *      - The accounts array should be NULL.
 *      - numUsers should be 0.
 * 
 * @return A pointer to the Channel if successful, NULL if unsuccessful.
 */
Channel *createChannel(void) {
  Channel *newChannel = malloc(sizeof(Channel));

  // if malloc fails
  if (newChannel == NULL) {
    return NULL;
  }

  // initialize LinkedList
  newChannel->posts.head = NULL;
  newChannel->posts.size = 0;

  // initialize users affay
  newChannel->users = NULL;
  newChannel->numUsers = 0;

  return newChannel;
}

/** createAccount
 *
 * Creates a new account with the provided username and accountID. 
 * 
 * Make sure to add the account to the channel. Remember, if this is
 * the first account, the account array will be NULL.
 * 
 * You will have to resize the accounts array to add the new account
 * to the channel. Think about what function you can use to help you
 * with this.
 * 
 * All accountID's must be unique. If the given accountID already exists
 * in the channel, return FAILURE. You may find it helpful to implement
 * findUser() first.
 * 
 * @param channel A pointer to the channel you are adding the account to
 * @param username A pointer to a string representing the name of the account
 * @param accountID An integer representing the ID of the account
 * @return FAILURE if 
 *          - any of the parameters are NULL,
 *          - the accountID is not unique, or
 *          - if malloc fails. 
 * SUCCESS if successful.
 */
int createAccount(Channel *channel, const char *username, int accountID) {
  // check for fail cases
  if (channel == NULL || username == NULL) {
    return FAILURE;
  }

  if (findUser(channel, accountID) != NULL) {
    return FAILURE;
  }

  // allocate memory for new account
  Account *newAccount = malloc(sizeof(Account));
  
  if (newAccount == NULL) {
    return FAILURE;
  }

  // allocate memory for username
  newAccount->username = malloc((strlen(username) + 1));
  if (newAccount->username == NULL) {
    free(newAccount);
    return FAILURE;
  }
  strcpy(newAccount->username, username);

  // set accountid of new account
  newAccount->accountID = accountID;

  // reallocate memory
  Account **tempAccount = (Account**) realloc(channel->users, (channel->numUsers + 1) * sizeof(Account**));
  if (tempAccount == NULL) {
    free(newAccount->username);
    free(newAccount);
    return FAILURE;
  }
  
  channel->numUsers = channel->numUsers + 1;
  channel->users = tempAccount;
  channel->users[channel->numUsers - 1] = newAccount;

  return SUCCESS;

  // UNUSED(channel);
  // UNUSED(username);
  // UNUSED(accountID);
}

/** createPost
 *
 * Creates a new post with the provided text, sender ID, channel ID, and post ID.
 * Remember to add the post to the back of the LinkedList of posts in the 
 * provided channel and increment the size of the LinkedList accordingly.
 * 
 * There can be a maximum of 10 reactions on a single post (MAX_REACTION_NUM).
 * By default, no reactions are on a post.
 * 
 * All postID's must be unique. If the given postID already exists
 * in the channel, return FAILURE. You may find it helpful to implement
 * findPost() first.
 * 
 * Additionally, you must ensure that the given senderID is associated with
 * an account in the channel. You may find it helpful to implement findUser()
 * first.
 *
 * @param channel A pointer to the channel where the post will be created
 * @param text A pointer to a string representing the content of the post
 * @param senderID An integer representing the ID of the account that sent the post
 * @param postID An integer representing the unique ID of the post
 * @return FAILURE if 
 *          - any of the parameters are NULL,
 *          - the senderID is not an account in the channel,
 *          - the postID is not unique, or
 *          - if malloc fails. 
 * SUCCESS if successful.
 */
int createPost(Channel *channel, const char *text, int senderID, int postID) {
  UNUSED(channel);
  UNUSED(text);
  UNUSED(senderID);
  UNUSED(postID);
  
  if (channel == NULL || text == NULL) {
    return FAILURE;
  }

  if (findUser(channel, senderID) == NULL) {
    return FAILURE;
  }

  if (findPost(channel, postID) != NULL) {
    return FAILURE;
  }

  Post *newPost = malloc(sizeof(Post));

  if (newPost == NULL) {
    return FAILURE;
  }

  newPost->text = malloc(strlen(text) + 1);
  if (newPost->text == NULL) {
    free(newPost);
    return FAILURE;
  }
  strcpy(newPost->text, text);

  newPost->postID = postID;
  newPost->senderID = senderID;
  newPost->numReactions = 0;

  Node *newNode = malloc(sizeof(Node));
  if (newNode == NULL) {
      free(newPost->text);
      free(newPost);
      return FAILURE;
  }
  newNode->data = newPost;
  newNode->next = NULL;

  if (channel->posts.head == NULL) {
    channel->posts.head = newNode;
  } else {
      Node *current = channel->posts.head;
      while (current->next != NULL) {
          current = current->next;
      }
      current->next = newNode;
  }

  channel->posts.size = channel->posts.size + 1;

  return SUCCESS;
}

/** addReaction
 *
 * Adds a reaction to the specified post. Remember to increment numReactions
 * in the provided post accordingly and add it to the reactions array.
 * 
 * Hint: Do you need to malloc?
 * 
 * You may assume that the accountID is valid for this method. However,
 * we are declaring that a user can only react once to a post. So it is upon
 * you to enforce this constraint. For example, if the user has already reacted
 * with HEART, they cannot also react with SKULL.
 * 
 * Additionally, a post can have a maximum of 10 reactions.
 *
 * @param post A pointer to the post to which the reaction will be added
 * @param accountID An integer representing the ID of the account that reacted to the post
 * @param reaction An enum value representing the type of reaction
 * @return FAILURE if 
 *          - any of the parameters are NULL,
 *          - the post cannot have more reactions, or
 *          - the user has already reacted to the post
 * SUCCESS if successful.
 */
int addReaction(Post *post, int accountID, enum ReactionType reaction) {
  UNUSED(post);
  UNUSED(accountID);
  UNUSED(reaction);

  if (post == NULL) {
    return FAILURE;
  }

  if (post->numReactions == 10) {
    return FAILURE;
  }

  for (int i = 0; i < post->numReactions; ++i) {
    if (post->reactions[i].userID == accountID) {
      return FAILURE;
    }
  }

  post->reactions[post->numReactions].userID = accountID;
  post->reactions[post->numReactions].reaction = reaction;

  post->numReactions = post->numReactions + 1;

  return SUCCESS;
}

/** findUser
 *
 * Finds and returns the user with the specified userID within the
 * given channel.
 *
 * @param channel A pointer to the channel where the post will be searched
 * @param accountID An integer representing the ID of the user to be found
 * @return A pointer to the found user, or NULL if the channel is NULL or
 * no user with the given ID exists.
 */
Account* findUser(Channel *channel, int accountID) {
  if (channel == NULL) {
    return NULL;
  }

  for (int i = 0; i < channel->numUsers; ++i) {
    if (channel->users[i]->accountID == accountID) {
      return channel->users[i];
    }
  }

  return NULL;

  // UNUSED(channel);
  // UNUSED(accountID);
}

/** findPost
 *
 * Finds and returns the post with the specified postID within the given channel.
 *
 * @param channel A pointer to the channel where the post will be searched
 * @param postID An integer representing the ID of the post to be found
 * @return A pointer to the found post, or NULL if the channel is NULL or
 * no post with the given ID exists.
 */
Post *findPost(Channel *channel, int postID) {
  if (channel == NULL) {
    return NULL;
  }

  Node* current = channel->posts.head;

  while(current != NULL) {
    if (current->data->postID == postID) {
      return current->data;
    }
    current = current->next;
  }  

  // UNUSED(channel);
  // UNUSED(postID);
  return NULL;
}

/** searchForPalindrome
 *
 * Searches for the first post that is a palindrome. The text of the
 * post must be a case-sensitive palindrome (including spaces and all). 
 * 
 * Ex:  "evil rats on no star live" -> good
 *      "Evil rats on no star live" -> nope (Capitalization differs)
 *      "was it a car or a cat I saw" -> bad (the spaces ruin the palindrome)
 *      "Can I be a palindrome" -> no
 *      "rise to vote, sir" -> nope again (spaces and punctuation)
 * 
 * Return a pointer to the post that contains the palindrome. Also, set
 * the out variable names index to contain the index of the post in the
 * linked list.
 *
 * @param channel An pointer to the channel to search
 * @param index A pointer to a int that should be set to the index of the post
 *              in the Linked List. If no palindromic post is found, do not touch.
 * @return A pointer to the found post, or NULL if no palindromic post was found
 *         or if any of the parameters are NULL.
 *         Also, set the index out variable appropriately.
 */
Post *searchForPalindrome(Channel *channel, int* index) {
  if (channel == NULL || index == NULL) {
    return NULL;
  }

  Node *currentPost = channel->posts.head;
  int postIndex = 0;

  while (currentPost != NULL) {
    char *currentText = currentPost->data->text;
    int currLength = strlen(currentText);
    int isPalindrome = 1;

    for (int i = 0; i < currLength / 2; ++i) {
      if (currentText[i] != currentText[currLength - i - 1]) {
        isPalindrome = 0;
        break;
      }
    }

    if (isPalindrome == 1) {
      *index = postIndex;
      return currentPost->data;
    }

    currentPost = currentPost->next;
    ++postIndex;
  }

  return NULL;
}

/** deleteReaction
 *
 * Deletes a reaction from the specified post. Remember to decrement numReactions
 * and remove it from the reactions array in the provided post.
 * 
 * Make sure when you are deleting from an array, you shift the remaining elements
 * towards the beginning of the array, if needed.
 *
 * @param post A pointer to the post from which the reaction will be deleted
 * @param accountID An integer representing the ID of the user whose reaction will be deleted
 * @param reaction An enum value representing the type of reaction to be deleted
 * @return FAILURE if the post is NULL or if the reaction isn't found, SUCCESS if successful.
 */
int deleteReaction(Post *post, int accountID, enum ReactionType reaction) {
  if (post == NULL) {
    return FAILURE;
  }

  for (int i = 0; i < post->numReactions; ++i) {
    if (post->reactions[i].userID == accountID && post->reactions[i].reaction == reaction) {
      // desired reactioh=n found, remove it

      for (int j = i; j < post->numReactions - 1; ++j) {
        post->reactions[j] = post->reactions[j + 1];
      }
      
      post->numReactions = post->numReactions - 1;
      return SUCCESS;
       
    }
  }

  // UNUSED(post);
  // UNUSED(accountID);
  // UNUSED(reaction);
  return FAILURE;
}

/** deletePost
 *
 * Deletes the post with the specified post ID from the given channel. Remember
 * to remove the post of the LinkedList of posts in the provided channel, free any structs/
 * data structures, and decrement numPosts accordingly.
 *
 * @param channel A pointer to the channel where the post is located
 * @param postID An integer representing the ID of the post to be deleted
 * @return FAILURE if the channel is NULL or if the post isn't found, SUCCESS if successful.
 */
int deletePost(Channel *channel, int postID) {
  if (channel == NULL) {
    return FAILURE;
  }

  Node *current = channel->posts.head;
  Node *prev = NULL;

  while (current != NULL) {
    if (current->data->postID == postID) {
      // post to delete found
      // prev->next = current->next;
      if (prev == NULL) {
        channel->posts.head = current->next;
      } else {
        prev->next = current->next;
      }

      free(current->data->text);
      free(current->data);
      free(current);

      channel->posts.size = channel->posts.size - 1;
      return SUCCESS;
    }

    // increment current
    prev = current;
    current = current->next;
  }

  UNUSED(channel);
  UNUSED(postID);
  return FAILURE;
}

/** deleteAccount
 *
 * Deletes the account with the specified accountID. Make sure to remove it from the accounts
 * array in the channel and decrement numAccounts accordingly. 
 * 
 * Make sure when you are deleting from an array, you shift the remaining elements, 
 * if needed.
 * 
 * When an account is deleted, all of that user's posts and reactions should be 
 * deleted as well.
 *
 * @param channel A pointer to the channel you are deleting the account from
 * @param accountID An integer representing the ID of the account to be deleted
 * @return FAILURE if the channel is NULL or if the account isn't found, SUCCESS if successful.
 */
int deleteAccount(Channel *channel, int accountID) {
  if (channel == NULL) {
    return FAILURE;
  }

  int found = 0;
  for (int i = 0; i < channel->numUsers; ++i) {
    if (channel->users[i]->accountID == accountID) {
      // account found, delete user
      free(channel->users[i]->username);
      free(channel->users[i]);
      for (int j = i; j < channel->numUsers - 1; ++j) {
        channel->users[j] = channel->users[j + 1];
      }
      channel->numUsers = channel->numUsers - 1;
      found = 1;
      break;
    }
  }

  if (!found) {
    return FAILURE;
  }

  Node *current = channel->posts.head;

  while (current != NULL) {
    Node *temp = current->next;
    for (int k = 0; k < current->data->numReactions; ++k) {
      if (current->data->reactions[k].userID == accountID) {
        deleteReaction(current->data, accountID, current->data->reactions[k].reaction);
      }
    }

    if (current->data->senderID == accountID) {
      deletePost(channel, current->data->postID);
    }

    current = temp;
  }
  return SUCCESS;

  UNUSED(channel);
  UNUSED(accountID);
}

/** deleteChannel
 *
 * Deletes the channel that is passed in. Make sure to free any structs/
 * data structures contained in the channel.
 * 
 * Do nothing if the channel is NULL.
 * 
 * @param channel A pointer to the channel that you are deleting
 */
void deleteChannel(Channel *channel) {
  if (channel == NULL) {
    return;
  }

  Node *current = channel->posts.head;
  while (current != NULL) {
    Node *temp = current->next;
    free(current->data->text);
    free(current->data);
    free(current);

    current = temp;
  }

  for (int i = 0; i < channel->numUsers; ++i) {
    free(channel->users[i]->username);
    free(channel->users[i]);
  }
  free(channel->users);
  free(channel);

  UNUSED(channel);
}

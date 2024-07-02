#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This is just to remove compiler warnings associated with unused variables.
// Delete calls to this as you implement functions.
#define UNUSED(x) ((void)(x))

#define SUCCESS 0
#define FAILURE -1

#define MAX_REACTION_NUM 10

// Enum for reaction types
enum ReactionType {
    SMILEY,
    THUMBS_UP,
    HEART,
    SKULL,
    SOBBING
};

// Struct for reactions
typedef struct reaction {
    int userID;
    enum ReactionType reaction;
} Reaction;

// Struct for posts
typedef struct post {
    int postID;
    int senderID;
    char* text;
    Reaction reactions[MAX_REACTION_NUM];
    int numReactions;
} Post;

// Struct for accounts
typedef struct account {
    char* username;
    int accountID;
} Account;

typedef struct node {
    struct node* next; 
    Post* data;
} Node;

typedef struct linked_list {
    Node *head; // Head pointer either points to a node with data or NULL (if the linked list is empty).
    int size;   // number of nodes in the linked list
} LinkedList;

// Struct for channels
typedef struct channel {
    LinkedList posts;
    Account** users;
    int numUsers;
} Channel;

// Function prototypes

Channel *createChannel(void);
int createAccount(Channel *channel, const char *username, int accountID);
int createPost(Channel *channel, const char *text, int senderID, int postID);
int addReaction(Post *post, int accountID, enum ReactionType reaction);

Account* findUser(Channel *channel, int accountID);
Post *findPost(Channel *channel, int postID);
Post *searchForPalindrome(Channel *channel, int* index);

int deleteReaction(Post *post, int accountID, enum ReactionType reaction);
int deletePost(Channel *channel, int postID);
int deleteAccount(Channel *channel, int accountID);
void deleteChannel(Channel *channel);


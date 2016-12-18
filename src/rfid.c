#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "rfid.h"

card_t *head = NULL;

void rfid_print_card(const card_t *card) {
    for (uint8_t i = 0; i < card->uid_size; i++) {
        printf("%02X", card->uid[i]);
    }
    printf(": %s", card->user);
}

card_t* rfid_find_card(const card_t *card)
{
    if (head != NULL) {
        card_t *current;
        current = head;
        while (current != NULL) {
            if ((current->uid_size != card->uid_size) ||
                !memcmp(current->uid, card->uid, current->uid_size) ||
                !strcmp(current->user, card->user)) {
                
                return current;
            }
            current = current->next;
        }
    }
    return NULL;
}

void rfid_add_card(const card_t *card)
{
    card_t *found_card = rfid_find_card(card);
    if (found_card) {
        printf("Found card \"");
        rfid_print_card(found_card);
        printf("\", not adding it again.\n");
        return;
    }
    
    // Card doesn't exist, add it
    card_t *new_card;
    char *new_card_user;
    new_card = malloc(sizeof(card_t));
    new_card_user = malloc(strlen(card->user)+1);
    if (!new_card || !new_card_user) {
        printf_P(PSTR("Out of memory. Please remove cards.\n"));
        return;
    }
    
    // Copy card data 
    new_card->uid_size = card->uid_size;
    memcpy(new_card->uid, card->uid, card->uid_size);
    strcpy(new_card_user, card->user);
    new_card->user = new_card_user;
    new_card->next = NULL;

    // Update card list
    if (head == NULL) {
        head = new_card;
    } else {
        card_t *current;
        current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_card;
    }
    return;
}

void rfid_list_cards(void) {
    if (head == NULL) {
        printf_P(PSTR("No cards added\n"));
    } else {
        card_t *current;
        current = head;
        while (current->next != NULL) {
            rfid_print_card(current);
            printf("\n");
            current = current->next;
        }
        rfid_print_card(current);
        printf("\n");
    }
}

void rfid_remove_card_by_user(const char *user) {
    card_t *curr;
    card_t *prev;
    curr = head;
    prev = NULL;
    if (head == NULL) {
        printf_P(PSTR("No cards added\n"));
        return;
    } else {
        while (curr->next != NULL) {
            if (strcmp(curr->user, user) == 0) {
                break;
            }
            prev = curr;
            curr = curr->next;
        }
    }
    if (strcmp(curr->user, user) == 0) {
        if (prev == NULL && curr->next == NULL) {
            // this is the single card in the list
            free(curr->user);
            free(curr);
            head = NULL;
        } else if (prev == NULL && curr->next != NULL) {
            // this is the first card in the list, with more after it
            head = curr->next;
            free(curr->user);
            free(curr);
        } else if (prev != NULL && curr->next != NULL) {
            // this card is somewhere in the middle of the list
            prev->next = curr->next;
            free(curr->user);
            free(curr);
        } else if(prev != NULL && curr->next == NULL) {
            // this is the last card in the list
            prev->next = NULL;
            free(curr->user);
            free(curr);
        } else {
            fprintf_P(stderr, PSTR("Invalid situation when removing card\n"));
        }
    } else {
        
        printf_P(PSTR("Card not found\n"));
    }
}

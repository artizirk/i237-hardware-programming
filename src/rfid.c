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
        rfid_print_card(found_card);
        printf("\n");
        return;
    }
    
    // Card doesn't exist, add it
    card_t *new_card;
    char *new_card_user;
    new_card = malloc(sizeof(card_t));
    new_card_user = malloc(strlen(card->user)+1);
    if (!new_card || !new_card_user) {
        printf(PSTR("Out of memory. Please remove cards.\n"));
        return;
    }
    
    // Copy card data 
    new_card->uid_size = card->uid_size;
    memcpy(new_card->uid, card->uid, card->uid_size);
    strcpy(new_card_user, card->user);
    new_card->user = new_card_user;

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
        printf_P(PSTR("No cards added"));
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




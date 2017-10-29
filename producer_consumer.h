#ifndef PRODUCER_CONSUMER_H
#define PRODUCER_CONSUMER_H

extern unsigned int put_index; // index in the buffer where the 
                               // producer should insert an elt.

extern unsigned int remove_index;// index in the buffer where the 
                                 // consumer should get an elt.

void *producer_routine(void *args);
void *consumer_routine(void *args);

// routines for the buffer.
void put_elt(int);  // put an elt in the buffer.
void remove_elt(void); // print an elt in the buffer.

#endif

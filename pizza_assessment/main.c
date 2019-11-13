#include <stdio.h>

#include <pthread.h>

#include <unistd.h>

#include <stdbool.h>


int pizza_count = 0;
int tray_size = 5;

pthread_cond_t tray_not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t tray_not_empty = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex;

void * produce_pizza(void * arg) {
    while (1) {
        pthread_mutex_lock( & mutex);
        if (pizza_count >= tray_size) {

            pthread_cond_wait( & tray_not_full, & mutex);

        }
        sleep(1);

        pizza_count += 1;
        printf("Chef produced pizza: %d \n", pizza_count);
        pthread_mutex_unlock( & mutex);
        pthread_cond_signal( & tray_not_empty);

    }
    return NULL;
}

void * eat_pizza(void * arg) {
    while (1) {
        pthread_mutex_lock( & mutex);
        if (pizza_count < 1) {
            pthread_cond_wait( & tray_not_empty, & mutex);
        }
        sleep(1);

        pizza_count -= 1;

        printf("Client ate pizza: %d \n", pizza_count + 1);
        pthread_mutex_unlock( & mutex);
        pthread_cond_signal( & tray_not_full);
    }

    return NULL;
}

int main() {

    pthread_t pid, eid;

    pthread_create( & pid, NULL, produce_pizza, NULL);

    pthread_create( & eid, NULL, eat_pizza, NULL);

    pthread_join(pid, NULL);
    pthread_join(eid, NULL);
}
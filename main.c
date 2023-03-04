#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_TRAINS 100
struct timespec ts;

int N;
sem_t station;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int waiting_trains = 0;

void *train(void *arg) {
    int train_id = *(int *)arg;
    printf("Train %d arrived at the station.\n", train_id);

    pthread_mutex_lock(&mutex);
    waiting_trains++;
    pthread_mutex_unlock(&mutex);

    sem_wait(&station);

    pthread_mutex_lock(&mutex);
    waiting_trains--;
    printf("Train %d entered the station.\n", train_id);
    pthread_mutex_unlock(&mutex);

    ts.tv_sec = rand() % 5 + 1;
    ts.tv_nsec = 0;
    nanosleep(&ts, NULL);

    printf("Train %d left the station.\n", train_id);
    sem_post(&station);

    return NULL;
}

int main() {
    int N;
    printf("Enter the number of tracks at the station: ");
    scanf("%d", &N);
    sem_init(&station, 0, N);

    pthread_t trains[MAX_TRAINS];
    int train_ids[MAX_TRAINS];

    for (int i = 0; i < MAX_TRAINS; i++) {
        train_ids[i] = i;
        pthread_create(&trains[i], NULL, train, &train_ids[i]);
        ts.tv_sec = rand() % 3;
        ts.tv_nsec = 0;
        nanosleep(&ts, NULL);
    }

    for (int i = 0; i < MAX_TRAINS; i++) {
        pthread_join(trains[i], NULL);
    }

    sem_destroy(&station);

    return 0;
}
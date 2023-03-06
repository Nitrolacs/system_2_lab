/*! Код основного процесса программы */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define CORRECT_INPUT 2
#define END_STRING '\n'
#define MAX_TRAINS 10
struct timespec ts;

sem_t station;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int waitingTrains = 0;

int CheckingInput(const char message[], int lowerBound, int count)
{
    int userInput = 1;
    char inputChar = '\0';

    printf("%s", message);
    int input = scanf("%d%c", &userInput, &inputChar);

    while (input != CORRECT_INPUT || inputChar != END_STRING
           || userInput < lowerBound || userInput > count)
    {
        if (userInput >= lowerBound && userInput <= count)
        {
            while ((inputChar = getchar()) != '\n');
        }
        userInput = 1;
        printf("Неверный ввод. Попробуйте снова.\n%s", message);
        input = scanf("%d%c", &userInput, &inputChar);
    }
    return userInput;
}

void* train(void* arg) {
    int trainId = *(int*)arg + 1;
    printf("\033[1;32mПоезд №%d прибывает на станцию.\033[0m\n",
           trainId);

    // Используем мьютекс для обеспечения взаимного исключения.
    pthread_mutex_lock(&mutex);
    waitingTrains++;
    pthread_mutex_unlock(&mutex);

    // Поезд ждет на семафоре station, который позволяет ему войти на
    // станцию, если есть свободный путь
    sem_wait(&station);

    // Оказавшись на станции, поезд уменьшает счетчик waiting_trains и
    // выводит сообщение о том, что он вошел на станцию.
    pthread_mutex_lock(&mutex);
    waitingTrains--;
    printf("\033[1;34mПоезд №%d занимает путь.\033[0m\n", trainId);
    pthread_mutex_unlock(&mutex);

    ts.tv_sec = rand() % 5 + 3;
    ts.tv_nsec = 0;
    // Поезд "спит" случайное время для имитации загрузки и выгрузки
    // пассажиров перед тем как покинуть станцию.
    nanosleep(&ts, NULL);

    printf("\033[1;31mПоезд №%d покидает станцию.\033[0m\n", trainId);
    // Опубликовываем в семафоре station, указывая, что этот путь доступен.
    sem_post(&station);

    return NULL;
}

int main() {
    int numberTracks, lowerBound = 1;
    const char message[] = "Введите количество путей: ";

    printf("---------------------------\n");
    printf("| Железнодорожная станция |\n");
    printf("---------------------------\n");

    numberTracks = CheckingInput(message, lowerBound, MAX_TRAINS);

    sem_init(&station, 0, numberTracks);

    // Идентификаторы потоков поездов, которые будут созданы.
    pthread_t trains[MAX_TRAINS];
    // Целочисленные значения, представляющие идентификаторы каждого поезда.
    int trainIds[MAX_TRAINS];

    while (!0)
    {
        for (int i = 0; i < MAX_TRAINS; i++) {
            // Присваиваем поезду уникальный идентификатор.
            trainIds[i] = i;
            // Создаём новый поток для поезда.
            // Передаваемые аргументы: адрес элемента в массиве trains(чтобы
            // сохранить идентификатор потока), NULL (для атрибутов по умолчанию),
            // указатель на функцию train и адрес элемента в массиве trainIds
            // (чтобы передать идентификатор поезда в функцию train).
            pthread_create(&trains[i], NULL, train,
                           &trainIds[i]);

            ts.tv_sec = rand() % 3;
            ts.tv_nsec = 0;
            // Приостанавливаем выполнение программы на случайное время.
            nanosleep(&ts, NULL);
        }

        // Основной поток будет ждать завершения всех потоков поездов перед тем
        // как продолжить выполнение. Это важно чтобы убедиться что все ресурсы
        // (например семафор station) освобождаются корректно перед завершением
        // программы.
        for (int i = 0; i < MAX_TRAINS; i++) {
            pthread_join(trains[i], NULL);
        }

        // Уничтожаем семафор station.
        sem_destroy(&station);

        printf("Повторить выполнение программы? (введите y "
               "для этого): ");
        char choice;
        scanf(" %c", &choice);

        if (choice != 'y' && choice != 'Y')
        {
            break;
        }
    }

    return 0;
}
// 请注意这只是一个单独演示，要加入wmkc_thread库中请根据需要进行增删。
#include <wmkc_conf.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int condition = 0;

void *thread_func(void *arg) {
    pthread_mutex_lock(&mutex);
    while (condition == 0) {
        pthread_cond_wait(&cond, &mutex);
    }
    printf("Thread is now running.\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t thread;
    pthread_create(&thread, NULL, thread_func, NULL);

    sleep(1);
    printf("Main thread is now signaling the condition.\n");

    pthread_mutex_lock(&mutex);
    condition = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    pthread_join(thread, NULL);
    return 0;
}

#ifndef POSIX_EVENT_H
#define POSIX_EVENT_H

typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    int             signaled;
} posix_event_t;

inline void posix_event_init(posix_event_t* evt)
{
    pthread_mutex_init(&evt->mutex, NULL);
    pthread_cond_init(&evt->cond, NULL);
    evt->signaled = 0;
}

inline void posix_event_set(posix_event_t* evt)
{
    pthread_mutex_lock(&evt->mutex);
    evt->signaled = 1;
    pthread_cond_broadcast(&evt->cond);  // wake all waiting threads
    pthread_mutex_unlock(&evt->mutex);
}

inline void posix_event_reset(posix_event_t* evt)
{
    pthread_mutex_lock(&evt->mutex);
    evt->signaled = 0;
    pthread_mutex_unlock(&evt->mutex);
}

inline void posix_event_wait(posix_event_t* evt)
{
    pthread_mutex_lock(&evt->mutex);
    while (!evt->signaled)
    {
        pthread_cond_wait(&evt->cond, &evt->mutex);
    }
    evt->signaled = 0;
    pthread_mutex_unlock(&evt->mutex);
}

#endif
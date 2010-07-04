#include <stdio.h>
#include <pthread.h>

const int MAX_COUNT = 5;

typedef enum {PING_INIT, PING_READY, PONG_READY} READY;
READY g_ready = PING_INIT;
pthread_mutex_t thr_cond_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t thr_cond = PTHREAD_COND_INITIALIZER;

void * thread_ping (void * arg)
{
 int i = 0;

 /* wait for pong */ 
 pthread_mutex_lock(&thr_cond_lock);
 g_ready = PING_READY;
// printf("ping is waiting\n");
 pthread_cond_wait(&thr_cond, &thr_cond_lock);

 while(1) { 
  printf("ping(%d) -> ", ++i);
  pthread_cond_signal(&thr_cond);
  if (i == MAX_COUNT) {
   pthread_mutex_unlock(&thr_cond_lock);
   break;
  }
  else
   pthread_cond_wait(&thr_cond, &thr_cond_lock);
 }

 pthread_exit((void *)1); 
}

void * thread_pong (void * arg)
{
 int i = 0;

 /* wait for ping and wake up ping */ 
 while(1) {
  pthread_mutex_lock(&thr_cond_lock);
  if (g_ready == PING_READY) {
   pthread_cond_signal(&thr_cond); 
//   printf("pong is waiting\n");
   pthread_cond_wait(&thr_cond, &thr_cond_lock);
   break;
  } 
  else
   pthread_mutex_unlock(&thr_cond_lock);
 }

 while(1) {
  printf("pong(%d)\n", ++i);
  pthread_cond_signal(&thr_cond);
  if (i == MAX_COUNT) {
   pthread_mutex_unlock(&thr_cond_lock);
   break;
  }
  else
   pthread_cond_wait(&thr_cond, &thr_cond_lock);
 }
  
 pthread_exit((void *)2); 
}

int main()
{
 pthread_t pthread[2];
 int result[2];

 pthread_create(&pthread[1], NULL, thread_pong, NULL); 
 pthread_create(&pthread[0], NULL, thread_ping, NULL); 

 pthread_join(pthread[0], (void **)&result[0]);
 pthread_join(pthread[1], (void **)&result[1]);
// printf("result : %d %d\n", result[0], result[1]);

 return 0;
}

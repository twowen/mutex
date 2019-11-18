#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

#define GET_CNT_VAL(x) ((x) = (counter))
#define SET_CNT_VAL(x) ((counter) = (x))

static int counter = 0;

pthread_mutex_t mutex_flag = PTHREAD_MUTEX_INITIALIZER;

static void *child ()
{
	int i, tmp;

	for (i = 0; i < 3; i++) {
		pthread_mutex_lock(&mutex_flag);
		GET_CNT_VAL(tmp);
		SET_CNT_VAL(tmp + 1);
		sleep(1);	// let first thread slow down
		pthread_mutex_unlock(&mutex_flag);
		printf("counter = %d (my tid: %d)\n", GET_CNT_VAL(tmp), (unsigned int)pthread_self());
	}

	pthread_exit(NULL);
}

int main (void)
{
	pthread_t t1, t2;

	printf("my pid = %d\n", getpid());

	pthread_create(&t1, NULL, child, NULL);
	printf("create thread %d\n", (unsigned int)t1);
	pthread_create(&t2, NULL, child, NULL);
	printf("create thread %d\n", (unsigned int)t2);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	return 0;
}

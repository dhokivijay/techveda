typedef struct {
	pthread_spinlock_t spin;
	int writecount;
} pthread_seq_t;

int pthread_seq_init_np(pthread_seq_t *);

int pthread_seq_rdlock_np(pthread_seq_t *);

int pthread_seq_rdunlock_np(pthread_seq_t *);

int pthread_seq_wrlock_np(pthread_seq_t *);
int pthread_seq_wrunlock_np(pthread_seq_t *);

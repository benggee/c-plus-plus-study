CC=g++
CFLAGS=-g -std=c++11

.PHONY: all clean

all: create_thread thread_param thread_resource_release thread_cancel thread_signal thread_safe thread_mutex thread_spinlock thread_rwlock thread_cond thread_sem thread_pthread_cond_wait thread_producter_consumer thread_producter_consumer_sem_mutex thread_producter_consumer_sem thread_client thread_server	

create_thread: 
	$(CC) $(CFLAGS) -o create_thread create_thread.cpp -lpthread -lm -lc

thread_param:
	$(CC) $(CFLAGS) -o thread_param thread_param.cpp -lpthread -lm -lc

thread_resource_release:
	$(CC) $(CFLAGS) -o thread_resource_release thread_resource_release.cpp -lpthread -lm -lc

thread_cancel:
	$(CC) $(CFLAGS) -o thread_cancel thread_cancel.cpp -lpthread -lm -lc

thread_signal:
	$(CC) $(CFLAGS) -o thread_signal thread_signal.cpp -lpthread -lm -lc

thread_safe:
	$(CC) $(CFLAGS) -o thread_safe thread_safe.cpp -lpthread -lm -lc

thread_mutex:
	$(CC) $(CFLAGS) -o thread_mutex thread_mutex.cpp -lpthread -lm -lc

thread_spinlock:
	$(CC) $(CFLAGS) -o thread_spinlock thread_spinlock.cpp -lpthread -lm -lc

thread_rwlock:
	$(CC) $(CFLAGS) -o thread_rwlock thread_rwlock.cpp -lpthread -lm -lc

thread_cond:
	$(CC) $(CFLAGS) -o thread_cond thread_cond.cpp -lpthread -lm -lc

thread_sem:
	$(CC) $(CFLAGS) -o thread_sem thread_sem.cpp -lpthread -lm -lc

thread_pthread_cond_wait:
	$(CC) $(CFLAGS) -o thread_pthread_cond_wait thread_pthread_cond_wait.cpp -lpthread -lm -lc

thread_producter_consumer:
	$(CC) $(CFLAGS) -o thread_producter_consumer thread_producter_consumer.cpp -lpthread -lm -lc

thread_producter_consumer_sem_mutex:
	$(CC) $(CFLAGS) -o thread_producter_consumer_sem_mutex thread_producter_consumer_sem_mutex.cpp -lpthread -lm -lc

thread_producter_consumer_sem:
	$(CC) $(CFLAGS) -o thread_producter_consumer_sem thread_producter_consumer_sem.cpp -lpthread -lm -lc

thread_client:
	$(CC) $(CFLAGS) -o thread_client thread_client.cpp ../lib/_public.cpp -lpthread -lm -lc

thread_server:
	$(CC) $(CFLAGS) -o thread_server thread_server.cpp ../lib/_public.cpp -lpthread -lm -lc

clean:
	rm -rf create_thread thread_param thread_resource_release thread_cancel thread_signal thread_safe thread_mutex thread_spinlock thread_rwlock thread_cond thread_sem thread_pthread_cond_wait thread_producter_consumer thread_producter_consumer_sem_mutex thread_producter_consumer_sem thread_client thread_server test


/** 
 * @file thread_management.h
 * A thread launcher that keeps track of running threads.
 *   
 * This utility file tries to ease the use of threads. Thread references are
 * save for future use like cancelling all threads. Additionally, it abstracts
 * from the used thread library as much as possible.
 *
 * @author Matthias Liebig
 */

#ifndef HTHREAD_MANAGEMENT_USED
#define HTHREAD_MANAGEMENT_USED

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define THREAD_MGMT_MSG_PREFIX "[thread management] "

int tm_init();

int start_thread(void *(*start_routine) (void *), void *args);

int thread_terminated();

int tm_destroy(int forceShutdown);

int tm_join_active_threads();

#endif

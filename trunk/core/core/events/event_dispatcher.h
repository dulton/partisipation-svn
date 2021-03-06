#ifndef HEVENT_DISPATCHER_USED
#define HEVENT_DISPATCHER_USED

#include <core/events/events.h>
#include <util/queue/queue.h>

typedef struct {
	queue eventPool;
	pthread_mutex_t poolLock;
	pthread_cond_t wakeUp;
	pthread_mutex_t wakeUpLock;
	int callId;
	int sipCallId;
	int doShutdown;
} sm_data;

typedef struct {
	event trigger;
	void **params;
} call_trigger;

int ed_init();

int ed_destroy();

int event_dispatch(event evt, void **params, int *callId);

int ed_shutdown_all();

#endif

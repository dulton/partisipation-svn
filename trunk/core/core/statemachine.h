#ifndef H_STATEMACHINE_USED
#define H_STATEMACHINE_USED

typedef enum {
	INITIAL,
	CHECKING,
	INVITING,
	INVITED,
	CONNECTED,
	TERMINATING,
	FINAL
} sm_state;

void *sm_start(void *args);

#endif

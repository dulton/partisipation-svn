#ifndef HCALLS_USED
#define HCALLS_USED

int make_call(int const accountId, char *const callee);

int end_call(int const callId);

int accept_call(int const callId);

#endif

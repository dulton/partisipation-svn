#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <core/gui_input/registration_receiver.h>
#include <core/gui_output/gui_callback_sender.h>
#include <util/logging/logger.h>
#include <util/config/globals.h>

#define REG_RCVR_MSG_PREFIX "[registration receiver] "

char *gi_register_gui(char *const address, int const port) {

	LOG_DEBUG(REG_RCVR_MSG_PREFIX "register_gui(address: %s port: %d)",
			  address, port);

	int rc;

	if (config.remote.callback.guiCallback.guiURL) {
		free(config.remote.callback.guiCallback.guiURL);
	}

	config.remote.callback.guiCallback.guiURL =
		(char *) malloc(1024 * sizeof(char));
	sprintf(config.remote.callback.guiCallback.guiURL, "http://%s:%d/RPC2",
			address, port);

	LOG_DEBUG(REG_RCVR_MSG_PREFIX "callback URL is now %s",
			  config.remote.callback.guiCallback.guiURL);

	rc = go_register_core();

	// TODO: what happens in error case? (atm: access violation)
	if (!rc) {
		LOG_DEBUG(REG_RCVR_MSG_PREFIX "register_gui result=ERROR");
		return "ERROR";
	}

	LOG_DEBUG(REG_RCVR_MSG_PREFIX "register_gui result=OK");
	return "OK";
}

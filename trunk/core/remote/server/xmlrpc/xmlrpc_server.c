#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <xmlrpc.h>
#include <xmlrpc_server.h>
#include <xmlrpc_server_abyss.h>

#include <remote/server/xmlrpc/xmlrpc_server.h>
#include <remote/server/xmlrpc/registration_remote.h>
#include <remote/server/xmlrpc/calls_remote.h>
#include <remote/server/xmlrpc/accounts_remote.h>
#include <remote/server/xmlrpc/volume_remote.h>
#include <remote/server/xmlrpc/extras_remote.h>
#include <util/threads/thread_management.h>
#include <util/config/globals.h>
#include <util/logging/logger.h>

#define XMLRPC_SERVER_MSG_PREFIX "[xmlrpc server] "

int serverRunning = 0;

void xs_init() {
}

void generate_method_name(const char *suffix, char *target) {
	strcpy(target, config.remote.server.xmlrpcServer.registerPrefix);
	strcat(target, ".");
	strcat(target, suffix);
}

void *xmlrpc_server_thread(void *param) {

	xmlrpc_server_abyss_parms serverparm;
	xmlrpc_registry *registryP;
	xmlrpc_env env;
	char name[80];

	/*
	 * server initializiation 
	 */

	xs_init();

	xmlrpc_env_init(&env);

	registryP = xmlrpc_registry_new(&env);

	// registration management:
	generate_method_name("registerGui", name);
	xmlrpc_registry_add_method(&env, registryP, NULL, name,
							   &register_gui_RP, NULL);

	generate_method_name("unregisterGui", name);
	xmlrpc_registry_add_method(&env, registryP, NULL, name,
							   &unregister_gui_RP, NULL);

	// call management:
	generate_method_name("makeCall", name);
	xmlrpc_registry_add_method(&env, registryP, NULL, name, &make_call_RP,
							   NULL);

	generate_method_name("endCall", name);
	xmlrpc_registry_add_method(&env, registryP, NULL, name, &end_call_RP,
							   NULL);

	generate_method_name("acceptCall", name);
	xmlrpc_registry_add_method(&env, registryP, NULL, name,
							   &accept_call_RP, NULL);

	// account management:

	generate_method_name("accountGetAll", name);
	xmlrpc_registry_add_method(&env, registryP, NULL, name,
							   &account_get_all_RP, NULL);

	generate_method_name("accountSet", name);
	xmlrpc_registry_add_method(&env, registryP, NULL, name,
							   &account_set_RP, NULL);

	generate_method_name("accountGet", name);
	xmlrpc_registry_add_method(&env, registryP, NULL, name,
							   &account_get_RP, NULL);

	generate_method_name("accountCreate", name);
	xmlrpc_registry_add_method(&env, registryP, NULL, name,
							   &account_create_RP, NULL);

	generate_method_name("accountDelete", name);
	xmlrpc_registry_add_method(&env, registryP, NULL, name,
							   &account_delete_RP, NULL);

	generate_method_name("accountSave", name);
	xmlrpc_registry_add_method(&env, registryP, NULL, name,
							   &account_save_RP, NULL);

	generate_method_name("register", name);
	xmlrpc_registry_add_method(&env, registryP, NULL, name,
							   &account_register_RP, NULL);

	generate_method_name("unregister", name);
	xmlrpc_registry_add_method(&env, registryP, NULL, name,
							   &account_unregister_RP, NULL);

	generate_method_name("registerAuto", name);
	xmlrpc_registry_add_method(&env, registryP, NULL, name,
							   &account_register_auto_RP, NULL);

	// volume management:

	generate_method_name("setSpeakerVolume", name);
	xmlrpc_registry_add_method(&env, registryP, NULL, name,
							   &set_speaker_volume_RP, NULL);

	generate_method_name("getSpeakerVolume", name);
	xmlrpc_registry_add_method(&env, registryP, NULL, name,
							   &get_speaker_volume_RP, NULL);

	generate_method_name("setMicroVolume", name);
	xmlrpc_registry_add_method(&env, registryP, NULL, name,
							   &set_micro_volume_RP, NULL);

	generate_method_name("getMicroVolume", name);
	xmlrpc_registry_add_method(&env, registryP, NULL, name,
							   &get_micro_volume_RP, NULL);

	// extras management:

	generate_method_name("sendDtmf", name);
	xmlrpc_registry_add_method(&env, registryP, NULL, name, &send_dtmf_RP,
							   NULL);

	/*
	 * In the modern form of the Abyss API, we supply parameters in memory
	 * like a normal API.  We select the modern form by setting
	 * config_file_name to NULL: 
	 */
	serverparm.config_file_name = NULL;
	serverparm.registryP = registryP;
	serverparm.port_number = config.remote.server.xmlrpcServer.port;
	serverparm.log_file_name =
		config.remote.server.xmlrpcServer.logFileName;

	LOG_INFO(XMLRPC_SERVER_MSG_PREFIX "Running XML-RPC server...");

	xmlrpc_server_abyss(&env, &serverparm, XMLRPC_APSIZE(log_file_name));

	/*
	 * xmlrpc_server_abyss() never returns 
	 */

	thread_terminated();
	return 0;
}

int start_xmlrpc_server_thread() {
	int rc;

	if (serverRunning) {
		// xmlrpc server thread is already running
		return 0;
	}

	rc = start_thread(xmlrpc_server_thread, NULL);
	if (rc == 0) {
		// ERROR
		return 0;
	}
	serverRunning = 1;
	return 1;
}

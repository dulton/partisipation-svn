#include <stdlib.h>
#include <stdio.h>

#include <xmlrpc.h>
#include <remote/server/xmlrpc/accounts_remote.h>
#include <remote/server/accounts.h>
#include <util/config/globals.h>

xmlrpc_value *account_get_all_RP(xmlrpc_env * const env,
								 xmlrpc_value * const param_array,
								 void *const server_context) {

	printf("entering accountGetAll...\n");

	int *ids;
	int len;
	int i;

	ids =
		(int *) malloc(config.accounts.accountManagement.
					   maxAccountIdAmount * sizeof(int));
	account_get_all(ids, &len);

	xmlrpc_value *result = xmlrpc_array_new(env);

	for (i = 0; i < len; i++) {
		xmlrpc_value *item = xmlrpc_build_value(env, "i", ids[i]);

		xmlrpc_array_append_item(env, result, item);
	}
	free(ids);

	printf("leaving accountGetAll\n");
	return result;
}

xmlrpc_value *account_set_RP(xmlrpc_env * const env,
							 xmlrpc_value * const param_array,
							 void *const server_context) {

	printf("entering accountSet...\n");

	xmlrpc_int32 accountId;
	char *attribute;
	char *value;
	int result;

	xmlrpc_decompose_value(env, param_array, "(iss)", &accountId,
						   &attribute, &value);
	if (env->fault_occurred) {
		return NULL;
	}

	result = account_set(accountId, attribute, value);

	printf("leaving accountSet\n");
	return xmlrpc_build_value(env, "b", result);
}

xmlrpc_value *account_get_RP(xmlrpc_env * const env,
							 xmlrpc_value * const param_array,
							 void *const server_context) {

	printf("entering accountGet...\n");

	xmlrpc_int32 accountId;
	char *attribute;

	char *result =
		(char *) malloc(config.accounts.accountManagement.maxValueLength *
						sizeof(char));

	xmlrpc_decompose_value(env, param_array, "(is)", &accountId,
						   &attribute);
	if (env->fault_occurred) {
		return NULL;
	}

	account_get(accountId, attribute, result);

	xmlrpc_value *xmlres = xmlrpc_build_value(env, "s", result);

	free(result);

	printf("leaving accountGet\n");
	return xmlres;

}

xmlrpc_value *account_create_RP(xmlrpc_env * const env,
								xmlrpc_value * const param_array,
								void *const server_context) {

	printf("entering accountCreate...\n");

	int result;

	result = account_create();

	printf("leaving accountCreate\n");
	return xmlrpc_build_value(env, "i", result);
}

xmlrpc_value *account_delete_RP(xmlrpc_env * const env,
								xmlrpc_value * const param_array,
								void *const server_context) {

	printf("entering accountDelete...\n");

	xmlrpc_int32 accountId;
	int result;

	xmlrpc_decompose_value(env, param_array, "(i)", &accountId);
	if (env->fault_occurred) {
		return NULL;
	}

	result = account_delete(accountId);

	printf("leaving accountDelete\n");
	return xmlrpc_build_value(env, "b", result);
}

xmlrpc_value *account_register_RP(xmlrpc_env * const env,
								  xmlrpc_value * const param_array,
								  void *const server_context) {

	printf("entering register...\n");

	xmlrpc_int32 accountId;
	int result;

	xmlrpc_decompose_value(env, param_array, "(i)", &accountId);
	if (env->fault_occurred) {
		return NULL;
	}

	result = account_register(accountId);

	printf("leaving register\n");
	return xmlrpc_build_value(env, "b", result);
}

xmlrpc_value *account_unregister_RP(xmlrpc_env * const env,
									xmlrpc_value * const param_array,
									void *const server_context) {

	printf("entering unregister...\n");

	xmlrpc_int32 accountId;
	int result;

	xmlrpc_decompose_value(env, param_array, "(i)", &accountId);
	if (env->fault_occurred) {
		return NULL;
	}

	result = account_unregister(accountId);

	printf("leaving unregister\n");
	return xmlrpc_build_value(env, "b", result);
}

xmlrpc_value *account_save_RP(xmlrpc_env * const env,
							  xmlrpc_value * const param_array,
							  void *const server_context) {

	printf("entering accountSave...\n");

	int result;

	result = account_save();

	printf("leaving accountSave\n");
	return xmlrpc_build_value(env, "i", result);
}

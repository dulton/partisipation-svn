#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <expat.h>
#include <accounts/account_management.h>
#include <constants.h>
#include <accounts/list.h>

#define BUFFSIZE 1000

char *XMLFILE = "accounts/accounts.xml";
int id = 0;
struct account *cur_acc;

/**
	This function changes the path to XML file with the account list. Default 
	path is "account.xml" in the same directory, where account_management.c 
	is located. 
*/
void set_xml_source(char *file) {
	XMLFILE = file;
}

/**
	This function is called by the eXpat parser, if an XML element opening tag 
  	has been discovered.
  	
  	Each attribute within the array of attributes occupies 2 consecutive places
  	like this - array[n]: <attribute name>, array[n+1]: <attribute value>
  	
  	@param void* data User data
  	@param char* name Name of the XML element
  	@param char** attr Array of attributes of the XML element
*/
void elem_start(void *data, const char *name, const char **attr) {
	printf("elem_start - enter\n");
	if (strcmp(name, "account") == 0) {
		cur_acc = (struct account *) malloc(sizeof(struct account));
		int i;
		char *val;

		for (i = 0; i < 20; i += 2) {

			int len = strlen(attr[i + 1]);
			const char *s = attr[i + 1];

			val = (char *) malloc(len);
			strcpy(val, s);

			if (strcmp(attr[i], "name") == 0) {
				cur_acc->name = val;
			} else if (strcmp(attr[i], "username") == 0) {
				cur_acc->username = val;
			} else if (strcmp(attr[i], "domain") == 0) {
				cur_acc->domain = val;
			} else if (strcmp(attr[i], "authusername") == 0) {
				cur_acc->authusername = val;
			} else if (strcmp(attr[i], "password") == 0) {
				cur_acc->password = val;
			} else if (strcmp(attr[i], "displayname") == 0) {
				cur_acc->displayname = val;
			} else if (strcmp(attr[i], "outboundproxy") == 0) {
				cur_acc->outboundproxy = val;
			} else if (strcmp(attr[i], "registrar") == 0) {
				cur_acc->registrar = val;
			} else if (strcmp(attr[i], "autoregister") == 0) {
				cur_acc->autoregister = atoi(val);
				free(val);
			} else if (strcmp(attr[i], "id") == 0) {
				int cur_id = atoi(val);

				cur_acc->id = cur_id;
				// set ID counter to the highest value
				if (cur_id > id)
					id = cur_id;
				free(val);
			}
		}
	}
	printf("elem_start - exit\n");
}

/**
	This function is called by the eXpat parser, if an XML element closing tag 
	has been discovered. 
    
    @param void* data User data
    @param char* name Name of the XML element
*/
void elem_end(void *data, const char *name) {
	printf("elem_end - start\n");
	if (strcmp(name, "account") == 0) {
		add_node(cur_acc);
	}
	printf("elem_end - exit\n");
}

/**
	Initialize list of accounts by parsing an XML file.
*/
void account_management_init() {

	printf("account_management.c - account_management_init() - enter\n");

	int xmlfile = open(XMLFILE, O_RDONLY);
	XML_Parser parser = XML_ParserCreate(NULL);

	XML_SetElementHandler(parser, elem_start, elem_end);
	for (;;) {
		int bytes_read;
		void *buf = XML_GetBuffer(parser, BUFFSIZE);

		if (buf == NULL) {
			// if memory could not be allocated
			printf("Buffer allocation failed.\n");
		}

		bytes_read = read(xmlfile, buf, BUFFSIZE);

		if (bytes_read < 0) {
			// if file could not be read
			printf("No file found or file reading error.\n");
		}

		if (!XML_ParseBuffer(parser, bytes_read, bytes_read == 0)) {
			// if XML document is not well-formed
			printf("XML parse error in line %d: %s\n",
				   XML_GetCurrentLineNumber(parser),
				   XML_ErrorString(XML_GetErrorCode(parser)));
			break;
		}
		// if end of file reached
		if (bytes_read == 0)
			break;
	}
	XML_ParserFree(parser);
	close(xmlfile);

	print_list();

	printf("account_management.c - account_management_init() - exit\n");
}

/**
	Save current accounts to an XML file.
*/
void account_list_save() {
	// open file in write-mode
	FILE *xmlfile = fopen(XMLFILE, "w");

	if (xmlfile) {
		// write the XML declaration
		fprintf(xmlfile,
				"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n");
		fprintf(xmlfile, "<!DOCTYPE list SYSTEM \"./accounts.dtd\">\n");
		// open document root element
		fprintf(xmlfile, "<list>\n");

		struct node *tmp = head;

		// continue until the end of the list
		while (tmp) {
			// get current account
			struct account *acc = tmp->acc;

			// save account data
			fprintf(xmlfile, "\t<account\n");
			fprintf(xmlfile, "\t\tid=\"%d\"\n", acc->id);
			fprintf(xmlfile, "\t\tname=\"%s\"\n", acc->name);
			fprintf(xmlfile, "\t\tusername=\"%s\"\n", acc->username);
			fprintf(xmlfile, "\t\tdomain=\"%s\"\n", acc->domain);
			fprintf(xmlfile, "\t\tauthusername=\"%s\"\n",
					acc->authusername);
			fprintf(xmlfile, "\t\tpassword=\"%s\"\n", acc->password);
			fprintf(xmlfile, "\t\tdisplayname=\"%s\"\n", acc->displayname);
			fprintf(xmlfile, "\t\toutboundproxy=\"%s\"\n",
					acc->outboundproxy);
			fprintf(xmlfile, "\t\tregistrar=\"%s\"\n", acc->registrar);
			fprintf(xmlfile, "\t\tautoregister=\"%d\"\n",
					acc->autoregister);
			fprintf(xmlfile, "\t/>\n");
			// go on with next account
			tmp = tmp->next;
		}
		// close document root element
		fprintf(xmlfile, "</list>\n");
	} else {
		printf("I/O error for file %s\n", XMLFILE);
	}
	// close file
	fclose(xmlfile);
}

/**
	This function delivers the IDs of all known accounts.
	
	@param int* accountIds Pointer to array to be filled with account IDs
	@param int* length Pointer to an integer to store length of this array
*/
void account_get_all(int *accountIds, int *length) {

	printf("account_management.c - account_get_all() - enter\n");

	*length = get_length();

	if (*length < MAX_ACCOUNTID_AMOUNT) {
		int i = 0;
		struct node *tmp = head;

		while (tmp) {
			accountIds[i] = tmp->acc->id;
			tmp = tmp->next;
			i++;
		}
	} else {
		*length = 0;
		printf("Error: More accounts than memory reserved.");
	}
	printf("account_management.c - account_get_all() - exit\n");
}

/**
	This function sets certain account attribute to the specified value.
	
	@param int accountId ID of the account whose attribute is to be set
	@param char* attribute Name of the attribute to be set
	@param char* value Value of the attribute to be set
	@return int 1 if OK, 0 if error
*/
int
account_set(int const accountId, char *const attribute, char *const value) 
{

	printf("account_management.c - account_set() - enter\n");

	struct account *acc = get_node(accountId)->acc;
	char *new_val = (char *) malloc(strlen(value));

	strcpy(new_val, value);

	if (strcmp(attribute, "id") == 0) {
		acc->id = atoi(value);
		free(new_val);
	} else if (strcmp(attribute, "name") == 0) {
		free(acc->name);
		acc->name = new_val;
	} else if (strcmp(attribute, "username") == 0) {
		free(acc->username);
		acc->username = new_val;
	} else if (strcmp(attribute, "domain") == 0) {
		free(acc->domain);
		acc->domain = new_val;
	} else if (strcmp(attribute, "authusername") == 0) {
		free(acc->authusername);
		acc->authusername = new_val;
	} else if (strcmp(attribute, "password") == 0) {
		free(acc->password);
		acc->password = new_val;
	} else if (strcmp(attribute, "displayname") == 0) {
		free(acc->displayname);
		acc->displayname = new_val;
	} else if (strcmp(attribute, "outboundproxy") == 0) {
		free(acc->outboundproxy);
		acc->outboundproxy = new_val;
	} else if (strcmp(attribute, "registrar") == 0) {
		free(acc->registrar);
		acc->registrar = new_val;
	} else if (strcmp(attribute, "autoregister") == 0) {
		acc->autoregister = atoi(value);
		free(new_val);
	} else {
		free(new_val);
		return 0;
	}

	printf("account_management.c - account_set() - exit\n");

	return 1;
}

/**
	This function delivers certain attribute value for the specified account.
	
	Note that only as much characters are read as defined by MAX_VALUE_LENGTH
	from <constants.h>. Anything else is cut away, but this length ought be
	enough for any convenient value. 
	
	@param int accountId ID of the account whose attribute is to be read
	@param char attribute Name of the attribute to be read
	@param char* result Char-pointer to store the value of the attribute in
*/
void account_get(int const accountId, char *const attribute, char *result) {

	printf("account_management.c - account_get() - enter\n");

	printf("id: %d, att: %s\n", accountId, attribute);
	print_list();

	struct account *acc = get_node(accountId)->acc;

	if (strcmp(attribute, "id") == 0) {
		snprintf(result, 10, "%d", acc->id);
	} else if (strcmp(attribute, "name") == 0) {
		strncpy(result, acc->name, MAX_VALUE_LENGTH);
	} else if (strcmp(attribute, "username") == 0) {
		strncpy(result, acc->username, MAX_VALUE_LENGTH);
	} else if (strcmp(attribute, "domain") == 0) {
		strncpy(result, acc->domain, MAX_VALUE_LENGTH);
	} else if (strcmp(attribute, "authusername") == 0) {
		strncpy(result, acc->authusername, MAX_VALUE_LENGTH);
	} else if (strcmp(attribute, "password") == 0) {
		strncpy(result, acc->password, MAX_VALUE_LENGTH);
	} else if (strcmp(attribute, "displayname") == 0) {
		strncpy(result, acc->displayname, MAX_VALUE_LENGTH);
	} else if (strcmp(attribute, "outboundproxy") == 0) {
		strncpy(result, acc->outboundproxy, MAX_VALUE_LENGTH);
	} else if (strcmp(attribute, "registrar") == 0) {
		strncpy(result, acc->registrar, MAX_VALUE_LENGTH);
	} else if (strcmp(attribute, "autoregister") == 0) {
		snprintf(result, 10, "%d", acc->autoregister);
	}

	printf("account_management.c - account_get() - exit\n");
}

/**
	This function creates new account.
	
	@return int ID of the new account
*/
int account_create() {

	printf("account_management.c - account_create() - enter\n");

	struct account *a = (struct account *) malloc(sizeof(struct account));

	id++;

	a->id = id;
	a->autoregister = 0;
	a->name = NULL;
	a->username = NULL;
	a->domain = NULL;
	a->authusername = NULL;
	a->password = NULL;
	a->displayname = NULL;
	a->outboundproxy = NULL;
	a->registrar = NULL;

	add_node(a);

	account_list_save();

	printf("account_management.c - account_create() - exit\n");

	return id;
}

/**
	This function deletes account with the specified ID.
	
	@param int accountId ID of the account to be deleted
	@return int Always 1
*/
int account_delete(int const accountId) {

	printf("account_management.c - account_delete() - enter\n");

	del_node(accountId);

	account_list_save();

	printf("account_management.c - account_delete() - exit\n");

	return 1;
}

int account_register(int const accountId) {

	// do something
	printf("accountId: %d\n", accountId);

	return 1;
}

int account_unregister(int const accountId) {

	// do something
	printf("accountId: %d\n", accountId);

	return 1;
}

int account_save() {
	printf("account_management.c - account_save() - enter\n");

	account_list_save();

	printf("account_management.c - account_save() - exit\n");
	return 1;
}

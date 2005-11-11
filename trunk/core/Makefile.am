XMLRPC_C_CONFIG = /usr/local/bin/xmlrpc-c-config
LIBS_CLIENT = $(shell $(XMLRPC_C_CONFIG) client --ldadd)
LIBS_ABYSS = $(shell $(XMLRPC_C_CONFIG) abyss-server --ldadd)

bin_PROGRAMS = core_launcher
core_launcher_SOURCES = core_launcher.c util/config/xml/config_reader.c util/config/xml/config_writer.c util/logging/simple/logger.c util/threads/thread_management.c util/queue/queue.c accounts/account_management.c util/list/list.c core/callIDs/call_id_generator.c core/events/event_dispatcher.c core/events/statemachine.c core/events/call_status.c core/events/transaction_info.c sipstack/eXosip/osip_adapter.c core/sip_input/sip_listener.c remote/callback/xmlrpc/xmlrpc_callback_client.c remote/callback/xmlrpc/gui_callback.c remote/server/xmlrpc/xmlrpc_server.c remote/server/xmlrpc/accounts_remote.c remote/server/xmlrpc/calls_remote.c remote/server/xmlrpc/extras_remote.c remote/server/xmlrpc/registration_remote.c remote/server/xmlrpc/volume_remote.c remote/server/accounts.c remote/server/calls.c remote/server/extras.c remote/server/registration.c remote/server/volume.c core/gui_input/accounts_receiver.c core/gui_input/calls_receiver.c core/gui_input/extras_receiver.c core/gui_input/registration_receiver.c core/gui_input/volume_receiver.c core/gui_output/gui_callback_sender.c core/sip_output/registrar_manager.c

core_launcher_LDADD = -lpthread -lgenx -leXosip2 -losip2 -losipparser2

core_launcher_LDFLAGS = -Llib/genx $(LIBS_CLIENT) $(LIBS_ABYSS)

EXTRA_DIST = README INSTALL COPYING ChangeLog

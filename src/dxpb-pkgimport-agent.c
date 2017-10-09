/*
 * dxpb_pkgimport_agent.c
 *
 * Runtime for xbps-src reader.
 */

#include <getopt.h>
#include <czmq.h>
#include "dxpb.h"
#include "pkgimport_client.h"

#include "dxpb-common.h"

#define DEFAULT_IMPORT_ENDPOINT "ipc:///var/run/dxpb/pkgimport.sock"
#define DEFAULT_XBPS_SRC "./xbps-src"

#define VERBOSE_FLAG 1
#define ERR_FLAG 2

void
help(void)
{
#include "dxpb-pkgimport-agent-help.inc"
	printf("%.*s\n", ___doc_dxpb_pkgimport_agent_help_txt_len, ___doc_dxpb_pkgimport_agent_help_txt);
}

int
run(int flags, const char *endpoint, const char *xbps_src)
{
	assert((flags & ERR_FLAG) == 0);
	enum ret_codes retVal = ERR_CODE_BAD;
	pkgimport_client_t *client;
	zactor_t *actor;

	client = pkgimport_client_new();
	assert(client);
	actor = pkgimport_client_actor(client);
	assert(actor);
	zstr_sendx(actor, "SET XBPS_SRC PATH", xbps_src, NULL);
	zstr_sendx(actor, "CONSTRUCT", endpoint, NULL);

	zpoller_t *polling = zpoller_new(actor);
	assert(polling);
	zactor_t *rc = zpoller_wait(polling, -1);
	char *tmp = zstr_recv(rc);
	if (!strcmp(tmp, "FAILURE"))
		retVal = ERR_CODE_SADSOCK;
	else if (!strcmp(tmp, "SUCCESS"))
		retVal = ERR_CODE_OK;
	zsock_flush(rc);

	while (retVal == ERR_CODE_OK && (rc = zpoller_wait(polling, -1)) != NULL) {
		fprintf(stderr, "pkgimport-agent: %s\n", zstr_recv(rc));
		// We don't have any communication specified.
		zsock_flush(rc);
	}

	zstr_sendx(actor, "$TERM", NULL);

	zpoller_destroy(&polling);
	pkgimport_client_destroy(&client);
	return retVal;
}

int
main(int argc, char * const *argv)
{
	int c;
	int flags = 0;
	const char *optstring = "Lvr:i:x:";
	char *default_endpoint = DEFAULT_IMPORT_ENDPOINT;
	char *default_xbps_src = DEFAULT_XBPS_SRC;
	char *endpoint = NULL;
	char *xbps_src = NULL;

	while ((c = getopt(argc, argv, optstring)) != -1) {
		switch(c) {
		case 'L':
			print_license();
			return 0;
		case 'h':
			help();
			return 0;
		case 'i':
			endpoint = optarg;
			break;
		case 'x':
			xbps_src = optarg;
			break;
		case 'v':
			flags |= VERBOSE_FLAG;
			break;
		case '?':
			flags |= ERR_FLAG;
			break;
		case ':':
			flags |= ERR_FLAG;
			break;
		}
	}

	prologue(argv[0]);

	if (flags & ERR_FLAG) {
		fprintf(stderr, "Exiting due to errors.\n");
		help();
		exit(ERR_CODE_BADWORLD);
	}

	if (!endpoint)
		endpoint = default_endpoint;
	if (!xbps_src)
		xbps_src = default_xbps_src;

	return run(flags, endpoint, xbps_src);
}
/*
 * Copyright (c) 2013-2014 RIPE NCC <atlas@ripe.net>
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 * Standalone version of the event-based ntp. 
 */
//config:config EVNTP
//config:       bool "evntp"
//config:       default n
//config:       help
//config:               standalone version of event-driven ntp

//applet:IF_EVNTP(APPLET(evntp, BB_DIR_BIN, BB_SUID_DROP))

//kbuild:lib-$(CONFIG_EVNTP) += evntp.o

//usage:#define evntp_trivial_usage
//usage:       "todo"
//usage:#define evntp_full_usage
//usage:       "todo"

#include "libbb.h"
#include <syslog.h>
#include <event2/dns.h>
#include <event2/event.h>
#include <event2/event_struct.h>

#include "eperd.h"

static void done(void *state UNUSED_PARAM, int error)
{
	fprintf(stderr, "And we are done\n");
	exit(error);
}

int evntp_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int evntp_main(int argc UNUSED_PARAM, char **argv)
{
	int r;
	void *state;

	/* Create libevent event base */
	EventBase= event_base_new();
	if (!EventBase)
	{
		fprintf(stderr, "evntp: event_base_new failed\n");
		exit(1);
	}
	DnsBase= evdns_base_new(EventBase, 1 /*initialize*/);
	if (!DnsBase)
	{
		fprintf(stderr, "evdns_base_new failed\n");
		exit(1);
	}

	state= ntp_ops.init(argc, argv, done);
	if (!state)
	{
		fprintf(stderr, "evntp: ntp_ops.init failed\n");
		exit(1);
	}
	ntp_ops.start(state);

	r= event_base_loop(EventBase, 0);
	if (r != 0)
	{
		fprintf(stderr, "evntp: event_base_loop failed\n");
		exit(1);
	}
	return 0; /* not reached */
}


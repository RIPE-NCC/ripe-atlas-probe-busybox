/*
 * Copyright (c) 2013 RIPE NCC <atlas@ripe.net>
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

#define REG_INIT_REPLY_REL "status/reg_init_reply.txt"

#include "libbb.h"
int get_probe_id(void)
{
        static int probe_id= -1;

        size_t len;
        char *check, *fn;
        const char *key;
        FILE *fp;
        char buf[80];

	if (probe_id > 0)
		return probe_id;	/* Assume probe ID never changes */

	fn= atlas_path(REG_INIT_REPLY_REL);
        fp= fopen(fn, "r");
	free(fn); fn= NULL;
        if (!fp)
                return -1;

        probe_id= -1;
        while (fgets(buf, sizeof(buf), fp) != NULL)
        {
                if (strchr(buf, '\n') == NULL)
                        continue;
                key= "PROBE_ID ";
                len= strlen(key);

                if (strncmp(buf, key, len) != 0 || strlen(buf) <= len)
                        continue;
                probe_id= strtol(buf+len, &check, 10);
                break;
        }
        fclose(fp);
        return probe_id;
}

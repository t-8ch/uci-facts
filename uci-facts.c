/*
  Copyright (c) 2016, Thomas Weißschuh <freifunk@t-8ch.de>
  Copyright (c) 2016, Matthias Schiffer <mschiffer@universe-factory.net>
  All rights reserved.
  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>

#include <uci.h>

#include "uci-facts.h"

static const struct uci_facts_mapping mappings[] = {
	{"network.mesh_wan.ifname"},
	{"network.mesh_wan.auto"},
	{"network.mesh_lan.ifname"},
	{"network.mesh_lan.auto"},
	{"simple-tc.mesh_vpn.enabled"},
	{"wireless.radio0.channel"},
	{"wireless.radio1.channel"},
	{"wireless.radio0.htmode"},
	{"wireless.radio1.htmode"},
	{"wireless.client_radio0.disabled"},
	{"wireless.client_radio1.disabled"},
	{"wireless.client_radio0.ssid"},
	{"wireless.client_radio1.ssid"},
	{"tunneldigger.@broker[0]"},
	{"fastd.mesh_vpn.enabled"},
	{NULL},
};

const char *uci_facts_fact_name(const struct uci_facts_mapping * const m) {
	const char *fact_name = m->fact_name;
	if (fact_name)
		return fact_name;
	return m->uci_name;
}

struct uci_facts_mapping *uci_facts_get_mappings(void) {
	struct uci_facts_mapping *ret = malloc(sizeof(mappings));
	if (!ret)
		return NULL;

	memcpy(ret, mappings, sizeof(mappings));

	struct uci_context *ctx = uci_alloc_context();
	ctx->flags &= ~UCI_FLAG_STRICT;

	struct uci_ptr ptr = {
		.target = UCI_TYPE_OPTION,
	};
	int i = 0;

	while (true) {
		struct uci_facts_mapping *m = ret + (i++);
		if (!m->uci_name)
			break;

		char *uci_name = strdup(m->uci_name);
		if (!uci_name)
			goto end;
		int status = uci_lookup_ptr(ctx, &ptr, uci_name, true);

		if (UCI_OK != status)
			continue;
		if (!(ptr.flags & UCI_LOOKUP_COMPLETE))
			continue;
		if (UCI_TYPE_STRING != ptr.o->type)
			continue;

		const char *value = strdup(ptr.o->v.string);

		if (!value)
			continue;

		m->value = strdup(value);
		if (!m->value)
			goto end;
	}

end:
	uci_free_context(ctx);
	return ret;
}

void uci_facts_free_mappings(struct uci_facts_mapping *m) {
	free(m);
}

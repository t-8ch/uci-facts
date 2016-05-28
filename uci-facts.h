#pragma once

struct uci_facts_mapping {
	const char *uci_name;
	const char *fact_name;
	char *value;
};

struct uci_facts_mapping *uci_facts_get_mappings(void);
void uci_facts_free_mappings(struct uci_facts_mapping *m);
const char *uci_facts_fact_name(const struct uci_facts_mapping * const m);

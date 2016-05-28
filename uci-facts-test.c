#include <stdio.h>
#include <stdbool.h>

#include "uci-facts.h"

int main(int argc, char **argv) {
	int i = 0;

	const struct uci_facts_mapping* ms = uci_facts_get_mappings();

	while (true) {
		struct uci_facts_mapping m = ms[i++];
		if (!m.uci_name)
			break;

		const char *fact_name = uci_facts_fact_name(&m);

		if (fact_name) {
			printf("%s:\t%s\n", fact_name, m.value);
		}
	}

	return 0;
}

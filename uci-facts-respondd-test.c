#include <stdio.h>

#include <json-c/json.h>
#include <respondd.h>

extern const struct respondd_provider_info respondd_providers[];

int main(int argc, char **argv) {
	int i = 0;

	while (1) {
		const struct respondd_provider_info p = respondd_providers[i++];
		if (!p.request)
			break;

		struct json_object *o = json_object_new_object();
		json_object_object_add(o, p.request, p.provider());
		printf("%s\n", json_object_to_json_string_ext(o, JSON_C_TO_STRING_PRETTY));
	}
}

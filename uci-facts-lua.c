#include <stdlib.h>
#include <stdbool.h>

#define LUA_LIB
#define LUA_COMPAT_MODULE

#include "lua.h"
#include "lauxlib.h"

#include "uci-facts.h"

static int uci_facts(lua_State *L) {
	int i = 0;

	const struct uci_facts_mapping* ms = uci_facts_get_mappings();

	lua_newtable(L);

	while (true) {
		struct uci_facts_mapping m = ms[i++];
		if (!m.uci_name)
			break;

		const char *fact_name = uci_facts_fact_name(&m);

		if (fact_name) {
			lua_pushstring(L, fact_name);
			lua_pushstring(L, m.value);
			lua_settable(L, -3);
		}
	}

	return 1;
}

static const luaL_Reg uci_factlib[] = {
	{"facts", uci_facts},
	{NULL, NULL},
};

LUALIB_API int luaopen_uci_facts(lua_State *L) {
	luaL_register(L, "uci_facts", uci_factlib);
	return 1;
}

LUALIB_API int luaopen_facts_lua(lua_State *L) {
	return luaopen_uci_facts(L);
}

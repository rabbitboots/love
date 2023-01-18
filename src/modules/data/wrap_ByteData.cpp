/**
 * Copyright (c) 2006-2023 LOVE Development Team
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 **/

#include "wrap_ByteData.h"
#include "wrap_Data.h"

// Lua 5.3
#include "libraries/lua53/lstrlib.h"

// C
#include <string.h>

namespace love
{
namespace data
{

ByteData *luax_checkbytedata(lua_State *L, int idx)
{
	return luax_checktype<ByteData>(L, idx);
}

int w_ByteData_clone(lua_State *L)
{
	ByteData *t = luax_checkbytedata(L, 1);
	ByteData *c = nullptr;
	luax_catchexcept(L, [&](){ c = t->clone(); });
	luax_pushtype(L, c);
	c->release();
	return 1;
}

int w_ByteData_pack(lua_State *L)
{
	ByteData *t = luax_checkbytedata(L, 1);
	const char *fmt = luaL_checkstring(L, 2);
	int offset = (int) luaL_checkinteger(L, 3) - 1;

	if (offset < 0)
		return luaL_error(L, "bad argument #2 to 'ByteData:pack' (initial position out of range)");

	luaL_Buffer_53 b;
	lua53_str_pack(L, fmt, 4, &b);

	if (offset + b.nelems > t->getSize())
		return luaL_error(L, "Packed data doesn't fit into source ByteData.");

	memcpy( (uint8 *) t->getData() + offset, b.ptr, b.nelems);

	return 0;
}
	
static const luaL_Reg w_ByteData_functions[] =
{
	{ "clone", w_ByteData_clone },
	{ "pack", w_ByteData_pack },
	{ 0, 0 }
};

int luaopen_bytedata(lua_State *L)
{
	luax_register_type(L, &ByteData::type, w_Data_functions, w_ByteData_functions, nullptr);
	love::data::luax_rundatawrapper(L, ByteData::type);
	return 0;
}

} // data
} // love

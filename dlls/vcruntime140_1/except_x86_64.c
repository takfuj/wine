/*
 * C++ exception handling (ver. 4)
 *
 * Copyright 2020 Piotr Caban
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifdef __x86_64__

#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(seh);

EXCEPTION_DISPOSITION __cdecl __CxxFrameHandler4(EXCEPTION_RECORD *rec,
        ULONG64 frame, CONTEXT *context, DISPATCHER_CONTEXT *dispatch)
{
    FIXME("%p %lx %p %p\n", rec, frame, context, dispatch);
    return ExceptionContinueSearch;
}

#endif
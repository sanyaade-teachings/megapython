/*
 * PyMite - A flyweight Python interpreter for 8-bit microcontrollers and more.
 * Copyright 2007 David Greenberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#undef __FILE_ID__
#define __FILE_ID__ 0x17

/**
 * Slice Object Type
 *
 * Slice object type operations.
 *
 * Log
 * ---
 *
 * 2008/01/14   First
 */

/***************************************************************
 * Includes
 **************************************************************/

#include "pm.h"


/***************************************************************
 * Functions
 **************************************************************/

PmReturn_t
slice_new(pPmObj_t start, pPmObj_t end, pPmObj_t step, pPmObj_t *r_pobj)
{
    PmReturn_t retval = PM_RET_OK;
    pPmSlice_t pslice = C_NULL;

    /* Allocate a slice */
    retval = heap_getChunk(sizeof(PmSlice_t), (uint8_t **)r_pobj);
    PM_RETURN_IF_ERROR(retval);

    /* Set slice type, empty the contents */
    pslice = (pPmSlice_t)*r_pobj;
    OBJ_SET_TYPE(pslice, OBJ_TYPE_SLC);
    pslice->start = (pPmInt_t)start;
    pslice->end = (pPmInt_t)end;
    pslice->step = (pPmInt_t)step;

    /* Algorithm doesn't support negative stepping */
    if (pslice->step->val < 0)
    {
        PM_RAISE(retval, PM_RET_EX_VAL);
    }

    return retval;
}

#ifdef HAVE_PRINT
PmReturn_t
slice_print(pPmObj_t pslice)
{
    PmReturn_t retval = PM_RET_OK;

    C_ASSERT(pslice != C_NULL);

    /* If it's not a slice, raise TypeError */
    if (OBJ_GET_TYPE(pslice) != OBJ_TYPE_SLC)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    plat_putByte('[');

    if (((pPmSlice_t)pslice)->start != C_NULL)
        int_print((pPmObj_t)(((pPmSlice_t)pslice)->start));
    plat_putByte(':');

    if (((pPmSlice_t)pslice)->end != C_NULL)
        int_print((pPmObj_t)(((pPmSlice_t)pslice)->end));
    plat_putByte(':');

    if (((pPmSlice_t)pslice)->step != C_NULL)
        int_print((pPmObj_t)(((pPmSlice_t)pslice)->step));

    return plat_putByte(']');
}
#endif /* HAVE_PRINT */

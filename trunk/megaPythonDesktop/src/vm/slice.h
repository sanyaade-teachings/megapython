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

#ifndef __SLICE_H__
#define __SLICE_H__

/**
 * Slice Object Type
 *
 * Slice object type header.
 *
 * Log:
 *
 * 2008/01/14   First.
 */

/***************************************************************
 * Constants
 **************************************************************/

/***************************************************************
 * Types
 **************************************************************/

/**
 * Slice obj
 *
 * Object represents a slice
 */
typedef struct PmSlice_s
{
    /** Object descriptor */
    PmObjDesc_t od;

    /** Start index of slice */
    pPmInt_t start;

    /** End index of slice */
    pPmInt_t end;

    /** Step index of slice */
    pPmInt_t step;

} PmSlice_t,
 *pPmSlice_t;


/***************************************************************
 * Prototypes
 **************************************************************/

/**
 * Allocates a new Slice object.
 *
 * If there is not enough memory to allocate the Slice,
 * the return status will indicate an OutOfMemoryError
 * that must be passed up to the interpreter.
 * Otherwise, a ptr to the slice is returned by reference
 * and the return status is OK.
 *
 * @param   r_pobj Return; addr of ptr to obj
 * @return  Return status
 */
PmReturn_t slice_new(pPmObj_t start, pPmObj_t end, pPmObj_t step, pPmObj_t *r_pobj);

#ifdef HAVE_PRINT
/**
 * Prints out a slice. Uses obj_print() to print elements.
 *
 * @param pobj Object to print.
 * @return Return status
 */
PmReturn_t slice_print(pPmObj_t pobj);

#endif /* HAVE_PRINT */

#endif /* __SLICE_H__ */

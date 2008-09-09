/*
 * PyMite - A flyweight Python interpreter for 8-bit microcontrollers and more.
 * Copyright 2002 Dean Hall
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
#define __FILE_ID__ 0x0B

/**
 * List Object Type
 *
 * List object type operations.
 *
 * Log
 * ---
 *
 * 2007/01/17   #76: Print will differentiate on strings and print tuples
 * 2007/01/09   #75: Printing support (P.Adelt)
 * 2007/01/09   #75: implemented list_remove() and list_index() (P.Adelt)
 * 2006/08/29   #15 - All mem_*() funcs and pointers in the vm should use
 *              unsigned not signed or void
 * 2002/04/22   First.
 */

/***************************************************************
 * Includes
 **************************************************************/

#include "pm.h"


/***************************************************************
 * Functions
 **************************************************************/

PmReturn_t
list_append(pPmObj_t plist, pPmObj_t pobj)
{
    PmReturn_t retval = PM_RET_STUB;

    C_ASSERT(plist != C_NULL);
    C_ASSERT(pobj != C_NULL);

    /* If plist is not a list, raise a TypeError exception */
    if (OBJ_GET_TYPE(plist) != OBJ_TYPE_LST)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    /* Append object to list */
    retval = seglist_appendItem(((pPmList_t)plist)->val, pobj);
    PM_RETURN_IF_ERROR(retval);

    /* Increment list length */
    ((pPmList_t)plist)->length++;

    return retval;
}


PmReturn_t
list_getItem(pPmObj_t plist, int16_t index, pPmObj_t *r_pobj)
{
    PmReturn_t retval;

    /* If it's not a list, raise TypeError */
    if (OBJ_GET_TYPE(plist) != OBJ_TYPE_LST)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    /* Adjust the index */
    if (index < 0)
    {
        index += ((pPmList_t)plist)->length;
    }

    /* Check the bounds of the index */
    if ((index < 0) || (index >= ((pPmList_t)plist)->length))
    {
        PM_RAISE(retval, PM_RET_EX_INDX);
        return retval;
    }

    /* Get item from seglist */
    retval = seglist_getItem(((pPmList_t)plist)->val, index, r_pobj);
    return retval;
}


PmReturn_t
list_insert(pPmObj_t plist, int16_t index, pPmObj_t pobj)
{
    PmReturn_t retval;
    int16_t len;

    C_ASSERT(plist != C_NULL);
    C_ASSERT(pobj != C_NULL);

    /* Raise a TypeError if plist is not a List */
    if (OBJ_GET_TYPE(plist) != OBJ_TYPE_LST)
    {
        retval = PM_RET_EX_TYPE;
        PM_RETURN_IF_ERROR(retval);
    }

    /* Adjust an out-of-bounds index value */
    len = ((pPmList_t)plist)->length;
    if (index < 0)
    {
        index += len;
    }
    if (index < 0)
    {
        index = 0;
    }
    if (index > len)
    {
        index = len;
    }

    /* Insert the item in the container */
    retval = seglist_insertItem(((pPmList_t)plist)->val, pobj, index);
    PM_RETURN_IF_ERROR(retval);

    /* Increment list length */
    ((pPmList_t)plist)->length++;
    return retval;
}


PmReturn_t
list_new(pPmObj_t *r_pobj)
{
    PmReturn_t retval = PM_RET_OK;
    pPmList_t plist = C_NULL;

    /* Allocate a list */
    retval = heap_getChunk(sizeof(PmList_t), (uint8_t **)r_pobj);
    PM_RETURN_IF_ERROR(retval);

    /* Set list type, empty the contents */
    plist = (pPmList_t)*r_pobj;
    OBJ_SET_TYPE(plist, OBJ_TYPE_LST);
    plist->length = 0;

    /* Create empty seglist */
    retval = seglist_new(&plist->val);
    return retval;
}


PmReturn_t
list_copy(pPmObj_t pobj, pPmObj_t *r_pobj)
{
    return list_replicate(pobj, 1, r_pobj);
}


PmReturn_t
list_replicate(pPmObj_t psrclist, int16_t n, pPmObj_t *r_pnewlist)
{
    PmReturn_t retval = PM_RET_OK;
    int16_t i = 0;
    int16_t j = 0;
    int16_t length = 0;
    pPmObj_t pitem = C_NULL;

    C_ASSERT(psrclist != C_NULL);
    C_ASSERT(r_pnewlist != C_NULL);

    /* If first arg is not a list, raise TypeError */
    if (OBJ_GET_TYPE(psrclist) != OBJ_TYPE_LST)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }
    length = ((pPmList_t)psrclist)->length;

    /* Allocate new list */
    retval = list_new(r_pnewlist);
    PM_RETURN_IF_ERROR(retval);

    /* Copy srclist the designated number of times */
    for (i = n; i > 0; i--)
    {
        /* Iterate over the length of srclist */
        for (j = 0; j < length; j++)
        {
            retval = list_getItem(psrclist, j, &pitem);
            PM_RETURN_IF_ERROR(retval);
            retval = list_append(*r_pnewlist, pitem);
            PM_RETURN_IF_ERROR(retval);
        }
    }
    return retval;
}


PmReturn_t
list_setItem(pPmObj_t plist, int16_t index, pPmObj_t pobj)
{
    PmReturn_t retval;

    /* If it's not a list, raise TypeError */
    if (OBJ_GET_TYPE(plist) != OBJ_TYPE_LST)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    /* Adjust the index */
    if (index < 0)
    {
        index += ((pPmList_t)plist)->length;
    }

    /* Check the bounds of the index */
    if ((index < 0) || (index >= ((pPmList_t)plist)->length))
    {
        PM_RAISE(retval, PM_RET_EX_INDX);
        return retval;
    }

    /* Set the item */
    retval = seglist_setItem(((pPmList_t)plist)->val, pobj, index);
    return retval;
}


PmReturn_t
list_remove(pPmObj_t plist, pPmObj_t item)
{
    PmReturn_t retval = PM_RET_OK;
    uint16_t index;

    /* If it's not a list, raise TypeError */
    if (OBJ_GET_TYPE(plist) != OBJ_TYPE_LST)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    /* Locate the item to remove */
    retval = list_index(plist, item, &index);
    PM_RETURN_IF_ERROR(retval);

    /* Remove the item and decrement the list length */
    retval = list_removeIndex(plist, index);
    return retval;

}

PmReturn_t
list_removeIndex(pPmObj_t plist, int16_t index)
{
    PmReturn_t retval = PM_RET_OK;

    /* If it's not a list, raise TypeError */
    if (OBJ_GET_TYPE(plist) != OBJ_TYPE_LST)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    /* Handle negative indices correctly (count backwards from that end) */
    if (index < 0)
    {
        index += ((pPmList_t)plist)->length;
    }

    /* Remove the item and decrement the list length */
    retval = seglist_removeItem(((pPmList_t)plist)->val, index);
    ((pPmList_t)plist)->length--;
    return retval;

}


PmReturn_t
list_index(pPmObj_t plist, pPmObj_t pitem, uint16_t *r_index)
{
    PmReturn_t retval = PM_RET_OK;
    pSeglist_t pseglist;
    pPmObj_t pobj;
    uint16_t index;

    /* If it's not a list, raise TypeError */
    if (OBJ_GET_TYPE(plist) != OBJ_TYPE_LST)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    pseglist = ((pPmList_t)plist)->val;

    /* Iterate over the list's contents */
    for (index = 0; index < pseglist->sl_length; index++)
    {
        retval = seglist_getItem(pseglist, index, &pobj);
        PM_RETURN_IF_ERROR(retval);

        /* If the list item matches the given item, return the index */
        if (obj_compare(pobj, pitem) == C_SAME)
        {
            *r_index = index;
            return PM_RET_OK;
        }
    }

    return PM_RET_EX_VAL;
}

PmReturn_t
list_clear(pPmObj_t plist)
{
    PmReturn_t retval = PM_RET_OK;
    pSeglist_t pseglist;

    /* Ensure we're clearing a list */
    if (OBJ_GET_TYPE(plist) != OBJ_TYPE_LST)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
     }

     pseglist = ((pPmList_t)plist)->val;

     /* clear the list */
     retval = seglist_clear(pseglist);
     return retval;
}

PmReturn_t
list_storeSlice(pPmObj_t plist, int16_t startIndex, int16_t endIndex, pPmObj_t piter)
{
    PmReturn_t retval = PM_RET_OK;
    pPmObj_t pobj;
    int16_t t16;

    /* Handle negative indices correctly (count backwards from that end) */
    if (endIndex < 0)
    {
        endIndex += ((pPmList_t)plist)->length;
    }
    if (startIndex < 0)
    {
        startIndex += ((pPmList_t)plist)->length;
    }

    /* plist must be a list */
    if (OBJ_GET_TYPE(plist) != OBJ_TYPE_LST)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    /* Shorten the list if needed */
    /* Find out what's taken out (end - start) */
    t16 = (((pPmList_t)plist)->length) - endIndex + startIndex;
    /* Find out what's added in and do typechecking on piter*/
    if (OBJ_GET_TYPE(piter) == OBJ_TYPE_TUP)
    {
        t16 += ((pPmTuple_t)piter)->length;
    }
    else if (OBJ_GET_TYPE(piter) == OBJ_TYPE_LST)
    {
        t16 += ((pPmList_t)piter)->length;
    }
    else
    {
        /* piter isn't a tuple or a list */
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    /*printf("len(list)=%d len(other)=%d start=%d end=%d\n",((pPmList_t)plist)->length,t16+endIndex-startIndex-(((pPmList_t)plist)->length),startIndex,endIndex);*/

    /* If less than list's length */
    while (t16 < (((pPmList_t)plist)->length))
    {
        /* Delete list items from start of slice until it's the right size */
        retval = list_removeIndex(plist, startIndex);
        PM_RETURN_IF_ERROR(retval);
    }

    /* If greater than list's length */
    while (t16 > (((pPmList_t)plist)->length))
    {
        /* Add another item, update length */
        /* We use the int on the stack, because it's convenient */
        retval = list_insert(plist, startIndex, piter);
        PM_RETURN_IF_ERROR(retval);
    }

    /* Iterate and insert new elements */
    /* If iterable's a tuple */
    if (OBJ_GET_TYPE(piter) == OBJ_TYPE_TUP)
    {
        for (t16 = 0; t16 < (((pPmTuple_t)piter)->length); t16++)
        {
            /* Set the (N+offset)th element of the list to the Nth element of the iterable */
            retval = tuple_getItem(piter, t16, &pobj);
            PM_RETURN_IF_ERROR(retval);

            retval = list_setItem(plist, t16 + startIndex, pobj);
            PM_RETURN_IF_ERROR(retval);
        }
    }

    /* If iterable's a list */
    else if (OBJ_GET_TYPE(piter) == OBJ_TYPE_LST)
    {
        for (t16 = 0; t16 < (((pPmList_t)piter)->length); t16++)
        {
            /* Set the (N+offset)th element of the list to the Nth element of the iterable */
            retval = list_getItem(piter, t16, &pobj);
            PM_RETURN_IF_ERROR(retval);

            retval = list_setItem(plist, t16 + startIndex, pobj);
            PM_RETURN_IF_ERROR(retval);
        }
    }
    /* If here is reached, type checking succeed and operations succeeded.  There's no further error checking to do. */
    return retval;
}

PmReturn_t
list_getSlice(pPmObj_t plist, int16_t startIndex, int16_t endIndex, int16_t step, pPmObj_t *rlist)
{
    PmReturn_t retval = PM_RET_OK;
    pPmObj_t pobj;
    uint16_t i;

    /* Sanity check that slice step isn't 0 */
    if (step == 0)
    {
        PM_RAISE(retval, PM_RET_EX_VAL);
        return retval;
    }

    /* Handle negative indices correctly (count backwards from that end) */
    if (endIndex < 0)
    {
        endIndex += ((pPmList_t)plist)->length;
    }
    if (startIndex < 0)
    {
        startIndex += ((pPmList_t)plist)->length;
    }

    /* plist must be a list */
    if (OBJ_GET_TYPE(plist) != OBJ_TYPE_LST)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    retval = list_new(rlist);
    PM_RETURN_IF_ERROR(retval);

    /* Copy the middle bit, as requested */
    for (i = startIndex; i < endIndex; i+=step)
    {
        retval = list_getItem(plist, i, &pobj);
        PM_RETURN_IF_ERROR(retval);
        retval = list_append(*rlist, pobj);
        PM_RETURN_IF_ERROR(retval);
    }

    return retval;
}


#ifdef HAVE_PRINT
PmReturn_t
list_print(pPmObj_t plist)
{
    PmReturn_t retval = PM_RET_OK;
    int16_t index;
    pSeglist_t vals;
    pPmObj_t pstartIndex;

    C_ASSERT(plist != C_NULL);

    /* If it's not a list, raise TypeError */
    if (OBJ_GET_TYPE(plist) != OBJ_TYPE_LST)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    plat_putByte('[');

    vals = ((pPmList_t)plist)->val;

    /* Iterate over the list's contents */
    for (index = 0; index < ((pPmList_t)plist)->length; index++)
    {
        if (index != 0)
        {
            plat_putByte(',');
            plat_putByte(' ');
        }

        /* Print each item */
        retval = seglist_getItem(vals, index, &pstartIndex);
        PM_RETURN_IF_ERROR(retval);
        retval = obj_print(pstartIndex, 1);
        PM_RETURN_IF_ERROR(retval);
    }

    return plat_putByte(']');
}
#endif /* HAVE_PRINT */

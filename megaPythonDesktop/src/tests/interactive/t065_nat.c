#undef __FILE_ID__
#define __FILE_ID__ 0x0A
/**
 * PyMite usr native function file
 *
 * automatically created by pmImgCreator.py
 * on Fri Jan 11 13:20:26 2008
 *
 * DO NOT EDIT THIS FILE.
 * ANY CHANGES WILL BE LOST.
 *
 * @file    t065_nat.c
 */

#define __IN_LIBNATIVE_C__
#include "pm.h"

PmReturn_t
nat_placeholder_func(pPmFrame_t *ppframe)
{

    /*
     * Use placeholder because an index 
     * value of zero denotes the stdlib.
     * This function should not be called.
     */
    PmReturn_t retval;
    PM_RAISE(retval, PM_RET_EX_SYS);
    return retval;

}

/* native function lookup table */
PmReturn_t (* usr_nat_fxn_table[])(pPmFrame_t *) =
{
    nat_placeholder_func,
};
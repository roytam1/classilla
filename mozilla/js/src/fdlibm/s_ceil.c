/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * The contents of this file are subject to the Netscape Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is Mozilla Communicator client code, released
 * March 31, 1998.
 *
 * The Initial Developer of the Original Code is Sun Microsystems,
 * Inc.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation. All
 * Rights Reserved.
 *
 * Contributor(s): 
 *
 * Alternatively, the contents of this file may be used under the
 * terms of the GNU Public License (the "GPL"), in which case the
 * provisions of the GPL are applicable instead of those above.
 * If you wish to allow use of your version of this file only
 * under the terms of the GPL and not to allow others to use your
 * version of this file under the NPL, indicate your decision by
 * deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL.  If you do not delete
 * the provisions above, a recipient may use your version of this
 * file under either the NPL or the GPL.
 */

/* @(#)s_ceil.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

/*
 * ceil(x)
 * Return x rounded toward -inf to integral value
 * Method:
 *	Bit twiddling.
 * Exception:
 *	Inexact flag raised if x not equal to ceil(x).
 */

#include "fdlibm.h"

#ifdef __STDC__
static const double really_big = 1.0e300;
#else
static double really_big = 1.0e300;
#endif

#ifdef __STDC__
	double fd_ceil(double x)
#else
	double fd_ceil(x)
	double x;
#endif
{
        fd_twoints u;
	int i0,i1,j0;
	unsigned i,j;
        u.d = x;
	i0 =  __HI(u);
	i1 =  __LO(u);
	j0 = ((i0>>20)&0x7ff)-0x3ff;
	if(j0<20) {
	    if(j0<0) { 	/* raise inexact if x != 0 */
		if(really_big+x>0.0) {/* return 0*sign(x) if |x|<1 */
		    if(i0<0) {i0=0x80000000;i1=0;} 
		    else if((i0|i1)!=0) { i0=0x3ff00000;i1=0;}
		}
	    } else {
		i = (0x000fffff)>>j0;
		if(((i0&i)|i1)==0) return x; /* x is integral */
		if(really_big+x>0.0) {	/* raise inexact flag */
		    if(i0>0) i0 += (0x00100000)>>j0;
		    i0 &= (~i); i1=0;
		}
	    }
	} else if (j0>51) {
	    if(j0==0x400) return x+x;	/* inf or NaN */
	    else return x;		/* x is integral */
	} else {
	    i = ((unsigned)(0xffffffff))>>(j0-20);
	    if((i1&i)==0) return x;	/* x is integral */
	    if(really_big+x>0.0) { 		/* raise inexact flag */
		if(i0>0) {
		    if(j0==20) i0+=1; 
		    else {
			j = i1 + (1<<(52-j0));
			if((int)j<i1) i0+=1;	/* got a carry */
			i1 = j;
		    }
		}
		i1 &= (~i);
	    }
	}
        u.d = x;
	__HI(u) = i0;
	__LO(u) = i1;
        x = u.d;
	return x;
}

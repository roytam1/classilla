/*
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 * 
 * The Original Code is the Netscape security libraries.
 * 
 * The Initial Developer of the Original Code is Netscape
 * Communications Corporation.  Portions created by Netscape are 
 * Copyright (C) 1994-2000 Netscape Communications Corporation.  All
 * Rights Reserved.
 * 
 * Contributor(s):
 * 
 * Alternatively, the contents of this file may be used under the
 * terms of the GNU General Public License Version 2 or later (the
 * "GPL"), in which case the provisions of the GPL are applicable 
 * instead of those above.  If you wish to allow use of your 
 * version of this file only under the terms of the GPL and not to
 * allow others to use your version of this file under the MPL,
 * indicate your decision by deleting the provisions above and
 * replace them with the notice and other provisions required by
 * the GPL.  If you do not delete the provisions above, a recipient
 * may use your version of this file under either the MPL or the
 * GPL.
 */

#ifndef _GENAME_H_
#define _GENAME_H_

#include "plarena.h"
#include "seccomon.h"
#include "secoidt.h"
#include "secasn1.h"
#include "secder.h"
#include "certt.h"

/************************************************************************/
SEC_BEGIN_PROTOS

extern const SEC_ASN1Template CERT_GeneralNamesTemplate[];

extern CERTGeneralName *
cert_get_next_general_name(CERTGeneralName *current);

extern CERTGeneralName *
cert_get_prev_general_name(CERTGeneralName *current);

extern SECItem *
CERT_EncodeGeneralName(CERTGeneralName *genName, SECItem *dest,
		       PRArenaPool *arena);

extern SECItem **
cert_EncodeGeneralNames(PRArenaPool *arena, CERTGeneralName *names);

extern CERTGeneralName *
CERT_DecodeGeneralName(PRArenaPool *arena, SECItem *encodedName,
		       CERTGeneralName  *genName);

extern CERTGeneralName *
cert_DecodeGeneralNames(PRArenaPool *arena, SECItem **encodedGenName);

extern SECStatus
cert_DestroyGeneralNames(CERTGeneralName *name);

extern SECStatus 
cert_EncodeNameConstraints(CERTNameConstraints *constraints, PRArenaPool *arena,
			   SECItem *dest);

extern CERTNameConstraints *
cert_DecodeNameConstraints(PRArenaPool *arena, SECItem *encodedConstraints);

extern CERTGeneralName *
cert_CombineNamesLists(CERTGeneralName *list1, CERTGeneralName *list2);

extern CERTNameConstraint *
cert_CombineConstraintsLists(CERTNameConstraint *list1, CERTNameConstraint *list2);

SECStatus
CERT_CompareGeneralName(CERTGeneralName *a, CERTGeneralName *b);

SECStatus
CERT_CopyGeneralName(PRArenaPool      *arena, 
		     CERTGeneralName  *dest, 
		     CERTGeneralName  *src);

/* General Name Lists are a thread safe, reference counting layer to 
 * general names */

void
CERT_DestroyGeneralNameList(CERTGeneralNameList *list);

CERTGeneralNameList *
CERT_CreateGeneralNameList(CERTGeneralName *name);

SECStatus
CERT_CompareGeneralNameLists(CERTGeneralNameList *a, CERTGeneralNameList *b);

void *
CERT_GetGeneralNameFromListByType(CERTGeneralNameList *list,
				  CERTGeneralNameType type,
				  PRArenaPool *arena);

void
CERT_AddGeneralNameToList(CERTGeneralNameList *list, 
			  CERTGeneralNameType type,
			  void *data, SECItem *oid);


CERTGeneralNameList *
CERT_DupGeneralNameList(CERTGeneralNameList *list);


int
CERT_GetNamesLength(CERTGeneralName *names);
/************************************************************************/
SEC_END_PROTOS

#endif

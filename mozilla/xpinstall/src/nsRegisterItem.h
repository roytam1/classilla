/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
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
 * The Original Code is Mozilla Communicator client code, 
 * released March 31, 1998. 
 *
 * The Initial Developer of the Original Code is Netscape Communications 
 * Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation. All
 * Rights Reserved.
 *
 * Contributor(s): 
 *     Daniel Veditz <dveditz@netscape.com>
 *     Douglas Turner <dougt@netscape.com>
 */


#ifndef nsRegisterItem_h__
#define nsRegisterItem_h__

#include "prtypes.h"
#include "nsString.h"
#include "nsInstall.h"
#include "nsInstallObject.h"


class nsRegisterItem : public nsInstallObject 
{
    public:

        nsRegisterItem( nsInstall* install,
                        nsIFile* chrome,
                        PRUint32 chromeType,
                        const char* path );

        virtual ~nsRegisterItem();

        PRInt32 Prepare();
        PRInt32 Complete();
        void  Abort();
        char* toString();

        PRBool CanUninstall();
        PRBool RegisterPackageNode();

    private:
        nsresult GetURLFromIFile(nsIFile *aFile, char **aOutURL);

        nsCString mURL;
        nsCOMPtr<nsIFile> mChrome;
        PRUint32  mChromeType;
        nsCOMPtr<nsIFile> mProgDir;
        nsCString mPath; 
};

#endif /* nsRegisterItem_h__ */

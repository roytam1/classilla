/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Mozilla Communicator.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 2002
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include "nsFormSigningDialog.h"
#include "nsNSSDialogHelper.h"
#include "nsCOMPtr.h"
#include "nsIDialogParamBlock.h"
#include "nsIComponentManager.h"
#include "nsIServiceManager.h"
#include "nsIInterfaceRequestor.h"
#include "nsIInterfaceRequestorUtils.h"
#include "nsLiteralString.h"
#include "nsString2.h"

NS_IMPL_THREADSAFE_ISUPPORTS1(nsFormSigningDialog, nsIFormSigningDialog);

nsFormSigningDialog::nsFormSigningDialog()
{
  NS_INIT_ISUPPORTS();
}

nsFormSigningDialog::~nsFormSigningDialog()
{
}

NS_IMETHODIMP
nsFormSigningDialog::ConfirmSignText(nsIInterfaceRequestor *ctx, 
  const nsAString &host, const nsAString &signText,
  const PRUnichar **certNickList, const PRUnichar **certDetailsList, PRUint32 count, 
  PRInt32 *selectedIndex, nsAString &password, PRBool *canceled) 
{
  nsresult rv;
  PRUint32 i;

  *canceled = PR_TRUE;

  // Get the parent window for the dialog
  nsCOMPtr<nsIDOMWindowInternal> parent = do_GetInterface(ctx);

  nsCOMPtr<nsIDialogParamBlock> block(do_CreateInstance(NS_DIALOGPARAMBLOCK_CONTRACTID));
  if (!block) return NS_ERROR_FAILURE;

  block->SetNumberStrings(3+count*2);

  rv = block->SetString(0, PromiseFlatString(host).get());
  if (NS_FAILED(rv)) return rv;

  rv = block->SetString(1, PromiseFlatString(signText).get());
  if (NS_FAILED(rv)) return rv;

  for (i = 0; i < count; i++) {
	  rv = block->SetString(i+2, certNickList[i]);
	  if (NS_FAILED(rv)) return rv;
  }

  for (i = 0; i < count; i++) {
	  rv = block->SetString(i+count+2, certDetailsList[i]);
	  if (NS_FAILED(rv)) return rv;
  }

  rv = block->SetInt(0, count);
  if (NS_FAILED(rv)) return rv;

  rv = nsNSSDialogHelper::openDialog(nsnull,
                                "chrome://pippki/content/formsigning.xul",
                                block);
  if (NS_FAILED(rv)) return rv;

  PRInt32 status;

  rv = block->GetInt(0, &status);
  if (NS_FAILED(rv)) return rv;

  *canceled = (status == 0)?PR_TRUE:PR_FALSE;
  if (!*canceled) {
    rv = block->GetInt(1, selectedIndex);
    if (NS_FAILED(rv)) return rv;

		PRUnichar * pw;
    rv = block->GetString(0, &pw);
    if (NS_FAILED(rv)) return rv;

		password = pw;
		nsMemory::Free(pw);
  }
  return rv;
}

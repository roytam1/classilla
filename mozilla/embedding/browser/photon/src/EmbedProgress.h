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
 * The Original Code is mozilla.org code.
 * 
 * The Initial Developer of the Original Code is Christopher Blizzard.
 * Portions created by Christopher Blizzard are Copyright (C)
 * Christopher Blizzard.  All Rights Reserved.
 * 
 * Contributor(s):
 *   Christopher Blizzard <blizzard@mozilla.org>
 *   Brian Edmond <briane@qnx.com>
 */

#ifndef __EmbedProgress_h
#define __EmbedProgress_h

#include <nsIWebProgressListener.h>
#include <nsWeakReference.h>
#include "EmbedPrivate.h"

class EmbedProgress : public nsIWebProgressListener,
                      public nsSupportsWeakReference
{
 public:
  EmbedProgress();
  virtual ~EmbedProgress();

	PRBool mSkipOnState, mDownloadDocument;

  nsresult Init(EmbedPrivate *aOwner);

  NS_DECL_ISUPPORTS

  NS_DECL_NSIWEBPROGRESSLISTENER

 private:

  static void RequestToURIString (nsIRequest *aRequest, char **aString);

  EmbedPrivate *mOwner;

};

#endif /* __EmbedProgress_h */

/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * Pango
 * pango-engine.h: Module handling
 *
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
 * The Original Code is the Pango Library (www.pango.org) 
 * 
 * The Initial Developer of the Original Code is Red Hat Software
 * Portions created by Red Hat are Copyright (C) 2000 Red Hat Software.
 * 
 * Contributor(s): 
 * 
 * Alternatively, the contents of this file may be used under the
 * terms of the GNU Lessor General Public License Version 2 (the 
 * "LGPL"), in which case the provisions of the LGPL are applicable 
 * instead of those above.  If you wish to allow use of your 
 * version of this file only under the terms of the LGPL and not to
 * allow others to use your version of this file under the MPL,
 * indicate your decision by deleting the provisions above and
 * replace them with the notice and other provisions required by
 * the LGPL. If you do not delete the provisions above, a recipient
 * may use your version of this file under either the MPL or the
 * LGPL.
*/

#ifndef __PANGO_ENGINE_H__
#define __PANGO_ENGINE_H__

#include "pango-types.h"
#include "pango-glyph.h"
#include "pango-coverage.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Module API */

#define PANGO_ENGINE_TYPE_SHAPE "PangoEngineShape"
#define PANGO_RENDER_TYPE_X    "PangoRenderX"
#define PANGO_RENDER_TYPE_NONE "PangoRenderNone"

typedef struct _PangoEngineInfo PangoEngineInfo;
typedef struct _PangoEngineRange PangoEngineRange;
typedef struct _PangoEngine PangoEngine;

struct _PangoEngineRange 
{
  guint32 start;
  guint32 end;
  gchar   *langs;
};

struct _PangoEngineInfo
{
  gchar            *id;
  gchar            *engine_type;
  gchar            *render_type;
  PangoEngineRange *ranges;
  gint             n_ranges;
};

struct _PangoEngine
{
  gchar *id;
  gchar *type;
  gint  length;
};

struct _PangoEngineShape
{
  PangoEngine engine;
  void (*script_shape) (const char       *fontCharset, 
                        const gunichar2  *text, 
                        int              length, 
                        PangoAnalysis    *analysis, 
                        PangoGlyphString *glyphs);
  PangoCoverage *(*get_coverage) (const char *fontCharset, const char *lang);

};

/* A module should export the following functions */
void         script_engine_list(PangoEngineInfo **engines, int *n_engines);
PangoEngine *script_engine_load(const char *id);
void         script_engine_unload(PangoEngine *engine);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PANGO_ENGINE_H__ */

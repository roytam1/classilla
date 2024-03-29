#! /bin/sh
#
# The contents of this file are subject to the Netscape Public
# License Version 1.1 (the "License"); you may not use this file
# except in compliance with the License. You may obtain a copy of
# the License at http://www.mozilla.org/NPL/
#
# Software distributed under the License is distributed on an "AS
# IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
# implied. See the License for the specific language governing
# rights and limitations under the License.
#
# The Original Code is mozilla.org code.
#
# The Initial Developer of the Original Code is Netscape
# Communications Corporation.  Portions created by Netscape are
# Copyright (C) 1999 Netscape Communications Corporation. All
# Rights Reserved.
#
# Contributor(s): 
#

# allmakefiles.sh - List of all makefiles. 
#   Appends the list of makefiles to the variable, MAKEFILES.
#   There is no need to rerun autoconf after adding makefiles.
#   You only need to run configure.
#
#   Please keep the modules in this file in sync with those in
#   mozilla/build/unix/modules.mk
#

MAKEFILES=""

# add_makefiles - Shell function to add makefiles to MAKEFILES
add_makefiles() {
    MAKEFILES="$MAKEFILES $*"
}

if [ "$srcdir" = "" ]; then
    srcdir=.
fi

#
# Common makefiles used by everyone
#
add_makefiles "
Makefile
build/Makefile
build/unix/Makefile
config/Makefile
config/autoconf.mk
config/mkdepend/Makefile
config/doxygen.cfg
include/Makefile
"

if [ "$MOZ_COMPOSER" ]; then
MAKEFILES_composer="
editor/composer/Makefile
editor/ui/Makefile
"
fi

MAKEFILES_db="
db/Makefile
db/mdb/Makefile
db/mdb/public/Makefile
db/mork/Makefile
db/mork/build/Makefile
db/mork/src/Makefile
"

MAKEFILES_dbm="
dbm/Makefile
dbm/include/Makefile
dbm/src/Makefile
dbm/tests/Makefile
"

MAKEFILES_dom="
dom/Makefile
dom/public/Makefile
dom/public/base/Makefile
dom/public/coreEvents/Makefile
dom/public/idl/Makefile
dom/public/idl/base/Makefile
dom/public/idl/core/Makefile
dom/public/idl/css/Makefile
dom/public/idl/events/Makefile
dom/public/idl/html/Makefile
dom/public/idl/range/Makefile
dom/public/idl/stylesheets/Makefile
dom/public/idl/views/Makefile
dom/public/idl/xbl/Makefile
dom/public/idl/xpath/Makefile
dom/public/idl/xul/Makefile
dom/src/Makefile
dom/src/base/Makefile
dom/src/build/Makefile
dom/src/events/Makefile
dom/src/jsurl/Makefile
"

MAKEFILES_editor="
editor/Makefile
editor/public/Makefile
editor/idl/Makefile
editor/txmgr/Makefile
editor/txmgr/idl/Makefile
editor/txmgr/public/Makefile
editor/txmgr/src/Makefile
editor/txmgr/tests/Makefile
editor/txtsvc/Makefile
editor/txtsvc/public/Makefile
editor/txtsvc/src/Makefile
"

MAKEFILES_expat="
expat/Makefile
expat/xmlparse/Makefile
expat/xmltok/Makefile
"

MAKEFILES_extensions="
extensions/Makefile
"

MAKEFILES_gc="
gc/boehm/Makefile
gc/boehm/leaksoup/Makefile
"

MAKEFILES_gfx="
gfx/Makefile
gfx/idl/Makefile
gfx/public/Makefile
gfx/src/Makefile
gfx/src/beos/Makefile
gfx/src/gtk/Makefile
gfx/src/ps/Makefile
gfx/src/photon/Makefile
gfx/src/mac/Makefile
gfx/src/qt/Makefile
gfx/src/xlib/Makefile
gfx/src/os2/Makefile
gfx/src/xlibrgb/Makefile
gfx/src/windows/Makefile
gfx/tests/Makefile
"

MAKEFILES_htmlparser="
htmlparser/Makefile
htmlparser/robot/Makefile
htmlparser/robot/test/Makefile
htmlparser/public/Makefile
htmlparser/src/Makefile
htmlparser/tests/Makefile
htmlparser/tests/grabpage/Makefile
htmlparser/tests/logparse/Makefile
htmlparser/tests/html/Makefile
htmlparser/tests/outsinks/Makefile
"

MAKEFILES_intl="
intl/Makefile
intl/chardet/Makefile
intl/chardet/public/Makefile
intl/chardet/src/Makefile
intl/uconv/Makefile
intl/uconv/idl/Makefile
intl/uconv/public/Makefile
intl/uconv/src/Makefile
intl/uconv/tests/Makefile
intl/uconv/ucvja/Makefile
intl/uconv/ucvlatin/Makefile
intl/uconv/ucvcn/Makefile
intl/uconv/ucvtw/Makefile
intl/uconv/ucvtw2/Makefile
intl/uconv/ucvko/Makefile
intl/uconv/ucvibm/Makefile
intl/locale/Makefile
intl/locale/public/Makefile
intl/locale/idl/Makefile
intl/locale/src/Makefile
intl/locale/src/unix/Makefile
intl/locale/src/os2/Makefile
intl/locale/src/windows/Makefile
intl/locale/tests/Makefile
intl/lwbrk/Makefile
intl/lwbrk/src/Makefile
intl/lwbrk/public/Makefile
intl/lwbrk/tests/Makefile
intl/unicharutil/Makefile
intl/unicharutil/idl/Makefile
intl/unicharutil/src/Makefile
intl/unicharutil/public/Makefile
intl/unicharutil/tables/Makefile
intl/unicharutil/tests/Makefile
intl/unicharutil/tools/Makefile
intl/strres/Makefile
intl/strres/public/Makefile
intl/strres/src/Makefile
intl/strres/tests/Makefile
"

if [ "$SUNCTL" ] ; then
MAKEFILES_intl="$MAKEFILES_intl
intl/ctl/Makefile
intl/ctl/public/Makefile
intl/ctl/src/Makefile
intl/ctl/src/pangoLite/Makefile
intl/ctl/src/thaiShaper/Makefile
intl/ctl/src/hindiShaper/Makefile
"
fi

MAKEFILES_js="
js/Makefile
js/src/Makefile
js/src/fdlibm/Makefile
"

MAKEFILES_liveconnect="
js/src/liveconnect/Makefile
js/src/liveconnect/classes/Makefile
"

MAKEFILES_xpconnect="
js/src/xpconnect/Makefile
js/src/xpconnect/public/Makefile
js/src/xpconnect/idl/Makefile
js/src/xpconnect/shell/Makefile
js/src/xpconnect/src/Makefile
js/src/xpconnect/loader/Makefile
js/src/xpconnect/tests/Makefile
js/src/xpconnect/tests/components/Makefile
js/src/xpconnect/tests/idl/Makefile
js/src/xpconnect/shell/Makefile
js/src/xpconnect/tools/Makefile
js/src/xpconnect/tools/idl/Makefile
js/src/xpconnect/tools/idl/Makefile
"

MAKEFILES_jsdebugger="
js/jsd/Makefile
js/jsd/idl/Makefile
"

MAKEFILES_content="
content/Makefile
content/base/Makefile
content/base/public/Makefile
content/base/src/Makefile
content/events/Makefile
content/events/public/Makefile
content/events/src/Makefile
content/html/Makefile
content/html/content/Makefile
content/html/content/public/Makefile
content/html/content/src/Makefile
content/html/document/Makefile
content/html/document/public/Makefile
content/html/document/src/Makefile
content/html/style/Makefile
content/html/style/public/Makefile
content/html/style/src/Makefile
content/xml/Makefile
content/xml/content/Makefile
content/xml/content/public/Makefile
content/xml/content/src/Makefile
content/xml/document/Makefile
content/xml/document/public/Makefile
content/xml/document/src/Makefile
content/xul/Makefile
content/xul/content/Makefile
content/xul/content/public/Makefile
content/xul/content/src/Makefile
content/xul/document/Makefile
content/xul/document/public/Makefile
content/xul/document/src/Makefile
content/xul/templates/public/Makefile
content/xul/templates/src/Makefile
content/xbl/Makefile
content/xbl/public/Makefile
content/xbl/src/Makefile
content/xbl/builtin/Makefile
content/xbl/builtin/unix/Makefile
content/xbl/builtin/win/Makefile
content/xsl/Makefile
content/xsl/document/Makefile
content/xsl/document/src/Makefile
content/shared/Makefile
content/shared/public/Makefile
content/shared/src/Makefile
"

MAKEFILES_layout="
layout/Makefile
layout/base/Makefile
layout/base/public/Makefile
layout/base/src/Makefile
layout/base/tests/Makefile
layout/build/Makefile
layout/html/Makefile
layout/html/base/Makefile
layout/html/base/src/Makefile
layout/html/document/Makefile
layout/html/document/src/Makefile
layout/html/forms/Makefile
layout/html/forms/public/Makefile
layout/html/forms/src/Makefile
layout/html/style/Makefile
layout/html/style/src/Makefile
layout/html/table/Makefile
layout/html/table/public/Makefile
layout/html/table/src/Makefile
layout/html/tests/Makefile
layout/tools/Makefile
layout/xul/Makefile
layout/xul/base/Makefile
layout/xul/base/public/Makefile
layout/xul/base/src/Makefile
layout/xul/base/src/tree/Makefile
layout/xul/base/src/tree/src/Makefile
layout/xul/base/src/tree/public/Makefile
"

MAKEFILES_libimg="
modules/libimg/Makefile
"

MAKEFILES_libjar="
modules/libjar/Makefile
modules/libjar/standalone/Makefile
"

MAKEFILES_libreg="
modules/libreg/Makefile
modules/libreg/include/Makefile
modules/libreg/src/Makefile
"

MAKEFILES_libpref="
modules/libpref/Makefile
modules/libpref/public/Makefile
modules/libpref/src/Makefile
"

MAKEFILES_libutil="
modules/libutil/Makefile
modules/libutil/public/Makefile
modules/libutil/src/Makefile
"

MAKEFILES_oji="
modules/oji/Makefile
modules/oji/public/Makefile
modules/oji/src/Makefile
"

MAKEFILES_plugin="
modules/plugin/Makefile
modules/plugin/base/src/Makefile
modules/plugin/base/public/Makefile
modules/plugin/samples/simple/Makefile
modules/plugin/samples/SanePlugin/Makefile
modules/plugin/samples/default/unix/Makefile
modules/plugin/tools/sdk/Makefile
modules/plugin/tools/sdk/samples/Makefile
modules/plugin/tools/sdk/samples/common/Makefile
modules/plugin/tools/sdk/samples/basic/windows/Makefile
modules/plugin/tools/sdk/samples/scriptable/windows/Makefile
modules/plugin/tools/sdk/samples/simple/Makefile
modules/plugin/tools/sdk/samples/winless/windows/Makefile
"

MAKEFILES_access_builtin="
extensions/access-builtin/Makefile
extensions/access-builtin/accessproxy/Makefile
"

MAKEFILES_netwerk="
netwerk/Makefile
netwerk/base/Makefile
netwerk/base/public/Makefile
netwerk/base/src/Makefile
netwerk/build/Makefile
netwerk/build2/Makefile
netwerk/cache/Makefile
netwerk/cache/public/Makefile
netwerk/cache/src/Makefile
netwerk/dns/Makefile
netwerk/dns/public/Makefile
netwerk/dns/src/Makefile
netwerk/protocol/Makefile
netwerk/protocol/about/Makefile
netwerk/protocol/about/public/Makefile
netwerk/protocol/about/src/Makefile
netwerk/protocol/data/Makefile
netwerk/protocol/data/public/Makefile
netwerk/protocol/data/src/Makefile
netwerk/protocol/file/Makefile
netwerk/protocol/file/public/Makefile
netwerk/protocol/file/src/Makefile
netwerk/protocol/ftp/Makefile
netwerk/protocol/ftp/public/Makefile
netwerk/protocol/ftp/src/Makefile
netwerk/protocol/gopher/Makefile
netwerk/protocol/gopher/src/Makefile
netwerk/protocol/http/Makefile
netwerk/protocol/http/public/Makefile
netwerk/protocol/http/src/Makefile
netwerk/protocol/jar/Makefile
netwerk/protocol/jar/public/Makefile
netwerk/protocol/jar/src/Makefile
netwerk/protocol/keyword/Makefile
netwerk/protocol/keyword/src/Makefile
netwerk/protocol/res/Makefile
netwerk/protocol/res/public/Makefile
netwerk/protocol/res/src/Makefile
netwerk/mime/Makefile
netwerk/mime/public/Makefile
netwerk/mime/src/Makefile
netwerk/socket/Makefile
netwerk/socket/base/Makefile
netwerk/streamconv/Makefile
netwerk/streamconv/converters/Makefile
netwerk/streamconv/public/Makefile
netwerk/streamconv/src/Makefile
netwerk/streamconv/test/Makefile
netwerk/test/Makefile
netwerk/testserver/Makefile
netwerk/resources/Makefile

uriloader/exthandler/Makefile
intl/strres/public/Makefile
intl/locale/idl/Makefile
$MAKEFILES_js
modules/libpref/public/Makefile
"

MAKEFILES_uriloader="
uriloader/Makefile
uriloader/base/Makefile
uriloader/build/Makefile
uriloader/exthandler/Makefile
"

MAKEFILES_profile="
profile/Makefile
profile/src/Makefile
profile/public/Makefile
profile/resources/Makefile
profile/pref-migrator/Makefile
profile/pref-migrator/public/Makefile
profile/pref-migrator/src/Makefile
profile/pref-migrator/resources/Makefile
profile/defaults/Makefile
profile/dirserviceprovider/Makefile
profile/dirserviceprovider/public/Makefile
profile/dirserviceprovider/src/Makefile
"

MAKEFILES_rdf="
rdf/Makefile
rdf/base/Makefile
rdf/base/idl/Makefile
rdf/base/public/Makefile
rdf/base/src/Makefile
rdf/chrome/Makefile
rdf/chrome/build/Makefile
rdf/chrome/public/Makefile
rdf/chrome/src/Makefile
rdf/chrome/tools/Makefile
rdf/chrome/tools/chromereg/Makefile
rdf/util/Makefile
rdf/util/public/Makefile
rdf/util/src/Makefile
rdf/resources/Makefile
rdf/build/Makefile
rdf/datasource/Makefile
rdf/datasource/public/Makefile
rdf/datasource/src/Makefile
rdf/tests/Makefile
rdf/tests/localfile/Makefile
rdf/tests/rdfsink/Makefile
rdf/tests/rdfcat/Makefile
rdf/tests/rdfpoll/Makefile
"

MAKEFILES_sun_java="
sun-java/Makefile
sun-java/stubs/Makefile
sun-java/stubs/include/Makefile
sun-java/stubs/jri/Makefile
"

MAKEFILES_caps="
caps/Makefile
caps/idl/Makefile
caps/include/Makefile
caps/src/Makefile
"

MAKEFILES_view="
view/Makefile
view/public/Makefile
view/src/Makefile
"

MAKEFILES_docshell="
docshell/Makefile
docshell/base/Makefile
docshell/build/Makefile
"

MAKEFILES_webshell="
webshell/Makefile
webshell/public/Makefile
webshell/tests/Makefile
webshell/tests/viewer/Makefile
webshell/tests/viewer/public/Makefile
webshell/tests/viewer/unix/Makefile
webshell/tests/viewer/unix/gtk/Makefile
webshell/tests/viewer/unix/qt/Makefile
webshell/tests/viewer/unix/xlib/Makefile
"

MAKEFILES_widget="
widget/Makefile
widget/public/Makefile
widget/src/Makefile
widget/src/beos/Makefile
widget/src/build/Makefile
widget/src/gtk/Makefile
widget/src/gtk2/mai/Makefile
widget/src/gtksuperwin/Makefile
widget/src/gtkxtbin/Makefile
widget/src/photon/Makefile
widget/src/mac/Makefile
widget/src/cocoa/Makefile
widget/src/xlib/Makefile
widget/src/os2/Makefile
widget/src/os2/tests/Makefile
widget/src/qt/Makefile
widget/src/windows/Makefile
widget/src/windows/expose/Makefile
widget/src/windows/expose/ISimpleDOMNode/Makefile
widget/src/windows/expose/ISimpleDOMDocument/Makefile
widget/src/xlibxtbin/Makefile
widget/src/xpwidgets/Makefile
widget/src/support/Makefile
"

MAKEFILES_xpcom="
string/Makefile
string/obsolete/Makefile
string/public/Makefile
string/src/Makefile
string/embed/Makefile
string/embed/standalone/Makefile
xpcom/Makefile
xpcom/base/Makefile
xpcom/build/Makefile
xpcom/components/Makefile
xpcom/ds/Makefile
xpcom/glue/Makefile
xpcom/glue/standalone/Makefile
xpcom/io/Makefile
xpcom/typelib/Makefile
xpcom/reflect/Makefile
xpcom/typelib/xpt/Makefile
xpcom/typelib/xpt/public/Makefile
xpcom/typelib/xpt/src/Makefile
xpcom/typelib/xpt/tests/Makefile
xpcom/typelib/xpt/tools/Makefile
xpcom/typelib/xpidl/Makefile
xpcom/reflect/xptcall/Makefile
xpcom/reflect/xptcall/public/Makefile
xpcom/reflect/xptcall/src/Makefile
xpcom/reflect/xptcall/src/md/Makefile
xpcom/reflect/xptcall/src/md/os2/Makefile
xpcom/reflect/xptcall/src/md/test/Makefile
xpcom/reflect/xptcall/src/md/unix/Makefile
xpcom/reflect/xptcall/src/md/win32/Makefile
xpcom/reflect/xptcall/tests/Makefile
xpcom/reflect/xptinfo/Makefile
xpcom/reflect/xptinfo/public/Makefile
xpcom/reflect/xptinfo/src/Makefile
xpcom/reflect/xptinfo/tests/Makefile
xpcom/proxy/Makefile
xpcom/proxy/public/Makefile
xpcom/proxy/src/Makefile
xpcom/proxy/tests/Makefile
xpcom/sample/Makefile
xpcom/threads/Makefile
xpcom/tools/Makefile
xpcom/tools/registry/Makefile
xpcom/tools/windows/Makefile
xpcom/windbgdlg/Makefile

$MAKEFILES_libreg
$MAKEFILES_libjar
intl/unicharutil/public/Makefile
intl/uconv/public/Makefile
netwerk/base/public/Makefile
netwerk/build/Makefile
"

MAKEFILES_xpcom_tests="
xpcom/tests/Makefile
xpcom/tests/dynamic/Makefile
xpcom/tests/services/Makefile
xpcom/tests/windows/Makefile
"

MAKEFILES_string="$MAKEFILES_xpcom"

MAKEFILES_xpinstall="
xpinstall/Makefile
xpinstall/packager/Makefile
xpinstall/public/Makefile
xpinstall/res/Makefile
xpinstall/src/Makefile
xpinstall/stub/Makefile
xpinstall/wizard/libxpnet/Makefile
xpinstall/wizard/libxpnet/src/Makefile
xpinstall/wizard/libxpnet/test/Makefile
xpinstall/wizard/unix/src2/Makefile
xpinstall/wizard/windows/nsinstall/Makefile
xpinstall/wizard/windows/nsztool/Makefile
xpinstall/wizard/windows/uninstall/Makefile
xpinstall/wizard/windows/setup/Makefile
xpinstall/wizard/windows/setuprsc/Makefile
xpinstall/wizard/windows/ren8dot3/Makefile
xpinstall/wizard/windows/ds32/Makefile
xpinstall/wizard/windows/GetShortPathName/Makefile
"

MAKEFILES_xpfe="
xpfe/Makefile
xpfe/browser/Makefile
xpfe/browser/public/Makefile
xpfe/browser/src/Makefile
xpfe/browser/samples/Makefile
xpfe/browser/samples/sampleimages/Makefile
xpfe/components/Makefile
xpfe/components/shistory/Makefile
xpfe/components/shistory/public/Makefile
xpfe/components/shistory/src/Makefile
xpfe/components/bookmarks/Makefile
xpfe/components/bookmarks/public/Makefile
xpfe/components/bookmarks/src/Makefile
xpfe/components/directory/Makefile
xpfe/components/download-manager/Makefile
xpfe/components/download-manager/src/Makefile
xpfe/components/download-manager/public/Makefile
xpfe/components/timebomb/Makefile
xpfe/components/timebomb/tools/Makefile
xpfe/components/regviewer/Makefile
xpfe/components/find/Makefile
xpfe/components/find/public/Makefile
xpfe/components/find/src/Makefile
xpfe/components/filepicker/Makefile
xpfe/components/filepicker/public/Makefile
xpfe/components/filepicker/src/Makefile
xpfe/components/history/Makefile
xpfe/components/history/src/Makefile
xpfe/components/history/public/Makefile
xpfe/components/prefwindow/Makefile
xpfe/components/prefwindow/resources/Makefile
xpfe/components/prefwindow/resources/content/Makefile
xpfe/components/prefwindow/resources/content/unix/Makefile
xpfe/components/prefwindow/resources/content/win/Makefile
xpfe/components/prefwindow/resources/locale/Makefile
xpfe/components/prefwindow/resources/locale/en-US/Makefile
xpfe/components/prefwindow/resources/locale/en-US/unix/Makefile
xpfe/components/prefwindow/resources/locale/en-US/win/Makefile
xpfe/components/related/Makefile
xpfe/components/related/src/Makefile
xpfe/components/related/public/Makefile
xpfe/components/search/Makefile
xpfe/components/search/datasets/Makefile
xpfe/components/search/public/Makefile
xpfe/components/search/src/Makefile
xpfe/components/sidebar/Makefile
xpfe/components/sidebar/public/Makefile
xpfe/components/sidebar/src/Makefile
xpfe/components/autocomplete/Makefile
xpfe/components/autocomplete/public/Makefile
xpfe/components/autocomplete/src/Makefile
xpfe/components/urlbarhistory/Makefile
xpfe/components/urlbarhistory/public/Makefile
xpfe/components/urlbarhistory/src/Makefile
xpfe/components/urlwidget/Makefile
xpfe/components/winhooks/Makefile
xpfe/components/alerts/Makefile
xpfe/components/alerts/public/Makefile
xpfe/components/alerts/src/Makefile
xpfe/components/console/Makefile
xpfe/components/resetPref/Makefile
xpfe/components/killAll/Makefile
xpfe/components/build/Makefile
xpfe/appshell/Makefile
xpfe/appshell/src/Makefile
xpfe/appshell/public/Makefile
xpfe/bootstrap/Makefile
xpfe/browser/Makefile
xpfe/browser/src/Makefile
xpfe/browser/resources/Makefile
xpfe/browser/resources/content/Makefile
xpfe/browser/resources/content/unix/Makefile
xpfe/browser/resources/content/win/Makefile
xpfe/browser/resources/locale/Makefile
xpfe/browser/resources/locale/en-US/Makefile
xpfe/browser/resources/locale/en-US/unix/Makefile
xpfe/global/Makefile
xpfe/global/buildconfig.html
xpfe/global/resources/Makefile
xpfe/global/resources/content/Makefile
xpfe/global/resources/content/os2/Makefile
xpfe/global/resources/content/unix/Makefile
xpfe/global/resources/locale/Makefile
xpfe/global/resources/locale/en-US/Makefile
xpfe/global/resources/locale/en-US/mac/Makefile
xpfe/global/resources/locale/en-US/os2/Makefile
xpfe/global/resources/locale/en-US/unix/Makefile
xpfe/global/resources/locale/en-US/win/Makefile
xpfe/communicator/Makefile
xpfe/communicator/resources/Makefile
xpfe/communicator/resources/locale/Makefile
xpfe/communicator/resources/locale/en-US/Makefile
xpfe/communicator/resources/content/Makefile
xpfe/communicator/resources/content/unix/Makefile
xpfe/communicator/resources/content/win/Makefile
"

MAKEFILES_embedding="
embedding/Makefile
embedding/base/Makefile
embedding/browser/Makefile
embedding/browser/activex/src/Makefile
embedding/browser/activex/src/control/Makefile
embedding/browser/activex/src/control_kicker/Makefile
embedding/browser/build/Makefile
embedding/browser/chrome/Makefile
embedding/browser/webBrowser/Makefile
embedding/browser/gtk/Makefile
embedding/browser/gtk/src/Makefile
embedding/browser/gtk/tests/Makefile
embedding/browser/photon/Makefile
embedding/browser/photon/src/Makefile
embedding/browser/photon/tests/Makefile
embedding/components/Makefile
embedding/components/build/Makefile
embedding/components/windowwatcher/Makefile
embedding/components/windowwatcher/public/Makefile
embedding/components/windowwatcher/src/Makefile
embedding/components/ui/Makefile
embedding/components/ui/helperAppDlg/Makefile
embedding/components/ui/progressDlg/Makefile
embedding/config/Makefile
embedding/tests/Makefile
embedding/tests/winEmbed/Makefile
embedding/tests/mfcembed/Makefile
embedding/tests/mfcembed/components/Makefile
"

MAKEFILES_psm2="
security/manager/Makefile
security/manager/boot/Makefile
security/manager/boot/src/Makefile
security/manager/boot/public/Makefile
security/manager/ssl/Makefile
security/manager/ssl/src/Makefile
security/manager/ssl/resources/Makefile
security/manager/ssl/public/Makefile
security/manager/pki/Makefile
security/manager/pki/resources/Makefile
security/manager/pki/src/Makefile
security/manager/pki/public/Makefile
netwerk/protocol/http/public/Makefile
netwerk/build/Makefile
netwerk/base/public/Makefile
netwerk/socket/base/Makefile
uriloader/base/Makefile
intl/locale/idl/Makefile
intl/strres/public/Makefile
dom/public/Makefile
dom/public/base/Makefile
rdf/base/idl/Makefile
xpfe/appshell/public/Makefile
caps/idl/Makefile
layout/html/forms/public/Makefile
gfx/public/Makefile
gfx/idl/Makefile
widget/public/Makefile
layout/base/public/Makefile
docshell/base/Makefile
modules/libpref/public/Makefile
content/base/public/Makefile
intl/locale/public/Makefile
"

MAKEFILES_inspector="
extensions/inspector/Makefile
extensions/inspector/base/Makefile
extensions/inspector/base/public/Makefile
extensions/inspector/base/src/Makefile
extensions/inspector/base/src/win/Makefile
extensions/inspector/build/Makefile
extensions/inspector/build/src/Makefile
extensions/inspector/resources/Makefile
"

MAKEFILES_transformiix="
extensions/transformiix/source/base/Makefile
extensions/transformiix/source/main/Makefile
extensions/transformiix/source/xml/dom/standalone/Makefile
extensions/transformiix/source/xml/dom/Makefile
extensions/transformiix/source/xml/dom/mozImpl/Makefile
extensions/transformiix/source/xml/parser/Makefile
extensions/transformiix/source/xml/Makefile
extensions/transformiix/source/xpath/Makefile
extensions/transformiix/source/xslt/functions/Makefile
extensions/transformiix/source/xslt/util/Makefile
extensions/transformiix/source/xslt/Makefile
extensions/transformiix/source/Makefile
extensions/transformiix/Makefile
"

MAKEFILES_tridentprofile="
extensions/tridentprofile/Makefile
extensions/tridentprofile/public/Makefile
extensions/tridentprofile/resources/Makefile
extensions/tridentprofile/src/Makefile
"

MAKEFILES_typeaheadfind="
extensions/typeaheadfind/public/Makefile
extensions/typeaheadfind/resources/Makefile
extensions/typeaheadfind/src/Makefile
extensions/typeaheadfind/Makefile
"

MAKEFILES_phoenix="
browser/Makefile
browser/app/Makefile
browser/base/Makefile
browser/components/Makefile
browser/components/bookmarks/Makefile
browser/components/build/Makefile
browser/components/console/Makefile
browser/components/downloads/Makefile
browser/components/downloads/public/Makefile
browser/components/downloads/src/Makefile
browser/components/prefwindow/Makefile
browser/components/prefwindow/content/Makefile
browser/components/prefwindow/locale/Makefile
browser/components/search/Makefile
browser/components/search/resources/Makefile
toolkit/Makefile
toolkit/components/autocomplete/Makefile
toolkit/components/autocomplete/public/Makefile
toolkit/components/autocomplete/src/Makefile
toolkit/components/build/Makefile
toolkit/components/satchel/Makefile
toolkit/components/satchel/public/Makefile
toolkit/components/satchel/src/Makefile
toolkit/skin/unix/Makefile
toolkit/skin/win/Makefile
toolkit/xre/Makefile
"

MAKEFILES_minotaur="
mail/Makefile
mail/base/Makefile
mail/components/Makefile
mail/components/compose/Makefile
"

MAKEFILES_sql="
extensions/sql/Makefile
extensions/sql/base/Makefile
extensions/sql/base/public/Makefile
extensions/sql/base/src/Makefile
extensions/sql/base/resources/Makefile
extensions/sql/pgsql/public/Makefile
extensions/sql/pgsql/src/Makefile
extensions/sql/build/Makefile
extensions/sql/build/src/Makefile
extensions/sql/packager/Makefile
extensions/sql/packager/mac/Makefile
extensions/sql/packager/unix/Makefile
extensions/sql/packager/win/Makefile
extensions/sql/tests/Makefile
"


if [ "$MACOSX" ]; then
    MAKEFILES_macmorefiles="
       lib/mac/MoreFiles/Makefile
"
fi

if [ "$MOZ_MAIL_NEWS" ]; then
    if [ -f ${srcdir}/mailnews/makefiles ]; then
        MAKEFILES_mailnews=`cat ${srcdir}/mailnews/makefiles`
    fi
fi


    MAKEFILES_libpr0n="
        modules/libpr0n/Makefile
        modules/libpr0n/public/Makefile
        modules/libpr0n/src/Makefile
        modules/libpr0n/decoders/Makefile
        modules/libpr0n/decoders/gif/Makefile
        modules/libpr0n/decoders/png/Makefile
        modules/libpr0n/decoders/ppm/Makefile
        modules/libpr0n/decoders/jpeg/Makefile
        modules/libpr0n/decoders/bmp/Makefile
        modules/libpr0n/decoders/mng/Makefile
        modules/libpr0n/decoders/icon/Makefile
        modules/libpr0n/decoders/icon/win/Makefile
        modules/libpr0n/decoders/xbm/Makefile
"

    MAKEFILES_accessible="
       accessible/Makefile
       accessible/public/Makefile
       accessible/src/Makefile
       accessible/src/base/Makefile
       accessible/src/html/Makefile
       accessible/src/xul/Makefile
       accessible/build/Makefile
"
if [ ! "$SYSTEM_JPEG" ]; then
    MAKEFILES_jpeg="jpeg/Makefile"
fi

if [ ! "$SYSTEM_ZLIB" ]; then
    MAKEFILES_zlib="
	modules/zlib/Makefile
	modules/zlib/src/Makefile
"
fi

MAKEFILES_zlib="
    $MAKEFILES_zlib
    modules/zlib/standalone/Makefile
"


if [ ! "$SYSTEM_PNG" ]; then
    MAKEFILES_libimg="$MAKEFILES_libimg modules/libimg/png/Makefile"
fi

if [ ! "$SYSTEM_MNG" ]; then
    MAKEFILES_libimg="$MAKEFILES_libimg modules/libimg/mng/Makefile"
fi

#
# l10n/
#
if [ -f ${srcdir}/l10n/makefiles.all ]; then
    MAKEFILES_langpacks=`cat ${srcdir}/l10n/makefiles.all`
fi

if [ "$MOZ_L10N" ]; then
    MAKEFILES_l10n="l10n/Makefile"

    if [ "$MOZ_L10N_LANG" ]; then
	MAKEFILES_l10n_lang="
		l10n/lang/Makefile
		l10n/lang/addressbook/Makefile
		l10n/lang/bookmarks/Makefile
		l10n/lang/directory/Makefile
		l10n/lang/editor/Makefile
		l10n/lang/global/Makefile
		l10n/lang/history/Makefile
		l10n/lang/messenger/Makefile
		l10n/lang/messengercompose/Makefile
		l10n/lang/navigator/Makefile
		l10n/lang/pref/Makefile
		l10n/lang/related/Makefile
		l10n/lang/sidebar/Makefile
		l10n/lang/addressbook/locale/Makefile
		l10n/lang/bookmarks/locale/Makefile
		l10n/lang/directory/locale/Makefile
		l10n/lang/editor/locale/Makefile
		l10n/lang/global/locale/Makefile
		l10n/lang/history/locale/Makefile
		l10n/lang/messenger/locale/Makefile
		l10n/lang/messengercompose/locale/Makefile
		l10n/lang/navigator/locale/Makefile
		l10n/lang/pref/locale/Makefile
		l10n/lang/related/locale/Makefile
		l10n/lang/sidebar/locale/Makefile
"
	fi
fi

# tools/jprof
if [ "$MOZ_JPROF" ]; then
    MAKEFILES_jprof="tools/jprof/Makefile
        tools/jprof/stub/Makefile"
fi

# tools/leaky
if [ "$MOZ_LEAKY" ]; then
    MAKEFILES_leaky="tools/leaky/Makefile"
fi

# tools/trace-malloc
if [ "$NS_TRACE_MALLOC" ]; then
    MAKEFILES_tracemalloc="tools/trace-malloc/Makefile tools/trace-malloc/lib/Makefile"
fi

# tools/codesighs
if [ "$MOZ_MAPINFO" ]; then
    MAKEFILES_codesighs="tools/codesighs/Makefile"
fi

# MathML
if [ "$MOZ_MATHML" ]; then
    MAKEFILES_intl="$MAKEFILES_intl
	intl/uconv/ucvmath/Makefile
"
    MAKEFILES_layout="$MAKEFILES_layout
	layout/mathml/Makefile
	layout/mathml/base/Makefile
	layout/mathml/base/src/Makefile
	layout/mathml/content/Makefile
	layout/mathml/content/src/Makefile
"
fi

#libart
if [ "$MOZ_INTERNAL_LIBART_LGPL" ]; then
    MAKEFILES_libart="other-licenses/libart_lgpl/Makefile"
fi

# svg
if [ "$MOZ_SVG" ]; then
    MAKEFILES_content="$MAKEFILES_content
	content/svg/Makefile
	content/svg/document/Makefile
	content/svg/document/src/Makefile
	content/svg/content/Makefile
	content/svg/content/src/Makefile
"
    MAKEFILES_dom="$MAKEFILES_dom
	dom/public/idl/svg/Makefile
"
    MAKEFILES_layout="$MAKEFILES_layout
	layout/svg/Makefile
	layout/svg/base/Makefile
	layout/svg/base/src/Makefile
"
fi

# directory/xpcom
if [ "$MOZ_LDAP_XPCOM" ]; then
    MAKEFILES_ldap="
	directory/xpcom/Makefile
	directory/xpcom/base/Makefile
	directory/xpcom/base/public/Makefile
	directory/xpcom/base/src/Makefile
"
fi

# modules/staticmod

if [ "$MOZ_STATIC_COMPONENTS" -o "$MOZ_META_COMPONENTS" ]; then
    MAKEFILES_static_components="$MAKEFILES_static_components
	modules/staticmod/Makefile
"
fi

for extension in $MOZ_EXTENSIONS; do
    case "$extension" in
        access-builtin ) MAKEFILES_extensions="$MAKEFILES_extensions
            extensions/access-builtin/Makefile
            extensions/access-builtin/accessproxy/Makefile
            " ;;
        content-packs ) MAKEFILES_extensions="$MAKEFILES_extensions
            extensions/content-packs/Makefile
            extensions/content-packs/resources/Makefile
            " ;;
        cookie ) MAKEFILES_extensions="$MAKEFILES_extensions
            extensions/cookie/Makefile
            extensions/cookie/tests/Makefile
            " ;;
        cview ) MAKEFILES_extensions="$MAKEFILES_extensions
            extensions/cview/Makefile
            extensions/cview/resources/Makefile
            " ;;
        datetime ) MAKEFILES_extensions="$MAKEFILES_extensions
            extensions/datetime/Makefile
            " ;;
        finger ) MAKEFILES_extensions="$MAKEFILES_extensions
            extensions/finger/Makefile
            " ;;
        help ) MAKEFILES_extensions="$MAKEFILES_extensions
            extensions/help/Makefile
            extensions/help/resources/Makefile
            " ;;
        inspector ) MAKEFILES_extensions="$MAKEFILES_extensions
            $MAKEFILES_inspector"
            ;;
        typeaheadfind ) MAKEFILES_extensions="$MAKEFILES_extensions
            $MAKEFILES_typeaheadfind"
            ;;
        irc ) MAKEFILES_extensions="$MAKEFILES_extensions
            extensions/irc/Makefile
            " ;;
        p3p ) MAKEFILES_extensions="$MAKEFILES_extensions
            extensions/p3p/Makefile
            extensions/p3p/public/Makefile
            extensions/p3p/src/Makefile
            " ;;
        pics ) MAKEFILES_extensions="$MAKEFILES_extensions
            extensions/pics/Makefile
            extensions/pics/public/Makefile
            extensions/pics/src/Makefile
            extensions/pics/tests/Makefile
            " ;;
        pref ) MAKEFILES_extensions="$MAKEFILES_extensions
            extensions/pref/Makefile
            extensions/pref/autoconfig/Makefile
            extensions/pref/autoconfig/public/Makefile
            extensions/pref/autoconfig/src/Makefile
            extensions/pref/autoconfig/resources/Makefile
            " ;;
        spellcheck ) MAKEFILES_extensions="$MAKEFILES_extensions
            extensions/spellcheck/Makefile
            extensions/spellcheck/idl/Makefile
            extensions/spellcheck/myspell/Makefile
            extensions/spellcheck/myspell/dictionaries/Makefile
            extensions/spellcheck/src/Makefile
            " ;;
        transformiix ) MAKEFILES_extensions="$MAKEFILES_extensions
            $MAKEFILES_transformiix"
            ;;
        tridentprofile ) MAKEFILES_extensions="$MAKEFILES_extensions
            $MAKEFILES_tridentprofile"
            ;;
        universalchardet ) MAKEFILES_extensions="$MAKEFILES_extensions
            extensions/universalchardet/Makefile
            extensions/universalchardet/src/Makefile
            extensions/universalchardet/tests/Makefile
            " ;;
        venkman ) MAKEFILES_extensions="$MAKEFILES_extensions
            extensions/venkman/Makefile"
            ;;
        wallet ) MAKEFILES_extensions="$MAKEFILES_extensions
            extensions/wallet/Makefile
            extensions/wallet/public/Makefile
            extensions/wallet/src/Makefile
            extensions/wallet/editor/Makefile
            extensions/wallet/cookieviewer/Makefile
            extensions/wallet/signonviewer/Makefile
            extensions/wallet/walletpreview/Makefile
            extensions/wallet/build/Makefile
            " ;;
        xmlextras ) MAKEFILES_extensions="$MAKEFILES_extensions
            extensions/xmlextras/Makefile
            extensions/xmlextras/base/Makefile
            extensions/xmlextras/base/src/Makefile
            extensions/xmlextras/base/public/Makefile
            extensions/xmlextras/build/Makefile
            extensions/xmlextras/build/src/Makefile
            extensions/xmlextras/proxy/Makefile
            extensions/xmlextras/proxy/public/Makefile
            extensions/xmlextras/proxy/src/Makefile
            extensions/xmlextras/schema/Makefile
            extensions/xmlextras/schema/public/Makefile
            extensions/xmlextras/schema/src/Makefile
            extensions/xmlextras/soap/Makefile
            extensions/xmlextras/soap/public/Makefile
            extensions/xmlextras/soap/src/Makefile
            extensions/xmlextras/tests/Makefile
            extensions/xmlextras/wsdl/Makefile
            extensions/xmlextras/wsdl/public/Makefile
            extensions/xmlextras/wsdl/src/Makefile
            " ;;
        xml-rpc ) MAKEFILES_extensions="$MAKEFILES_extensions
            extensions/xml-rpc/Makefile
            extensions/xml-rpc/idl/Makefile
            extensions/xml-rpc/src/Makefile
            " ;;
        xmlterm ) MAKEFILES_extensions="$MAKEFILES_extensions
            extensions/xmlterm/Makefile
            extensions/xmlterm/base/Makefile
            extensions/xmlterm/geckoterm/Makefile
            extensions/xmlterm/linetest/Makefile
            extensions/xmlterm/scripts/Makefile
            extensions/xmlterm/tests/Makefile
            extensions/xmlterm/ui/Makefile
            " ;;
        interfaceinfo ) MAKEFILES_extensions="$MAKEFILES_extensions
            extensions/interfaceinfo/Makefile
            extensions/interfaceinfo/public/Makefile
            extensions/interfaceinfo/src/Makefile
            " ;;
        sql ) MAKEFILES_extensions="$MAKEFILES_extensions
            $MAKEFILES_sql"
            ;;
    esac
done


#
# Translate from BUILD_MODULES into the proper makefiles list
#
if [ "$BUILD_MODULES" = "all" ]; then

MAKEFILES_themes=`cat ${srcdir}/themes/makefiles`

MAKEFILES_calendar="
        calendar/Makefile
        calendar/libxpical/Makefile
        calendar/resources/Makefile
	other-licenses/libical/Makefile
	other-licenses/libical/src/Makefile
	other-licenses/libical/src/libical/Makefile
	other-licenses/libical/src/libicalss/Makefile
       "

add_makefiles "
$MAKEFILES_caps
$MAKEFILES_db
$MAKEFILES_dbm
$MAKEFILES_docshell
$MAKEFILES_dom
$MAKEFILES_editor
$MAKEFILES_codesighs
$MAKEFILES_composer
$MAKEFILES_embedding
$MAKEFILES_expat
$MAKEFILES_extensions
$MAKEFILES_gc
$MAKEFILES_gfx
$MAKEFILES_accessible
$MAKEFILES_htmlparser
$MAKEFILES_intl
$MAKEFILES_java_supplement
$MAKEFILES_ldap
$MAKEFILES_leaky
$MAKEFILES_jpeg
$MAKEFILES_jprof
$MAKEFILES_js
$MAKEFILES_jsdebugger
$MAKEFILES_l10n
$MAKEFILES_l10n_lang
$MAKEFILES_langpacks
$MAKEFILES_content
$MAKEFILES_layout
$MAKEFILES_libart
$MAKEFILES_libreg
$MAKEFILES_libimg
$MAKEFILES_libpr0n
$MAKEFILES_libjar
$MAKEFILES_libpref
$MAKEFILES_libutil
$MAKEFILES_liveconnect
$MAKEFILES_macmorefiles
$MAKEFILES_mailnews
$MAKEFILES_oji
$MAKEFILES_plugin
$MAKEFILES_netwerk
$MAKEFILES_profile
$MAKEFILES_rdf
$MAKEFILES_static_components
$MAKEFILES_sun_java
$MAKEFILES_themes
$MAKEFILES_tracemalloc
$MAKEFILES_uriloader
$MAKEFILES_view
$MAKEFILES_webshell
$MAKEFILES_widget
$MAKEFILES_xft
$MAKEFILES_xpcom
$MAKEFILES_xpcom_tests
$MAKEFILES_xpconnect
$MAKEFILES_xpinstall
$MAKEFILES_xpfe
$MAKEFILES_zlib
"

if test -n "$MOZ_PSM"; then
    add_makefiles "$MAKEFILES_psm2"
fi

if test -n "$MOZ_CALENDAR"; then
    add_makefiles "$MAKEFILES_calendar"
fi

if test -n "$MOZ_PHOENIX"; then
    add_makefiles "$MAKEFILES_phoenix"
fi

if test -n "$MOZ_MINOTAUR"; then
    add_makefiles "$MAKEFILES_minotaur"
fi

else

# Standalone modules go here

MAKEFILES_TestStreamConv="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile netwerk/streamconv/test/Makefile"

MAKEFILES_TestXPC="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile intl/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/cache/Makefile webshell/Makefile embedding/components/windowwatcher/Makefile expat/Makefile intl/lwbrk/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/libjar/Makefile xpfe/components/shistory/Makefile editor/composer/Makefile editor/txtsvc/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile rdf/util/Makefile rdf/Makefile content/xul/content/Makefile content/xul/templates/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile embedding/components/jsconsole/Makefile embedding/base/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile intl/chardet/Makefile docshell/Makefile modules/oji/Makefile accessible/Makefile modules/libutil/Makefile layout/Makefile view/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile xpfe/browser/Makefile profile/pref-migrator/Makefile profile/Makefile xpfe/appshell/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile embedding/components/ui/helperAppDlg/Makefile uriloader/Makefile uriloader/exthandler/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile js/src/xpconnect/tests/Makefile"

MAKEFILES_TestXPTC="modules/libreg/Makefile string/Makefile xpcom/Makefile"

MAKEFILES_accessibility="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile accessible/Makefile"

MAKEFILES_accessproxy="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile"

MAKEFILES_addrbook="mailnews/Makefile js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile embedding/components/windowwatcher/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile intl/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/lwbrk/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile expat/Makefile htmlparser/Makefile security/manager/ssl/Makefile mailnews/local/Makefile mailnews/imap/Makefile mailnews/db/msgdb/Makefile mailnews/compose/Makefile mailnews/extensions/smime/Makefile mailnews/mime/Makefile mailnews/news/Makefile mailnews/import/Makefile mailnews/base/Makefile mailnews/base/util/Makefile mailnews/addrbook/Makefile"

MAKEFILES_appcomps="modules/zlib/Makefile modules/libimg/png/Makefile expat/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile intl/Makefile js/src/fdlibm/Makefile js/src/Makefile embedding/components/windowwatcher/Makefile netwerk/mime/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/libjar/Makefile netwerk/cache/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile embedding/components/jsconsole/Makefile embedding/base/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile intl/chardet/Makefile xpfe/browser/Makefile profile/pref-migrator/Makefile profile/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile gfx/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile"

MAKEFILES_apprunner="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile tools/jprof/Makefile modules/mpfilelocprovider/Makefile xpinstall/Makefile"

MAKEFILES_appshell="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile editor/composer/Makefile editor/txtsvc/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile embedding/components/jsconsole/Makefile embedding/base/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile profile/pref-migrator/Makefile profile/Makefile xpfe/appshell/Makefile"

MAKEFILES_autoconfig="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile extensions/pref/autoconfig/Makefile"

MAKEFILES_browser="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile xpfe/components/shistory/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile xpfe/browser/Makefile"

MAKEFILES_caps="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile js/src/xpconnect/Makefile intl/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/cache/Makefile webshell/Makefile embedding/components/windowwatcher/Makefile expat/Makefile intl/lwbrk/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/libjar/Makefile xpfe/components/shistory/Makefile editor/composer/Makefile editor/txtsvc/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile rdf/util/Makefile rdf/Makefile content/xul/content/Makefile content/xul/templates/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile embedding/components/jsconsole/Makefile embedding/base/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile intl/chardet/Makefile docshell/Makefile modules/oji/Makefile accessible/Makefile modules/libutil/Makefile layout/Makefile view/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile xpfe/browser/Makefile profile/pref-migrator/Makefile profile/Makefile xpfe/appshell/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile embedding/components/ui/helperAppDlg/Makefile uriloader/Makefile uriloader/exthandler/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile"

MAKEFILES_chardet="modules/libreg/Makefile string/Makefile xpcom/Makefile netwerk/mime/Makefile netwerk/cache/Makefile expat/Makefile intl/Makefile intl/lwbrk/Makefile js/src/fdlibm/Makefile js/src/Makefile embedding/components/windowwatcher/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile embedding/components/webbrowserpersist/Makefile webshell/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile rdf/util/Makefile rdf/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile modules/libjar/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile docshell/Makefile embedding/components/ui/helperAppDlg/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile js/src/liveconnect/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile uriloader/exthandler/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile modules/libpref/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/chardet/Makefile"

MAKEFILES_chrome="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile profile/Makefile view/Makefile editor/composer/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile rdf/chrome/Makefile"

MAKEFILES_commandhandler="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/commandhandler/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile"

MAKEFILES_composer="editor/composer/Makefile"

MAKEFILES_content="modules/zlib/Makefile modules/libimg/png/Makefile expat/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile intl/Makefile js/src/fdlibm/Makefile js/src/Makefile embedding/components/windowwatcher/Makefile netwerk/mime/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/libjar/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile editor/composer/Makefile editor/txtsvc/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile accessible/Makefile modules/libutil/Makefile layout/Makefile xpfe/components/shistory/Makefile intl/chardet/Makefile xpfe/browser/Makefile netwerk/cache/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile gfx/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile content/Makefile"

MAKEFILES_cookie="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile extensions/cookie/Makefile"

MAKEFILES_dbm="dbm/Makefile"

MAKEFILES_docshell="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile xpfe/components/shistory/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile embedding/components/webbrowserpersist/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile docshell/Makefile"

MAKEFILES_dom="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile"

MAKEFILES_downloadmanager="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile db/mork/Makefile db/mdb/Makefile embedding/components/ui/progressDlg/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/download-manager/Makefile"

MAKEFILES_editor="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile editor/composer/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile editor/txmgr/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile editor/Makefile"

MAKEFILES_embed_base="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile editor/composer/Makefile editor/txtsvc/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile view/Makefile embedding/base/Makefile"

MAKEFILES_embedcomponents="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile editor/composer/Makefile editor/txtsvc/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile xpfe/components/sidebar/Makefile dom/Makefile embedding/components/jsconsole/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile"

MAKEFILES_expat="expat/Makefile"

MAKEFILES_exthandler="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile embedding/components/webbrowserpersist/Makefile webshell/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile rdf/util/Makefile rdf/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile docshell/Makefile embedding/components/ui/helperAppDlg/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile uriloader/exthandler/Makefile"

MAKEFILES_filepicker="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile xpfe/components/filepicker/Makefile"

MAKEFILES_find="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile embedding/components/windowwatcher/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile modules/libjar/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile js/src/liveconnect/Makefile modules/oji/Makefile xpfe/components/sidebar/Makefile dom/Makefile"

MAKEFILES_gfx="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile xpfe/components/shistory/Makefile embedding/components/windowwatcher/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile modules/libutil/Makefile content/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile uriloader/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile gfx/Makefile"

MAKEFILES_gtkembedmoz="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile modules/mpfilelocprovider/Makefile embedding/browser/gtk/src/Makefile embedding/browser/gtk/tests/Makefile"

MAKEFILES_gtkxtbin="widget/src/gtkxtbin/Makefile"

MAKEFILES_helperAppDlg="embedding/components/ui/helperAppDlg/Makefile"

MAKEFILES_htmlparser="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile embedding/components/windowwatcher/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile intl/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/lwbrk/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile expat/Makefile htmlparser/Makefile"

MAKEFILES_i18n="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile"

MAKEFILES_iiextras="modules/libreg/Makefile string/Makefile xpcom/Makefile extensions/interfaceinfo/Makefile"

MAKEFILES_imgbmp="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile xpfe/components/shistory/Makefile embedding/components/windowwatcher/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile modules/libutil/Makefile content/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile uriloader/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile gfx/Makefile modules/libpr0n/decoders/bmp/Makefile"

MAKEFILES_imggif="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile xpfe/components/shistory/Makefile embedding/components/windowwatcher/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile modules/libutil/Makefile content/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile uriloader/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile gfx/Makefile modules/libpr0n/decoders/gif/Makefile"

MAKEFILES_imgjpeg="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile xpfe/components/shistory/Makefile embedding/components/windowwatcher/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile modules/libutil/Makefile content/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile uriloader/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile gfx/Makefile modules/libpr0n/decoders/jpeg/Makefile"

MAKEFILES_imglib2="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile netwerk/cache/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile"

MAKEFILES_imgmng="modules/zlib/Makefile jpeg/Makefile modules/libimg/mng/Makefile js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/libjar/Makefile xpfe/components/shistory/Makefile embedding/components/windowwatcher/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile modules/libimg/png/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile modules/libutil/Makefile content/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile uriloader/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile gfx/Makefile modules/libpr0n/decoders/mng/Makefile"

MAKEFILES_imgpng="modules/zlib/Makefile js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/libjar/Makefile xpfe/components/shistory/Makefile embedding/components/windowwatcher/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile modules/libutil/Makefile content/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile uriloader/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile gfx/Makefile modules/libpr0n/decoders/png/Makefile"

MAKEFILES_imgppm="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile xpfe/components/shistory/Makefile embedding/components/windowwatcher/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile modules/libutil/Makefile content/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile uriloader/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile gfx/Makefile modules/libpr0n/decoders/ppm/Makefile"

MAKEFILES_imgxbm="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile xpfe/components/shistory/Makefile embedding/components/windowwatcher/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile modules/libutil/Makefile content/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile uriloader/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile gfx/Makefile modules/libpr0n/decoders/xbm/Makefile"

MAKEFILES_impComm4xMail="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile mailnews/Makefile security/manager/ssl/Makefile mailnews/local/Makefile mailnews/imap/Makefile mailnews/db/msgdb/Makefile mailnews/import/Makefile mailnews/addrbook/Makefile mailnews/compose/Makefile mailnews/extensions/smime/Makefile mailnews/mime/Makefile mailnews/news/Makefile mailnews/base/Makefile mailnews/base/util/Makefile mailnews/import/comm4x/Makefile"

MAKEFILES_impText="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile mailnews/Makefile security/manager/ssl/Makefile mailnews/local/Makefile mailnews/imap/Makefile mailnews/db/msgdb/Makefile mailnews/compose/Makefile mailnews/extensions/smime/Makefile mailnews/mime/Makefile mailnews/news/Makefile mailnews/import/Makefile mailnews/base/Makefile mailnews/base/util/Makefile mailnews/addrbook/Makefile mailnews/import/text/Makefile"

MAKEFILES_import="mailnews/Makefile js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile embedding/components/windowwatcher/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile intl/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/lwbrk/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile expat/Makefile htmlparser/Makefile security/manager/ssl/Makefile mailnews/addrbook/Makefile mailnews/local/Makefile mailnews/imap/Makefile mailnews/db/msgdb/Makefile mailnews/compose/Makefile mailnews/extensions/smime/Makefile mailnews/mime/Makefile mailnews/news/Makefile mailnews/base/Makefile mailnews/base/util/Makefile mailnews/import/Makefile"

MAKEFILES_inspector="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile extensions/inspector/Makefile"

MAKEFILES_intl="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile intl/Makefile"

MAKEFILES_jar="modules/zlib/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile js/src/fdlibm/Makefile js/src/Makefile js/src/xpconnect/Makefile intl/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/cache/Makefile webshell/Makefile embedding/components/windowwatcher/Makefile expat/Makefile intl/lwbrk/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile xpfe/components/shistory/Makefile editor/composer/Makefile editor/txtsvc/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile rdf/util/Makefile rdf/Makefile content/xul/content/Makefile content/xul/templates/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile embedding/components/jsconsole/Makefile embedding/base/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile intl/chardet/Makefile docshell/Makefile modules/oji/Makefile accessible/Makefile modules/libutil/Makefile layout/Makefile view/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile xpfe/browser/Makefile profile/pref-migrator/Makefile profile/Makefile xpfe/appshell/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile embedding/components/ui/helperAppDlg/Makefile uriloader/Makefile uriloader/exthandler/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile modules/libjar/Makefile"

MAKEFILES_java="sun-java/stubs/Makefile js/jsd/classes/Makefile"

MAKEFILES_jpeg="jpeg/Makefile"

MAKEFILES_js="js/src/fdlibm/Makefile js/src/Makefile"

MAKEFILES_jsconsole="embedding/components/jsconsole/Makefile"

MAKEFILES_jsdebug="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile js/jsd/Makefile"

MAKEFILES_jsloader="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile js/src/xpconnect/loader/Makefile"

MAKEFILES_jsurl="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile"

MAKEFILES_layout="modules/libreg/Makefile string/Makefile xpcom/Makefile netwerk/mime/Makefile netwerk/cache/Makefile expat/Makefile intl/Makefile modules/libutil/Makefile htmlparser/Makefile embedding/components/windowwatcher/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile js/src/fdlibm/Makefile js/src/Makefile js/src/xpconnect/Makefile modules/libjar/Makefile xpfe/components/shistory/Makefile embedding/components/webbrowserpersist/Makefile webshell/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile rdf/util/Makefile rdf/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile intl/chardet/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/oji/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile docshell/Makefile caps/Makefile embedding/components/ui/helperAppDlg/Makefile uriloader/Makefile content/Makefile gfx/Makefile xpfe/components/xremote/Makefile widget/Makefile xpfe/components/sidebar/Makefile dom/Makefile uriloader/exthandler/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile modules/libpref/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/lwbrk/Makefile accessible/Makefile layout/Makefile"

MAKEFILES_libart_lgpl=""

MAKEFILES_libreg="modules/libreg/Makefile"

MAKEFILES_liveconnect="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile intl/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/cache/Makefile webshell/Makefile embedding/components/windowwatcher/Makefile expat/Makefile intl/lwbrk/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile modules/libjar/Makefile xpfe/components/shistory/Makefile editor/composer/Makefile editor/txtsvc/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile rdf/util/Makefile rdf/Makefile content/xul/content/Makefile content/xul/templates/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile embedding/components/jsconsole/Makefile embedding/base/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile intl/chardet/Makefile docshell/Makefile modules/oji/Makefile accessible/Makefile modules/libutil/Makefile layout/Makefile view/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile xpfe/browser/Makefile profile/pref-migrator/Makefile profile/Makefile xpfe/appshell/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile embedding/components/ui/helperAppDlg/Makefile uriloader/Makefile uriloader/exthandler/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile js/src/liveconnect/Makefile"

MAKEFILES_locale="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile embedding/components/windowwatcher/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile modules/libjar/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile xpfe/components/xremote/Makefile widget/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile"

MAKEFILES_lwbrk="modules/libreg/Makefile string/Makefile xpcom/Makefile netwerk/mime/Makefile netwerk/cache/Makefile expat/Makefile intl/Makefile js/src/fdlibm/Makefile js/src/Makefile embedding/components/windowwatcher/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile embedding/components/webbrowserpersist/Makefile webshell/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile rdf/util/Makefile rdf/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile modules/libjar/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile intl/chardet/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile docshell/Makefile embedding/components/ui/helperAppDlg/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile js/src/liveconnect/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile uriloader/exthandler/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile modules/libpref/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/lwbrk/Makefile"

MAKEFILES_mailnews="mailnews/Makefile"

MAKEFILES_mime="mailnews/Makefile js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile embedding/components/windowwatcher/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile intl/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/lwbrk/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile expat/Makefile htmlparser/Makefile mailnews/local/Makefile mailnews/imap/Makefile mailnews/db/msgdb/Makefile mailnews/news/Makefile mailnews/import/Makefile mailnews/addrbook/Makefile mailnews/compose/Makefile mailnews/base/Makefile mailnews/base/util/Makefile security/manager/ssl/Makefile mailnews/extensions/smime/Makefile mailnews/mime/Makefile"

MAKEFILES_mimeemitter="mailnews/Makefile js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile embedding/components/windowwatcher/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile intl/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/lwbrk/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile expat/Makefile htmlparser/Makefile security/manager/ssl/Makefile mailnews/local/Makefile mailnews/imap/Makefile mailnews/db/msgdb/Makefile mailnews/import/Makefile mailnews/addrbook/Makefile mailnews/compose/Makefile mailnews/extensions/smime/Makefile mailnews/mime/Makefile mailnews/news/Makefile mailnews/base/Makefile mailnews/base/util/Makefile mailnews/mime/emitters/Makefile"

MAKEFILES_mimetype="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/cache/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile netwerk/mime/Makefile"

MAKEFILES_mng="modules/zlib/Makefile jpeg/Makefile modules/libimg/mng/Makefile"

MAKEFILES_mork="modules/libreg/Makefile string/Makefile xpcom/Makefile db/mork/Makefile db/mdb/Makefile"

MAKEFILES_mozldap="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile directory/xpcom/base/Makefile"

MAKEFILES_mpfilelocprovider="modules/libreg/Makefile string/Makefile xpcom/Makefile modules/mpfilelocprovider/Makefile"

MAKEFILES_msgbase="mailnews/Makefile js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile embedding/components/windowwatcher/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile intl/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/lwbrk/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile expat/Makefile htmlparser/Makefile security/manager/ssl/Makefile mailnews/local/Makefile mailnews/imap/Makefile mailnews/db/msgdb/Makefile mailnews/compose/Makefile mailnews/extensions/smime/Makefile mailnews/mime/Makefile mailnews/import/Makefile mailnews/addrbook/Makefile mailnews/base/util/Makefile mailnews/news/Makefile mailnews/base/Makefile"

MAKEFILES_msgbaseutil="mailnews/Makefile js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile embedding/components/windowwatcher/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile intl/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/lwbrk/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile expat/Makefile htmlparser/Makefile security/manager/ssl/Makefile mailnews/local/Makefile mailnews/imap/Makefile mailnews/db/msgdb/Makefile mailnews/import/Makefile mailnews/addrbook/Makefile mailnews/compose/Makefile mailnews/extensions/smime/Makefile mailnews/mime/Makefile mailnews/news/Makefile mailnews/base/Makefile mailnews/base/util/Makefile"

MAKEFILES_msgcompose="mailnews/Makefile js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile embedding/components/windowwatcher/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile intl/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/lwbrk/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile expat/Makefile htmlparser/Makefile security/manager/ssl/Makefile mailnews/extensions/smime/Makefile mailnews/local/Makefile mailnews/imap/Makefile mailnews/db/msgdb/Makefile mailnews/mime/Makefile mailnews/news/Makefile mailnews/import/Makefile mailnews/addrbook/Makefile mailnews/base/Makefile mailnews/base/util/Makefile mailnews/compose/Makefile"

MAKEFILES_msgdb="mailnews/Makefile js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile embedding/components/windowwatcher/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile modules/libjar/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile xpfe/components/xremote/Makefile widget/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile security/manager/ssl/Makefile mailnews/import/Makefile mailnews/addrbook/Makefile mailnews/local/Makefile mailnews/imap/Makefile mailnews/compose/Makefile mailnews/extensions/smime/Makefile mailnews/mime/Makefile mailnews/news/Makefile mailnews/base/Makefile mailnews/base/util/Makefile mailnews/db/msgdb/Makefile"

MAKEFILES_msgimap="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile mailnews/Makefile security/manager/ssl/Makefile mailnews/db/msgdb/Makefile mailnews/import/Makefile mailnews/addrbook/Makefile mailnews/compose/Makefile mailnews/extensions/smime/Makefile mailnews/mime/Makefile mailnews/news/Makefile mailnews/local/Makefile mailnews/base/Makefile mailnews/base/util/Makefile mailnews/imap/Makefile"

MAKEFILES_msglocal="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile mailnews/Makefile security/manager/ssl/Makefile mailnews/imap/Makefile mailnews/db/msgdb/Makefile mailnews/import/Makefile mailnews/addrbook/Makefile mailnews/compose/Makefile mailnews/extensions/smime/Makefile mailnews/mime/Makefile mailnews/news/Makefile mailnews/base/Makefile mailnews/base/util/Makefile mailnews/local/Makefile"

MAKEFILES_msgmdn="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile mailnews/Makefile security/manager/ssl/Makefile mailnews/local/Makefile mailnews/imap/Makefile mailnews/db/msgdb/Makefile mailnews/import/Makefile mailnews/addrbook/Makefile mailnews/compose/Makefile mailnews/extensions/smime/Makefile mailnews/mime/Makefile mailnews/news/Makefile mailnews/base/Makefile mailnews/base/util/Makefile mailnews/extensions/mdn/Makefile"

MAKEFILES_msgnews="mailnews/Makefile js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile embedding/components/windowwatcher/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile intl/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/lwbrk/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile expat/Makefile htmlparser/Makefile security/manager/ssl/Makefile mailnews/local/Makefile mailnews/imap/Makefile mailnews/db/msgdb/Makefile mailnews/compose/Makefile mailnews/extensions/smime/Makefile mailnews/mime/Makefile mailnews/import/Makefile mailnews/addrbook/Makefile mailnews/base/Makefile mailnews/base/util/Makefile mailnews/news/Makefile"

MAKEFILES_msgsmime="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile mailnews/Makefile mailnews/local/Makefile mailnews/imap/Makefile mailnews/db/msgdb/Makefile mailnews/compose/Makefile security/manager/ssl/Makefile mailnews/mime/Makefile mailnews/import/Makefile mailnews/addrbook/Makefile mailnews/base/util/Makefile mailnews/news/Makefile mailnews/base/Makefile mailnews/extensions/smime/Makefile"

MAKEFILES_necko="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile"

MAKEFILES_necko2="modules/zlib/Makefile modules/libimg/png/Makefile expat/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile intl/Makefile js/src/fdlibm/Makefile js/src/Makefile embedding/components/windowwatcher/Makefile netwerk/mime/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/libjar/Makefile netwerk/cache/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile gfx/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile"

MAKEFILES_nkcache="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile netwerk/cache/Makefile"

MAKEFILES_nkdatetime="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile"

MAKEFILES_nkfinger="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile"

MAKEFILES_npsimple="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile modules/plugin/samples/simple/Makefile"

MAKEFILES_oji="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile editor/composer/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile modules/libjar/Makefile rdf/chrome/Makefile content/xul/document/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile editor/txtsvc/Makefile editor/txmgr/Makefile editor/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile js/src/liveconnect/Makefile modules/oji/Makefile"

MAKEFILES_p3p="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile extensions/cookie/Makefile extensions/p3p/Makefile"

MAKEFILES_plugin="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile"

MAKEFILES_png="modules/zlib/Makefile modules/libimg/png/Makefile"

MAKEFILES_pref="modules/libreg/Makefile string/Makefile xpcom/Makefile netwerk/mime/Makefile netwerk/cache/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile js/src/fdlibm/Makefile js/src/Makefile js/src/xpconnect/Makefile intl/lwbrk/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/libjar/Makefile xpfe/components/shistory/Makefile embedding/components/webbrowserpersist/Makefile webshell/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile rdf/util/Makefile rdf/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile intl/chardet/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile docshell/Makefile modules/oji/Makefile accessible/Makefile modules/libutil/Makefile layout/Makefile view/Makefile content/Makefile caps/Makefile embedding/components/ui/helperAppDlg/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile uriloader/exthandler/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile modules/libpref/Makefile"

MAKEFILES_prefetch="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile"

MAKEFILES_prefmigr="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile profile/pref-migrator/Makefile"

MAKEFILES_profile="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile view/Makefile editor/composer/Makefile content/xul/content/Makefile content/xul/templates/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile profile/pref-migrator/Makefile profile/Makefile"

MAKEFILES_progressDlg="embedding/components/ui/progressDlg/Makefile"

MAKEFILES_rdf="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile rdf/util/Makefile rdf/Makefile"

MAKEFILES_rdfutil="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile rdf/Makefile rdf/util/Makefile"

MAKEFILES_regviewer="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile rdf/util/Makefile rdf/Makefile xpfe/components/regviewer/Makefile"

MAKEFILES_shistory="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile intl/lwbrk/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/libjar/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile modules/oji/Makefile editor/composer/Makefile editor/txtsvc/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile accessible/Makefile modules/libutil/Makefile layout/Makefile rdf/Makefile uriloader/exthandler/Makefile intl/chardet/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/shistory/Makefile"

MAKEFILES_sidebar="xpfe/components/sidebar/Makefile"

MAKEFILES_string="modules/libreg/Makefile string/Makefile xpcom/Makefile"

MAKEFILES_svg_doc="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile content/svg/document/Makefile"

MAKEFILES_test_necko="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile dbm/Makefile netwerk/test/Makefile"

MAKEFILES_timebombgen="xpfe/components/timebomb/tools/Makefile"

MAKEFILES_transformiix="extensions/transformiix/source/base/Makefile extensions/transformiix/source/lib/Makefile extensions/transformiix/source/main/Makefile extensions/transformiix/source/xml/dom/standalone/Makefile extensions/transformiix/source/xml/dom/Makefile extensions/transformiix/source/xml/dom/mozImpl/Makefile extensions/transformiix/source/xml/parser/Makefile extensions/transformiix/source/xml/Makefile extensions/transformiix/source/xpath/Makefile extensions/transformiix/source/xslt/functions/Makefile extensions/transformiix/source/xslt/util/Makefile extensions/transformiix/source/xslt/Makefile extensions/transformiix/source/Makefile extensions/transformiix/Makefile"

MAKEFILES_txmgr="modules/libreg/Makefile string/Makefile xpcom/Makefile editor/txmgr/Makefile"

MAKEFILES_txtsvc="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile editor/composer/Makefile editor/txmgr/Makefile editor/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile editor/txtsvc/Makefile"

MAKEFILES_typeaheadfind="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile xpfe/components/Makefile"

MAKEFILES_uconv="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile embedding/components/windowwatcher/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile modules/libjar/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile intl/chardet/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile xpfe/components/xremote/Makefile widget/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile intl/Makefile intl/unicharutil/Makefile intl/locale/Makefile intl/uconv/Makefile"

MAKEFILES_ucvcn="modules/libreg/Makefile string/Makefile xpcom/Makefile netwerk/mime/Makefile netwerk/cache/Makefile expat/Makefile intl/Makefile intl/lwbrk/Makefile js/src/fdlibm/Makefile js/src/Makefile embedding/components/windowwatcher/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile embedding/components/webbrowserpersist/Makefile webshell/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile rdf/util/Makefile rdf/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile modules/libjar/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile intl/chardet/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile docshell/Makefile embedding/components/ui/helperAppDlg/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile js/src/liveconnect/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile uriloader/exthandler/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile modules/libpref/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/uconv/ucvcn/Makefile"

MAKEFILES_ucvibm="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile embedding/components/windowwatcher/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile modules/libjar/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile intl/chardet/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile xpfe/components/xremote/Makefile widget/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile intl/Makefile intl/unicharutil/Makefile intl/locale/Makefile intl/uconv/Makefile intl/uconv/ucvibm/Makefile"

MAKEFILES_ucvja="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile embedding/components/windowwatcher/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile modules/libjar/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile intl/chardet/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile xpfe/components/xremote/Makefile widget/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile intl/Makefile intl/unicharutil/Makefile intl/locale/Makefile intl/uconv/Makefile intl/uconv/ucvja/Makefile"

MAKEFILES_ucvko="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile embedding/components/windowwatcher/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile modules/libjar/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile intl/chardet/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile xpfe/components/xremote/Makefile widget/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile intl/Makefile intl/unicharutil/Makefile intl/locale/Makefile intl/uconv/Makefile intl/uconv/ucvko/Makefile"

MAKEFILES_ucvlatin="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile embedding/components/windowwatcher/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile modules/libjar/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile intl/chardet/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile xpfe/components/xremote/Makefile widget/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile intl/Makefile intl/unicharutil/Makefile intl/locale/Makefile intl/uconv/Makefile intl/uconv/ucvlatin/Makefile"

MAKEFILES_ucvmath="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile embedding/components/windowwatcher/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile modules/libjar/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile intl/chardet/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile xpfe/components/xremote/Makefile widget/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile intl/Makefile intl/unicharutil/Makefile intl/locale/Makefile intl/uconv/Makefile intl/uconv/ucvmath/Makefile"

MAKEFILES_ucvtw="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile embedding/components/windowwatcher/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile modules/libjar/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile intl/chardet/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile xpfe/components/xremote/Makefile widget/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile intl/Makefile intl/unicharutil/Makefile intl/locale/Makefile intl/uconv/Makefile intl/uconv/ucvtw/Makefile"

MAKEFILES_ucvtw2="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile embedding/components/windowwatcher/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile modules/libjar/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile intl/chardet/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile xpfe/components/xremote/Makefile widget/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile intl/Makefile intl/unicharutil/Makefile intl/locale/Makefile intl/uconv/Makefile intl/uconv/ucvtw2/Makefile"

MAKEFILES_unicharutil="modules/libreg/Makefile string/Makefile xpcom/Makefile netwerk/mime/Makefile netwerk/cache/Makefile expat/Makefile intl/Makefile intl/lwbrk/Makefile js/src/fdlibm/Makefile js/src/Makefile embedding/components/windowwatcher/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile embedding/components/webbrowserpersist/Makefile webshell/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile rdf/util/Makefile rdf/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile modules/libjar/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile intl/chardet/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile docshell/Makefile embedding/components/ui/helperAppDlg/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile js/src/liveconnect/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile uriloader/exthandler/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile modules/libpref/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile"

MAKEFILES_universalchardet="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile embedding/components/windowwatcher/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile modules/libjar/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile intl/chardet/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile xpfe/components/xremote/Makefile widget/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile intl/Makefile intl/unicharutil/Makefile intl/locale/Makefile intl/uconv/Makefile extensions/universalchardet/Makefile"

MAKEFILES_uriloader="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile xpfe/components/shistory/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile uriloader/Makefile"

MAKEFILES_util="modules/libreg/Makefile string/Makefile xpcom/Makefile modules/libutil/Makefile"

MAKEFILES_vcard="mailnews/Makefile js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile embedding/components/windowwatcher/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile intl/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/lwbrk/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile expat/Makefile htmlparser/Makefile security/manager/ssl/Makefile mailnews/local/Makefile mailnews/imap/Makefile mailnews/db/msgdb/Makefile mailnews/import/Makefile mailnews/addrbook/Makefile mailnews/compose/Makefile mailnews/extensions/smime/Makefile mailnews/mime/Makefile mailnews/news/Makefile mailnews/base/Makefile mailnews/base/util/Makefile mailnews/mime/cthandlers/vcard/Makefile"

MAKEFILES_view="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile view/Makefile"

MAKEFILES_wallet="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile extensions/wallet/Makefile"

MAKEFILES_walletviewers="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile extensions/wallet/Makefile extensions/wallet/build/Makefile extensions/wallet/cookieviewer/Makefile extensions/wallet/editor/Makefile extensions/wallet/signonviewer/Makefile extensions/wallet/walletpreview/Makefile"

MAKEFILES_webbrowserpersist="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile embedding/components/webbrowserpersist/Makefile"

MAKEFILES_webbrwsr="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile profile/pref-migrator/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile embedding/components/webbrowserpersist/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile"

MAKEFILES_webshell="webshell/Makefile"

MAKEFILES_webshell_tests="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile webshell/tests/Makefile"

MAKEFILES_widget="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile"

MAKEFILES_windowwatcher="embedding/components/windowwatcher/Makefile"

MAKEFILES_xlibrgb="gfx/src/xlibrgb/Makefile"

MAKEFILES_xml_rpc="extensions/xml-rpc/Makefile"

MAKEFILES_xmlextras="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile embedding/components/windowwatcher/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile modules/libimg/png/Makefile jpeg/Makefile netwerk/cache/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile view/Makefile editor/composer/Makefile editor/txtsvc/Makefile content/xul/content/Makefile content/xul/templates/Makefile profile/pref-migrator/Makefile profile/Makefile embedding/components/jsconsole/Makefile embedding/components/commandhandler/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile editor/Makefile intl/chardet/Makefile modules/libutil/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile xpfe/components/shistory/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile extensions/xmlextras/Makefile"

MAKEFILES_xmlterm="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile embedding/components/windowwatcher/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile modules/libjar/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile xpfe/components/xremote/Makefile widget/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile extensions/xmlterm/Makefile"

MAKEFILES_xpcom="modules/libreg/Makefile string/Makefile xpcom/Makefile"

MAKEFILES_xpcomsample="modules/libreg/Makefile string/Makefile xpcom/Makefile xpcom/sample/Makefile"

MAKEFILES_xpconnect="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile intl/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/cache/Makefile webshell/Makefile embedding/components/windowwatcher/Makefile expat/Makefile intl/lwbrk/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/libjar/Makefile xpfe/components/shistory/Makefile editor/composer/Makefile editor/txtsvc/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile rdf/util/Makefile rdf/Makefile content/xul/content/Makefile content/xul/templates/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile embedding/components/jsconsole/Makefile embedding/base/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile intl/chardet/Makefile docshell/Makefile modules/oji/Makefile accessible/Makefile modules/libutil/Makefile layout/Makefile view/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile xpfe/browser/Makefile profile/pref-migrator/Makefile profile/Makefile xpfe/appshell/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile embedding/components/ui/helperAppDlg/Makefile uriloader/Makefile uriloader/exthandler/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile"

MAKEFILES_xpconnect_tests="js/src/xpconnect/tests/Makefile"

MAKEFILES_xpcshell="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile intl/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/cache/Makefile webshell/Makefile embedding/components/windowwatcher/Makefile expat/Makefile intl/lwbrk/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/libjar/Makefile xpfe/components/shistory/Makefile editor/composer/Makefile editor/txtsvc/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile rdf/util/Makefile rdf/Makefile content/xul/content/Makefile content/xul/templates/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile embedding/components/jsconsole/Makefile embedding/base/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile intl/chardet/Makefile docshell/Makefile modules/oji/Makefile accessible/Makefile modules/libutil/Makefile layout/Makefile view/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile xpfe/browser/Makefile profile/pref-migrator/Makefile profile/Makefile xpfe/appshell/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile embedding/components/ui/helperAppDlg/Makefile uriloader/Makefile uriloader/exthandler/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile js/src/xpconnect/shell/Makefile"

MAKEFILES_xpctest="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile intl/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/cache/Makefile webshell/Makefile embedding/components/windowwatcher/Makefile expat/Makefile intl/lwbrk/Makefile modules/zlib/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/libjar/Makefile xpfe/components/shistory/Makefile editor/composer/Makefile editor/txtsvc/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile rdf/util/Makefile rdf/Makefile content/xul/content/Makefile content/xul/templates/Makefile rdf/chrome/Makefile content/xul/document/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile embedding/components/jsconsole/Makefile embedding/base/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile intl/chardet/Makefile docshell/Makefile modules/oji/Makefile accessible/Makefile modules/libutil/Makefile layout/Makefile view/Makefile content/Makefile embedding/components/webbrowserpersist/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile xpfe/browser/Makefile profile/pref-migrator/Makefile profile/Makefile xpfe/appshell/Makefile xpfe/components/xremote/Makefile widget/Makefile gfx/Makefile intl/locale/Makefile intl/uconv/Makefile intl/unicharutil/Makefile htmlparser/Makefile embedding/components/ui/helperAppDlg/Makefile uriloader/Makefile uriloader/exthandler/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile caps/Makefile js/src/xpconnect/Makefile js/src/xpconnect/tests/Makefile js/src/xpconnect/tests/components/Makefile"

MAKEFILES_xpinstall="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile xpinstall/Makefile"

MAKEFILES_xpistub="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile xpinstall/Makefile xpinstall/stub/Makefile"

MAKEFILES_xpnet="xpinstall/wizard/libxpnet/Makefile"

MAKEFILES_xremoteservice="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/templates/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile xpfe/components/xremote/Makefile"

MAKEFILES_xul="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile content/xul/templates/Makefile view/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile content/xul/content/Makefile"

MAKEFILES_xuldoc="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile rdf/chrome/Makefile profile/Makefile view/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile content/xul/templates/Makefile content/xul/content/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile content/xul/document/Makefile"

MAKEFILES_xultmpl="js/src/fdlibm/Makefile js/src/Makefile modules/libreg/Makefile string/Makefile xpcom/Makefile expat/Makefile intl/Makefile netwerk/cache/Makefile intl/lwbrk/Makefile sun-java/stubs/Makefile js/jsd/classes/Makefile js/src/liveconnect/Makefile modules/zlib/Makefile modules/libjar/Makefile embedding/components/windowwatcher/Makefile modules/libimg/png/Makefile jpeg/Makefile modules/libpr0n/public/Makefile modules/libpr0n/src/Makefile xpfe/components/shistory/Makefile webshell/Makefile embedding/components/ui/helperAppDlg/Makefile rdf/util/Makefile rdf/Makefile gfx/src/xlibrgb/Makefile widget/src/gtkxtbin/Makefile modules/plugin/Makefile uriloader/exthandler/Makefile embedding/components/webbrowserpersist/Makefile embedding/browser/webBrowser/Makefile embedding/browser/build/Makefile profile/pref-migrator/Makefile view/Makefile content/xul/content/Makefile content/xul/document/Makefile rdf/chrome/Makefile profile/Makefile editor/composer/Makefile editor/txmgr/Makefile embedding/components/commandhandler/Makefile editor/Makefile editor/txtsvc/Makefile embedding/components/jsconsole/Makefile xpfe/components/find/Makefile embedding/components/find/Makefile embedding/components/appstartup/Makefile embedding/components/windowwatcher/Makefile embedding/components/printingui/Makefile embedding/components/build/Makefile embedding/base/Makefile xpfe/appshell/Makefile xpfe/browser/Makefile embedding/components/ui/progressDlg/Makefile xpfe/components/download-manager/Makefile db/mork/Makefile db/mdb/Makefile directory/xpcom/base/Makefile xpfe/components/Makefile docshell/Makefile uriloader/Makefile modules/libutil/Makefile content/Makefile gfx/Makefile modules/oji/Makefile accessible/Makefile layout/Makefile htmlparser/Makefile intl/chardet/Makefile intl/uconv/Makefile intl/unicharutil/Makefile intl/locale/Makefile xpfe/components/xremote/Makefile widget/Makefile caps/Makefile js/src/xpconnect/Makefile modules/libpref/Makefile netwerk/mime/Makefile netwerk/protocol/ftp/Makefile netwerk/protocol/gopher/Makefile netwerk/protocol/viewsource/Makefile netwerk/build2/Makefile netwerk/base/Makefile netwerk/protocol/about/Makefile netwerk/protocol/data/Makefile netwerk/protocol/file/Makefile netwerk/protocol/http/Makefile netwerk/protocol/jar/Makefile netwerk/protocol/keyword/Makefile netwerk/protocol/res/Makefile netwerk/dns/Makefile netwerk/socket/Makefile netwerk/streamconv/Makefile netwerk/cookie/Makefile netwerk/build/Makefile xpfe/components/sidebar/Makefile dom/Makefile content/xul/templates/Makefile"

MAKEFILES_zlib="modules/zlib/Makefile"

    for mod in $BUILD_MODULES; do
        case $mod in
        TestStreamConv) add_makefiles "$MAKEFILES_TestStreamConv" ;;
        TestXPC) add_makefiles "$MAKEFILES_TestXPC" ;;
        TestXPTC) add_makefiles "$MAKEFILES_TestXPTC" ;;
        accessibility) add_makefiles "$MAKEFILES_accessibility" ;;
        accessproxy) add_makefiles "$MAKEFILES_accessproxy" ;;
        addrbook) add_makefiles "$MAKEFILES_addrbook" ;;
        appcomps) add_makefiles "$MAKEFILES_appcomps" ;;
        apprunner) add_makefiles "$MAKEFILES_apprunner" ;;
        appshell) add_makefiles "$MAKEFILES_appshell" ;;
        autoconfig) add_makefiles "$MAKEFILES_autoconfig" ;;
        browser) add_makefiles "$MAKEFILES_browser" ;;
        caps) add_makefiles "$MAKEFILES_caps" ;;
        chardet) add_makefiles "$MAKEFILES_chardet" ;;
        chrome) add_makefiles "$MAKEFILES_chrome" ;;
        commandhandler) add_makefiles "$MAKEFILES_commandhandler" ;;
        composer) add_makefiles "$MAKEFILES_composer" ;;
        content) add_makefiles "$MAKEFILES_content" ;;
        cookie) add_makefiles "$MAKEFILES_cookie" ;;
        dbm) add_makefiles "$MAKEFILES_dbm" ;;
        docshell) add_makefiles "$MAKEFILES_docshell" ;;
        dom) add_makefiles "$MAKEFILES_dom" ;;
        downloadmanager) add_makefiles "$MAKEFILES_downloadmanager" ;;
        editor) add_makefiles "$MAKEFILES_editor" ;;
        embed_base) add_makefiles "$MAKEFILES_embed_base" ;;
        embedcomponents) add_makefiles "$MAKEFILES_embedcomponents" ;;
        expat) add_makefiles "$MAKEFILES_expat" ;;
        exthandler) add_makefiles "$MAKEFILES_exthandler" ;;
        filepicker) add_makefiles "$MAKEFILES_filepicker" ;;
        find) add_makefiles "$MAKEFILES_find" ;;
        gfx) add_makefiles "$MAKEFILES_gfx" ;;
        gtkembedmoz) add_makefiles "$MAKEFILES_gtkembedmoz" ;;
        gtkxtbin) add_makefiles "$MAKEFILES_gtkxtbin" ;;
        helperAppDlg) add_makefiles "$MAKEFILES_helperAppDlg" ;;
        htmlparser) add_makefiles "$MAKEFILES_htmlparser" ;;
        i18n) add_makefiles "$MAKEFILES_i18n" ;;
        iiextras) add_makefiles "$MAKEFILES_iiextras" ;;
        imgbmp) add_makefiles "$MAKEFILES_imgbmp" ;;
        imggif) add_makefiles "$MAKEFILES_imggif" ;;
        imgjpeg) add_makefiles "$MAKEFILES_imgjpeg" ;;
        imglib2) add_makefiles "$MAKEFILES_imglib2" ;;
        imgmng) add_makefiles "$MAKEFILES_imgmng" ;;
        imgpng) add_makefiles "$MAKEFILES_imgpng" ;;
        imgppm) add_makefiles "$MAKEFILES_imgppm" ;;
        imgxbm) add_makefiles "$MAKEFILES_imgxbm" ;;
        impComm4xMail) add_makefiles "$MAKEFILES_impComm4xMail" ;;
        impText) add_makefiles "$MAKEFILES_impText" ;;
        import) add_makefiles "$MAKEFILES_import" ;;
        inspector) add_makefiles "$MAKEFILES_inspector" ;;
        intl) add_makefiles "$MAKEFILES_intl" ;;
        jar) add_makefiles "$MAKEFILES_jar" ;;
        java) add_makefiles "$MAKEFILES_java" ;;
        jpeg) add_makefiles "$MAKEFILES_jpeg" ;;
        js) add_makefiles "$MAKEFILES_js" ;;
        jsconsole) add_makefiles "$MAKEFILES_jsconsole" ;;
        jsdebug) add_makefiles "$MAKEFILES_jsdebug" ;;
        jsloader) add_makefiles "$MAKEFILES_jsloader" ;;
        jsurl) add_makefiles "$MAKEFILES_jsurl" ;;
        layout) add_makefiles "$MAKEFILES_layout" ;;
        libart_lgpl) add_makefiles "$MAKEFILES_libart_lgpl" ;;
        libreg) add_makefiles "$MAKEFILES_libreg" ;;
        liveconnect) add_makefiles "$MAKEFILES_liveconnect" ;;
        locale) add_makefiles "$MAKEFILES_locale" ;;
        lwbrk) add_makefiles "$MAKEFILES_lwbrk" ;;
        mailnews) add_makefiles "$MAKEFILES_mailnews" ;;
        mime) add_makefiles "$MAKEFILES_mime" ;;
        mimeemitter) add_makefiles "$MAKEFILES_mimeemitter" ;;
        mimetype) add_makefiles "$MAKEFILES_mimetype" ;;
        mng) add_makefiles "$MAKEFILES_mng" ;;
        mork) add_makefiles "$MAKEFILES_mork" ;;
        mozldap) add_makefiles "$MAKEFILES_mozldap" ;;
        mpfilelocprovider) add_makefiles "$MAKEFILES_mpfilelocprovider" ;;
        msgbase) add_makefiles "$MAKEFILES_msgbase" ;;
        msgbaseutil) add_makefiles "$MAKEFILES_msgbaseutil" ;;
        msgcompose) add_makefiles "$MAKEFILES_msgcompose" ;;
        msgdb) add_makefiles "$MAKEFILES_msgdb" ;;
        msgimap) add_makefiles "$MAKEFILES_msgimap" ;;
        msglocal) add_makefiles "$MAKEFILES_msglocal" ;;
        msgmdn) add_makefiles "$MAKEFILES_msgmdn" ;;
        msgnews) add_makefiles "$MAKEFILES_msgnews" ;;
        msgsmime) add_makefiles "$MAKEFILES_msgsmime" ;;
        necko) add_makefiles "$MAKEFILES_necko" ;;
        necko2) add_makefiles "$MAKEFILES_necko2" ;;
        nkcache) add_makefiles "$MAKEFILES_nkcache" ;;
        nkdatetime) add_makefiles "$MAKEFILES_nkdatetime" ;;
        nkfinger) add_makefiles "$MAKEFILES_nkfinger" ;;
        npsimple) add_makefiles "$MAKEFILES_npsimple" ;;
        oji) add_makefiles "$MAKEFILES_oji" ;;
        p3p) add_makefiles "$MAKEFILES_p3p" ;;
        plugin) add_makefiles "$MAKEFILES_plugin" ;;
        png) add_makefiles "$MAKEFILES_png" ;;
        pref) add_makefiles "$MAKEFILES_pref" ;;
        prefetch) add_makefiles "$MAKEFILES_prefetch" ;;
        prefmigr) add_makefiles "$MAKEFILES_prefmigr" ;;
        profile) add_makefiles "$MAKEFILES_profile" ;;
        progressDlg) add_makefiles "$MAKEFILES_progressDlg" ;;
        rdf) add_makefiles "$MAKEFILES_rdf" ;;
        rdfutil) add_makefiles "$MAKEFILES_rdfutil" ;;
        regviewer) add_makefiles "$MAKEFILES_regviewer" ;;
        shistory) add_makefiles "$MAKEFILES_shistory" ;;
        sidebar) add_makefiles "$MAKEFILES_sidebar" ;;
        string) add_makefiles "$MAKEFILES_string" ;;
        svg_doc) add_makefiles "$MAKEFILES_svg_doc" ;;
        test_necko) add_makefiles "$MAKEFILES_test_necko" ;;
        timebombgen) add_makefiles "$MAKEFILES_timebombgen" ;;
        transformiix) add_makefiles "$MAKEFILES_transformiix" ;;
        txmgr) add_makefiles "$MAKEFILES_txmgr" ;;
        txtsvc) add_makefiles "$MAKEFILES_txtsvc" ;;
        typeaheadfind) add_makefiles "$MAKEFILES_typeaheadfind" ;;
        uconv) add_makefiles "$MAKEFILES_uconv" ;;
        ucvcn) add_makefiles "$MAKEFILES_ucvcn" ;;
        ucvibm) add_makefiles "$MAKEFILES_ucvibm" ;;
        ucvja) add_makefiles "$MAKEFILES_ucvja" ;;
        ucvko) add_makefiles "$MAKEFILES_ucvko" ;;
        ucvlatin) add_makefiles "$MAKEFILES_ucvlatin" ;;
        ucvmath) add_makefiles "$MAKEFILES_ucvmath" ;;
        ucvtw) add_makefiles "$MAKEFILES_ucvtw" ;;
        ucvtw2) add_makefiles "$MAKEFILES_ucvtw2" ;;
        unicharutil) add_makefiles "$MAKEFILES_unicharutil" ;;
        universalchardet) add_makefiles "$MAKEFILES_universalchardet" ;;
        uriloader) add_makefiles "$MAKEFILES_uriloader" ;;
        util) add_makefiles "$MAKEFILES_util" ;;
        vcard) add_makefiles "$MAKEFILES_vcard" ;;
        view) add_makefiles "$MAKEFILES_view" ;;
        wallet) add_makefiles "$MAKEFILES_wallet" ;;
        walletviewers) add_makefiles "$MAKEFILES_walletviewers" ;;
        webbrowserpersist) add_makefiles "$MAKEFILES_webbrowserpersist" ;;
        webbrwsr) add_makefiles "$MAKEFILES_webbrwsr" ;;
        webshell) add_makefiles "$MAKEFILES_webshell" ;;
        webshell_tests) add_makefiles "$MAKEFILES_webshell_tests" ;;
        widget) add_makefiles "$MAKEFILES_widget" ;;
        windowwatcher) add_makefiles "$MAKEFILES_windowwatcher" ;;
        xlibrgb) add_makefiles "$MAKEFILES_xlibrgb" ;;
        xml-rpc) add_makefiles "$MAKEFILES_xml_rpc" ;;
        xmlextras) add_makefiles "$MAKEFILES_xmlextras" ;;
        xmlterm) add_makefiles "$MAKEFILES_xmlterm" ;;
        xpcom) add_makefiles "$MAKEFILES_xpcom" ;;
        xpcomsample) add_makefiles "$MAKEFILES_xpcomsample" ;;
        xpconnect) add_makefiles "$MAKEFILES_xpconnect" ;;
        xpconnect_tests) add_makefiles "$MAKEFILES_xpconnect_tests" ;;
        xpcshell) add_makefiles "$MAKEFILES_xpcshell" ;;
        xpctest) add_makefiles "$MAKEFILES_xpctest" ;;
        xpinstall) add_makefiles "$MAKEFILES_xpinstall" ;;
        xpistub) add_makefiles "$MAKEFILES_xpistub" ;;
        xpnet) add_makefiles "$MAKEFILES_xpnet" ;;
        xremoteservice) add_makefiles "$MAKEFILES_xremoteservice" ;;
        xul) add_makefiles "$MAKEFILES_xul" ;;
        xuldoc) add_makefiles "$MAKEFILES_xuldoc" ;;
        xultmpl) add_makefiles "$MAKEFILES_xultmpl" ;;
        zlib) add_makefiles "$MAKEFILES_zlib" ;;
        esac
    done

fi

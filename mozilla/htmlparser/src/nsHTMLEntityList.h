/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: NPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is 
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1999
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the NPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the NPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

/******

  This file contains the list of all HTML entities 
  See nsHTMLEntities.h for access to the enum values for entities

  It is designed to be used as inline input to nsHTMLEntities.cpp *only*
  through the magic of C preprocessing.

  All entires must be enclosed in the macro HTML_ENTITY which will have cruel
  and unusual things done to it

  It is recommended (but not strictly necessary) to keep all entries
  in alphabetical order

  The first argument to HTML_ENTITY is the string value of the entity
  The second argument it HTML_ENTITY is the unicode value of the entity

 ******/

// ISO 8859-1 entities.
// See the HTML4.0 spec for this list in it's DTD form
HTML_ENTITY(nbsp, 160)
HTML_ENTITY(iexcl, 161)
HTML_ENTITY(cent, 162)
HTML_ENTITY(pound, 163)
HTML_ENTITY(curren, 164)
HTML_ENTITY(yen, 165)
HTML_ENTITY(brvbar, 166)
HTML_ENTITY(sect, 167)
HTML_ENTITY(uml, 168)
HTML_ENTITY(copy, 169)
HTML_ENTITY(ordf, 170)
HTML_ENTITY(laquo, 171)
HTML_ENTITY(not, 172)
HTML_ENTITY(shy, 173)
HTML_ENTITY(reg, 174)
HTML_ENTITY(macr, 175)
HTML_ENTITY(deg, 176)
HTML_ENTITY(plusmn, 177)
HTML_ENTITY(sup2, 178)
HTML_ENTITY(sup3, 179)
HTML_ENTITY(acute, 180)
HTML_ENTITY(micro, 181)
HTML_ENTITY(para, 182)
HTML_ENTITY(middot, 183)
HTML_ENTITY(cedil, 184)
HTML_ENTITY(sup1, 185)
HTML_ENTITY(ordm, 186)
HTML_ENTITY(raquo, 187)
HTML_ENTITY(frac14, 188)
HTML_ENTITY(frac12, 189)
HTML_ENTITY(frac34, 190)
HTML_ENTITY(iquest, 191)
HTML_ENTITY(Agrave, 192)
HTML_ENTITY(Aacute, 193)
HTML_ENTITY(Acirc, 194)
HTML_ENTITY(Atilde, 195)
HTML_ENTITY(Auml, 196)
HTML_ENTITY(Aring, 197)
HTML_ENTITY(AElig, 198)
HTML_ENTITY(Ccedil, 199)
HTML_ENTITY(Egrave, 200)
HTML_ENTITY(Eacute, 201)
HTML_ENTITY(Ecirc, 202)
HTML_ENTITY(Euml, 203)
HTML_ENTITY(Igrave, 204)
HTML_ENTITY(Iacute, 205)
HTML_ENTITY(Icirc, 206)
HTML_ENTITY(Iuml, 207)
HTML_ENTITY(ETH, 208)
HTML_ENTITY(Ntilde, 209)
HTML_ENTITY(Ograve, 210)
HTML_ENTITY(Oacute, 211)
HTML_ENTITY(Ocirc, 212)
HTML_ENTITY(Otilde, 213)
HTML_ENTITY(Ouml, 214)
HTML_ENTITY(times, 215)
HTML_ENTITY(Oslash, 216)
HTML_ENTITY(Ugrave, 217)
HTML_ENTITY(Uacute, 218)
HTML_ENTITY(Ucirc, 219)
HTML_ENTITY(Uuml, 220)
HTML_ENTITY(Yacute, 221)
HTML_ENTITY(THORN, 222)
HTML_ENTITY(szlig, 223)
HTML_ENTITY(agrave, 224)
HTML_ENTITY(aacute, 225)
HTML_ENTITY(acirc, 226)
HTML_ENTITY(atilde, 227)
HTML_ENTITY(auml, 228)
HTML_ENTITY(aring, 229)
HTML_ENTITY(aelig, 230)
HTML_ENTITY(ccedil, 231)
HTML_ENTITY(egrave, 232)
HTML_ENTITY(eacute, 233)
HTML_ENTITY(ecirc, 234)
HTML_ENTITY(euml, 235)
HTML_ENTITY(igrave, 236)
HTML_ENTITY(iacute, 237)
HTML_ENTITY(icirc, 238)
HTML_ENTITY(iuml, 239)
HTML_ENTITY(eth, 240)
HTML_ENTITY(ntilde, 241)
HTML_ENTITY(ograve, 242)
HTML_ENTITY(oacute, 243)
HTML_ENTITY(ocirc, 244)
HTML_ENTITY(otilde, 245)
HTML_ENTITY(ouml, 246)
HTML_ENTITY(divide, 247)
HTML_ENTITY(oslash, 248)
HTML_ENTITY(ugrave, 249)
HTML_ENTITY(uacute, 250)
HTML_ENTITY(ucirc, 251)
HTML_ENTITY(uuml, 252)
HTML_ENTITY(yacute, 253)
HTML_ENTITY(thorn, 254)
HTML_ENTITY(yuml, 255)

// Symbols, mathematical symbols and Greek letters
// See the HTML4.0 spec for this list in it's DTD form
HTML_ENTITY(fnof, 402)
HTML_ENTITY(Alpha, 913)
HTML_ENTITY(Beta, 914)
HTML_ENTITY(Gamma, 915)
HTML_ENTITY(Delta, 916)
HTML_ENTITY(Epsilon, 917)
HTML_ENTITY(Zeta, 918)
HTML_ENTITY(Eta, 919)
HTML_ENTITY(Theta, 920)
HTML_ENTITY(Iota, 921)
HTML_ENTITY(Kappa, 922)
HTML_ENTITY(Lambda, 923)
HTML_ENTITY(Mu, 924)
HTML_ENTITY(Nu, 925)
HTML_ENTITY(Xi, 926)
HTML_ENTITY(Omicron, 927)
HTML_ENTITY(Pi, 928)
HTML_ENTITY(Rho, 929)
HTML_ENTITY(Sigma, 931)
HTML_ENTITY(Tau, 932)
HTML_ENTITY(Upsilon, 933)
HTML_ENTITY(Phi, 934)
HTML_ENTITY(Chi, 935)
HTML_ENTITY(Psi, 936)
HTML_ENTITY(Omega, 937)
HTML_ENTITY(alpha, 945)
HTML_ENTITY(beta, 946)
HTML_ENTITY(gamma, 947)
HTML_ENTITY(delta, 948)
HTML_ENTITY(epsilon, 949)
HTML_ENTITY(zeta, 950)
HTML_ENTITY(eta, 951)
HTML_ENTITY(theta, 952)
HTML_ENTITY(iota, 953)
HTML_ENTITY(kappa, 954)
HTML_ENTITY(lambda, 955)
HTML_ENTITY(mu, 956)
HTML_ENTITY(nu, 957)
HTML_ENTITY(xi, 958)
HTML_ENTITY(omicron, 959)
HTML_ENTITY(pi, 960)
HTML_ENTITY(rho, 961)
HTML_ENTITY(sigmaf, 962)
HTML_ENTITY(sigma, 963)
HTML_ENTITY(tau, 964)
HTML_ENTITY(upsilon, 965)
HTML_ENTITY(phi, 966)
HTML_ENTITY(chi, 967)
HTML_ENTITY(psi, 968)
HTML_ENTITY(omega, 969)
HTML_ENTITY(thetasym, 977)
HTML_ENTITY(upsih, 978)
HTML_ENTITY(piv, 982)
HTML_ENTITY(bull, 8226)
HTML_ENTITY(hellip, 8230)
HTML_ENTITY(prime, 8242)
HTML_ENTITY(Prime, 8243)
HTML_ENTITY(oline, 8254)
HTML_ENTITY(frasl, 8260)
HTML_ENTITY(weierp, 8472)
HTML_ENTITY(image, 8465)
HTML_ENTITY(real, 8476)
HTML_ENTITY(trade, 8482)
HTML_ENTITY(alefsym, 8501)
HTML_ENTITY(larr, 8592)
HTML_ENTITY(uarr, 8593)
HTML_ENTITY(rarr, 8594)
HTML_ENTITY(darr, 8595)
HTML_ENTITY(harr, 8596)
HTML_ENTITY(crarr, 8629)
HTML_ENTITY(lArr, 8656)
HTML_ENTITY(uArr, 8657)
HTML_ENTITY(rArr, 8658)
HTML_ENTITY(dArr, 8659)
HTML_ENTITY(hArr, 8660)
HTML_ENTITY(forall, 8704)
HTML_ENTITY(part, 8706)
HTML_ENTITY(exist, 8707)
HTML_ENTITY(empty, 8709)
HTML_ENTITY(nabla, 8711)
HTML_ENTITY(isin, 8712)
HTML_ENTITY(notin, 8713)
HTML_ENTITY(ni, 8715)
HTML_ENTITY(prod, 8719)
HTML_ENTITY(sum, 8721)
HTML_ENTITY(minus, 8722)
HTML_ENTITY(lowast, 8727)
HTML_ENTITY(radic, 8730)
HTML_ENTITY(prop, 8733)
HTML_ENTITY(infin, 8734)
HTML_ENTITY(ang, 8736)
HTML_ENTITY(and, 8743)
HTML_ENTITY(or, 8744)
HTML_ENTITY(cap, 8745)
HTML_ENTITY(cup, 8746)
HTML_ENTITY(int, 8747)
HTML_ENTITY(there4, 8756)
HTML_ENTITY(sim, 8764)
HTML_ENTITY(cong, 8773)
HTML_ENTITY(asymp, 8776)
HTML_ENTITY(ne, 8800)
HTML_ENTITY(equiv, 8801)
HTML_ENTITY(le, 8804)
HTML_ENTITY(ge, 8805)
HTML_ENTITY(sub, 8834)
HTML_ENTITY(sup, 8835)
HTML_ENTITY(nsub, 8836)
HTML_ENTITY(sube, 8838)
HTML_ENTITY(supe, 8839)
HTML_ENTITY(oplus, 8853)
HTML_ENTITY(otimes, 8855)
HTML_ENTITY(perp, 8869)
HTML_ENTITY(sdot, 8901)
HTML_ENTITY(lceil, 8968)
HTML_ENTITY(rceil, 8969)
HTML_ENTITY(lfloor, 8970)
HTML_ENTITY(rfloor, 8971)
HTML_ENTITY(lang, 9001)
HTML_ENTITY(rang, 9002)
HTML_ENTITY(loz, 9674)
HTML_ENTITY(spades, 9824)
HTML_ENTITY(clubs, 9827)
HTML_ENTITY(hearts, 9829)
HTML_ENTITY(diams, 9830)

// Markup-significant and internationalization characters
// See the HTML4.0 spec for this list in it's DTD form
HTML_ENTITY(quot, 34)
HTML_ENTITY(amp, 38)
HTML_ENTITY(lt, 60)
HTML_ENTITY(gt, 62)
HTML_ENTITY(OElig, 338)
HTML_ENTITY(oelig, 339)
HTML_ENTITY(Scaron, 352)
HTML_ENTITY(scaron, 353)
HTML_ENTITY(Yuml, 376)
HTML_ENTITY(circ, 710)
HTML_ENTITY(tilde, 732)
HTML_ENTITY(ensp, 8194)
HTML_ENTITY(emsp, 8195)
HTML_ENTITY(thinsp, 8201)
HTML_ENTITY(zwnj, 8204)
HTML_ENTITY(zwj, 8205)
HTML_ENTITY(lrm, 8206)
HTML_ENTITY(rlm, 8207)
HTML_ENTITY(ndash, 8211)
HTML_ENTITY(mdash, 8212)
HTML_ENTITY(lsquo, 8216)
HTML_ENTITY(rsquo, 8217)
HTML_ENTITY(sbquo, 8218)
HTML_ENTITY(ldquo, 8220)
HTML_ENTITY(rdquo, 8221)
HTML_ENTITY(bdquo, 8222)
HTML_ENTITY(dagger, 8224)
HTML_ENTITY(Dagger, 8225)
HTML_ENTITY(permil, 8240)
HTML_ENTITY(lsaquo, 8249)
HTML_ENTITY(rsaquo, 8250)
HTML_ENTITY(euro, 8364)

// Navigator entity extensions; apos is from XML
HTML_ENTITY(apos, 39) 
HTML_ENTITY(AMP, 38)
HTML_ENTITY(COPY, 169)
HTML_ENTITY(GT, 62)
HTML_ENTITY(LT, 60)
HTML_ENTITY(QUOT, 34)
HTML_ENTITY(REG, 174)


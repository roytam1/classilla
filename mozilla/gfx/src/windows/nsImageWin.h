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
 * Portions created by the Initial Developer are Copyright (C) 1998
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

#ifndef nsImageWin_h___
#define nsImageWin_h___

#include <windows.h>
#include "nsIImage.h"

/* for compatibility with VC++ 5 */
#if !defined(AC_SRC_OVER)
#define AC_SRC_OVER                 0x00
#define AC_SRC_ALPHA                0x01
#pragma pack(1)
typedef struct {
    BYTE   BlendOp;
    BYTE   BlendFlags;
    BYTE   SourceConstantAlpha;
    BYTE   AlphaFormat;
}BLENDFUNCTION;
#pragma pack()
#endif

typedef BOOL (WINAPI *ALPHABLENDPROC)(
  HDC hdcDest,
  int nXOriginDest,
  int nYOriginDest,
  int nWidthDest,
  int hHeightDest,
  HDC hdcSrc,
  int nXOriginSrc,
  int nYOriginSrc,
  int nWidthSrc,
  int nHeightSrc,
  BLENDFUNCTION blendFunction);

class nsImageWin : public nsIImage{
public:
  nsImageWin();
  ~nsImageWin();

  NS_DECL_ISUPPORTS

  /**
  @see nsIImage.h
  */
  virtual PRInt32     GetBytesPix()       { return mNumBytesPixel; }
  virtual PRInt32     GetHeight()         { return mBHead->biHeight; }
  virtual PRBool      GetIsRowOrderTopToBottom() { return PR_FALSE; }
  virtual PRInt32     GetWidth()          { return mBHead->biWidth; }
  virtual PRUint8*    GetBits() ;
  virtual PRInt32     GetLineStride()     { return mRowBytes; }

  virtual PRBool      GetHasAlphaMask()   { return mAlphaBits != nsnull; }

  NS_IMETHOD          Draw(nsIRenderingContext &aContext, nsDrawingSurface aSurface, PRInt32 aX, PRInt32 aY,
                           PRInt32 aWidth, PRInt32 aHeight);
  NS_IMETHOD          Draw(nsIRenderingContext &aContext, nsDrawingSurface aSurface, PRInt32 aSX, PRInt32 aSY,
                           PRInt32 aSWidth, PRInt32 aSHeight,
                           PRInt32 aDX, PRInt32 aDY, PRInt32 aDWidth, PRInt32 aDHeight);
  NS_IMETHOD          DrawToImage(nsIImage* aDstImage, nscoord aDX, nscoord aDY,
                                  nscoord aDWidth, nscoord aDHeight);
  virtual nsColorMap* GetColorMap() {return mColorMap;}
  virtual void        ImageUpdated(nsIDeviceContext *aContext, PRUint8 aFlags, nsRect *aUpdateRect);
  virtual nsresult    Init(PRInt32 aWidth, PRInt32 aHeight, PRInt32 aDepth, nsMaskRequirements aMaskRequirements);
  virtual nsresult    Optimize(nsIDeviceContext* aContext);
  virtual PRUint8*    GetAlphaBits()      { return mAlphaBits; }
  virtual PRInt32     GetAlphaLineStride(){ return mARowBytes; }


  NS_IMETHOD DrawTile(nsIRenderingContext &aContext,
                      nsDrawingSurface aSurface,
                      PRInt32 aSXOffset, PRInt32 aSYOffset,
                      const nsRect &aTileRect);

  /** 
   * Progressivly double the bitmap size as we blit.. very fast way to tile
   * @param aSurface  the surface to blit to
   * @param aDestBufferWidth   Width of buffer
   * @param aDestBufferHeight  Height of buffer
   * @param aScaledTileWidth   Width of tile
   * @param aScaledTileHeight  Height of tile
   * @param aX0,aY0,aX1,aY1    Coordinates of screen to blit to
   * @return if TRUE, no errors
  */

  PRBool ProgressiveDoubleBlit(nsDrawingSurface aSurface,
                              PRInt32 aDestBufferWidth, PRInt32 aDestBufferHeight,
                              PRInt32 aScaledTileWidth,PRInt32 aScaledTileHeight,
                              PRInt32 aX0,PRInt32 aY0,
                              PRInt32 aX1,PRInt32 aY1);

   /** 
   * Return the header size of the Device Independent Bitmap(DIB).
   * @return size of header in bytes
   */
  PRIntn      GetSizeHeader(){return sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * mNumPaletteColors;}

  /** 
   * Return the image size of the Device Independent Bitmap(DIB).
   * @update dc - 10/29/98
   * @return size of image in bytes
   */
  PRIntn      GetSizeImage(){ return mSizeImage; }

  /** 
   * Calculate the number of bytes spaned for this image for a given width
   * @param aWidth is the width to calculate the number of bytes for
   * @return the number of bytes in this span
   */
  PRInt32  CalcBytesSpan(PRUint32  aWidth);

  /**
   * Get the alpha depth for the image mask
   * @update - lordpixel 2001/05/16
   * @return  the alpha mask depth for the image, ie, 0, 1 or 8
   */
  virtual PRInt8 GetAlphaDepth() {return(mAlphaDepth);}

  /** 
   * Get the DIB specific informations for this bitmap.
   * @update dc - 10/29/98
   * @return VOID
   */
  void* GetBitInfo() {return mBHead;}

  NS_IMETHOD   LockImagePixels(PRBool aMaskPixels);
  NS_IMETHOD   UnlockImagePixels(PRBool aMaskPixels);

  // VER_PLATFORM_WIN32_WINDOWS == Win 95/98/ME
  // VER_PLATFORM_WIN32_NT == Win NT/2K/XP/.NET Server
  static PRInt32 gPlatform;

private:
  /** 
   * Clean the device Independent bits that could be allocated by this object.
   * @update dc - 04/05/00
   */
  void CleanUpDIB();

  /** 
   * Clean the device dependent bits that could be allocated by this object.
   * @update dc - 04/05/00
   */
  void CleanUpDDB();

  void CleanUpDIBSection();

  void CreateImageWithAlphaBits(HDC TheHDC);

  /** 
   * Create a Device Dependent bitmap from a drawing surface
   * @update dc - 10/29/98
   * @param aSurface - The drawingsurface to create the DDB from.
   */
  void CreateDDB(nsDrawingSurface aSurface);

  void DrawComposited24(unsigned char *aBits, int aX, int aY, int aWidth, int aHeight);
  nsresult DrawComposited(HDC TheHDC, int aDX, int aDY, int aDWidth, int aDHeight,
                          int aSX, int aSY, int aSWidth, int aSHeight);
  static PRBool CanAlphaBlend(void);

  /** 
   * Create a Device Dependent bitmap from a drawing surface
   * @update dc - 05/20/99
   * @param aSurface - The drawingsurface to create the DIB from.
   */
  nsresult ConvertDDBtoDIB();


  /** 
   * Print a DDB
   * @update dc - 05/20/99
   * @param aSurface - The drawingsurface to create the DIB from.
   * @param aX - x location to place image
   * @param aX - y location to place image
   * @param aWidth - width of DIB
   * @param aHeight - height of DIB
   */
  nsresult PrintDDB(nsDrawingSurface aSurface,PRInt32 aX,PRInt32 aY,PRInt32 aWidth,PRInt32 aHeight,PRUint32  aROP);


  
  /** ---------------------------------------------------
   *  build a tile area by doubling the image until it reaches its limits
   *  @param aTheHDC -- HDC to render to
   *  @param aSrcRect -- Rectangle we are build with the image
   *  @param aHeight -- height of the tile
   *  @param aWidth -- width of the tile
   */
  void  BuildTile(HDC aTheHDC, nsRect &aSrcRect,
                  PRInt32 aWidth, PRInt32 aHeight, PRInt32 aCopyMode);



  /** 
   * Get an index in the palette that matches as closly as possible the passed in RGB colors
   * @update dc - 4/20/2000
   * @param aR - Red component of the color to match
   * @param aG - Green component of the color to match
   * @param aB - Blue component of the color to match
   * @return - The closest palette match
   */
  PRUint8 PaletteMatch(PRUint8 r, PRUint8 g, PRUint8 b);

  PRPackedBool        mInitialized;
  PRInt8              mNumBytesPixel;     // number of bytes per pixel
  PRInt16             mNumPaletteColors;  // Number of colors in the pallete 256 
  PRInt32             mSizeImage;         // number of bytes
  PRInt32             mRowBytes;          // number of bytes per row
  PRUint8*            mImageBits;         // starting address of DIB bits
  PRBool              mIsOptimized;       // Did we convert our DIB to a HBITMAP
  nsColorMap*         mColorMap;          // Redundant with mColorTable, but necessary

  PRInt32             mDecodedX1;			    //Keeps track of what part of image
  PRInt32             mDecodedY1;			    // has been decoded.
  PRInt32             mDecodedX2; 
  PRInt32             mDecodedY2; 
  PRBool              mIsLocked;			    // variable to keep track of the locking
  PRBool              mDIBTemp;           // boolean to let us know if DIB was created as temp

  // alpha layer members
  PRUint8             *mAlphaBits;        // alpha layer if we made one
  PRInt8              mAlphaDepth;        // alpha layer depth
  PRInt16             mARowBytes;         // number of bytes per row in the image for tha alpha
  PRInt8              mImageCache;        // place to save off the old image for fast animation
  HBITMAP             mHBitmap;           // the GDI bitmaps
  HBITMAP             mDIBSection;
  LPBITMAPINFOHEADER  mBHead;             // BITMAPINFOHEADER

  static ALPHABLENDPROC gAlphaBlend;      // AlphaBlend function pointer

};

#endif

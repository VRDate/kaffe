/**
 * tlk.c - 
 *
 * Copyright (c) 1998
 *      Transvirtual Technologies, Inc.  All rights reserved.
 *
 * See the file "license.terms" for information on usage and redistribution 
 * of this file. 
 */


#define MAIN

#include "toolkit.h"
#include "tlkprops.h"


/********************************************************************************
 * auxiliary functions
 */

int
xErrorHandler ( Display *dsp, XErrorEvent *err )
{
  char buf[128];
  char key[64];

  /*
   * If there are clueless apps using resident Graphics objects in
   * asnyc drawing ops (without checking for window visibility first),
   * we might get BadDrawable errors. Since this is clearly a bug
   * of the app (which has to clean up resident Graphics objects
   * before closing windows), we don't want to add costly checks
   * on every Graphics op (which couldn't be safe, anyway).
   * The only way to handle this safely would be to link Graphics
   * objects to their targets, and this would add the danger of
   * severe memory leaks in case the Graphics objects aren't disposed
   * (for efficiency reasons, we cach Graphics instances).
   * Ignoring BadDrawables is not very nice, but reasonably safe.
   */
  /*
  if ( err->error_code == BadDrawable )
	return 0;
  */

  sprintf( key, "%d", err->error_code);
  XGetErrorDatabaseText( dsp, "XProtoError", key, "", buf, sizeof( buf));

  DBG( awt,("X error:      %s\n", buf));

  sprintf( key, "%d", err->request_code);
  XGetErrorDatabaseText( dsp, "XRequest", key, "", buf, sizeof( buf));

  DBG( awt, ("  request:    %s\n", buf));
  DBG( awt, ("  resource:   %X\n", err->resourceid));

  //DBG_ACTION( awt, (*JniEnv)->ThrowNew( JniEnv, AWTError, "X error occured"));

  return 0;
}


/********************************************************************************
 * exported functions
 */

jint 
Java_java_awt_Toolkit_tlkProperties ( JNIEnv* env, jclass clazz )
{
  jint    props = TLK_EXTERNAL_DECO;

#if defined(USE_THREADED_AWT)
  props |= TLK_IS_BLOCKING;
  props |= TLK_NEEDS_FLUSH;
#endif

  return props;
}

jboolean
Java_java_awt_Toolkit_tlkInit ( JNIEnv* env, jclass clazz, jstring name )
{
  char    *dspName;

  getBuffer(X, 128);

  JniEnv = env;
  AWTError = (*env)->FindClass( env, "java/awt/AWTError");

  XSetErrorHandler( xErrorHandler);

  if ( name ) {
	dspName = java2CString( env, X, name);
  }
  else {
	if ( !(dspName = getenv( "DISPLAY")) )
	  dspName = ":0.0";
  }

  if ( !(X->dsp = XOpenDisplay( dspName)) ) {
	DBG( awt, ("XOpenDisplay failed: %s\n", dspName));
	return JNI_FALSE;
  }

  DBG( awt, ("synchronize X\n"));
  DBG_ACTION(awt, XSynchronize( X->dsp, True));

  X->root   = DefaultRootWindow( X->dsp);

  /*
   * We just can use XShm in case we don't run remote, and we better don't rely on
   * XShmQueryExtension to make this distinction
   */
#ifdef HAVE_LIBXEXT
  if ( (dspName[0] == ':') || (strncmp( "localhost", dspName, 9) == 0) ) {
	if ( XShmQueryExtension( X->dsp) ){
	  X->shm =  USE_SHM;
	  /*
	   * threshold shouldn't be much smaller than page size, since this is
	   * usually the smallest amount of sharable memory, anyway
	   */
	  X->shmThreshold = 4096;
	}
  }
#endif

  WM_PROTOCOLS     = XInternAtom( X->dsp, "WM_PROTOCOLS", False);
  WM_DELETE_WINDOW = XInternAtom( X->dsp, "WM_DELETE_WINDOW", False);

  WM_TAKE_FOCUS    = XInternAtom( X->dsp, "WM_TAKE_FOCUS", False);
  WAKEUP           = XInternAtom( X->dsp, "WAKEUP", False);
  RETRY_FOCUS      = XInternAtom( X->dsp, "RETRY_FOCUS", False);

  return JNI_TRUE;
}

void
Java_java_awt_Toolkit_tlkTerminate ( JNIEnv* env, jclass clazz )
{
  if ( X->cbdOwner ) {
	XDestroyWindow( X->dsp, X->cbdOwner);
	X->cbdOwner = 0;
  }

  if ( X->wakeUp ) {
	XDestroyWindow( X->dsp, X->wakeUp);
	X->wakeUp = 0;
  }

  if ( X->dsp ){
	XSync( X->dsp, False);
	XCloseDisplay( X->dsp);
	X->dsp = 0;
  }
}


jstring
Java_java_awt_Toolkit_tlkVersion ( JNIEnv* env, jclass clazz )
{
  return (*env)->NewStringUTF( env, "X-1.0");
}

jint
Java_java_awt_Toolkit_tlkGetResolution ( JNIEnv* env, jclass clazz )
{
  /*
   * This is just a guess since WidthMMOfScreen most probably isn't exact.
   * We are interested in pixels per inch
   */
  Screen *scr = DefaultScreenOfDisplay( X->dsp);
  return ((WidthOfScreen( scr) * 254) + 5) / (WidthMMOfScreen( scr) *10);
}

jint
Java_java_awt_Toolkit_tlkGetScreenHeight ( JNIEnv* env, jclass clazz )
{
  return DisplayHeight( X->dsp, DefaultScreen( X->dsp));
}

jint
Java_java_awt_Toolkit_tlkGetScreenWidth ( JNIEnv* env, jclass clazz )
{
  return DisplayWidth( X->dsp, DefaultScreen( X->dsp));
}

void
Java_java_awt_Toolkit_tlkSync ( JNIEnv* env, jclass clazz )
{
  /*
   * this one flushes the request buffer and waits until all reqs have been processed
   * (a roundtrip for input based on prior requests)
   */
  XSync( X->dsp, False);
}

void
Java_java_awt_Toolkit_tlkFlush ( JNIEnv* env, jclass clazz )
{
  /* simply flush request buffer (mainly for background threads and blocked AWT) */
  XFlush( X->dsp);
}


void
Java_java_awt_Toolkit_tlkBeep ( JNIEnv* env, jclass clazz )
{
  XBell( X->dsp, 100);
}

void
Java_java_awt_Toolkit_tlkDisplayBanner ( JNIEnv* env, jclass clazz, jstring banner )
{
}

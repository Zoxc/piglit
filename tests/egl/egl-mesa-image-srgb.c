/*
 * Copyright © 2010 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Authors:
 *    John Kåre Alsaker <john.kare.alsaker@gmail.com>
 *    Kristian Høgsberg <krh@bitplanet.net>
 */

/** @file egl-mesa-image-srgb.c
 *
 * Test EGL_MESA_image_sRGB
 */

#include "piglit-util-gl-common.h"
#include "egl-util.h"

#if defined(EGL_MESA_image_sRGB) && defined(EGL_KHR_image_pixmap)

const char *extensions[] = { "EGL_MESA_image_sRGB", "EGL_KHR_image_pixmap", NULL };

static const EGLint default_attribs[] = {
	EGL_GAMMA_MESA,	EGL_DEFAULT_MESA,
	EGL_NONE
};

static const EGLint bad_attribs[] = {
	EGL_GAMMA_MESA,	EGL_BAD_ATTRIBUTE,
	EGL_NONE
};

static enum piglit_result
draw(struct egl_state *state)
{
	Pixmap pixmap;
	EGLImageKHR img;
	PFNEGLCREATEIMAGEKHRPROC create_image_khr;

	create_image_khr = (PFNEGLCREATEIMAGEKHRPROC)
		eglGetProcAddress("eglCreateImageKHR");

	if (create_image_khr == NULL) {
		fprintf(stderr, "could not getproc eglCreateImageKHR");
		piglit_report_result(PIGLIT_FAIL);
	}

	pixmap = XCreatePixmap(state->dpy, state->win, 1, 1, state->depth);

	img = create_image_khr(state->egl_dpy, state->ctx,
				  EGL_NATIVE_PIXMAP_KHR,
				  (EGLClientBuffer)pixmap,
				  default_attribs);
	if (!img) {
		fprintf(stderr, "default view eglCreateImageKHR() failed\n");
		piglit_report_result(PIGLIT_FAIL);
	}

	img = create_image_khr(state->egl_dpy, state->ctx,
				  EGL_NATIVE_PIXMAP_KHR,
				  (EGLClientBuffer)pixmap,
				  bad_attribs);

	if (img || (eglGetError() != EGL_BAD_VIEW_MESA)) {
		fprintf(stderr, "unsupported view eglCreateImageKHR() failed\n");
		piglit_report_result(PIGLIT_FAIL);
	}

	return PIGLIT_PASS;
}

int
main(int argc, char *argv[])
{
	struct egl_test test;

	egl_init_test(&test);
	test.extensions = extensions;
	test.draw = draw;

	return egl_util_run(&test, argc, argv);
}

#else

int
main(int argc, char *argv[])
{
	piglit_report_result(PIGLIT_SKIP);

	return 0;
}

#endif

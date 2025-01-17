/* Copyright (c) 2021 Jin Li, http://www.luvfight.me

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#pragma once

#define DORA_DEFAULT_ORG_NAME "LuvFight"
#define DORA_DEFAULT_APP_NAME "DorothySSR"

#define NS_DOROTHY_BEGIN namespace Dorothy {
#define NS_DOROTHY_END }

#define NS_BEGIN(name) namespace name {
#define NS_END(name) }

/** @brief Debug flag, set with the compilar flag by default. */
#ifndef DORA_DEBUG
	#if NDEBUG
		#define DORA_DEBUG 0
	#else
		#define DORA_DEBUG 1
	#endif
#endif

/** @brief Disable log function, set with the debug flag. */
#ifndef DORA_DISABLE_LOG
	#if DORA_DEBUG
		#define DORA_DISABLE_LOG 0
	#else
		#define DORA_DISABLE_LOG 1
	#endif
#endif

/** @brief Disable assert function, set with the debug flag. */
#ifndef DORA_DISABLE_ASSERTION
	#if DORA_DEBUG
		#define DORA_DISABLE_ASSERTION 0
	#else
		#define DORA_DISABLE_ASSERTION 1
	#endif
#endif

/** @brief The buffer size for content copy function.
*/
#ifndef DORA_COPY_BUFFER_SIZE
	#define DORA_COPY_BUFFER_SIZE 4096
#endif

/** @brief Flag to disable lua binding debug codes.
*/
#if !DORA_DEBUG
	#define TOLUA_RELEASE
#endif

/** @brief The single squared texture size for baking fonts.
*/
#ifndef DORA_FONT_TEXTURE_SIZE
	#define DORA_FONT_TEXTURE_SIZE 2048
#endif

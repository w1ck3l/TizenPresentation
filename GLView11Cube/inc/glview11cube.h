/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __glview11cube_H__
#define __glview11cube_H__

#include <dlog.h>
#include <Elementary.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "glview11cube"

#define APPDATA_KEY "AppData"

typedef struct appdata
{
   Evas_Object *table, *bg;
   Evas_Object *win;
   Evas_Object *glview;
   Ecore_Animator *anim;
   Evas_Object *conform;

   GLuint tex_ids[2];
   int current_tex_index;
} appdata_s;

#endif /* __glview11cube_GLES_H__ */

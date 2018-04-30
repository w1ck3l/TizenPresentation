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

#include <app.h>
#include <efl_extension.h>
#include "glview11cube.h"
#include "glview11cube_GLES.h"

static Evas_Object*
_glview_create(appdata_s *ad)
{
   Evas_Object *obj;

   /* Create a GLView with an OpenGL-ES 1.1 context */
   obj = elm_glview_version_add(ad->win, EVAS_GL_GLES_1_X);
   elm_table_pack(ad->table, obj, 0, 0, 1, 1);
   evas_object_data_set(obj, APPDATA_KEY, ad);

   elm_glview_mode_set(obj, ELM_GLVIEW_ALPHA | ELM_GLVIEW_DEPTH);
   elm_glview_resize_policy_set(obj, ELM_GLVIEW_RESIZE_POLICY_RECREATE);
   elm_glview_render_policy_set(obj, ELM_GLVIEW_RENDER_POLICY_ON_DEMAND);

   elm_glview_init_func_set(obj, init_gles);
   elm_glview_del_func_set(obj, destroy_gles);
   elm_glview_resize_func_set(obj, resize_gl);
   elm_glview_render_func_set(obj, draw_gl);

   return obj;
}

static void
_win_resize_cb(void *data, Evas *e EINA_UNUSED,
               Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   int w, h;
   appdata_s *ad = data;

   evas_object_geometry_get(ad->win, NULL, NULL, &w, &h);
   evas_object_resize(ad->table, w, h);
   evas_object_resize(ad->bg, w, h);
}

static Eina_Bool
_anim_cb(void *data)
{
   appdata_s *ad = data;

   elm_glview_changed_set(ad->glview);
   return ECORE_CALLBACK_RENEW;
}

static void
_destroy_anim(void *data, Evas *evas, Evas_Object *obj, void *event_info)
{
   Ecore_Animator *ani = data;
   ecore_animator_del(ani);
}


static void
_close_cb(void *data EINA_UNUSED,
          Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   ui_app_exit();
}

static bool
app_create(void *data)
{
   Evas_Object *o, *t;
   appdata_s *ad = (appdata_s*)data;

   /* Force OpenGL engine */
   elm_config_accel_preference_set("opengl");

   /* Add a window */
   ad->win = o = elm_win_add(NULL,"glview", ELM_WIN_BASIC);
   evas_object_smart_callback_add(o, "delete,request", _close_cb, ad);
   evas_object_event_callback_add(o, EVAS_CALLBACK_RESIZE, _win_resize_cb, ad);
   eext_object_event_callback_add(o, EEXT_CALLBACK_BACK, _close_cb, ad);
   evas_object_show(o);

   if (elm_win_wm_rotation_supported_get(o))
   {
        int rots[4] = { 0, 90, 180, 270 };
        elm_win_wm_rotation_available_rotations_set(o, rots, 4);
   }

   /* Add a background */
   ad->bg = o = elm_bg_add(ad->win);
   elm_win_resize_object_add(ad->win, ad->bg);
   elm_bg_color_set(o, 68, 68, 68);
   evas_object_show(o);

   /* Add a resize conformant */
   elm_win_conformant_set(ad->win, EINA_TRUE);
   elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
   elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_TRANSPARENT);

   ad->conform = o = elm_conformant_add(ad->win);
   elm_win_resize_object_add(ad->win, ad->conform);
   evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(o);

   ad->table = t = elm_table_add(ad->win);
   elm_object_content_set(ad->conform, t);
   evas_object_show(t);

   o = elm_button_add(ad->win);
   elm_object_text_set(o, "Quit");
   evas_object_smart_callback_add(o, "clicked", _close_cb, ad);
   elm_table_pack(t, o, 0, 1, 1, 1);
   evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(o, 0.00001, 0.00001);
   evas_object_show(o);

   ad->glview = o = _glview_create(ad);
   evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(o);

   ad->anim = ecore_animator_add(_anim_cb, ad);
   evas_object_event_callback_add(ad->glview, EVAS_CALLBACK_DEL, _destroy_anim, ad->anim);

   return true;
}

static void
app_pause(void *data)
{
	appdata_s *ad = data;
	ecore_animator_freeze(ad->anim);
}

static void
app_resume(void *data)
{
	appdata_s *ad = data;
	ecore_animator_thaw(ad->anim);
}

int
main(int argc, char *argv[])
{
	appdata_s ad = {0,};
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {NULL,};

	event_callback.create = app_create;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG, "The application failed to start, and returned %d", ret);

	return ret;
}

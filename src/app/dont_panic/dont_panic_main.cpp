/****************************************************************************
 *   Aug 3 12:17:11 2020
 *   Copyright  2020  Dirk Brosswick
 *   Email: dirk.brosswick@googlemail.com
 ****************************************************************************/
 
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include "config.h"
#include <TTGO.h>

#include "dont_panic.h"
#include "dont_panic_main.h"
#include "dont_panic_quote.h"

#include "gui/mainbar/app_tile/app_tile.h"
#include "gui/mainbar/main_tile/main_tile.h"
#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"

lv_obj_t *dont_panic_main_tile = NULL;
lv_style_t dont_panic_main_style;

lv_task_t * _dont_panic_task;

LV_IMG_DECLARE(exit_32px);
LV_IMG_DECLARE(setup_32px);
LV_IMG_DECLARE(refresh_32px);
LV_IMG_DECLARE(dont_panic_240);
LV_FONT_DECLARE(Ubuntu_72px);

void enter_dont_panic_quote_event_cb( lv_obj_t * obj, lv_event_t event );
void dont_panic_task( lv_task_t * task );

void dont_panic_main_setup( uint32_t tile_num ) {

    dont_panic_main_tile = mainbar_get_tile_obj( tile_num );
    lv_style_copy( &dont_panic_main_style, mainbar_get_style() );

    lv_obj_t * exit_btn = lv_imgbtn_create( dont_panic_main_tile, NULL);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_RELEASED, &dont_panic_240);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_PRESSED, &dont_panic_240);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_RELEASED, &dont_panic_240);
    lv_imgbtn_set_src(exit_btn, LV_BTN_STATE_CHECKED_PRESSED, &dont_panic_240);
    lv_obj_add_style(exit_btn, LV_IMGBTN_PART_MAIN, &dont_panic_main_style );
    lv_obj_align(exit_btn, dont_panic_main_tile, LV_ALIGN_CENTER, 0, 0 );
    lv_obj_set_event_cb( exit_btn, enter_dont_panic_quote_event_cb );

    // create an task that runs every secound
    _dont_panic_task = lv_task_create( dont_panic_task, 1000, LV_TASK_PRIO_MID, NULL );
}

void enter_dont_panic_quote_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       statusbar_hide( true );
                                        dont_panic_load_random_quote();
                                        mainbar_jump_to_tilenumber( dont_panic_get_app_setup_tile_num(), LV_ANIM_ON );
                                        break;
    }
}

void dont_panic_task( lv_task_t * task ) {
    // put your code her
}
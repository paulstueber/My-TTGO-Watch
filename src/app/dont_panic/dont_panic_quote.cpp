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
#include "dont_panic_quote.h"

#include "gui/mainbar/mainbar.h"
#include "gui/statusbar.h"

#include "hardware/json_psram_allocator.h"

lv_obj_t *dont_panic_quote_tile = NULL;
lv_style_t dont_panic_quote_style;

lv_obj_t *dont_panic_foobar_switch = NULL;
lv_obj_t * quote_label = NULL;
lv_obj_t * page  = NULL;

LV_IMG_DECLARE(exit_32px);

static void exit_dont_panic_quote_event_cb( lv_obj_t * obj, lv_event_t event );

void dont_panic_quote_setup( uint32_t tile_num ) {

    dont_panic_quote_tile = mainbar_get_tile_obj( tile_num );
    lv_style_copy( &dont_panic_quote_style, mainbar_get_style() );

    lv_style_set_bg_color( &dont_panic_quote_style, LV_OBJ_PART_MAIN, LV_COLOR_RED);
    lv_style_set_bg_opa( &dont_panic_quote_style, LV_OBJ_PART_MAIN, LV_OPA_100);
    lv_style_set_border_width( &dont_panic_quote_style, LV_OBJ_PART_MAIN, 0);
    lv_obj_add_style( dont_panic_quote_tile, LV_OBJ_PART_MAIN, &dont_panic_quote_style );

    lv_obj_t *exit_cont = lv_obj_create( dont_panic_quote_tile, NULL );
    lv_obj_set_size( exit_cont, lv_disp_get_hor_res( NULL ) , 40);
    lv_obj_add_style( exit_cont, LV_OBJ_PART_MAIN, &dont_panic_quote_style  );
    lv_obj_align( exit_cont, dont_panic_quote_tile, LV_ALIGN_IN_TOP_MID, 0, 10 );

    lv_obj_t *exit_btn = lv_imgbtn_create( exit_cont, NULL);
    lv_imgbtn_set_src( exit_btn, LV_BTN_STATE_RELEASED, &exit_32px);
    lv_imgbtn_set_src( exit_btn, LV_BTN_STATE_PRESSED, &exit_32px);
    lv_imgbtn_set_src( exit_btn, LV_BTN_STATE_CHECKED_RELEASED, &exit_32px);
    lv_imgbtn_set_src( exit_btn, LV_BTN_STATE_CHECKED_PRESSED, &exit_32px);
    lv_obj_add_style( exit_btn, LV_IMGBTN_PART_MAIN, &dont_panic_quote_style );
    lv_obj_align( exit_btn, exit_cont, LV_ALIGN_IN_TOP_LEFT, 10, 0 );
    lv_obj_set_event_cb( exit_btn, exit_dont_panic_quote_event_cb );
    
    lv_obj_t *exit_label = lv_label_create( exit_cont, NULL);
    lv_obj_add_style( exit_label, LV_OBJ_PART_MAIN, &dont_panic_quote_style  );
    lv_label_set_text( exit_label, "close");
    lv_obj_align( exit_label, exit_btn, LV_ALIGN_OUT_RIGHT_MID, 5, 0 );

    lv_obj_t *quote_button = lv_btn_create( dont_panic_quote_tile, NULL);
    lv_obj_set_size(quote_button, lv_disp_get_hor_res( NULL ), lv_disp_get_ver_res( NULL ) - 45);
    lv_obj_add_style( quote_button, LV_IMGBTN_PART_MAIN, &dont_panic_quote_style );
    lv_obj_align( quote_button, exit_cont, LV_ALIGN_OUT_BOTTOM_MID, 0, 0 );
    lv_obj_set_event_cb( quote_button, exit_dont_panic_quote_event_cb );

        /*Create a page*/
    page = lv_page_create(quote_button, NULL);
    //lv_obj_set_size(page, lv_disp_get_hor_res( NULL ), lv_disp_get_ver_res( NULL ) - 45);
    lv_obj_set_size(page, lv_disp_get_hor_res( NULL ), lv_disp_get_ver_res( NULL ) - 45);
    lv_obj_add_style( page, LV_OBJ_PART_MAIN, &dont_panic_quote_style  );
    lv_obj_align(page, quote_button, LV_ALIGN_CENTER, 0, 0 );

    /*Create a label on the page*/
    quote_label = lv_label_create(page, NULL);
    lv_label_set_long_mode(quote_label, LV_LABEL_LONG_BREAK);            /*Automatically break long lines*/
    lv_obj_set_width(quote_label, lv_page_get_width_fit(page));          /*Set the label width to max value to not show hor. scroll bars*/
    lv_label_set_text(quote_label, "Don't Panic!");
    dont_panic_load_random_quote();
}

static void exit_dont_panic_quote_event_cb( lv_obj_t * obj, lv_event_t event ) {
    switch( event ) {
        case( LV_EVENT_CLICKED ):       mainbar_jump_to_maintile( LV_ANIM_OFF );
                                        break;
    }
}

/*
 *
 */
void dont_panic_load_random_quote( void ) {
    if ( SPIFFS.exists( DONT_PANIC_JSON_QUOTES_FILE ) ) {        
        fs::File file = SPIFFS.open( DONT_PANIC_JSON_QUOTES_FILE, FILE_READ );
        if (!file) {
            log_e("Can't open file: %s!", DONT_PANIC_JSON_QUOTES_FILE );
        }
        else {
            log_i("Loaded file: %s!", DONT_PANIC_JSON_QUOTES_FILE );
            int filesize = file.size();
            SpiRamJsonDocument doc( filesize * 2 );

            DeserializationError error = deserializeJson( doc, file );
            if ( error ) {
                log_e("deserializeJson() failed: %s", error.c_str() );
            }
            else {
                //JsonArray quotes = doc.to<JsonArray>();
                uint8_t index = random (0, doc.size());
                log_i("selected qoute nr %d of %d : %s", index, doc.size(), doc[index]);
                lv_label_set_text(quote_label, doc[index]);
                lv_page_scroll_ver(page, lv_obj_get_height(quote_label));
            }        
            doc.clear();
        }
        file.close();
    } else {
        log_i("No file %s exists!", DONT_PANIC_JSON_QUOTES_FILE );
    }
}

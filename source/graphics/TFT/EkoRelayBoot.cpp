#if HAS_TFT && defined(VIEW_320x240)
#include "graphics/view/TFT/EkoRelayBoot.h"
#include "fonts.h"
#include "screens.h"

LV_IMAGE_DECLARE(img_ekorelay_boot);

void EkoRelayBoot::init(const char *firmware_version)
{
    lv_image_set_src(objects.boot_logo, &img_ekorelay_boot);
    lv_obj_align(objects.boot_logo, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(objects.boot_screen, lv_color_hex(0xf7f7f2), 0);
    lv_obj_move_background(objects.boot_logo);
    lv_obj_remove_flag(objects.boot_logo, LV_OBJ_FLAG_CLICKABLE);
    lv_label_set_text(objects.meshtastic_url, "");
    lv_obj_set_style_text_color(objects.boot_screen, lv_color_hex(0x003c2b), 0);
    lv_obj_set_style_text_font(objects.firmware_label, &ui_font_montserrat_12, 0);
    lv_obj_set_width(objects.firmware_label, 312);
    lv_obj_set_style_text_align(objects.firmware_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(objects.firmware_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_align(objects.firmware_label, LV_ALIGN_BOTTOM_MID, 0, -2);
    lv_label_set_text(objects.firmware_label, firmware_version);
}
#endif

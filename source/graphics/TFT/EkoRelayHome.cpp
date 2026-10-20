#if HAS_TFT && defined(VIEW_320x240)
#include "graphics/view/TFT/EkoRelayHome.h"
#include "fonts.h"
#include "images.h"
#include "screens.h"

LV_IMAGE_DECLARE(img_ekorelay_mark);

namespace EkoRelayHome
{
static lv_obj_t *menu;
static lv_obj_t *nodeCount;
static lv_obj_t *messageCount;
static lv_obj_t *statusLabel;

static lv_obj_t *label(lv_obj_t *parent, const char *text, int x, int y, const lv_font_t *font)
{
    auto obj = lv_label_create(parent);
    lv_label_set_text(obj, text);
    lv_obj_set_pos(obj, x, y);
    lv_obj_set_style_text_font(obj, font, 0);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
    return obj;
}

static void openScreen(lv_event_t *event)
{
    auto target = static_cast<lv_obj_t *>(lv_event_get_user_data(event));
    lv_group_focus_obj(target);
    lv_obj_send_event(target, LV_EVENT_CLICKED, nullptr);
}

static lv_obj_t *card(int row, const char *title, const char *subtitle, const lv_image_dsc_t *icon, lv_obj_t *target)
{
    auto button = lv_button_create(menu);
    lv_obj_set_pos(button, 8, 28 + row * 46);
    lv_obj_set_size(button, 266, 42);
    lv_obj_set_style_pad_all(button, 0, 0);
    lv_obj_set_style_radius(button, 8, 0);
    lv_obj_set_style_shadow_width(button, 0, 0);
    lv_obj_set_style_bg_color(button, lv_color_hex(0xf7f7f2), 0);
    lv_obj_set_style_text_color(button, lv_color_hex(0x073e2e), 0);
    lv_obj_set_style_bg_color(button, lv_color_hex(0xd6eacb), LV_STATE_PRESSED);
    lv_obj_set_style_outline_color(button, lv_color_hex(0x9ed469), LV_STATE_FOCUS_KEY);
    lv_obj_set_style_outline_width(button, 2, LV_STATE_FOCUS_KEY);
    lv_obj_add_event_cb(button, openScreen, LV_EVENT_CLICKED, target);
    auto image = lv_image_create(button);
    lv_image_set_src(image, icon);
    lv_obj_align(image, LV_ALIGN_LEFT_MID, 9, 0);
    lv_obj_set_style_image_recolor(image, lv_color_hex(0x165a3e), 0);
    lv_obj_set_style_image_recolor_opa(image, LV_OPA_COVER, 0);
    label(button, title, 46, 2, &ui_font_montserrat_14);
    auto detail = label(button, subtitle, 46, 22, &ui_font_montserrat_12);
    lv_obj_set_size(detail, 198, 17);
    lv_label_set_long_mode(detail, LV_LABEL_LONG_CLIP);
    lv_group_add_obj(lv_group_get_default(), button);
    return detail;
}

void showMenu()
{
    if (!menu)
        return;
    lv_obj_add_flag(objects.home_container, LV_OBJ_FLAG_HIDDEN);
    lv_obj_remove_flag(menu, LV_OBJ_FLAG_HIDDEN);
}

void init()
{
    if (menu)
        return;
    lv_obj_set_style_bg_color(objects.home_panel, lv_color_hex(0x073e2e), 0);
    lv_obj_set_style_pad_all(objects.home_panel, 0, 0);
    lv_obj_set_style_bg_color(objects.top_panel, lv_color_hex(0x073e2e), 0);
    lv_obj_set_style_text_color(objects.top_panel, lv_color_hex(0xf7f7f2), 0);
    lv_obj_set_style_pad_all(objects.top_panel, 0, 0);
    lv_obj_set_width(objects.top_panel, 220);
    lv_label_set_text(objects.meshtastic_label, "EkoRelay");
    lv_obj_set_style_text_font(objects.meshtastic_label, &ui_font_montserrat_16, 0);
    lv_obj_align(objects.meshtastic_label, LV_ALIGN_LEFT_MID, 48, 0);
    lv_image_set_src(objects.meshtastic_image, &img_ekorelay_mark);
    lv_obj_set_size(objects.meshtastic_image, 34, 16);
    lv_obj_align(objects.meshtastic_image, LV_ALIGN_LEFT_MID, 8, 0);
    lv_obj_set_style_image_recolor_opa(objects.meshtastic_image, LV_OPA_TRANSP, 0);

    menu = lv_obj_create(objects.home_panel);
    lv_obj_remove_style_all(menu);
    lv_obj_set_size(menu, LV_PCT(100), LV_PCT(100));
    lv_obj_remove_flag(menu, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_text_color(menu, lv_color_hex(0xe2eedb), 0);
    statusLabel = label(menu, "LoRa: —    --:--", 8, 4, &ui_font_montserrat_12);
    lv_obj_set_width(statusLabel, 240);
    nodeCount = card(0, "Узлы рядом", "Ожидание данных", &img_nodes_button_image, objects.nodes_button);
    messageCount = card(1, "Сообщения", "Ожидание данных", &img_messages_button_image, objects.messages_button);
    card(2, "Карта", "Узлы и местоположение", &img_map_button_image, objects.map_button);
    card(3, "Настройки", "Сеть и устройство", &img_settings_button_image, objects.settings_button);

    auto diagnostics = lv_button_create(objects.top_panel);
    lv_obj_set_size(diagnostics, 24, 22);
    lv_obj_align(diagnostics, LV_ALIGN_RIGHT_MID, -4, 0);
    lv_obj_set_style_pad_all(diagnostics, 0, 0);
    lv_obj_set_style_bg_opa(diagnostics, LV_OPA_TRANSP, 0);
    lv_obj_set_style_shadow_width(diagnostics, 0, 0);
    lv_obj_set_style_text_color(diagnostics, lv_color_hex(0xf7f7f2), 0);
    auto info = label(diagnostics, "i", 0, 0, &ui_font_montserrat_16);
    lv_obj_center(info);
    lv_obj_add_event_cb(
        diagnostics,
        [](lv_event_t *) {
            bool showingMenu = !lv_obj_has_flag(menu, LV_OBJ_FLAG_HIDDEN);
            if (showingMenu) {
                lv_obj_add_flag(menu, LV_OBJ_FLAG_HIDDEN);
                lv_obj_remove_flag(objects.home_container, LV_OBJ_FLAG_HIDDEN);
            } else {
                showMenu();
            }
        },
        LV_EVENT_CLICKED, nullptr);
    lv_group_add_obj(lv_group_get_default(), diagnostics);
    showMenu();
}

void updateCounts(unsigned online, unsigned total, unsigned unread)
{
    if (!menu)
        return;
    lv_label_set_text_fmt(nodeCount, "В сети: %u / Всего: %u", online, total);
    lv_label_set_text_fmt(messageCount, "Непрочитанных: %u", unread);
}

void updateStatus(bool radioEnabled, bool recentPacket, const char *clock)
{
    if (menu)
        lv_label_set_text_fmt(statusLabel, "LoRa: %s   %s",
                              !radioEnabled  ? "TX выкл."
                              : recentPacket ? "приём"
                                             : "ожидание",
                              clock);
}
} // namespace EkoRelayHome
#endif

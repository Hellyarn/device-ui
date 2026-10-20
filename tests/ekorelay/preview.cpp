#include "graphics/view/TFT/EkoRelayBoot.h"
#include "graphics/view/TFT/EkoRelayHome.h"
#include "lv_i18n.h"
#include "lvgl.h"
#include "ui.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <initializer_list>
static uint16_t pixels[320 * 240];
static int clicks[4];
static void clicked(lv_event_t *e)
{
    ++clicks[(uintptr_t)lv_event_get_user_data(e)];
}
static lv_obj_t *findText(lv_obj_t *p, const char *text)
{
    if (lv_obj_check_type(p, &lv_label_class) && !strcmp(lv_label_get_text(p), text))
        return p;
    for (unsigned i = 0; i < lv_obj_get_child_count(p); ++i)
        if (auto found = findText(lv_obj_get_child(p, i), text))
            return found;
    return nullptr;
}
static void capture(const char *path)
{
    lv_obj_update_layout(lv_screen_active());
    lv_refr_now(nullptr);
    FILE *f = fopen(path, "wb");
    assert(f);
    fprintf(f, "P6\n320 240\n255\n");
    for (auto p : pixels) {
        unsigned char rgb[3] = {(unsigned char)(((p >> 11) & 31) * 255 / 31), (unsigned char)(((p >> 5) & 63) * 255 / 63),
                                (unsigned char)((p & 31) * 255 / 31)};
        fwrite(rgb, 1, 3, f);
    }
    fclose(f);
}
int main(int argc, char **argv)
{
    const char *dir = argc > 1 ? argv[1] : ".";
    char path[1024];
    lv_init();
    auto d = lv_display_create(320, 240);
    lv_display_set_color_format(d, LV_COLOR_FORMAT_RGB565);
    lv_display_set_buffers(d, pixels, nullptr, sizeof(pixels), LV_DISPLAY_RENDER_MODE_FULL);
    lv_display_set_flush_cb(d, [](lv_display_t *d, const lv_area_t *, uint8_t *) { lv_display_flush_ready(d); });
    lv_group_set_default(lv_group_create());
    lv_i18n_init(lv_i18n_language_pack);
    lv_i18n_set_locale("ru");
    ui_init_boot();
    EkoRelayBoot::init("2.7.26-test");
    lv_screen_load(objects.boot_screen);
    snprintf(path, sizeof(path), "%s/boot.ppm", dir);
    capture(path);
    assert(!lv_obj_has_flag(objects.firmware_label, LV_OBJ_FLAG_HIDDEN));
    assert(!strcmp(lv_label_get_text(objects.meshtastic_url), ""));
    assert(lv_obj_get_y(objects.firmware_label) >= 216);
    lv_label_set_text(objects.firmware_label, "Starting radio / проверка");
    assert(lv_obj_get_width(objects.firmware_label) == 312);
    ui_init();
    lv_screen_load(objects.main_screen);
    EkoRelayHome::init();
    EkoRelayHome::updateCounts(3, 12, 2);
    EkoRelayHome::updateStatus(true, true, "14:27");
    lv_obj_t *targets[] = {objects.nodes_button, objects.messages_button, objects.map_button, objects.settings_button};
    const char *titles[] = {"Узлы рядом", "Сообщения", "Карта", "Настройки"};
    for (unsigned i = 0; i < 4; ++i)
        lv_obj_add_event_cb(targets[i], clicked, LV_EVENT_CLICKED, (void *)(uintptr_t)i);
    snprintf(path, sizeof(path), "%s/home.ppm", dir);
    capture(path);
    for (unsigned i = 0; i < 4; ++i) {
        auto t = findText(objects.home_panel, titles[i]);
        assert(t);
        auto b = lv_obj_get_parent(t);
        lv_area_t a;
        lv_obj_get_coords(b, &a);
        assert(a.x1 >= 38 && a.x2 < 320 && a.y1 >= 24 && a.y2 < 240);
        lv_obj_send_event(b, LV_EVENT_CLICKED, nullptr);
        assert(clicks[i] == 1);
        assert(lv_group_get_focused(lv_group_get_default()) == targets[i]);
    }
    assert(findText(objects.home_panel, "В сети: 3 / Всего: 12"));
    assert(findText(objects.home_panel, "Непрочитанных: 2"));
    EkoRelayHome::updateCounts(0, 0, 0);
    EkoRelayHome::updateStatus(false, false, "--:--");
    assert(findText(objects.home_panel, "Непрочитанных: 0"));
    auto info = findText(objects.top_panel, "i");
    assert(info);
    assert(EkoRelayHome::focusMenu());
    assert(lv_group_get_focused(lv_group_get_default()) == lv_obj_get_parent(findText(objects.home_panel, titles[0])));
    lv_obj_send_event(lv_obj_get_parent(info), LV_EVENT_CLICKED, nullptr);
    assert(!lv_obj_has_flag(objects.home_container, LV_OBJ_FLAG_HIDDEN));
    assert(!EkoRelayHome::focusMenu());
    EkoRelayHome::showMenu();
    assert(lv_obj_has_flag(objects.home_container, LV_OBJ_FLAG_HIDDEN));
    for (auto font : {&ui_font_montserrat_12, &ui_font_montserrat_14}) {
        lv_font_glyph_dsc_t glyph;
        for (uint32_t ch = 0x410; ch <= 0x44f; ++ch) {
            assert(lv_font_get_glyph_dsc(font, &glyph, ch, 0));
            assert(!glyph.is_placeholder);
        }
    }
    puts("PASS: boot/footer, bounds, four navigation events, group focus, diagnostics, live counters, Cyrillic fonts");
}

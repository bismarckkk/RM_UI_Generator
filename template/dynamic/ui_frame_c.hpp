//
// Created by abc55 on 2025/5/7.
//

#ifndef UI_FRAME_C_DYNAMIC_HPP
#define UI_FRAME_C_DYNAMIC_HPP

#include <string>

inline const std::string ui_frame_c_template = R"(//
// Created by RM UI Designer
// Dynamic Edition
//

#include "string.h"
#include "ui_interface.h"
#include "ui_{{ frame }}.h"

#define TOTAL_FIGURE {{ length(objs) }}
#define TOTAL_STRING {{ length(textObjs) }}

## if length(objs) != 0
ui_interface_figure_t ui_{{ frame }}_now_figures[TOTAL_FIGURE];
uint8_t ui_{{ frame }}_dirty_figure[TOTAL_FIGURE];
## endif
## if length(textObjs) != 0
ui_interface_string_t ui_{{ frame }}_now_strings[TOTAL_STRING];
uint8_t ui_{{ frame }}_dirty_string[TOTAL_STRING];
## endif

#ifndef MANUAL_DIRTY
## if length(objs) != 0
ui_interface_figure_t ui_{{ frame }}_last_figures[TOTAL_FIGURE];
## endif
## if length(textObjs) != 0
ui_interface_string_t ui_{{ frame }}_last_strings[TOTAL_STRING];
## endif
#endif

## if length(objs) != 0 and length(textObjs) != 0
#define SCAN_AND_SEND() ui_scan_and_send(ui_g_now_figures, ui_g_dirty_figure, ui_g_now_strings, ui_g_dirty_string, TOTAL_FIGURE, TOTAL_STRING)
## else if length(objs) != 0
#define SCAN_AND_SEND() ui_scan_and_send(ui_g_now_figures, ui_g_dirty_figure, NULL, NULL, TOTAL_FIGURE, TOTAL_STRING)
## else if length(textObjs) != 0
#define SCAN_AND_SEND() ui_scan_and_send(NULL, NULL, ui_g_now_strings, ui_g_dirty_string, TOTAL_FIGURE, TOTAL_STRING)
## endif

void ui_init_{{ frame }}() {
## for obj in objs
{{ obj.init_code }}
## endfor
## for obj in textObjs
{{ obj.init_code }}
## endfor
    uint32_t idx = 0;
## if length(objs) != 0
    for (int i = 0; i < TOTAL_FIGURE; i++) {
        ui_{{ frame }}_now_figures[i].figure_name[2] = idx & 0xFF;
        ui_{{ frame }}_now_figures[i].figure_name[1] = (idx >> 8) & 0xFF;
        ui_{{ frame }}_now_figures[i].figure_name[0] = (idx >> 16) & 0xFF;
        ui_{{ frame }}_now_figures[i].operate_type = 1;
#ifndef MANUAL_DIRTY
        ui_{{ frame }}_last_figures[i] = ui_{{ frame }}_now_figures[i];
#endif
        ui_{{ frame }}_dirty_figure[i] = 1;
        idx++;
    }
## endif
## if length(textObjs) != 0
    for (int i = 0; i < TOTAL_STRING; i++) {
        ui_{{ frame }}_now_strings[i].figure_name[2] = idx & 0xFF;
        ui_{{ frame }}_now_strings[i].figure_name[1] = (idx >> 8) & 0xFF;
        ui_{{ frame }}_now_strings[i].figure_name[0] = (idx >> 16) & 0xFF;
        ui_{{ frame }}_now_strings[i].operate_type = 1;
#ifndef MANUAL_DIRTY
        ui_{{ frame }}_last_strings[i] = ui_{{ frame }}_now_strings[i];
#endif
        ui_{{ frame }}_dirty_string[i] = 1;
        idx++;
    }
## endif

    SCAN_AND_SEND();

## if length(objs) != 0
    for (int i = 0; i < TOTAL_FIGURE; i++) {
        ui_{{ frame }}_now_figures[i].operate_type = 2;
    }
## endif
## if length(textObjs) != 0
    for (int i = 0; i < TOTAL_STRING; i++) {
        ui_{{ frame }}_now_strings[i].operate_type = 2;
    }
## endif
}

void ui_update_{{ frame }}() {
#ifndef MANUAL_DIRTY
## if length(objs) != 0
    for (int i = 0; i < TOTAL_FIGURE; i++) {
        if (memcmp(&ui_{{ frame }}_now_figures[i], &ui_{{ frame }}_last_figures[i], sizeof(ui_{{ frame }}_now_figures[i])) != 0) {
            ui_{{ frame }}_dirty_figure[i] = 1;
            ui_{{ frame }}_last_figures[i] = ui_{{ frame }}_now_figures[i];
        }
    }
## endif
## if length(textObjs) != 0
    for (int i = 0; i < TOTAL_STRING; i++) {
        if (memcmp(&ui_{{ frame }}_now_strings[i], &ui_{{ frame }}_last_strings[i], sizeof(ui_{{ frame }}_now_strings[i])) != 0) {
            ui_{{ frame }}_dirty_string[i] = 1;
            ui_{{ frame }}_last_strings[i] = ui_{{ frame }}_now_strings[i];
        }
    }
## endif
#endif
    SCAN_AND_SEND();
}
)";

#endif //UI_FRAME_C_DYNAMIC_HPP

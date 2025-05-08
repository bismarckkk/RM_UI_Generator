//
// Created by abc55 on 2025/5/8.
//

#ifndef UI_FRAME_C_STATIC_HPP
#define UI_FRAME_C_STATIC_HPP

#include <string>

inline const std::string ui_frame_c_template_s = R"(//
// Created by RM UI Designer
// Static Edition
//

#include <string.h>

#include "ui_interface.h"

## for group in groups
## for split in group.splits
## set split_name_arr=["", "", 0]
## set split_name_arr.0=frame
## set split_name_arr.1=group.name
## set split_name_arr.2=split.split_id
## set split_name=join(split_name_arr, "_")
ui_{{ split.message_len }}_frame_t ui_{{ split_name }};

## for obj in split.objs
{{ obj.type }} *{{ obj.name }} = ({{ obj.type }}*)&(ui_{{ split_name }}.data[{{ loop.index }}]);
## endfor

inline void _ui_init_{{ split_name }}() {
    for (int i = 0; i < {{ length(split.objs) }}; i++) {
        ui_{{ split_name }}.data[i].figure_name[0] = {{ frame_id }};
        ui_{{ split_name }}.data[i].figure_name[1] = {{ group.id }};
        ui_{{ split_name }}.data[i].figure_name[2] = i + {{ split.start_id }};
        ui_{{ split_name }}.data[i].operate_type = 1;
    }
    for (int i = {{ length(split.objs) }}; i < {{ split.message_len }}; i++) {
        ui_{{ split_name }}.data[i].operate_type = 0;
    }

## for obj in split.objs
{{ obj.init_code }}
## endfor

    ui_proc_{{ split.message_len }}_frame(&ui_{{ split_name }});
    SEND_MESSAGE((uint8_t *) &ui_{{ split_name }}, sizeof(ui_{{ split_name }}));
}

inline void _ui_update_{{ split_name }}() {
    for (int i = 0; i < {{ length(split.objs) }}; i++) {
        ui_{{ split_name }}.data[i].operate_type = 2;
    }

    ui_proc_{{ split.message_len }}_frame(&ui_{{ split_name }});
    SEND_MESSAGE((uint8_t *) &ui_{{ split_name }}, sizeof(ui_{{ split_name }}));
}

inline void _ui_remove_{{ split_name }}() {
    for (int i = 0; i < {{ length(split.objs) }}; i++) {
        ui_{{ split_name }}.data[i].operate_type = 3;
    }

    ui_proc_{{ split.message_len }}_frame(&ui_{{ split_name }});
    SEND_MESSAGE((uint8_t *) &ui_{{ split_name }}, sizeof(ui_{{ split_name }}));
}
## endfor

## for split in group.textSplits
## set split_name_arr=["", "", 0]
## set split_name_arr.0=frame
## set split_name_arr.1=group.name
## set split_name_arr.2=split.split_id
## set split_name=join(split_name_arr, "_")
ui_string_frame_t ui_{{ split_name }};
ui_interface_string_t* {{ split.obj.name }} = &(ui_{{ split_name }}.option);

inline void _ui_init_{{ split_name }}() {
    ui_{{ split_name }}.option.figure_name[0] = {{ frame_id }};
    ui_{{ split_name }}.option.figure_name[1] = {{ group.id }};
    ui_{{ split_name }}.option.figure_name[2] = {{ split.start_id }};
    ui_{{ split_name }}.option.operate_type = 1;

{{ split.obj.init_code }}

    ui_proc_string_frame(&ui_{{ split_name }});
    SEND_MESSAGE((uint8_t *) &ui_{{ split_name }}, sizeof(ui_{{ split_name }}));
}

inline void _ui_update_{{ split_name }}() {
    ui_{{ split_name }}.option.operate_type = 2;

    ui_proc_string_frame(&ui_{{ split_name }});
    SEND_MESSAGE((uint8_t *) &ui_{{ split_name }}, sizeof(ui_{{ split_name }}));
}

inline void _ui_remove_{{ split_name }}() {
    ui_{{ split_name }}.option.operate_type = 3;

    ui_proc_string_frame(&ui_{{ split_name }});
    SEND_MESSAGE((uint8_t *) &ui_{{ split_name }}, sizeof(ui_{{ split_name }}));
}
## endfor

void ui_init_{{ frame }}_{{ group.name }}() {
## for split in group.splits
    _ui_init_{{ frame }}_{{ group.name }}_{{ split.split_id }}();
## endfor
## for split in group.textSplits
    _ui_init_{{ frame }}_{{ group.name }}_{{ split.split_id }}();
## endfor
}

void ui_update_{{ frame }}_{{ group.name }}() {
## for split in group.splits
    _ui_update_{{ frame }}_{{ group.name }}_{{ split.split_id }}();
## endfor
## for split in group.textSplits
    _ui_update_{{ frame }}_{{ group.name }}_{{ split.split_id }}();
## endfor
}

void ui_remove_{{ frame }}_{{ group.name }}() {
## for split in group.splits
    _ui_remove_{{ frame }}_{{ group.name }}_{{ split.split_id }}();
## endfor
## for split in group.textSplits
    _ui_remove_{{ frame }}_{{ group.name }}_{{ split.split_id }}();
## endfor
}

## endfor
)";

#endif //UI_FRAME_C_STATIC_HPP

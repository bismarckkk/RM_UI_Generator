//
// Created by abc55 on 2025/5/7.
//

#ifndef UI_FRAME_H_DYNAMIC_HPP
#define UI_FRAME_H_DYNAMIC_HPP

#include <string>

inline const std::string ui_frame_h_template = R"(//
// Created by RM UI Designer
// Dynamic Edition
//

#ifndef UI_{{ frame }}_H
#define UI_{{ frame }}_H

#include "ui_interface.h"

## if length(objs) != 0
extern ui_interface_figure_t ui_{{ frame }}_now_figures[{{ length(objs) }}];
extern uint8_t ui_{{ frame }}_dirty_figure[{{ length(objs) }}];
## endif
## if length(textObjs) != 0
extern ui_interface_string_t ui_{{ frame }}_now_strings[{{ length(textObjs) }}];
extern uint8_t ui_{{ frame }}_dirty_string[{{ length(textObjs) }}];
## endif

## for obj in objs
#define {{ obj.name }} (({{ obj.type }}*)&(ui_{{ frame }}_now_figures[{{ loop.index }}]))
## endfor

## for obj in textObjs
#define {{ obj.name }} (&(ui_{{ frame }}_now_strings[{{ loop.index }}]))
## endfor

#ifdef MANUAL_DIRTY
## for obj in objs
#define {{ obj.name }}_dirty (ui_{{ frame }}_dirty_figure[{{ loop.index }}])
## endfor

## for obj in textObjs
#define {{ obj.name }}_dirty (ui_{{ frame }}_dirty_string[{{ loop.index }}])
## endfor
#endif

void ui_init_{{ frame }}();
void ui_update_{{ frame }}();

#endif // UI_{{ frame }}_H
)";

#endif //UI_FRAME_H_DYNAMIC_HPP

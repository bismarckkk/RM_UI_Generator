//
// Created by abc55 on 2025/5/8.
//

#ifndef UI_FRAME_H_STATIC_HPP
#define UI_FRAME_H_STATIC_HPP

#include <string>

inline const std::string ui_frame_h_template_s = R"(//
// Created by RM UI Designer
// Static Edition
//

#ifndef UI_{{ frame }}_H
#define UI_{{ frame }}_H

#include "ui_interface.h"

## for group in groups
## for split in group.splits
## for obj in split.objs
extern {{ obj.type }} *{{ obj.name }};
## endfor
## endfor
## for split in group.textSplits
extern {{ split.obj.type }} *{{ split.obj.name }};
## endfor

void ui_init_{{ frame }}_{{ group.name }}();
void ui_update_{{ frame }}_{{ group.name }}();
void ui_remove_{{ frame }}_{{ group.name }}();

## endfor

#endif // UI_{{ frame }}_H
)";

#endif //UI_FRAME_H_STATIC_HPP

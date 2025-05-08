//
// Created by abc55 on 2025/5/8.
//

#ifndef UI_H_STATIC_HPP
#define UI_H_STATIC_HPP

#include <string>

inline const std::string ui_h_template_s = R"(//
// Created by RM UI Designer
// Static Edition
//

#ifndef UI_H
#define UI_H
#ifdef __cplusplus
extern "C" {
#endif

#include "ui_interface.h"

## for frame in frames
#include "ui_{{ frame.name }}.h"

## for group in frame.groups
void ui_init_{{ frame.name }}_{{ group }}();
void ui_update_{{ frame.name }}_{{ group }}();
void ui_remove_{{ frame.name }}_{{ group }}();
## endfor
## endfor

#ifdef __cplusplus
}
#endif

#endif // UI_H
)";

#endif //UI_H_STATIC_HPP

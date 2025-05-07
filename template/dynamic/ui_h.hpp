//
// Created by abc55 on 2025/5/7.
//

#ifndef UI_H_HPP
#define UI_H_HPP

#include <string>

inline const std::string ui_h_template = R"(//
// Created by RM UI Designer
// Dynamic Edition
//

#ifndef UI_H
#define UI_H
#ifdef __cplusplus
extern "C" {
#endif

#include "ui_interface.h"

## for frame in frames
#include "ui_{{frame}}.h"
## endfor

#ifdef __cplusplus
}
#endif

#endif // UI_H
)";

#endif //UI_H_HPP

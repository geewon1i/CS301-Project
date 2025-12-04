#ifndef MODE2_H
#define MODE2_H

#include "main.h"

// 外部可调用的入口（保留为阻塞或改为 step 风格）
void mode2_loop(void);
// 如果你想改为非阻塞：也可提供 mode2_step(void);

#endif // MODE2_H

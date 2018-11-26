#include "bios_dir.h"
#include "lcd_160x.h"

#if defined(CASE_ASCIIDISPLAY)
#if (DD_LCD_1601)
#include "lcd_160x1.c"
#elif (DD_ZIP)
#include "lcd_160x2.c"
#endif
#endif

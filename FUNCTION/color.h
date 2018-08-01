/*************************************************************************
	> Motto : Be better!
	> Author: ShadowsGtt 
	> Mail  : 1025814447@qq.com
	> Time  : 2018年08月01日 星期三 18时15分44秒
 ************************************************************************/

#ifndef _COLOR_H
#define _COLOR_H

#define RED          "\033[31m"         //红色
#define LRED         "\033[1;31m"       //亮红
#define GREEN        "\033[0;32;32m"    //绿色
#define LGREEN       "\033[1;32m"       //亮绿
#define BLUE         "\033[0;32;34m"    //蓝色
#define LBLUE        "\033[1;34m"       //亮蓝色
#define GRAY         "\033[1;30m"       //灰色
#define CYAN         "\033[0;36m"       //青色
#define LCYAN        "\033[1;36m"       //亮青色
#define PURPLE       "\033[0;35m"       //紫色
#define LPURPLE      "\033[1;35m"       //亮紫色
#define BROWN        "\033[0;33m"       //棕色
#define YELLOW       "\033[1;33m"       //黄色
#define LGRAY        "\033[0;37m"       //亮灰色
#define WHITE        "\033[1;37m"       //白色

#define PRINT(color,words)      \
    printf(                     \
    color                       \
    "%s"                        \
    "\033[0m",                  \
    words                       \
    )

static void test_color()
{
    PRINT(RED,"红色\n");
    PRINT(LRED,"亮红色\n");
    PRINT(GREEN,"绿色\n");
    PRINT(LGREEN,"亮绿色\n");
    PRINT(BLUE,"蓝色\n");
    PRINT(LBLUE,"亮蓝色\n");
    PRINT(GRAY,"灰色\n");
    PRINT(LGRAY,"亮灰色\n");
    PRINT(CYAN,"青色\n");
    PRINT(LCYAN,"亮青色\n");
    PRINT(PURPLE,"紫色\n");
    PRINT(LPURPLE,"亮紫色\n");
    PRINT(BROWN,"棕色\n");
    PRINT(YELLOW,"黄色\n");
    PRINT(WHITE,"白色\n");
}
#endif  //end 

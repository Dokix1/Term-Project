#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"

USING_NS_CC; //cocos2d命名空间

/* _tWinMain函数 ：Windows应用程序入口函数 替代标准main函数 */
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
    /* 消除编译警告 */
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    AppDelegate app; //创建AppDelegate对象 负责管理游戏的生命周期、平台和语言的设置
    return Application::getInstance()->run(); //运行应用程序 启动游戏的主循环
}
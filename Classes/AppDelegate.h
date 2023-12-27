#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

/* AppDelegate类 */
class AppDelegate : private cocos2d::Application { //cocos2d::Application的私有派生类
public:
    AppDelegate(); //构造函数
    virtual ~AppDelegate(); //析构函数

    virtual void initGLContextAttrs(); //初始化OpenGL属性
    virtual bool applicationDidFinishLaunching(); //初始化Director和场景

    virtual void applicationDidEnterBackground();  //程序进入后台时调用
    virtual void applicationWillEnterForeground(); //程序重新进入前台时调用
};

#endif 
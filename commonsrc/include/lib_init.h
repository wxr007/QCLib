
//description:在应用第三方库的时候，可能会需要做些必要的初始化；在程序退出的时候，需要做一些必要的清理工作;
//            这两个函数的功能，就是这个.

#pragma once


//这里的函数，一定要在系统启动的时候调用，否则会有问题.
namespace QCOOL
{
	void InitLib();
	void UninitLib();
}
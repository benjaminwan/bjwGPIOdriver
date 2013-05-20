更多详情请查看：http://www.arm9home.net/read.php?tid-27259.html

1.驱动本身没啥好说的，采用platform driver写法，属于练手之作，简单易懂吧，如果内核相关函数缺失，请自己下载GPIOKernelPatch并打好补丁。
1.1、驱动文件名bjwgpio.c，放到内核的drivers/gpio文件夹，支持的内核都使用这个驱动文件。
1.2、然后修改drivers/gpio文件夹里的Kconfig，在endif前增加(如果你patch或覆盖了如上补丁，那么此步就自动做好了，请跳过)。
config GPIO_BJW
	tristate "GPIO support for Tiny6410/Mini6410/Tiny210/Mini210"
	depends on CPU_S5PV210 || CPU_S3C6410
	default y
	help
	  This option enables support for All GPIO read/write/cfg/pull on Tiny6410/Mini6410/Tiny210/Mini210.
1.3、最后修改drivers/gpio文件夹里的Makefile，在最后增加一行(如果你patch或覆盖了如上补丁，那么此步就自动做好了，请跳过)
obj-$(CONFIG_GPIO_BJW)	+= bjwgpio.o

2、重新编译内核
先make menuconfig
然后确认如下选项是否已经选中
Device Drivers  --->
-*- GPIO Support  --->
<*>   GPIO support for Tiny6410/Mini6410/Tiny210/Mini210
最后保存，并编译。

3、修改设备打开读写权限
不修改设备打开权限的话，应用程序操作时会出错无法打开设备。
驱动设备只有一个，为/dev/bjw-gpio。
需要改rootfs里的init.rc，在“on init”节里找到友善自己修改权限的地方，并加上我们的语句，如下：
#{{ added by FriendlyARM
chmod 0666 /dev/bjw-gpio
然后重新制作ROM，连同上面编译好的内核，重新烧写。

4、应用程序和JNI
jni的源代码和apk的源代码一起，在jni文件夹里。
如果需要重新编译jni，请自行下载ndk。
本人使用的是android-ndk-r8b-windows编译的。
linux下的测试程序请参考jni来编写，这里就不提供了。


5、简便测试方法
不想用应用程序测试的话，可以在adb shell或调试串口中使用如下命令控制，可能会有出错提示但不影响。
Tiny/Mini6410点灯：
LED1:echo GPK 4 0 > /dev/bjw-gpio
LED2:echo GPK 5 0 > /dev/bjw-gpio
LED3:echo GPK 6 0 > /dev/bjw-gpio
LED4:echo GPK 7 0 > /dev/bjw-gpio

Tiny/Mini6410灭灯：
LED1:echo GPK 4 1 > /dev/bjw-gpio
LED2:echo GPK 5 1 > /dev/bjw-gpio
LED3:echo GPK 6 1 > /dev/bjw-gpio
LED4:echo GPK 7 1 > /dev/bjw-gpio

Tiny/Mini210点灯：
LED1:echo GPJ2 0 0 > /dev/bjw-gpio
LED2:echo GPJ2 1 0 > /dev/bjw-gpio
LED3:echo GPJ2 2 0 > /dev/bjw-gpio
LED4:echo GPJ2 3 0 > /dev/bjw-gpio

Tiny/Mini210灭灯：
LED1:echo GPJ2 0 1 > /dev/bjw-gpio
LED2:echo GPJ2 1 1 > /dev/bjw-gpio
LED3:echo GPJ2 2 1 > /dev/bjw-gpio
LED4:echo GPJ2 3 1 > /dev/bjw-gpio


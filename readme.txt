����������鿴��http://www.arm9home.net/read.php?tid-27259.html

1.��������ûɶ��˵�ģ�����platform driverд������������֮�������׶��ɣ�����ں���غ���ȱʧ�����Լ�����GPIOKernelPatch����ò�����
1.1�������ļ���bjwgpio.c���ŵ��ں˵�drivers/gpio�ļ��У�֧�ֵ��ں˶�ʹ����������ļ���
1.2��Ȼ���޸�drivers/gpio�ļ������Kconfig����endifǰ����(�����patch�򸲸������ϲ�������ô�˲����Զ������ˣ�������)��
config GPIO_BJW
	tristate "GPIO support for Tiny6410/Mini6410/Tiny210/Mini210"
	depends on CPU_S5PV210 || CPU_S3C6410
	default y
	help
	  This option enables support for All GPIO read/write/cfg/pull on Tiny6410/Mini6410/Tiny210/Mini210.
1.3������޸�drivers/gpio�ļ������Makefile�����������һ��(�����patch�򸲸������ϲ�������ô�˲����Զ������ˣ�������)
obj-$(CONFIG_GPIO_BJW)	+= bjwgpio.o

2�����±����ں�
��make menuconfig
Ȼ��ȷ������ѡ���Ƿ��Ѿ�ѡ��
Device Drivers  --->
-*- GPIO Support  --->
<*>   GPIO support for Tiny6410/Mini6410/Tiny210/Mini210
��󱣴棬�����롣

3���޸��豸�򿪶�дȨ��
���޸��豸��Ȩ�޵Ļ���Ӧ�ó������ʱ������޷����豸��
�����豸ֻ��һ����Ϊ/dev/bjw-gpio��
��Ҫ��rootfs���init.rc���ڡ�on init�������ҵ������Լ��޸�Ȩ�޵ĵط������������ǵ���䣬���£�
#{{ added by FriendlyARM
chmod 0666 /dev/bjw-gpio
Ȼ����������ROM����ͬ�������õ��ںˣ�������д��

4��Ӧ�ó����JNI
jni��Դ�����apk��Դ����һ����jni�ļ����
�����Ҫ���±���jni������������ndk��
����ʹ�õ���android-ndk-r8b-windows����ġ�
linux�µĲ��Գ�����ο�jni����д������Ͳ��ṩ�ˡ�


5�������Է���
������Ӧ�ó�����ԵĻ���������adb shell����Դ�����ʹ������������ƣ����ܻ��г�����ʾ����Ӱ�졣
Tiny/Mini6410��ƣ�
LED1:echo GPK 4 0 > /dev/bjw-gpio
LED2:echo GPK 5 0 > /dev/bjw-gpio
LED3:echo GPK 6 0 > /dev/bjw-gpio
LED4:echo GPK 7 0 > /dev/bjw-gpio

Tiny/Mini6410��ƣ�
LED1:echo GPK 4 1 > /dev/bjw-gpio
LED2:echo GPK 5 1 > /dev/bjw-gpio
LED3:echo GPK 6 1 > /dev/bjw-gpio
LED4:echo GPK 7 1 > /dev/bjw-gpio

Tiny/Mini210��ƣ�
LED1:echo GPJ2 0 0 > /dev/bjw-gpio
LED2:echo GPJ2 1 0 > /dev/bjw-gpio
LED3:echo GPJ2 2 0 > /dev/bjw-gpio
LED4:echo GPJ2 3 0 > /dev/bjw-gpio

Tiny/Mini210��ƣ�
LED1:echo GPJ2 0 1 > /dev/bjw-gpio
LED2:echo GPJ2 1 1 > /dev/bjw-gpio
LED3:echo GPJ2 2 1 > /dev/bjw-gpio
LED4:echo GPJ2 3 1 > /dev/bjw-gpio


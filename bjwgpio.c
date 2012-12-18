#include <asm/uaccess.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/device.h>
#include <linux/poll.h>
#include <linux/pci.h>
#include <linux/gpio.h>
#include <plat/gpio-core.h>
#include <plat/gpio-cfg.h>
#include <plat/gpio-cfg-helpers.h>

#define GPIO_MAGIC 'G'
#define GPIO_GET_DBGCTRL   0x40044700
#define GPIO_SET_DBGCTRL   0x40044701
#define GPIO_GET_CFGPIN   0x40044702
#define GPIO_SET_CFGPIN   0x40044703
#define GPIO_GET_PULL  0x40044704
#define GPIO_SET_PULL  0x40044705
#define GPIO_MAXNR 5
/*
#define GPIO_GET_DBGCTRL   _IOW(GPIO_MAGIC,0,int)
#define GPIO_SET_DBGCTRL   _IOW(GPIO_MAGIC,1,int)
#define GPIO_GET_CFGPIN   _IOW(GPIO_MAGIC,2,int)
#define GPIO_SET_CFGPIN   _IOW(GPIO_MAGIC,3,int)
#define GPIO_GET_PULL  _IOW(GPIO_MAGIC,4,int)
#define GPIO_SET_PULL  _IOW(GPIO_MAGIC,5,int)
*/

static int dbg_enable = 0;

#define PLOG(fmt, args...) \
	do { \
	if(dbg_enable) \
	printk(fmt, ##args); \
	} while (0)

struct gpio_unit {
	const char *gpio_group;
	unsigned int gpio_start;
	int gpio_nr;
};
#if defined(CONFIG_CPU_S3C6410)
static struct gpio_unit gpio_units[] = {
	[0]={"GPA",S3C64XX_GPA(0),S3C64XX_GPIO_A_NR},
	[1]={"GPB",S3C64XX_GPB(0),S3C64XX_GPIO_B_NR},
	[2]={"GPC",S3C64XX_GPC(0),S3C64XX_GPIO_C_NR},
	[3]={"GPD",S3C64XX_GPD(0),S3C64XX_GPIO_D_NR},
	[4]={"GPE",S3C64XX_GPE(0),S3C64XX_GPIO_E_NR},
	[5]={"GPF",S3C64XX_GPF(0),S3C64XX_GPIO_F_NR},
	[6]={"GPG",S3C64XX_GPG(0),S3C64XX_GPIO_G_NR},
	[7]={"GPH",S3C64XX_GPH(0),S3C64XX_GPIO_H_NR},
	[8]={"GPI",S3C64XX_GPI(0),S3C64XX_GPIO_I_NR},
	[9]={"GPJ",S3C64XX_GPJ(0),S3C64XX_GPIO_J_NR},
	[10]={"GPK",S3C64XX_GPK(0),S3C64XX_GPIO_K_NR},
	[11]={"GPL",S3C64XX_GPL(0),S3C64XX_GPIO_L_NR},
	[12]={"GPM",S3C64XX_GPM(0),S3C64XX_GPIO_M_NR},
	[13]={"GPN",S3C64XX_GPN(0),S3C64XX_GPIO_N_NR},
	[14]={"GPO",S3C64XX_GPO(0),S3C64XX_GPIO_O_NR},
	[15]={"GPP",S3C64XX_GPP(0),S3C64XX_GPIO_P_NR},
	[16]={"GPQ",S3C64XX_GPQ(0),S3C64XX_GPIO_Q_NR},
};
#elif defined(CONFIG_CPU_S5PV210)
static struct gpio_unit gpio_units[] = {
	[0]={"GPA0",S5PV210_GPA0(0),S5PV210_GPIO_A0_NR},
	[1]={"GPA1",S5PV210_GPA1(0),S5PV210_GPIO_A1_NR},
	[2]={"GPB",S5PV210_GPB(0),S5PV210_GPIO_B_NR},
	[3]={"GPC0",S5PV210_GPC0(0),S5PV210_GPIO_C0_NR},
	[4]={"GPC1",S5PV210_GPC1(0),S5PV210_GPIO_C1_NR},
	[5]={"GPD0",S5PV210_GPD0(0),S5PV210_GPIO_D0_NR},
	[6]={"GPD1",S5PV210_GPD1(0),S5PV210_GPIO_D1_NR},
	[7]={"GPE0",S5PV210_GPE0(0),S5PV210_GPIO_E0_NR},
	[8]={"GPE1",S5PV210_GPE1(0),S5PV210_GPIO_E1_NR},
	[9]={"GPF0",S5PV210_GPF0(0),S5PV210_GPIO_F0_NR},
	[10]={"GPF1",S5PV210_GPF1(0),S5PV210_GPIO_F1_NR},
	[11]={"GPF2",S5PV210_GPF2(0),S5PV210_GPIO_F2_NR},
	[12]={"GPF3",S5PV210_GPF3(0),S5PV210_GPIO_F3_NR},
	[13]={"GPG0",S5PV210_GPG0(0),S5PV210_GPIO_G0_NR},
	[14]={"GPG1",S5PV210_GPG1(0),S5PV210_GPIO_G1_NR},
	[15]={"GPG2",S5PV210_GPG2(0),S5PV210_GPIO_G2_NR},
	[16]={"GPG3",S5PV210_GPG3(0),S5PV210_GPIO_G3_NR},
	[17]={"GPH0",S5PV210_GPH0(0),S5PV210_GPIO_H0_NR},
	[18]={"GPH1",S5PV210_GPH1(0),S5PV210_GPIO_H1_NR},
	[19]={"GPH2",S5PV210_GPH2(0),S5PV210_GPIO_H2_NR},
	[20]={"GPH3",S5PV210_GPH3(0),S5PV210_GPIO_H3_NR},
	[21]={"GPI",S5PV210_GPI(0),S5PV210_GPIO_I_NR},
	[22]={"GPJ0",S5PV210_GPJ0(0),S5PV210_GPIO_J0_NR},
	[23]={"GPJ1",S5PV210_GPJ1(0),S5PV210_GPIO_J1_NR},
	[24]={"GPJ2",S5PV210_GPJ2(0),S5PV210_GPIO_J2_NR},
	[25]={"GPJ3",S5PV210_GPJ3(0),S5PV210_GPIO_J3_NR},
	[26]={"GPJ4",S5PV210_GPJ4(0),S5PV210_GPIO_J4_NR},
	[27]={"MP01",S5PV210_MP01(0),S5PV210_GPIO_MP01_NR},
	[28]={"MP02",S5PV210_MP02(0),S5PV210_GPIO_MP02_NR},
	[29]={"MP03",S5PV210_MP03(0),S5PV210_GPIO_MP03_NR},
	[30]={"MP04",S5PV210_MP04(0),S5PV210_GPIO_MP04_NR},
	[31]={"MP05",S5PV210_MP05(0),S5PV210_GPIO_MP05_NR},
	[32]={"MP06",S5PV210_MP06(0),S5PV210_GPIO_MP06_NR},
	[33]={"MP07",S5PV210_MP07(0),S5PV210_GPIO_MP07_NR},
	[34]={"MP10",S5PV210_MP10(0),S5PV210_GPIO_MP10_NR},
	[35]={"MP11",S5PV210_MP11(0),S5PV210_GPIO_MP11_NR},
	[36]={"MP12",S5PV210_MP12(0),S5PV210_GPIO_MP12_NR},
	[37]={"MP13",S5PV210_MP13(0),S5PV210_GPIO_MP13_NR},
	[38]={"MP14",S5PV210_MP14(0),S5PV210_GPIO_MP14_NR},
	[39]={"MP15",S5PV210_MP15(0),S5PV210_GPIO_MP15_NR},
	[40]={"MP16",S5PV210_MP16(0),S5PV210_GPIO_MP16_NR},
	[41]={"MP17",S5PV210_MP17(0),S5PV210_GPIO_MP17_NR},
	[42]={"MP18",S5PV210_MP18(0),S5PV210_GPIO_MP18_NR},
	[43]={"MP20",S5PV210_MP20(0),S5PV210_GPIO_MP20_NR},
	[44]={"MP21",S5PV210_MP21(0),S5PV210_GPIO_MP21_NR},
	[45]={"MP22",S5PV210_MP22(0),S5PV210_GPIO_MP22_NR},
	[46]={"MP23",S5PV210_MP23(0),S5PV210_GPIO_MP23_NR},
	[47]={"MP24",S5PV210_MP24(0),S5PV210_GPIO_MP24_NR},
	[48]={"MP25",S5PV210_MP25(0),S5PV210_GPIO_MP25_NR},
	[49]={"MP26",S5PV210_MP26(0),S5PV210_GPIO_MP26_NR},
	[50]={"MP27",S5PV210_MP27(0),S5PV210_GPIO_MP27_NR},
	[51]={"MP28",S5PV210_MP28(0),S5PV210_GPIO_MP28_NR},
	[52]={"ETC0",S5PV210_ETC0(0),S5PV210_GPIO_ETC0_NR},
	[53]={"ETC1",S5PV210_ETC1(0),S5PV210_GPIO_ETC1_NR},
	[54]={"ETC2",S5PV210_ETC2(0),S5PV210_GPIO_ETC2_NR},
	[55]={"ETC4",S5PV210_ETC4(0),S5PV210_GPIO_ETC4_NR},
};
#endif


static int bjw_gpio_get_pin_io(char* pin_group,int pin_num)
{
	int pin_io = -EINVAL;
	int i;
	
	for(i=0; i<ARRAY_SIZE(gpio_units); i++)
	{
		if(strcmp(gpio_units[i].gpio_group, pin_group)==0)
		{
			if(pin_num<(gpio_units[i].gpio_nr))
			{
				pin_io = gpio_units[i].gpio_start + pin_num;
			}
			break;
		}
	}
	if(pin_io<0)
		PLOG("Can't find gpio: %s(%d) !\n", pin_group, pin_num);
	else
		PLOG("Find gpio: %s(%d), pin_io = %d\n", pin_group, pin_num, pin_io);
	return pin_io;
}

static int bjw_gpio_read(struct file *filp,char __user *buf_user,size_t size,loff_t *offset)
{
	char cmd[16];
	char pin_group[16];
	int ret,pin_num,pin_io,val;
	ret=copy_from_user(cmd,buf_user,size);
	if(ret!=0)
		return -EINVAL;
	sscanf(cmd, "%s %d", pin_group, &pin_num);
	PLOG("cmd:get %s(%d) value\n", pin_group, pin_num);
	pin_io=bjw_gpio_get_pin_io(pin_group,pin_num);
	if(pin_io<0)
		return pin_io;
	gpio_request(pin_io, pin_group);
	val = gpio_get_value(pin_io);
	gpio_free(pin_io);
	PLOG("Get %s(%d) value = %d\r\n",pin_group,pin_num,val);
	return val;
}

static int bjw_gpio_write(struct file *filp,const char __user *buf_user,size_t size,loff_t *offset)
{
	char cmd[16];
	char pin_group[16];
	int ret,pin_num,pin_io,val;
	ret=copy_from_user(cmd,buf_user,size);
	if(ret!=0)
		return -EINVAL;
	sscanf(cmd, "%s %d %d", pin_group, &pin_num,&val);
	PLOG("cmd: set %s(%d) value %d\n", pin_group, pin_num,val);
	pin_io=bjw_gpio_get_pin_io(pin_group,pin_num);
	if(pin_io<0)
		return pin_io;
	gpio_request(pin_io, pin_group);
	gpio_set_value(pin_io,val);
	gpio_free(pin_io);
	PLOG("Set %s(%d) value = %d\n", pin_group, pin_num, val);
	return pin_io;
} 
 
struct gpio_pin_data {
char* pin_group;
int pin_num;
int pin_data;
};

struct gpio_info {
char* pin_group;
int pin_num;
};

static long bjw_gpio_ioctl(struct file *filp,unsigned int cmd,unsigned long arg)
{
	int pin_io;
	unsigned ret;
	s3c_gpio_pull_t ret_pull;
//	PLOG("GPIO_GET_DBGCTRL  = %x,GPIO_SET_DBGCTRL=%x\n",GPIO_GET_DBGCTRL,GPIO_SET_DBGCTRL);
//	PLOG("GPIO_GET_CFGPIN  = %x,GPIO_SET_CFGPIN=%x\n",GPIO_GET_CFGPIN,GPIO_SET_CFGPIN);
//	PLOG("GPIO_GET_PULL  = %x,GPIO_SET_PULL=%x\n",GPIO_GET_PULL,GPIO_SET_PULL);
	if(_IOC_TYPE(cmd)!=GPIO_MAGIC) return -EINVAL;
	if(_IOC_NR(cmd)>GPIO_MAXNR || _IOC_NR(cmd)<0)
		return -EINVAL;
	switch (cmd) {
	case GPIO_GET_DBGCTRL:
		PLOG("GPIO_GET_DBGCTRL \r\n");
		return dbg_enable;
	case GPIO_SET_DBGCTRL:
		PLOG("GPIO_SET_DBGCTRL \r\n");
		dbg_enable=arg;
		PLOG("set dbg_enable = %lu\n", arg);
	break;
	case GPIO_GET_CFGPIN:
	{
		struct gpio_info pin_info;
		PLOG("GPIO_GET_CFGPIN \r\n");
		if(copy_from_user(&pin_info,(const char*)arg,sizeof(pin_info)) !=0)
			return -EINVAL;
		PLOG("cmd:Get %s(%d) cfgpin \r\n",pin_info.pin_group,pin_info.pin_num);
		pin_io=bjw_gpio_get_pin_io(pin_info.pin_group,pin_info.pin_num);
		if(pin_io<0)
			return pin_io;
		gpio_request(pin_io, pin_info.pin_group);
		ret=s3c_gpio_getcfg(pin_io);
		ret=ret & 0xf;
		PLOG("get pin_cfg:%d \r\n",ret);
		gpio_free(pin_io);
		return ret;
	}
	case GPIO_SET_CFGPIN:
	{
		struct gpio_pin_data cfg_data;
		PLOG("GPIO_SET_CFGPIN \r\n");
		if(copy_from_user(&cfg_data,(const char*)arg,sizeof(cfg_data)) !=0)
			return -EINVAL;
		PLOG("cmd:Set %s(%d) cfgpin:%d \r\n",cfg_data.pin_group,cfg_data.pin_num,cfg_data.pin_data);
		pin_io=bjw_gpio_get_pin_io(cfg_data.pin_group,cfg_data.pin_num);
		if(pin_io<0)
			return pin_io;
		gpio_request(pin_io, cfg_data.pin_group);
		s3c_gpio_cfgpin(pin_io,S3C_GPIO_SFN(cfg_data.pin_data));
		gpio_free(pin_io);
	}
	break;
	case GPIO_GET_PULL:
	{
		struct gpio_info pin_info;
		PLOG("GPIO_GET_PULL \r\n");
		if(copy_from_user(&pin_info,(const char*)arg,sizeof(pin_info)) !=0)
			return -EINVAL;
		PLOG("cmd:Get %s(%d) pull \r\n",pin_info.pin_group,pin_info.pin_num);
		pin_io=bjw_gpio_get_pin_io(pin_info.pin_group,pin_info.pin_num);
		if(pin_io<0)
			return pin_io;
		gpio_request(pin_io, pin_info.pin_group);
		ret_pull=s3c_gpio_getpull(pin_io);
		gpio_free(pin_io);
		PLOG("get pin_pull:%d \r\n",ret_pull);
		return ret_pull;
	}

	case GPIO_SET_PULL:
	{

		struct gpio_pin_data pull_data;
		PLOG("GPIO_SET_PULL \r\n");
		if(copy_from_user(&pull_data,(const char*)arg,sizeof(pull_data)) !=0)
			return -EINVAL;
		PLOG("cmd:Set %s(%d) pull:%d \r\n",pull_data.pin_group,pull_data.pin_num,pull_data.pin_data);
		pin_io=bjw_gpio_get_pin_io(pull_data.pin_group,pull_data.pin_num);
		if(pin_io<0)
			return pin_io;
		gpio_request(pin_io, pull_data.pin_group);
		switch (pull_data.pin_data) {
		case 0:
			s3c_gpio_setpull(pin_io,S3C_GPIO_PULL_NONE);
		break;
		case 1:
			s3c_gpio_setpull(pin_io,S3C_GPIO_PULL_DOWN);
		break;
		case 2:
			s3c_gpio_setpull(pin_io,S3C_GPIO_PULL_UP);
		break;
		default:
			PLOG("Can't Set %s(%d) pull:%d \r\n",pull_data.pin_group,pull_data.pin_num,pull_data.pin_data);
		break;
		}
		gpio_free(pin_io);

	}
	break;
	default:
		PLOG("Can't Find cmd:%x\n", cmd);
	break;
	}	
	return 0;
}

static struct file_operations dev_fops = {
    .owner   =   THIS_MODULE,
//    .ioctl= bjw_gpio_ioctl,if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 35)
    .unlocked_ioctl= bjw_gpio_ioctl,
    .read    =   bjw_gpio_read,
    .write   =   bjw_gpio_write,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "bjw-gpio",
	.fops = &dev_fops,
};

static int bjw_gpio_probe(struct platform_device *dev)
{
	int ret;
	PLOG("bjw platform dirver find bjw platfrom device.\n");
	ret = misc_register(&misc);
	if(ret)
		return ret;
	return 0;
}

static int bjw_gpio_remove(struct platform_device *pdev)
{
	PLOG("bjw GPIO platfrom device has removed.\n");
	misc_deregister(&misc);
	return 0;
}

static struct platform_device  bjw_gpio_device= {
        .name = "bjw-gpio",
        .id   = -1,
};

static struct platform_driver bjw_gpio_driver = {
	.probe	= bjw_gpio_probe,
	.remove	= bjw_gpio_remove,
	.driver	= {
		.name	= "bjw-gpio",
		.owner = THIS_MODULE,
	},
};

int __init bjw_gpio_init(void)
{
        platform_device_register(&bjw_gpio_device);
        platform_driver_register(&bjw_gpio_driver);
        return 0;
}
void __exit bjw_gpio_exit(void)
{
	platform_driver_unregister(&bjw_gpio_driver);
	platform_device_unregister(&bjw_gpio_device); 
}

module_init(bjw_gpio_init);
module_exit(bjw_gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Benjaminwan");
MODULE_DESCRIPTION("GPIO for Samsung s3c6410|s5pv210");
MODULE_VERSION("0.1");

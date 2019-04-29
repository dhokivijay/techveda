#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x15b2dc7b, "module_layout" },
	{ 0xc87a6c36, "unregister_netdev" },
	{ 0x142754df, "free_netdev" },
	{ 0x73bbe45a, "register_netdev" },
	{ 0x835d5084, "alloc_netdev_mqs" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0xfb2b9f6b, "consume_skb" },
	{ 0x1dd697a4, "ether_setup" },
	{ 0x50eedeb8, "printk" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "81497D8D815C237C7254823");

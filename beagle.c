/*
 * (C) Copyright 2004-2008
 * Texas Instruments, <www.ti.com>
 *
 * Author :
 *	Sunil Kumar <sunilsaini05@gmail.com>
 *	Shashi Ranjan <shashiranjanmca05@gmail.com>
 *
 * Derived from Beagle Board and 3430 SDP code by
 *	Richard Woodruff <r-woodruff2@ti.com>
 *	Syed Mohammed Khasim <khasim@ti.com>
 *
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <common.h>
#include <twl4030.h>
#include <asm/io.h>
#include <asm/arch/mux.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/gpio.h>
#include <asm/mach-types.h>
#include "beagle.h"
#include "logo.h"

static int beagle_revision;

/*
 * Routine: board_init
 * Description: Early hardware init.
 */
int board_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	gpmc_init(); /* in SRAM or SDRAM, finish GPMC */
	/* board id for Linux */
	gd->bd->bi_arch_number = MACH_TYPE_OMAP3_BEAGLE;
	/* boot param addr */
	gd->bd->bi_boot_params = (OMAP34XX_SDRC_CS0 + 0x100);

	return 0;
}

/*
 * Routine: beagle_get_revision
 * Description: Return the revision of the BeagleBoard this code is running on.
 */
int beagle_get_revision(void)
{
	return beagle_revision;
}

/*
 * Routine: beagle_identify
 * Description: Detect if we are running on a Beagle revision Ax/Bx,
 *		C1/2/3, C4 or D. This can be done by reading
 *		the level of GPIO173, GPIO172 and GPIO171. This should
 *		result in
 *		GPIO173, GPIO172, GPIO171: 1 1 1 => Ax/Bx
 *		GPIO173, GPIO172, GPIO171: 1 1 0 => C1/2/3
 *		GPIO173, GPIO172, GPIO171: 1 0 1 => C4
 *		GPIO173, GPIO172, GPIO171: 0 0 0 => D
 */
void beagle_identify(void)
{
	omap_request_gpio(171);
	omap_request_gpio(172);
	omap_request_gpio(173);
	omap_set_gpio_direction(171, 1);
	omap_set_gpio_direction(172, 1);
	omap_set_gpio_direction(173, 1);

	beagle_revision = omap_get_gpio_datain(173) << 2 |
			  omap_get_gpio_datain(172) << 1 |
			  omap_get_gpio_datain(171);
	omap_free_gpio(171);
	omap_free_gpio(172);
	omap_free_gpio(173);

	printf("Board revision ");

	switch (beagle_revision) {
	case REVISION_AXBX:
		printf("Ax/Bx\n");
		break;
	case REVISION_CX:
		printf("C1/C2/C3\n");
		break;
	case REVISION_C4:
		printf("C4\n");
		break;
	case REVISION_D:
		printf("D\n");
		break;
	default:
		printf("unknown 0x%02x\n", beagle_revision);
	}
}

/*
 * Configure DSS to display background color on DVID
 * Configure VENC to display color bar on S-Video
 */
void display_init(void)
{
	omap3_dss_venc_config(&venc_config_std_tv);
	omap3_dss_panel_config(&dvid_cfg);
	omap3_dss_set_background_col(DVI_BEAGLE_ORANGE_COL);
}

void dss_init(void)
{
	unsigned long i, y;
	unsigned long p = 0;
	unsigned char pixel[3];

	/* assuming a resolution of 1280x720 - draw logo into dss buffer */

	/* Add the image data */
	for (i = 0; i < header_data_rle_size; i++) {
		for (y = 0; y < header_data_rle[i][0]; y++) {
			HEADER_PIXEL(header_data_rle[i][1], pixel);
			*((unsigned short *)(0x80500000 + p*2)) =
				((((pixel[0])&0xf8) << 8) |
				(((pixel[1])&0xfc) << 3) |
				(((pixel[2])&0xf8) >> 3));
			p++;
		}
	}

	writel(0xfefffedf, 0x48310034); /* GPIO_OE(1) */
	writel(0x01000120, 0x48310094); /* GPIO_SETDATAOUT(1) */
	writel(0x0001b00c, 0x48004D44); /* CM_CLKSEL2_PLL */
	writel(0x00001006, 0x48004E40);
	writel(0x00370037, 0x48004D00);
	writel(0x00000002, 0x48050C00);
	writel(0x0000001B, 0x48050C04);
	writel(0x00000040, 0x48050C08);
	writel(0x00000000, 0x48050C0C);
	writel(0x00000000, 0x48050C10);
	writel(0x00008000, 0x48050C14);
	writel(0x00000000, 0x48050C18);
	writel(0x00008359, 0x48050C1C);
	writel(0x0000020C, 0x48050C20);
	writel(0x00000000, 0x48050C24);
	writel(0x043F2631, 0x48050C28);
	writel(0x00000024, 0x48050C2C);
	writel(0x00000130, 0x48050C30);
	writel(0x00000198, 0x48050C34);
	writel(0x000001C0, 0x48050C38);
	writel(0x0000006A, 0x48050C3C);
	writel(0x0000005C, 0x48050C40);
	writel(0x00000000, 0x48050C44);
	writel(0x00000001, 0x48050C48);
	writel(0x0000003F, 0x48050C4C);
	writel(0x21F07C1F, 0x48050C50);
	writel(0x00000000, 0x48050C54);
	writel(0x00000015, 0x48050C58);
	writel(0x00001400, 0x48050C5C);
	writel(0x00000000, 0x48050C60);
	writel(0x069300F4, 0x48050C64);
	writel(0x0016020C, 0x48050C68);
	writel(0x00060107, 0x48050C6C);
	writel(0x008D034E, 0x48050C70);
	writel(0x000F0359, 0x48050C74);
	writel(0x01A00000, 0x48050C78);
	writel(0x020501A0, 0x48050C7C);
	writel(0x01AC0024, 0x48050C80);
	writel(0x020D01AC, 0x48050C84);
	writel(0x00000006, 0x48050C88);
	writel(0x00000000, 0x48050C8C);
	writel(0x03480079, 0x48050C90);
	writel(0x02040024, 0x48050C94);
	writel(0x00000000, 0x48050C98);
	writel(0x00000000, 0x48050C9C);
	writel(0x0001008A, 0x48050CA0);
	writel(0x01AC0106, 0x48050CA4);
	writel(0x01060006, 0x48050CA8);
	writel(0x00000000, 0x48050CAC);
	writel(0x00140001, 0x48050CB0);
	writel(0x00010001, 0x48050CB4);
	writel(0x00FF0000, 0x48050CB8);
	writel(0x00000000, 0x48050CBC);
	writel(0x00000000, 0x48050CC0);
	writel(0x0000000D, 0x48050CC4);
	writel(0x00000000, 0x48050CC8);
	writel(0x00000001, 0x48050010);
	writel(0x00000078, 0x48050040);
	writel(0x00000000, 0x48050044);
	writel(0x00000000, 0x48050048);
	writel(0x00000000, 0x48050050);
	writel(0x00000000, 0x48050058);
	writel(0x00002015, 0x48050410);
	writel(0x00000001, 0x48050414);
	writel(0x00000004, 0x48050444);
	writel(0xFFFFFFFF, 0x4805044c);
	writel(0x00000000, 0x48050450);
	writel(0x00000000, 0x48050454);
	writel(0x00000000, 0x48050458);
	writel(0x0ff03f31, 0x48050464);
	writel(0x01400504, 0x48050468);
	writel(0x00007028, 0x4805046c);
	writel(0x00010002, 0x48050470);
	writel(0x00ef027f, 0x48050478);
	writel(0x02cf04ff, 0x4805047c);
	writel(0x80500000, 0x48050480);
	writel(0x80500000, 0x48050484);
	writel(0x00000000, 0x48050488);
	writel(0x02cf04ff, 0x4805048c);
	writel(0x0000008d, 0x480504a0);
	writel(0x03fc03bc, 0x480504a4);
	writel(0x00000400, 0x480504a8);
	writel(0x00000001, 0x480504ac);
	writel(0x00000001, 0x480504b0);
	writel(0x00000000, 0x480504b4);
	writel(0x807ff000, 0x480504b8);
	udelay(1000);
	writel(0x0001836b, 0x48050440);
	udelay(1000);
	writel(0x0001836b, 0x48050440);
	udelay(1000);
	writel(0x0001836b, 0x48050440);
	udelay(1000);
}
 
/*
 * Routine: misc_init_r
 * Description: Configure board specific parts
 */
int misc_init_r(void)
{
	struct gpio *gpio5_base = (struct gpio *)OMAP34XX_GPIO5_BASE;
	struct gpio *gpio6_base = (struct gpio *)OMAP34XX_GPIO6_BASE;

	display_init();
	beagle_identify();
	dss_init();

	twl4030_power_init();
	twl4030_led_init();

	if (beagle_revision == REVISION_C4) {

		/* Select TWL4030 VSEL to support 720Mhz */
		twl4030_pmrecv_vsel_cfg(TWL4030_PM_RECEIVER_VAUX2_DEDICATED,
					VAUX2_VSEL_18,
					TWL4030_PM_RECEIVER_VAUX2_DEV_GRP,
					DEV_GRP_P1);

		twl4030_pmrecv_vsel_cfg(TWL4030_PM_RECEIVER_VDD1_VSEL,
					VDD1_VSEL_14,
					TWL4030_PM_RECEIVER_VDD1_DEV_GRP,
					DEV_GRP_P1);

		prcm_config_720mhz();
	}

	/* Configure GPIOs to output */
	writel(~(GPIO23 | GPIO10 | GPIO8 | GPIO2 | GPIO1), &gpio6_base->oe);
	writel(~(GPIO31 | GPIO30 | GPIO29 | GPIO28 | GPIO22 | GPIO21 |
		GPIO15 | GPIO14 | GPIO13 | GPIO12), &gpio5_base->oe);

	/* Set GPIOs */
	writel(GPIO23 | GPIO10 | GPIO8 | GPIO2 | GPIO1,
		&gpio6_base->setdataout);
	writel(GPIO31 | GPIO30 | GPIO29 | GPIO28 | GPIO22 | GPIO21 |
		GPIO15 | GPIO14 | GPIO13 | GPIO12, &gpio5_base->setdataout);

	omap3_dss_enable();
	dieid_num_r();

	return 0;
}

/*
 * Routine: set_muxconf_regs
 * Description: Setting up the configuration Mux registers specific to the
 *		hardware. Many pins need to be moved from protect to primary
 *		mode.
 */
void set_muxconf_regs(void)
{
	MUX_BEAGLE();

	if (beagle_revision != REVISION_AXBX)
		MUX_BEAGLE_C();
}

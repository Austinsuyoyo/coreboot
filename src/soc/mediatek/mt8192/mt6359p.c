/* SPDX-License-Identifier: GPL-2.0-only */

#include <assert.h>
#include <console/console.h>
#include <delay.h>
#include <soc/pmif.h>
#include <soc/mt6359p.h>

static const struct pmic_setting init_setting[] = {
	{0x20, 0xA, 0xA, 0},
	{0x24, 0x1F00, 0x1F00, 0},
	{0x30, 0x1, 0x1, 0},
	{0x32, 0x1, 0x1, 0},
	{0x94, 0x0, 0xFFFF, 0},
	{0x10C, 0x10, 0x10, 0},
	{0x112, 0x4, 0x4, 0},
	{0x118, 0x8, 0x8, 0},
	{0x14A, 0x20, 0x20, 0},
	{0x198, 0x0, 0x1FF, 0},
	{0x1B2, 0x3, 0x3, 0},
	{0x3B0, 0x0, 0x300, 0},
	{0x790, 0x3, 0x3, 0},
	{0x7A6, 0xF800, 0xFC00, 0},
	{0x7A8, 0x0, 0x280, 0},
	{0x98A, 0x80, 0x80, 0},
	{0x992, 0xF00, 0xF00, 0},
	{0xA08, 0x1, 0x1, 0},
	{0xA0C, 0x300, 0x300, 0},
	{0xA10, 0x0, 0x4000, 0},
	/*
	 * RG_STRUP_LONG_PRESS_EXT_SEL
	 * only allow pwrkey boot after long-press shutdown.
	 */
	{0xA12, 0x1F2, 0x1F3, 0},
	{0xA24, 0xFFFF, 0xFFFF, 0},
	{0xA26, 0xFFE0, 0xFFE0, 0},
	{0xA2C, 0xC0DF, 0xC0DF, 0},
	{0xA2E, 0xEBE0, 0xEBE0, 0},
	{0xA34, 0x8000, 0x8000, 0},
	{0xA3C, 0x1C00, 0x1F00, 0},
	{0xA3E, 0x341, 0x7FFF, 0},
	{0xA40, 0x1042, 0x7FFF, 0},
	{0xA42, 0xC05, 0x7FFF, 0},
	{0xA44, 0x20E5, 0x7FFF, 0},
	{0xA46, 0x2652, 0x7FFF, 0},
	{0xA48, 0x716A, 0x7FFF, 0},
	{0xA4A, 0x62EC, 0x7FFF, 0},
	{0xA4C, 0x5676, 0x7FFF, 0},
	{0xA4E, 0x6F34, 0x7FFF, 0},
	{0xA50, 0xC0, 0x7FFF, 0},
	{0xA9C, 0x4000, 0x4000, 0},
	{0xA9E, 0x2E11, 0xFF11, 0},
	{0xF8C, 0x115, 0x115, 0},
	{0x1188, 0x0, 0x8000, 0},
	{0x1198, 0x13, 0x3FF, 0},
	{0x119E, 0x6000, 0x7000, 0},
	{0x11D4, 0x0, 0x2, 0},
	{0x1212, 0x0, 0x2, 0},
	{0x1224, 0x0, 0x2, 0},
	{0x1238, 0x0, 0x2, 0},
	{0x124A, 0x0, 0x2, 0},
	{0x125C, 0x0, 0x2, 0},
	{0x125E, 0x0, 0x8000, 0},
	{0x1260, 0x1, 0xFFF, 0},
	{0x1262, 0x4, 0x4, 0},
	{0x1412, 0x8, 0x8, 0},
	{0x148E, 0x18, 0x7F, 0},
	{0x1492, 0x505, 0x7F7F, 0},
	{0x1514, 0x2, 0x2, 0},
	{0x1520, 0x0, 0x2, 0},
	{0x152C, 0x7F, 0x7F, 0},
	{0x158E, 0x18, 0x7F, 0},
	{0x1592, 0xC00, 0x7F00, 0},
	{0x160E, 0x18, 0x7F, 0},
	{0x1694, 0x2, 0x2, 0},
	{0x16A0, 0x0, 0x2, 0},
	{0x16AE, 0x50, 0x7F, 0},
	{0x170E, 0x18, 0x7F, 0},
	{0x178E, 0x18, 0x7F, 0},
	{0x198A, 0x5004, 0x502C, 0},
	{0x198C, 0x11, 0x3F, 0},
	{0x198E, 0x1E0, 0x1E0, 0},
	{0x1990, 0xFD, 0xFF, 0},
	{0x1994, 0x10, 0x38, 0},
	{0x1996, 0x2004, 0xA02C, 0},
	{0x1998, 0x11, 0x3F, 0},
	{0x199A, 0xFB78, 0xFF78, 0},
	{0x199E, 0x2, 0x7, 0},
	{0x19A0, 0x1050, 0x10F1, 0},
	{0x19A2, 0x18, 0x38, 0},
	{0x19A4, 0xF, 0xF, 0},
	{0x19A6, 0x30, 0xFF, 0},
	{0x19AC, 0x4200, 0x4680, 0},
	{0x19AE, 0x6E, 0x7E, 0},
	{0x19B0, 0x3C00, 0x3C00, 0},
	{0x19B4, 0x20FD, 0xFFFF, 0},
	{0x19DE, 0x1, 0x1, 6},		/* RG_VGPUVCORE_PH2_OFF, disable phase 2 */
	{0x1A08, 0x4200, 0x4680, 0},
	{0x1A0A, 0x6E, 0x7E, 0},
	{0x1A0C, 0x3C00, 0x3C00, 0},
	{0x1A10, 0x20FD, 0xFFFF, 0},
	{0x1A14, 0x4208, 0x4698, 0},
	{0x1A16, 0x46, 0x7E, 0},
	{0x1A18, 0x3C00, 0x3C00, 0},
	{0x1A1C, 0x30FF, 0xFFFF, 0},
	{0x1A1E, 0x0, 0x200, 0},
	{0x1A20, 0x4208, 0x4698, 0},
	{0x1A22, 0x4A, 0x7E, 0},
	{0x1A24, 0x3C00, 0x3C00, 0},
	{0x1A28, 0x3000, 0xFF00, 0},
	{0x1A2C, 0x20, 0x74, 0},
	{0x1A2E, 0x1E, 0x1E, 0},
	{0x1A30, 0x42, 0xFF, 0},
	{0x1A32, 0x480, 0x7E0, 0},
	{0x1A34, 0x20, 0x74, 0},
	{0x1A36, 0x1E, 0x1E, 0},
	{0x1A38, 0x42, 0xFF, 0},
	{0x1A3A, 0x480, 0x7E0, 0},
	{0x1A3C, 0x14C, 0x3CC, 0},
	{0x1A3E, 0x23C, 0x3FC, 0},
	{0x1A40, 0xC400, 0xFF00, 0},
	{0x1A42, 0x80, 0xFF, 0},
	{0x1A44, 0x702C, 0xFF2C, 0},
	{0x1B0E, 0xF, 0xF, 0},
	{0x1B10, 0x1, 0x1, 0},
	{0x1B14, 0xFFFF, 0xFFFF, 0},
	{0x1B1A, 0x3FFF, 0x3FFF, 0},
	{0x1B32, 0x8, 0x8, 0},
	{0x1B8A, 0x30, 0x8030, 0},
	{0x1B9C, 0x10, 0x8010, 0},
	{0x1BA0, 0x4000, 0x4000, 0},
	{0x1BAE, 0x1410, 0x9C10, 0},
	{0x1BB2, 0x2, 0x2, 0},
	{0x1BC0, 0x10, 0x8010, 0},
	{0x1BD2, 0x13, 0x8013, 0},
	{0x1BE4, 0x10, 0x8010, 0},
	{0x1C0A, 0x10, 0x8010, 0},
	{0x1C1E, 0x10, 0x8010, 0},
	{0x1C30, 0x10, 0x8010, 0},
	{0x1C42, 0x10, 0x8010, 0},
	{0x1C54, 0x32, 0x8033, 0},
	{0x1C66, 0x10, 0x8010, 0},
	{0x1C8A, 0x10, 0x8010, 0},
	{0x1C8E, 0x4000, 0x4000, 0},
	{0x1C9C, 0x10, 0x8010, 0},
	{0x1CAE, 0x10, 0x8010, 0},
	{0x1CC0, 0x10, 0x8010, 0},
	{0x1CD2, 0x33, 0x8033, 0},
	{0x1CE4, 0x33, 0x8033, 0},
	{0x1D0A, 0x10, 0x8010, 0},
	{0x1D1E, 0x10, 0x8010, 0},
	{0x1D22, 0x4000, 0x4000, 0},
	{0x1D30, 0x10, 0x8010, 0},
	{0x1D34, 0x4000, 0x4000, 0},
	{0x1D42, 0x30, 0x8030, 0},
	{0x1D46, 0x4000, 0x4000, 0},
	{0x1D54, 0x30, 0x8030, 0},
	{0x1D66, 0x32, 0x8033, 0},
	{0x1D8A, 0x10, 0x8010, 0},
	{0x1D9C, 0x10, 0x8010, 0},
	{0x1E8A, 0x10, 0x8010, 0},
	{0x1E92, 0xC12, 0x7F7F, 0},
	{0x1EAA, 0x10, 0x8010, 0},
	{0x1EB2, 0xC1F, 0x7F7F, 0},
	{0x1F0A, 0x10, 0x8010, 0},
	{0x1F12, 0xC1F, 0x7F7F, 0},
	{0x1F30, 0x10, 0x8010, 0},
	{0x1F38, 0xF1F, 0x7F7F, 0},
	{0x200A, 0x8, 0xC, 0},
	{0x202C, 0x8, 0xC, 0},
	{0x208C, 0x100, 0xF00, 0},
	{0x209C, 0x80, 0x1E0, 0},
};

static const struct pmic_setting lp_setting[] = {
	/* Suspend */
	{0x1520, 0x0, 0x1, 0x1},
	{0x1514, 0x1, 0x1, 0x1},
	{0x151a, 0x0, 0x1, 0x1},
	{0x14a0, 0x1, 0x1, 0x1},
	{0x1494, 0x1, 0x1, 0x1},
	{0x149a, 0x1, 0x1, 0x1},
	{0x1714, 0x1, 0x1, 0xf},
	{0x1794, 0x1, 0x1, 0xf},
	{0x15a0, 0x1, 0x1, 0x0},
	{0x1594, 0x1, 0x1, 0x0},
	{0x159a, 0x1, 0x1, 0x0},
	{0x1614, 0x1, 0x1, 0xf},
	{0x16a0, 0x0, 0x1, 0x1},
	{0x1694, 0x1, 0x1, 0x1},
	{0x169a, 0x0, 0x1, 0x1},
	{0x1820, 0x1, 0x1, 0x0},
	{0x1814, 0x1, 0x1, 0x0},
	{0x181a, 0x1, 0x1, 0x0},
	{0x18a0, 0x1, 0x1, 0x0},
	{0x1894, 0x1, 0x1, 0x0},
	{0x189a, 0x1, 0x1, 0x0},
	{0x1e96, 0x1, 0x1, 0xf},
	{0x1eb6, 0x1, 0x1, 0xf},
	{0x1f16, 0x1, 0x1, 0xf},
	{0x1f3c, 0x1, 0x1, 0xf},
	{0x1c58, 0x1, 0x1, 0xf},
	{0x1d8e, 0x1, 0x1, 0xf},
	{0x1c34, 0x1, 0x1, 0xf},
	{0x1c22, 0x1, 0x1, 0xf},
	{0x1bae, 0x0, 0x1, 0xb},
	{0x1bb2, 0x1, 0x1, 0x1},
	{0x1bb8, 0x0, 0x1, 0x1},
	{0x1cb2, 0x1, 0x1, 0xf},
	{0x1bd6, 0x1, 0x1, 0xf},
	{0x1bc0, 0x1, 0x1, 0xa},
	{0x1bc4, 0x1, 0x1, 0x0},
	{0x1bca, 0x1, 0x1, 0x0},
	{0x1d22, 0x1, 0x1, 0xe},
	{0x1d28, 0x0, 0x1, 0xe},
	{0x1c66, 0x1, 0x1, 0xa},
	{0x1c6a, 0x1, 0x1, 0x0},
	{0x1c70, 0x1, 0x1, 0x0},
	{0x1c42, 0x1, 0x1, 0xb},
	{0x1c46, 0x1, 0x1, 0x1},
	{0x1c4c, 0x1, 0x1, 0x1},
	{0x1d34, 0x1, 0x1, 0xe},
	{0x1d3a, 0x0, 0x1, 0xe},
	{0x1b8a, 0x0, 0x1, 0xb},
	{0x1b8e, 0x1, 0x1, 0x1},
	{0x1b94, 0x0, 0x1, 0x1},
	{0x1d42, 0x1, 0x1, 0xa},
	{0x1d46, 0x1, 0x1, 0x0},
	{0x1d4c, 0x0, 0x1, 0x0},
	{0x1ca0, 0x1, 0x1, 0xf},
	{0x1c8a, 0x1, 0x1, 0xa},
	{0x1c8e, 0x1, 0x1, 0x0},
	{0x1c94, 0x1, 0x1, 0x0},
	{0x1b9c, 0x1, 0x1, 0xa},
	{0x1ba0, 0x1, 0x1, 0x0},
	{0x1ba6, 0x1, 0x1, 0x0},
	{0x1be8, 0x1, 0x1, 0xf},
	{0x1c0e, 0x1, 0x1, 0xf},
	{0x1d0a, 0x1, 0x1, 0xa},
	{0x1d0e, 0x1, 0x1, 0x0},
	{0x1d14, 0x1, 0x1, 0x0},
	{0x1cc4, 0x1, 0x1, 0xf},
	{0x1d6a, 0x1, 0x1, 0xf},
	{0x1cd6, 0x1, 0x1, 0xf},
	{0x1ce8, 0x1, 0x1, 0xf},
	{0x1da0, 0x1, 0x1, 0xf},
	{0x1d58, 0x1, 0x1, 0xf},

	/* Deepidle */
	{0x15a0, 0x1, 0x1, 0x2},
	{0x1594, 0x1, 0x1, 0x2},
	{0x159a, 0x1, 0x1, 0x2},
	{0x1820, 0x1, 0x1, 0x2},
	{0x1814, 0x1, 0x1, 0x2},
	{0x181a, 0x1, 0x1, 0x2},
	{0x18a0, 0x1, 0x1, 0x2},
	{0x1894, 0x1, 0x1, 0x2},
	{0x189a, 0x1, 0x1, 0x2},
	{0x1bc0, 0x1, 0x1, 0xc},
	{0x1bc4, 0x1, 0x1, 0x2},
	{0x1bca, 0x1, 0x1, 0x2},
	{0x1c66, 0x1, 0x1, 0xc},
	{0x1c6a, 0x1, 0x1, 0x2},
	{0x1c70, 0x1, 0x1, 0x2},
	{0x1d42, 0x1, 0x1, 0xc},
	{0x1d46, 0x1, 0x1, 0x2},
	{0x1d4c, 0x0, 0x1, 0x2},
	{0x1c8a, 0x1, 0x1, 0xc},
	{0x1c8e, 0x1, 0x1, 0x2},
	{0x1c94, 0x1, 0x1, 0x2},
	{0x1b9c, 0x1, 0x1, 0xc},
	{0x1ba0, 0x1, 0x1, 0x2},
	{0x1ba6, 0x1, 0x1, 0x2},
	{0x1be8, 0x1, 0x1, 0xf},
	{0x1c0e, 0x1, 0x1, 0xf},
	{0x1d0a, 0x1, 0x1, 0xc},
	{0x1d0e, 0x1, 0x1, 0x2},
	{0x1d14, 0x1, 0x1, 0x2},
	{0x1d0e, 0x1, 0x1, 0x2},
	{0x1d14, 0x1, 0x1, 0x2},
};

static struct pmif *pmif_arb = NULL;
static void mt6359p_write(u32 reg, u32 data)
{
	pmif_arb->write(pmif_arb, 0, reg, data);
}

static u32 mt6359p_read_field(u32 reg, u32 mask, u32 shift)
{
	return pmif_arb->read_field(pmif_arb, 0, reg, mask, shift);
}

static void mt6359p_write_field(u32 reg, u32 val, u32 mask, u32 shift)
{
	pmif_arb->write_field(pmif_arb, 0, reg, val, mask, shift);
}

static void pmic_set_power_hold(void)
{
	mt6359p_write_field(PMIC_PWRHOLD, 0x1, 0x1, 0);
}

static void pmic_wdt_set(void)
{
	/* [5]=1, RG_WDTRSTB_DEB */
	mt6359p_write_field(PMIC_TOP_RST_MISC_SET, 0x20, 0xFFFF, 0);
	/* [1]=0, RG_WDTRSTB_MODE */
	mt6359p_write_field(PMIC_TOP_RST_MISC_CLR, 0x02, 0xFFFF, 0);
	/* [0]=1, RG_WDTRSTB_EN */
	mt6359p_write_field(PMIC_TOP_RST_MISC_SET, 0x01, 0xFFFF, 0);
}

static void pmic_init_setting(void)
{
	for (int i = 0; i < ARRAY_SIZE(init_setting); i++)
		mt6359p_write_field(init_setting[i].addr, init_setting[i].val,
			init_setting[i].mask, init_setting[i].shift);
}

static void pmic_lp_setting(void)
{
	for (int i = 0; i < ARRAY_SIZE(lp_setting); i++)
		mt6359p_write_field(lp_setting[i].addr, lp_setting[i].val,
			lp_setting[i].mask, lp_setting[i].shift);
}

static void pmic_wk_vs2_voter_setting(void)
{
	/*
	 *  1. Set VS2_VOTER_VOSEL = 1.35V
	 *  2. Clear VS2_VOTER
	 *  3. Set VS2_VOSEL = 1.4V
	 */
	mt6359p_write_field(PMIC_VS2_VOTER_CFG, 0x2C, 0x7F, 0);
	mt6359p_write_field(PMIC_VS2_VOTER, 0, 0xFFF, 0);
	mt6359p_write_field(PMIC_VS2_ELR0, 0x30, 0x7F, 0);
}

void mt6359p_buck_set_voltage(u32 buck_id, u32 buck_uv)
{
	u32 vol_offset, vol_reg, vol;

	if (!pmif_arb)
		die("ERROR: pmif_arb not initialized");

	switch (buck_id) {
	case MT6359P_GPU11:
		vol_offset = 400000;
		vol_reg = PMIC_VGPU11_ELR0;
		break;
	case MT6359P_SRAM_PROC1:
		vol_offset = 500000;
		vol_reg = PMIC_VSRAM_PROC1_ELR;
		break;
	case MT6359P_SRAM_PROC2:
		vol_offset = 500000;
		vol_reg = PMIC_VSRAM_PROC2_ELR;
		break;
	default:
		die("ERROR: Unknown buck_id %u", buck_id);
		return;
	};

	vol = (buck_uv - vol_offset) / 6250;
	mt6359p_write_field(vol_reg, vol, 0x7F, 0);
}

u32 mt6359p_buck_get_voltage(u32 buck_id)
{
	u32 vol_shift, vol_offset, vol_reg, vol;

	if (!pmif_arb)
		die("ERROR: pmif_arb not initialized");

	switch (buck_id) {
	case MT6359P_GPU11:
		vol_shift = 0;
		vol_offset = 400000;
		vol_reg = PMIC_VGPU11_DBG0;
		break;
	case MT6359P_SRAM_PROC1:
		vol_shift = 8;
		vol_offset = 500000;
		vol_reg = PMIC_VSRAM_PROC1_VOSEL1;
		break;
	case MT6359P_SRAM_PROC2:
		vol_shift = 8;
		vol_offset = 500000;
		vol_reg = PMIC_VSRAM_PROC2_VOSEL1;
		break;
	default:
		die("ERROR: Unknown buck_id %u", buck_id);
		return 0;
	};

	vol = mt6359p_read_field(vol_reg, 0x7F, vol_shift);
	return vol_offset + vol * 6250;
}

void mt6359p_set_vm18_voltage(u32 vm18_uv)
{
	u32 reg_vol, reg_cali;

	if (!pmif_arb)
		die("ERROR: pmif_arb not initialized");

	assert(vm18_uv >= 1700000);
	assert(vm18_uv < 2000000);

	reg_vol = (vm18_uv / 1000 - VM18_VOL_OFFSET) / 100;
	reg_cali = ((vm18_uv / 1000) % 100) / 10;
	mt6359p_write(PMIC_VM18_ANA_CON0, (reg_vol << VM18_VOL_REG_SHIFT) | reg_cali);
}

u32 mt6359p_get_vm18_voltage(void)
{
	u32 reg_vol, reg_cali;

	if (!pmif_arb)
		die("ERROR: pmif_arb not initialized");

	reg_vol = 100 * mt6359p_read_field(PMIC_VM18_ANA_CON0, 0xF, VM18_VOL_REG_SHIFT);
	reg_cali = 10 * mt6359p_read_field(PMIC_VM18_ANA_CON0, 0xF, 0);
	return 1000 * (VM18_VOL_OFFSET + reg_vol + reg_cali);
}

static void init_pmif_arb(void)
{
	if (!pmif_arb) {
		pmif_arb = get_pmif_controller(PMIF_SPI, 0);
		if (!pmif_arb)
			die("ERROR: No spi device");
	}

	if (pmif_arb->is_pmif_init_done(pmif_arb))
		die("ERROR - Failed to initialize pmif spi");
}

void mt6359p_init(void)
{
	init_pmif_arb();
	pmic_set_power_hold();
	pmic_wdt_set();
	pmic_init_setting();
	pmic_lp_setting();
	pmic_wk_vs2_voter_setting();
}

void mt6359p_romstage_init(void)
{
	init_pmif_arb();
}

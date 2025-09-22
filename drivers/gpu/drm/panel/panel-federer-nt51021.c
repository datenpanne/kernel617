// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2025 FIXME
// Generated with linux-mdss-dsi-panel-driver-generator from vendor device tree:
//   Copyright (c) 2013, The Linux Foundation. All rights reserved. (FIXME)

#include <linux/backlight.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/regulator/consumer.h>

#include <drm/drm_mipi_dsi.h>
#include <drm/drm_modes.h>
#include <drm/drm_panel.h>
#include <drm/drm_probe_helper.h>

struct huawei_nt51021 {
	struct drm_panel panel;
	struct mipi_dsi_device *dsi;
//	struct regulator_bulk_data *supplies;
	struct gpio_desc *reset_gpio;
//	struct gpio_desc *vcc_pwr_gpio;
//	struct gpio_desc *bl_pwr_gpio;
	struct gpio_desc *vled_en_gpio;

	struct regulator *vcc;
	struct regulator *vddio;
	struct regulator *vsp;
	struct regulator *vsn;

	int hw_led_en_flag; //taken from original source
};

#define HUAWEI_NT51021_BRIGHTNESS 0x9f

/*static const struct regulator_bulk_data huawei_nt51021_supplies[] = {
	{ .supply = "vddio" },
	{ .supply = "vsp" },
	{ .supply = "vsn" },
};*/

static inline
struct huawei_nt51021 *to_huawei_nt51021(struct drm_panel *panel)
{
	return container_of(panel, struct huawei_nt51021, panel);
}

static void huawei_nt51021_reset(struct huawei_nt51021 *ctx)
{
	gpiod_set_value_cansleep(ctx->reset_gpio, 0);
	usleep_range(1000, 2000);
	gpiod_set_value_cansleep(ctx->reset_gpio, 1);
	msleep(20);
	gpiod_set_value_cansleep(ctx->reset_gpio, 0);
	msleep(30);
}

/*static void huawei_nt51021_gpio_power(struct huawei_nt51021 *ctx, int enable)
{
	gpiod_direction_output(ctx->bl_pwr_gpio, enable);
	msleep(50);
	gpiod_direction_output(ctx->vcc_pwr_gpio, enable);
	msleep(500);
}*/

static void huawei_nt51021_gpio_vled(struct huawei_nt51021 *ctx, int enable)
{
	gpiod_set_value(ctx->vled_en_gpio, enable);

	if (enable) {
		ctx->hw_led_en_flag = 1;
	} else {
		ctx->hw_led_en_flag = 0;
	}
}

static int huawei_nt51021_on(struct huawei_nt51021 *ctx)
{
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };

	ctx->dsi->mode_flags |= MIPI_DSI_MODE_LPM;

	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x8f, 0xa5);
	mipi_dsi_usleep_range(&dsi_ctx, 1000, 2000);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x01, 0x00);
	mipi_dsi_msleep(&dsi_ctx, 20);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x8f, 0xa5);
	mipi_dsi_usleep_range(&dsi_ctx, 1000, 2000);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x83, 0x00);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x84, 0x00);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x8c, 0x80);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xcd, 0x6c);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xc8, 0xfc);
	//mipi_dsi_generic_write_seq_multi(&dsi_ctx, HUAWEI_NT51021_BRIGHTNESS, 0x00);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x97, 0x00);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x83, 0xbb);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x84, 0x22);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x96, 0x00);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x90, 0xc0);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x91, 0xa0);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x9a, 0x10);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x94, 0x78);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x95, 0xb1);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xa1, 0xff);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xa2, 0xfa);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xa3, 0xf3);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xa4, 0xed);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xa5, 0xe7);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xa6, 0xe2);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xa7, 0xdc);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xa8, 0xd7);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xa9, 0xd1);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xaa, 0xcc);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xb4, 0x1c);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xb5, 0x38);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xb6, 0x30);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x83, 0xaa);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x84, 0x11);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xa9, 0x4b);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x85, 0x04);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x86, 0x08);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x9c, 0x10);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x83, 0x00);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x84, 0x00);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, HUAWEI_NT51021_BRIGHTNESS, 0x00);
	mipi_dsi_dcs_exit_sleep_mode_multi(&dsi_ctx);
	mipi_dsi_usleep_range(&dsi_ctx, 5000, 6000);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x8f, 0x00);
	mipi_dsi_usleep_range(&dsi_ctx, 1000, 2000);

	return dsi_ctx.accum_err;
}

static int huawei_nt51021_off(struct huawei_nt51021 *ctx)
{
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };

	ctx->dsi->mode_flags &= ~MIPI_DSI_MODE_LPM;

	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x8f, 0xa5);
	mipi_dsi_msleep(&dsi_ctx, 20);
	mipi_dsi_dcs_set_display_off_multi(&dsi_ctx);
	mipi_dsi_msleep(&dsi_ctx, 100);
	mipi_dsi_dcs_enter_sleep_mode_multi(&dsi_ctx);
	mipi_dsi_msleep(&dsi_ctx, 120);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x8f, 0x00);
	mipi_dsi_usleep_range(&dsi_ctx, 4000, 5000);

	return dsi_ctx.accum_err;
}

static int huawei_nt51021_prepare(struct drm_panel *panel)
{
	struct huawei_nt51021 *ctx = to_huawei_nt51021(panel);
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };
	struct device *dev = &ctx->dsi->dev;
	int ret;

	ret = regulator_enable(ctx->vcc);
	if (ret < 0) {
		dev_err(dev, "Failed to enable vcc regulator: %d\n", ret);
		return ret;
	}
	msleep(500);

	ret = regulator_enable(ctx->vddio);
	if (ret < 0) {
		dev_err(dev, "Failed to enable VDDIO regulator: %d\n", ret);
		return ret;
	}
	msleep(50);

	ret = regulator_enable(ctx->vsp);
	if (ret < 0) {
		dev_err(dev, "Failed to enable VSP regulator: %d\n", ret);
		return ret;
	}
	msleep(500);

	ret = regulator_enable(ctx->vsn);
	if (ret < 0) {
		dev_err(dev, "Failed to enable VSN regulator: %d\n", ret);
		return ret;
	}

	ret = mipi_dsi_dcs_nop(ctx->dsi);
		if (ret < 0) {
			dev_err(dev, "Failed to send NOP: %d\n", ret);
	}
	mipi_dsi_msleep(&dsi_ctx, 80);

	huawei_nt51021_reset(ctx);

	ret = huawei_nt51021_on(ctx);
	if (ret < 0) {
		dev_err(dev, "Failed to initialize panel: %d\n", ret);
		gpiod_set_value_cansleep(ctx->reset_gpio, 1);
		goto regdsb;
	}

	//huawei_nt51021_gpio_vled(ctx,1);
	//msleep(100);
	mipi_dsi_dcs_set_display_on_multi(&dsi_ctx);
	mipi_dsi_msleep(&dsi_ctx, 80);

	return 0;

regdsb:
	regulator_disable(ctx->vsn);
	regulator_disable(ctx->vsp);
	usleep_range(5000, 7000);
	gpiod_set_value_cansleep(ctx->reset_gpio, 1);
	regulator_disable(ctx->vddio);
	regulator_disable(ctx->vcc);
	return ret;
}

static int huawei_nt51021_enable(struct drm_panel *panel)
{
	struct huawei_nt51021 *ctx = to_huawei_nt51021(panel);
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };

	huawei_nt51021_gpio_vled(ctx,1);
	msleep(100);

	mipi_dsi_dcs_set_display_on_multi(&dsi_ctx);
	mipi_dsi_msleep(&dsi_ctx, 80);

	msleep(30);

	return 0;
}

static int huawei_nt51021_unprepare(struct drm_panel *panel)
{
	struct huawei_nt51021 *ctx = to_huawei_nt51021(panel);
	struct device *dev = &ctx->dsi->dev;
	int ret;

	ret = huawei_nt51021_off(ctx);
	if (ret < 0)
		dev_err(dev, "Failed to un-initialize panel: %d\n", ret);

	regulator_disable(ctx->vsn);
	regulator_disable(ctx->vsp);
	usleep_range(5000, 7000);
	gpiod_set_value_cansleep(ctx->reset_gpio, 1);
	regulator_disable(ctx->vddio);
	regulator_disable(ctx->vcc);

	return 0;
}

static int huawei_nt51021_disable(struct drm_panel *panel)
{
	struct huawei_nt51021 *ctx = to_huawei_nt51021(panel);
	struct device *dev = &ctx->dsi->dev;
	int ret;

	ret = huawei_nt51021_off(ctx);
	if (ret < 0)
		dev_err(dev, "Failed to un-initialize panel: %d\n", ret);

	huawei_nt51021_gpio_vled(ctx,0);
	msleep(200);

	return 0;
}

static const struct drm_display_mode huawei_nt51021_mode = {
	.clock = (1200 + 64 + 4 + 36) * (1920 + 104 + 2 + 24) * 60 / 1000,
	.hdisplay = 1200,
	.hsync_start = 1200 + 64,
	.hsync_end = 1200 + 64 + 4,
	.htotal = 1200 + 64 + 4 + 36,
	.vdisplay = 1920,
	.vsync_start = 1920 + 104,
	.vsync_end = 1920 + 104 + 2,
	.vtotal = 1920 + 104 + 2 + 24,
	.width_mm = 135,
	.height_mm = 217,
	.type = DRM_MODE_TYPE_DRIVER,
};

static int huawei_nt51021_get_modes(struct drm_panel *panel,
					  struct drm_connector *connector)
{
	return drm_connector_helper_get_modes_fixed(connector, &huawei_nt51021_mode);
}

static const struct drm_panel_funcs huawei_nt51021_panel_funcs = {
	.prepare = huawei_nt51021_prepare,
	.enable = huawei_nt51021_enable,
	.disable = huawei_nt51021_disable,
	.unprepare = huawei_nt51021_unprepare,
	.get_modes = huawei_nt51021_get_modes,
};

static int huawei_nt51021_set_brightness(struct mipi_dsi_device *dsi,
					u16 brightness)
{
	const u8 chang_page0_index0[2] = {0x83, 0x00};
	const u8 chang_page0_index1[2] = {0x84, 0x00};
	u8 payload[2] = { brightness & 0xff, brightness >> 8 };
	int ret;

	mipi_dsi_dcs_write_buffer(dsi, chang_page0_index0,
					ARRAY_SIZE(chang_page0_index0));

	mipi_dsi_dcs_write_buffer(dsi, chang_page0_index1,
					ARRAY_SIZE(chang_page0_index1));

	//mipi_dsi_generic_write_seq_multi(&dsi_ctx, HUAWEI_NT51021_BRIGHTNESS,  payload, sizeof(payload));
	ret = mipi_dsi_dcs_write(dsi, HUAWEI_NT51021_BRIGHTNESS,
				 payload, sizeof(payload));
	if (ret < 0)
		return ret;

	return 0;
}
/*
static int huawei_nt51021_get_brightness(struct mipi_dsi_device *dsi,
					u16 *brightness)
{
	int ret;

	ret = mipi_dsi_dcs_read(dsi, HUAWEI_NT51021_BRIGHTNESS,
				brightness, sizeof(*brightness));
	if (ret <= 0) {
		if (ret == 0)
			ret = -ENODATA;

		return ret;
	}
	return 0;
}*/

static int huawei_nt51021_bl_update_status(struct backlight_device *bl)
{
	struct mipi_dsi_device *dsi = bl_get_data(bl);
	//struct huawei_nt51021 *ctx = mipi_dsi_get_drvdata(dsi);
	//struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };
	u16 brightness = backlight_get_brightness(bl);
	int ret;

	/*gpiod_set_value_cansleep(ctx->vled_en_gpio, !!brightness);
	mipi_dsi_msleep(&dsi_ctx, 200);*/

	dsi->mode_flags &= ~MIPI_DSI_MODE_LPM;

	ret = huawei_nt51021_set_brightness(dsi, brightness);
	//ret = mipi_dsi_dcs_write(dsi, HUAWEI_NT51021_BRIGHTNESS, &brightness, 1);
	dsi->mode_flags |= MIPI_DSI_MODE_LPM;
	if (ret < 0)
		return ret;

	return 0;
}
/*
static int huawei_nt51021_bl_get_brightness(struct backlight_device *bl)
{
	struct mipi_dsi_device *dsi = bl_get_data(bl);
	u16 brightness;
	int ret;

	dsi->mode_flags &= ~MIPI_DSI_MODE_LPM;

	ret = huawei_nt51021_get_brightness(dsi, &brightness);
	if (ret < 0)
		return ret;

	dsi->mode_flags |= MIPI_DSI_MODE_LPM;

	return brightness & 0xff;
}*/

static const struct backlight_ops huawei_nt51021_bl_ops = {
	.update_status = huawei_nt51021_bl_update_status,
	//.get_brightness = huawei_nt51021_bl_get_brightness,
};

static struct backlight_device *
huawei_nt51021_create_backlight(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	const struct backlight_properties props = {
		.type = BACKLIGHT_RAW,
		.brightness = 128,
		.max_brightness = 255,
	};

	return devm_backlight_device_register(dev, dev_name(dev), dev, dsi,
					      &huawei_nt51021_bl_ops, &props);
}

static int huawei_nt51021_probe(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	struct huawei_nt51021 *ctx;
	int ret;

	ctx = devm_kzalloc(dev, sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	ctx->vcc = devm_regulator_get(dev, "vcc");
	if (IS_ERR(ctx->vcc))
		return dev_err_probe(dev, PTR_ERR(ctx->vcc),
				     "Failed to get vcc regulator\n");

	ctx->vddio = devm_regulator_get(dev, "vddio");
	if (IS_ERR(ctx->vddio))
		return dev_err_probe(dev, PTR_ERR(ctx->vddio),
				     "Failed to get vddio regulator\n");

	ctx->vsp = devm_regulator_get(dev, "vsp");
	if (IS_ERR(ctx->vsp))
		return dev_err_probe(dev, PTR_ERR(ctx->vsp),
				     "Failed to get vsp regulator\n");

	ctx->vsn = devm_regulator_get(dev, "vsn");
	if (IS_ERR(ctx->vsn))
		return dev_err_probe(dev, PTR_ERR(ctx->vsn),
				     "Failed to get vsn regulator\n");

	ctx->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio))
		return dev_err_probe(dev, PTR_ERR(ctx->reset_gpio),
				     "Failed to get reset-gpios\n");

	ctx->vled_en_gpio = devm_gpiod_get(dev, "backlight", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->vled_en_gpio))
		return dev_err_probe(dev, PTR_ERR(ctx->vled_en_gpio),
				     "Failed to get backlight-gpios\n");

	ctx->dsi = dsi;
	mipi_dsi_set_drvdata(dsi, ctx);

	dsi->lanes = 4;
	dsi->format = MIPI_DSI_FMT_RGB888;
	dsi->mode_flags = MIPI_DSI_MODE_VIDEO
			| MIPI_DSI_MODE_VIDEO_BURST
			| MIPI_DSI_MODE_VIDEO_HSE
			| MIPI_DSI_MODE_NO_EOT_PACKET;
			//| MIPI_DSI_CLOCK_NON_CONTINUOUS; //black screeen!?

	drm_panel_init(&ctx->panel, dev, &huawei_nt51021_panel_funcs,
		       DRM_MODE_CONNECTOR_DSI);
	ctx->panel.prepare_prev_first = true;

	ctx->panel.backlight = huawei_nt51021_create_backlight(dsi);
	if (IS_ERR(ctx->panel.backlight))
		return dev_err_probe(dev, PTR_ERR(ctx->panel.backlight),
				     "Failed to create backlight\n");

	drm_panel_add(&ctx->panel);

	ret = mipi_dsi_attach(dsi);
	if (ret < 0) {
		drm_panel_remove(&ctx->panel);
		return dev_err_probe(dev, ret, "Failed to attach to DSI host\n");
	}

	return 0;
}

static void huawei_nt51021_remove(struct mipi_dsi_device *dsi)
{
	struct huawei_nt51021 *ctx = mipi_dsi_get_drvdata(dsi);
	int ret;

	ret = mipi_dsi_detach(dsi);
	if (ret < 0)
		dev_err(&dsi->dev, "Failed to detach from DSI host: %d\n", ret);

	drm_panel_remove(&ctx->panel);
}

static const struct of_device_id huawei_nt51021_of_match[] = {
	{ .compatible =  "huawei,boe-nt51021" },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, huawei_nt51021_of_match);

static struct mipi_dsi_driver huawei_nt51021_driver = {
	.probe = huawei_nt51021_probe,
	.remove = huawei_nt51021_remove,
	.driver = {
		.name = "panel-federer-nt51021",
		.of_match_table = huawei_nt51021_of_match,
	},
};
module_mipi_dsi_driver(huawei_nt51021_driver);

MODULE_AUTHOR("linux-mdss-dsi-panel-driver-generator <fix@me>"); // FIXME
MODULE_DESCRIPTION("DRM driver for BOE_NT51021_10_1200P_VIDEO");
MODULE_LICENSE("GPL");

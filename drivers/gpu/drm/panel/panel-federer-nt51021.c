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
	struct gpio_desc *reset_gpio;
	struct gpio_desc *vcc_pwr_gpio;
	struct gpio_desc *bl_pwr_gpio;
	struct gpio_desc *vled_en_gpio;

	struct regulator *vddio;
	struct regulator *vsp;
	struct regulator *vsn;

	int hw_led_en_flag; //from original source
};

#define HUAWEI_NT51021_BRIGHTNESS 0x9f

static inline
struct huawei_nt51021 *to_huawei_nt51021(struct drm_panel *panel)
{
	return container_of(panel, struct huawei_nt51021, panel);
}

static int huawei_nt51021_gpio_vled(struct huawei_nt51021 *ctx, int enable)
{
	int ret;

	if (enable==1) {
		ret = gpiod_set_value(ctx->vled_en_gpio, enable);
		if (ret < 0) {
			dev_err(&ctx->dsi->dev, "Failed to enable VLED gpio: %d\n", ret);
			return ret;
		}
		ctx->hw_led_en_flag = 1;
	} else {
		gpiod_set_value(ctx->vled_en_gpio, enable);
		ctx->hw_led_en_flag = 0;
	}

	return 0;
}

static int huawei_nt51021_on(struct huawei_nt51021 *ctx)
{
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };

	ctx->dsi->mode_flags |= MIPI_DSI_MODE_LPM;

	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x83, 0x00);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x84, 0x00);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x8c, 0x80);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xcd, 0x6c);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xc8, 0xfc);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, HUAWEI_NT51021_BRIGHTNESS, 0x00);
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
	mipi_dsi_dcs_exit_sleep_mode_multi(&dsi_ctx);
	mipi_dsi_usleep_range(&dsi_ctx, 5000, 6000);

	return dsi_ctx.accum_err;
}

static int huawei_nt51021_prepare(struct drm_panel *panel)
{
	struct huawei_nt51021 *ctx = to_huawei_nt51021(panel);
	struct device *dev = &ctx->dsi->dev;
	int ret;

	ret = regulator_enable(ctx->vddio);
	if (ret < 0) return ret;
	msleep(20);

	gpiod_set_value_cansleep(ctx->vcc_pwr_gpio, 1);
	msleep(300);

	// 3. Analoge Spannungen für die Source-Driver
	ret = regulator_enable(ctx->vsp);
	if (ret < 0) goto err_vddio;
	ret = regulator_enable(ctx->vsn);
	if (ret < 0) {
		regulator_disable(ctx->vsp);
		goto err_vddio;
	}
	msleep(5);
	
	//Backlight-Power GPIO
	gpiod_set_value_cansleep(ctx->bl_pwr_gpio, 1);
	msleep(30);

	// 4. Reset-Zyklus: Aktivieren -> Warten -> Deaktivieren
	// Annahme: Active Low (0 = Reset aktiv). 
	// Da wir mit GPIOD_OUT_HIGH proben, ist '1' in der Software = Hardware Reset Pin LOW.
	gpiod_set_value_cansleep(ctx->reset_gpio, 1); 
	usleep_range(10000, 15000);
	gpiod_set_value_cansleep(ctx->reset_gpio, 0); // Reset verlassen
	msleep(20);

	// 5. Initialisierungs-Register senden (LP Mode)
	ret = huawei_nt51021_on(ctx);
	if (ret < 0) {
		dev_err(dev, "Failed to send init sequence: %d\n", ret);
		goto err_vsn;
	}

	return 0;

err_vsn:
	regulator_disable(ctx->vsn);
	regulator_disable(ctx->vsp);
err_vddio:
	regulator_disable(ctx->vddio);
	return ret;
}

static int huawei_nt51021_enable(struct drm_panel *panel)
{
	struct huawei_nt51021 *ctx = to_huawei_nt51021(panel);
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };
	int ret;

	// 2. VLED Regulator (Die Hochspannung für die LEDs)
	ret = huawei_nt51021_gpio_vled(ctx, 1);
	if (ret < 0)
		return ret;

	msleep(200);

	// 3. Display aus dem Sleep holen und Bild anzeigen
	mipi_dsi_dcs_exit_sleep_mode_multi(&dsi_ctx);
	msleep(150); // Notwendig nach Sleep Exit
	mipi_dsi_dcs_set_display_on_multi(&dsi_ctx);

	return dsi_ctx.accum_err;
}

static int huawei_nt51021_unprepare(struct drm_panel *panel)
{
	struct huawei_nt51021 *ctx = to_huawei_nt51021(panel);

	// 1. Reset wieder aktivieren (Pin auf Low ziehen)
	// Software '1' -> Hardware Reset Pin LOW bei GPIOD_OUT_HIGH
	gpiod_set_value_cansleep(ctx->reset_gpio, 1);
	msleep(20);

	// 2. Analoge Spannungen (Source Driver) zuerst entladen
	regulator_disable(ctx->vsn);
	regulator_disable(ctx->vsp);
	usleep_range(3000,6000);
	
	gpiod_set_value_cansleep(ctx->bl_pwr_gpio, 0);
	msleep(20);

	// 4. Digitale Logik (VDDIO) zuletzt trennen
	regulator_disable(ctx->vddio);

	gpiod_set_value_cansleep(ctx->vcc_pwr_gpio, 0);
	msleep(300);

	return 0;
}

static int huawei_nt51021_disable(struct drm_panel *panel)
{
	struct huawei_nt51021 *ctx = to_huawei_nt51021(panel);
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };

	// 1. Backlight zuerst aus, um Artefakte beim Abschalten zu verbergen
	huawei_nt51021_gpio_vled(ctx, 0);
	msleep(100);
	// 2. Display-Aus Befehl und Sleep-Modus
	mipi_dsi_dcs_set_display_off_multi(&dsi_ctx);
	msleep(20);
	mipi_dsi_dcs_enter_sleep_mode_multi(&dsi_ctx);
	
	// Dem Panel Zeit geben, den internen Status zu speichern (DCS Standard: 120ms)
	msleep(120);

	return dsi_ctx.accum_err;
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

static int huawei_nt51021_set_brightness(struct mipi_dsi_device *dsi, u16 brightness)
{
	u8 val = (u8)brightness;
	u8 cmd_p0_83[] = { 0x83, 0x00 };
	u8 cmd_p0_84[] = { 0x84, 0x00 };
	u8 cmd_pa_83[] = { 0x83, 0xaa };
	u8 cmd_p1_84[] = { 0x84, 0x11 };
	int ret;

	// Page 0
	mipi_dsi_generic_write(dsi, cmd_p0_83, sizeof(cmd_p0_83));
	mipi_dsi_generic_write(dsi, cmd_p0_84, sizeof(cmd_p0_84));

	// Write Brightnes
	ret = mipi_dsi_dcs_write(dsi, HUAWEI_NT51021_BRIGHTNESS, &val, 1);

	// back to page aa and 11
	mipi_dsi_generic_write(dsi, cmd_pa_83, sizeof(cmd_pa_83));
	mipi_dsi_generic_write(dsi, cmd_p1_84, sizeof(cmd_p1_84));

	return ret;
}

static int huawei_nt51021_bl_update_status(struct backlight_device *bl)
{
	struct mipi_dsi_device *dsi = bl_get_data(bl);
	//struct huawei_nt51021 *ctx = mipi_dsi_get_drvdata(dsi);
	//struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };
	u16 brightness = backlight_get_brightness(bl);
	int ret;

	ret = huawei_nt51021_set_brightness(dsi, brightness);

	if (ret < 0)
		return ret;

	return 0;
}

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

	ctx->bl_pwr_gpio = devm_gpiod_get(dev, "backlight", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->bl_pwr_gpio))
		return dev_err_probe(dev, PTR_ERR(ctx->bl_pwr_gpio),
				     "Failed to get backlight-gpios\n");

	ctx->vled_en_gpio = devm_gpiod_get(dev, "blpower", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->vled_en_gpio))
		return dev_err_probe(dev, PTR_ERR(ctx->vled_en_gpio),
				     "Failed to get backlight-gpios\n");
				     
	ctx->vcc_pwr_gpio = devm_gpiod_get(dev, "power", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->vcc_pwr_gpio))
		return dev_err_probe(dev, PTR_ERR(ctx->vcc_pwr_gpio),
				     "Failed to get power-gpios\n");


	ctx->dsi = dsi;
	mipi_dsi_set_drvdata(dsi, ctx);

	dsi->lanes = 4;
	dsi->format = MIPI_DSI_FMT_RGB888;
	dsi->mode_flags = MIPI_DSI_MODE_VIDEO
			| MIPI_DSI_MODE_VIDEO_BURST
			| MIPI_DSI_MODE_VIDEO_HSE
			| MIPI_DSI_MODE_NO_EOT_PACKET;
			//| MIPI_DSI_CLOCK_NON_CONTINUOUS; //black screen!?

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


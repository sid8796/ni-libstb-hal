/*
 * determine the capabilities of the hardware.
 * part of libstb-hal
 *
 * (C) 2010-2012 Stefan Seyfried
 *
 * License: GPL v2 or later
 */

#include <config.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <hardware_caps.h>

static int initialized = 0;
static hw_caps_t caps;

hw_caps_t *get_hwcaps(void)
{
	if (initialized)
		return &caps;

	memset(&caps, 0, sizeof(hw_caps_t));

	caps.pip_devs = 0;
	if (access("/dev/dvb/adapter0/video1", F_OK) != -1)
		caps.pip_devs = 1;
	if (access("/dev/dvb/adapter0/video2", F_OK) != -1)
		caps.pip_devs = 2;
	if (access("/dev/dvb/adapter0/video3", F_OK) != -1)
		caps.pip_devs = 3;
	if (caps.pip_devs > 0)
		caps.can_pip = 1;

#if BOXMODEL_VUDUO
	caps.has_CI = 2;
	caps.can_cec = 1;
	caps.can_cpufreq = 0;
	caps.can_shutdown = 1;
	caps.display_xres = 16;
	caps.display_type = HW_DISPLAY_LINE_TEXT;
	caps.display_can_deepstandby = 1;
	caps.display_can_set_brightness = 1;
	caps.display_can_umlauts = 0; /* need test */
	caps.display_has_statusline = 0;
	caps.display_has_colon = 0;
	caps.has_button_timer = 1;
	caps.has_HDMI = 1;
	caps.has_SCART = 1;
//	caps.has_SCART_input = 1;
	strcpy(caps.startup_file, "");
	strcpy(caps.boxmodel, "vuduo");
	strcpy(caps.boxvendor, "VU+");
	strcpy(caps.boxname, "DUO");
	strcpy(caps.boxarch, "BCM7335");
#endif

#if BOXMODEL_FORMULER3
    /* Initial profile based on the F3 runtime inventory, 2026-11-02.
     * Enable panel/CI/CEC/deep standby only after device-level validation.
     * F3 is not a VU+ model: keep vendor-specific branches disabled.
     */
    caps.has_HDMI = 1;
    caps.pip_devs = 0;
    caps.can_pip = 0;
    caps.can_cec = 0;
    caps.can_shutdown = 0;
    caps.can_cpufreq = 0;
    caps.display_type = HW_DISPLAY_NONE;
    strcpy(caps.boxmodel, "formuler3");
    strcpy(caps.boxvendor, "Formuler");
    strcpy(caps.boxname, "F3");
    strcpy(caps.boxarch, "BCM7362");
#endif

	initialized = 1;
	return &caps;
}

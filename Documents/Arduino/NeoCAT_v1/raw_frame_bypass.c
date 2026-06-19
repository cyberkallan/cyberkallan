/**
 * raw_frame_bypass.c — NeoCAT v1
 *
 * Overrides the ESP-IDF ieee80211_raw_frame_sanity_check function
 * using the GCC linker --wrap mechanism. This allows esp_wifi_80211_tx()
 * to transmit ALL 802.11 frame types including management frames
 * (deauth, disassoc, etc.) that are normally blocked.
 *
 * The linker flag -Wl,--wrap=ieee80211_raw_frame_sanity_check
 * is set in platform.local.txt.
 *
 * For authorized security testing only.
 */

#include "esp_err.h"

int __wrap_ieee80211_raw_frame_sanity_check(int32_t arg, int32_t arg2, int32_t arg3) {
    return 0;  // Always allow — bypass frame type filtering
}

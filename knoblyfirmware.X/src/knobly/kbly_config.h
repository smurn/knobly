#ifndef _KBLY_CONFIG_H_
#define _KBLY_CONFIG_H_

// GENERAL
// -------
#define KBLY_N_JOYSTICKS 2          // Number of joysticks
#define KBLY_TOTAL_BYTES 5          // Sum of all BTN_BYTES (see below)
#define KBLY_SCAN_INTERVAL 100      // Time between scans (one row each) [us]
#define KBLY_BUTTON_ROWS 2          // Rows of buttons (8 btns each).
#define KBLY_ENCODER_ROWS 14        // Rows of encoders (4 encs each).
#define KBLY_DEBOUNCE_DELAY 20000   // Delay until released [us].
#define KBLY_ENCODER_PULSE_WIDTH 40000 // // Delay until released [us].

#endif // _KBLY_CONFIG_H_
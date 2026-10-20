# EkoRelay artwork

`boot-320x240.png` is the supplied, approved composition: symbol, wordmark and Russian slogan. Keep the bottom field free for the runtime version/status label.

Regenerate the LVGL 9 RGB565 arrays with Python + Pillow:

```sh
python tools/convert_ekorelay.py
```

The converter writes `source/graphics/TFT/EkoRelayImages.c`, outside the EEZ generated directory, so exporting the original screens does not overwrite branding. RGB565 is little-endian, two bytes per pixel; the full screen consumes 153600 bytes in flash and needs no PNG decoder or SD card. The small header mark is derived from the same artwork.

The 320x240 view installs the image behind the existing boot controls and keeps the bottom version/status label visible. Programming mode hides the artwork and retains its original messages, PIN and Bluetooth control. Other view sizes retain their original custom-file loading behavior.

## Home menu

The EkoRelay 320x240 home view adds four rounded cards and keeps the existing side navigation, messages restoration, map loading, settings lock and input handlers. Cards forward a click to the corresponding original navigation button after transferring group focus. The `i` button switches to the original home diagnostics; Home returns to the cards. No radio settings are changed.

Counts use `nodesOnline`, `nodeCount` and `unreadMessages`. Online means the firmware's last-heard window, not measured physical distance. LoRa status distinguishes TX disabled, a recently received packet and waiting; it does not promise end-to-end connectivity. The clock uses the existing device time/12-hour preference and shows `--:--` until valid time is available. The original battery panel retains its live telemetry.

The view uses the existing Montserrat 14/12 Cyrillic fonts, a 266x42 card area, and the symbol crop from the supplied artwork. The native LVGL harness in `tests/ekorelay` checks navigation forwarding, focus, layout and counters. Hardware input, display orientation/colors and radio behavior still require a physical T-Deck check.

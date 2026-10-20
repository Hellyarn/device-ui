# EkoRelay artwork

`boot-320x240.png` is the supplied, approved composition: symbol, wordmark and Russian slogan. Keep the bottom field free for the runtime version/status label.

Regenerate the LVGL 9 RGB565 arrays with Python + Pillow:

```sh
python tools/convert_ekorelay.py
```

The converter writes `source/graphics/TFT/EkoRelayImages.c`, outside the EEZ generated directory, so exporting the original screens does not overwrite branding. RGB565 is little-endian, two bytes per pixel; the full screen consumes 153600 bytes in flash and needs no PNG decoder or SD card. The small header mark is derived from the same artwork.

The 320x240 view installs the image behind the existing boot controls and keeps the bottom version/status label visible. Programming mode hides the artwork and retains its original messages, PIN and Bluetooth control. Other view sizes retain their original custom-file loading behavior.

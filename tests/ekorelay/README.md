# Headless LVGL validation

Use LVGL v9.3.0, CMake and a C/C++17 compiler. This harness compiles the real EEZ screens, fonts, resources and EkoRelay layout modules with the device UI configuration. Its only configuration change is a larger host heap.

```sh
cmake -S tests/ekorelay -B build/ekorelay -DLVGL_ROOT=/absolute/path/to/lvgl-9.3.0
cmake --build build/ekorelay --parallel 4
ctest --test-dir build/ekorelay --output-on-failure
```

`boot.ppm` and `home.ppm` are software-rendered 320x240 frames. Menu counts and clock in the preview are test fixtures, not production defaults. The checks cover boot footer visibility, no duplicate wordmark, card bounds, forwarding all four click events to the existing navigation buttons, focus transfer, diagnostics toggling, counters including zero, and Cyrillic glyph coverage in both menu fonts. The harness does not emulate the radio, hardware keyboard, touch controller or framebuffer driver, or run firmware navigation handlers.

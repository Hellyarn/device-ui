"""Generate LVGL 9 little-endian RGB565 resources: python tools/convert_ekorelay.py."""
from pathlib import Path
from PIL import Image

root = Path(__file__).resolve().parents[1]
image = Image.open(root / "resources/ekorelay/boot-320x240.png").convert("RGB")
assert image.size == (320, 240)


def descriptor(image, name):
    data = bytearray()
    for r, g, b in image.getdata():
        value = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)
        data.extend(value.to_bytes(2, "little"))
    rows = ["    " + ", ".join(f"0x{b:02x}" for b in data[i:i + 24]) + ","
            for i in range(0, len(data), 24)]
    return (f"static const uint8_t {name}_data[] = {{\n" + "\n".join(rows) +
            f"\n}};\nconst lv_image_dsc_t {name} = {{\n"
            f"    .header = {{.magic = LV_IMAGE_HEADER_MAGIC, .cf = LV_COLOR_FORMAT_RGB565,"
            f" .flags = 0, .w = {image.width}, .h = {image.height}, .stride = {image.width * 2}}},\n"
            f"    .data_size = sizeof({name}_data),\n    .data = {name}_data,\n}};\n")


output = '#if HAS_TFT && defined(VIEW_320x240)\n#include "lvgl.h"\n'
output += descriptor(image, "img_ekorelay_boot")
output += descriptor(image.crop((62, 32, 259, 126)).resize((34, 16), Image.Resampling.LANCZOS),
                     "img_ekorelay_mark")
output += "#endif\n"
(root / "source/graphics/TFT/EkoRelayImages.c").write_text(output, encoding="utf-8", newline="\n")

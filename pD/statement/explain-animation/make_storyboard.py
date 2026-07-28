import argparse
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont


PANELS = [
    (0.23, "1  BRANCH", "#B678FF"),
    (0.41, "2  SQUASH", "#FFC857"),
    (0.59, "3  REPLAY 1/2", "#4C8DFF"),
    (0.70, "4  CONFLICT", "#FF5C70"),
    (0.78, "5  RESOLVED", "#39D98A"),
    (1.00, "6  LINEAR HISTORY", "#39D98A"),
]


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()

    animation = Image.open(args.source)
    panel_width = 360
    frame_height = round(animation.height * panel_width / animation.width)
    title_height = 34
    panel_height = title_height + frame_height
    sheet = Image.new("RGB", (panel_width * 3, panel_height * 2), "#0B1020")
    draw = ImageDraw.Draw(sheet)
    font = ImageFont.truetype(
        "/usr/share/fonts/truetype/noto/NotoSans-Bold.ttf", 18
    )

    for slot, (position, title, color) in enumerate(PANELS):
        frame_index = round(position * (animation.n_frames - 1))
        animation.seek(frame_index)
        frame = animation.convert("RGB").resize(
            (panel_width, frame_height), Image.Resampling.LANCZOS
        )
        x = (slot % 3) * panel_width
        y = (slot // 3) * panel_height
        sheet.paste(frame, (x, y + title_height))
        draw.rectangle((x, y, x + panel_width - 1, y + title_height), fill="#151D33")
        draw.text((x + 12, y + 7), title, font=font, fill=color)
        draw.rectangle(
            (x, y, x + panel_width - 1, y + panel_height - 1),
            outline="#53617D",
            width=2,
        )

    args.output.parent.mkdir(parents=True, exist_ok=True)
    sheet.save(args.output, optimize=True)
    print(f"wrote {args.output}: {sheet.width}x{sheet.height}")


if __name__ == "__main__":
    main()

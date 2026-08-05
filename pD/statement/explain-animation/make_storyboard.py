import argparse
from pathlib import Path

from PIL import Image, ImageDraw


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()

    animation = Image.open(args.source)
    fractions = (0.23, 0.45, 0.92)
    panel_width = 420
    panel_height = round(animation.height * panel_width / animation.width)
    margin = 12
    sheet = Image.new(
        "RGB",
        (panel_width * 3 + margin * 4, panel_height + margin * 2),
        "#0B1020",
    )
    draw = ImageDraw.Draw(sheet)

    for slot, fraction in enumerate(fractions):
        frame_index = round(fraction * (animation.n_frames - 1))
        animation.seek(frame_index)
        frame = animation.convert("RGB").resize(
            (panel_width, panel_height),
            Image.Resampling.LANCZOS,
        )
        x = margin + slot * (panel_width + margin)
        sheet.paste(frame, (x, margin))
        draw.rounded_rectangle(
            (x, margin, x + panel_width - 1, margin + panel_height - 1),
            radius=8,
            outline="#53617D",
            width=3,
        )

    args.output.parent.mkdir(parents=True, exist_ok=True)
    sheet.save(args.output)
    print(
        f"wrote {args.output}: {sheet.width}x{sheet.height}, "
        f"frames={animation.n_frames}"
    )


if __name__ == "__main__":
    main()

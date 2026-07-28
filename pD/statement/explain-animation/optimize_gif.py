import argparse
from pathlib import Path

from PIL import Image, ImageColor


def optimize(source: Path, output: Path, width: int, fps: int, colors: int) -> None:
    rgb_frames = []
    if source.is_dir():
        frame_paths = sorted(source.glob("*.png"))
        if not frame_paths:
            raise ValueError(f"no PNG frames found in {source}")
        with Image.open(frame_paths[0]) as first_frame:
            height = round(first_frame.height * width / first_frame.width)
        output_duration = round(1000 / fps)
        for frame_path in frame_paths:
            with Image.open(frame_path) as frame:
                rgb_frames.append(
                    frame.convert("RGB").resize(
                        (width, height), Image.Resampling.LANCZOS
                    )
                )
    else:
        image = Image.open(source)
        source_duration = image.info.get("duration", 1000 / 24)
        target_duration = 1000 / fps
        step = max(1, round(target_duration / source_duration))
        output_duration = round(source_duration * step)
        height = round(image.height * width / image.width)
        for frame_index in range(0, image.n_frames, step):
            image.seek(frame_index)
            resized = image.convert("RGB").resize(
                (width, height), Image.Resampling.LANCZOS
            )
            rgb_frames.append(resized)

    # One palette for the whole animation avoids color shimmer and lets GIF
    # encode unchanged areas as compact delta frames.
    must_keep = [
        "#0B1020", "#151D33", "#EDF3FF", "#95A2C0", "#53617D",
        "#4C8DFF", "#B678FF", "#FFC857", "#FF5C70", "#39D98A",
    ]
    backdrops = [ImageColor.getrgb("#0B1020"), ImageColor.getrgb("#151D33")]
    design_colors = [ImageColor.getrgb(color) for color in must_keep]
    shade_steps = max(2, min(12, colors // (2 * len(design_colors))))

    palette_colors = []
    for backdrop in backdrops:
        for color in design_colors:
            for step_index in range(shade_steps + 1):
                alpha = step_index / shade_steps
                palette_colors.append(
                    tuple(
                        round(backdrop[channel] * (1 - alpha) + color[channel] * alpha)
                        for channel in range(3)
                    )
                )
    # Preserve order while removing duplicates, then pad to a legal GIF palette.
    palette_colors = list(dict.fromkeys(palette_colors))[:256]
    palette_data = [channel for color in palette_colors for channel in color]
    palette_data.extend([0] * (768 - len(palette_data)))
    palette = Image.new("P", (1, 1))
    palette.putpalette(palette_data)
    frames = [
        frame.quantize(palette=palette, dither=Image.Dither.NONE)
        for frame in rgb_frames
    ]

    output.parent.mkdir(parents=True, exist_ok=True)
    frames[0].save(
        output,
        save_all=True,
        append_images=frames[1:],
        duration=output_duration,
        loop=0,
        optimize=True,
        disposal=1,
    )

    with Image.open(output) as rendered:
        rendered_duration = 0
        rendered_frames = rendered.n_frames
        for frame_index in range(rendered_frames):
            rendered.seek(frame_index)
            rendered_duration += rendered.info.get("duration", 0)
    seconds = rendered_duration / 1000
    print(
        f"wrote {output}: {width}x{height}, {rendered_frames} frames, "
        f"{seconds:.2f}s, {colors} colors"
    )


def contact_sheet(source: Path, output: Path, columns: int = 3, rows: int = 2) -> None:
    image = Image.open(source)
    count = columns * rows
    indices = [
        round(i * (image.n_frames - 1) / (count - 1))
        for i in range(count)
    ]
    tile_width = 360
    tile_height = round(image.height * tile_width / image.width)
    sheet = Image.new("RGB", (tile_width * columns, tile_height * rows), "#0B1020")

    for slot, frame_index in enumerate(indices):
        image.seek(frame_index)
        frame = image.convert("RGB").resize(
            (tile_width, tile_height), Image.Resampling.LANCZOS
        )
        x = (slot % columns) * tile_width
        y = (slot // columns) * tile_height
        sheet.paste(frame, (x, y))

    output.parent.mkdir(parents=True, exist_ok=True)
    sheet.save(output, optimize=True)
    print(f"wrote {output}: sampled frames {indices}")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--width", type=int, default=720)
    parser.add_argument("--fps", type=int, default=12)
    parser.add_argument("--colors", type=int, default=128)
    parser.add_argument("--contact-sheet", type=Path)
    args = parser.parse_args()

    optimize(args.source, args.output, args.width, args.fps, args.colors)
    if args.contact_sheet:
        contact_sheet(args.output, args.contact_sheet)


if __name__ == "__main__":
    main()

import argparse
from pathlib import Path

from PIL import Image, ImageColor


BACKGROUND_COLORS = ["#0B1020", "#151D33"]
DESIGN_COLORS = [
    "#EDF3FF",
    "#95A2C0",
    "#53617D",
    "#4C8DFF",
    "#B678FF",
    "#FFC857",
    "#FF5C70",
    "#39D98A",
]


def make_palette(color_count: int) -> Image.Image:
    if not 32 <= color_count <= 256:
        raise ValueError("colors must be between 32 and 256")

    backgrounds = [ImageColor.getrgb(color) for color in BACKGROUND_COLORS]
    design = [ImageColor.getrgb(color) for color in DESIGN_COLORS]
    palette_colors = backgrounds + design

    shade_steps = 12
    for backdrop in backgrounds:
        for color in design:
            for step in range(1, shade_steps):
                alpha = step / shade_steps
                palette_colors.append(
                    tuple(
                        round(backdrop[channel] * (1 - alpha) + color[channel] * alpha)
                        for channel in range(3)
                    )
                )

    palette_colors = list(dict.fromkeys(palette_colors))[:color_count]
    palette_data = [channel for color in palette_colors for channel in color]
    palette_data.extend([0] * (768 - len(palette_data)))
    palette = Image.new("P", (1, 1))
    palette.putpalette(palette_data)
    return palette


def sample_indices(animation: Image.Image, fps: int) -> tuple[list[int], int]:
    durations = []
    for frame_index in range(animation.n_frames):
        animation.seek(frame_index)
        durations.append(animation.info.get("duration", round(1000 / 30)))

    total_duration = sum(durations)
    frame_count = max(1, round(total_duration * fps / 1000))
    output_duration = max(1, round(total_duration / frame_count))
    timestamps = [
        min(total_duration - 1, (frame_index + 0.5) * total_duration / frame_count)
        for frame_index in range(frame_count)
    ]

    indices = []
    source_index = 0
    source_end = durations[0]
    for timestamp in timestamps:
        while source_index + 1 < len(durations) and timestamp >= source_end:
            source_index += 1
            source_end += durations[source_index]
        indices.append(source_index)
    return indices, output_duration


def optimize(
    source: Path,
    output: Path,
    width: int,
    fps: int,
    colors: int,
) -> None:
    animation = Image.open(source)
    height = round(animation.height * width / animation.width)
    indices, output_duration = sample_indices(animation, fps)
    palette = make_palette(colors)

    frames = []
    for source_index in indices:
        animation.seek(source_index)
        frame = animation.convert("RGB").resize(
            (width, height),
            Image.Resampling.LANCZOS,
        )
        frames.append(frame.quantize(palette=palette, dither=Image.Dither.NONE))

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

    print(
        f"wrote {output}: {width}x{height}, {len(frames)} frames, "
        f"{output_duration} ms/frame, {colors} colors"
    )


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--width", type=int, default=512)
    parser.add_argument("--fps", type=int, default=12)
    parser.add_argument("--colors", type=int, default=128)
    args = parser.parse_args()
    optimize(args.source, args.output, args.width, args.fps, args.colors)


if __name__ == "__main__":
    main()

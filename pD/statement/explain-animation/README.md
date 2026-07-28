# Git workflow animation

This Manim scene visualizes the workflow used by the problem statement:

1. `main` and `feat/fancy` diverge, with two commits editing the same hunk.
2. Four feature commits are squashed into two commits.
3. The squashed commits are replayed onto `main`, one at a time.
4. The two conflicting commits flash red, then turn green when resolved.
5. The second replay finishes a clean linear history.

## Render the review GIF

Run these commands from the repository root. Rendering lossless PNG frames first
prevents GIF palette noise on the static dark background.

```bash
docker run --rm \
  -v "$PWD/pD/statement/explain-animation:/manim" \
  -w /manim \
  manimcommunity/manim:latest \
  manim render -r 720,405 --fps 12 -g -v warning \
  git_workflow.py GitWorkflowAnimation
```

```bash
docker run --rm \
  -v "$PWD/pD/statement/explain-animation:/manim" \
  -w /manim \
  manimcommunity/manim:latest \
  python optimize_gif.py media/images/git_workflow git-workflow.gif \
  --width 720 --fps 12 --colors 96
```

The final output is `git-workflow.gif`. The checked-in render is 720x405,
approximately 20.7 seconds long, and under 0.5 MB.

## Render the PDF storyboard

```bash
docker run --rm \
  -v "$PWD/pD/statement:/statement" \
  -w /statement/explain-animation \
  manimcommunity/manim:latest \
  python make_storyboard.py ../git-workflow.gif ../git-workflow-storyboard.png
```

The six-panel storyboard is the browser-independent fallback used directly in
the PDF. The full GIF remains attached for PDF viewers that support embedded
files, such as Firefox.

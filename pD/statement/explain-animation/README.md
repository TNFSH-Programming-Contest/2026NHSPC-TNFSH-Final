# Git explanation animations

The statement uses four independent Manim scenes:

1. BranchAnimation
2. SquashAnimation
3. RebaseAnimation
4. ConflictAnimation

Each scene is first rendered at 1280x720 and 30 FPS. The downloadable copy is
then converted to 512x288, uniformly sampled at 12 FPS, and quantized with one
shared 128-color palette. The full-resolution PNG storyboards remain in the
PDF, while the downloadable animations stay reasonably clear and compact.

Run from the repository root:

    docker run --rm -v "$PWD/pD/statement:/statement" -w /statement/explain-animation manimcommunity/manim:latest manim render -r 1280,720 --fps 30 --format=gif -v warning git_workflow.py BranchAnimation SquashAnimation RebaseAnimation ConflictAnimation

Generate the lossless PNG storyboards from the direct Manim renders:

    python make_storyboard.py media/videos/git_workflow/720p30/BranchAnimation*.gif ../branch-storyboard.png
    python make_storyboard.py media/videos/git_workflow/720p30/SquashAnimation*.gif ../squash-storyboard.png
    python make_storyboard.py media/videos/git_workflow/720p30/RebaseAnimation*.gif ../rebase-storyboard.png
    python make_storyboard.py media/videos/git_workflow/720p30/ConflictAnimation*.gif ../conflict-storyboard.png

Create the smaller downloadable GIF attachments:

    python optimize_gif.py media/videos/git_workflow/720p30/BranchAnimation*.gif ../branch.gif
    python optimize_gif.py media/videos/git_workflow/720p30/SquashAnimation*.gif ../squash.gif
    python optimize_gif.py media/videos/git_workflow/720p30/RebaseAnimation*.gif ../rebase.gif
    python optimize_gif.py media/videos/git_workflow/720p30/ConflictAnimation*.gif ../conflict.gif

The PDF displays the four PNG storyboards and embeds the four optimized GIFs.
It also keeps the original combined `git-workflow.gif` as a compact overview.

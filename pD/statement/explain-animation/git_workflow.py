from manim import *


BG = "#0B1020"
PANEL = "#151D33"
TEXT = "#EDF3FF"
MUTED = "#95A2C0"
EDGE = "#53617D"
MAIN = "#4C8DFF"
FEATURE = "#B678FF"
GOLD = "#FFC857"
RED = "#FF5C70"
GREEN = "#39D98A"


class GitScene(Scene):
    def setup(self):
        self.camera.background_color = BG

    def header(self, title, subtitle, color):
        title_text = Text(title, font_size=38, weight=BOLD, color=TEXT)
        title_text.to_edge(UP, buff=0.28)
        subtitle_text = Text(subtitle, font_size=20, color=MUTED)
        subtitle_text.next_to(title_text, DOWN, buff=0.10)
        line = Line(LEFT * 6.6, RIGHT * 6.6, color=color, stroke_width=2)
        line.next_to(subtitle_text, DOWN, buff=0.18)
        return VGroup(title_text, subtitle_text, line)

    def commit(self, tag, caption, point, color, label_direction=DOWN):
        body = Circle(
            radius=0.34,
            stroke_color=color,
            stroke_width=4,
            fill_color=interpolate_color(ManimColor(color), ManimColor(BG), 0.18),
            fill_opacity=1,
        )
        tag_text = Text(tag, font_size=19, weight=BOLD, color=TEXT).move_to(body)
        caption_text = Text(caption, font_size=17, color=MUTED)
        caption_text.next_to(body, label_direction, buff=0.16)
        node = VGroup(body, tag_text, caption_text).move_to(point)
        node.body = body
        node.set_z_index(4)
        return node

    def edge(self, left, right, color=EDGE, dashed=False):
        edge_type = DashedLine if dashed else Line
        line = edge_type(
            left.body.get_center(),
            right.body.get_center(),
            buff=0.34,
            color=color,
            stroke_width=4,
        )
        line.set_z_index(1)
        return line

    def branch_badge(self, name, color, point):
        label = Text(name, font_size=20, weight=BOLD, color=color)
        box = RoundedRectangle(
            width=label.width + 0.48,
            height=0.50,
            corner_radius=0.12,
            stroke_color=color,
            stroke_width=2,
            fill_color=PANEL,
            fill_opacity=1,
        )
        return VGroup(box, label.move_to(box)).move_to(point)

    def status(self, text, color, point=DOWN * 3.15):
        label = Text(text, font_size=23, weight=BOLD, color=color)
        box = RoundedRectangle(
            width=label.width + 0.65,
            height=0.64,
            corner_radius=0.15,
            stroke_color=color,
            stroke_width=2,
            fill_color=PANEL,
            fill_opacity=0.98,
        )
        return VGroup(box, label.move_to(box)).move_to(point).set_z_index(8)


class BranchAnimation(GitScene):
    def construct(self):
        header = self.header(
            "BRANCH",
            "Two histories continue from the same commit",
            MAIN,
        )
        self.play(FadeIn(header, shift=DOWN * 0.12), run_time=0.7)

        main_y = -1.35
        feature_y = 1.05
        main_badge = self.branch_badge("v2.0", MAIN, [-6.15, main_y, 0])
        feature_badge = self.branch_badge(
            "feat/fancy", FEATURE, [-5.85, feature_y, 0]
        )
        m0 = self.commit("M0", "initial", [-4.65, main_y, 0], MAIN)
        base = self.commit("B", "branch point", [-3.05, main_y, 0], MAIN)
        m1 = self.commit("M1", "main update", [-0.95, main_y, 0], MAIN)
        f1 = self.commit("F1", "feature 1", [-1.85, feature_y, 0], FEATURE, UP)
        f2 = self.commit("F2", "feature 2", [0.25, feature_y, 0], FEATURE, UP)
        f3 = self.commit("F3", "feature 3", [2.35, feature_y, 0], FEATURE, UP)

        main_edges = VGroup(self.edge(m0, base), self.edge(base, m1))
        feature_edges = VGroup(
            self.edge(base, f1, FEATURE),
            self.edge(f1, f2, FEATURE),
            self.edge(f2, f3, FEATURE),
        )
        self.play(FadeIn(main_badge), FadeIn(feature_badge), run_time=0.4)
        self.play(
            LaggedStart(
                GrowFromCenter(m0),
                Create(main_edges[0]),
                GrowFromCenter(base),
                Create(main_edges[1]),
                GrowFromCenter(m1),
                lag_ratio=0.20,
            ),
            run_time=1.35,
        )
        self.play(
            LaggedStart(
                Create(feature_edges[0]),
                GrowFromCenter(f1),
                Create(feature_edges[1]),
                GrowFromCenter(f2),
                Create(feature_edges[2]),
                GrowFromCenter(f3),
                lag_ratio=0.18,
            ),
            run_time=1.55,
        )
        note = self.status("THE BRANCHES HAVE DIVERGED", FEATURE)
        self.play(FadeIn(note, shift=UP * 0.12), run_time=0.5)
        self.wait(1.1)


class SquashAnimation(GitScene):
    def construct(self):
        header = self.header(
            "SQUASH",
            "Several consecutive commits become one commit",
            GOLD,
        )
        self.play(FadeIn(header, shift=DOWN * 0.12), run_time=0.7)

        points = [-5.1, -2.75, -0.4, 2.15, 4.5]
        commits = [
            self.commit(f"F{i + 1}", f"operation {i + 1}", [x, 0.65, 0], FEATURE)
            for i, x in enumerate(points)
        ]
        edges = VGroup(
            *[self.edge(commits[i], commits[i + 1], FEATURE) for i in range(4)]
        )
        sequence = []
        for i, node in enumerate(commits):
            sequence.append(GrowFromCenter(node))
            if i < len(edges):
                sequence.append(Create(edges[i]))
        self.play(
            LaggedStart(
                *sequence,
                lag_ratio=0.10,
            ),
            run_time=1.5,
        )

        group1 = SurroundingRectangle(
            VGroup(*commits[:3]),
            buff=0.25,
            color=GOLD,
            stroke_width=3,
            corner_radius=0.15,
        )
        group2 = SurroundingRectangle(
            VGroup(*commits[3:]),
            buff=0.25,
            color=GOLD,
            stroke_width=3,
            corner_radius=0.15,
        )
        self.play(Create(group1), Create(group2), run_time=0.55)
        self.wait(0.35)

        first_point = np.array([-2.2, -1.55, 0])
        second_point = np.array([2.2, -1.55, 0])
        self.play(
            *[
                node.animate.move_to(first_point).scale(0.38).set_opacity(0)
                for node in commits[:3]
            ],
            *[
                node.animate.move_to(second_point).scale(0.38).set_opacity(0)
                for node in commits[3:]
            ],
            FadeOut(edges),
            FadeOut(group1),
            FadeOut(group2),
            run_time=1.15,
        )
        self.remove(*commits)

        s1 = self.commit("S1", "F1 + F2 + F3", first_point, GOLD, UP)
        s2 = self.commit("S2", "F4 + F5", second_point, GOLD, UP)
        result_edge = self.edge(s1, s2, GOLD)
        self.play(
            GrowFromCenter(s1),
            GrowFromCenter(s2),
            Create(result_edge),
            run_time=0.75,
        )
        note = self.status("5 COMMITS  →  2 SQUASH COMMITS", GOLD)
        self.play(FadeIn(note, shift=UP * 0.12), run_time=0.45)
        self.wait(1.1)


class RebaseAnimation(GitScene):
    def construct(self):
        header = self.header(
            "REBASE",
            "Replay squash commits onto the new branch, one by one",
            FEATURE,
        )
        self.play(FadeIn(header, shift=DOWN * 0.12), run_time=0.7)

        main_y = -1.35
        feature_y = 1.05
        m0 = self.commit("M0", "base", [-5.0, main_y, 0], MAIN)
        base = self.commit("B", "branch point", [-3.45, main_y, 0], MAIN)
        m1 = self.commit("M1", "new main", [-1.55, main_y, 0], MAIN)
        s1 = self.commit("S1", "squash 1", [-1.35, feature_y, 0], GOLD, UP)
        s2 = self.commit("S2", "squash 2", [0.75, feature_y, 0], GOLD, UP)
        main_edges = VGroup(self.edge(m0, base), self.edge(base, m1))
        feature_edges = VGroup(
            self.edge(base, s1, FEATURE),
            self.edge(s1, s2, FEATURE),
        )
        self.add(m0, base, m1, s1, s2, main_edges, feature_edges)
        self.wait(0.6)

        replay1 = s1.copy()
        self.add(replay1)
        destination1 = np.array([0.65, main_y, 0])
        path1 = ArcBetweenPoints(replay1.get_center(), destination1, angle=-PI / 2.2)
        self.play(MoveAlongPath(replay1, path1), run_time=1.15, rate_func=smooth)
        replay1_target = self.commit("S1′", "replayed first", destination1, GREEN)
        self.play(Transform(replay1, replay1_target), run_time=0.40)
        replay1_edge = self.edge(m1, replay1, GREEN)
        self.play(Create(replay1_edge), run_time=0.30)
        self.play(s1.animate.set_opacity(0.25), run_time=0.25)

        replay2 = s2.copy()
        self.add(replay2)
        destination2 = np.array([3.0, main_y, 0])
        path2 = ArcBetweenPoints(replay2.get_center(), destination2, angle=-PI / 2.2)
        self.play(MoveAlongPath(replay2, path2), run_time=1.15, rate_func=smooth)
        replay2_target = self.commit("S2′", "replayed second", destination2, GREEN)
        self.play(Transform(replay2, replay2_target), run_time=0.40)
        replay2_edge = self.edge(replay1, replay2, GREEN)
        self.play(
            Create(replay2_edge),
            FadeOut(feature_edges),
            FadeOut(s1),
            FadeOut(s2),
            run_time=0.65,
        )
        note = self.status("REBASE COMPLETE", GREEN)
        self.play(FadeIn(note, shift=UP * 0.12), run_time=0.45)
        self.wait(1.1)


class ConflictAnimation(GitScene):
    def construct(self):
        header = self.header(
            "CONFLICT",
            "A replayed change disagrees with the current content",
            RED,
        )
        self.play(FadeIn(header, shift=DOWN * 0.12), run_time=0.7)

        current = self.commit("M", "current", [-2.25, 0.25, 0], MAIN)
        replay = self.commit("S′", "old  →  new", [2.25, 0.25, 0], FEATURE)
        connection = DashedLine(
            current.body.get_center(),
            replay.body.get_center(),
            buff=0.34,
            color=RED,
            stroke_width=4,
        )
        condition = self.status(
            "current  ∉  { old, new }",
            RED,
            point=UP * 1.75,
        )
        conflict = self.status("CONFLICT", RED)
        self.play(
            GrowFromCenter(current),
            GrowFromCenter(replay),
            Create(connection),
            FadeIn(condition),
            run_time=0.8,
        )
        self.play(FadeIn(conflict, scale=0.85), run_time=0.35)
        for _ in range(2):
            self.play(
                Indicate(current.body, color=RED, scale_factor=1.32),
                Indicate(replay.body, color=RED, scale_factor=1.32),
                run_time=0.48,
            )
        self.play(
            current.body.animate.set_stroke(RED, width=6).set_fill(RED, opacity=0.88),
            replay.body.animate.set_stroke(RED, width=6).set_fill(RED, opacity=0.88),
            run_time=0.28,
        )
        self.wait(0.35)

        resolved = self.status("RESOLVED  ·  current  ←  new", GREEN)
        resolved_condition = self.status(
            "current  =  new",
            GREEN,
            point=UP * 1.75,
        )
        self.play(
            Transform(conflict, resolved),
            Transform(condition, resolved_condition),
            connection.animate.set_color(GREEN),
            current.body.animate.set_stroke(GREEN, width=5).set_fill(
                interpolate_color(ManimColor(GREEN), ManimColor(BG), 0.12),
                opacity=1,
            ),
            replay.body.animate.set_stroke(GREEN, width=5).set_fill(
                interpolate_color(ManimColor(GREEN), ManimColor(BG), 0.12),
                opacity=1,
            ),
            run_time=0.75,
        )
        self.wait(1.25)

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


class GitWorkflowAnimation(Scene):
    def make_commit(self, tag, caption, point, color, label_direction=DOWN):
        body = Circle(
            radius=0.31,
            stroke_color=color,
            stroke_width=3,
            fill_color=interpolate_color(ManimColor(color), ManimColor(BG), 0.18),
            fill_opacity=1,
        )
        tag_text = Text(tag, font_size=18, weight=BOLD, color=TEXT)
        tag_text.move_to(body)
        caption_text = Text(caption, font_size=17, color=MUTED)
        caption_text.next_to(body, label_direction, buff=0.17)
        node = VGroup(body, tag_text, caption_text).move_to(point)
        node.body = body
        node.tag_text = tag_text
        node.caption_text = caption_text
        node.set_z_index(4)
        return node

    def edge(self, left, right, color=EDGE, dashed=False):
        edge_type = DashedLine if dashed else Line
        line = edge_type(
            left.body.get_center(),
            right.body.get_center(),
            buff=0.31,
            color=color,
            stroke_width=4 if not dashed else 3,
        )
        line.set_z_index(1)
        return line

    def branch_badge(self, name, color):
        text = Text(name, font_size=20, weight=BOLD, color=color)
        box = RoundedRectangle(
            width=text.width + 0.42,
            height=0.47,
            corner_radius=0.12,
            stroke_color=color,
            stroke_width=2,
            fill_color=PANEL,
            fill_opacity=1,
        )
        return VGroup(box, text.move_to(box))

    def stage_badge(self, number, label, color):
        number_text = Text(number, font_size=17, weight=BOLD, color=BG)
        number_box = RoundedRectangle(
            width=0.52,
            height=0.38,
            corner_radius=0.10,
            stroke_width=0,
            fill_color=color,
            fill_opacity=1,
        )
        number_text.move_to(number_box)
        label_text = Text(label, font_size=18, weight=BOLD, color=TEXT)
        content = VGroup(VGroup(number_box, number_text), label_text).arrange(
            RIGHT, buff=0.16
        )
        panel = RoundedRectangle(
            width=content.width + 0.34,
            height=0.54,
            corner_radius=0.13,
            stroke_color=EDGE,
            stroke_width=1.5,
            fill_color=PANEL,
            fill_opacity=0.96,
        )
        content.move_to(panel)
        return VGroup(panel, content).to_corner(UR, buff=0.42)

    def recolor(self, node, color):
        node.body.set_stroke(color, width=4)
        node.body.set_fill(
            interpolate_color(ManimColor(color), ManimColor(BG), 0.12),
            opacity=1,
        )

    def construct(self):
        self.camera.background_color = BG

        title = Text(
            "MERGE  ·  SQUASH  ·  REBASE",
            font_size=36,
            weight=BOLD,
            color=TEXT,
        ).to_edge(UP, buff=0.32)
        subtitle = Text(
            "Turn a feature branch into a clean, linear history",
            font_size=19,
            color=MUTED,
        ).next_to(title, DOWN, buff=0.12)
        divider = Line(LEFT * 6.7, RIGHT * 6.7, color=EDGE, stroke_width=1)
        divider.next_to(subtitle, DOWN, buff=0.20)

        stage = self.stage_badge("01", "BRANCH", MAIN)
        self.play(
            FadeIn(title, shift=DOWN * 0.15),
            FadeIn(subtitle, shift=DOWN * 0.12),
            Create(divider),
            FadeIn(stage, shift=LEFT * 0.15),
            run_time=0.9,
        )

        main_y = -1.25
        feature_y = 1.05
        main_badge = self.branch_badge("v2.0", MAIN).move_to([-6.25, main_y, 0])
        feature_badge = self.branch_badge("feat/fancy", FEATURE).move_to(
            [-5.95, feature_y, 0]
        )

        b0 = self.make_commit("M0", "initial", [-4.75, main_y, 0], MAIN)
        b1 = self.make_commit("M1", "branch point", [-3.35, main_y, 0], MAIN)
        m2 = self.make_commit("M2", "hotfix · hunk A", [-1.25, main_y, 0], MAIN)

        f1 = self.make_commit("F1", "UI", [-2.35, feature_y, 0], FEATURE, UP)
        f2 = self.make_commit(
            "F2", "edit hunk A", [-0.65, feature_y, 0], FEATURE, UP
        )
        f3 = self.make_commit("F3", "tests", [1.05, feature_y, 0], FEATURE, UP)
        f4 = self.make_commit("F4", "docs", [2.75, feature_y, 0], FEATURE, UP)

        main_edges = VGroup(self.edge(b0, b1), self.edge(b1, m2))
        feature_edges = VGroup(
            self.edge(b1, f1, FEATURE),
            self.edge(f1, f2, FEATURE),
            self.edge(f2, f3, FEATURE),
            self.edge(f3, f4, FEATURE),
        )

        self.play(FadeIn(main_badge), FadeIn(feature_badge), run_time=0.45)
        self.play(
            LaggedStart(
                GrowFromCenter(b0),
                Create(main_edges[0]),
                GrowFromCenter(b1),
                Create(main_edges[1]),
                GrowFromCenter(m2),
                lag_ratio=0.22,
            ),
            run_time=1.45,
        )
        self.play(
            LaggedStart(
                Create(feature_edges[0]),
                GrowFromCenter(f1),
                Create(feature_edges[1]),
                GrowFromCenter(f2),
                Create(feature_edges[2]),
                GrowFromCenter(f3),
                Create(feature_edges[3]),
                GrowFromCenter(f4),
                lag_ratio=0.14,
            ),
            run_time=1.75,
        )

        conflict_hint = self.edge(m2, f2, RED, dashed=True)
        hint_text = Text(
            "both commits edit hunk A",
            font_size=18,
            weight=BOLD,
            color=RED,
        ).move_to([0.12, -0.05, 0])
        hint_panel = RoundedRectangle(
            width=hint_text.width + 0.38,
            height=0.46,
            corner_radius=0.10,
            stroke_color=RED,
            stroke_width=1.5,
            fill_color=PANEL,
            fill_opacity=0.96,
        ).move_to(hint_text)
        hint = VGroup(hint_panel, hint_text)
        self.play(Create(conflict_hint), FadeIn(hint, scale=0.9), run_time=0.65)
        self.play(
            Indicate(m2.body, color=RED, scale_factor=1.22),
            Indicate(f2.body, color=RED, scale_factor=1.22),
            run_time=0.65,
        )
        self.wait(0.45)
        self.play(FadeOut(conflict_hint), FadeOut(hint), run_time=0.4)

        squash_stage = self.stage_badge("02", "SQUASH", GOLD)
        self.play(Transform(stage, squash_stage), run_time=0.45)

        group1 = SurroundingRectangle(
            VGroup(f1.body, f2.body),
            buff=0.24,
            color=GOLD,
            stroke_width=2.5,
            corner_radius=0.14,
        )
        group2 = SurroundingRectangle(
            VGroup(f3.body, f4.body),
            buff=0.24,
            color=GOLD,
            stroke_width=2.5,
            corner_radius=0.14,
        )
        squash_label1 = Text("SQUASH", font_size=15, weight=BOLD, color=GOLD)
        squash_label2 = squash_label1.copy()
        squash_label1.next_to(group1, DOWN, buff=0.10)
        squash_label2.next_to(group2, DOWN, buff=0.10)
        self.play(
            Create(group1),
            Create(group2),
            FadeIn(squash_label1),
            FadeIn(squash_label2),
            run_time=0.65,
        )

        s1 = self.make_commit("S1", "F1 + F2", [-1.35, feature_y, 0], GOLD, UP)
        s2 = self.make_commit("S2", "F3 + F4", [1.75, feature_y, 0], GOLD, UP)

        self.play(
            FadeOut(feature_edges),
            FadeOut(group1),
            FadeOut(group2),
            FadeOut(squash_label1),
            FadeOut(squash_label2),
            f1.animate.move_to(s1.body).scale(0.55).set_opacity(0),
            f2.animate.move_to(s1.body).scale(0.55).set_opacity(0),
            f3.animate.move_to(s2.body).scale(0.55).set_opacity(0),
            f4.animate.move_to(s2.body).scale(0.55).set_opacity(0),
            FadeIn(s1, scale=0.35),
            FadeIn(s2, scale=0.35),
            run_time=1.25,
        )
        self.remove(f1, f2, f3, f4)

        squashed_edges = VGroup(
            self.edge(b1, s1, GOLD),
            self.edge(s1, s2, GOLD),
        )
        self.play(
            Create(squashed_edges[0]),
            Create(squashed_edges[1]),
            Indicate(s1.body, color=GOLD, scale_factor=1.18),
            Indicate(s2.body, color=GOLD, scale_factor=1.18),
            run_time=0.85,
        )
        squash_note = Text(
            "4 commits  →  2 squash commits",
            font_size=21,
            weight=BOLD,
            color=GOLD,
        ).to_edge(DOWN, buff=0.40)
        self.play(FadeIn(squash_note, shift=UP * 0.12), run_time=0.4)
        self.wait(0.65)
        self.play(FadeOut(squash_note), run_time=0.3)

        rebase_stage = self.stage_badge("03", "REBASE · REPLAY 1/2", FEATURE)
        self.play(Transform(stage, rebase_stage), run_time=0.45)

        replay1 = s1.copy()
        self.add(replay1)
        destination1 = np.array([0.55, main_y, 0])
        path1 = ArcBetweenPoints(
            replay1.get_center(), destination1, angle=-PI / 2.3, color=FEATURE
        )
        moving_arrow1 = Arrow(
            path1.point_from_proportion(0.25),
            path1.point_from_proportion(0.64),
            color=FEATURE,
            buff=0,
            stroke_width=3,
            max_tip_length_to_length_ratio=0.16,
        )
        self.play(Create(moving_arrow1), run_time=0.35)
        self.play(MoveAlongPath(replay1, path1), run_time=1.25, rate_func=smooth)
        self.play(FadeOut(moving_arrow1), s1.animate.set_opacity(0.25), run_time=0.3)

        replay1_target = self.make_commit(
            "S1′", "F1 + F2 · replayed", destination1, FEATURE
        )
        self.play(Transform(replay1, replay1_target), run_time=0.35)
        main_to_s1 = self.edge(m2, replay1, FEATURE)
        self.play(Create(main_to_s1), run_time=0.35)

        conflict_word = Text(
            "CONFLICT",
            font_size=27,
            weight=BOLD,
            color=RED,
        ).move_to([-0.35, -0.18, 0])
        conflict_panel = RoundedRectangle(
            width=conflict_word.width + 0.55,
            height=0.60,
            corner_radius=0.13,
            stroke_color=RED,
            stroke_width=2,
            fill_color=PANEL,
            fill_opacity=0.97,
        ).move_to(conflict_word)
        conflict_callout = VGroup(conflict_panel, conflict_word).set_z_index(8)
        self.play(FadeIn(conflict_callout, scale=0.82), run_time=0.35)
        for _ in range(2):
            self.play(
                Indicate(m2.body, color=RED, scale_factor=1.32),
                Indicate(replay1.body, color=RED, scale_factor=1.32),
                run_time=0.48,
            )
        self.play(
            m2.body.animate.set_stroke(RED, width=5).set_fill(RED, opacity=0.88),
            replay1.body.animate.set_stroke(RED, width=5).set_fill(
                RED, opacity=0.88
            ),
            run_time=0.28,
        )
        self.wait(0.30)

        resolved_word = Text(
            "CONFLICT RESOLVED",
            font_size=23,
            weight=BOLD,
            color=GREEN,
        ).move_to(conflict_word)
        resolved_panel = RoundedRectangle(
            width=resolved_word.width + 0.55,
            height=0.60,
            corner_radius=0.13,
            stroke_color=GREEN,
            stroke_width=2,
            fill_color=PANEL,
            fill_opacity=0.97,
        ).move_to(resolved_word)
        resolved_callout = VGroup(resolved_panel, resolved_word).set_z_index(8)
        self.play(
            Transform(conflict_callout, resolved_callout),
            m2.body.animate.set_stroke(GREEN, width=4).set_fill(
                interpolate_color(ManimColor(GREEN), ManimColor(BG), 0.12),
                opacity=1,
            ),
            replay1.body.animate.set_stroke(GREEN, width=4).set_fill(
                interpolate_color(ManimColor(GREEN), ManimColor(BG), 0.12),
                opacity=1,
            ),
            run_time=0.70,
        )
        self.wait(0.55)
        self.play(FadeOut(conflict_callout), run_time=0.3)

        rebase_stage2 = self.stage_badge("03", "REBASE · REPLAY 2/2", FEATURE)
        self.play(Transform(stage, rebase_stage2), run_time=0.4)

        replay2 = s2.copy()
        self.add(replay2)
        destination2 = np.array([2.75, main_y, 0])
        path2 = ArcBetweenPoints(
            replay2.get_center(), destination2, angle=-PI / 2.25, color=FEATURE
        )
        moving_arrow2 = Arrow(
            path2.point_from_proportion(0.22),
            path2.point_from_proportion(0.62),
            color=FEATURE,
            buff=0,
            stroke_width=3,
            max_tip_length_to_length_ratio=0.16,
        )
        self.play(Create(moving_arrow2), run_time=0.3)
        self.play(MoveAlongPath(replay2, path2), run_time=1.20, rate_func=smooth)
        self.play(FadeOut(moving_arrow2), run_time=0.25)

        replay2_target = self.make_commit(
            "S2′", "F3 + F4 · replayed", destination2, GREEN
        )
        self.play(Transform(replay2, replay2_target), run_time=0.35)
        main_to_s2 = self.edge(replay1, replay2, GREEN)
        self.play(
            Create(main_to_s2),
            FadeOut(squashed_edges),
            FadeOut(s1),
            FadeOut(s2),
            FadeOut(feature_badge),
            run_time=0.65,
        )

        done_stage = self.stage_badge("04", "DONE", GREEN)
        self.play(Transform(stage, done_stage), run_time=0.4)
        final_banner_text = Text(
            "REBASE COMPLETE  ·  CLEAN LINEAR HISTORY",
            font_size=24,
            weight=BOLD,
            color=GREEN,
        )
        final_banner_panel = RoundedRectangle(
            width=final_banner_text.width + 0.65,
            height=0.66,
            corner_radius=0.16,
            stroke_color=GREEN,
            stroke_width=2,
            fill_color=PANEL,
            fill_opacity=0.98,
        )
        final_banner = VGroup(
            final_banner_panel, final_banner_text.move_to(final_banner_panel)
        ).to_edge(DOWN, buff=0.35)
        self.play(
            FadeIn(final_banner, shift=UP * 0.15),
            main_badge[0].animate.set_stroke(GREEN),
            main_badge[1].animate.set_color(GREEN),
            Indicate(replay2.body, color=GREEN, scale_factor=1.18),
            run_time=0.75,
        )
        self.wait(1.8)

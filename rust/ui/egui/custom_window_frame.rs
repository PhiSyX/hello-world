#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

use eframe::egui;

fn main() {
    let options = eframe::NativeOptions {
        decorated: false,
        transparent: true,
        resizable: true,
        min_window_size: Some(egui::vec2(800.0, 600.0)),
        ..Default::default()
    };
    eframe::run_native(
        "Custom window frame",
        options,
        Box::new(|_cc| Box::new(MyApp::default())),
    );
}

#[derive(Default)]
struct MyApp {}

impl eframe::App for MyApp {
    fn clear_color(&self, _visuals: &egui::Visuals) -> egui::Rgba {
        egui::Rgba::TRANSPARENT
    }

    fn update(&mut self, ctx: &egui::Context, frame: &mut eframe::Frame) {
        custon_window_frame(ctx, frame, "Window Title", |ui| {
            ui.label("C'est juste du contenu pour la fenêtre");
            ui.horizontal(|ui| {
                ui.label("Theme:");
                egui::widgets::global_dark_light_mode_buttons(ui);
            });
        });
    }
}

fn custon_window_frame(
    ctx: &egui::Context,
    frame: &mut eframe::Frame,
    title: &str,
    add_contents: impl FnOnce(&mut egui::Ui),
) {
    use egui::*;

    let mut style: egui::Style = (*ctx.style()).clone();
    style.visuals.widgets.noninteractive.fg_stroke.color =
        Color32::from_rgb(255, 255, 255);
    style.visuals.widgets.noninteractive.bg_fill =
        Color32::from_rgb(29, 33, 37);
    ctx.set_style(style);

    let text_color = ctx.style().visuals.text_color();

    let height = 40.0;

    CentralPanel::default()
        .frame(Frame::none())
        .show(ctx, |ui| {
            let rect = ui.max_rect();
            let painter = ui.painter();

            painter.rect(
                rect.shrink(1.0),
                8.0,
                ctx.style().visuals.window_fill(),
                Stroke::new(1.0, text_color),
            );

            painter.text(
                rect.center_top() + vec2(0.0, height / 2.0),
                Align2::CENTER_CENTER,
                title,
                FontId::proportional(height - 2.0),
                text_color,
            );

            painter.line_segment(
                [
                    rect.left_top() + vec2(2.0, height),
                    rect.right_top() + vec2(-2.0, height),
                ],
                Stroke::new(1.0, text_color),
            );

            let close_response = ui.put(
                Rect::from_min_size(rect.left_top(), Vec2::splat(height)),
                Button::new(RichText::new("❌").size(height - 4.0))
                    .frame(false),
            );
            if close_response.clicked() {
                println!("ok");
                frame.quit();
            }

            let title_bar_rect = {
                let mut rect = rect;
                rect.max.y = rect.min.y + height;
                rect
            };
            let title_bar_response = ui.interact(
                title_bar_rect,
                Id::new("title_bar"),
                Sense::drag(),
            );
            if title_bar_response.drag_started() {
                frame.drag_window();
            }

            let content_rect = {
                let mut rect = rect;
                rect.min.y = title_bar_rect.max.y;
                rect
            }
            .shrink(4.0);
            let mut content_ui = ui.child_ui(content_rect, *ui.layout());
            add_contents(&mut content_ui);
        });
}

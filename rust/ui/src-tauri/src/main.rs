#![cfg_attr(
    all(not(debug_assertions), target_os = "windows"),
    windows_subsystem = "windows"
)]

use std::process::{Command, Stdio};

fn main() {
    tauri::Builder::default()
        .invoke_handler(tauri::generate_handler![call_for_test_executable])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}

#[derive(serde::Serialize)]
struct CommandOutput {
    program: &'static str,
    status: String,
    output: Vec<String>,
}

#[derive(serde::Serialize)]
enum CommandError {
    UnableToRunProgram,
    UnableToGetUtf8String,
}

#[tauri::command]
fn call_for_test_executable() -> Result<CommandOutput, CommandError> {
    let executable = "for-test.exe";

    let output = Command::new(executable)
        .stdout(Stdio::piped())
        .output()
        .map_err(|_| CommandError::UnableToRunProgram)?;


    let status = output.status.to_string();
    let stdout = String::from_utf8(output.stdout)
        .map_err(|_| CommandError::UnableToGetUtf8String)?
        .split('\n')
        .map(|a| a.to_string())
        .collect::<Vec<String>>();

    Ok(CommandOutput {
        program: executable,
        output: stdout,
        status,
    })
}

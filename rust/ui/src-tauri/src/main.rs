#![cfg_attr(
    all(not(debug_assertions), target_os = "windows"),
    windows_subsystem = "windows"
)]

use std::{
    io::Read,
    process::{Command, Stdio},
};

fn main() {
    tauri::Builder::default()
        .invoke_handler(tauri::generate_handler![call_for_test_executable])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}

#[derive(serde::Serialize)]
struct CommandOutput {
    pid: u32,
    program: &'static str,
    output: Vec<String>,
}

#[derive(serde::Serialize)]
enum CommandError {
    UnableToRunProgram,
    FailedToOpenStdout,
}

#[tauri::command]
fn call_for_test_executable() -> Result<CommandOutput, CommandError> {
    let executable = "for-test.exe";

    let child = Command::new(executable)
        .stdout(Stdio::piped())
        .spawn()
        .map_err(|_| CommandError::UnableToRunProgram)?;

    let pid = child.id();

    let mut output =
        child.stdout.ok_or(CommandError::FailedToOpenStdout)?;
    let mut buf = String::new();
    let _ = output.by_ref().read_to_string(&mut buf);

    let stdout = buf
        .split('\n')
        .map(|a| a.to_string())
        .collect::<Vec<String>>();

    Ok(CommandOutput {
        program: executable,
        output: stdout,
        pid,
    })
}

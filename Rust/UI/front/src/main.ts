import './style.css'

import { appWindow } from '@tauri-apps/api/window'
import { invoke } from '@tauri-apps/api'

document
    .getElementById('title-bar-minimize')
    .addEventListener('click', () => appWindow.minimize())
document
    .getElementById('title-bar-maximize')
    .addEventListener('click', () => appWindow.toggleMaximize())
document
    .getElementById('title-bar-close')
    .addEventListener('click', () => appWindow.close())

let $btn = document.querySelector("#my-btn");
let $output = document.querySelector("output");

$btn?.addEventListener("click", () => {
    $output.textContent = "";
    invoke("call_for_test_executable").then((payload) => {
        console.log(payload);

        payload.output.forEach((line) => {
            let $p = document.createElement("p");
            $p.textContent = line;
            $output?.appendChild($p);
        });
    });
});

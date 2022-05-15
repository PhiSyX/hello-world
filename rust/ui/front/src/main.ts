import './style.css'

import { appWindow } from '@tauri-apps/api/window'

document
    .getElementById('title-bar-minimize')
    .addEventListener('click', () => appWindow.minimize())
document
    .getElementById('title-bar-maximize')
    .addEventListener('click', () => appWindow.toggleMaximize())
document
    .getElementById('title-bar-close')
    .addEventListener('click', () => appWindow.close())

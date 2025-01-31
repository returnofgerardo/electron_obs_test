/**
 * This file is loaded via the <script> tag in the index.html file and will
 * be executed in the renderer process for that window. No Node.js APIs are
 * available in this process because `nodeIntegration` is turned off and
 * `contextIsolation` is turned on. Use the contextBridge API in `preload.js`
 * to expose Node.js functionality from the main process.
 */


const startupBtn = document.getElementById('startup_btn');
const resetVidBtn = document.getElementById('resetv_btn');

startupBtn.addEventListener('click', async (event) => {
    event.target.disabled = true;
    const result = await window.ipc.obsStartup();
    if (!result)
    {
        event.target.disabled = false;
    }
});

resetVidBtn.addEventListener('click', async (event) => {
    event.target.disabled = true;
    const result = await window.ipc.obsResetVideo();
    if (!result)
    {
        event.target.disabled = false;
    }
});

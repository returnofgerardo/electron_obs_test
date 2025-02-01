/**
 * This file is loaded via the <script> tag in the index.html file and will
 * be executed in the renderer process for that window. No Node.js APIs are
 * available in this process because `nodeIntegration` is turned off and
 * `contextIsolation` is turned on. Use the contextBridge API in `preload.js`
 * to expose Node.js functionality from the main process.
 */


const startupBtn = document.getElementById('startup_btn');
const resetVidBtn = document.getElementById('resetv_btn');

const handleOBSResponse = (button, success) => {
    if (success)
    {
        button.innerText = 'Success!';
    }
    else
    {
        // Let user try again
        button.disabled = false;
    }
};

startupBtn.addEventListener('click', async (event) => {
    event.target.disabled = true;
    const result = await window.ipc.obsStartup();
    handleOBSResponse(event.target, result);
});

resetVidBtn.addEventListener('click', async (event) => {
    event.target.disabled = true;
    const result = await window.ipc.obsResetVideo();
    handleOBSResponse(event.target, result);
});

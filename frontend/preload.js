const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("lacinka", {
  transform: (input, mode) => ipcRenderer.invoke("transform", String(input || ""), String(mode || "0")),
  minimize: () => ipcRenderer.invoke("window:minimize"),
  close: () => ipcRenderer.invoke("window:close"),
  setAlwaysOnTop: (flag) => ipcRenderer.invoke("window:always-on-top", Boolean(flag))
});

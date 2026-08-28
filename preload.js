const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("lacinka", {
  minimize: () => ipcRenderer.invoke("window:minimize"),
  close: () => ipcRenderer.invoke("window:close"),
  setAlwaysOnTop: (flag) => ipcRenderer.invoke("window:always-on-top", Boolean(flag))
});

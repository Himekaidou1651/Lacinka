const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("lacinka", {
  transform: (input, mode) => ipcRenderer.invoke("transform", String(input || ""), String(mode || "0")),
  minimize: () => ipcRenderer.invoke("window:minimize"),
  maximize: () => ipcRenderer.invoke("window:maximize"),
  close: () => ipcRenderer.invoke("window:close"),
  setAlwaysOnTop: (flag) => ipcRenderer.invoke("window:always-on-top", Boolean(flag)),
  onMaximizeChange: (callback) => ipcRenderer.on("window:maximize-change", (_event, maximized) => callback(Boolean(maximized)))
});

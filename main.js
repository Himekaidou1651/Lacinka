const { app, BrowserWindow, ipcMain } = require("electron");
const { spawn } = require("child_process");
const path = require("path");

let mainWindow = null;

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 1280,
    height: 860,
    minWidth: 420,
    minHeight: 620,
    backgroundColor: "#f7fafc",
    frame: false,
    title: "Lacinka",
    webPreferences: {
      preload: path.join(__dirname, "frontend", "preload.js"),
      contextIsolation: true,
      nodeIntegration: false,
      sandbox: true
    }
  });

  mainWindow.loadFile(path.join(__dirname, "frontend", "index.html"));
  mainWindow.on("closed", () => {
    mainWindow = null;
  });
}

app.whenReady().then(() => {
  createWindow();

  app.on("activate", () => {
    if (BrowserWindow.getAllWindows().length === 0) {
      createWindow();
    }
  });
});

app.on("window-all-closed", () => {
  if (process.platform !== "darwin") {
    app.quit();
  }
});

ipcMain.handle("window:minimize", () => {
  if (mainWindow) {
    mainWindow.minimize();
  }
});

ipcMain.handle("window:close", () => {
  if (mainWindow) {
    mainWindow.close();
  }
});

ipcMain.handle("window:always-on-top", (_event, flag) => {
  if (mainWindow) {
    mainWindow.setAlwaysOnTop(Boolean(flag));
    return mainWindow.isAlwaysOnTop();
  }
  return false;
});

ipcMain.handle("transform", async (_event, input, mode) => {
  return runTransform(String(input || ""), String(mode || "0"));
});

function runTransform(input, mode) {
  const exeName = process.platform === "win32" ? "transform_cli.exe" : "transform_cli";
  const exePath = app.isPackaged
    ? path.join(process.resourcesPath, exeName)
    : path.join(__dirname, exeName);

  return new Promise((resolve, reject) => {
    const child = spawn(exePath, [mode], {
      windowsHide: true,
      stdio: ["pipe", "pipe", "pipe"]
    });

    let output = "";
    let error = "";

    child.stdout.setEncoding("utf8");
    child.stderr.setEncoding("utf8");
    child.stdout.on("data", (chunk) => {
      output += chunk;
    });
    child.stderr.on("data", (chunk) => {
      error += chunk;
    });
    child.on("error", (err) => {
      reject(new Error(`无法启动转写引擎：${err.message}`));
    });
    child.on("close", (code) => {
      if (code === 0) {
        resolve(output);
      } else {
        reject(new Error(error || `转写引擎退出码：${code}`));
      }
    });

    child.stdin.end(input, "utf8");
  });
}

var TEXTS = {
  statusWaiting: "等待输入",
  statusProcessing: "正在转写",
  statusDone: "转写完成",
  statusCopied: "已复制输出",
  statusDownloaded: "已下载输出",
  runButton: "转写",
  processingButton: "处理中...",
  emptyOutput: "转写结果将在此显示",
  emptyWarning: "请先输入待转写内容",
  lastRunPrefix: "上次运行: "
};

var SAMPLES = window.LacinkaCommon.samples;

var state = {
  mode: "0",
  input: "",
  output: "",
  busy: false,
  theme: "light",
  toastTimer: 0
};

function $(selector) {
  return document.querySelector(selector);
}

function $all(selector) {
  return Array.prototype.slice.call(document.querySelectorAll(selector));
}

function countText(value) {
  return String(value || "").length;
}

function formatCount(value) {
  return countText(value) + " 字符";
}

function updateCount(el, value) {
  var n = countText(value);
  el.textContent = formatCount(value);
  el.dataset.level = n > 5000 ? "danger" : n > 1000 ? "warning" : "normal";
}

function showToast(message) {
  var toast = $("#toast");
  toast.textContent = message;
  toast.classList.add("show");
  clearTimeout(state.toastTimer);
  state.toastTimer = setTimeout(function () {
    toast.classList.remove("show");
  }, 1800);
}

function setStatus(status) {
  $("#status-text").textContent = status;
}

function setError(message) {
  var banner = $("#error-banner");
  banner.textContent = message || "";
  banner.hidden = !message;
}

function renderText() {
  var input = $("#input-text");
  var output = $("#output-text");
  state.input = input.value;
  updateCount($("#input-count"), state.input);
  updateCount($("#output-count"), state.output);
  $("#input-meter").style.width = Math.min(countText(state.input) / 5000 * 100, 100) + "%";
  output.textContent = state.output || TEXTS.emptyOutput;
  output.classList.toggle("empty", !state.output);
  output.classList.remove("fade-in");
  window.requestAnimationFrame(function () {
    output.classList.add("fade-in");
  });
}

function setMode(mode) {
  state.mode = String(mode);
  $all(".mode-card").forEach(function (card) {
    card.classList.toggle("active", card.dataset.mode === state.mode);
  });
}

function setBusy(flag, nextStatus) {
  state.busy = Boolean(flag);
  $("#run-transform").disabled = state.busy;
  $("#run-transform").classList.toggle("busy", state.busy);
  $("#run-label").textContent = state.busy ? TEXTS.processingButton : TEXTS.runButton;
  setStatus(state.busy ? TEXTS.statusProcessing : nextStatus || TEXTS.statusDone);
}

function runTransform() {
  state.input = $("#input-text").value;
  if (!state.input.trim()) {
    setError(TEXTS.emptyWarning);
    setStatus(TEXTS.statusWaiting);
    showToast(TEXTS.emptyWarning);
    return;
  }

  setError("");
  setBusy(true);
  window.lacinka.transform(state.input, state.mode).then(function (output) {
    state.output = output;
    setBusy(false, TEXTS.statusDone);
    $("#last-run").textContent = TEXTS.lastRunPrefix + new Date().toLocaleTimeString();
    renderText();
  }).catch(function (error) {
    setBusy(false, TEXTS.statusWaiting);
    setError(error && error.message ? error.message : "转写失败");
    showToast("转写失败");
  });
}

function clearAll() {
  $("#input-text").value = "";
  state.input = "";
  state.output = "";
  setError("");
  setStatus(TEXTS.statusWaiting);
  $("#last-run").textContent = TEXTS.lastRunPrefix + "--";
  renderText();
}

function swapText() {
  var input = $("#input-text");
  var oldInput = input.value;
  input.value = state.output;
  state.output = oldInput;
  renderText();
}

function copyOutput() {
  if (!state.output) {
    showToast("暂无输出可复制");
    return;
  }
  navigator.clipboard.writeText(state.output).then(function () {
    setStatus(TEXTS.statusCopied);
    showToast(TEXTS.statusCopied);
  }).catch(function () {
    showToast("复制失败");
  });
}

function downloadOutput(format) {
  if (!state.output) {
    showToast("暂无输出可下载");
    return;
  }
  var content = format === "json"
    ? JSON.stringify({ mode: state.mode, input: state.input, output: state.output }, null, 2)
    : state.output;
  var type = format === "json" ? "application/json" : "text/plain";
  var blob = new Blob([content], { type: type + ";charset=utf-8" });
  var url = URL.createObjectURL(blob);
  var link = document.createElement("a");
  link.href = url;
  link.download = "Lacinka_output." + format;
  link.click();
  URL.revokeObjectURL(url);
  setStatus(TEXTS.statusDownloaded);
  showToast(TEXTS.statusDownloaded);
}

function setDownloadMenu(open) {
  $("#download-options").hidden = !open;
  $("#download-menu").setAttribute("aria-expanded", String(open));
}

function toggleDownloadMenu() {
  setDownloadMenu($("#download-options").hidden);
}

function setMaximizeIcon(maximized) {
  $("#btn-maximize").textContent = maximized ? "❐" : "□";
  $("#btn-maximize").setAttribute("aria-label", maximized ? "还原" : "最大化");
}

function bindEvents() {
  $("#btn-minimize").addEventListener("click", function () {
    window.lacinka.minimize();
  });
  $("#btn-maximize").addEventListener("click", function () {
    window.lacinka.maximize().then(setMaximizeIcon);
  });
  window.lacinka.onMaximizeChange(setMaximizeIcon);
  $("#btn-close").addEventListener("click", function () {
    window.lacinka.close();
  });
  $("#always-on-top").addEventListener("change", function (event) {
    window.lacinka.setAlwaysOnTop(event.target.checked);
  });
  $("#theme-toggle").addEventListener("click", function () {
    state.theme = state.theme === "light" ? "dark" : "light";
    $(".window").dataset.theme = state.theme;
    $("#theme-toggle").textContent = state.theme === "light" ? "◐" : "◑";
  });
  $("#input-text").addEventListener("input", renderText);
  $("#run-transform").addEventListener("click", runTransform);
  $("#clear-all").addEventListener("click", clearAll);
  $("#swap-text").addEventListener("click", swapText);
  $("#copy-output").addEventListener("click", copyOutput);
  $("#download-menu").addEventListener("click", toggleDownloadMenu);
  $all("[data-mode]").forEach(function (card) {
    card.addEventListener("click", function () {
      setMode(card.dataset.mode);
    });
  });
  $("#insert-sample").addEventListener("click", function () {
    $("#input-text").value = SAMPLES[state.mode] || "";
    renderText();
  });
  $all("[data-download]").forEach(function (item) {
    item.addEventListener("click", function () {
      setDownloadMenu(false);
      downloadOutput(item.dataset.download);
    });
  });
  document.addEventListener("click", function (event) {
    var wrap = document.querySelector(".menu-wrap");
    if (wrap && !wrap.contains(event.target)) {
      setDownloadMenu(false);
    }
  });
  document.addEventListener("keydown", function (event) {
    if (event.ctrlKey && event.key === "Enter") {
      runTransform();
    }
    if (event.key === "Escape") {
      setDownloadMenu(false);
    }
  });
}

function bootstrap() {
  bindEvents();
  setMode("0");
  setStatus(TEXTS.statusWaiting);
  renderText();
}

document.addEventListener("DOMContentLoaded", bootstrap);

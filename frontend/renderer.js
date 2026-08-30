var I18N = window.LacinkaI18n;
var currentLocale = "zh-CN";

function t(key) {
  var dict = I18N[currentLocale] || I18N["zh-CN"] || {};
  return Object.prototype.hasOwnProperty.call(dict, key) ? dict[key] : key;
}

var SAMPLES = window.LacinkaCommon.samples;
var CONFIG = window.LacinkaCommon.config;

var state = {
  mode: "0",
  input: "",
  output: "",
  busy: false,
  theme: "light",
  toastTimer: 0,
  statusKey: "statusWaiting",
  lastRunTime: null
};

function $(selector) {
  return document.querySelector(selector);
}

function $all(selector) {
  return Array.prototype.slice.call(document.querySelectorAll(selector));
}

function applyI18n() {
  document.documentElement.lang = currentLocale;
  $all("[data-i18n]").forEach(function (el) {
    el.textContent = t(el.dataset.i18n);
  });
  $all("[data-i18n-placeholder]").forEach(function (el) {
    el.placeholder = t(el.dataset.i18nPlaceholder);
  });
  $all("[data-i18n-aria-label]").forEach(function (el) {
    el.setAttribute("aria-label", t(el.dataset.i18nAriaLabel));
  });
  $all("[data-i18n-title]").forEach(function (el) {
    el.title = t(el.dataset.i18nTitle);
  });
}

function setLocale(locale) {
  if (!I18N[locale]) {
    return;
  }
  currentLocale = locale;
  applyI18n();
  refreshText();
}

function countText(value) {
  return String(value || "").length;
}

function formatCount(value) {
  return countText(value) + " " + t("charsUnit");
}

function updateCount(el, value) {
  var n = countText(value);
  el.textContent = formatCount(value);
  el.dataset.level = n > CONFIG.maxChars ? "danger" : n > CONFIG.warningChars ? "warning" : "normal";
}

function showToast(message) {
  var toast = $("#toast");
  toast.textContent = message;
  toast.classList.add("show");
  clearTimeout(state.toastTimer);
  state.toastTimer = setTimeout(function () {
    toast.classList.remove("show");
  }, CONFIG.toastDurationMs);
}

function setStatus(key) {
  state.statusKey = key;
  $("#status-text").textContent = t(key);
}

function renderLastRun() {
  $("#last-run").textContent = t("lastRunPrefix") + (state.lastRunTime || "--");
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
  $("#input-meter").style.width = Math.min(countText(state.input) / CONFIG.maxChars * CONFIG.meterMaxPercent, CONFIG.meterMaxPercent) + "%";
  output.textContent = state.output || t("emptyOutput");
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

function setBusy(flag, nextStatusKey) {
  state.busy = Boolean(flag);
  $("#run-transform").disabled = state.busy;
  $("#run-transform").classList.toggle("busy", state.busy);
  $("#run-label").textContent = state.busy ? t("processingButton") : t("runButton");
  setStatus(state.busy ? "statusProcessing" : nextStatusKey || "statusDone");
}

function runTransform() {
  state.input = $("#input-text").value;
  if (!state.input.trim()) {
    setError(t("emptyWarning"));
    setStatus("statusWaiting");
    showToast(t("emptyWarning"));
    return;
  }

  setError("");
  setBusy(true);
  window.lacinka.transform(state.input, state.mode).then(function (output) {
    state.output = output;
    setBusy(false, "statusDone");
    state.lastRunTime = new Date().toLocaleTimeString();
    renderLastRun();
    renderText();
  }).catch(function (error) {
    setBusy(false, "statusWaiting");
    setError(error && error.message ? error.message : t("transformFailed"));
    showToast(t("transformFailed"));
  });
}

function clearAll() {
  $("#input-text").value = "";
  state.input = "";
  state.output = "";
  state.lastRunTime = null;
  setError("");
  setStatus("statusWaiting");
  renderLastRun();
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
    showToast(t("noOutputToCopy"));
    return;
  }
  navigator.clipboard.writeText(state.output).then(function () {
    setStatus("statusCopied");
    showToast(t("statusCopied"));
  }).catch(function () {
    showToast(t("copyFailed"));
  });
}

function downloadOutput(format) {
  if (!state.output) {
    showToast(t("noOutputToDownload"));
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
  setStatus("statusDownloaded");
  showToast(t("statusDownloaded"));
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
  $("#btn-maximize").setAttribute("aria-label", maximized ? t("restore") : t("maximize"));
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
  $("#lang-toggle").addEventListener("click", function () {
    setLocale(currentLocale === "en" ? "zh-CN" : "en");
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

function refreshText() {
  setBusy(state.busy, state.statusKey);
  renderLastRun();
  renderText();
}

function bootstrap() {
  applyI18n();
  bindEvents();
  setMode("0");
  setBusy(false, "statusWaiting");
  renderLastRun();
  renderText();
}

document.addEventListener("DOMContentLoaded", bootstrap);

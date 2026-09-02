var I18N = window.LacinkaI18n;
var currentLocale = "zh-CN";

function t(key) {
  var dict = I18N[currentLocale] || I18N["zh-CN"] || {};
  return Object.prototype.hasOwnProperty.call(dict, key) ? dict[key] : key;
}

var SAMPLES = window.LacinkaCommon.samples;
var GROUPS = window.LacinkaCommon.groups;
var CONFIG = window.LacinkaCommon.config;
var MODE_ACCENTS = {
  "0": "blue",
  "1": "green",
  "2": "rose",
  "3": "amber",
  "4": "gray",
  "5": "teal",
  "6": "violet",
  "7": "gold",
  "8": "indigo",
  "9": "cyan",
  "10": "pink",
  "11": "lime",
  "12": "teal",
};

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

function getGroupForMode(mode) {
  var value = String(mode);
  var found = "";
  Object.keys(GROUPS).some(function (group) {
    if (GROUPS[group].indexOf(value) !== -1) {
      found = group;
      return true;
    }
    return false;
  });
  return found;
}

function renderGroupButtons() {
  var activeGroup = getGroupForMode(state.mode);
  $all(".group-button").forEach(function (button) {
    var group = button.dataset.group;
    var selected = button.querySelector(".group-selected");
    var groupModes = GROUPS[group] || [];
    var groupMode = group === activeGroup ? state.mode : groupModes[0];
    button.classList.toggle("active", group === activeGroup);
    button.setAttribute("aria-pressed", String(group === activeGroup));
    if (selected) {
      selected.textContent = t("mode" + groupMode + "Name");
    }
  });
}

function createModeCard(mode) {
  var card = document.createElement("button");
  var name = document.createElement("span");
  var desc = document.createElement("span");

  card.className = "mode-card";
  card.type = "button";
  card.dataset.mode = mode;
  card.dataset.accent = MODE_ACCENTS[mode] || "blue";
  card.setAttribute("role", "radio");
  card.setAttribute("aria-checked", String(mode === state.mode));
  card.classList.toggle("active", mode === state.mode);

  name.className = "mode-name";
  name.dataset.i18n = "mode" + mode + "Name";
  name.textContent = t(name.dataset.i18n);

  desc.className = "mode-desc";
  desc.dataset.i18n = "mode" + mode + "Desc";
  desc.textContent = t(desc.dataset.i18n);

  card.appendChild(name);
  card.appendChild(desc);
  card.addEventListener("click", function () {
    setMode(mode);
    closePopup();
  });

  return card;
}

function openGroup(group) {
  var modes = GROUPS[String(group)] || [];
  var overlay = $("#mode-popup");
  var panel = overlay.querySelector(".popup-panel");
  var list = $("#popup-mode-list");

  panel.dataset.group = String(group);
  list.textContent = "";
  modes.forEach(function (mode) {
    list.appendChild(createModeCard(mode));
  });

  overlay.hidden = false;
  window.requestAnimationFrame(function () {
    var active = list.querySelector(".mode-card.active") || list.querySelector(".mode-card");
    if (active) {
      active.focus();
    }
  });
}

function closePopup() {
  $("#mode-popup").hidden = true;
}

function setMode(mode) {
  state.mode = String(mode);
  $all(".mode-card").forEach(function (card) {
    card.classList.toggle("active", card.dataset.mode === state.mode);
    card.setAttribute("aria-checked", String(card.dataset.mode === state.mode));
  });
  renderGroupButtons();
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
  $all("[data-group]").forEach(function (button) {
    button.addEventListener("click", function () {
      openGroup(button.dataset.group);
    });
  });
  $("#mode-popup").addEventListener("click", function (event) {
    if (event.target === event.currentTarget) {
      closePopup();
    }
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
      closePopup();
    }
  });
}

function refreshText() {
  setBusy(state.busy, state.statusKey);
  renderGroupButtons();
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

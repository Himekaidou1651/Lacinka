function $(selector) {
  return document.querySelector(selector);
}

function setYear() {
  var el = $("#year");
  if (el) {
    el.textContent = String(new Date().getFullYear());
  }
}

function bindWindowButtons() {
  var minimizeBtn = $("#btn-minimize");
  var closeBtn = $("#btn-close");
  if (minimizeBtn) {
    minimizeBtn.addEventListener("click", function () {
      window.lacinka.minimize();
    });
  }
  if (closeBtn) {
    closeBtn.addEventListener("click", function () {
      window.lacinka.close();
    });
  }
}

function bootstrap() {
  setYear();
  bindWindowButtons();
}

document.addEventListener("DOMContentLoaded", bootstrap);

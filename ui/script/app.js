// 宿主注入的 native 对象负责真正的转写执行。
var nativeHost = native;

// 统一收口所有界面文案。
var TEXTS = {
    windowTitle: "Lacinka",
    appTitle: "Lacinka \u8f6c\u5199\u5de5\u5177",
    statusWaiting: "\u7b49\u5f85\u8f93\u5165",
    statusProcessing: "\u6b63\u5728\u8f6c\u5199",
    statusDone: "\u8f6c\u5199\u5b8c\u6210",
    statusCopied: "\u5df2\u590d\u5236\u8f93\u51fa",
    statusDownloaded: "\u5df2\u4e0b\u8f7d\u8f93\u51fa",
    pillReady: "\u5c31\u7eea",
    pillWorking: "\u5de5\u4f5c\u4e2d",
    runButton: "\u8f6c\u5199",
    processingButton: "\u5904\u7406\u4e2d...",
    clearButton: "\u6e05\u7a7a",
    swapButton: "\u4ea4\u6362",
    copyButton: "\u590d\u5236",
    downloadButton: "\u4e0b\u8f7dTXT",
    hellasSampleButton: "\u5e0c\u814a\u793a\u4f8b",
    jugoslavSampleButton: "\u585e\u5c14\u7ef4\u4e9a\u793a\u4f8b",
    charactersSuffix: " \u5b57\u7b26",
    lastRunPrefix: "\u4e0a\u6b21\u8fd0\u884c: ",
    neverRun: "--",
    emptyOutput: "\u8f6c\u5199\u7ed3\u679c\u5c06\u5728\u6b64\u663e\u793a",
    emptyInput: "\u5728\u6b64\u8f93\u5165\u5f85\u8f6c\u5199\u5185\u5bb9..."
};

// 统一收口模式标签。
var MODES = {
    "0": "Hellas",
    "1": "Jugoslav"
};

// 统一收口示例文本。
var SAMPLES = {
    "0": "\u0395\u03bb\u03bb\u03b1\u03c2 \u03ba\u03b1\u03b9 \u03b3\u03bb\u03c9\u03c3\u03c3\u03b1.",
    "1": "\u0421\u0440\u0431\u0438\u0458\u0430 \u0438 \u0408\u0443\u0433\u043e\u0441\u043b\u0430\u0432\u0438\u0458\u0430."
};

// 统一收口视觉参数。
var STYLE = {
    palette: {
        background: "#f7fafc",
        surface: "#ffffff",
        surfaceAlt: "#edf2f7",
        text: "#1a202c",
        muted: "#4a5568",
        border: "#cbd5e0",
        primary: "#2b6cb0",
        primarySoft: "#ebf4ff",
        success: "#38a169",
        warning: "#d69e2e",
        danger: "#e53e3e",
        output: "#1a202c",
        input: "#ffffff",
        pillReady: "#38a169",
        pillWorking: "#d69e2e"
    },
    radius: {
        panel: 6,
        button: 6,
        input: 6,
        pill: 999
    },
    spacing: {
        xs: 4,
        sm: 8,
        md: 12,
        lg: 16,
        xl: 24
    },
    font: {
        family: 'Arial, "Microsoft YaHei", sans-serif',
        baseSize: 14,
        titleSize: 17,
        smallSize: 12,
        lineHeight: 1.5
    },
    layout: {
        desktopBreakpoint: 1200,
        headerHeight: 68,
        footerHeight: 44,
        padding: 16,
        controlWidth: 240,
        leftColumnMin: 320,
        rightColumnMin: 320
    }
};

// 当前界面状态。
var state = {
    mode: 0,
    input: "",
    output: "",
    error: "",
    status: TEXTS.statusWaiting,
    pill: TEXTS.pillReady,
    busy: false,
    lastRun: TEXTS.neverRun
};

// 宿主能力适配层。
var api = {
    text: function (key) {
        return TEXTS[key] || key;
    },
    number: function (key) {
        return STYLE.layout[key] || 0;
    },
    modeLabel: function (mode) {
        return MODES[String(mode)] || String(mode);
    },
    sample: function (mode) {
        return SAMPLES[String(mode)] || "";
    },
    transform: function (input, mode) {
        return nativeHost.transform(String(input), String(mode));
    }
};

function normalizeText(value) {
    return String(value == null ? "" : value);
}

function formatCount(value) {
    return String(normalizeText(value).length) + api.text("charactersSuffix");
}

function text(key) {
    return api.text(key);
}

function number(key) {
    return api.number(key);
}

function modeLabel(mode) {
    return api.modeLabel(mode);
}

function sample(mode) {
    return api.sample(mode);
}

function setMode(mode) {
    state.mode = Number(mode) || 0;
}

function setStatus(statusText, pillText) {
    state.status = normalizeText(statusText);
    state.pill = normalizeText(pillText);
}

function setInput(value) {
    state.input = normalizeText(value);
}

function setOutput(value) {
    state.output = normalizeText(value);
}

function setError(value) {
    state.error = normalizeText(value);
}

function clearRuntimeState() {
    state.error = "";
    state.output = "";
    state.status = text("statusWaiting");
    state.pill = text("pillReady");
    state.busy = false;
    state.lastRun = text("neverRun");
}

function fillSample(mode) {
    setMode(mode);
    setInput(sample(mode));
}

function runTransform() {
    if (state.busy) {
        return;
    }
    state.busy = true;
    setStatus(text("statusProcessing"), text("pillWorking"));
    setError("");
    state.output = api.transform(state.input, state.mode);
    setStatus(text("statusDone"), text("pillReady"));
    state.lastRun = text("lastRunPrefix") + new Date().toLocaleTimeString();
    state.busy = false;
}

function clearAll() {
    setInput("");
    setOutput("");
    setError("");
    setStatus(text("statusWaiting"), text("pillReady"));
    state.lastRun = text("lastRunPrefix") + text("neverRun");
}

function swapText() {
    var oldInput = state.input;
    state.input = state.output;
    state.output = oldInput;
}

function copyOutput() {
    setStatus(text("statusCopied"), text("pillReady"));
}

function downloadOutput() {
    setStatus(text("statusDownloaded"), text("pillReady"));
}

function getLayoutMode(width) {
    return width >= number("desktopBreakpoint") ? "desktop" : "compact";
}

function getHeaderTitle() {
    return text("appTitle");
}

function getInputPlaceholder() {
    return text("emptyInput");
}

function getOutputPlaceholder() {
    return state.output ? state.output : text("emptyOutput");
}

function getThemeName() {
    return "light";
}

function getStyleSnapshot() {
    return {
        background: STYLE.palette.background,
        surface: STYLE.palette.surface,
        surfaceAlt: STYLE.palette.surfaceAlt,
        text: STYLE.palette.text,
        muted: STYLE.palette.muted,
        border: STYLE.palette.border,
        primary: STYLE.palette.primary,
        primarySoft: STYLE.palette.primarySoft,
        success: STYLE.palette.success,
        warning: STYLE.palette.warning,
        danger: STYLE.palette.danger,
        inputBg: STYLE.palette.input,
        outputBg: STYLE.palette.output,
        radius: STYLE.radius.panel,
        buttonRadius: STYLE.radius.button,
        inputRadius: STYLE.radius.input,
        pillRadius: STYLE.radius.pill,
        fontFamily: STYLE.font.family,
        fontSize: STYLE.font.baseSize,
        titleSize: STYLE.font.titleSize,
        smallSize: STYLE.font.smallSize,
        lineHeight: STYLE.font.lineHeight
    };
}

function renderSnapshot() {
    return {
        title: getHeaderTitle(),
        mode: state.mode,
        input: state.input,
        output: state.output,
        error: state.error,
        status: state.status,
        pill: state.pill,
        lastRun: state.lastRun,
        inputCount: formatCount(state.input),
        outputCount: formatCount(state.output),
        layout: getLayoutMode(1280),
        theme: getThemeName(),
        style: getStyleSnapshot()
    };
}

function lacinkaText(key) {
    return text(key);
}

function lacinkaNumber(key) {
    return number(key);
}

function lacinkaModeLabel(mode) {
    return modeLabel(mode);
}

function lacinkaSample(mode) {
    return sample(mode);
}

function lacinkaTransform(input, mode) {
    return api.transform(input, mode);
}

function lacinkaSnapshot() {
    return renderSnapshot();
}

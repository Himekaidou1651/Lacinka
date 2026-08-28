var LacinkaUi = (function (native) {
    var texts = {
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
        neverRun: "--"
    };

    var modeLabels = {
        "0": "Hellas",
        "1": "Jugoslav"
    };

    var samples = {
        "0": "\u0395\u03bb\u03bb\u03b1\u03c2 \u03ba\u03b1\u03b9 \u03b3\u03bb\u03c9\u03c3\u03c3\u03b1.",
        "1": "\u0421\u0440\u0431\u0438\u0458\u0430 \u0438 \u0408\u0443\u0433\u043e\u0441\u043b\u0430\u0432\u0438\u0458\u0430."
    };

    var numbers = {
        desktopBreakpoint: 1200,
        headerHeight: 68,
        footerHeight: 44,
        padding: 16,
        controlWidth: 240
    };

    function text(key) {
        return texts[key] || key;
    }

    function number(key) {
        return numbers[key] || 0;
    }

    function modeLabel(mode) {
        return modeLabels[String(mode)] || String(mode);
    }

    function sample(mode) {
        return samples[String(mode)] || "";
    }

    function transform(input, mode) {
        if (!input) {
            return "";
        }
        return native.transform(String(mode), String(input));
    }

    return {
        text: text,
        number: number,
        modeLabel: modeLabel,
        sample: sample,
        transform: transform
    };
})(native);

function lacinkaText(key) {
    return LacinkaUi.text(key);
}

function lacinkaNumber(key) {
    return LacinkaUi.number(key);
}

function lacinkaModeLabel(mode) {
    return LacinkaUi.modeLabel(mode);
}

function lacinkaSample(mode) {
    return LacinkaUi.sample(mode);
}

function lacinkaTransform(input, mode) {
    return LacinkaUi.transform(input, mode);
}

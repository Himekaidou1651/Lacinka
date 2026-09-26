# Łacinka

[![English](https://img.shields.io/badge/Docs-English-8B5CF6?style=flat-square)](./README.md)

Łacinka 是一个桌面转写工具。

## 功能

- 13 种转写模式
- 输入面板带实时字数统计与长度进度条
- 「插入示例」按钮：不同模式插入不同的示例文本
- 交换输入输出、复制输出、导出 `.txt` / `.json`
- 昼夜主题切换与窗口置顶
- 汉/英界面切换
- 状态栏显示当前状态与上次运行时间；错误横幅与轻提示（toast）
- 键盘快捷键：`Ctrl+Enter` 触发转写，`Esc` 关闭下载菜单

## 转写模式

| 模式 | 源语言 | 目标转写 |
| --- | --- | --- |
| 0 | 希腊语 | 拉丁字母 |
| 1 | 塞克语 | 拉丁字母 |
| 2 | 朝鲜语 | 罗马音 |
| 3 | 白罗斯语 | 拉丁字母-Łacinka |
| 4 | 白罗斯语 | 拉丁字母-2007 |
| 5 | 乌克兰语 | 拉丁字母-Łacinka |
| 6 | 拉丁语 | 教会拉丁字母 |
| 7 | 俄语 | 拉丁字母-Łacinka |
| 8 | 俄语 | 旧拉丁字母-Łacinka |
| 9 | 波斯语（塔吉克方言） | 拉丁字母 |
| 10 | 亚美尼亚语（东-埃里温） | 拉丁字母 |
| 11 | 格鲁吉亚语（骑士体） | 拉丁字母 |
| 12 | 保加利亚语（马其顿方言） | 拉丁字母 |

## 工作方式

前端 Electron 负责界面交互，转写逻辑由 `transform_cli` C++ 程序完成。
桌面应用是原生转换器的外壳。

## 运行

启动 `Lacinka.exe`。

## 项目结构

- `main.js` - Electron 主进程（窗口管理、IPC、启动 `transform_cli`）
- `electron-start.js` - 本地开发启动器
- `frontend/` - 渲染进程界面
  - `index.html`、`style.css`、`renderer.js` - 界面与交互逻辑
  - `preload.js` - 上下文桥接
  - `i18n/` - 中英文语言字典
- `core/common/Common.js` - 共享示例文本与配置
- `core/transform/` - 各语言转写实现
- `launcher/` - 构建脚本
- `assets/icons/` - 应用图标

## 备注

- 字数超过 12000 字符时标记为超限
- 「示例」按钮会按当前模式插入对应的示例文本
- 导出格式：纯文本 `.txt` 与 `.json`

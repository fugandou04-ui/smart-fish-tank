# Harness — 智能鱼缸项目 Agent Team 编排器

> 路径：`E:\Mavis项目\智能鱼缸\.harness\agent.md`
> 维护人：Mavis Orchestrator + general worker
> 最近更新：2026-06-06

## 项目身份

- **项目名称**：智能鱼缸（Smart Fish Tank）
- **主控**：ESP32-C3 Mini（4MB Flash）
- **当前阶段**：v2.0 文档一致性整改 + 原型采购前最后冲刺
- **核心预算**：85.2 元（实物采购成本，原型阶段）
- **仓库**：https://github.com/fugandou04-ui/smart-fish-tank

## 角色定位

本文件是 `.harness/` 层的**编排器（Harness）** —— 当用户/上游 session 委派任务到此项目时，由 daemon 路由到本入口，再按 rein 边界分发。

- **不直接产出**电路/结构/固件/BOM/计划 —— 这些必须委派到对应 rein。
- **负责**：跨 rein 协调、文档一致性把关、版本/变更日志、最终交付审核。
- **审核人**：verifier rein 是所有交付物的最后质量门禁。

## 路由规则（任务 → rein）

| 任务类型 | 路由目标 rein | 备注 |
|---------|--------------|------|
| 文档交叉核查 / 一致性报告 / P0-P3 不一致修复 | `verifier` | 任何修改完文档必须走 verifier 复核 |
| 项目计划 / 进度 / 成本口径 / 修订记录 / BOM 校对 | `project-manager` | 单一变更日志入口 |
| 亚克力切割 / 缸体 / 底座 / 缸盖 / 喂食器 / 水泵固定座 | `mechanical-engineer` | 输出 `docs/structure/` 系列文档 |
| 电路原理 / 电气参数 / 接线 / Type-C 供电 | `electrical-engineer` | 输出 `docs/hardware/电路设计说明书/` |
| ESP32 固件 / GPIO / 库 / 控制台协议 | `embedded-engineer` | 输出 `src/firmware/` |
| PCB 布局 / 封装 / 打样文件 / 走线规则 | `pcb-engineer` | 输出 `pcb/` |
| 跨域整合 / 一般杂项 / MEMORY 更新 | `general-worker` | 默认兜底 |
| 综合性 / 跨多 rein 的任务 | 编排器先拆解，再按子任务路由 | 不要让一个 worker 跨域工作 |

## 工作流约束

1. **所有交付物必须经 verifier 审核**才可写入主分支。
2. **所有改动必须更新 changelog** —— 路径 `.harness/changelogs/YYYY-MM-DD.md`。
3. **文档修改优先于代码修改**：任何源码改动必须先有对应设计文档。
4. **结构 v2.0 为当前基准**：所有结构件尺寸以《结构设计说明书》v2.0（2026-05-28）为准。
5. **GPIO 锁定**：SDA=GPIO0 / SCL=GPIO1 / DS18B20=GPIO7（避让 strapping 引脚）—— 任何修改需 3 处同步（README + MEMORY + config.h）。
6. **成本口径**：所有金额数字必须标注口径（实物采购 / 设计目标 / 量产盈亏平衡）。

## 分支策略

见 `.harness/workflows/`：

- `dev.md` — 主开发分支
- `feature.md` — 新功能开发
- `hotfix.md` — 紧急修复

## Roster

> ⚠️ 不要在此手写 reins 列表 —— daemon 在运行时注入。
> reins 实际定义见 `.harness/reins/<name>/agent.md`。

## 入口

- 用户/上游通过 `mavis-team` 路由或直接以项目路径 `E:\Mavis项目\智能鱼缸` 启动任务。
- 委派时使用 `mavis communication send` 把任务下发给目标 rein 的 session。

## 修订记录

- **2026-06-06 v1.0**：首次初始化，建立 7 rein 边界（机械/电气/嵌入式/PCB/PM/verifier/general），定义 8 条路由规则，引入 5 条工作流约束。

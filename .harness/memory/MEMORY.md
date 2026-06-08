# Harness Memory — 智能鱼缸项目 Agent Team 记忆

> 路径：`E:\Mavis项目\智能鱼缸\.harness\memory\MEMORY.md`
> 维护人：Mavis Orchestrator
> 最近更新：2026-06-06

> ⚠️ 本文件是 `.harness/` 层的**跨任务记忆**，记录团队协作层面的经验教训。
> **项目层记忆**见 `E:\Mavis项目\智能鱼缸\MEMORY.md`（含 GPIO / 成本 / 结构 / 决策记录）。

## 团队结构

7 个 rein，清晰分工：

| Rein | 对应 Mavis agent | 域 |
|------|----------------|-----|
| mechanical-engineer | agent-fb5e22234b6c | 结构件 / 切割 / 装配 |
| electrical-engineer | agent-0d6cdfc1eb81 | 电路原理 / 电气参数 |
| embedded-engineer | agent-3270b7271885 | 固件代码 / GPIO / 库 |
| pcb-engineer | agent-660e65ccbec2 | PCB layout / 打样 |
| project-manager | agent-460cdc236e70 | 计划 / 成本 / 文档治理 |
| verifier | （待分配） | 文档一致性审核 |
| general-worker | general | 兜底杂项 |

## 跨域同步真值

**任何修改涉及以下真值，必须 N 处同步**：

| 真值 | 同步点 | 数量 |
|------|--------|------|
| GPIO | README + MEMORY + config.h | 3 |
| 成本数字 | 文档 + MEMORY 口径 + 项目计划书 | 3 |
| 结构尺寸 | 切割方案 + 详细设计 + 装配说明书 | 3 |
| 器件型号 | BOM + 电路文档 + PCB JSON | 3 |
| 库依赖 | 固件 README + 实际 #include | 2 |

## 已知协作教训

### 2026-06-06 — 文档一致性整改

- 9 份项目文档交叉对比发现 25 处不一致（4 P0 / 7 P1 / 8 P2 / 6 P3）
- 教训：**多文档项目必须有 verifier 审核** —— 单靠各 specialist 自审会漏掉跨文档矛盾
- 教训：**修改数字时必须带"口径"** —— 不同口径的数字（实物/设计目标/量产）混用是常见错误源
- 教训：**结构大版本变更（v2.0）必须出影响范围报告** —— 否则下游文档同步会失控

### 2026-06-06 — 任务 p2-medium 经验

- 中度不一致项（8 项）由 general-worker 兜底完成
- 教训：P2/P3 级问题可以批量交给 general-worker，P0/P1 必须 specialist

## 路由经验

- 综合性任务（如"修所有 P1"）必须由编排器先拆解为单一 rein 任务
- 不要让一个 worker 跨多个 specialist 域工作（会引入新的不一致）
- hotfix 必须小时级响应，但**不绕过 verifier**

## 工作流选择

| 场景 | 工作流 | 备注 |
|------|--------|------|
| 添加新功能 | feature | 3-14 天 |
| 修复 bug | dev | 常规 |
| 紧急修复 | hotfix | 小时级 |
| 文档整改 | dev | 走 PM 主导 |
| 一致性核查 | dev | verifier 触发 |

## 待办

- [ ] 给 verifier rein 分配实际 Mavis agent ID
- [ ] 跑通一次完整 feature 工作流作为测试
- [ ] 建立跨 rein 的"通知"机制（避免手动同步遗漏）

## 修订记录

- **2026-06-06 v1.0**：首次建立 harness 团队记忆，定义跨域同步真值表，沉淀 P0/P1/P2 整改经验。

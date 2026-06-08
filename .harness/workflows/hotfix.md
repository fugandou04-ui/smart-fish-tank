# Workflow: hotfix — 紧急修复分支

> 路径：`E:\Mavis项目\智能鱼缸\.harness\workflows\hotfix.md`
> 维护人：Mavis Orchestrator
> 最近更新：2026-06-06

## 用途

**紧急修复生产阻塞问题**。例如：固件崩溃、文档致命错误、采购 BOM 数字严重偏差、关键引脚接线错误等。

## 触发条件

满足以下任一：

- 🔴 P0 致命问题（影响原型采购/焊接/烧录）
- 🔴 跨多文档的关键数字严重偏差（>10%）
- 🔴 编译/烧录失败的代码 bug
- 🔴 实物组装后功能失效

## 分支命名

```
hotfix/<rein-scope>/<short-name>
```

**示例**：
- `hotfix/fw-config/gpio2-strapping-bug` — 修复 strapping 引脚
- `hotfix/hw-bom/relay-count-1to2` — 修复继电器数量（1→2）
- `hotfix/doc/oled-address-mismatch` — 修复 OLED 地址不一致

## 生命周期（加速流程）

```
1. 紧急任务到达编排器
2. 编排器立即派给对应 specialist（绕过常规排序）
3. specialist 在 hotfix 分支紧急修复
4. 修复完成后立即通知 verifier
5. verifier 加速审核（小时级）
6. 直接合入 main + dev
7. 同步更新 changelog 和 MEMORY
8. 视情况 backport 到 feature 分支
```

## 时间要求

| 阶段 | SLA |
|------|-----|
| 编排器分配 | < 30 分钟 |
| specialist 修复 | < 4 小时（P0）/ < 24 小时（P1 升级） |
| verifier 审核 | < 2 小时 |
| 合并 + 通知 | < 30 分钟 |

## 紧急绕过

hotfix 可绕过：

- ❌ 常规排序（PM 不再拆解，直接派 specialist）
- ❌ 长 PR review 流程（同行 review 1 人即可）
- ❌ 多日等待（小时级完成）

但**不绕过**：

- ✅ verifier 审核（必须签字）
- ✅ changelog 更新
- ✅ MEMORY 修订记录
- ✅ 文档同步（如果涉及 GPIO/尺寸/成本数字）

## 完成定义（DoD）

- [ ] 修复提交到 hotfix 分支
- [ ] commit message 标 `[hotfix]` 前缀
- [ ] verifier 签字（小时级）
- [ ] 合入 main + dev
- [ ] changelog 标记 ⚠ HOTFIX
- [ ] MEMORY 修订记录追加 hotfix 条目
- [ ] 通知所有相关 rein（如 GPIO 修复需通知 embedded + electrical）

## 不允许

- ❌ 用 hotfix 偷渡功能（必须是修复，不是新功能）
- ❌ 绕过 verifier
- ❌ 不更新 changelog

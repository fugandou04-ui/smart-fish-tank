/**
 * @file webpage.h
 * @brief 智能鱼缸Web控制页面
 * @author ESP32 Firmware
 * @date 2026-05-21
 */

#ifndef WEBPAGE_H
#define WEBPAGE_H

// ============================================
// 主页HTML
// ============================================
const char INDEX_HTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>智能鱼缸控制面板</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
        }
        .container {
            max-width: 800px;
            margin: 0 auto;
        }
        .header {
            text-align: center;
            color: white;
            margin-bottom: 30px;
        }
        .header h1 {
            font-size: 2.5em;
            margin-bottom: 10px;
        }
        .status-bar {
            background: rgba(255,255,255,0.2);
            border-radius: 10px;
            padding: 15px;
            margin-bottom: 20px;
            display: flex;
            justify-content: space-around;
            flex-wrap: wrap;
            gap: 10px;
        }
        .status-item {
            text-align: center;
            color: white;
        }
        .status-value {
            font-size: 1.8em;
            font-weight: bold;
        }
        .status-label {
            font-size: 0.9em;
            opacity: 0.9;
        }
        .card {
            background: white;
            border-radius: 15px;
            padding: 25px;
            margin-bottom: 20px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.2);
        }
        .card-title {
            font-size: 1.3em;
            color: #333;
            margin-bottom: 20px;
            padding-bottom: 10px;
            border-bottom: 2px solid #667eea;
        }
        .control-row {
            display: flex;
            justify-content: space-between;
            align-items: center;
            padding: 15px 0;
            border-bottom: 1px solid #eee;
        }
        .control-row:last-child {
            border-bottom: none;
        }
        .control-label {
            font-size: 1.1em;
            color: #555;
        }
        .toggle {
            position: relative;
            width: 60px;
            height: 30px;
        }
        .toggle input {
            opacity: 0;
            width: 0;
            height: 0;
        }
        .slider {
            position: absolute;
            cursor: pointer;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: #ccc;
            transition: 0.3s;
            border-radius: 30px;
        }
        .slider:before {
            position: absolute;
            content: "";
            height: 22px;
            width: 22px;
            left: 4px;
            bottom: 4px;
            background-color: white;
            transition: 0.3s;
            border-radius: 50%;
        }
        input:checked + .slider {
            background-color: #667eea;
        }
        input:checked + .slider:before {
            transform: translateX(30px);
        }
        .btn {
            padding: 12px 30px;
            border: none;
            border-radius: 8px;
            font-size: 1em;
            cursor: pointer;
            transition: all 0.3s;
        }
        .btn-primary {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
        }
        .btn-primary:hover {
            transform: translateY(-2px);
            box-shadow: 0 5px 15px rgba(102, 126, 234, 0.4);
        }
        .btn-danger {
            background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%);
            color: white;
        }
        .color-picker {
            display: flex;
            gap: 10px;
            align-items: center;
        }
        .color-btn {
            width: 40px;
            height: 40px;
            border-radius: 50%;
            border: 3px solid #ddd;
            cursor: pointer;
            transition: transform 0.2s;
        }
        .color-btn:hover {
            transform: scale(1.1);
        }
        .color-btn.selected {
            border-color: #667eea;
            box-shadow: 0 0 10px rgba(102, 126, 234, 0.5);
        }
        .slider-container {
            width: 100%;
            max-width: 200px;
        }
        input[type="range"] {
            width: 100%;
            height: 8px;
            border-radius: 4px;
            background: #ddd;
            outline: none;
            -webkit-appearance: none;
        }
        input[type="range"]::-webkit-slider-thumb {
            -webkit-appearance: none;
            width: 20px;
            height: 20px;
            border-radius: 50%;
            background: #667eea;
            cursor: pointer;
        }
        .schedule-grid {
            display: grid;
            grid-template-columns: repeat(2, 1fr);
            gap: 15px;
        }
        .schedule-item {
            background: #f8f9fa;
            padding: 15px;
            border-radius: 10px;
        }
        .schedule-item label {
            display: block;
            margin-bottom: 8px;
            color: #555;
            font-size: 0.9em;
        }
        .schedule-item input {
            width: 100%;
            padding: 10px;
            border: 1px solid #ddd;
            border-radius: 5px;
            font-size: 1em;
        }
        .time-display {
            font-size: 1.2em;
            color: #667eea;
            font-weight: bold;
        }
        .toast {
            position: fixed;
            top: 20px;
            right: 20px;
            background: #333;
            color: white;
            padding: 15px 25px;
            border-radius: 10px;
            opacity: 0;
            transition: opacity 0.3s;
            z-index: 1000;
        }
        .toast.show {
            opacity: 1;
        }
        .footer {
            text-align: center;
            color: white;
            margin-top: 20px;
            opacity: 0.8;
        }
        @media (max-width: 600px) {
            .schedule-grid {
                grid-template-columns: 1fr;
            }
            .status-bar {
                flex-direction: column;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🐠 智能鱼缸</h1>
            <p id="uptime">运行时间: 计算中...</p>
        </div>

        <div class="status-bar">
            <div class="status-item">
                <div class="status-value" id="temperature">--</div>
                <div class="status-label">水温 (°C)</div>
            </div>
            <div class="status-item">
                <div class="status-value" id="pump-status">OFF</div>
                <div class="status-label">水泵状态</div>
            </div>
            <div class="status-item">
                <div class="status-value" id="led-status">OFF</div>
                <div class="status-label">灯光状态</div>
            </div>
            <div class="status-item">
                <div class="status-value" id="ip">--</div>
                <div class="status-label">IP地址</div>
            </div>
        </div>

        <div class="card">
            <h2 class="card-title">💧 水泵控制</h2>
            <div class="control-row">
                <span class="control-label">水泵开关</span>
                <label class="toggle">
                    <input type="checkbox" id="pump-toggle" onchange="togglePump(this.checked)">
                    <span class="slider"></span>
                </label>
            </div>
        </div>

        <div class="card">
            <h2 class="card-title">💡 灯光控制</h2>
            <div class="control-row">
                <span class="control-label">灯光开关</span>
                <label class="toggle">
                    <input type="checkbox" id="led-toggle" onchange="toggleLed(this.checked)">
                    <span class="slider"></span>
                </label>
            </div>
            <div class="control-row">
                <span class="control-label">亮度调节</span>
                <div class="slider-container">
                    <input type="range" id="brightness-slider" min="0" max="255" value="128" onchange="setBrightness(this.value)">
                </div>
            </div>
            <div class="control-row">
                <span class="control-label">颜色选择</span>
                <div class="color-picker">
                    <div class="color-btn" style="background:#ff0000" onclick="setColor(0xff0000)"></div>
                    <div class="color-btn" style="background:#ff8800" onclick="setColor(0xff8800)"></div>
                    <div class="color-btn" style="background:#ffff00" onclick="setColor(0xffff00)"></div>
                    <div class="color-btn" style="background:#00ff00" onclick="setColor(0x00ff00)"></div>
                    <div class="color-btn" style="background:#00ffff" onclick="setColor(0x00ffff)"></div>
                    <div class="color-btn" style="background:#0088ff" onclick="setColor(0x0088ff)"></div>
                    <div class="color-btn" style="background:#8800ff" onclick="setColor(0x8800ff)"></div>
                    <div class="color-btn" style="background:#ff00ff" onclick="setColor(0xff00ff)"></div>
                    <div class="color-btn" style="background:#ffffff" onclick="setColor(0xffffff)"></div>
                </div>
            </div>
            <div class="control-row">
                <span class="control-label">自动灯光</span>
                <label class="toggle">
                    <input type="checkbox" id="auto-light-toggle" onchange="setAutoLight(this.checked)">
                    <span class="slider"></span>
                </label>
            </div>
        </div>

        <div class="card">
            <h2 class="card-title">🍽️ 喂食控制</h2>
            <div class="control-row">
                <span class="control-label">手动喂食</span>
                <button class="btn btn-danger" onclick="feedNow()">立即喂食</button>
            </div>
            <div class="control-row">
                <span class="control-label">自动喂食</span>
                <label class="toggle">
                    <input type="checkbox" id="auto-feed-toggle" onchange="setAutoFeed(this.checked)">
                    <span class="slider"></span>
                </label>
            </div>
        </div>

        <div class="card">
            <h2 class="card-title">⏰ 定时设置</h2>
            <div class="schedule-grid">
                <div class="schedule-item">
                    <label>开灯时间</label>
                    <input type="time" id="light-on-time" value="08:00" onchange="setLightSchedule()">
                </div>
                <div class="schedule-item">
                    <label>关灯时间</label>
                    <input type="time" id="light-off-time" value="20:00" onchange="setLightSchedule()">
                </div>
                <div class="schedule-item">
                    <label>喂食时间</label>
                    <input type="time" id="feed-time" value="09:00" onchange="setLightSchedule()">
                </div>
                <div class="schedule-item">
                    <label>目标温度</label>
                    <input type="number" id="target-temp" value="25" min="18" max="32" step="0.5" onchange="setTargetTemp(this.value)">
                </div>
            </div>
        </div>

        <div class="card">
            <h2 class="card-title">⚙️ WiFi设置</h2>
            <div class="control-row">
                <span class="control-label">当前SSID</span>
                <span id="wifi-ssid">--</span>
            </div>
            <div class="control-row">
                <span class="control-label">信号强度</span>
                <span id="wifi-rssi">-- dBm</span>
            </div>
        </div>

        <div class="footer">
            <p>ESP32-C3 Smart Fish Tank | <span id="version">v1.0.0</span></p>
        </div>
    </div>

    <div class="toast" id="toast"></div>

    <script>
        let currentLedColor = 0xffffff;
        let currentBrightness = 128;

        function showToast(message) {
            const toast = document.getElementById('toast');
            toast.textContent = message;
            toast.classList.add('show');
            setTimeout(() => toast.classList.remove('show'), 3000);
        }

        function togglePump(enabled) {
            fetch('/api/pump?state=' + (enabled ? '1' : '0'))
                .then(r => r.json())
                .then(data => {
                    if (data.success) showToast('水泵' + (enabled ? '开启' : '关闭'));
                    else showToast('操作失败');
                })
                .catch(() => showToast('网络错误'));
        }

        function toggleLed(enabled) {
            fetch('/api/led?state=' + (enabled ? '1' : '0'))
                .then(r => r.json())
                .then(data => {
                    if (data.success) showToast('灯光' + (enabled ? '开启' : '关闭'));
                    else showToast('操作失败');
                })
                .catch(() => showToast('网络错误'));
        }

        function setBrightness(value) {
            currentBrightness = parseInt(value);
            fetch('/api/led?brightness=' + value + '&color=' + currentLedColor.toString(16))
                .then(r => r.json())
                .catch(() => {});
        }

        function setColor(color) {
            currentLedColor = color;
            document.querySelectorAll('.color-btn').forEach(btn => btn.classList.remove('selected'));
            event.target.classList.add('selected');
            fetch('/api/led?brightness=' + currentBrightness + '&color=' + color.toString(16))
                .then(r => r.json())
                .then(data => {
                    if (data.success) showToast('颜色已更改');
                })
                .catch(() => {});
        }

        function feedNow() {
            fetch('/api/feed')
                .then(r => r.json())
                .then(data => {
                    if (data.success) showToast('喂食完成！');
                    else showToast('操作失败');
                })
                .catch(() => showToast('网络错误'));
        }

        function setAutoLight(enabled) {
            fetch('/api/schedule?type=light&auto=' + (enabled ? '1' : '0'))
                .then(r => r.json())
                .then(data => {
                    if (data.success) showToast('自动灯光' + (enabled ? '开启' : '关闭'));
                })
                .catch(() => showToast('网络错误'));
        }

        function setAutoFeed(enabled) {
            fetch('/api/schedule?type=feed&auto=' + (enabled ? '1' : '0'))
                .then(r => r.json())
                .then(data => {
                    if (data.success) showToast('自动喂食' + (enabled ? '开启' : '关闭'));
                })
                .catch(() => showToast('网络错误'));
        }

        function setLightSchedule() {
            const lightOn = document.getElementById('light-on-time').value;
            const lightOff = document.getElementById('light-off-time').value;
            const feedTime = document.getElementById('feed-time').value;
            
            fetch('/api/schedule?lightOn=' + lightOn + '&lightOff=' + lightOff + '&feed=' + feedTime)
                .then(r => r.json())
                .then(data => {
                    if (data.success) showToast('定时设置已保存');
                })
                .catch(() => showToast('网络错误'));
        }

        function setTargetTemp(temp) {
            fetch('/api/config?targetTemp=' + temp)
                .then(r => r.json())
                .then(data => {
                    if (data.success) showToast('目标温度已设置');
                })
                .catch(() => showToast('网络错误'));
        }

        function updateStatus() {
            fetch('/api/status')
                .then(r => r.json())
                .then(data => {
                    document.getElementById('temperature').textContent = data.temperature ? data.temperature.toFixed(1) : '--';
                    document.getElementById('pump-status').textContent = data.pump ? 'ON' : 'OFF';
                    document.getElementById('led-status').textContent = data.led ? 'ON' : 'OFF';
                    document.getElementById('ip').textContent = data.ip || '--';
                    document.getElementById('pump-toggle').checked = data.pump;
                    document.getElementById('led-toggle').checked = data.led;
                    document.getElementById('brightness-slider').value = data.brightness || 128;
                    document.getElementById('wifi-ssid').textContent = data.ssid || '--';
                    document.getElementById('wifi-rssi').textContent = (data.rssi || 0) + ' dBm';
                    
                    if (data.uptime) {
                        const days = Math.floor(data.uptime / 86400);
                        const hours = Math.floor((data.uptime % 86400) / 3600);
                        const mins = Math.floor((data.uptime % 3600) / 60);
                        document.getElementById('uptime').textContent = '运行时间: ' + 
                            (days > 0 ? days + '天 ' : '') + hours + '小时' + mins + '分';
                    }
                })
                .catch(() => {});
        }

        function updateSchedule() {
            fetch('/api/schedule')
                .then(r => r.json())
                .then(data => {
                    if (data.lightOn) document.getElementById('light-on-time').value = data.lightOn;
                    if (data.lightOff) document.getElementById('light-off-time').value = data.lightOff;
                    if (data.feedTime) document.getElementById('feed-time').value = data.feedTime;
                    if (data.targetTemp) document.getElementById('target-temp').value = data.targetTemp;
                    document.getElementById('auto-light-toggle').checked = data.autoLight;
                    document.getElementById('auto-feed-toggle').checked = data.autoFeed;
                })
                .catch(() => {});
        }

        // 初始化
        updateStatus();
        updateSchedule();
        setInterval(updateStatus, 5000);
    </script>
</body>
</html>
)=====";

// ============================================
// API响应JSON模板
// ============================================
const char API_STATUS_OK[] PROGMEM = R"({"success":true})";
const char API_STATUS_ERROR[] PROGMEM = R"({"success":false,"error":"Operation failed"})";

#endif // WEBPAGE_H

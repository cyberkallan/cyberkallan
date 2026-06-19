#pragma once
/**
 * @file chat_ui.h
 * @brief NeoCAT V1 — CATroom Chat Interface (PROGMEM)
 *
 * Offline ESP32-S3 chat system. Join screen with avatar picker,
 * real-time WebSocket chat, admin drawer. Cyberpunk glassmorphism.
 * Zero external dependencies — everything inline.
 */

#include <pgmspace.h>

const char CHAT_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no,viewport-fit=cover">
<meta name="apple-mobile-web-app-capable" content="yes">
<meta name="apple-mobile-web-app-status-bar-style" content="black-translucent">
<meta name="theme-color" content="#050505">
<title>NeoCAT CATroom</title>
<style>
*,*::before,*::after{margin:0;padding:0;box-sizing:border-box}
:root{
  --bg:#050505;--s1:#0a0a0a;--s2:#111;--brd:rgba(255,255,255,.06);
  --g:#00ff41;--g2:#00dd38;--g3:rgba(0,255,65,.10);--g5:rgba(0,255,65,.22);--g6:rgba(0,255,65,.38);
  --cyan:#00d4ff;--cyan-d:rgba(0,212,255,.08);
  --red:#ff3b3b;--orange:#ff9800;
  --w:#e8e8e8;--dim:#666;--dim2:#444;
  --r:14px;--font:system-ui,-apple-system,'Segoe UI',sans-serif;
  --mono:'SF Mono','Fira Code','Cascadia Code',monospace;
  --ease:cubic-bezier(.4,0,.2,1);
}
html,body{height:100%;font-family:var(--font);background:var(--bg);color:var(--w);overflow:hidden;
  -webkit-tap-highlight-color:transparent;-webkit-font-smoothing:antialiased}
button{font-family:inherit;cursor:pointer;border:none;outline:none;-webkit-tap-highlight-color:transparent}
input{font-family:inherit;outline:none;border:none;background:none;color:var(--w)}
::-webkit-scrollbar{width:0;display:none}

@keyframes fadeIn{from{opacity:0}to{opacity:1}}
@keyframes slideUp{from{opacity:0;transform:translateY(12px)}to{opacity:1;transform:none}}
@keyframes slideRight{from{transform:translateX(100%)}to{transform:none}}
@keyframes pulse{0%,100%{transform:scale(1)}50%{transform:scale(1.3)}}
@keyframes breathe{0%,100%{opacity:.5}50%{opacity:1}}
@keyframes spin{to{transform:rotate(360deg)}}
@keyframes rippleOut{0%{transform:scale(0);opacity:.4}100%{transform:scale(4);opacity:0}}

/* ── App shell ─────────────────────────── */
#app{max-width:520px;margin:0 auto;height:100vh;height:100dvh;position:relative;display:flex;flex-direction:column;overflow:hidden}

/* ── Join Screen ───────────────────────── */
#joinScreen{position:absolute;inset:0;z-index:50;background:var(--bg);display:flex;align-items:center;justify-content:center;
  padding:env(safe-area-inset-top) 20px env(safe-area-inset-bottom);animation:fadeIn .5s var(--ease)}
#joinScreen.hidden{display:none}
.join-card{width:100%;max-width:360px;background:var(--s1);border:1px solid var(--brd);border-radius:20px;
  padding:32px 24px 24px;text-align:center;animation:slideUp .6s var(--ease)}
.join-emoji{font-size:3rem;margin-bottom:8px}
.join-room{font-size:1.3rem;font-weight:700;color:var(--w);margin-bottom:4px}
.join-sub{font-size:.7rem;color:var(--dim);margin-bottom:24px}
.join-label{display:block;font-size:.65rem;color:var(--dim);text-transform:uppercase;letter-spacing:2px;margin-bottom:8px;text-align:left}
.join-input{width:100%;height:48px;background:var(--s2);border:1px solid var(--brd);border-radius:var(--r);
  padding:0 16px;font-size:16px;color:var(--w);transition:border-color .2s}
.join-input:focus{border-color:var(--g)}
.join-input::placeholder{color:var(--dim2)}
.avatar-row{display:flex;justify-content:center;gap:16px;margin:20px 0 24px}
.avatar-opt{width:56px;height:56px;border-radius:50%;border:2px solid var(--brd);background:var(--s2);
  display:flex;align-items:center;justify-content:center;cursor:pointer;transition:all .25s var(--ease);position:relative;overflow:hidden}
.avatar-opt:active{transform:scale(.92)}
.avatar-opt.sel{border-color:var(--g);box-shadow:0 0 16px var(--g3)}
.avatar-opt svg{width:32px;height:32px}
.join-btn{width:100%;height:50px;border-radius:var(--r);background:linear-gradient(135deg,#00cc33,var(--g));
  color:#000;font-size:.95rem;font-weight:700;letter-spacing:1px;position:relative;overflow:hidden;transition:transform .15s,box-shadow .3s}
.join-btn:active{transform:scale(.97)}
.join-btn:hover{box-shadow:0 0 24px var(--g3)}
.join-btn .rip{position:absolute;border-radius:50%;background:rgba(255,255,255,.35);animation:rippleOut .6s linear forwards;pointer-events:none}
.powered{font-size:.55rem;color:var(--dim2);margin-top:16px;letter-spacing:1px}

/* ── Chat Screen ───────────────────────── */
#chatScreen{display:none;flex-direction:column;height:100%;animation:fadeIn .4s var(--ease)}
#chatScreen.active{display:flex}

/* Top bar */
.topbar{height:56px;min-height:56px;background:var(--s1);border-bottom:1px solid var(--brd);
  display:flex;align-items:center;padding:0 14px;gap:10px;
  padding-top:env(safe-area-inset-top);z-index:10}
.topbar-room{display:flex;align-items:center;gap:8px;flex:1;min-width:0}
.topbar-emoji{font-size:1.4rem}
.topbar-name{font-size:.95rem;font-weight:700;white-space:nowrap;overflow:hidden;text-overflow:ellipsis}
.online-badge{display:flex;align-items:center;gap:6px;font-size:.7rem;color:var(--dim);white-space:nowrap}
.online-dot{width:7px;height:7px;border-radius:50%;background:var(--g);animation:pulse 2s ease infinite}
.admin-btn{width:36px;height:36px;border-radius:50%;background:rgba(255,255,255,.04);display:none;
  align-items:center;justify-content:center;transition:background .2s}
.admin-btn:active{background:rgba(255,255,255,.1)}
.admin-btn svg{width:18px;height:18px;stroke:var(--dim);fill:none;stroke-width:1.8}

/* Messages */
.msg-area{flex:1;overflow-y:auto;padding:12px 14px 8px;display:flex;flex-direction:column;gap:4px;
  -webkit-overflow-scrolling:touch;scroll-behavior:smooth}
.msg-wrap{animation:slideUp .3s var(--ease) both}
.msg-row{display:flex;gap:8px;margin-bottom:2px;max-width:85%}
.msg-row.sent{margin-left:auto;flex-direction:row-reverse;max-width:78%}
.msg-avatar{width:24px;height:24px;border-radius:50%;background:var(--s2);display:flex;align-items:center;
  justify-content:center;flex-shrink:0;margin-top:18px}
.msg-avatar svg{width:16px;height:16px}
.msg-body{min-width:0}
.msg-sender{font-size:.65rem;font-weight:600;margin-bottom:2px;padding-left:4px}
.msg-bubble{padding:9px 13px;border-radius:14px;font-size:.88rem;line-height:1.45;word-wrap:break-word;word-break:break-word}
.msg-row:not(.sent) .msg-bubble{background:rgba(255,255,255,.04);border-left:2px solid var(--cyan)}
.msg-row.sent .msg-bubble{background:rgba(0,255,65,.12);border-left:2px solid var(--g)}
.msg-time{font-size:.55rem;color:var(--dim2);font-family:var(--mono);margin-top:2px;padding-left:4px}
.msg-row.sent .msg-time{text-align:right;padding-right:4px;padding-left:0}
.sys-msg{text-align:center;font-size:.72rem;color:var(--dim);font-style:italic;padding:6px 0;animation:breathe 3s ease infinite}

/* Scroll FAB */
.scroll-fab{position:absolute;bottom:72px;right:16px;width:36px;height:36px;border-radius:50%;
  background:var(--s2);border:1px solid var(--brd);display:none;align-items:center;justify-content:center;
  z-index:5;box-shadow:0 4px 16px rgba(0,0,0,.5);cursor:pointer;transition:transform .2s}
.scroll-fab:active{transform:scale(.9)}
.scroll-fab svg{width:16px;height:16px;stroke:var(--w);fill:none;stroke-width:2}

/* Input bar */
.input-bar{min-height:60px;background:rgba(10,10,10,.85);backdrop-filter:blur(20px);-webkit-backdrop-filter:blur(20px);
  border-top:1px solid var(--brd);display:flex;align-items:center;gap:10px;
  padding:8px 14px;padding-bottom:max(8px,env(safe-area-inset-bottom))}
.input-bar input{flex:1;height:42px;background:var(--s2);border:1px solid var(--brd);border-radius:22px;
  padding:0 16px;font-size:16px;color:var(--w);transition:border-color .2s}
.input-bar input:focus{border-color:var(--g)}
.input-bar input::placeholder{color:var(--dim2)}
.send-btn{width:42px;height:42px;border-radius:50%;background:var(--g);display:flex;align-items:center;
  justify-content:center;flex-shrink:0;transition:transform .15s,opacity .2s}
.send-btn:active{transform:scale(.88)}
.send-btn svg{width:18px;height:18px;fill:#000;stroke:none}

/* ── Admin Drawer ──────────────────────── */
.drawer-overlay{position:fixed;inset:0;background:rgba(0,0,0,.6);backdrop-filter:blur(4px);-webkit-backdrop-filter:blur(4px);
  z-index:90;display:none;animation:fadeIn .2s}
.drawer-overlay.open{display:block}
.drawer{position:fixed;top:0;right:0;bottom:0;width:280px;max-width:85vw;background:var(--s1);border-left:1px solid var(--brd);
  z-index:91;transform:translateX(100%);transition:transform .3s var(--ease);overflow-y:auto;
  padding:env(safe-area-inset-top) 0 env(safe-area-inset-bottom)}
.drawer.open{transform:none}
.drawer-hdr{display:flex;align-items:center;justify-content:space-between;padding:18px 16px 14px;
  border-bottom:1px solid var(--brd)}
.drawer-hdr h3{font-size:.95rem;font-weight:700}
.close-btn{width:32px;height:32px;border-radius:50%;background:rgba(255,255,255,.05);display:flex;
  align-items:center;justify-content:center;transition:background .2s}
.close-btn:active{background:rgba(255,255,255,.12)}
.close-btn svg{width:16px;height:16px;stroke:var(--dim);fill:none;stroke-width:2}
.drawer-section{padding:16px}
.drawer-section h4{font-size:.65rem;color:var(--dim);text-transform:uppercase;letter-spacing:2px;margin-bottom:12px}
.drawer-section+.drawer-section{border-top:1px solid var(--brd)}
.d-input{width:100%;height:40px;background:var(--s2);border:1px solid var(--brd);border-radius:10px;
  padding:0 12px;font-size:.85rem;color:var(--w);margin-bottom:10px}
.d-input:focus{border-color:var(--g)}
.emoji-row{display:flex;gap:10px;margin-bottom:14px}
.emoji-opt{width:38px;height:38px;border-radius:10px;background:var(--s2);border:1px solid var(--brd);
  display:flex;align-items:center;justify-content:center;font-size:1.2rem;cursor:pointer;transition:all .2s}
.emoji-opt.sel{border-color:var(--g);background:var(--g3)}
.d-btn{width:100%;height:40px;border-radius:10px;font-size:.8rem;font-weight:600;transition:transform .15s,opacity .2s}
.d-btn:active{transform:scale(.97)}
.d-btn.green{background:var(--g);color:#000}
.d-btn.orange{background:var(--orange);color:#000;margin-bottom:10px}
.d-btn.red{background:var(--red);color:#fff}
.user-item{display:flex;align-items:center;gap:10px;padding:8px 0;border-bottom:1px solid var(--brd)}
.user-item:last-child{border-bottom:none}
.user-av{width:28px;height:28px;border-radius:50%;background:var(--s2);display:flex;align-items:center;justify-content:center;flex-shrink:0}
.user-av svg{width:18px;height:18px}
.user-info{flex:1;min-width:0}
.user-name{font-size:.8rem;font-weight:600;white-space:nowrap;overflow:hidden;text-overflow:ellipsis}
.user-meta{font-size:.55rem;color:var(--dim);font-family:var(--mono)}
.kick-btn{padding:4px 10px;border-radius:8px;background:rgba(255,59,59,.12);color:var(--red);
  font-size:.65rem;font-weight:600;flex-shrink:0;transition:background .2s}
.kick-btn:active{background:rgba(255,59,59,.25)}

/* ── Overlays ──────────────────────────── */
.overlay{position:fixed;inset:0;z-index:100;background:rgba(5,5,5,.92);backdrop-filter:blur(8px);
  display:none;align-items:center;justify-content:center;flex-direction:column;gap:16px;animation:fadeIn .3s}
.overlay.show{display:flex}
.overlay-spin{width:28px;height:28px;border:2px solid var(--dim2);border-top-color:var(--g);border-radius:50%;animation:spin .8s linear infinite}
.overlay-text{font-size:.85rem;color:var(--dim)}
.overlay-kick{font-size:1.1rem;color:var(--red);font-weight:700}
</style>
</head>
<body>
<div id="app">

<!-- ═══ JOIN SCREEN ═══ -->
<div id="joinScreen">
<div class="join-card">
  <div class="join-emoji" id="jEmoji">🐱</div>
  <div class="join-room" id="jRoom">CATroom</div>
  <div class="join-sub">Offline Encrypted Chat</div>
  <label class="join-label">Your Name</label>
  <input class="join-input" id="jName" type="text" maxlength="20" placeholder="Enter a handle..." autocomplete="off" spellcheck="false">
  <label class="join-label" style="margin-top:18px">Choose Avatar</label>
  <div class="avatar-row" id="avRow"></div>
  <button class="join-btn" id="jBtn" onclick="doJoin(event)">Enter CATroom</button>
  <div class="powered">Powered by NeoCAT V1</div>
</div>
</div>

<!-- ═══ CHAT SCREEN ═══ -->
<div id="chatScreen">
  <div class="topbar">
    <div class="topbar-room">
      <span class="topbar-emoji" id="tEmoji">🐱</span>
      <span class="topbar-name" id="tName">CATroom</span>
    </div>
    <div class="online-badge"><span class="online-dot"></span><span id="tOnline">0 online</span></div>
    <button class="admin-btn" id="adminBtn" onclick="toggleDrawer(true)">
      <svg viewBox="0 0 24 24"><path d="M12 15.5A3.5 3.5 0 1 0 12 8.5a3.5 3.5 0 0 0 0 7z"/><path d="M19.43 12.98a7.8 7.8 0 0 0 .07-1 7.8 7.8 0 0 0-.07-1l2.11-1.65a.5.5 0 0 0 .12-.64l-2-3.46a.5.5 0 0 0-.61-.22l-2.49 1a7.3 7.3 0 0 0-1.69-.98l-.38-2.65A.49.49 0 0 0 14 2h-4a.49.49 0 0 0-.49.42l-.38 2.65a7.3 7.3 0 0 0-1.69.98l-2.49-1a.5.5 0 0 0-.61.22l-2 3.46a.49.49 0 0 0 .12.64l2.11 1.65a7.9 7.9 0 0 0 0 2l-2.11 1.65a.5.5 0 0 0-.12.64l2 3.46a.5.5 0 0 0 .61.22l2.49-1a7.3 7.3 0 0 0 1.69.98l.38 2.65c.05.24.26.42.49.42h4c.24 0 .44-.18.49-.42l.38-2.65a7.3 7.3 0 0 0 1.69-.98l2.49 1a.5.5 0 0 0 .61-.22l2-3.46a.49.49 0 0 0-.12-.64z"/></svg>
    </button>
  </div>
  <div class="msg-area" id="msgArea"></div>
  <button class="scroll-fab" id="scrollFab" onclick="scrollBottom()">
    <svg viewBox="0 0 24 24"><polyline points="6 9 12 15 18 9"/></svg>
  </button>
  <div class="input-bar">
    <input id="msgInput" type="text" placeholder="Type a message..." autocomplete="off" spellcheck="false" maxlength="500" onkeydown="if(event.key==='Enter')sendMsg()">
    <button class="send-btn" onclick="sendMsg()">
      <svg viewBox="0 0 24 24"><path d="M2 21l21-9L2 3v7l15 2-15 2z"/></svg>
    </button>
  </div>
</div>

<!-- ═══ ADMIN DRAWER ═══ -->
<div class="drawer-overlay" id="dOverlay" onclick="toggleDrawer(false)"></div>
<div class="drawer" id="drawer">
  <div class="drawer-hdr">
    <h3>Admin Panel</h3>
    <button class="close-btn" onclick="toggleDrawer(false)">
      <svg viewBox="0 0 24 24"><line x1="18" y1="6" x2="6" y2="18"/><line x1="6" y1="6" x2="18" y2="18"/></svg>
    </button>
  </div>
  <div class="drawer-section">
    <h4>Room Settings</h4>
    <input class="d-input" id="dRoomName" placeholder="Room name">
    <input class="d-input" id="dRoomPass" placeholder="Password (empty = open)">
    <div class="emoji-row" id="emojiRow"></div>
    <button class="d-btn green" onclick="updateRoom()">Update Room</button>
  </div>
  <div class="drawer-section">
    <h4>Connected Users</h4>
    <div id="dUsers"></div>
  </div>
  <div class="drawer-section">
    <h4>Danger Zone</h4>
    <button class="d-btn orange" onclick="clearMessages()">Clear Messages</button>
    <button class="d-btn red" onclick="stopRoom()">Stop Room</button>
  </div>
</div>

<!-- ═══ OVERLAYS ═══ -->
<div class="overlay" id="ovReconnect">
  <div class="overlay-spin"></div>
  <div class="overlay-text">Reconnecting…</div>
</div>
<div class="overlay" id="ovKicked">
  <div class="overlay-kick">You were kicked</div>
  <div class="overlay-text">Disconnected by admin</div>
</div>

</div><!-- /app -->

<script>
/* ══ Shortcuts ══ */
const $=s=>document.querySelector(s),$$=s=>document.querySelectorAll(s);

/* ══ Avatar SVGs ══ */
const AVS=[
// 0: Ghost — hooded figure, cyan eyes
`<svg viewBox="0 0 64 64" fill="none" xmlns="http://www.w3.org/2000/svg"><path d="M32 4C18 4 10 16 10 28v24c0 2 1 4 4 4h4l2-6 2 6h6l2-6 2 6h6l2-6 2 6h4c3 0 4-2 4-4V28C50 16 46 4 32 4z" fill="#2a1a3e" stroke="#6b4f9e" stroke-width="1.5"/><path d="M14 20c0 0 4-12 18-12s18 12 18 12" fill="#1a0e2e"/><circle cx="24" cy="30" r="3" fill="#00d4ff"><animate attributeName="opacity" values="1;.4;1" dur="2.5s" repeatCount="indefinite"/></circle><circle cx="40" cy="30" r="3" fill="#00d4ff"><animate attributeName="opacity" values="1;.4;1" dur="2.5s" begin=".3s" repeatCount="indefinite"/></circle></svg>`,
// 1: Skull — cyber skull, magenta accents
`<svg viewBox="0 0 64 64" fill="none" xmlns="http://www.w3.org/2000/svg"><path d="M32 6C19 6 12 16 12 28c0 8 3 14 8 18v6c0 2 1 3 3 3h18c2 0 3-1 3-3v-6c5-4 8-10 8-18C52 16 45 6 32 6z" fill="#1a1a2e" stroke="#ff2d95" stroke-width="1.5"/><rect x="20" y="22" width="10" height="8" rx="2" fill="#0a0a1a" stroke="#ff2d95" stroke-width="1"/><rect x="34" y="22" width="10" height="8" rx="2" fill="#0a0a1a" stroke="#ff2d95" stroke-width="1"/><path d="M28 40v6M32 40v8M36 40v6" stroke="#ff2d95" stroke-width="1.5"/><path d="M22 40h20" stroke="#ff2d95" stroke-width="1.5"/><path d="M20 38c4 3 8 4 12 4s8-1 12-4" stroke="#ff2d95" stroke-width="1" fill="none"/></svg>`,
// 2: CyberCat — cat face, green neon, goggles
`<svg viewBox="0 0 64 64" fill="none" xmlns="http://www.w3.org/2000/svg"><path d="M12 52V24L8 8l14 10h20L56 8l-4 16v28c0 2-2 4-4 4H16c-2 0-4-2-4-4z" fill="#0a1a0a" stroke="#00ff41" stroke-width="1.5"/><ellipse cx="24" cy="30" rx="7" ry="6" fill="#0a0a0a" stroke="#00ff41" stroke-width="1.2"/><ellipse cx="40" cy="30" rx="7" ry="6" fill="#0a0a0a" stroke="#00ff41" stroke-width="1.2"/><circle cx="24" cy="30" r="3" fill="#00ff41" opacity=".8"/><circle cx="40" cy="30" r="3" fill="#00ff41" opacity=".8"/><line x1="31" y1="28" x2="33" y2="28" stroke="#00ff41" stroke-width="1.2"/><path d="M28 40c2 2 6 2 8 0" stroke="#00ff41" stroke-width="1.2" fill="none"/><circle cx="32" cy="38" r="1.5" fill="#00ff41"/><path d="M20 44l-6 4M44 44l6 4M20 46l-5 2M44 46l5 2" stroke="#00ff41" stroke-width="1" opacity=".5"/></svg>`,
// 3: Bot — round robot, orange, antenna
`<svg viewBox="0 0 64 64" fill="none" xmlns="http://www.w3.org/2000/svg"><circle cx="32" cy="8" r="3" fill="#ff9800"/><line x1="32" y1="11" x2="32" y2="18" stroke="#ff9800" stroke-width="2"/><rect x="14" y="18" width="36" height="32" rx="16" fill="#1a1208" stroke="#ff9800" stroke-width="1.5"/><rect x="22" y="28" width="8" height="6" rx="1" fill="#ff9800" opacity=".9"/><rect x="34" y="28" width="8" height="6" rx="1" fill="#ff9800" opacity=".9"/><path d="M26 42h12" stroke="#ff9800" stroke-width="1.5" stroke-linecap="round"/><line x1="10" y1="34" x2="14" y2="34" stroke="#ff9800" stroke-width="2" stroke-linecap="round"/><line x1="50" y1="34" x2="54" y2="34" stroke="#ff9800" stroke-width="2" stroke-linecap="round"/><rect x="20" y="52" width="8" height="4" rx="2" fill="#ff9800" opacity=".6"/><rect x="36" y="52" width="8" height="4" rx="2" fill="#ff9800" opacity=".6"/></svg>`
];

const AV_COLORS=['#b06aff','#ff2d95','#00ff41','#ff9800'];
const EMOJIS=['🐱','💀','👾','🔒'];

/* ══ State ══ */
let ws=null,myName='',myAvatar=0,myId=0,isAdmin=false;
let messages=[],users=[],lastMsgId=0,reconDelay=1000;
let roomEmoji='🐱',roomName='CATroom',selEmoji=0;
let autoScroll=true;

/* ══ Init ══ */
(function init(){
  // Build avatar picker
  const row=$('#avRow');
  AVS.forEach((svg,i)=>{
    const d=document.createElement('div');
    d.className='avatar-opt'+(i===0?' sel':'');
    d.innerHTML=svg;
    d.onclick=()=>{$$('.avatar-opt').forEach(e=>e.classList.remove('sel'));d.classList.add('sel');myAvatar=i};
    row.appendChild(d);
  });
  // Build emoji picker
  const er=$('#emojiRow');
  EMOJIS.forEach((em,i)=>{
    const d=document.createElement('div');
    d.className='emoji-opt'+(i===0?' sel':'');
    d.textContent=em;
    d.onclick=()=>{$$('.emoji-opt').forEach(e=>e.classList.remove('sel'));d.classList.add('sel');selEmoji=i};
    er.appendChild(d);
  });
  // Fetch room info
  fetch('/api/chat/room').then(r=>r.json()).then(d=>{
    if(d.name){roomName=d.name;$('#jRoom').textContent=d.name;$('#tName').textContent=d.name}
    if(d.emoji){roomEmoji=d.emoji;$('#jEmoji').textContent=d.emoji;$('#tEmoji').textContent=d.emoji}
  }).catch(()=>{});
  // Scroll detection
  const ma=$('#msgArea');
  ma.addEventListener('scroll',()=>{
    const diff=ma.scrollHeight-ma.scrollTop-ma.clientHeight;
    autoScroll=diff<60;
    $('#scrollFab').style.display=autoScroll?'none':'flex';
  });
  // Focus name input
  setTimeout(()=>$('#jName').focus(),400);
})();

/* ══ Join ══ */
function doJoin(ev){
  ripple(ev);
  const name=$('#jName').value.trim();
  if(!name){$('#jName').style.borderColor='var(--red)';$('#jName').focus();return}
  myName=name;
  $('#joinScreen').classList.add('hidden');
  $('#chatScreen').classList.add('active');
  if(isAdmin)$('#adminBtn').style.display='flex';
  connectWS();
}

/* ══ Ripple ══ */
function ripple(ev){
  const btn=ev.currentTarget,r=btn.getBoundingClientRect();
  const s=Math.max(r.width,r.height);
  const el=document.createElement('span');
  el.className='rip';
  el.style.cssText=`width:${s}px;height:${s}px;left:${ev.clientX-r.left-s/2}px;top:${ev.clientY-r.top-s/2}px`;
  btn.appendChild(el);
  setTimeout(()=>el.remove(),600);
}

/* ══ WebSocket ══ */
function connectWS(){
  if(ws&&(ws.readyState===0||ws.readyState===1))return;
  const h=location.host;
  ws=new WebSocket('ws://'+h+'/chat-ws');
  ws.onopen=()=>{
    reconDelay=1000;
    $('#ovReconnect').classList.remove('show');
    ws.send(JSON.stringify({cmd:'join',name:myName,avatar:myAvatar}));
  };
  ws.onmessage=ev=>{
    let d;try{d=JSON.parse(ev.data)}catch(e){return}
    switch(d.type){
      case 'welcome':
        myId=d.clientId||0;
        isAdmin=!!d.isAdmin;
        if(isAdmin)$('#adminBtn').style.display='flex';
        if(d.room){
          roomName=d.room.name||roomName;roomEmoji=d.room.emoji||roomEmoji;
          $('#tName').textContent=roomName;$('#tEmoji').textContent=roomEmoji;
          $('#dRoomName').value=roomName;
        }
        if(d.messages&&d.messages.length){
          messages=d.messages;renderAllMsgs();
        }
        if(d.users){users=d.users;updateUsers()}
        break;
      case 'message':
        messages.push(d);
        renderMsg(d);
        trimDOM();
        if(autoScroll)scrollBottom();
        break;
      case 'system':
        messages.push(d);
        renderSys(d);
        trimDOM();
        if(autoScroll)scrollBottom();
        break;
      case 'users':
        users=d.users||[];updateUsers();
        break;
      case 'room_update':
        if(d.name){roomName=d.name;$('#tName').textContent=d.name}
        if(d.emoji){roomEmoji=d.emoji;$('#tEmoji').textContent=d.emoji}
        break;
      case 'kicked':
        ws.close();ws=null;
        $('#ovKicked').classList.add('show');
        break;
      case 'cleared':
        messages=[];$('#msgArea').innerHTML='';
        break;
    }
  };
  ws.onclose=()=>{
    if($('#ovKicked').classList.contains('show'))return;
    $('#ovReconnect').classList.add('show');
    setTimeout(()=>connectWS(),reconDelay);
    reconDelay=Math.min(reconDelay*2,16000);
  };
  ws.onerror=()=>ws.close();
}

/* ══ Sending ══ */
function sendMsg(){
  const inp=$('#msgInput'),text=inp.value.trim();
  if(!text||!ws||ws.readyState!==1)return;
  ws.send(JSON.stringify({cmd:'msg',text:text}));
  inp.value='';inp.focus();
}

/* ══ Rendering ══ */
function renderAllMsgs(){
  const ma=$('#msgArea');ma.innerHTML='';
  const slice=messages.slice(-100);
  slice.forEach(m=>{if(m.type==='system')renderSys(m);else renderMsg(m)});
  scrollBottom();
}

function renderMsg(m){
  const ma=$('#msgArea');
  const sent=m.clientId===myId;
  const wrap=document.createElement('div');
  wrap.className='msg-wrap';
  const row=document.createElement('div');
  row.className='msg-row'+(sent?' sent':'');
  if(!sent){
    const av=document.createElement('div');
    av.className='msg-avatar';
    av.innerHTML=AVS[m.avatar||0]||AVS[0];
    row.appendChild(av);
  }
  const body=document.createElement('div');
  body.className='msg-body';
  if(!sent){
    const sn=document.createElement('div');
    sn.className='msg-sender';
    sn.style.color=AV_COLORS[m.avatar||0];
    sn.textContent=m.name||'Anon';
    body.appendChild(sn);
  }
  const bbl=document.createElement('div');
  bbl.className='msg-bubble';
  bbl.textContent=m.text||'';
  body.appendChild(bbl);
  const ts=document.createElement('div');
  ts.className='msg-time';
  ts.textContent=fmtTime(m.time);
  body.appendChild(ts);
  row.appendChild(body);
  wrap.appendChild(row);
  ma.appendChild(wrap);
}

function renderSys(m){
  const ma=$('#msgArea');
  const el=document.createElement('div');
  el.className='sys-msg msg-wrap';
  el.textContent=m.text||'';
  ma.appendChild(el);
}

function trimDOM(){
  const ma=$('#msgArea');
  while(ma.children.length>100)ma.removeChild(ma.firstChild);
}

function scrollBottom(){
  const ma=$('#msgArea');
  requestAnimationFrame(()=>{ma.scrollTop=ma.scrollHeight});
  autoScroll=true;$('#scrollFab').style.display='none';
}

function updateUsers(){
  $('#tOnline').textContent=users.length+' online';
  // Admin drawer user list
  const c=$('#dUsers');c.innerHTML='';
  users.forEach(u=>{
    const d=document.createElement('div');
    d.className='user-item';
    d.innerHTML=`<div class="user-av">${AVS[u.avatar||0]||AVS[0]}</div>
      <div class="user-info"><div class="user-name">${esc(u.name)}</div>
      <div class="user-meta">${esc(u.ip||'')} · ${fmtUptime(u.joined)}</div></div>`;
    if(isAdmin&&u.clientId!==myId){
      const kb=document.createElement('button');
      kb.className='kick-btn';kb.textContent='Kick';
      kb.onclick=()=>kickUser(u.clientId);
      d.appendChild(kb);
    }
    c.appendChild(d);
  });
}

/* ══ Helpers ══ */
function esc(s){const d=document.createElement('div');d.textContent=s;return d.innerHTML}
function fmtTime(t){if(!t)return '';try{const d=new Date(t);return d.getHours().toString().padStart(2,'0')+':'+d.getMinutes().toString().padStart(2,'0')}catch(e){return ''}}
function fmtUptime(s){if(!s&&s!==0)return '';const m=Math.floor(s/60),h=Math.floor(m/60);return h>0?h+'h '+m%60+'m':m+'m'}

function post(url,body){return fetch(url,{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify(body)})}

/* ══ Admin actions ══ */
function toggleDrawer(open){
  $('#dOverlay').classList.toggle('open',open);
  $('#drawer').classList.toggle('open',open);
  if(open){$('#dRoomName').value=roomName}
}

async function kickUser(id){try{await post('/api/chat/kick',{clientId:id})}catch(e){}}
async function updateRoom(){
  const name=$('#dRoomName').value.trim()||roomName;
  const password=$('#dRoomPass').value;
  const emoji=EMOJIS[selEmoji];
  try{await post('/api/chat/room',{name,password,emoji});toggleDrawer(false)}catch(e){}
}
async function clearMessages(){try{await post('/api/chat/clear',{});toggleDrawer(false)}catch(e){}}
async function stopRoom(){if(confirm('Stop the room?')){try{await post('/api/chat/room/stop',{});toggleDrawer(false)}catch(e){}}}
</script>
</body>
</html>
)rawliteral";

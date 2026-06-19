#pragma once
/**
 * @file web_dashboard.h
 * @brief NeoCAT V1 — Premium Mobile-App Dashboard (PROGMEM)
 *
 * Powered by Glyph S3. PWA-ready. Ripple effects, staggered
 * card animations, sliding tab indicator, animated boot screen,
 * frosted glass navigation. Zero external dependencies.
 */

#include <pgmspace.h>

/* ── PWA Manifest (served at /manifest.json) ─────────────────────────── */
const char PWA_MANIFEST[] PROGMEM = R"json({
  "name":"NeoCAT V1 — Glyph S3",
  "short_name":"NeoCAT",
  "description":"ESP32-S3 Security Toolkit powered by Glyph S3",
  "start_url":"/",
  "display":"standalone",
  "orientation":"portrait",
  "background_color":"#050505",
  "theme_color":"#050505",
  "categories":["utilities","security"],
  "icons":[
    {"src":"data:image/svg+xml,<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 100 100'><rect fill='%23050505' width='100' height='100' rx='15'/><g stroke='%2300ff41' stroke-width='3' stroke-linejoin='round' stroke-linecap='round' fill='none'><path d='M 50 90 L 20 45 L 15 10 L 40 25 L 50 20 L 60 25 L 85 10 L 80 45 Z'/><path d='M 20 45 L 50 70 L 80 45'/><path d='M 40 25 L 50 50 L 60 25'/><path d='M 32 42 L 44 48' stroke-width='4'/><path d='M 68 42 L 56 48' stroke-width='4'/></g></svg>","sizes":"any","type":"image/svg+xml"}
  ]
})json";

/* ── Main Dashboard HTML ─────────────────────────────────────────────── */
const char DASHBOARD_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no,viewport-fit=cover">
<meta name="apple-mobile-web-app-capable" content="yes">
<meta name="apple-mobile-web-app-status-bar-style" content="black-translucent">
<meta name="mobile-web-app-capable" content="yes">
<meta name="theme-color" content="#050505">
<meta name="apple-mobile-web-app-title" content="NeoCAT">
<meta name="application-name" content="NeoCAT V1">
<link rel="manifest" href="/manifest.json">
<link rel="icon" href="data:image/svg+xml,<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 100 100' stroke='%2300ff41' stroke-width='3' stroke-linejoin='round' stroke-linecap='round' fill='none'><path d='M 50 90 L 20 45 L 15 10 L 40 25 L 50 20 L 60 25 L 85 10 L 80 45 Z'/><path d='M 20 45 L 50 70 L 80 45'/><path d='M 40 25 L 50 50 L 60 25'/><path d='M 32 42 L 44 48' stroke-width='4'/><path d='M 68 42 L 56 48' stroke-width='4'/></svg>">
<title>NeoCAT V1</title>
<style>
/* ═══════════════════════════════════════════════════════════════════
   ROOT / RESET / TOKENS
   ═══════════════════════════════════════════════════════════════════ */
*,*::before,*::after{margin:0;padding:0;box-sizing:border-box}
:root{
  --bg:#050505;--s1:#0a0a0a;--s2:#111;--s3:#191919;--s4:#222;
  --b1:#1a1a1a;--b2:#2a2a2a;--b3:#333;
  --g:#00ff41;--g2:#00dd38;--g3:rgba(0,255,65,.10);--g4:rgba(0,255,65,.05);
  --g5:rgba(0,255,65,.22);--g6:rgba(0,255,65,.38);--g7:rgba(0,255,65,.55);
  --red:#ff3b3b;--red-d:rgba(255,59,59,.10);--red-b:rgba(255,59,59,.22);
  --warn:#ffb800;--warn-d:rgba(255,184,0,.10);
  --purple:#b06aff;--purple-d:rgba(176,106,255,.10);--purple-b:rgba(176,106,255,.22);
  --cyan:#00e5ff;--cyan-d:rgba(0,229,255,.08);
  --w:#f0f0f0;--w2:#ccc;--dim:#888;--dim2:#555;--dim3:#333;
  --r:16px;--rs:10px;--rr:24px;
  --nav:68px;--top:54px;
  --font:'Inter',-apple-system,BlinkMacSystemFont,'Segoe UI',system-ui,sans-serif;
  --mono:'JetBrains Mono','Fira Code','SF Mono','Cascadia Code',monospace;
  --ease:cubic-bezier(.4,0,.2,1);--spring:cubic-bezier(.34,1.56,.64,1);
}
html,body{height:100%;font-family:var(--font);background:var(--bg);color:var(--w);overflow:hidden;-webkit-tap-highlight-color:transparent;-webkit-font-smoothing:antialiased}
button{font-family:inherit;cursor:pointer;border:none;outline:none;-webkit-tap-highlight-color:transparent}
input,textarea,select{font-family:inherit;outline:none}
::-webkit-scrollbar{width:0;display:none}

/* ═══════════════════════════════════════════════════════════════════
   ANIMATIONS
   ═══════════════════════════════════════════════════════════════════ */
@keyframes fadeIn{from{opacity:0}to{opacity:1}}
@keyframes slideUp{from{opacity:0;transform:translateY(24px)}to{opacity:1;transform:none}}
@keyframes slideDown{from{opacity:0;transform:translateY(-12px)}to{opacity:1;transform:none}}
@keyframes scaleIn{from{opacity:0;transform:scale(.92)}to{opacity:1;transform:none}}
@keyframes pulse{0%,100%{opacity:1}50%{opacity:.3}}
@keyframes breathe{0%,100%{box-shadow:0 0 8px var(--g3)}50%{box-shadow:0 0 28px var(--g5)}}
@keyframes spin{to{transform:rotate(360deg)}}
@keyframes glow{0%,100%{box-shadow:0 0 10px var(--g3)}50%{box-shadow:0 0 30px var(--g5)}}
@keyframes blink{50%{opacity:0}}
@keyframes countPop{from{transform:scale(.8);opacity:.3}to{transform:scale(1);opacity:1}}
@keyframes toastIn{from{transform:translateY(-30px) scale(.95);opacity:0}to{transform:none;opacity:1}}
@keyframes toastOut{to{transform:translateY(-30px) scale(.95);opacity:0}}
@keyframes bootPulse{0%{transform:scale(1);filter:drop-shadow(0 0 15px var(--g3))}100%{transform:scale(1.08);filter:drop-shadow(0 0 45px var(--g6))}}
@keyframes gradientShift{0%{background-position:0% 50%}50%{background-position:100% 50%}100%{background-position:0% 50%}}
@keyframes borderFlow{0%{border-color:var(--g3)}33%{border-color:var(--cyan-d)}66%{border-color:var(--purple-b)}100%{border-color:var(--g3)}}
@keyframes rippleOut{0%{transform:scale(0);opacity:.45}100%{transform:scale(4);opacity:0}}
@keyframes matrixFade{0%{opacity:0;transform:translateY(-8px)}15%{opacity:.6}100%{opacity:0;transform:translateY(40px)}}
@keyframes shimmer{0%{background-position:-200% 0}100%{background-position:200% 0}}
@keyframes floatUp{0%{opacity:0;transform:translateY(6px)}50%{opacity:1}100%{opacity:0;transform:translateY(-6px)}}

/* ═══════════════════════════════════════════════════════════════════
   BOOT SCREEN
   ═══════════════════════════════════════════════════════════════════ */
#boot{
  position:fixed;inset:0;z-index:9999;background:var(--bg);
  display:flex;flex-direction:column;align-items:center;justify-content:center;
  transition:opacity .7s var(--ease),filter .7s var(--ease);
}
#boot.done{opacity:0;filter:blur(8px);pointer-events:none}
.b-particles{position:absolute;inset:0;overflow:hidden;opacity:.15}
.b-p{position:absolute;width:1px;background:var(--g);border-radius:1px;animation:matrixFade linear infinite;opacity:0}
.b-icon{animation:bootPulse 1.4s ease-in-out infinite alternate;z-index:1;filter:drop-shadow(0 0 15px rgba(0,255,65,0.4))}
.b-brand{font-size:2.4rem;font-weight:200;letter-spacing:14px;margin-top:.8rem;z-index:1;
  color:#fff;text-shadow:0 0 20px rgba(255,255,255,0.3);text-transform:uppercase;margin-left:14px}
.b-sub{font-size:.65rem;color:var(--g);letter-spacing:8px;margin-top:6px;text-transform:uppercase;z-index:1;font-weight:400}
.b-powered{font-size:.5rem;color:var(--dim);letter-spacing:5px;margin-top:10px;text-transform:uppercase;z-index:1}
.b-log{width:260px;margin-top:1.5rem;font-family:var(--mono);font-size:.58rem;color:var(--g2);line-height:1.9;min-height:80px;max-height:80px;overflow:hidden;z-index:1}
.b-progress{width:220px;height:3px;background:var(--s2);border-radius:2px;margin-top:1rem;overflow:hidden;z-index:1}
.b-bar{height:100%;width:0;background:linear-gradient(90deg,var(--g),var(--cyan));box-shadow:0 0 12px var(--g5);border-radius:2px;transition:width .2s var(--ease)}
.b-ver{margin-top:1.2rem;font-size:.5rem;color:var(--dim3);letter-spacing:3px;z-index:1}

/* ═══════════════════════════════════════════════════════════════════
   LEGAL MODAL
   ═══════════════════════════════════════════════════════════════════ */
#legal{
  position:fixed;inset:0;z-index:9998;background:rgba(0,0,0,.9);
  backdrop-filter:blur(16px);-webkit-backdrop-filter:blur(16px);
  display:none;align-items:center;justify-content:center;padding:1rem;
}
#legal.active{display:flex}
.l-card{
  background:var(--s1);border:1px solid var(--b1);border-radius:var(--r);
  padding:2rem;max-width:400px;width:100%;text-align:center;
  box-shadow:0 24px 64px rgba(0,0,0,.6),0 0 0 1px var(--b1);animation:scaleIn .4s var(--spring);
}
.l-icon{font-size:2.5rem;margin-bottom:.7rem}
.l-title{font-size:1.05rem;font-weight:700;margin-bottom:.8rem;color:var(--g)}
.l-body{text-align:left;font-size:.76rem;color:var(--dim);line-height:1.75;margin-bottom:1.3rem}
.l-body strong{color:var(--g)}.l-body ul{margin:.5rem 0 0 1.2rem}.l-body li{margin:.3rem 0}
.l-actions{display:flex;gap:.8rem;justify-content:center}

/* ═══════════════════════════════════════════════════════════════════
   APP SHELL
   ═══════════════════════════════════════════════════════════════════ */
#app{display:none;flex-direction:column;height:100vh;height:100dvh;overflow:hidden;max-width:520px;margin:0 auto;position:relative}
#app.visible{display:flex}

/* ── TOPBAR ──────────────────────────────────────────────────────── */
.topbar{
  height:var(--top);display:flex;align-items:center;justify-content:space-between;
  padding:0 16px;
  background:rgba(5,5,5,.85);backdrop-filter:saturate(180%) blur(20px);-webkit-backdrop-filter:saturate(180%) blur(20px);
  border-bottom:1px solid var(--b1);flex-shrink:0;z-index:100;
}
.tb-left{display:flex;align-items:center;gap:10px}
.tb-dot{width:8px;height:8px;border-radius:50%;background:var(--dim2);transition:all .5s var(--ease)}
.tb-dot.on{background:var(--g);box-shadow:0 0 12px var(--g5);animation:breathe 3s ease-in-out infinite}
.tb-brand{font-family:var(--font);font-weight:300;font-size:1.05rem;letter-spacing:4px;text-transform:uppercase;
  background:linear-gradient(135deg,var(--w),var(--g),var(--w));background-size:200% 200%;
  -webkit-background-clip:text;-webkit-text-fill-color:transparent;background-clip:text;animation:gradientShift 3s ease infinite}
.tb-right{display:flex;align-items:center;gap:8px}
.tb-chip{font-family:var(--mono);font-size:.55rem;color:var(--dim);padding:4px 10px;background:var(--s2);border:1px solid var(--b1);border-radius:20px;letter-spacing:.5px}
.tb-status{font-size:.55rem;color:var(--dim2);font-family:var(--mono);letter-spacing:.5px}

/* ── MAIN CONTENT ────────────────────────────────────────────────── */
main{flex:1;overflow-y:auto;overflow-x:hidden;padding:14px 14px 20px;scroll-behavior:smooth;-webkit-overflow-scrolling:touch}
.tab-content{display:none}
.tab-content.active{display:block;animation:slideUp .35s var(--ease)}

/* ── BOTTOM NAV ──────────────────────────────────────────────────── */
#bottom-nav{
  height:var(--nav);display:flex;align-items:stretch;position:relative;
  background:rgba(8,8,8,.96);backdrop-filter:saturate(180%) blur(24px);-webkit-backdrop-filter:saturate(180%) blur(24px);
  border-top:1px solid var(--b1);flex-shrink:0;z-index:100;
  padding-bottom:env(safe-area-inset-bottom);
}
.nav-btn{
  flex:1;display:flex;flex-direction:column;align-items:center;justify-content:center;
  gap:4px;background:none;color:var(--dim2);font-size:.52rem;font-weight:600;
  letter-spacing:.8px;text-transform:uppercase;transition:color .3s var(--ease);position:relative;padding:6px 0;
}
.nav-btn svg{width:21px;height:21px;stroke:currentColor;fill:none;stroke-width:1.7;stroke-linecap:round;stroke-linejoin:round;transition:all .3s var(--ease)}
.nav-btn.active{color:var(--g)}
.nav-btn.active svg{filter:drop-shadow(0 0 8px var(--g5));transform:translateY(-1px)}
.nav-indicator{
  position:absolute;top:-1px;height:2.5px;background:var(--g);border-radius:0 0 4px 4px;
  box-shadow:0 0 12px var(--g5),0 2px 8px var(--g3);
  transition:left .35s var(--spring),width .35s var(--spring);
}
.nav-btn:active svg{transform:scale(.85)}

/* ═══════════════════════════════════════════════════════════════════
   COMPONENTS
   ═══════════════════════════════════════════════════════════════════ */

/* Ripple */
.ripple{position:absolute;border-radius:50%;background:rgba(0,255,65,.18);transform:scale(0);animation:rippleOut .55s var(--ease);pointer-events:none}

/* Card */
.card{
  background:var(--s1);border:1px solid var(--b1);border-radius:var(--r);
  padding:16px;margin-bottom:12px;position:relative;overflow:hidden;
  transition:border-color .3s var(--ease),transform .3s var(--ease),box-shadow .3s var(--ease);
}
.card::before{content:'';position:absolute;top:0;left:0;right:0;height:1px;background:linear-gradient(90deg,transparent 5%,var(--g4) 50%,transparent 95%)}
.card:active{transform:scale(.985);border-color:var(--b2)}
.card-title{font-size:.68rem;font-weight:700;text-transform:uppercase;letter-spacing:1.2px;color:var(--dim);margin-bottom:12px;display:flex;align-items:center;gap:7px}
.card-title .dot{width:5px;height:5px;border-radius:50%;background:var(--g);box-shadow:0 0 8px var(--g5)}

/* Hero */
.hero{
  background:linear-gradient(135deg,rgba(0,255,65,.06),rgba(0,229,255,.04),rgba(176,106,255,.03));
  border:1px solid var(--g3);border-radius:var(--r);
  padding:22px 20px;margin-bottom:14px;position:relative;overflow:hidden;
  animation:borderFlow 6s ease infinite;
}
.hero::before{content:'';position:absolute;top:0;left:0;right:0;height:1px;background:linear-gradient(90deg,transparent,var(--g5),var(--cyan-d),transparent);animation:shimmer 4s linear infinite;background-size:200% 100%}
.hero::after{content:'';position:absolute;bottom:-30px;right:-30px;width:120px;height:120px;background:radial-gradient(circle,var(--g4) 0%,transparent 70%);border-radius:50%;pointer-events:none}
.hero-title{font-size:1.4rem;font-weight:900;letter-spacing:-.5px;
  background:linear-gradient(135deg,var(--g),var(--cyan));-webkit-background-clip:text;-webkit-text-fill-color:transparent;background-clip:text}
.hero-sub{color:var(--dim);font-size:.7rem;font-family:var(--mono);margin-top:4px}
.hero-badge{display:inline-flex;align-items:center;gap:4px;margin-top:8px;padding:3px 10px;background:var(--g4);border:1px solid var(--g3);border-radius:20px;font-size:.55rem;color:var(--g);font-weight:600;letter-spacing:.5px}

/* Stat grid */
.stat-grid{display:grid;grid-template-columns:1fr 1fr;gap:8px;margin-bottom:14px}
.stat{
  background:var(--s1);border:1px solid var(--b1);border-radius:var(--rs);
  padding:14px 12px;text-align:center;position:relative;overflow:hidden;
  transition:all .3s var(--ease);
}
.stat::before{content:'';position:absolute;top:0;left:0;right:0;height:1px;background:linear-gradient(90deg,transparent,var(--g4),transparent)}
.stat:active{border-color:var(--g3);transform:scale(.96)}
.stat-icon{font-size:1.3rem;margin-bottom:3px}
.stat-val{font-size:1.25rem;font-weight:900;font-variant-numeric:tabular-nums;color:var(--g);font-family:var(--mono);text-shadow:0 0 16px var(--g3);animation:countPop .35s var(--spring)}
.stat-lbl{font-size:.55rem;color:var(--dim2);text-transform:uppercase;letter-spacing:1px;margin-top:3px;font-weight:600}

/* Buttons */
.btn{
  display:inline-flex;align-items:center;justify-content:center;gap:6px;
  padding:11px 20px;border-radius:var(--rs);font-size:.76rem;font-weight:700;
  transition:all .25s var(--ease);position:relative;overflow:hidden;letter-spacing:.3px;
}
.btn:active{transform:scale(.95)}
.btn-p{background:var(--g);color:#000;box-shadow:0 0 20px var(--g3),0 4px 12px rgba(0,0,0,.3)}
.btn-p:active{box-shadow:0 0 35px var(--g5)}
.btn-d{background:var(--red-d);color:var(--red);border:1px solid var(--red-b)}
.btn-d:active{background:rgba(255,59,59,.18)}
.btn-o{background:var(--s2);color:var(--w2);border:1px solid var(--b1)}
.btn-o:active{border-color:var(--g2);color:var(--g);background:var(--g4)}
.btn-a{background:var(--g4);color:var(--g);border:1px solid var(--g3)}
.btn-a:active{background:var(--g3);border-color:var(--g5)}
.btn-g{background:var(--s2);color:var(--w2);border:1px solid var(--b1)}
.btn-g:active{border-color:var(--g2);color:var(--g)}
.btn-s{padding:9px 14px;font-size:.7rem;border-radius:8px}
.btn-f{width:100%;justify-content:center}
.btn-grp{display:flex;flex-wrap:wrap;gap:8px;margin-bottom:12px}
.btn:disabled{opacity:.25;cursor:not-allowed;transform:none!important}

/* Scan button */
.scan-btn{
  width:100%;padding:14px;background:var(--s2);border:1px solid var(--b1);
  color:var(--g);font-weight:700;font-size:.8rem;border-radius:var(--rs);
  margin-bottom:12px;display:flex;align-items:center;justify-content:center;gap:8px;
  position:relative;overflow:hidden;transition:all .25s var(--ease);
}
.scan-btn:active{background:var(--g4);border-color:var(--g5);transform:scale(.97)}
.scan-btn.scanning{color:var(--cyan);border-color:var(--cyan-d)}

/* Spinner */
.spinner{width:16px;height:16px;border:2px solid transparent;border-top-color:currentColor;border-radius:50%;animation:spin .6s linear infinite;display:inline-block}

/* Lists */
.li{
  display:flex;align-items:center;gap:12px;
  padding:12px;border-radius:var(--rs);
  background:var(--s1);border:1px solid var(--b1);
  margin-bottom:6px;cursor:pointer;transition:all .25s var(--ease);position:relative;overflow:hidden;
}
.li:active{transform:scale(.98)}
.li.sel{border-color:var(--g3);background:var(--s2);box-shadow:inset 3px 0 0 var(--g)}
.li-icon{font-size:1rem;flex-shrink:0}
.li-body{flex:1;min-width:0}
.li-name{font-size:.8rem;font-weight:600;white-space:nowrap;overflow:hidden;text-overflow:ellipsis}
.li-sub{font-size:.6rem;color:var(--dim);margin-top:2px;font-family:var(--mono)}
.li-right{text-align:right;flex-shrink:0}

/* Signal bars */
.sig{display:flex;align-items:flex-end;gap:2px;height:14px}
.sig b{width:3px;border-radius:1px;background:var(--b2);transition:all .3s;display:block}
.sig b.on{background:var(--g)}
.sig b:nth-child(1){height:3px}.sig b:nth-child(2){height:5px}.sig b:nth-child(3){height:8px}.sig b:nth-child(4){height:11px}.sig b:nth-child(5){height:14px}
.sig.fair b.on{background:var(--warn)}.sig.weak b.on,.sig.poor b.on{background:var(--red)}

/* Terminal */
.term-input-box{display:flex;gap:10px;margin-top:10px;align-items:center}
.term-input{flex:1;background:rgba(0,0,0,0.5);border:1px solid var(--border);color:var(--cyan);font-family:var(--mono);font-size:0.85rem;padding:10px;border-radius:6px;outline:none;transition:border-color .3s}
.term-input:focus{border-color:var(--cyan)}
.term-btn{padding:10px 15px;background:var(--cyan-d);color:var(--cyan);border:1px solid var(--cyan);border-radius:6px;cursor:pointer;font-weight:700;transition:all .3s}
.term-btn:hover{background:var(--cyan);color:#000;box-shadow:0 0 10px var(--cyan)}

/* Radar Sweep Animation */
.radar-sweep{position:absolute;top:0;left:0;width:50%;height:50%;background:linear-gradient(45deg,rgba(0,229,255,0) 0%,rgba(0,229,255,0.8) 100%);transform-origin:bottom right;animation:radarSweep 2s infinite linear;border-radius:100% 0 0 0}
.radar-sweep.ble{background:linear-gradient(45deg,rgba(176,106,255,0) 0%,rgba(176,106,255,0.8) 100%)}
@keyframes radarSweep{from{transform:rotate(0deg)}to{transform:rotate(360deg)}}
.term{
  background:rgba(0,0,0,.7);border:1px solid var(--b1);border-radius:var(--rs);
  padding:12px;font-family:var(--mono);font-size:.65rem;color:var(--g);
  height:45vh;overflow-y:auto;white-space:pre-wrap;word-break:break-all;line-height:1.8;
}
.term .line{animation:fadeIn .2s}.term .ts{color:var(--dim2)}.term .warn{color:var(--warn)}.term .err{color:var(--red)}.term .info{color:var(--g)}

/* Input */
.inp{
  width:100%;padding:11px 14px;background:var(--s2);border:1px solid var(--b1);
  border-radius:var(--rs);color:var(--w);font-size:.78rem;
  transition:all .25s var(--ease);
}
.inp:focus{border-color:var(--g3);box-shadow:0 0 0 3px var(--g4)}
textarea.inp{resize:vertical;min-height:80px;font-family:var(--mono);font-size:.68rem}
select.inp{appearance:none;background-image:url("data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='12' height='12' fill='%23888'%3E%3Cpath d='M6 8L1 3h10z'/%3E%3C/svg%3E");background-repeat:no-repeat;background-position:right 12px center}

/* Color picker */
.clr-row{display:flex;align-items:center;gap:12px;margin-bottom:12px}
.clr-prev{width:36px;height:36px;border-radius:var(--rs);border:1px solid var(--b1)}
input[type="color"]{width:36px;height:36px;border:none;border-radius:var(--rs);background:none;cursor:pointer;padding:0}

/* Bar chart */
.bars{display:flex;align-items:flex-end;gap:2px;height:60px;margin-top:12px}
.bars div{flex:1;background:linear-gradient(to top,var(--g),var(--g2));border-radius:2px 2px 0 0;min-height:2px;transition:height .3s;opacity:.6}

/* Section label */
.sec{font-size:.6rem;font-weight:700;color:var(--dim2);text-transform:uppercase;letter-spacing:1.5px;margin:18px 0 10px;display:flex;align-items:center;gap:8px}
.sec::after{content:'';flex:1;height:1px;background:linear-gradient(90deg,var(--b1),transparent)}

/* Attack banner */
.atk{
  padding:10px 14px;border-radius:var(--rs);margin-bottom:12px;
  display:none;align-items:center;gap:10px;font-size:.72rem;font-weight:700;
  animation:glow 2s ease-in-out infinite;
}
.atk.active{display:flex}
.atk.w{background:var(--red-d);color:var(--red);border:1px solid var(--red-b)}
.atk.b{background:var(--purple-d);color:var(--purple);border:1px solid var(--purple-b)}

/* Empty */
.empty{text-align:center;padding:40px 20px;color:var(--dim2)}
.empty .icon{font-size:2.2rem;margin-bottom:8px;opacity:.35}.empty p{font-size:.72rem}

/* Toasts */
#toast-container{position:fixed;top:calc(var(--top) + 10px);left:50%;transform:translateX(-50%);z-index:9999;display:flex;flex-direction:column;gap:8px;width:calc(100% - 32px);max-width:400px;pointer-events:none}
.toast{
  display:flex;align-items:center;gap:10px;padding:12px 16px;border-radius:var(--rs);
  font-size:.74rem;font-weight:600;pointer-events:auto;
  animation:toastIn .35s var(--spring);backdrop-filter:blur(20px);-webkit-backdrop-filter:blur(20px);
  box-shadow:0 8px 24px rgba(0,0,0,.4);
}
.toast.info{background:rgba(0,255,65,.08);border:1px solid var(--g5);color:var(--g)}
.toast.success{background:rgba(0,255,65,.08);border:1px solid var(--g5);color:var(--g)}
.toast.error{background:var(--red-d);border:1px solid var(--red-b);color:var(--red)}
.toast.warning{background:var(--warn-d);border:1px solid rgba(255,184,0,.22);color:var(--warn)}
.toast.hide{animation:toastOut .3s var(--ease) forwards}

/* Modal */
.mdl{
  position:fixed;inset:0;background:rgba(0,0,0,.85);backdrop-filter:blur(10px);-webkit-backdrop-filter:blur(10px);
  z-index:1000;display:none;align-items:center;justify-content:center;padding:20px;
}
.mdl.active{display:flex}
.mdl-box{
  background:var(--s1);border:1px solid var(--b1);border-radius:var(--r);
  padding:24px;width:100%;max-width:340px;animation:scaleIn .3s var(--spring);text-align:center;
  box-shadow:0 24px 64px rgba(0,0,0,.5);
}
.mdl-box h3{font-size:1rem;margin-bottom:8px;color:var(--g);font-weight:700}
.mdl-box p{color:var(--dim);font-size:.76rem;margin-bottom:18px;line-height:1.55}
.mdl-acts{display:flex;gap:10px;justify-content:center}

/* PWA install banner */
.pwa-banner{
  position:fixed;bottom:calc(var(--nav) + 12px + env(safe-area-inset-bottom));left:50%;transform:translateX(-50%);
  z-index:101;padding:10px 18px;background:var(--s2);border:1px solid var(--g3);
  border-radius:var(--rr);font-size:.7rem;color:var(--g);font-weight:600;
  display:none;align-items:center;gap:8px;box-shadow:0 8px 24px rgba(0,0,0,.4);
  animation:slideUp .4s var(--spring);cursor:pointer;
}

/* About card */
.about{text-align:center;padding:60px 20px;margin-top:14px;background:#020202;border:1px solid #111;position:relative;overflow:hidden;border-radius:var(--r)}
.about-bg-flare{position:absolute;top:-50%;left:50%;transform:translateX(-50%);width:200%;height:100%;background:radial-gradient(ellipse at top,rgba(0,255,65,.12) 0%,transparent 60%);pointer-events:none}
.about-logo-wrap{display:flex;justify-content:center;margin-bottom:25px;position:relative;z-index:1}
.about-name{font-size:2rem;font-weight:200;letter-spacing:14px;color:#fff;text-shadow:0 0 20px rgba(255,255,255,.2);margin-left:14px;position:relative;z-index:1;text-transform:uppercase}
.about-powered{color:var(--dim);font-size:.5rem;margin:15px 0 35px;font-weight:400;letter-spacing:8px;text-transform:uppercase;position:relative;z-index:1}
.about-divider{width:1px;height:40px;background:linear-gradient(to bottom,var(--g),transparent);margin:0 auto 30px;position:relative;z-index:1}
.about-role{font-size:.45rem;color:var(--dim2);text-transform:uppercase;letter-spacing:6px;margin-bottom:12px;position:relative;z-index:1}
.about-author{font-size:1.1rem;color:#fff;font-weight:300;letter-spacing:8px;margin-bottom:40px;position:relative;z-index:1;text-shadow:0 0 10px rgba(255,255,255,.2)}
.about-legal{font-size:.55rem;color:#333;letter-spacing:2px;margin-top:20px;position:relative;z-index:1}

@media(min-width:600px){
  main{padding:20px 24px}
  .stat-grid{grid-template-columns:repeat(4,1fr)}
  .btn-grp{flex-wrap:nowrap}
}
</style>
</head>
<body>

<!-- ══ BOOT SCREEN ══ -->
<div id="boot">
  <div class="b-particles" id="b-particles"></div>
  <div class="b-icon">
    <svg width="72" height="72" viewBox="0 0 100 100" fill="none" stroke="var(--g)" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round" style="filter:drop-shadow(0 0 15px rgba(0,255,65,0.6));">
      <path d="M 50 90 L 20 45 L 15 10 L 40 25 L 50 20 L 60 25 L 85 10 L 80 45 Z"/>
      <path d="M 20 45 L 50 70 L 80 45"/>
      <path d="M 40 25 L 50 50 L 60 25"/>
      <path d="M 32 42 L 44 48" stroke-width="4"/>
      <path d="M 68 42 L 56 48" stroke-width="4"/>
    </svg>
  </div>
  <div class="b-brand">NEOCAT</div>
  <div class="b-sub">Security Toolkit</div>
  <div class="b-powered">Powered by Glyph S3 &times; Hexlabs</div>
  <div class="b-log" id="b-log"></div>
  <div class="b-progress"><div class="b-bar" id="b-bar"></div></div>
  <div class="b-ver">V1.0.0 &bull; ESP32-S3</div>
</div>

<!-- ══ LEGAL ══ -->
<div id="legal">
  <div class="l-card">
    <div class="l-icon">&#x1F6E1;</div>
    <div class="l-title">Legal &amp; Ethical Notice</div>
    <div class="l-body">
      <p><strong>NeoCAT V1</strong> is for authorised security testing only.</p>
      <ul>
        <li>Use only on systems you <strong>own</strong> or have <strong>written permission</strong></li>
        <li>Unauthorised access is a criminal offence</li>
        <li>The developers accept no responsibility for misuse</li>
      </ul>
    </div>
    <div class="l-actions">
      <button class="btn btn-g btn-s" onclick="location='about:blank'">Decline</button>
      <button class="btn btn-p btn-s" id="accept-btn">I Accept</button>
    </div>
  </div>
</div>

<!-- ══ APP ══ -->
<div id="app">

<header class="topbar">
  <div class="tb-left">
    <div class="tb-dot" id="conn-dot"></div>
    <span class="tb-brand" style="display:flex;align-items:center;gap:10px;">
      <svg width="22" height="22" viewBox="0 0 100 100" fill="none" stroke="url(#navGrad)" stroke-width="3.5" stroke-linecap="round" stroke-linejoin="round" style="filter:drop-shadow(0 0 5px rgba(0,255,65,0.5));"><defs><linearGradient id="navGrad" x1="0%" y1="0%" x2="100%" y2="100%"><stop offset="0%" stop-color="#00ff41"/><stop offset="100%" stop-color="#00e5ff"/></linearGradient></defs><path d="M 50 90 L 20 45 L 15 10 L 40 25 L 50 20 L 60 25 L 85 10 L 80 45 Z"/><path d="M 20 45 L 50 70 L 80 45"/><path d="M 40 25 L 50 50 L 60 25"/><path d="M 32 42 L 44 48" stroke-width="5"/><path d="M 68 42 L 56 48" stroke-width="5"/></svg>
      NEOCAT V1
    </span>
  </div>
  <div class="tb-right">
    <span class="tb-chip">192.168.4.1</span>
    <span class="tb-status" id="conn-label">OFFLINE</span>
  </div>
</header>

<main>

<!-- ── DASHBOARD ─────────────────────────────────────── -->
<section id="tab-dashboard" class="tab-content active">
  <div class="hero" style="background: linear-gradient(135deg, var(--s3) 0%, var(--s1) 100%); border: 1px solid var(--g5);">
    <div class="hero-title" style="display:flex;align-items:center;justify-content:center;gap:8px;">
      <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="var(--g)" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M13 2L3 14h9l-1 8 10-12h-9l1-8z"/></svg>
      System Online
    </div>
    <div class="hero-sub" id="hero-status">Initialising&hellip;</div>
    <div class="hero-badge" style="display:flex;align-items:center;justify-content:center;gap:6px;">
      <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><rect x="2" y="3" width="20" height="14" rx="2" ry="2"/><line x1="8" y1="21" x2="16" y2="21"/><line x1="12" y1="17" x2="12" y2="21"/></svg>
      NeoCAT V1 &bull; Glyph S3
    </div>
  </div>
  <div class="stat-grid">
    <div class="stat"><div class="stat-icon"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"/><polyline points="12 6 12 12 16 14"/></svg></div><div class="stat-val" id="s-uptime">00:00:00</div><div class="stat-lbl">Uptime</div></div>
    <div class="stat"><div class="stat-icon"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="2" y="2" width="20" height="8" rx="2" ry="2"/><rect x="2" y="14" width="20" height="8" rx="2" ry="2"/><line x1="6" y1="6" x2="6" y2="6"/><line x1="6" y1="18" x2="6" y2="18"/></svg></div><div class="stat-val" id="s-heap">--</div><div class="stat-lbl">Memory</div></div>
    <div class="stat"><div class="stat-icon"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M4 22h14a2 2 0 0 0 2-2V7.5L14.5 2H6a2 2 0 0 0-2 2v4"/><polyline points="14 2 14 8 20 8"/><path d="M2 15h10"/><path d="M9 18l3-3-3-3"/></svg></div><div class="stat-val" id="s-pkts">0</div><div class="stat-lbl">Packets</div></div>
    <div class="stat"><div class="stat-icon"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M5 12.55a11 11 0 0 1 14.08 0"/><path d="M1.42 9a16 16 0 0 1 21.16 0"/><path d="M8.53 16.11a6 6 0 0 1 6.95 0"/><circle cx="12" cy="20" r="1"/></svg></div><div class="stat-val" id="s-devs">0</div><div class="stat-lbl">Devices</div></div>
  </div>
  <div class="sec">Quick Actions</div>
  <div class="btn-grp">
    <button class="btn btn-a btn-s" onclick="doWifiScan()"><svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M5 12.55a11 11 0 0 1 14.08 0"/><path d="M1.42 9a16 16 0 0 1 21.16 0"/><path d="M8.53 16.11a6 6 0 0 1 6.95 0"/><circle cx="12" cy="20" r="1"/></svg> WiFi Scan</button>
    <button class="btn btn-o btn-s" onclick="doBleScan()"><svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="6.5 6.5 17.5 17.5 12 23 12 1 17.5 6.5 6.5 17.5"/></svg> BLE Scan</button>
    <button class="btn btn-o btn-s" onclick="quickBeacon()"><svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M4 22h14a2 2 0 0 0 2-2V7.5L14.5 2H6a2 2 0 0 0-2 2v4"/><polyline points="14 2 14 8 20 8"/><path d="M2 15h10"/><path d="M9 18l3-3-3-3"/></svg> Beacon</button>
  </div>
</section>

<!-- ── WIFI ───────────────────────────────────────────── -->
<section id="tab-wifi" class="tab-content">
  <div class="atk w" id="wifi-atk-banner"><span class="spinner"></span><span id="wifi-atk-label">Attack active</span><span style="margin-left:auto;font-variant-numeric:tabular-nums;font-family:var(--mono);font-size:.65rem" id="wifi-atk-pkts">0 pkts</span></div>
  <button class="scan-btn" id="btn-wifi-scan" onclick="doWifiScan()"><svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="11" cy="11" r="8"/><path d="M21 21l-4.35-4.35"/></svg>Scan Networks</button>
  <div id="wifi-list-wrap"><div class="empty" id="wifi-empty"><div class="icon"><svg width="48" height="48" viewBox="0 0 24 24" fill="none" stroke="var(--dim3)" stroke-width="2"><path d="M5 12.55a11 11 0 0 1 14.08 0"/><path d="M1.42 9a16 16 0 0 1 21.16 0"/><path d="M8.53 16.11a6 6 0 0 1 6.95 0"/><circle cx="12" cy="20" r="1"/></svg></div><p>Tap scan to discover networks</p></div><div id="wifi-list"></div></div>
  <div class="sec">Attack Panel</div>
  <div class="btn-grp"><button class="btn btn-d btn-s" onclick="startDeauth('selected')"><svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M13 2L3 14h9l-1 8 10-12h-9l1-8z"/></svg> Deauth</button><button class="btn btn-d btn-s" onclick="startDeauth('all')"><svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polygon points="13 2 3 14 12 14 11 22 21 10 12 10 13 2"/></svg> Deauth All</button></div>
  <div class="card">
    <div class="card-title"><span class="dot"></span>Beacon & Rickroll Flood</div>
    <div style="display:flex;gap:8px;margin-bottom:8px">
      <select class="inp" style="flex:1;padding:8px" id="beacon-tpl" onchange="fillBeacon(this.value)">
        <option value="">-- Theme --</option>
        <option value="funny">Funny</option>
        <option value="hacker">Hacker Mode</option>
        <option value="god">God Mode</option>
        <option value="creepy">Creepy</option>
        <option value="meme">Meme</option>
        <option value="tech">Tech Support</option>
      </select>
    </div>
    <div style="display:flex;gap:8px;margin-bottom:8px">
      <select class="inp" style="flex:1;padding:8px" id="song-lang" onchange="fillSong()">
        <option value="">-- Language --</option>
        <option value="en">English</option>
        <option value="ml">Malayalam</option>
        <option value="ta">Tamil</option>
        <option value="hi">Hindi</option>
        <option value="kn">Kannada</option>
      </select>
      <select class="inp" style="flex:1;padding:8px" id="song-cat" onchange="fillSong()">
        <option value="">-- Song Type --</option>
        <option value="revenge">Revenge</option>
        <option value="love">Love</option>
        <option value="sad">Sad</option>
        <option value="funny">Funny</option>
        <option value="happy">Happy</option>
        <option value="proposal">Proposal</option>
      </select>
    </div>
    <textarea class="inp" id="beacon-ssids" placeholder="Select a template above, or enter custom SSIDs (one per line)" rows="4"></textarea>
    <div class="btn-grp" style="margin-top:10px">
      <button class="btn btn-a btn-s" onclick="startBeacon()"><svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M4 22h14a2 2 0 0 0 2-2V7.5L14.5 2H6a2 2 0 0 0-2 2v4"/><polyline points="14 2 14 8 20 8"/><path d="M2 15h10"/><path d="M9 18l3-3-3-3"/></svg> Beacon Flood</button>
      <button class="btn btn-o btn-s" onclick="startRickroll()"><svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M9 18V5l12-2v13"/><circle cx="6" cy="18" r="3"/><circle cx="18" cy="16" r="3"/></svg> Rickroll Flood</button>
      <button class="btn btn-o btn-s" onclick="startProbe()"><svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="11" cy="11" r="8"/><line x1="21" y1="21" x2="16.65" y2="16.65"/></svg> Probe Flood</button>
    </div>
  </div>
  <button class="btn btn-d btn-f" id="btn-wifi-stop" onclick="stopWifiAttack()" style="display:none"><svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="3" y="3" width="18" height="18" rx="2" ry="2"/><line x1="9" y1="9" x2="15" y2="15"/><line x1="15" y1="9" x2="9" y2="15"/></svg> Stop All</button>

  <div class="sec">Clone AP</div>
  <div class="card" id="clone-card">
    <div class="card-title"><span class="dot" style="background:var(--orange)"></span><svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="margin-right:8px;vertical-align:-3px"><path d="M16 17l5-5-5-5"/><path d="M21 12H9"/><path d="M3 6v14a2 2 0 0 0 2 2h14"/></svg>Clone Selected Network</div>
    <div id="clone-info" style="font-size:.68rem;color:var(--dim);font-family:var(--mono);padding:6px 0;margin-bottom:8px">Select a network above first</div>
    <div style="display:flex;flex-direction:column;gap:10px">
      <select class="inp" id="clone-template" style="padding:10px">
        <option value="0">🎭 Prank — "You've been pranked!"</option>
        <option value="1">💀 Hacker Mode — Matrix style</option>
        <option value="2">🤣 Funny — Rick Roll</option>
      </select>
      <input type="text" class="inp" id="clone-title" placeholder="Custom title text (optional)" maxlength="64">
      <div class="btn-grp">
        <button class="btn btn-a btn-s" id="btn-clone-start" onclick="startCloneAP()"><svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M16 17l5-5-5-5"/><path d="M21 12H9"/><path d="M3 6v14a2 2 0 0 0 2 2h14"/></svg> Clone & Start</button>
        <button class="btn btn-d btn-s" id="btn-clone-stop" onclick="stopCloneAP()" style="display:none"><svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="3" y="3" width="18" height="18" rx="2" ry="2"/><line x1="9" y1="9" x2="15" y2="15"/><line x1="15" y1="9" x2="9" y2="15"/></svg> Stop Clone</button>
      </div>
    </div>
    <div id="clone-status" style="display:none;margin-top:12px;padding:12px;background:rgba(255,152,0,.08);border:1px solid rgba(255,152,0,.2);border-radius:var(--r);font-size:.7rem;color:var(--orange);font-family:var(--mono)">
      <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="vertical-align:-2px;margin-right:6px"><path d="M10.29 3.86L1.82 18a2 2 0 0 0 1.71 3h16.94a2 2 0 0 0 1.71-3L13.71 3.86a2 2 0 0 0-3.42 0z"/><line x1="12" y1="9" x2="12" y2="13"/><line x1="12" y1="17" x2="12.01" y2="17"/></svg>
      Clone AP active — <span id="clone-ssid-label"></span><br>
      <span style="font-size:.6rem;opacity:.7">Admin: go to <a href="/dashboard" style="color:var(--g)">192.168.4.1/dashboard</a></span>
    </div>
  </div>
</section>

<!-- ── BLE ────────────────────────────────────────────── -->
<section id="tab-ble" class="tab-content">
  <div class="atk b" id="ble-atk-banner"><span class="spinner"></span><span id="ble-atk-label">BLE Spam active</span><span style="margin-left:auto;font-variant-numeric:tabular-nums;font-family:var(--mono);font-size:.65rem" id="ble-atk-pkts">0 pkts</span></div>
  <button class="scan-btn" id="btn-ble-scan" onclick="doBleScan()"><svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="11" cy="11" r="8"/><path d="M21 21l-4.35-4.35"/></svg>Scan BLE Devices</button>
  <div id="ble-list-wrap"><div class="empty" id="ble-empty"><div class="icon"><svg width="48" height="48" viewBox="0 0 24 24" fill="none" stroke="var(--dim3)" stroke-width="2"><polyline points="6.5 6.5 17.5 17.5 12 23 12 1 17.5 6.5 6.5 17.5"/></svg></div><p>Tap scan to discover BLE devices</p></div><div id="ble-list"></div></div>
  <div class="sec">BLE Spam</div>
  <div class="btn-grp"><button class="btn btn-o btn-s" onclick="startBleSpam('apple')"><svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M12 20.94c1.5 0 2.75 1.06 4 1.06 3 0 6-8 6-12.22A4.91 4.91 0 0 0 17 5c-2.22 0-4 1.44-5 2-1-.56-2.78-2-5-2a4.9 4.9 0 0 0-5 4.78C2 14 5 22 8 22c1.25 0 2.5-1.06 4-1.06Z"/><path d="M10 2c1 .5 2 2 2 5"/></svg> Apple</button><button class="btn btn-o btn-s" onclick="startBleSpam('samsung')"><svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="5" y="2" width="14" height="20" rx="2" ry="2"/><line x1="12" y1="18" x2="12.01" y2="18"/></svg> Samsung</button><button class="btn btn-o btn-s" onclick="startBleSpam('windows')"><svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="3" y="3" width="7" height="7"/><rect x="14" y="3" width="7" height="7"/><rect x="14" y="14" width="7" height="7"/><rect x="3" y="14" width="7" height="7"/></svg> Windows</button><button class="btn btn-a btn-s" onclick="startBleSpam('all')"><svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polygon points="13 2 3 14 12 14 11 22 21 10 12 10 13 2"/></svg> All</button></div>
  <button class="btn btn-d btn-f" id="btn-ble-stop" onclick="stopBleSpam()" style="display:none"><svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="3" y="3" width="18" height="18" rx="2" ry="2"/><line x1="9" y1="9" x2="15" y2="15"/><line x1="15" y1="9" x2="9" y2="15"/></svg> Stop BLE</button>
</section>

<!-- ── TERMINAL ───────────────────────────────────────── -->
<section id="tab-terminal" class="tab-content">
  <div class="card">
    <div class="card-title"><span class="dot" style="background:var(--cyan)"></span>Interactive Shell</div>
    <div class="term" id="term-display" style="height:350px;font-size:0.8rem"></div>
    <div class="term-input-box">
      <span id="term-prompt" style="color:var(--g);font-family:var(--mono);font-size:0.85rem">neocat&gt;</span>
      <input type="text" id="term-input" class="term-input" placeholder="Type 'help' for commands..." autocomplete="off" spellcheck="false">
      <button class="term-btn" onclick="termSend()">&#x21B2;</button>
    </div>
  </div>
</section>

<!-- ── MONITOR ───────────────────────────────────────── -->
<section id="tab-monitor" class="tab-content">
  <div class="card-title"><span class="dot"></span>Live Console</div>
  <div class="term" id="terminal"></div>
  <div style="margin-top:6px;text-align:right;font-size:.55rem;color:var(--dim2);font-family:var(--mono)"><span id="log-count">0</span> entries</div>
  <div class="sec">Packet Rate</div>
  <div class="bars" id="pkt-chart"></div>
</section>

<!-- ── SETTINGS ──────────────────────────────────────── -->
<section id="tab-settings" class="tab-content">
  <div class="card"><div class="card-title"><span class="dot"></span><svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="margin-right:8px;vertical-align:-3px"><path d="M21 15a2 2 0 0 1-2 2H7l-4 4V5a2 2 0 0 1 2-2h14a2 2 0 0 1 2 2z"/></svg> CATroom Chat</div><div style="display:flex;flex-direction:column;gap:10px"><input type="text" class="inp" id="chat-name" placeholder="Room Name" value="CATroom"><input type="text" class="inp" id="chat-pass" placeholder="Room Password (optional)"><div id="chat-status" style="font-size:.68rem;color:var(--dim);font-family:var(--mono);padding:6px 0">Room: Inactive</div><div class="btn-grp"><button class="btn btn-p btn-s" id="btn-chat-start" onclick="startChatRoom()"><svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M4.5 16.5c-1.5 1.26-2 5-2 5s3.74-.5 5-2c.71-.84.7-2.13-.09-2.91a2.18 2.18 0 0 0-2.91-.09z"/><path d="m12 15-3-3a22 22 0 0 1 2-3.95A12.88 12.88 0 0 1 22 2c0 2.72-.78 7.5-6 11a22.35 22.35 0 0 1-4 2z"/><path d="M9 12H4s.55-3.03 2-4c1.62-1.08 5 0 5 0"/><path d="M12 15v5s3.03-.55 4-2c1.08-1.62 0-5 0-5"/></svg> Start Room</button><button class="btn btn-d btn-s" id="btn-chat-stop" onclick="stopChatRoom()" style="display:none"><svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="3" y="3" width="18" height="18" rx="2" ry="2"/><line x1="9" y1="9" x2="15" y2="15"/><line x1="15" y1="9" x2="9" y2="15"/></svg> Stop Room</button></div><button class="btn btn-p btn-f" onclick="window.open('/chat','_blank')" style="margin-top:5px;background:var(--s3)"><svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="margin-right:6px"><path d="M18 13v6a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2V8a2 2 0 0 1 2-2h6"/><polyline points="15 3 21 3 21 9"/><line x1="10" y1="14" x2="21" y2="3"/></svg> Open Chat UI</button></div></div>
  <div class="card"><div class="card-title"><span class="dot"></span><svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="margin-right:8px;vertical-align:-3px"><path d="M22 12h-4l-3 9L9 3l-3 9H2"/></svg> System Health</div><div style="font-size:.72rem;line-height:2.2;color:var(--dim)"><div>Battery <span style="float:right;color:var(--w);font-family:var(--mono)" id="sh-bat">--</span></div><div>CPU Temp <span style="float:right;color:var(--w);font-family:var(--mono)" id="sh-temp">--</span></div><div>CPU Freq <span style="float:right;color:var(--w);font-family:var(--mono)" id="sh-freq">--</span></div><div>Free RAM <span style="float:right;color:var(--w);font-family:var(--mono)" id="sh-ram">--</span></div><div>Chat Room <span style="float:right;font-family:var(--mono)" id="sh-chat" style="color:var(--dim2)">Inactive</span></div><div>Chat Users <span style="float:right;color:var(--w);font-family:var(--mono)" id="sh-chatusers">0</span></div></div></div>
  <div class="card"><div class="card-title"><span class="dot"></span>LED Control</div><select class="inp" id="led-mode" onchange="setLedMode(this.value)" style="margin-bottom:10px"><option value="idle">Idle</option><option value="scanning">Scanning</option><option value="attacking">Attacking</option><option value="off">Off</option><option value="custom">Custom Color</option></select><div class="clr-row" id="custom-color-row" style="display:none"><input type="color" id="led-color" value="#00ff41" onchange="setLedColor(this.value)"><div class="clr-prev" id="color-preview" style="background:#00ff41"></div><span style="font-size:.68rem;color:var(--dim);font-family:var(--mono)" id="color-hex">#00ff41</span></div></div>
  <div class="card"><div class="card-title"><span class="dot"></span>Device Config</div><div style="display:flex;flex-direction:column;gap:10px"><input type="text" class="inp" id="cfg-ssid" placeholder="WiFi Name (SSID)"><input type="text" class="inp" id="cfg-pass" placeholder="WiFi Password (blank=open)"><input type="text" class="inp" id="cfg-user" placeholder="Your Name (Boot Animation)"><input type="text" class="inp" id="cfg-insta" placeholder="Instagram ID (Boot Animation)"><button class="btn btn-p btn-f" onclick="saveConfig()"><svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M19 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h11l5 5v11a2 2 0 0 1-2 2z"/><polyline points="17 21 17 13 7 13 7 21"/><polyline points="7 3 7 8 15 8"/></svg> Save Settings</button></div></div>
  <div class="card"><div class="card-title"><span class="dot"></span>System Info</div><div style="font-size:.72rem;line-height:2.2;color:var(--dim)"><div>Chip <span style="float:right;color:var(--w);font-family:var(--mono)" id="si-chip">--</span></div><div>Rev <span style="float:right;color:var(--w);font-family:var(--mono)" id="si-rev">--</span></div><div>CPU <span style="float:right;color:var(--w);font-family:var(--mono)" id="si-cpu">--</span></div><div>Flash <span style="float:right;color:var(--w);font-family:var(--mono)" id="si-flash">--</span></div><div>SDK <span style="float:right;color:var(--w);font-family:var(--mono)" id="si-sdk">--</span></div><div>Firmware <span style="float:right;color:var(--g);font-weight:700;font-family:var(--mono)" id="si-fw">V1.0.0</span></div></div></div>
  <button class="btn btn-d btn-f" onclick="confirmReboot()"><svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M3 12a9 9 0 1 0 9-9 9.75 9.75 0 0 0-6.74 2.74L3 8"/><path d="M3 3v5h5"/></svg> Reboot Device</button>
  <div class="card about">
    <div class="about-bg-flare"></div>
    <div class="about-logo-wrap">
      <svg width="64" height="64" viewBox="0 0 100 100" fill="none" stroke="var(--g)" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round" style="filter:drop-shadow(0 0 15px rgba(0,255,65,0.4));">
        <path d="M 50 90 L 20 45 L 15 10 L 40 25 L 50 20 L 60 25 L 85 10 L 80 45 Z"/>
        <path d="M 20 45 L 50 70 L 80 45"/>
        <path d="M 40 25 L 50 50 L 60 25"/>
        <path d="M 32 42 L 44 48" stroke-width="4"/>
        <path d="M 68 42 L 56 48" stroke-width="4"/>
      </svg>
    </div>
    
    <div class="about-name">NEOCAT</div>
    <div class="about-powered">Powered by Glyph S3 &times; Hexlabs</div>
    
    <div class="about-divider"></div>
    
    <div class="about-role">Lead Architect</div>
    <div class="about-author">ARJUN TM</div>
    
    <div style="display:flex; justify-content:center; gap:25px; position:relative; z-index:1;">
      <a href="https://github.com/cyberkallan" target="_blank" style="color:var(--dim); transition:all 0.3s;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.2" stroke-linecap="round" stroke-linejoin="round"><path d="M9 19c-5 1.5-5-2.5-7-3m14 6v-3.87a3.37 3.37 0 0 0-.94-2.61c3.14-.35 6.44-1.54 6.44-7A5.44 5.44 0 0 0 20 4.77 5.07 5.07 0 0 0 19.91 1S18.73.65 16 2.48a13.38 13.38 0 0 0-7 0C6.27.65 5.09 1 5.09 1A5.07 5.07 0 0 0 5 4.77a5.44 5.44 0 0 0-1.5 3.78c0 5.42 3.3 6.61 6.44 7A3.37 3.37 0 0 0 9 18.13V22"/></svg></a>
      <a href="https://instagram.com/imarjunarz" target="_blank" style="color:var(--dim); transition:all 0.3s;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.2" stroke-linecap="round" stroke-linejoin="round"><rect x="2" y="2" width="20" height="20" rx="5" ry="5"/><path d="M16 11.37A4 4 0 1 1 12.63 8 4 4 0 0 1 16 11.37z"/><line x1="17.5" y1="6.5" x2="17.51" y2="6.5"/></svg></a>
      <a href="https://youtube.com/" target="_blank" style="color:var(--dim); transition:all 0.3s;"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.2" stroke-linecap="round" stroke-linejoin="round"><path d="M22.54 6.42a2.78 2.78 0 0 0-1.94-2C18.88 4 12 4 12 4s-6.88 0-8.6.46a2.78 2.78 0 0 0-1.94 2A29 29 0 0 0 1 11.75a29 29 0 0 0 .46 5.33A2.78 2.78 0 0 0 3.4 19c1.72.46 8.6.46 8.6.46s6.88 0 8.6-.46a2.78 2.78 0 0 0 1.94-2 29 29 0 0 0 .46-5.25 29 29 0 0 0-.46-5.33z"/><polygon points="9.75 15.02 15.5 11.75 9.75 8.48 9.75 15.02"/></svg></a>
    </div>
    
    <div class="about-legal">&copy; 2026 NEOCAT SECURITY</div>
  </div>
</section>

</main>

<!-- ── BOTTOM NAV ─────────────────────────────────────── -->
<nav id="bottom-nav">
  <div class="nav-indicator" id="nav-ind"></div>
  <button class="nav-btn active" data-tab="dashboard"><svg viewBox="0 0 24 24"><path d="M3 9l9-7 9 7v11a2 2 0 01-2 2H5a2 2 0 01-2-2z"/><polyline points="9 22 9 12 15 12 15 22"/></svg>HOME</button>
  <button class="nav-btn" data-tab="wifi"><svg viewBox="0 0 24 24"><path d="M5 12.55a11 11 0 0114.08 0"/><path d="M1.42 9a16 16 0 0121.16 0"/><path d="M8.53 16.11a6 6 0 016.95 0"/><circle cx="12" cy="20" r="1"/></svg>WIFI</button>
  <button class="nav-btn" data-tab="ble"><svg viewBox="0 0 24 24"><polyline points="6.5 6.5 17.5 17.5 12 23 12 1 17.5 6.5 6.5 17.5"/></svg>BLE</button>
  <button class="nav-btn" data-tab="terminal"><svg viewBox="0 0 24 24"><polyline points="4 17 10 11 4 5"/><line x1="12" y1="19" x2="20" y2="19"/></svg>TERM</button>
  <button class="nav-btn" data-tab="monitor"><svg viewBox="0 0 24 24"><rect x="2" y="3" width="20" height="14" rx="2"/><line x1="8" y1="21" x2="16" y2="21"/><line x1="12" y1="17" x2="12" y2="21"/></svg>LOG</button>
  <button class="nav-btn" data-tab="settings"><svg viewBox="0 0 24 24"><circle cx="12" cy="12" r="3"/><path d="M19.4 15a1.65 1.65 0 00.33 1.82l.06.06a2 2 0 01-2.83 2.83l-.06-.06a1.65 1.65 0 00-1.82-.33 1.65 1.65 0 00-1 1.51V21a2 2 0 01-4 0v-.09A1.65 1.65 0 009 19.4a1.65 1.65 0 00-1.82.33l-.06.06a2 2 0 01-2.83-2.83l.06-.06A1.65 1.65 0 004.68 15a1.65 1.65 0 00-1.51-1H3a2 2 0 010-4h.09A1.65 1.65 0 004.6 9a1.65 1.65 0 00-.33-1.82l-.06-.06a2 2 0 012.83-2.83l.06.06A1.65 1.65 0 009 4.68a1.65 1.65 0 001-1.51V3a2 2 0 014 0v.09a1.65 1.65 0 001 1.51 1.65 1.65 0 001.82-.33l.06-.06a2 2 0 012.83 2.83l-.06.06A1.65 1.65 0 0019.4 9a1.65 1.65 0 001.51 1H21a2 2 0 010 4h-.09a1.65 1.65 0 00-1.51 1z"/></svg>SETTINGS</button>
</nav>

<div id="toast-container"></div>

<div class="mdl" id="modal"><div class="mdl-box"><h3 id="modal-title">Confirm</h3><p id="modal-msg">Are you sure?</p><div class="mdl-acts"><button class="btn btn-g btn-s" onclick="closeModal()">Cancel</button><button class="btn btn-d btn-s" id="modal-confirm">Confirm</button></div></div></div>

</div><!-- /app -->

<script>
(function(){
"use strict";

/* ── Boot particles ────────────────────────────────────────────── */
(function(){
  const c=document.getElementById('b-particles');
  for(let i=0;i<40;i++){
    const p=document.createElement('div');
    p.className='b-p';
    p.style.cssText='left:'+Math.random()*100+'%;height:'+(8+Math.random()*20)+'px;animation-duration:'+(1.5+Math.random()*2.5)+'s;animation-delay:'+Math.random()*3+'s;';
    c.appendChild(p);
  }
})();

/* ── Boot sequence ─────────────────────────────────────────────── */
const bootLines=[
  '[SYS] Cyberkallan NeoCAT V1 loading...',
  '[CPU] ESP32-S3 dual-core @ 240MHz',
  '[MEM] Heap allocation OK',
  '[NET] WiFi promiscuous mode init...',
  '[BLE] Bluetooth LE stack ready',
  '[HTTP] Binding port 80...',
  '[WS] WebSocket handler registered',
  '[LED] RGB controller online',
  '[PWA] Service manifest loaded',
  '[OK] All systems nominal — Cyberkallan'
];
const bLog=document.getElementById('b-log');
const bBar=document.getElementById('b-bar');
const boot=document.getElementById('boot');
const legal=document.getElementById('legal');
let bI=0;
function bTick(){
  if(bI<bootLines.length){
    const el=document.createElement('div');
    el.textContent=bootLines[bI];
    el.style.animation='fadeIn .2s';
    bLog.appendChild(el);
    bLog.scrollTop=bLog.scrollHeight;
    bBar.style.width=((bI+1)/bootLines.length*100)+'%';
    bI++;
    setTimeout(bTick,100+Math.random()*160);
  }else{
    setTimeout(()=>{
      boot.classList.add('done');
      setTimeout(()=>{
        boot.style.display='none';
        if(!localStorage.getItem('neocat_accepted')){
          legal.classList.add('active');
        }else{showApp();}
      },700);
    },350);
  }
}
document.getElementById('accept-btn').onclick=()=>{
  localStorage.setItem('neocat_accepted','1');
  legal.classList.remove('active');
  showApp();
};
function showApp(){document.getElementById('app').classList.add('visible');}
(async()=>{
  try {
    const cfg = await api('/api/config');
    if(cfg && cfg.user && cfg.user !== 'Guest' && cfg.user !== '') bootLines.splice(1, 0, '[USR] Welcome, ' + cfg.user);
    if(cfg && cfg.insta && cfg.insta !== '') bootLines.splice(2, 0, '[IG] ' + cfg.insta);
    if(cfg) {
      $('#cfg-ssid').value = cfg.ssid || '';
      $('#cfg-pass').value = cfg.pass || '';
      $('#cfg-user').value = cfg.user || '';
      $('#cfg-insta').value = cfg.insta || '';
    }
  } catch(e) {}
  setTimeout(bTick,250);
})();

/* ── Ripple effect ─────────────────────────────────────────────── */
document.addEventListener('click',function(e){
  const btn=e.target.closest('.btn,.scan-btn,.nav-btn,.li');
  if(!btn) return;
  const r=document.createElement('span');
  r.className='ripple';
  const rect=btn.getBoundingClientRect();
  const sz=Math.max(rect.width,rect.height);
  r.style.width=r.style.height=sz+'px';
  r.style.left=(e.clientX-rect.left-sz/2)+'px';
  r.style.top=(e.clientY-rect.top-sz/2)+'px';
  btn.appendChild(r);
  setTimeout(()=>r.remove(),600);
});

/* ── State ─────────────────────────────────────────────────────── */
let ws=null,wsOn=false,rDelay=1000;
const MAX_R=16000;
let selNet=-1,wifiNets=[],bleDevs=[];
let wAtk=false,bSpam=false;
let wPkts=0,bPkts=0,logN=0;
const MAX_LOG=500;
let pktH=new Array(30).fill(0),lastPkt=0;

const $=s=>document.querySelector(s);
const $$=s=>document.querySelectorAll(s);
const connDot=$('#conn-dot'),connLabel=$('#conn-label'),terminal=$('#terminal');

/* ── Templates ─────────────────────────────────────────────────── */
const TPL={beacon:{funny:"Free Virus\nFBI Surveillance Van\nTellMyWifiLoveHer\nItHurtsWhenIP\nDropItLikeItsHotspot\nSilence of the LANs\nNo More Mister WiFi",hacker:"Ransomware_C&C\nBotnet_Node_73\nTrojan_Dropper\nMetasploit_Bind\nKali_Linux_Root\nDarkWeb_Gateway\nZeroDay_Exploit",god:"God\nJesus\nHoly Spirit\nZeus\nThor\nOdin\nRa",creepy:"I Can Hear You Breathing\nLook Behind You\nWe Know What You Did\nThey Are Coming\nRun\nHide\nDont Look Away",meme:"Doge\nPepe\nHarambe\nStonks\nBig Chungus\nRick Astley\nShrek",tech:"Connecting...\nLoading...\nError 404\nNo Internet\nUpdating Windows\nRebooting Router\nFirmware Corrupted"},songs:{en:{revenge:"I hope you suffer\nBefore we die\nI will destroy you\nYou are nothing\nBurn in hell",love:"Never gonna give you up\nNever gonna let you down\nNever gonna run around and desert you\nNever gonna make you cry\nNever gonna say goodbye",sad:"Hello darkness my old friend\nI've come to talk with you again\nBecause a vision softly creeping\nLeft its seeds while I was sleeping",funny:"I'm a Barbie girl\nIn the Barbie world\nLife in plastic\nIt's fantastic",happy:"Because I'm happy\nClap along if you feel like a room without a roof",proposal:"Marry me, Juliet\nYou'll never have to be alone\nI love you\nAnd that's all I really know"},ml:{revenge:"Ninte kanneeril\nNjan arakkum\nOru pidi saambal\nAthilaanu ninte\nAvashishtam",love:"Anuraga Vilochananayi\nAthirile ponnum poovum\nKaralile thean chorum\nNinne kanaan\nNjan vannu",sad:"Aararum kaanathe\nNjan karayum\nEnthe ninnakku\nManassilaavilla\nNenjile murivukal",funny:"Kuttanadan punjayile\nKochu penne kuyilaale\nNinte kalyanam\nEnnanu\nParayu",happy:"Lajjavathiye\nNinte kalla chiri\nKaanum neram\nEnikku santhosham\nOru paadu",proposal:"Ente hridayam\nNinakkayi mathram\nNee ennu\nSammatham\nTharum"},ta:{revenge:"Unnai azhippaan\nNaan varuven\nEn kopam\nUnnai chutta\nEriyum",love:"Kanmani anbodu\nKaadhalan naan\nEzhuthum\nKavidhaigal\nUnakkaga",sad:"Nenjukkul peidhidum\nMaamazhai\nNeerukul moozhgi\nNaan saagiren",funny:"Why this kolaveri\nKolaveri di\nSoup song\nFlop song\nPadu",happy:"Aaluma doluma\nIsai mazhai\nTherikkavidalama\nSanthosham\nKondaadu",proposal:"Ennai kalyanam\nSeithu kolvaya\nUnnai\nKaadhalikkiren\nUyire"},hi:{revenge:"Inteqam dekhegi\nMera inteqam\nTum rote rahoge\nAansu nikalte\nRah jayenge",love:"Tum hi ho\nAb tum hi ho\nZindagi ab\nTum hi ho\nMeri aashiqui",sad:"Channa mereya\nMereya\nO piya\nDil ro raha\nHai",funny:"Bidi jalaile\nJigar se piya\nJigar ma badi\nAag hai\nDhua nikal",happy:"Badtameez dil\nBadtameez dil\nMaane na\nMaane na\nKhushi se",proposal:"Mujhse shaadi\nKarogi\nMain tera\nDeewana\nHoon"},kn:{revenge:"Ninagagi kayuthiddini\nNinna nashakke\nNanna kopa\nBeri hottide\nKadi",love:"Neene bari neene\nNanna manasali\nNinna bittu\nYaru illa\nKandu",sad:"Hrudaya odedu\nHogide\nNinna nenapu\nKaadutide\nMaremvey",funny:"Tarikere eri mele\nKari koli\nHuggi\nThinnoke\nBanni",happy:"Santoshakke\nHadu santhoshakke\nNali\nKuni\nNagu",proposal:"Nanna maduve\nAgutiya\nNinna\nPreetisuthene\nSada"}}};
function fillBeacon(v){if(v)$('#beacon-ssids').value=TPL.beacon[v]||''}
function fillSong(){const l=$('#song-lang').value;const c=$('#song-cat').value;if(l&&c)$('#beacon-ssids').value=TPL.songs[l]?.[c]||''}

/* ── Helpers ───────────────────────────────────────────────────── */
function fmtUp(s){const h=Math.floor(s/3600),m=Math.floor((s%3600)/60),sc=s%60;return String(h).padStart(2,'0')+':'+String(m).padStart(2,'0')+':'+String(sc).padStart(2,'0')}
function fmtB(b){if(b<1024)return b+' B';if(b<1048576)return(b/1024).toFixed(1)+' KB';return(b/1048576).toFixed(1)+' MB'}
function fmtN(n){return n.toLocaleString()}
function rssiQ(r){if(r>-50)return{q:'excellent',b:5};if(r>-60)return{q:'good',b:4};if(r>-70)return{q:'fair',b:3};if(r>-80)return{q:'weak',b:2};return{q:'poor',b:1}}
function sigHTML(rssi){const{q,b:bars}=rssiQ(rssi);let h='<div class="sig '+q+'">';for(let i=1;i<=5;i++)h+='<b class="'+(i<=bars?'on':'')+'"></b>';return h+'</div>'}
function ts(){return new Date().toLocaleTimeString()}

/* ── Toast ─────────────────────────────────────────────────────── */
function toast(msg,type='info',dur=3000){
  const c=$('#toast-container'),t=document.createElement('div');
  t.className='toast '+type;t.textContent=msg;c.appendChild(t);
  setTimeout(()=>{t.classList.add('hide');setTimeout(()=>t.remove(),300)},dur);
}

/* ── Modal ─────────────────────────────────────────────────────── */
let mCb=null;
function showModal(title,msg,cb){$('#modal-title').textContent=title;$('#modal-msg').textContent=msg;mCb=cb;$('#modal').classList.add('active')}
function closeModal(){$('#modal').classList.remove('active');mCb=null}
$('#modal-confirm').onclick=()=>{if(mCb)mCb();closeModal()};

/* ── Nav with sliding indicator ────────────────────────────────── */
const navBtns=$$('.nav-btn');
const navInd=$('#nav-ind');
function updateIndicator(btn){
  const rect=btn.getBoundingClientRect();
  const nav=btn.parentElement.getBoundingClientRect();
  navInd.style.left=(rect.left-nav.left+rect.width*.2)+'px';
  navInd.style.width=(rect.width*.6)+'px';
}
navBtns.forEach(btn=>{
  btn.addEventListener('click',()=>{
    const tab=btn.dataset.tab;
    navBtns.forEach(b=>b.classList.remove('active'));
    btn.classList.add('active');
    $$('.tab-content').forEach(s=>s.classList.remove('active'));
    $('#tab-'+tab).classList.add('active');
    updateIndicator(btn);
    if(tab==='monitor')scrollTerm();
  });
});
/* Init indicator position after DOM ready */
requestAnimationFrame(()=>{const a=$('.nav-btn.active');if(a)updateIndicator(a)});
window.addEventListener('resize',()=>{const a=$('.nav-btn.active');if(a)updateIndicator(a)});

/* ── LED ───────────────────────────────────────────────────────── */
$('#led-mode').onchange=function(){$('#custom-color-row').style.display=this.value==='custom'?'flex':'none'};

/* ── API ───────────────────────────────────────────────────────── */
async function api(p,o={}){try{const r=await fetch(p,o);return await r.json()}catch(e){toast('Request failed','error');return null}}
function post(p,b){return api(p,{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify(b)})}

/* ── Terminal ──────────────────────────────────────────────────── */
function addLog(msg,cls=''){const el=document.createElement('div');el.className='line'+(cls?' '+cls:'');el.innerHTML='<span class="ts">['+ts()+']</span> '+msg;terminal.appendChild(el);logN++;if(logN>MAX_LOG){terminal.removeChild(terminal.firstChild);logN--}$('#log-count').textContent=logN;scrollTerm()}
function scrollTerm(){terminal.scrollTop=terminal.scrollHeight}

/* ═══════════════════════════════════════════════════════════════
   WIFI
   ═══════════════════════════════════════════════════════════════ */
async function doWifiScan(){
  const btn=$('#btn-wifi-scan');btn.disabled=true;btn.innerHTML='<span class="spinner"></span> Scanning...';btn.classList.add('scanning');
  
  const empty=$('#wifi-empty');
  empty.style.display='block';
  empty.innerHTML='<div style="position:relative;width:80px;height:80px;margin:0 auto 20px;border-radius:50%;border:1px solid var(--cyan-d);background:rgba(0,229,255,0.05);overflow:hidden"><div class="radar-sweep"></div><div style="position:absolute;top:50%;left:50%;width:6px;height:6px;background:var(--cyan);border-radius:50%;transform:translate(-50%,-50%);box-shadow:0 0 10px var(--cyan)"></div></div><p style="color:var(--cyan);animation:pulse 1.5s infinite;font-weight:700;letter-spacing:1px">Scanning Frequencies...</p>';
  $('#wifi-list').innerHTML='';

  addLog('WiFi scan started','info');toast('Scanning networks...','info');
  await api('/api/wifi/scan');
  let att=0;const poll=setInterval(async()=>{att++;
    const d=await api('/api/wifi/results');
    if((d&&Array.isArray(d)&&d.length>0)||att>=10){clearInterval(poll);
      btn.disabled=false;btn.classList.remove('scanning');btn.innerHTML='<svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="11" cy="11" r="8"/><path d="M21 21l-4.35-4.35"/></svg>Scan Networks';
      if(d&&Array.isArray(d)){wifiNets=d;renderWifi();addLog('Found '+d.length+' networks','info');toast('Found '+d.length+' networks','success')}
      else toast('No results','error');
    }
  },1000);
}
function renderWifi(){
  const list=$('#wifi-list'),empty=$('#wifi-empty');
  if(!wifiNets.length){empty.style.display='block';list.innerHTML='';return}
  empty.style.display='none';
  list.innerHTML=wifiNets.map((n,i)=>{
    const enc=n.enc&&n.enc!=='OPEN';const sel=i===selNet?' sel':'';
    return '<div class="li'+sel+'" onclick="window._sn('+i+')" style="animation:slideUp .3s '+(i*.04)+'s both">'
      +'<div class="li-icon">'+(enc?'<svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="3" y="11" width="18" height="11" rx="2" ry="2"/><path d="M7 11V7a5 5 0 0 1 10 0v4"/></svg>':'<svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="3" y="11" width="18" height="11" rx="2" ry="2"/><path d="M7 11V7a5 5 0 0 1 9.9-1"/></svg>')+'</div>'
      +'<div class="li-body"><div class="li-name">'+(n.ssid||'&lt;Hidden&gt;')+'</div><div class="li-sub">CH '+n.channel+' &bull; '+n.bssid+'</div></div>'
      +'<div class="li-right">'+sigHTML(n.rssi)+'<div style="font-size:.55rem;color:var(--dim2);margin-top:2px;font-family:var(--mono)">'+n.rssi+'</div></div></div>';
  }).join('');
}
window._sn=function(i){selNet=(selNet===i)?-1:i;renderWifi()};

function startDeauth(t){
  if(t==='selected'){
    if(selNet<0){toast('Select a network first','warning');return}
    showModal('Deauth','Send deauth to "'+(wifiNets[selNet]?.ssid||'Hidden')+'"?',async()=>{
      await post('/api/wifi/deauth',{target:selNet});toast('Deauth started','error');addLog('Deauth: '+wifiNets[selNet]?.ssid,'err');
    });
  }else{
    showModal('Deauth All','Deauth ALL scanned networks?',async()=>{await post('/api/wifi/deauth',{target:'all'});toast('Deauth all','error');addLog('Deauth ALL','err')});
  }
}
const templates = {
  funny: "FBI Surveillance Van\nTellMyWifiLoveHer\nItHurtsWhenIP\nDropItLikeItsHotspot\nSilenceOfTheLANs\nWuTangLAN\nPrettyFlyForAWifi",
  hacker: "Ransomware_C2_Node\nBotnet_Command\nTrojan_Uplink\nKeylogger_Dump\nZeroDay_Payload\nBackdoor_Active",
  god: "God_Is_Watching\nYou_Have_Sinned\nRepent_Now\nHeaven_Guest\nDivine_Intervention",
  creepy: "I_Can_See_You\nLook_Behind_You\nIm_Under_Your_Bed\nWe_Are_Watching\nDont_Look_Away",
  meme: "Doge_Network\nMuch_Wifi\nVery_Speed\nWow_Connect\nStonks_Only_Go_Up\nRick_Astley_Fan_Club",
  tech: "Error_404_Wifi_Not_Found\nConnecting...\nLoading...\nUpdate_Required\nVirus_Detected_Click_Here"
};

const songs = {
  en: {
    revenge: "I'll make you pay\nYou crossed the line\nThis is the end\nNo turning back\nWatch your back",
    love: "Never gonna give you up\nNever gonna let you down\nNever gonna run around\nAnd desert you",
    sad: "All by myself\nDon't wanna be\nAll by myself\nAnymore",
    funny: "Baby shark doo doo\nMommy shark doo doo\nDaddy shark doo doo",
    happy: "Because I'm happy\nClap along if you feel\nLike a room without a roof",
    proposal: "Will you marry me\nBe my wife\nSpend my life with you"
  },
  ml: {
    revenge: "Ninne njan kanichu tharam\nIvide theerunnu\nIni thirichu varilla\nSookshicho",
    love: "Anuragathin velayil\nVaramai vannathi\nNee en swantham",
    sad: "Aararum ariyathe\nKarayunnu njan\nOru paatu moolunnu",
    funny: "Kuttanadan punchayile\nKochu penne\nKuyile",
    happy: "Aarpo erro\nSanthosham kond\nEnikku irikkan vayye",
    proposal: "Ente pathi aakumo\nKoode poramo\nJeevitham muzhuvan"
  },
  ta: {
    revenge: "Unnai vida maten\nIdhu dhaan mudivu\nThirumbi vara maten\nPathiram",
    love: "Ennai kollathey\nUyire uyire\nNee vendum",
    sad: "Kanneer thuligal\nYarukkum theriyamal\nAlugiren",
    funny: "Why this kolaveri\nDi\nKolaveri kolaveri di",
    happy: "Santhosham\nMugathil\nEppodhum",
    proposal: "En pondatti aagidriya\nKalyanam panniklama\nOru life"
  },
  hi: {
    revenge: "Chodunga nahi\nYe ant hai\nPeeche mat dekhna\nBachke rehna",
    love: "Tum hi ho\nAab tum hi ho\nMeri aashiqui\nAb tum hi ho",
    sad: "Tujhe bhula diya\nOh\nTujhe bhula diya",
    funny: "Lungi dance\nLungi dance\nLungi dance",
    happy: "Aaj main upar\nAasmaan neeche\nAaj main aage",
    proposal: "Mujhse shaadi karogi\nMeri banogi\nSath chalogi"
  },
  kn: {
    revenge: "Bidalare ninna\nIdu kone\nHirige hogolla\nEccharike",
    love: "Neene neene\nNanna usiru\nNinna bittu iralare",
    sad: "Kanneru bandide\nYaarigu helade\nAluthiddini",
    funny: "Bombaat hudugi\nNodoke\nSuper aagidale",
    happy: "Santhosha\nKushi\nNagu nagutha",
    proposal: "Nanna maduve aagthiya\nJotheyalli irtiya\nOndu jeevana"
  }
};

window.fillBeacon = function(val) {
  if (templates[val]) {
    $('#beacon-ssids').value = templates[val];
  } else {
    $('#beacon-ssids').value = '';
  }
};

window.fillSong = function() {
  const lang = $('#song-lang').value;
  const cat = $('#song-cat').value;
  if (lang && cat && songs[lang] && songs[lang][cat]) {
    $('#beacon-ssids').value = songs[lang][cat];
  } else {
    $('#beacon-ssids').value = '';
  }
};

async function startBeacon(){const s=$('#beacon-ssids').value.trim();if(!s){toast('Enter SSIDs','warning');return}await post('/api/wifi/beacon',{ssids:s});toast('Beacon flood started','info');addLog('Beacon: '+s.split('\n').length+' SSIDs','warn')}
async function startRickroll(){const s=$('#beacon-ssids').value.trim();if(!s){toast('Enter Lyrics','warning');return}await post('/api/wifi/rickroll',{lyrics:s});toast('Rickroll started','info');addLog('Rickroll flood','warn')}
async function startProbe(){await post('/api/wifi/probe',{});toast('Probe flood','info');addLog('Probe flood','warn')}
async function stopWifiAttack(){await api('/api/wifi/stop',{method:'POST'});toast('Stopped','success');addLog('WiFi stopped','info')}
function quickBeacon(){navBtns.forEach(b=>b.classList.remove('active'));navBtns[1].classList.add('active');updateIndicator(navBtns[1]);$$('.tab-content').forEach(s=>s.classList.remove('active'));$('#tab-wifi').classList.add('active');$('#beacon-ssids').focus()}

/* ═══════════════════════════════════════════════════════════════
   CLONE AP
   ═══════════════════════════════════════════════════════════════ */
// Update clone info label when a network is selected
var _origSn = window._sn;
window._sn = function(i) {
  selNet = (selNet===i) ? -1 : i;
  renderWifi();
  // Update clone info
  var info = $('#clone-info');
  if (selNet >= 0 && wifiNets[selNet]) {
    info.innerHTML = 'Target: <span style="color:var(--g);font-weight:700">' + (wifiNets[selNet].ssid || '&lt;Hidden&gt;') + '</span> (CH ' + wifiNets[selNet].channel + ')';
  } else {
    info.textContent = 'Select a network above first';
  }
};

async function startCloneAP() {
  if (selNet < 0 || !wifiNets[selNet]) {
    toast('Select a network first', 'warning');
    return;
  }
  var tpl = parseInt($('#clone-template').value) || 0;
  var title = $('#clone-title').value.trim() || wifiNets[selNet].ssid || 'Welcome';
  var ssidName = wifiNets[selNet].ssid;

  showModal('Clone AP',
    'Start clone AP "<b>' + ssidName + '</b>"?<br><span style="font-size:.7rem;opacity:.7">Captive portal will show the selected template.</span>',
    async () => {
      var res = await post('/api/wifi/clone', {
        scanIndex: selNet,
        template: tpl,
        title: title
      });
      if (res && res.status === 'starting') {
        toast('Clone AP starting: ' + ssidName, 'info');
        addLog('Clone AP: ' + ssidName + ' (tpl ' + tpl + ')', 'warn');
        $('#btn-clone-start').style.display = 'none';
        $('#btn-clone-stop').style.display = 'inline-flex';
        $('#clone-status').style.display = 'block';
        $('#clone-ssid-label').textContent = ssidName;
      } else {
        toast('Clone failed', 'error');
      }
    }
  );
}

async function stopCloneAP() {
  showModal('Stop Clone', 'Stop clone AP and revert to NeoCAT WiFi?', async () => {
    await post('/api/wifi/clone/stop', {});
    toast('Clone AP stopped', 'success');
    addLog('Clone AP stopped', 'info');
    $('#btn-clone-start').style.display = 'inline-flex';
    $('#btn-clone-stop').style.display = 'none';
    $('#clone-status').style.display = 'none';
  });
}

/* ═══════════════════════════════════════════════════════════════
   BLE
   ═══════════════════════════════════════════════════════════════ */
async function doBleScan(){
  const btn=$('#btn-ble-scan');btn.disabled=true;btn.innerHTML='<span class="spinner"></span> Scanning...';btn.classList.add('scanning');
  
  const empty=$('#ble-empty');
  empty.style.display='block';
  empty.innerHTML='<div style="position:relative;width:80px;height:80px;margin:0 auto 20px;border-radius:50%;border:1px solid var(--purple-d);background:rgba(176,106,255,0.05);overflow:hidden"><div class="radar-sweep ble"></div><div style="position:absolute;top:50%;left:50%;width:6px;height:6px;background:var(--purple);border-radius:50%;transform:translate(-50%,-50%);box-shadow:0 0 10px var(--purple)"></div></div><p style="color:var(--purple);animation:pulse 1.5s infinite;font-weight:700;letter-spacing:1px">Scanning BLE...</p>';
  $('#ble-list').innerHTML='';

  addLog('BLE scan started','info');toast('Scanning BLE...','info');
  await api('/api/ble/scan');
  setTimeout(async()=>{
    const d=await api('/api/ble/results');
    btn.disabled=false;btn.classList.remove('scanning');btn.innerHTML='<svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="11" cy="11" r="8"/><path d="M21 21l-4.35-4.35"/></svg>Scan BLE Devices';
    if(d&&Array.isArray(d)){bleDevs=d;renderBle();addLog('Found '+d.length+' BLE devices','info');toast('Found '+d.length+' BLE','success')}
  },6000);
}
function renderBle(){
  const list=$('#ble-list'),empty=$('#ble-empty');
  if(!bleDevs.length){empty.style.display='block';list.innerHTML='';return}
  empty.style.display='none';
  list.innerHTML=bleDevs.map((d,i)=>'<div class="li" style="animation:slideUp .3s '+(i*.04)+'s both"><div class="li-icon"><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="6.5 6.5 17.5 17.5 12 23 12 1 17.5 6.5 6.5 17.5"/></svg></div><div class="li-body"><div class="li-name">'+(d.name||'Unknown')+'</div><div class="li-sub">'+d.mac+'</div></div><div class="li-right">'+sigHTML(d.rssi)+'<div style="font-size:.55rem;color:var(--dim2);margin-top:2px;font-family:var(--mono)">'+d.rssi+'</div></div></div>').join('');
}
async function startBleSpam(t){await post('/api/ble/spam',{type:t});toast('BLE '+t+' spam started','info');addLog('BLE spam: '+t,'warn')}
async function stopBleSpam(){await api('/api/ble/stop',{method:'POST'});toast('BLE stopped','success');addLog('BLE stopped','info')}

/* ═══════════════════════════════════════════════════════════════
   SETTINGS
   ═══════════════════════════════════════════════════════════════ */
async function setLedMode(m){await post('/api/led/mode',{mode:m});toast('LED: '+m,'success')}
async function setLedColor(hex){const r=parseInt(hex.slice(1,3),16),g=parseInt(hex.slice(3,5),16),b=parseInt(hex.slice(5,7),16);$('#color-preview').style.background=hex;$('#color-hex').textContent=hex;await post('/api/led/color',{r,g,b})}
function confirmReboot(){showModal('Reboot','Restart the ESP32?',async()=>{toast('Rebooting...','warning');addLog('Reboot','err');await post('/api/system/reboot',{})})}
async function saveConfig(){
  const ssid=$('#cfg-ssid').value.trim(), pass=$('#cfg-pass').value, user=$('#cfg-user').value.trim(), insta=$('#cfg-insta').value.trim();
  if(!ssid){toast('SSID required','warning');return}
  if(pass && pass.length < 8){toast('Password must be 8+ chars','warning');return}
  await post('/api/config',{ssid,pass,user,insta});
  toast('Saved! Reboot to apply WiFi changes','success');
}
async function startChatRoom(){
  const name=$('#chat-name').value.trim()||'CATroom', pass=$('#chat-pass').value;
  if(pass && pass.length < 8){toast('Chat password must be 8+ chars','warning');return}
  await post('/api/chat/room',{name,password:pass});
  await post('/api/chat/room/start',{});
  toast('Chat room started! WiFi: '+name,'success');addLog('CATroom: '+name,'info');
  $('#btn-chat-start').style.display='none';$('#btn-chat-stop').style.display='inline-flex';
  $('#chat-status').innerHTML='Room: <span style="color:var(--g)">Active</span> — Connect to "'+name+'" WiFi then open <b>192.168.4.1/chat</b>';
}
async function stopChatRoom(){
  showModal('Stop Room','Stop chat and revert WiFi?',async()=>{
    await post('/api/chat/room/stop',{});
    toast('Chat room stopped','warning');addLog('CATroom stopped','warn');
    $('#btn-chat-start').style.display='inline-flex';$('#btn-chat-stop').style.display='none';
    $('#chat-status').textContent='Room: Inactive';
  });
}

/* ═══════════════════════════════════════════════════════════════
   TERMINAL
   ═══════════════════════════════════════════════════════════════ */
function termPrint(text, cls=''){
  const d=$('#term-display');
  d.innerHTML+='<div class="line '+cls+'">'+text.replace(/</g,'&lt;').replace(/>/g,'&gt;')+'</div>';
  d.scrollTop=d.scrollHeight;
}
window.termSend = async function(){
  const inp=$('#term-input');
  const cmd=inp.value.trim();
  if(!cmd)return;
  inp.value='';
  termPrint('neocat> '+cmd, 'info');
  
  if(cmd==='clear'){$('#term-display').innerHTML='';return}
  if(cmd==='help'){
    termPrint('Available Commands:');
    termPrint('  help         - Show this menu');
    termPrint('  clear        - Clear terminal');
    termPrint('  scan wifi    - Scan WiFi networks');
    termPrint('  scan ble     - Scan BLE devices');
    termPrint('  deauth all   - Deauth all networks');
    termPrint('  stop         - Stop all attacks');
    termPrint('  reboot       - Reboot device');
    return;
  }
  
  // Local shortcuts
  if(cmd==='scan wifi'){doWifiScan();termPrint('Starting WiFi Scan...','warn');return}
  if(cmd==='scan ble'){doBleScan();termPrint('Starting BLE Scan...','warn');return}
  if(cmd==='deauth all'){startDeauth('all');return}
  if(cmd==='stop'){stopWifiAttack();stopBleSpam();return}
  if(cmd==='reboot'){confirmReboot();return}
  
  // Forward to backend API for everything else
  try {
    const res = await post('/api/term', { cmd });
    if(res && res.output) {
      termPrint(res.output);
    } else {
      termPrint('No response from device', 'err');
    }
  } catch(e) {
    termPrint('Command failed: ' + e.message, 'err');
  }
}
$('#term-input').addEventListener('keypress',e=>{if(e.key==='Enter')termSend()});

window.doWifiScan=doWifiScan;window.doBleScan=doBleScan;window.quickBeacon=quickBeacon;
window.startDeauth=startDeauth;window.startBeacon=startBeacon;window.startRickroll=startRickroll;
window.startProbe=startProbe;window.stopWifiAttack=stopWifiAttack;
window.startBleSpam=startBleSpam;window.stopBleSpam=stopBleSpam;
window.setLedMode=setLedMode;window.setLedColor=setLedColor;window.confirmReboot=confirmReboot;
window.closeModal=closeModal;window.saveConfig=saveConfig;
window.startChatRoom=startChatRoom;window.stopChatRoom=stopChatRoom;

/* ═══════════════════════════════════════════════════════════════
   WEBSOCKET
   ═══════════════════════════════════════════════════════════════ */
function connectWS(){
  ws=new WebSocket('ws://'+location.host+'/ws');
  ws.onopen=()=>{wsOn=true;rDelay=1000;connDot.classList.add('on');connLabel.textContent='ONLINE';addLog('Connected','info')};
  ws.onclose=()=>{wsOn=false;connDot.classList.remove('on');connLabel.textContent='OFFLINE';addLog('Disconnected — retry in '+(rDelay/1000)+'s','err');setTimeout(connectWS,rDelay);rDelay=Math.min(rDelay*2,MAX_R)};
  ws.onerror=()=>{ws.close()};
  ws.onmessage=ev=>{try{handleWS(JSON.parse(ev.data))}catch(e){}};
}
function handleWS(d){
  if(d.type==='system_info'){
    $('#si-chip').textContent=d.chipModel||'--';$('#si-rev').textContent=d.chipRevision??'--';
    $('#si-cpu').textContent=(d.cpuFreqMHz||'--')+' MHz';$('#si-flash').textContent=fmtB(d.flashSize||0);
    $('#si-sdk').textContent=d.sdkVersion||'--';$('#si-fw').textContent=d.firmwareVersion||'--';
    $('#hero-status').textContent=d.chipModel+' @ '+d.cpuFreqMHz+' MHz — '+fmtB(d.flashSize)+' flash';return;
  }
  if(d.type==='status'){
    $('#s-uptime').textContent=fmtUp(d.uptime||0);$('#s-heap').textContent=fmtB(d.freeHeap||0);
    wPkts=d.wifiPackets||0;bPkts=d.blePackets||0;
    $('#s-pkts').textContent=fmtN(wPkts+bPkts);$('#s-devs').textContent=fmtN((d.wifiNetworks||0)+(d.bleDevices||0));
    /* System health */
    if(d.cpuTemp!==undefined){$('#sh-temp').textContent=d.cpuTemp.toFixed(1)+'°C';$('#sh-temp').style.color=d.cpuTemp>70?'#ff4444':'var(--w)'}
    if(d.cpuFreq)$('#sh-freq').textContent=d.cpuFreq+' MHz';
    $('#sh-ram').textContent=fmtB(d.freeHeap||0);$('#sh-ram').style.color=(d.freeHeap||0)<30000?'#ff4444':'var(--w)';
    /* Battery health */
    if(d.batteryPercent!==undefined){
      let bStr = d.batteryPercent + '% (' + d.batteryVoltage.toFixed(2) + 'V)';
      if(d.isCharging) bStr = '⚡ ' + bStr;
      $('#sh-bat').textContent = bStr;
      $('#sh-bat').style.color = d.batteryPercent < 20 && !d.isCharging ? '#ff4444' : 'var(--g)';
    }
    /* Chat status */
    if(d.chatActive){$('#sh-chat').textContent=d.chatRoom||'Active';$('#sh-chat').style.color='var(--g)';$('#btn-chat-start').style.display='none';$('#btn-chat-stop').style.display='inline-flex';$('#chat-status').innerHTML='Room: <span style="color:var(--g)">Active</span> — '+d.chatUsers+' users'}
    else{$('#sh-chat').textContent='Inactive';$('#sh-chat').style.color='var(--dim2)';$('#sh-chatusers').textContent='0'}
    if(d.chatUsers!==undefined)$('#sh-chatusers').textContent=d.chatUsers;
    wAtk=!!d.wifiAttacking;const wb=$('#wifi-atk-banner');
    if(wAtk){wb.classList.add('active');$('#wifi-atk-label').textContent=(d.wifiAttackType||'Attack')+' active';$('#wifi-atk-pkts').textContent=fmtN(wPkts)+' pkts';$('#btn-wifi-stop').style.display='block'}
    else{wb.classList.remove('active');$('#btn-wifi-stop').style.display='none'}
    bSpam=!!d.bleSpamming;const bb=$('#ble-atk-banner');
    if(bSpam){bb.classList.add('active');$('#ble-atk-label').textContent=(d.bleSpamType||'Spam')+' active';$('#ble-atk-pkts').textContent=fmtN(bPkts)+' pkts';$('#btn-ble-stop').style.display='block'}
    else{bb.classList.remove('active');$('#btn-ble-stop').style.display='none'}
    const ct=wPkts+bPkts,delta=ct-lastPkt;lastPkt=ct;pktH.push(Math.max(0,delta));if(pktH.length>30)pktH.shift();renderChart();return;
  }
  if(d.type==='pong')return;
  addLog(JSON.stringify(d),'info');
}
function renderChart(){const c=$('#pkt-chart'),mx=Math.max(1,...pktH);c.innerHTML=pktH.map(v=>'<div style="height:'+Math.max(2,(v/mx)*60)+'px"></div>').join('')}

/* ── Init ──────────────────────────────────────────────────────── */
renderChart();connectWS();
setInterval(()=>{if(ws&&ws.readyState===WebSocket.OPEN)ws.send('{"cmd":"ping"}')},15000);
(async()=>{const i=await api('/api/system/info');if(i)handleWS({type:'system_info',...i})})();
addLog('NeoCAT V1 Dashboard loaded','info');
addLog('Powered by Glyph S3','info');

/* ── PWA install prompt ────────────────────────────────────────── */
let deferredPrompt;
window.addEventListener('beforeinstallprompt',e=>{
  e.preventDefault();deferredPrompt=e;
  const banner=document.createElement('div');
  banner.className='pwa-banner';banner.style.display='flex';
  banner.innerHTML='&#x1F4F2; Install NeoCAT App';
  banner.onclick=()=>{deferredPrompt.prompt();deferredPrompt.userChoice.then(()=>{banner.remove();deferredPrompt=null})};
  document.body.appendChild(banner);
  setTimeout(()=>banner.remove(),10000);
});

})();
</script>
</body>
</html>
)rawliteral";

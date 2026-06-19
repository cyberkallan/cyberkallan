#pragma once
/**
 * @file web_dashboard.h
 * @brief NeoCAT V1 â€” Premium Mobile-App Dashboard (PROGMEM)
 *
 * Powered by Glyph S3. PWA-ready. Ripple effects, staggered
 * card animations, sliding tab indicator, animated boot screen,
 * frosted glass navigation. Zero external dependencies.
 */

#include <pgmspace.h>

/* â”€â”€ PWA Manifest (served at /manifest.json) â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ */
const char PWA_MANIFEST[] PROGMEM = R"json({
  "name":"NeoCAT V1 â€” Glyph S3",
  "short_name":"NeoCAT",
  "description":"ESP32-S3 Security Toolkit powered by Glyph S3",
  "start_url":"/",
  "display":"standalone",
  "orientation":"portrait",
  "background_color":"#050505",
  "theme_color":"#050505",
  "categories":["utilities","security"],
  "icons":[
    {"src":"data:image/svg+xml,<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 100 100'><rect fill='%23050505' width='100' height='100' rx='20'/><text x='50' y='62' text-anchor='middle' font-size='52'>ðŸ±</text></svg>","sizes":"any","type":"image/svg+xml"}
  ]
})json";

/* â”€â”€ Main Dashboard HTML â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ */
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
<link rel="icon" href="data:image/svg+xml,<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 100 100'><text y='.9em' font-size='90'>ðŸ±</text></svg>">
<title>NeoCAT V1</title>
<style>
/* â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
   ROOT / RESET / TOKENS
   â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â• */
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

/* â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
   ANIMATIONS
   â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â• */
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

/* â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
   BOOT SCREEN
   â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â• */
#boot{
  position:fixed;inset:0;z-index:9999;background:var(--bg);
  display:flex;flex-direction:column;align-items:center;justify-content:center;
  transition:opacity .7s var(--ease),filter .7s var(--ease);
}
#boot.done{opacity:0;filter:blur(8px);pointer-events:none}
.b-particles{position:absolute;inset:0;overflow:hidden;opacity:.15}
.b-p{position:absolute;width:1px;background:var(--g);border-radius:1px;animation:matrixFade linear infinite;opacity:0}
.b-icon{font-size:4rem;animation:bootPulse 1.4s ease-in-out infinite alternate;z-index:1}
.b-brand{font-size:2.2rem;font-weight:900;letter-spacing:6px;margin-top:.8rem;z-index:1;
  background:linear-gradient(135deg,var(--g),var(--cyan),var(--g));background-size:200% 200%;
  -webkit-background-clip:text;-webkit-text-fill-color:transparent;background-clip:text;
  animation:gradientShift 3s ease infinite}
.b-sub{font-size:.65rem;color:var(--dim);letter-spacing:4px;margin-top:6px;text-transform:uppercase;z-index:1}
.b-powered{font-size:.55rem;color:var(--dim2);letter-spacing:2px;margin-top:2px;z-index:1}
.b-log{width:260px;margin-top:1.5rem;font-family:var(--mono);font-size:.58rem;color:var(--g2);line-height:1.9;min-height:80px;max-height:80px;overflow:hidden;z-index:1}
.b-progress{width:220px;height:3px;background:var(--s2);border-radius:2px;margin-top:1rem;overflow:hidden;z-index:1}
.b-bar{height:100%;width:0;background:linear-gradient(90deg,var(--g),var(--cyan));box-shadow:0 0 12px var(--g5);border-radius:2px;transition:width .2s var(--ease)}
.b-ver{margin-top:1.2rem;font-size:.5rem;color:var(--dim3);letter-spacing:3px;z-index:1}

/* â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
   LEGAL MODAL
   â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â• */
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

/* â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
   APP SHELL
   â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â• */
#app{display:none;flex-direction:column;height:100vh;height:100dvh;overflow:hidden;max-width:520px;margin:0 auto;position:relative}
#app.visible{display:flex}

/* â”€â”€ TOPBAR â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ */
.topbar{
  height:var(--top);display:flex;align-items:center;justify-content:space-between;
  padding:0 16px;
  background:rgba(5,5,5,.85);backdrop-filter:saturate(180%) blur(20px);-webkit-backdrop-filter:saturate(180%) blur(20px);
  border-bottom:1px solid var(--b1);flex-shrink:0;z-index:100;
}
.tb-left{display:flex;align-items:center;gap:10px}
.tb-dot{width:8px;height:8px;border-radius:50%;background:var(--dim2);transition:all .5s var(--ease)}
.tb-dot.on{background:var(--g);box-shadow:0 0 12px var(--g5);animation:breathe 3s ease-in-out infinite}
.tb-brand{font-family:var(--mono);font-weight:800;font-size:.85rem;letter-spacing:1.5px;
  background:linear-gradient(135deg,var(--g),var(--cyan));-webkit-background-clip:text;-webkit-text-fill-color:transparent;background-clip:text}
.tb-right{display:flex;align-items:center;gap:8px}
.tb-chip{font-family:var(--mono);font-size:.55rem;color:var(--dim);padding:4px 10px;background:var(--s2);border:1px solid var(--b1);border-radius:20px;letter-spacing:.5px}
.tb-status{font-size:.55rem;color:var(--dim2);font-family:var(--mono);letter-spacing:.5px}

/* â”€â”€ MAIN CONTENT â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ */
main{flex:1;overflow-y:auto;overflow-x:hidden;padding:14px 14px 20px;scroll-behavior:smooth;-webkit-overflow-scrolling:touch}
.tab-content{display:none}
.tab-content.active{display:block;animation:slideUp .35s var(--ease)}

/* â”€â”€ BOTTOM NAV â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ */
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

/* â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
   COMPONENTS
   â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â• */

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
.about{text-align:center;padding:20px;margin-top:14px}
.about-logo{font-size:2.2rem;margin-bottom:6px}
.about-name{font-size:1.1rem;font-weight:900;letter-spacing:3px;font-family:var(--mono);
  background:linear-gradient(135deg,var(--g),var(--cyan));-webkit-background-clip:text;-webkit-text-fill-color:transparent;background-clip:text}
.about-powered{font-size:.6rem;color:var(--dim);margin-top:4px;letter-spacing:1.5px}
.about-legal{font-size:.52rem;color:var(--dim2);margin-top:6px;line-height:1.6}

@media(min-width:600px){
  main{padding:20px 24px}
  .stat-grid{grid-template-columns:repeat(4,1fr)}
  .btn-grp{flex-wrap:nowrap}
}
</style>
</head>
<body>

<!-- â•â• BOOT SCREEN â•â• -->
<div id="boot">
  <div class="b-particles" id="b-particles"></div>
  <div class="b-icon">&#x1F431;</div>
  <div class="b-brand">NEOCAT</div>
  <div class="b-sub">Security Toolkit</div>
  <div class="b-powered">Powered by Glyph S3</div>
  <div class="b-log" id="b-log"></div>
  <div class="b-progress"><div class="b-bar" id="b-bar"></div></div>
  <div class="b-ver">V1.0.0 &bull; ESP32-S3</div>
</div>

<!-- â•â• LEGAL â•â• -->
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

<!-- â•â• APP â•â• -->
<div id="app">

<header class="topbar">
  <div class="tb-left">
    <div class="tb-dot" id="conn-dot"></div>
    <span class="tb-brand">NEOCAT</span>
  </div>
  <div class="tb-right">
    <span class="tb-chip">192.168.4.1</span>
    <span class="tb-status" id="conn-label">OFFLINE</span>
  </div>
</header>

<main>

<!-- â”€â”€ DASHBOARD â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ -->
<section id="tab-dashboard" class="tab-content active">
  <div class="hero">
    <div class="hero-title">&#x26A1; System Online</div>
    <div class="hero-sub" id="hero-status">Initialising&hellip;</div>
    <div class="hero-badge">&#x1F431; NeoCAT V1 &bull; Glyph S3</div>
  </div>
  <div class="stat-grid">
    <div class="stat"><div class="stat-icon">&#x23F1;</div><div class="stat-val" id="s-uptime">00:00:00</div><div class="stat-lbl">Uptime</div></div>
    <div class="stat"><div class="stat-icon">&#x1F4BE;</div><div class="stat-val" id="s-heap">--</div><div class="stat-lbl">Memory</div></div>
    <div class="stat"><div class="stat-icon">&#x1F4E1;</div><div class="stat-val" id="s-pkts">0</div><div class="stat-lbl">Packets</div></div>
    <div class="stat"><div class="stat-icon">&#x1F4F6;</div><div class="stat-val" id="s-devs">0</div><div class="stat-lbl">Devices</div></div>
  </div>
  <div class="sec">Quick Actions</div>
  <div class="btn-grp">
    <button class="btn btn-a btn-s" onclick="doWifiScan()">&#x1F50D; WiFi Scan</button>
    <button class="btn btn-o btn-s" onclick="doBleScan()">&#x1F4F6; BLE Scan</button>
    <button class="btn btn-o btn-s" onclick="quickBeacon()">&#x1F4E1; Beacon</button>
  </div>
</section>

<!-- â”€â”€ WIFI â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ -->
<section id="tab-wifi" class="tab-content">
  <div class="atk w" id="wifi-atk-banner"><span class="spinner"></span><span id="wifi-atk-label">Attack active</span><span style="margin-left:auto;font-variant-numeric:tabular-nums;font-family:var(--mono);font-size:.65rem" id="wifi-atk-pkts">0 pkts</span></div>
  <button class="scan-btn" id="btn-wifi-scan" onclick="doWifiScan()"><svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="11" cy="11" r="8"/><path d="M21 21l-4.35-4.35"/></svg>Scan Networks</button>
  <div id="wifi-list-wrap"><div class="empty" id="wifi-empty"><div class="icon">&#x1F4E1;</div><p>Tap scan to discover networks</p></div><div id="wifi-list"></div></div>
  <div class="sec">Attack Panel</div>
  <div class="btn-grp"><button class="btn btn-d btn-s" onclick="startDeauth('selected')">&#x26A1; Deauth</button><button class="btn btn-d btn-s" onclick="startDeauth('all')">&#x1F4A5; Deauth All</button></div>
  <div class="card"><div class="card-title"><span class="dot"></span>Beacon Flood</div><textarea class="inp" id="beacon-ssids" placeholder="Enter SSIDs (one per line)&#10;FreeWiFi&#10;Guest Network" rows="3"></textarea><div class="btn-grp" style="margin-top:10px"><button class="btn btn-a btn-s" onclick="startBeacon()">&#x1F4E1; Beacon</button><button class="btn btn-o btn-s" onclick="startRickroll()">&#x1F3B5; Rick</button><button class="btn btn-o btn-s" onclick="startProbe()">&#x1F50E; Probe</button></div></div>
  <button class="btn btn-d btn-f" id="btn-wifi-stop" onclick="stopWifiAttack()" style="display:none">&#x1F6D1; Stop All</button>
</section>

<!-- â”€â”€ BLE â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ -->
<section id="tab-ble" class="tab-content">
  <div class="atk b" id="ble-atk-banner"><span class="spinner"></span><span id="ble-atk-label">BLE Spam active</span><span style="margin-left:auto;font-variant-numeric:tabular-nums;font-family:var(--mono);font-size:.65rem" id="ble-atk-pkts">0 pkts</span></div>
  <button class="scan-btn" id="btn-ble-scan" onclick="doBleScan()"><svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="11" cy="11" r="8"/><path d="M21 21l-4.35-4.35"/></svg>Scan BLE Devices</button>
  <div id="ble-list-wrap"><div class="empty" id="ble-empty"><div class="icon">&#x1F4F6;</div><p>Tap scan to discover BLE devices</p></div><div id="ble-list"></div></div>
  <div class="sec">BLE Spam</div>
  <div class="btn-grp"><button class="btn btn-o btn-s" onclick="startBleSpam('apple')">&#x1F34E; Apple</button><button class="btn btn-o btn-s" onclick="startBleSpam('samsung')">&#x1F4F1; Samsung</button><button class="btn btn-o btn-s" onclick="startBleSpam('windows')">&#x1F5A5; Windows</button><button class="btn btn-a btn-s" onclick="startBleSpam('all')">&#x1F4A5; All</button></div>
  <button class="btn btn-d btn-f" id="btn-ble-stop" onclick="stopBleSpam()" style="display:none">&#x1F6D1; Stop BLE</button>
</section>

<!-- â”€â”€ MONITOR â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ -->
<section id="tab-monitor" class="tab-content">
  <div class="card-title"><span class="dot"></span>Live Console</div>
  <div class="term" id="terminal"></div>
  <div style="margin-top:6px;text-align:right;font-size:.55rem;color:var(--dim2);font-family:var(--mono)"><span id="log-count">0</span> entries</div>
  <div class="sec">Packet Rate</div>
  <div class="bars" id="pkt-chart"></div>
</section>

<!-- â”€â”€ SETTINGS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ -->
<section id="tab-settings" class="tab-content">
  <div class="card"><div class="card-title"><span class="dot"></span>LED Control</div><select class="inp" id="led-mode" onchange="setLedMode(this.value)" style="margin-bottom:10px"><option value="idle">Idle</option><option value="scanning">Scanning</option><option value="attacking">Attacking</option><option value="off">Off</option><option value="custom">Custom Color</option></select><div class="clr-row" id="custom-color-row" style="display:none"><input type="color" id="led-color" value="#00ff41" onchange="setLedColor(this.value)"><div class="clr-prev" id="color-preview" style="background:#00ff41"></div><span style="font-size:.68rem;color:var(--dim);font-family:var(--mono)" id="color-hex">#00ff41</span></div></div>
  <div class="card"><div class="card-title"><span class="dot"></span>System Info</div><div style="font-size:.72rem;line-height:2.2;color:var(--dim)"><div>Chip <span style="float:right;color:var(--w);font-family:var(--mono)" id="si-chip">--</span></div><div>Rev <span style="float:right;color:var(--w);font-family:var(--mono)" id="si-rev">--</span></div><div>CPU <span style="float:right;color:var(--w);font-family:var(--mono)" id="si-cpu">--</span></div><div>Flash <span style="float:right;color:var(--w);font-family:var(--mono)" id="si-flash">--</span></div><div>SDK <span style="float:right;color:var(--w);font-family:var(--mono)" id="si-sdk">--</span></div><div>Firmware <span style="float:right;color:var(--g);font-weight:700;font-family:var(--mono)" id="si-fw">V1.0.0</span></div></div></div>
  <button class="btn btn-d btn-f" onclick="confirmReboot()">&#x1F504; Reboot Device</button>
  <div class="card about"><div class="about-logo">&#x1F431;</div><div class="about-name">NEOCAT V1</div><div class="about-powered">Powered by Glyph S3</div><div class="about-legal">ESP32-S3 Security Toolkit<br>For authorised research only.</div></div>
</section>

</main>

<!-- â”€â”€ BOTTOM NAV â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ -->
<nav id="bottom-nav">
  <div class="nav-indicator" id="nav-ind"></div>
  <button class="nav-btn active" data-tab="dashboard"><svg viewBox="0 0 24 24"><path d="M3 9l9-7 9 7v11a2 2 0 01-2 2H5a2 2 0 01-2-2z"/><polyline points="9 22 9 12 15 12 15 22"/></svg>HOME</button>
  <button class="nav-btn" data-tab="wifi"><svg viewBox="0 0 24 24"><path d="M5 12.55a11 11 0 0114.08 0"/><path d="M1.42 9a16 16 0 0121.16 0"/><path d="M8.53 16.11a6 6 0 016.95 0"/><circle cx="12" cy="20" r="1"/></svg>WIFI</button>
  <button class="nav-btn" data-tab="ble"><svg viewBox="0 0 24 24"><polyline points="6.5 6.5 17.5 17.5 12 23 12 1 17.5 6.5 6.5 17.5"/></svg>BLE</button>
  <button class="nav-btn" data-tab="monitor"><svg viewBox="0 0 24 24"><rect x="2" y="3" width="20" height="14" rx="2"/><line x1="8" y1="21" x2="16" y2="21"/><line x1="12" y1="17" x2="12" y2="21"/></svg>LOG</button>
  <button class="nav-btn" data-tab="settings"><svg viewBox="0 0 24 24"><circle cx="12" cy="12" r="3"/><path d="M19.4 15a1.65 1.65 0 00.33 1.82l.06.06a2 2 0 01-2.83 2.83l-.06-.06a1.65 1.65 0 00-1.82-.33 1.65 1.65 0 00-1 1.51V21a2 2 0 01-4 0v-.09A1.65 1.65 0 009 19.4a1.65 1.65 0 00-1.82.33l-.06.06a2 2 0 01-2.83-2.83l.06-.06A1.65 1.65 0 004.68 15a1.65 1.65 0 00-1.51-1H3a2 2 0 010-4h.09A1.65 1.65 0 004.6 9a1.65 1.65 0 00-.33-1.82l-.06-.06a2 2 0 012.83-2.83l.06.06A1.65 1.65 0 009 4.68a1.65 1.65 0 001-1.51V3a2 2 0 014 0v.09a1.65 1.65 0 001 1.51 1.65 1.65 0 001.82-.33l.06-.06a2 2 0 012.83 2.83l-.06.06A1.65 1.65 0 0019.4 9a1.65 1.65 0 001.51 1H21a2 2 0 010 4h-.09a1.65 1.65 0 00-1.51 1z"/></svg>SETTINGS</button>
</nav>

<div id="toast-container"></div>

<div class="mdl" id="modal"><div class="mdl-box"><h3 id="modal-title">Confirm</h3><p id="modal-msg">Are you sure?</p><div class="mdl-acts"><button class="btn btn-g btn-s" onclick="closeModal()">Cancel</button><button class="btn btn-d btn-s" id="modal-confirm">Confirm</button></div></div></div>

</div><!-- /app -->

<script>
(function(){
"use strict";

/* â”€â”€ Boot particles â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ */
(function(){
  const c=document.getElementById('b-particles');
  for(let i=0;i<40;i++){
    const p=document.createElement('div');
    p.className='b-p';
    p.style.cssText='left:'+Math.random()*100+'%;height:'+(8+Math.random()*20)+'px;animation-duration:'+(1.5+Math.random()*2.5)+'s;animation-delay:'+Math.random()*3+'s;';
    c.appendChild(p);
  }
})();

/* â”€â”€ Boot sequence â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ */
const bootLines=[
  '[SYS] NeoCAT V1 firmware loading...',
  '[CPU] ESP32-S3 dual-core @ 240MHz',
  '[MEM] Heap allocation OK',
  '[NET] WiFi promiscuous mode init...',
  '[BLE] Bluetooth LE stack ready',
  '[HTTP] Binding port 80...',
  '[WS] WebSocket handler registered',
  '[LED] RGB controller online',
  '[PWA] Service manifest loaded',
  '[OK] All systems nominal â€” Glyph S3'
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
setTimeout(bTick,250);

/* â”€â”€ Ripple effect â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ */
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

/* â”€â”€ State â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ */
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

/* â”€â”€ Helpers â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ */
function fmtUp(s){const h=Math.floor(s/3600),m=Math.floor((s%3600)/60),sc=s%60;return String(h).padStart(2,'0')+':'+String(m).padStart(2,'0')+':'+String(sc).padStart(2,'0')}
function fmtB(b){if(b<1024)return b+' B';if(b<1048576)return(b/1024).toFixed(1)+' KB';return(b/1048576).toFixed(1)+' MB'}
function fmtN(n){return n.toLocaleString()}
function rssiQ(r){if(r>-50)return{q:'excellent',b:5};if(r>-60)return{q:'good',b:4};if(r>-70)return{q:'fair',b:3};if(r>-80)return{q:'weak',b:2};return{q:'poor',b:1}}
function sigHTML(rssi){const{q,b:bars}=rssiQ(rssi);let h='<div class="sig '+q+'">';for(let i=1;i<=5;i++)h+='<b class="'+(i<=bars?'on':'')+'"></b>';return h+'</div>'}
function ts(){return new Date().toLocaleTimeString()}

/* â”€â”€ Toast â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ */
function toast(msg,type='info',dur=3000){
  const c=$('#toast-container'),t=document.createElement('div');
  t.className='toast '+type;t.textContent=msg;c.appendChild(t);
  setTimeout(()=>{t.classList.add('hide');setTimeout(()=>t.remove(),300)},dur);
}

/* â”€â”€ Modal â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ */
let mCb=null;
function showModal(title,msg,cb){$('#modal-title').textContent=title;$('#modal-msg').textContent=msg;mCb=cb;$('#modal').classList.add('active')}
function closeModal(){$('#modal').classList.remove('active');mCb=null}
$('#modal-confirm').onclick=()=>{if(mCb)mCb();closeModal()};

/* â”€â”€ Nav with sliding indicator â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ */
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

/* â”€â”€ LED â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ */
$('#led-mode').onchange=function(){$('#custom-color-row').style.display=this.value==='custom'?'flex':'none'};

/* â”€â”€ API â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ */
async function api(p,o={}){try{const r=await fetch(p,o);return await r.json()}catch(e){toast('Request failed','error');return null}}
function post(p,b){return api(p,{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify(b)})}

/* â”€â”€ Terminal â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ */
function addLog(msg,cls=''){const el=document.createElement('div');el.className='line'+(cls?' '+cls:'');el.innerHTML='<span class="ts">['+ts()+']</span> '+msg;terminal.appendChild(el);logN++;if(logN>MAX_LOG){terminal.removeChild(terminal.firstChild);logN--}$('#log-count').textContent=logN;scrollTerm()}
function scrollTerm(){terminal.scrollTop=terminal.scrollHeight}

/* â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
   WIFI
   â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â• */
async function doWifiScan(){
  const btn=$('#btn-wifi-scan');btn.disabled=true;btn.innerHTML='<span class="spinner"></span> Scanning...';btn.classList.add('scanning');
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
      +'<div class="li-icon">'+(enc?'&#x1F512;':'&#x1F513;')+'</div>'
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
async function startBeacon(){const s=$('#beacon-ssids').value.trim();if(!s){toast('Enter SSIDs','warning');return}await post('/api/wifi/beacon',{ssids:s});toast('Beacon flood started','info');addLog('Beacon: '+s.split('\n').length+' SSIDs','warn')}
async function startRickroll(){await post('/api/wifi/rickroll',{});toast('Rickroll started','info');addLog('Rickroll flood','warn')}
async function startProbe(){await post('/api/wifi/probe',{});toast('Probe flood','info');addLog('Probe flood','warn')}
async function stopWifiAttack(){await api('/api/wifi/stop',{method:'POST'});toast('Stopped','success');addLog('WiFi stopped','info')}
function quickBeacon(){navBtns.forEach(b=>b.classList.remove('active'));navBtns[1].classList.add('active');updateIndicator(navBtns[1]);$$('.tab-content').forEach(s=>s.classList.remove('active'));$('#tab-wifi').classList.add('active');$('#beacon-ssids').focus()}

/* â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
   BLE
   â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â• */
async function doBleScan(){
  const btn=$('#btn-ble-scan');btn.disabled=true;btn.innerHTML='<span class="spinner"></span> Scanning...';btn.classList.add('scanning');
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
  list.innerHTML=bleDevs.map((d,i)=>'<div class="li" style="animation:slideUp .3s '+(i*.04)+'s both"><div class="li-icon">&#x1F4F6;</div><div class="li-body"><div class="li-name">'+(d.name||'Unknown')+'</div><div class="li-sub">'+d.mac+'</div></div><div class="li-right">'+sigHTML(d.rssi)+'<div style="font-size:.55rem;color:var(--dim2);margin-top:2px;font-family:var(--mono)">'+d.rssi+'</div></div></div>').join('');
}
async function startBleSpam(t){await post('/api/ble/spam',{type:t});toast('BLE '+t+' spam started','info');addLog('BLE spam: '+t,'warn')}
async function stopBleSpam(){await api('/api/ble/stop',{method:'POST'});toast('BLE stopped','success');addLog('BLE stopped','info')}

/* â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
   SETTINGS
   â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â• */
async function setLedMode(m){await post('/api/led/mode',{mode:m});toast('LED: '+m,'success')}
async function setLedColor(hex){const r=parseInt(hex.slice(1,3),16),g=parseInt(hex.slice(3,5),16),b=parseInt(hex.slice(5,7),16);$('#color-preview').style.background=hex;$('#color-hex').textContent=hex;await post('/api/led/color',{r,g,b})}
function confirmReboot(){showModal('Reboot','Restart the ESP32?',async()=>{toast('Rebooting...','warning');addLog('Reboot','err');await post('/api/system/reboot',{})})}

window.doWifiScan=doWifiScan;window.doBleScan=doBleScan;window.quickBeacon=quickBeacon;
window.startDeauth=startDeauth;window.startBeacon=startBeacon;window.startRickroll=startRickroll;
window.startProbe=startProbe;window.stopWifiAttack=stopWifiAttack;
window.startBleSpam=startBleSpam;window.stopBleSpam=stopBleSpam;
window.setLedMode=setLedMode;window.setLedColor=setLedColor;window.confirmReboot=confirmReboot;
window.closeModal=closeModal;

/* â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
   WEBSOCKET
   â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â• */
function connectWS(){
  ws=new WebSocket('ws://'+location.host+'/ws');
  ws.onopen=()=>{wsOn=true;rDelay=1000;connDot.classList.add('on');connLabel.textContent='ONLINE';addLog('Connected','info')};
  ws.onclose=()=>{wsOn=false;connDot.classList.remove('on');connLabel.textContent='OFFLINE';addLog('Disconnected â€” retry in '+(rDelay/1000)+'s','err');setTimeout(connectWS,rDelay);rDelay=Math.min(rDelay*2,MAX_R)};
  ws.onerror=()=>{ws.close()};
  ws.onmessage=ev=>{try{handleWS(JSON.parse(ev.data))}catch(e){}};
}
function handleWS(d){
  if(d.type==='system_info'){
    $('#si-chip').textContent=d.chipModel||'--';$('#si-rev').textContent=d.chipRevision??'--';
    $('#si-cpu').textContent=(d.cpuFreqMHz||'--')+' MHz';$('#si-flash').textContent=fmtB(d.flashSize||0);
    $('#si-sdk').textContent=d.sdkVersion||'--';$('#si-fw').textContent=d.firmwareVersion||'--';
    $('#hero-status').textContent=d.chipModel+' @ '+d.cpuFreqMHz+' MHz â€” '+fmtB(d.flashSize)+' flash';return;
  }
  if(d.type==='status'){
    $('#s-uptime').textContent=fmtUp(d.uptime||0);$('#s-heap').textContent=fmtB(d.freeHeap||0);
    wPkts=d.wifiPackets||0;bPkts=d.blePackets||0;
    $('#s-pkts').textContent=fmtN(wPkts+bPkts);$('#s-devs').textContent=fmtN((d.wifiNetworks||0)+(d.bleDevices||0));
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

/* â”€â”€ Init â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ */
renderChart();connectWS();
setInterval(()=>{if(ws&&ws.readyState===WebSocket.OPEN)ws.send('{"cmd":"ping"}')},15000);
(async()=>{const i=await api('/api/system/info');if(i)handleWS({type:'system_info',...i})})();
addLog('NeoCAT V1 Dashboard loaded','info');
addLog('Powered by Glyph S3','info');

/* â”€â”€ PWA install prompt â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€ */
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


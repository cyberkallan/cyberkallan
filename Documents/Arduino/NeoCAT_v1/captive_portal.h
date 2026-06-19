#pragma once
/**
 * @file captive_portal.h
 * @brief NeoCAT V1 — Clone AP Captive Portal Templates (PROGMEM)
 *
 * Single self-contained HTML page that renders 3 different themed
 * captive portal experiences based on config fetched from /api/clone/info.
 * Templates: 0=Prank, 1=Hacker Mode, 2=Funny
 *
 * DISCLAIMER: For authorized security testing and education only.
 */

#include <pgmspace.h>

const char CAPTIVE_PORTAL_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no,viewport-fit=cover">
<meta name="theme-color" content="#050505">
<title>Welcome</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{min-height:100vh;min-height:100dvh;overflow-x:hidden}
body{font-family:system-ui,-apple-system,'Segoe UI',sans-serif;display:flex;align-items:center;justify-content:center;transition:background .5s}

/* ── Loading ── */
#loader{position:fixed;inset:0;background:#050505;display:flex;align-items:center;justify-content:center;z-index:999;transition:opacity .5s}
#loader .ring{width:40px;height:40px;border:3px solid rgba(0,255,65,.2);border-top-color:#00ff41;border-radius:50%;animation:spin .8s linear infinite}
@keyframes spin{to{transform:rotate(360deg)}}

/* ── Confetti ── */
.confetti{position:fixed;width:8px;height:8px;top:-10px;border-radius:50%;z-index:0;pointer-events:none}

/* ═══════════════════════════════════════════
   TEMPLATE 0: PRANK
   ═══════════════════════════════════════════ */
.t0{background:linear-gradient(135deg,#667eea 0%,#764ba2 50%,#f093fb 100%);color:#fff;text-align:center;padding:40px 20px}
.t0 .wrap{position:relative;z-index:1;max-width:400px;margin:0 auto}
.t0 .icon{font-size:80px;animation:bounce 1s ease infinite;filter:drop-shadow(0 8px 20px rgba(0,0,0,.3))}
.t0 h1{font-size:2rem;font-weight:900;margin:24px 0 12px;text-shadow:0 4px 20px rgba(0,0,0,.3);letter-spacing:2px}
.t0 .sub{font-size:1.1rem;opacity:.9;margin-bottom:30px;font-weight:300}
.t0 .glass{background:rgba(255,255,255,.12);backdrop-filter:blur(16px);-webkit-backdrop-filter:blur(16px);border:1px solid rgba(255,255,255,.2);border-radius:24px;padding:30px 24px}
.t0 .note{opacity:.5;font-size:.75rem;margin-top:24px;line-height:1.6}
.t0 .tag{display:inline-block;background:rgba(255,255,255,.15);border-radius:20px;padding:6px 16px;font-size:.6rem;letter-spacing:2px;text-transform:uppercase;margin-top:20px}
@keyframes bounce{0%,100%{transform:translateY(0) scale(1)}50%{transform:translateY(-18px) scale(1.05)}}
@keyframes fall{0%{transform:translateY(0) rotate(0)}100%{transform:translateY(110vh) rotate(720deg)}}

/* ═══════════════════════════════════════════
   TEMPLATE 1: HACKER MODE
   ═══════════════════════════════════════════ */
.t1{background:#000;color:#00ff41;font-family:'Courier New','SF Mono',monospace;padding:0;overflow:hidden}
.t1 canvas{position:fixed;top:0;left:0;width:100%;height:100%;z-index:0}
.t1 .wrap{position:relative;z-index:1;max-width:500px;margin:0 auto;text-align:center;padding:40px 20px}
.t1 .skull{font-size:60px;filter:drop-shadow(0 0 20px rgba(0,255,65,.6));margin-bottom:16px;animation:pulse 2s ease infinite}
.t1 h1{font-size:1.4rem;font-weight:700;letter-spacing:6px;text-transform:uppercase;text-shadow:0 0 20px #00ff41,0 0 40px rgba(0,255,65,.3);animation:glitch 3s infinite;margin-bottom:8px}
.t1 .target{font-size:.7rem;color:#00dd38;letter-spacing:3px;margin-bottom:24px;opacity:.7}
.t1 .term{text-align:left;background:rgba(0,20,0,.85);border:1px solid rgba(0,255,65,.3);border-radius:12px;padding:18px;margin:0 auto;max-width:420px;font-size:.72rem;line-height:1.9;overflow:hidden}
.t1 .term .ln{opacity:0;animation:typeIn .3s ease forwards}
.t1 .term .ln.ok{color:#00ff41}.t1 .term .ln.warn{color:#ffb300}.t1 .term .ln.err{color:#ff3b3b}
.t1 .bar{height:6px;background:rgba(0,255,65,.15);border-radius:3px;margin:20px 0 10px;overflow:hidden}
.t1 .bar-fill{height:100%;width:0;background:linear-gradient(90deg,#00ff41,#00e5ff);border-radius:3px;transition:width 8s ease}
.t1 .pct{font-size:.6rem;letter-spacing:2px;opacity:.6}
.t1 .footer{font-size:.5rem;opacity:.3;margin-top:30px;letter-spacing:2px}
@keyframes glitch{0%,85%,100%{text-shadow:0 0 20px #00ff41}88%{text-shadow:-3px 0 #ff0040,3px 0 #00ff41;transform:translateX(-2px)}92%{text-shadow:3px 0 #0040ff,-3px 0 #ff0040;transform:translateX(2px)}95%{text-shadow:0 0 20px #00ff41;transform:translateX(0)}}
@keyframes typeIn{to{opacity:1}}
@keyframes pulse{0%,100%{opacity:1}50%{opacity:.6}}
@keyframes scanline{0%{top:-100%}100%{top:200%}}

/* ═══════════════════════════════════════════
   TEMPLATE 2: FUNNY
   ═══════════════════════════════════════════ */
.t2{background:linear-gradient(135deg,#f5576c 0%,#ff9a9e 25%,#fad0c4 50%,#a18cd1 75%,#fbc2eb 100%);background-size:400% 400%;animation:gradBg 6s ease infinite;color:#fff;text-align:center;padding:40px 20px}
.t2 .wrap{position:relative;z-index:1;max-width:380px;margin:0 auto}
.t2 .icon{font-size:90px;animation:wiggle 2s ease infinite;filter:drop-shadow(0 8px 30px rgba(0,0,0,.2))}
.t2 h1{font-size:1.8rem;font-weight:900;margin:20px 0;text-shadow:0 4px 20px rgba(0,0,0,.25);letter-spacing:1px}
.t2 .lyrics{background:rgba(255,255,255,.18);backdrop-filter:blur(16px);-webkit-backdrop-filter:blur(16px);border:1px solid rgba(255,255,255,.25);border-radius:20px;padding:24px 20px;margin:20px 0;font-size:1rem;line-height:2;font-weight:500}
.t2 .lyrics span{display:block;animation:fadeSlide .5s ease both}
.t2 .qr{font-size:.65rem;opacity:.5;margin-top:24px}
.t2 .badge{display:inline-block;background:rgba(0,0,0,.15);border-radius:20px;padding:6px 16px;font-size:.55rem;letter-spacing:2px;text-transform:uppercase;margin-top:20px}
@keyframes wiggle{0%,100%{transform:rotate(0)}25%{transform:rotate(12deg)}50%{transform:rotate(-12deg)}75%{transform:rotate(8deg)}}
@keyframes gradBg{0%{background-position:0% 50%}50%{background-position:100% 50%}100%{background-position:0% 50%}}
@keyframes fadeSlide{from{opacity:0;transform:translateY(10px)}to{opacity:1;transform:translateY(0)}}
</style>
</head>
<body>

<div id="loader"><div class="ring"></div></div>
<div id="app" style="width:100%;min-height:100vh;min-height:100dvh;display:flex;align-items:center;justify-content:center"></div>

<script>
(function(){
  var cfg={template:0,title:'Welcome',ssid:''};

  fetch('/api/clone/info')
    .then(function(r){return r.json()})
    .then(function(d){cfg=d;boot()})
    .catch(function(){boot()});

  function boot(){
    setTimeout(function(){
      var ld=document.getElementById('loader');
      ld.style.opacity='0';
      setTimeout(function(){ld.style.display='none'},500);
      render(cfg.template||0, cfg.title||'Welcome', cfg.ssid||'');
    },800);
  }

  function esc(s){return String(s).replace(/</g,'&lt;').replace(/>/g,'&gt;').replace(/"/g,'&quot;')}

  function render(tpl, title, ssid){
    var app=document.getElementById('app');
    var body=document.body;

    /* ── PRANK ── */
    if(tpl===0){
      body.className='t0';
      app.innerHTML=
        '<div class="wrap"><div class="glass">'+
        '<div class="icon">\uD83D\uDE02</div>'+
        '<h1>YOU\'VE BEEN<br>PRANKED!</h1>'+
        '<div class="sub">'+esc(title)+'</div>'+
        '<div class="note">Someone on this network is having a little fun.<br>Don\'t worry — this is completely harmless!</div>'+
        '<div class="tag">Powered by NeoCAT</div>'+
        '</div></div>';
      // Confetti
      for(var i=0;i<60;i++){
        var c=document.createElement('div');
        c.className='confetti';
        var hue=Math.floor(Math.random()*360);
        var left=Math.random()*100;
        var dur=2+Math.random()*3;
        var del=Math.random()*4;
        var sz=5+Math.random()*6;
        c.style.cssText='left:'+left+'%;width:'+sz+'px;height:'+sz+'px;background:hsl('+hue+',90%,65%);animation:fall '+dur+'s linear '+del+'s infinite';
        body.appendChild(c);
      }
    }

    /* ── HACKER MODE ── */
    else if(tpl===1){
      body.className='t1';
      app.innerHTML=
        '<canvas id="mx"></canvas>'+
        '<div class="wrap">'+
        '<div class="skull">\u2620\uFE0F</div>'+
        '<h1>SYSTEM COMPROMISED</h1>'+
        '<div class="target">[ TARGET: '+esc(title||ssid)+' ]</div>'+
        '<div class="term" id="hterm"></div>'+
        '<div class="bar"><div class="bar-fill" id="barFill"></div></div>'+
        '<div class="pct" id="pctLabel">INITIALIZING...</div>'+
        '<div class="footer">NeoCAT V1 \u2022 AUTHORIZED TESTING ONLY</div>'+
        '</div>';

      // Matrix rain
      var cv=document.getElementById('mx'), ct=cv.getContext('2d');
      cv.width=window.innerWidth; cv.height=window.innerHeight;
      var cols=Math.floor(cv.width/14), drops=[];
      for(var j=0;j<cols;j++) drops[j]=Math.random()*cv.height/14;
      setInterval(function(){
        ct.fillStyle='rgba(0,0,0,.06)';
        ct.fillRect(0,0,cv.width,cv.height);
        ct.fillStyle='#00ff41';
        ct.font='14px monospace';
        for(var k=0;k<drops.length;k++){
          var ch=String.fromCharCode(0x30A0+Math.floor(Math.random()*96));
          ct.fillText(ch,k*14,drops[k]*14);
          if(drops[k]*14>cv.height && Math.random()>.975) drops[k]=0;
          drops[k]++;
        }
      },33);

      // Terminal lines
      var lines=[
        {t:'> Initializing exploit framework...',c:'ok'},
        {t:'> Scanning target interfaces...',c:'ok'},
        {t:'> Target: '+esc(title||ssid),c:'warn'},
        {t:'> Bypassing firewall rules... [OK]',c:'ok'},
        {t:'> Injecting payload v3.7.1... [OK]',c:'ok'},
        {t:'> Escalating privileges...',c:'warn'},
        {t:'> Root access obtained!',c:'err'},
        {t:'> Establishing persistence...',c:'ok'},
        {t:'> Exfiltrating data... [OK]',c:'ok'},
        {t:'> \u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2588 100%',c:'ok'},
        {t:'> FULL SYSTEM ACCESS ACHIEVED',c:'err'},
        {t:'',c:'ok'},
        {t:'> Just kidding. You got NeoCAT\'d \uD83D\uDE3C',c:'warn'}
      ];
      var hterm=document.getElementById('hterm');
      lines.forEach(function(l,idx){
        setTimeout(function(){
          var div=document.createElement('div');
          div.className='ln '+l.c;
          div.style.animationDelay=(idx*0.05)+'s';
          div.textContent=l.t;
          hterm.appendChild(div);
          hterm.scrollTop=hterm.scrollHeight;
        },idx*600);
      });
      // Progress bar
      setTimeout(function(){
        document.getElementById('barFill').style.width='100%';
      },200);
      setTimeout(function(){
        document.getElementById('pctLabel').textContent='OPERATION COMPLETE';
      },8500);
    }

    /* ── FUNNY ── */
    else if(tpl===2){
      body.className='t2';
      app.innerHTML=
        '<div class="wrap">'+
        '<div class="icon">\uD83E\uDD23</div>'+
        '<h1>'+esc(title)+'</h1>'+
        '<div class="lyrics">'+
        '<span style="animation-delay:.2s">\uD83C\uDFB5 Never gonna give you up</span>'+
        '<span style="animation-delay:.4s">\uD83C\uDFB5 Never gonna let you down</span>'+
        '<span style="animation-delay:.6s">\uD83C\uDFB5 Never gonna run around</span>'+
        '<span style="animation-delay:.8s">\uD83C\uDFB5 And desert you</span>'+
        '<span style="animation-delay:1s">\uD83C\uDFB5 Never gonna make you cry</span>'+
        '<span style="animation-delay:1.2s">\uD83C\uDFB5 Never gonna say goodbye</span>'+
        '</div>'+
        '<p style="font-size:.9rem;opacity:.85;margin:16px 0">You just got Rick Rolled via WiFi! \uD83C\uDFB6</p>'+
        '<div class="badge">NeoCAT V1 \u2022 Authorized Testing</div>'+
        '</div>';
    }
  }
})();
</script>
</body>
</html>
)rawliteral";

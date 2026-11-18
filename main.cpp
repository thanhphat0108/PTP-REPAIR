#include <WiFi.h>
#include <WiFiManager.h>      // tzapu/WiFiManager
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <time.h>

// Cloud qua ntfy.sh (HTTPS cong 443 - Viettel KHONG chan nhu MQTT 1883)
#define DECK_URL "https://ntfy.sh/ptp-deck-9f3k7q2x8m"             // nut app -> PC
#define PR_URL   "https://ntfy.sh/ptp-printer-9f3k7q2x8m/raw?poll=1&since=180s" // may in -> man
#include <TFT_eSPI.h>
#include <Preferences.h>
#include "F_TAB.h"   // Poppins Medium 12px
#include "F_SML.h"   // Poppins Medium 15px
#include "F_MED.h"   // Poppins Medium 18px
#include "F_BIG.h"   // Poppins Medium 46px (clock)
#include "LOGO.h"    // PTP REPAIR 64x64
#include "DECKI.h"   // 6 icon app 36x36

TFT_eSPI tft = TFT_eSPI();
Preferences prefs;
WiFiManager wm;

#define C565(r,g,b) (uint16_t)((((r)&0xF8)<<8)|(((g)&0xFC)<<3)|((b)>>3))
static const uint16_t BG=C565(11,13,17), CARD=C565(20,24,33), KEYAPP=C565(23,31,43),
  ORANGE=C565(242,162,74), MUTED=C565(139,147,166), DATEC=C565(195,201,214),
  GRN=C565(61,214,140), BLU=C565(74,168,255), WHITE=C565(232,234,240),
  RED=C565(224,70,70), DARK=C565(26,18,6), BORDER=C565(30,34,43), KEYTX=C565(178,186,202);

#define BUZZER 32
int page=0, curFont=-1; uint32_t lastSec=0;
bool ntpOn=true, wifiOK=false;
char eventTxt[48]="Chua dat su kien";
char pcIP[20]="";
char httpStatus[28]="San sang";
int pNoz=0,pNozT=0,pBed=0,pBedT=0,pPct=0,pLay=0,pTot=0,pRem=0;
char pState[16]="--"; bool pOnline=false; uint32_t pLast=0;
volatile bool pDirty=false; uint32_t lastPrintDraw=0;
uint32_t lastPoll=0; bool printerTried=false;
const char* DECK_LBL[6]={"Bambu","Facebook","EZVIZ","Zalo","Chrome","YouTube"};
const char* APPID[6]  ={"bambu","facebook","ezviz","zalo","chrome","youtube"};
const char* DOW[7]={"Chu Nhat","Thu Hai","Thu Ba","Thu Tu","Thu Nam","Thu Sau","Thu Bay"};
const char* DOWS[7]={"T2","T3","T4","T5","T6","T7","CN"};

void beep(int ms=35){ digitalWrite(BUZZER,LOW); delay(ms); digitalWrite(BUZZER,HIGH); }
void useFont(int id){ if(id==curFont)return; curFont=id;
  if(id==0)tft.loadFont(F_SML); else if(id==1)tft.loadFont(F_MED);
  else if(id==2)tft.loadFont(F_BIG); else tft.loadFont(F_TAB); }
int tw(int id,const char*s){ useFont(id); return tft.textWidth(s); }
void txt(int id,int x,int y,const char*s,uint16_t fg,uint16_t bg,uint8_t dat){
  useFont(id); tft.setTextDatum(dat); tft.setTextColor(fg,bg); tft.drawString(s,x,y); }
void rr(int x,int y,int w,int h,uint16_t c,int r=8){ tft.fillRoundRect(x,y,w,h,r,c); }
bool timeSynced(){ return time(nullptr) > 1600000000; }

void icHome(int cx,int cy,uint16_t c){ tft.fillTriangle(cx-8,cy-1,cx,cy-9,cx+8,cy-1,c);
  tft.fillRect(cx-6,cy-1,12,7,c); tft.fillRect(cx-2,cy+1,4,5,BG); }
void icGrid(int cx,int cy,uint16_t c){ for(int i=0;i<2;i++)for(int j=0;j<2;j++)
  tft.fillRoundRect(cx-7+j*8,cy-7+i*8,6,6,1,c); }
void icPrint(int cx,int cy,uint16_t c){ tft.fillRect(cx-4,cy-8,8,4,c);
  tft.fillRoundRect(cx-8,cy-4,16,8,2,c); tft.fillRect(cx-5,cy+1,10,6,c); tft.fillRect(cx-3,cy+3,6,2,BG); }
void icGear(int cx,int cy,uint16_t c){ tft.fillCircle(cx,cy,7,c);
  for(int a=0;a<360;a+=60){ float r=a*3.14159/180; tft.fillRect(cx+cos(r)*7-1,cy+sin(r)*7-1,3,3,c); }
  tft.fillCircle(cx,cy,3,BG); }

void tabBar(){
  tft.fillRect(0,195,320,45,BG); tft.drawFastHLine(0,196,320,BORDER);
  const char* L[]={"Trang chu","Deck","May in","Cai dat"};
  for(int i=0;i<4;i++){ int x=6+i*78; bool a=(i==page);
    rr(x,199,74,38,a?ORANGE:CARD,9);
    uint16_t ic=a?DARK:MUTED, bg=a?ORANGE:CARD; int cx=x+37;
    if(i==0)icHome(cx,209,ic); else if(i==1)icGrid(cx,209,ic);
    else if(i==2)icPrint(cx,209,ic); else icGear(cx,209,ic);
    txt(3,cx,227,L[i],a?DARK:MUTED,bg,MC_DATUM); }
}
void drawLogo(int x,int y){ tft.pushImage(x,y,LOGO_W,LOGO_H,LOGO); }

void homeClock(){
  tft.fillRect(8,26,232,56,BG);
  struct tm t; char b[8]="--:--", s[4]="";
  if(timeSynced() && getLocalTime(&t,0)){ sprintf(b,"%02d:%02d",t.tm_hour,t.tm_min); sprintf(s,"%02d",t.tm_sec); }
  txt(2,10,28,b,WHITE,BG,TL_DATUM); int w=tw(2,b);
  txt(1,16+w,36,s,ORANGE,BG,TL_DATUM);
}
void pageHome(){
  tft.fillRect(0,0,320,196,BG);
  tft.fillCircle(13,17,4, wifiOK?GRN:RED);
  char st[40];
  if(wifiOK) snprintf(st,sizeof(st),"WiFi: %s", WiFi.SSID().c_str());
  else strcpy(st,"Chua ket noi WiFi");
  txt(0,24,10,st,MUTED,BG,TL_DATUM);
  drawLogo(248,38); homeClock();
  struct tm t; bool ok = timeSynced() && getLocalTime(&t,0);
  char db[48];
  if(ok) snprintf(db,sizeof(db),"%s, %d Thang %d, %d",DOW[t.tm_wday],t.tm_mday,t.tm_mon+1,t.tm_year+1900);
  else strcpy(db,"Chua dong bo thoi gian");
  txt(0,10,92,db,DATEC,BG,TL_DATUM);
  if(ok){
    time_t nowt=time(nullptr); int wd=t.tm_wday; int dsm=(wd==0)?6:(wd-1);
    time_t mon=nowt-(time_t)dsm*86400L;
    for(int i=0;i<7;i++){ struct tm dt; time_t d=mon+(time_t)i*86400L; localtime_r(&d,&dt);
      bool today=(dt.tm_mday==t.tm_mday && dt.tm_mon==t.tm_mon);
      int x=8+i*43; rr(x,114,40,40,today?ORANGE:CARD,6);
      txt(0,x+20,117,DOWS[i], today?DARK:MUTED, today?ORANGE:CARD, TC_DATUM);
      char n[4]; sprintf(n,"%d",dt.tm_mday);
      txt(1,x+20,130,n, today?DARK:WHITE, today?ORANGE:CARD, TC_DATUM); }
  }
  tft.fillRect(10,164,6,8,ORANGE); tft.fillRect(9,163,2,2,ORANGE);
  txt(0,22,160,"Lich: ",MUTED,BG,TL_DATUM);
  txt(0,22+tw(0,"Lich: "),160,eventTxt,ORANGE,BG,TL_DATUM);
  tabBar();
}

// ---- deck ----
void deckRect(int i,int&x,int&y,int&w,int&h){ x=6+(i%3)*103; y=52+(i/3)*58; w=98; h=54; }
void keyApp(int i){
  int x,y,w,h; deckRect(i,x,y,w,h);
  rr(x,y,w,h,KEYAPP,9);
  tft.pushImage(x+w/2-DECKW/2, y+3, DECKW,DECKH, DECKI[i]);
  txt(3,x+w/2,y+45,DECK_LBL[i],KEYTX,KEYAPP,MC_DATUM);
}
void keyFlash(int i){
  int x,y,w,h; deckRect(i,x,y,w,h);
  tft.drawRoundRect(x,y,w,h,9,ORANGE); tft.drawRoundRect(x+1,y+1,w-2,h-2,8,ORANGE);
  delay(130);
  tft.drawRoundRect(x,y,w,h,9,KEYAPP); tft.drawRoundRect(x+1,y+1,w-2,h-2,8,KEYAPP);
}
void drawDeckStatus(){
  tft.fillRect(150,30,166,18,BG);
  txt(0,312,32,httpStatus,MUTED,BG,TR_DATUM);
}
void pageDeck(){
  tft.fillRect(0,0,320,196,BG);
  tft.fillCircle(13,17,4, wifiOK?GRN:RED);
  txt(1,24,8,"Stream Deck",WHITE,BG,TL_DATUM);
  char mip[26]; snprintf(mip,26,"Man: %s", wifiOK?WiFi.localIP().toString().c_str():"--");
  txt(0,312,12,mip,MUTED,BG,TR_DATUM);
  txt(0,10,32,"Che do: Cloud (Internet)", GRN, BG, TL_DATUM);
  txt(0,312,32,httpStatus,MUTED,BG,TR_DATUM);
  for(int i=0;i<6;i++) keyApp(i);
  tabBar();
}
// Gui lenh mo app -> ntfy (HTTPS POST, mot lan khi bam)
void openApp(const char* id){
  if(!wifiOK){ strcpy(httpStatus,"WiFi chua ket noi"); drawDeckStatus(); return; }
  strcpy(httpStatus,"Dang gui..."); drawDeckStatus();
  WiFiClientSecure cs; cs.setInsecure(); cs.setHandshakeTimeout(6);
  HTTPClient h; h.setConnectTimeout(6000); h.setTimeout(7000);
  bool ok=false;
  if(h.begin(cs, DECK_URL)){
    h.addHeader("Content-Type","text/plain");
    int code=h.POST(String(id));
    ok=(code>0 && code<400);
    h.end();
  }
  strcpy(httpStatus, ok?"Da gui - mo app":"Loi gui, thu lai");
  drawDeckStatus();
}

// Nhan trang thai may in tu ntfy (HTTPS GET, poll khi dang o trang May in)
void applyStatus(const char* buf){
  char st[16]="--"; int a[8]={0,0,0,0,0,0,0,0};
  int got=sscanf(buf,"%d|%d|%d|%d|%d|%d|%d|%d|%15s",&a[0],&a[1],&a[2],&a[3],&a[4],&a[5],&a[6],&a[7],st);
  if(got>=8){ pNoz=a[0];pNozT=a[1];pBed=a[2];pBedT=a[3];pPct=a[4];pLay=a[5];pTot=a[6];pRem=a[7];
    strncpy(pState,st,15);pState[15]=0; pOnline=true; pLast=millis(); pDirty=true; }
}
void pollPrinter(){
  if(!wifiOK) return;
  printerTried=true;
  WiFiClientSecure cs; cs.setInsecure(); cs.setHandshakeTimeout(5);
  HTTPClient h; h.setConnectTimeout(5000); h.setTimeout(6000);
  if(h.begin(cs, PR_URL)){
    int code=h.GET();
    if(code==200){
      String body=h.getString(); body.trim();
      if(body.length()>3){
        int nl=body.lastIndexOf('\n');
        String line=(nl>=0)?body.substring(nl+1):body; line.trim();
        if(line.length()>3) applyStatus(line.c_str());
      }
    }
    h.end();
  }
}

const char* stateVi(const char*s){
  if(!strcmp(s,"RUNNING"))return "Dang in";
  if(!strcmp(s,"PAUSE"))  return "Tam dung";
  if(!strcmp(s,"FINISH")) return "Hoan thanh";
  if(!strcmp(s,"IDLE"))   return "San sang";
  if(!strcmp(s,"PREPARE"))return "Chuan bi";
  if(!strcmp(s,"SLICING"))return "Dang cat lop";
  if(!strcmp(s,"FAILED")) return "Loi in";
  return s;
}
void pagePrint(){
  tft.fillRect(0,0,320,196,BG);
  bool on = pOnline && (millis()-pLast < 60000);
  tft.fillCircle(13,17,4, on?GRN:MUTED);
  txt(1,24,8,"May in 3D",WHITE,BG,TL_DATUM);
  const char* la[]={"Dau phun","Ban nhiet","Lop"};
  char val[16], suf[16];
  if(!on){
    txt(0,25,34, printerTried?"Chua ket noi may in":"Dang tai du lieu...",MUTED,BG,TL_DATUM);
    for(int i=0;i<3;i++){ int x=8+i*102; rr(x,54,96,48,CARD,8);
      txt(0,x+8,60,la[i],MUTED,CARD,TL_DATUM);
      txt(1,x+8,74,"--",KEYTX,CARD,TL_DATUM); }
    rr(8,110,304,14,CARD,7);
    txt(0,8,128,"0%",MUTED,BG,TL_DATUM);
  } else {
    txt(0,25,34,stateVi(pState),GRN,BG,TL_DATUM);
    for(int i=0;i<3;i++){ int x=8+i*102; rr(x,54,96,48,CARD,8);
      txt(0,x+8,60,la[i],MUTED,CARD,TL_DATUM);
      if(i==0){ sprintf(val,"%d",pNoz); sprintf(suf,"/%dC",pNozT); }
      else if(i==1){ sprintf(val,"%d",pBed); sprintf(suf,"/%dC",pBedT); }
      else { sprintf(val,"%d",pLay); sprintf(suf,"/%d",pTot); }
      txt(1,x+8,74,val,WHITE,CARD,TL_DATUM); int w=tw(1,val);
      txt(0,x+10+w,80,suf,MUTED,CARD,TL_DATUM); }
    rr(8,110,304,14,CARD,7);
    int pw=(int)(304.0*pPct/100.0); if(pw>3) rr(8,110,pw,14,ORANGE,7);
    sprintf(val,"%d%%",pPct); txt(0,8,128,val,MUTED,BG,TL_DATUM);
    sprintf(val,"con %d phut",pRem); txt(0,312,128,val,MUTED,BG,TR_DATUM);
  }
  rr(8,150,150,44,CARD,10); txt(0,83,172,"Tam dung",KEYTX,CARD,TC_DATUM);
  rr(164,150,148,44,CARD,10); txt(0,238,172,"Dung",KEYTX,CARD,TC_DATUM);
  tabBar();
}
void srow(int y,const char*k,const char*v,bool sw,bool on,uint16_t vc){
  txt(0,10,y,k,WHITE,BG,TL_DATUM);
  if(sw){ rr(280,y-1,34,18,on?GRN:CARD,9); tft.fillCircle(on?304:290,y+8,7,WHITE); }
  else txt(0,312,y,v,vc,BG,TR_DATUM);
  tft.drawFastHLine(8,y+25,304,BORDER);
}
void pageSettings(){
  tft.fillRect(0,0,320,196,BG);
  tft.fillCircle(13,17,4, wifiOK?GRN:RED);
  txt(1,24,8,"Cai dat",WHITE,BG,TL_DATUM);
  char rs[10], ps[22];
  if(wifiOK){ snprintf(rs,sizeof(rs),"%d dBm",WiFi.RSSI()); }
  else strcpy(rs,"--");
  snprintf(ps,sizeof(ps),"%s", strlen(pcIP)?pcIP:"chua dat");
  srow(44,"WiFi (cham de doi):",rs,false,false, wifiOK?GRN:ORANGE);
  srow(74, wifiOK?WiFi.SSID().c_str():"Chua ket noi","",false,false,MUTED);
  srow(104,"IP may tinh (PC):",ps,false,false, strlen(pcIP)?GRN:ORANGE);
  srow(134,"Dong bo gio (NTP)","",true, timeSynced(), MUTED);
  srow(164,"Khoi dong lai thiet bi","cham",false,false,ORANGE);
  tabBar();
}
void draw(){ if(page==0)pageHome(); else if(page==1)pageDeck();
  else if(page==2)pagePrint(); else pageSettings(); }

void saveCfg(){ prefs.begin("ptp",false); prefs.putString("event",eventTxt); prefs.putString("pcip",pcIP); prefs.end(); }

void splash(const char*l1,const char*l2,const char*l3){
  tft.fillScreen(BG); drawLogo(128,24);
  txt(1,160,104,l1,WHITE,BG,MC_DATUM);
  if(l2[0]) txt(1,160,130,l2,ORANGE,BG,MC_DATUM);
  if(l3[0]) txt(0,160,158,l3,MUTED,BG,MC_DATUM);
}
void apCb(WiFiManager* w){ splash("Mo WiFi tren dien thoai:","PTP-REPAIR-Setup","chon mang + nhap IP may tinh"); }

// ---- Ket noi WiFi NHANH: nho kenh + BSSID router lan truoc ----
void saveWifi(){
  prefs.begin("ptp",false);
  prefs.putString("wss",WiFi.SSID()); prefs.putString("wpw",WiFi.psk());
  prefs.putUChar("wch",WiFi.channel());
  uint8_t* b=WiFi.BSSID(); if(b) prefs.putBytes("wbssid",b,6);
  prefs.end();
}
void wifiDots(int n){ char b[5]=""; int k=n%4; for(int i=0;i<k;i++) b[i]='.';
  tft.fillRect(130,158,60,16,BG); txt(0,160,158,b,ORANGE,BG,TC_DATUM); }
bool waitConn(uint32_t ms){ uint32_t t0=millis(); int c=0;
  while(WiFi.status()!=WL_CONNECTED && millis()-t0<ms){ wifiDots(++c); delay(130); }
  return WiFi.status()==WL_CONNECTED; }
bool fastConnect(){
  prefs.begin("ptp",true);
  String ss=prefs.getString("wss",""), pw=prefs.getString("wpw","");
  uint8_t ch=prefs.getUChar("wch",0), bssid[6]; bool haveB=(prefs.getBytesLength("wbssid")==6);
  if(haveB) prefs.getBytes("wbssid",bssid,6);
  prefs.end();
  if(ss.length()==0) return false;
  WiFi.mode(WIFI_STA); WiFi.setSleep(false); WiFi.persistent(true);
  if(haveB && ch>0){ WiFi.begin(ss.c_str(),pw.c_str(),ch,bssid); if(waitConn(6000)) return true; WiFi.disconnect(); delay(50); }
  WiFi.begin(ss.c_str(),pw.c_str()); return waitConn(9000);
}

WiFiManagerParameter pEvent("event","Su kien (Lich)","",47);
WiFiManagerParameter pPC("pcip","IP may tinh (vd 192.168.1.10)","",19);

void pullParams(){
  if(strlen(pEvent.getValue())>0){ strncpy(eventTxt,pEvent.getValue(),47); eventTxt[47]=0; }
  if(strlen(pPC.getValue())>0){ strncpy(pcIP,pPC.getValue(),19); pcIP[19]=0; }
  saveCfg();
}
void reconfigWifi(){
  splash("Mo WiFi tren dien thoai:","PTP-REPAIR-Setup","chon mang + nhap IP may tinh");
  wm.setConfigPortalTimeout(180);
  if(wm.startConfigPortal("PTP-REPAIR-Setup")){ wifiOK=true; pullParams(); }
  else wifiOK=(WiFi.status()==WL_CONNECTED);
  if(wifiOK){ saveWifi(); configTime(7*3600,0,"pool.ntp.org","time.google.com"); }
  draw();
}

void handleTouch(int x,int y){
  beep();
  if(y>=199){ for(int i=0;i<4;i++){ int tx0=6+i*78; if(x>=tx0&&x<tx0+74){
    if(page!=i){ page=i; if(page==2) lastPoll=0; draw(); } return; } } return; }
  if(page==1){
    for(int i=0;i<6;i++){ int kx,ky,kw,kh; deckRect(i,kx,ky,kw,kh);
      if(x>=kx&&x<kx+kw&&y>=ky&&y<ky+kh){ keyFlash(i); openApp(APPID[i]); return; } }
  } else if(page==3){
    if(y<66) reconfigWifi();                       // WiFi
    else if(y>=94&&y<124) reconfigWifi();          // PC IP (cung portal)
    else if(y>=124&&y<154){ if(wifiOK) configTime(7*3600,0,"pool.ntp.org","time.google.com"); draw(); }
    else if(y>=154){ splash("Dang khoi dong lai...","",""); delay(600); ESP.restart(); }
  }
}

void doCalibrate(uint16_t*cal){ tft.fillScreen(BG);
  txt(1,160,96,"Cham lan luot 4 goc",ORANGE,BG,MC_DATUM);
  txt(0,160,122,"theo mui ten",MUTED,BG,MC_DATUM);
  tft.calibrateTouch(cal,ORANGE,BG,16); }
void loadCalibration(){
  uint16_t cal[5]={360,3500,340,3600,7};
  prefs.begin("ptp",false);
  bool have=(prefs.getBytesLength("cal")==10);
  if(have) prefs.getBytes("cal",cal,10);
  if(tft.getTouchRawZ()>600){ doCalibrate(cal); prefs.putBytes("cal",cal,10); }
  tft.setTouch(cal); prefs.end();
}

void setup(){
  pinMode(BUZZER,OUTPUT); digitalWrite(BUZZER,HIGH);
  tft.init(); tft.setRotation(3); tft.setSwapBytes(true); tft.fillScreen(BG);
  loadCalibration();
  prefs.begin("ptp",false); String ev=prefs.getString("event",""); String ip=prefs.getString("pcip",""); prefs.end();
  if(ev.length()>0){ strncpy(eventTxt,ev.c_str(),47); eventTxt[47]=0; }
  if(ip.length()>0){ strncpy(pcIP,ip.c_str(),19); pcIP[19]=0; }

  splash("Dang ket noi WiFi...","","");
  // Thu ket noi nhanh bang WiFi da luu (nho kenh + router)
  wifiOK = fastConnect();
  if(!wifiOK){
    // Chua co WiFi hoac doi router -> mo portal cai dat
    WiFi.mode(WIFI_STA); WiFi.setSleep(false);
    wm.addParameter(&pEvent); wm.addParameter(&pPC);
    wm.setAPCallback(apCb);
    wm.setConfigPortalTimeout(180);
    wm.setBreakAfterConfig(true);   // bam Luu la thoat portal + vao giao dien NGAY
    wm.setConnectTimeout(12);
    wm.setWiFiAutoReconnect(true);
    wm.autoConnect("PTP-REPAIR-Setup");
    pullParams();                   // luu IP may tinh + su kien vua nhap
    splash("Dang ket noi WiFi...","","");
    wifiOK = waitConn(8000);        // thu vai giay, con lai de chay nen
  }
  if(wifiOK){ saveWifi();
    configTime(7*3600,0,"pool.ntp.org","time.google.com"); } // dong bo gio chay nen, KHONG chan khoi dong
  draw();
}
void loop(){
  uint32_t now=millis();
  if(now-lastSec>=1000){ lastSec=now; if(page==0) homeClock(); }
  // Theo doi WiFi chay nen: khi ket noi len/rot thi tu cap nhat giao dien
  static uint32_t lwc=0;
  if(now-lwc>=2000){ lwc=now; bool c=(WiFi.status()==WL_CONNECTED);
    if(c!=wifiOK){ wifiOK=c; if(c){ saveWifi(); configTime(7*3600,0,"pool.ntp.org","time.google.com"); } draw(); } }
  // Chi doc may in khi dang xem trang May in -> cac trang khac muot 100%
  if(wifiOK && page==2 && now-lastPoll>=15000){ lastPoll=now; pollPrinter(); }
  if(page==2 && pDirty && now-lastPrintDraw>=800){ lastPrintDraw=now; pDirty=false; pagePrint(); }
  uint16_t tx,ty;
  if(tft.getTouch(&tx,&ty)){ handleTouch(tx,ty);
    while(tft.getTouch(&tx,&ty)) delay(10); delay(60); }
}

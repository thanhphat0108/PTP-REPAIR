#include <TFT_eSPI.h>
#include "F_SML.h"   // Poppins Medium 15px
#include "F_MED.h"   // Poppins Bold 18px
#include "F_BIG.h"   // Poppins Bold 46px (clock)
#include "LOGO.h"    // PTP REPAIR 64x64 RGB565

TFT_eSPI tft = TFT_eSPI();

#define C565(r,g,b) (uint16_t)((((r)&0xF8)<<8)|(((g)&0xFC)<<3)|((b)>>3))
static const uint16_t BG=C565(11,13,17), CARD=C565(20,24,33), KEYAPP=C565(23,31,43),
  ORANGE=C565(242,162,74), MUTED=C565(139,147,166), DATEC=C565(195,201,214),
  GRN=C565(61,214,140), BLU=C565(74,168,255), WHITE=C565(232,234,240),
  RED=C565(224,70,70), DARK=C565(26,18,6), BORDER=C565(30,34,43);

int page=0, hh=21, mm=44, ss=0;
uint32_t lastSec=0, lastPage=0; int curFont=-1;

void useFont(int id){ if(id==curFont)return; curFont=id;
  if(id==0)tft.loadFont(F_SML); else if(id==1)tft.loadFont(F_MED); else tft.loadFont(F_BIG); }
int tw(int id,const char*s){ useFont(id); return tft.textWidth(s); }
void txt(int id,int x,int y,const char*s,uint16_t fg,uint16_t bg,uint8_t dat){
  useFont(id); tft.setTextDatum(dat); tft.setTextColor(fg,bg); tft.drawString(s,x,y); }
void rr(int x,int y,int w,int h,uint16_t c,int r=8){ tft.fillRoundRect(x,y,w,h,r,c); }

// ---- icons ----
void icHome(int cx,int cy,uint16_t c){ tft.fillTriangle(cx-8,cy-1,cx,cy-9,cx+8,cy-1,c);
  tft.fillRect(cx-6,cy-1,12,7,c); tft.fillRect(cx-2,cy+1,4,5,BG); }
void icGrid(int cx,int cy,uint16_t c){ for(int i=0;i<2;i++)for(int j=0;j<2;j++)
  tft.fillRoundRect(cx-7+j*8,cy-7+i*8,6,6,1,c); }
void icPrint(int cx,int cy,uint16_t c){ tft.fillRect(cx-4,cy-8,8,4,c);
  tft.fillRoundRect(cx-8,cy-4,16,8,2,c); tft.fillRect(cx-5,cy+1,10,6,c); tft.fillRect(cx-3,cy+3,6,2,BG); }
void icGear(int cx,int cy,uint16_t c){ tft.fillCircle(cx,cy,7,c);
  for(int a=0;a<360;a+=60){ float r=a*3.14159/180; tft.fillRect(cx+cos(r)*7-1,cy+sin(r)*7-1,3,3,c); }
  tft.fillCircle(cx,cy,3,BG); }
void icMic(int cx,int cy,uint16_t c){ tft.fillRoundRect(cx-3,cy-8,6,10,3,c);
  tft.drawFastVLine(cx,cy+2,4,c); tft.drawFastHLine(cx-4,cy+6,9,c);
  tft.drawFastVLine(cx-5,cy-1,2,c); tft.drawFastVLine(cx+5,cy-1,2,c); }

void tabBar(){
  tft.fillRect(0,195,320,45,BG); tft.drawFastHLine(0,196,320,BORDER);
  const char* L[]={"Trang chu","Deck","May in","Cai dat"};
  for(int i=0;i<4;i++){ int x=6+i*78; bool a=(i==page);
    rr(x,199,74,38,a?ORANGE:CARD,9);
    uint16_t ic=a?DARK:MUTED, bg=a?ORANGE:CARD; int cx=x+37;
    if(i==0)icHome(cx,210,ic); else if(i==1)icGrid(cx,210,ic);
    else if(i==2)icPrint(cx,210,ic); else icGear(cx,210,ic);
    txt(0,cx,222,L[i],a?DARK:MUTED,bg,TC_DATUM); }
}
void drawLogo(int x,int y){ tft.pushImage(x,y,LOGO_W,LOGO_H,LOGO); }

void homeClock(){
  tft.fillRect(8,26,232,56,BG);
  char b[6]; sprintf(b,"%02d:%02d",hh,mm);
  txt(2,10,28,b,WHITE,BG,TL_DATUM); int w=tw(2,b);
  char s[3]; sprintf(s,"%02d",ss);
  txt(1,16+w,36,s,ORANGE,BG,TL_DATUM);
}
void pageHome(){
  tft.fillRect(0,0,320,196,BG);
  tft.fillCircle(13,17,4,GRN);
  txt(0,24,10,"Wi-Fi",MUTED,BG,TL_DATUM);
  txt(0,312,10,"28°C   21:44",MUTED,BG,TR_DATUM);
  drawLogo(248,24); homeClock();
  txt(0,10,92,"Thu Ba, 17 Thang 11, 2026",DATEC,BG,TL_DATUM);
  const char* d[]={"T2","T3","T4","T5","T6","T7","CN"};
  for(int i=0;i<7;i++){ int x=8+i*43; bool t=(i==1);
    rr(x,114,40,40,t?ORANGE:CARD,6);
    txt(0,x+20,117,d[i],t?DARK:MUTED,t?ORANGE:CARD,TC_DATUM);
    char n[3]; sprintf(n,"%d",16+i);
    txt(1,x+20,130,n,t?DARK:WHITE,t?ORANGE:CARD,TC_DATUM); }
  tft.fillRect(10,164,6,8,ORANGE); tft.fillRect(9,163,2,2,ORANGE);
  txt(0,22,160,"Lich: ",MUTED,BG,TL_DATUM);
  txt(0,22+tw(0,"Lich: "),160,"Giao hang khach 15:00",ORANGE,BG,TL_DATUM);
  tft.fillCircle(292,168,15,BLU); icMic(292,168,WHITE);
  tabBar();
}
void key(int x,int y,int w,int h,const char*l){
  rr(x,y,w,h,KEYAPP,9); txt(0,x+w/2,y+h/2,l,C565(178,186,202),KEYAPP,MC_DATUM); }
void pageDeck(){
  tft.fillRect(0,0,320,196,BG);
  tft.fillCircle(13,17,4,GRN);
  txt(1,24,8,"Stream Deck",WHITE,BG,TL_DATUM);
  txt(0,312,10,"21:44",MUTED,BG,TR_DATUM);
  txt(0,10,38,"MAY TINH",MUTED,BG,TL_DATUM);
  const char* k[]={"Bambu","OBS quay","CapCut","Chrome","Thu muc","Chup hinh"};
  for(int i=0;i<6;i++){ int x=6+(i%3)*103; int y=54+(i/3)*58; key(x,y,98,52,k[i]); }
  tabBar();
}
void pagePrint(){
  tft.fillRect(0,0,320,196,BG);
  tft.fillCircle(13,17,4,GRN);
  txt(1,24,8,"May in 3D",WHITE,BG,TL_DATUM);
  txt(0,312,10,"21:44",MUTED,BG,TR_DATUM);
  tft.fillCircle(15,40,3,GRN);
  txt(0,25,34,"Dang in: benchy.3mf",GRN,BG,TL_DATUM);
  const char* la[]={"Dau phun","Ban nhiet","Lop"};
  const char* va[]={"210","60","58"};
  const char* su[]={"/210°C","/60°C","/142"};
  for(int i=0;i<3;i++){ int x=8+i*102; rr(x,54,96,48,CARD,8);
    txt(0,x+8,60,la[i],MUTED,CARD,TL_DATUM);
    txt(1,x+8,74,va[i],WHITE,CARD,TL_DATUM); int wv=tw(1,va[i]);
    txt(0,x+10+wv,80,su[i],MUTED,CARD,TL_DATUM); }
  rr(8,110,304,14,CARD,7); rr(8,110,(int)(304*0.41),14,ORANGE,7);
  txt(0,8,128,"41%",MUTED,BG,TL_DATUM);
  txt(0,312,128,"con 37 phut",MUTED,BG,TR_DATUM);
  rr(8,150,96,44,ORANGE,10); txt(0,56,172,"Tam dung",DARK,ORANGE,TC_DATUM);
  rr(112,150,96,44,CARD,10); txt(0,160,172,"Chinh",WHITE,CARD,TC_DATUM);
  rr(216,150,96,44,CARD,10); txt(0,264,172,"Dung",RED,CARD,TC_DATUM);
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
  tft.fillCircle(13,17,4,GRN);
  txt(1,24,8,"Cai dat",WHITE,BG,TL_DATUM);
  txt(0,312,10,"21:44",MUTED,BG,TR_DATUM);
  srow(46,"Wi-Fi: Xuong_Sua","-52 dBm",false,false,MUTED);
  srow(76,"Dong bo gio (NTP)","",true,true,MUTED);
  srow(106,"Do sang man hinh","80%",false,false,MUTED);
  srow(136,"Lien ket may in Bambu","Da noi",false,false,GRN);
  srow(166,"Cap nhat OTA","v1.0",false,false,MUTED);
  tabBar();
}
void draw(){ if(page==0)pageHome(); else if(page==1)pageDeck();
  else if(page==2)pagePrint(); else pageSettings(); }
void setup(){
  tft.init(); tft.setRotation(3);
  tft.setSwapBytes(true); tft.fillScreen(BG); draw();
}
void loop(){
  uint32_t now=millis();
  if(now-lastSec>=1000){ lastSec=now;
    if(++ss>=60){ ss=0; if(++mm>=60){ mm=0; hh=(hh+1)%24; } }
    if(page==0)homeClock(); }
  if(now-lastPage>=6000){ lastPage=now; page=(page+1)%4; draw(); }
}

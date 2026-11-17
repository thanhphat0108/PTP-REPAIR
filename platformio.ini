#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

#define BG     tft.color565(21,23,28)
#define CARD   tft.color565(32,36,44)
#define ORANGE tft.color565(242,162,74)
#define MUTED  tft.color565(150,160,180)
#define GRN    tft.color565(61,214,140)
#define BLU    tft.color565(74,168,255)

int page = 0;
int hh = 20, mm = 52, ss = 49;
uint32_t lastSec = 0, lastPage = 0;

void tabBar() {
  const char* t[] = {"Trang chu","Deck","May in","Cai dat"};
  for (int i = 0; i < 4; i++) {
    int x = 6 + i * 78; bool a = (i == page);
    tft.fillRoundRect(x, 200, 74, 34, 7, a ? ORANGE : CARD);
    tft.setTextDatum(MC_DATUM); tft.setTextSize(1);
    tft.setTextColor(a ? tft.color565(26,18,6) : MUTED, a ? ORANGE : CARD);
    tft.drawString(t[i], x + 37, 217);
  }
}
void clockSmall() {
  char b[6]; sprintf(b, "%02d:%02d", hh, mm);
  tft.setTextDatum(TR_DATUM); tft.setTextColor(MUTED, BG); tft.setTextSize(1);
  tft.drawString(b, 314, 6);
}
void homeClock() {
  tft.fillRect(8,28,150,44,BG); tft.fillRect(160,28,46,26,BG);
  char b[6]; sprintf(b, "%02d:%02d", hh, mm);
  tft.setTextDatum(TL_DATUM); tft.setTextColor(TFT_WHITE, BG);
  tft.setTextSize(5); tft.drawString(b, 8, 30);
  char s[3]; sprintf(s, "%02d", ss);
  tft.setTextSize(2); tft.setTextColor(ORANGE, BG); tft.drawString(s, 160, 32);
}
void pageHome() {
  tft.fillRect(0,0,320,198,BG);
  tft.fillCircle(10,10,3,GRN);
  tft.setTextDatum(TL_DATUM); tft.setTextColor(MUTED,BG); tft.setTextSize(1);
  tft.drawString("Wi-Fi",18,6); clockSmall();
  tft.drawCircle(292,46,20,TFT_RED); tft.drawCircle(292,46,17,TFT_RED);
  tft.setTextDatum(MC_DATUM); tft.setTextColor(TFT_WHITE,BG);
  tft.drawString("PTP",292,42); tft.drawString("REPAIR",292,52);
  tft.setTextDatum(TL_DATUM); tft.setTextColor(MUTED,BG);
  tft.drawString("Thu Ba, 17 Thang 11, 2026",8,76);
  const char* d[]={"T2","T3","T4","T5","T6","T7","CN"};
  for(int i=0;i<7;i++){int x=8+i*43;bool t=(i==1);
    tft.fillRoundRect(x,92,40,32,5,t?ORANGE:CARD);
    tft.setTextDatum(MC_DATUM);tft.setTextSize(1);
    tft.setTextColor(t?tft.color565(90,52,16):MUTED,t?ORANGE:CARD);
    tft.drawString(d[i],x+20,101);
    tft.setTextSize(2);tft.setTextColor(t?tft.color565(26,18,6):TFT_WHITE,t?ORANGE:CARD);
    tft.drawString(String(16+i),x+20,115);}
  tft.setTextDatum(TL_DATUM);tft.setTextSize(1);
  tft.setTextColor(MUTED,BG);tft.drawString("Su kien tiep theo:",8,136);
  tft.setTextColor(ORANGE,BG);tft.drawString("Giao hang khach 15:00",108,136);
  tft.fillCircle(292,168,15,BLU);
  tft.setTextDatum(MC_DATUM);tft.setTextColor(tft.color565(6,18,31));tft.setTextSize(2);
  tft.drawString("M",292,168);
  homeClock(); tabBar();
}
void key(int x,int y,int w,int h,uint16_t col,const char*l,bool on){
  tft.fillRoundRect(x,y,w,h,8,on?col:CARD);
  tft.setTextDatum(MC_DATUM);tft.setTextSize(2);
  tft.setTextColor(on?tft.color565(26,18,6):MUTED,on?col:CARD);
  tft.drawString(l,x+w/2,y+h/2);
}
void pageDeck(){
  tft.fillRect(0,0,320,198,BG);
  tft.setTextDatum(TL_DATUM);tft.setTextColor(TFT_WHITE,BG);tft.setTextSize(2);
  tft.drawString("Stream Deck",8,8); clockSmall();
  key(6,40,100,70,ORANGE,"May kho",false);
  key(110,40,100,70,ORANGE,"Den ban",true);
  key(214,40,100,70,ORANGE,"Quat hut",false);
  key(6,118,100,70,ORANGE,"May han",false);
  key(110,118,100,70,BLU,"Den quay",false);
  key(214,118,100,70,TFT_RED,"Tat het",false);
  tabBar();
}
void pagePrint(){
  tft.fillRect(0,0,320,198,BG);
  tft.setTextDatum(TL_DATUM);tft.setTextColor(TFT_WHITE,BG);tft.setTextSize(2);
  tft.drawString("May in 3D",8,8); clockSmall();
  tft.fillCircle(104,15,3,GRN);
  tft.setTextSize(1);tft.setTextColor(MUTED,BG);
  tft.drawString("Dang in: benchy.3mf",116,11);
  const char* lab[]={"Dau phun","Ban nhiet","Lop"};
  const char* val[]={"210 C","60 C","87/142"};
  for(int i=0;i<3;i++){int x=8+i*102;
    tft.fillRoundRect(x,38,96,44,6,CARD);
    tft.setTextDatum(TL_DATUM);tft.setTextColor(MUTED,CARD);tft.setTextSize(1);
    tft.drawString(lab[i],x+8,46);
    tft.setTextColor(TFT_WHITE,CARD);tft.setTextSize(2);
    tft.drawString(val[i],x+8,60);}
  tft.fillRoundRect(8,96,304,14,7,CARD);
  tft.fillRoundRect(8,96,185,14,7,ORANGE);
  tft.setTextDatum(TL_DATUM);tft.setTextColor(MUTED,BG);tft.setTextSize(1);
  tft.drawString("61%",8,116);
  tft.setTextDatum(TR_DATUM);tft.drawString("con 38 phut",312,116);
  key(8,138,150,48,ORANGE,"Tam dung",true);
  key(164,138,148,48,TFT_RED,"Dung",true);
  tabBar();
}
void srow(int y,const char*k,const char*v,bool sw,bool on){
  tft.setTextDatum(TL_DATUM);tft.setTextColor(TFT_WHITE,BG);tft.setTextSize(1);
  tft.drawString(k,10,y);
  if(sw){tft.fillRoundRect(282,y-3,30,16,8,on?GRN:CARD);
    tft.fillCircle(on?302:292,y+5,6,TFT_WHITE);}
  else{tft.setTextDatum(TR_DATUM);tft.setTextColor(MUTED,BG);tft.drawString(v,312,y);}
  tft.drawFastHLine(8,y+21,304,CARD);
}
void pageSettings(){
  tft.fillRect(0,0,320,198,BG);
  tft.setTextDatum(TL_DATUM);tft.setTextColor(TFT_WHITE,BG);tft.setTextSize(2);
  tft.drawString("Cai dat",8,8); clockSmall();
  srow(44,"Wi-Fi: Xuong_Sua","-52 dBm",false,false);
  srow(76,"Dong bo gio (NTP)","",true,true);
  srow(108,"Do sang man hinh","80%",false,false);
  srow(140,"Cap nhat OTA","v1.0",false,false);
  srow(172,"Che do toi tu dong","",true,true);
  tabBar();
}
void draw(){
  if(page==0)pageHome();else if(page==1)pageDeck();
  else if(page==2)pagePrint();else pageSettings();
}
void setup(){ tft.init(); tft.setRotation(1); draw(); }
void loop(){
  uint32_t now=millis();
  if(now-lastSec>=1000){lastSec=now;
    if(++ss>=60){ss=0;if(++mm>=60){mm=0;hh=(hh+1)%24;}}
    if(page==0)homeClock();}
  if(now-lastPage>=6000){lastPage=now;page=(page+1)%4;draw();}
}

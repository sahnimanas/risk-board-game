#include"graphics.h"
#include<iostream>
#include<ctype.h>
#include<math.h>
#include<time.h>
#define CRED        COLOR(204,0,0)
#define CBLUE       COLOR(0,102,153)
#define CH          COLOR(255,100,0)
#define UP_ARROW    72
#define LEFT_ARROW  75
#define DOWN_ARROW  80
#define RIGHT_ARROW 77
#define ENTER       13
#define ESC         27
#define NUM         4
#define AI          1
#define SW          1240
#define SH          620
#define R           20
void *p;
char a[3],b[3];
int page=0;
int size;
int flag;
using namespace std;

int random(int low, int high)//return random no. b/w low & high
{ return (rand() % (high-low+1) + low); }

int menu()
{   readimagefile("img/start.gif",0,0,SW,SH);
	delay(600);
	while(!ismouseclick(WM_LBUTTONDOWN))
		{    if(mousex()>520 && mousex()<715 && mousey()>400 && mousey()<470)	
			   {setfillstyle(SOLID_FILL,BLACK);bar(520,430,715,435);
				setfillstyle(SOLID_FILL,WHITE);bar(520,500,715,505);}
		else if(mousex()>520 && mousex()<715 && mousey()>470 && mousey()<540)
			   {setfillstyle(SOLID_FILL,WHITE);bar(520,430,715,435);
				setfillstyle(SOLID_FILL,BLACK);bar(520,500,715,505);}
		else   {setfillstyle(SOLID_FILL,WHITE);bar(520,430,715,435);
				setfillstyle(SOLID_FILL,WHITE);bar(520,500,715,505);continue;}
		}
	int p,q;
	getmouseclick(WM_LBUTTONDOWN,p,q);
	     if(p<520 || p>715 || q<400 || q>540) return menu();
	else if(q<470)	return 1;
	else			return 0;
}

void map()
{
  putimage(0,0,p,COPY_PUT);
}

class state
{  int troops,owner,i,j,bs,bt;
   float reinf,att;
   int x,y;
   
   public:
   void				setval(int t, int o,int i1,int j1)  {troops=t;owner=o;i=i1;j=j1;}
   static void		start();
   static void		shuffle();
   static void		display();
   static void		display(int i1, int j1); 
   static void		display(int i1, int j1, int i2, int j2);
   static state*	select(int o,char* str);
   static state*	select(int o,int i1, int j1,char* str);
   static int		total(int o);
   static int		tps(int o);
   static void		reinforce(int o,int reinf);
   static void		attack(int o);
   void				checkborder();
   void				reinfp();
   static void		AIreinf(int a12);
   static void		AIattack(int a12);
   
}s[NUM][NUM],temp;

void state::start()
{int cod[]={290,170,\
630,165,\
815,144,\
1020,120,\

300,250,\
636,252,\
756,260,\
936,230,\

420,405,\
660,370,\
858,298,\
960,360,\

415,505,\
696,456,\
1000,480,\
1100,504         \
            };
 int ctr=0;
 for(int i=0;i<NUM;i++)
 for(int j=0;j<NUM;j++)
     {s[i][j].x=cod[2*(i*NUM+j)];
      s[i][j].y=cod[2*(i*NUM+j)+1];
      
      if(ctr<NUM*NUM/2) s[i][j].setval(random(1,3),0,i,j);
      else              s[i][j].setval(random(1,3),1,i,j);
      ctr++;
     }
}

void state::shuffle()
    {for(int i1=0;i1<NUM;i1++)
     for(int j1=0;j1<NUM;j1++)
         { int i2=random(0,NUM-1), j2=random(0,NUM-1);
           state t=s[i2][j2];
           s[i2][j2].troops = s[i1][j1].troops;
           s[i2][j2].owner  = s[i1][j1].owner;
           s[i1][j1].troops = t.troops;
           s[i1][j1].owner  = t.owner;
         }
    }

void state::display()
{    //page=1-page;
     setactivepage(page);
     setvisualpage(1-page);
     map();
     for(int i=0;i<NUM;i++)
     for(int j=0;j<NUM;j++)
     {
        if(s[i][j].owner==AI)setfillstyle(SOLID_FILL, CRED);
        else                 setfillstyle(SOLID_FILL,CBLUE);                             
        fillellipse(s[i][j].x,s[i][j].y,R,R);
        outtextxy(s[i][j].x-5,s[i][j].y-5,itoa(s[i][j].troops,a,10));
     }
     setactivepage(1-page);
     page=1-page;
}

void state::display(int i1, int j1)
{    state::display();
     setfillstyle(SOLID_FILL, COLOR(255,255,0));
     fillellipse (s[i1][j1].x,s[i1][j1].y,R+5,R+5);
     if(s[i1][j1].owner==AI)setfillstyle(SOLID_FILL, CRED);
     else                 setfillstyle(SOLID_FILL,CBLUE);                             
     fillellipse(s[i1][j1].x,s[i1][j1].y,R,R);
     outtextxy(s[i1][j1].x-5,s[i1][j1].y-5,itoa(s[i1][j1].troops,a,10));
}

void state::display(int i1, int j1, int i2, int j2)
{    state::display(i1,j1);
     setfillstyle(SOLID_FILL,CH);
     fillellipse (s[i2][j2].x,s[i2][j2].y,R+7,R+7);
     if(s[i2][j2].owner==AI)setfillstyle(SOLID_FILL,CRED);
     else                   setfillstyle(SOLID_FILL,CBLUE); 
     fillellipse (s[i2][j2].x,s[i2][j2].y,R,R);
     outtextxy   (s[i2][j2].x-5,s[i2][j2].y-5,itoa(s[i2][j2].troops,a,10));      
}

state* state::select(int o,char* str)
{    int i1=0,j1=0;char key;
     while(1)
     {    do
          {  state::display(i1,j1);
             outtextxy(504,570,str);  
//             cout<<i1<<","<<j1<<endl;
             key=getch();
             switch(key)
             {case UP_ARROW      : if    (i1!=0)   i1--;
                                   else            i1=NUM-1; break;
              case DOWN_ARROW    : if   (i1!=NUM-1)i1++;
                                   else            i1=0;     break;
              case LEFT_ARROW    : if   (j1!=0)    j1--;
                                   else            j1=NUM-1; break;
              case RIGHT_ARROW   : if  (j1!=NUM-1) j1++;
                                   else            j1=0;     break;
              case ESC           : state::display();outtextxy(504,550,"Action cancelled");page=1-page;return NULL;
             }
          }
          while(key!=ENTER);
		  if(s[i1][j1].owner!=o){outtextxy(504,570,"Error! Enter again!");getch();}
          else break;
     }
    return &(s[i1][j1]);
} 


state* state::select(int o,int ia, int ja,char* str)
{    char key;
     int il=1,ih=1,jl=1,jh=1;
     switch(ia)
         {case     0: il=0;break;
          case NUM-1: ih=0;break;}
     switch(ja)
         {case     0: jl=0;break;
          case NUM-1: jh=0;break;}
     int i1=ia-il,j1=ja-jl;
     
     while(1)
     {    do
          {  state::display(ia,ja,i1,j1);
			 /*page=1-page;
			 setactivepage(page);
			 setvisualpage(1-page);*/
			 outtextxy(504,570,str);    
             for(int i=ia-il; i<=ia+ih; i++)
             for(int j=ja-jl; j<=ja+jh; j++)
                 {if(s[i][j].owner==s[ia][ja].owner || i==i1 && j==j1)continue;
                  setfillstyle(SOLID_FILL,COLOR(51,204,51));
                  fillellipse (s[i][j].x,s[i][j].y,R+5,R+5);
                  if(s[i][j].owner==AI)setfillstyle(SOLID_FILL,CRED);
                  else                 setfillstyle(SOLID_FILL,CBLUE);                                                               
                  fillellipse (s[i][j].x,s[i][j].y,R,R);
                  outtextxy   (s[i][j].x-5,s[i][j].y-5,itoa(s[i][j].troops,a,10));
                 }
			 /*setactivepage(1-page);
			 page=1-page;*/
             
             key=getch();
             switch(key)
             {case UP_ARROW      : if(ia-i1<1 && i1>0)           i1--;break;
              case DOWN_ARROW    : if(i1-ia<1 && i1<NUM-1)       i1++;break;
              case LEFT_ARROW    : if(ja-j1<1 && j1>0)           j1--;break;
              case RIGHT_ARROW   : if(j1-ja<1 && j1<NUM-1)       j1++;break;
              case ESC           : state::display();outtextxy(504,550,"Action Cancelled");page=1-page;return NULL;
             }
          }
          while(key!=ENTER);
          if(s[i1][j1].owner!=o){outtextxy(504,570,"Error! Enter again! (Press Escape to cancel)");if(getch()==ESC)return NULL;}
          else break;
     }
    return &(s[i1][j1]);
}

int state::total(int o)
{    int ctr=0;
     for(int x=0;x<NUM;x++)
     for(int y=0;y<NUM;y++)
     if(s[x][y].owner==o)ctr++;
     return ctr;
}

int state::tps(int o)
{    int ctr=0;
     for(int x=0;x<NUM;x++)
     for(int y=0;y<NUM;y++)
     if(s[x][y].owner==o)ctr+=s[x][y].troops;
     return ctr;
}
void state::reinforce(int o,int reinf)
{
	 outtextxy(504,570,"Total reinforcements: ");outtextxy(504+textwidth("Total reinforcements: "),570,strcat(itoa(reinf,a,10)," "));
	 delay(1100);getch();
     state *target=NULL;
     while(target==NULL)target=state::select(o,"Select a state to reinforce:");
     //cleardevice();
     state::display(target->i,target->j);     
     outtextxy(504,570,"Add/remove troops:");
     char key;int c=0;
     do
     {fillellipse(target->x,target->y,R,R);
	  outtextxy(target->x-5,target->y-5,itoa(c+target->troops,a,10));
	  key=getch();
      switch(key)
          {case LEFT_ARROW  :
           case DOWN_ARROW  : if(c+target->troops>1)--c;break;
           case RIGHT_ARROW :
           case UP_ARROW    : if(c<reinf)++c;break;
          }
      if(key==ESC)
      {c=0;outtextxy(504,570,"Action Cancelled");}
       
	  state::display(target->i,target->j);
      outtextxy(504,570,"Add/remove troops:");
      
     }
     while(key!=ENTER && key!=ESC);
     
     target->troops+=c;
     reinf-=c;cleardevice();
     state::display();state::display();
     if(reinf>0)state::reinforce(o,reinf);
}

void state::attack(int o)
{state::display();state::display();
 att1:outtextxy(504,570,"Attack?");char x=getch();
 att2:
 if(x=='N'||x=='n'||x==ESC)return;
 
 state *attack,*target;
 
 while(1)
 {attack=state::select(o,"Select attacking state");
  if(attack==NULL)goto att1;
  if(attack->troops==1)
                       {outtextxy(504,570,"Not enough troops! Enter again (Press Escape to cancel)");
                        x=getch();
                        if(x==ESC)goto att1;
                        else continue;}
  else break;}
  
 cleardevice();
 target=state::select(AI,attack->i,attack->j,"Select target");
 if(target==NULL)goto att1;
  
 cleardevice();
 state::display(attack->i,attack->j,target->i,target->j);
 line(attack->x,attack->y,target->x,target->y);
 outtextxy(504,570,"Send troops:");
 char key;int t=0;
 do
 {  key=getch();
    switch(key)
        {case LEFT_ARROW  :
         case DOWN_ARROW  : if(t>0)--t;break;
         case RIGHT_ARROW :
         case UP_ARROW    : if(t<attack->troops-1)++t;break;
        }
    page=1-page;
    if(key==ESC)
       {t=0;outtextxy(504,550,"Attack Cancelled");goto att1;}
    state::display(attack->i,attack->j,target->i,target->j);
    line(attack->x,attack->y,target->x,target->y);
    outtextxy(504,570,"Send troops:");
	setfillstyle(SOLID_FILL,CBLUE);
	fillellipse(attack->x,attack->y,R,R);
    outtextxy(attack->x-5,attack->y-5,itoa(attack->troops-t,a,10));
    outtextxy((target->x+attack->x)/2,(target->y+attack->y)/2,itoa(t,a,10));swapbuffers();
 }
 while(key!=ENTER && key!=ESC);
 
 attack->troops-=t;
 int min=target->troops + target->troops/4 + random(1,2);
 if(t>= min)
     {target->owner  =o;
      target->troops=t+1-min;
      state::display();state::display();outtextxy(504,550,"Attack successful");}
 else
     {target->troops= target->troops + target->troops/4 - t;
      if(target->troops<=0)target->troops=1;
      state::display();state::display();
      outtextxy(504,550,"Attack failed");}
 if(state::total(AI)==0)return;
 outtextxy(504,570,"Attack again?");x=getch();goto att2;
}

void state::checkborder()
{bs=bt=0;
 int ih=1,jh=1,il=1,jl=1;
 
 switch(i)
 {case 0    : il=0;break;
  case NUM-1: ih=0;break;}
 
 switch(j)
 {case 0    : jl=0;break;
  case NUM-1: jh=0;break;}

 for(int x=i-il;x<=i+ih;x++)
 for(int y=j-jl;y<=j+jh;y++)
 if(s[x][y].owner!=owner){bs++;bt+=s[x][y].troops;} else continue;
}

void state::reinfp()
{reinf=0;
 checkborder();
 reinf=bs*float(bt)/float(troops);
}

void state::AIreinf(int a12)
{state *h1=&temp,*h2=&temp,*h3=&temp;
 h1->reinf=0;h2->reinf=0;h3->reinf=0;
 int n=0;
 n+=state::tps(a12)/3;
 state::display();
 for(int i=0;i<NUM;i++)
 for(int j=0;j<NUM;j++)
 if(s[i][j].owner==a12)
   {        s[i][j].reinfp();
         if(s[i][j].reinf==0 && s[i][j].troops>1) 
                             {outtextxy(s[i][j].x+textwidth(itoa(s[i][j].troops,a,10))-5,s[i][j].y-5,"-");
                              outtextxy(s[i][j].x+textwidth(itoa(s[i][j].troops,a,10))+textwidth("-")-5,s[i][j].y-5,itoa((s[i][j].troops-1),b,10));
                              n+=s[i][j].troops-1;s[i][j].troops=1;}
    else if(s[i][j].reinf>h1->reinf)
                             {h2=h1;h1=&s[i][j];}
    else if(s[i][j].reinf>h2->reinf)              
                             {h3=h2;h2=&s[i][j];}
    else if(s[i][j].reinf>h3->reinf)        
                              h3=&s[i][j];
   }
 
 float x=h1->reinf+h2->reinf+h3->reinf;

 outtextxy(h1->x+textwidth(itoa(h1->troops,a,10))-5,h1->y-5,"+");
 outtextxy(h1->x+textwidth(itoa(h1->troops,a,10))+textwidth("+")-5,h1->y-5,itoa((int)ceil(n*h1->reinf/x),b,10));
 h1->troops+=(int)ceil(n*h1->reinf/x);
 
 outtextxy(h2->x+textwidth(itoa(h2->troops,a,10))-5,h2->y-5,"+");
 outtextxy(h2->x+textwidth(itoa(h2->troops,a,10))+textwidth("+")-5,h2->y-5,itoa((int)ceil(n*h2->reinf/x),b,10));
 h2->troops+=(int)ceil(n*h2->reinf/x);
 
 outtextxy(h3->x+textwidth(itoa(h3->troops,a,10))-5,h3->y-5,"+");
 outtextxy(h3->x+textwidth(itoa(h3->troops,a,10))+textwidth("+")-5,h3->y-5,itoa((int)ceil(n*h3->reinf/x),b,10));
 h3->troops+=(int)ceil(n*h3->reinf/x);
 delay(200);getch();
 state::display();state::display();
}

void state::AIattack(int a12)
{    cleardevice();
     state::display();state::display();
     state *t=&temp,*a=&temp;
     t->reinf=0;
     a->reinf=0;
     a->troops=0;
     for(int x=0;x<NUM;x++)
     for(int y=0;y<NUM;y++)
     {   s[x][y].reinfp();
         if(s[x][y].owner!=a12)
         {s[x][y].reinf/=(s[x][y].bs*s[x][y].troops);if(s[x][y].reinf > t->reinf)t=&s[x][y];}
     }
     int min=t->troops + t->troops/4+random(1,2);
     int ih=1,jh=1,il=1,jl=1;
     switch(t->i)
     {case 0    : il=0;break;
      case NUM-1: ih=0;break;}
     switch(t->j)
     {case 0    : jl=0;break;
      case NUM-1: jh=0;break;}
     a->reinf=s[t->i][t->j].reinf;
     for(int x=t->i-il;x<=t->i+ih;x++)
     for(int y=t->j-jl;y<=t->j+jh;y++)
     if(s[x][y].owner==a12 && s[x][y].troops >min && s[x][y].troops>a->troops)a=&s[x][y];
     
     if(t==&temp || a==&temp){outtextxy(504,570,"No attack");return;}
     
     state::display(a->i,a->j,t->i,t->j);
     line(a->x,a->y,t->x,t->y);
     int x=min;
     x+=random(0,a->troops-min-1);
     outtextxy((a->x+t->x)/2,(a->y+t->y)/2,itoa(x,b,10));
     delay(400);
     a->troops-=x;
     t->owner  =a12;
     t->troops=x+1-min;
     state::display();state::display();
     outtextxy(504,570,"State captured!");
     delay(900);
     if(state::total(0)!=0)
     state::AIattack(a12);
}

int main()
{   
    srand(time(0));
    temp=s[0][0];
    int gdriver = DETECT, gmode;
    initwindow(SW, SH, "Risk",0,0,true);

	
	setlinestyle(SOLID_LINE,0,0);
    settextstyle(8,HORIZ_DIR,2);

	readimagefile("img/map.gif",0,0,SW,SH);
    int size=imagesize(0,0,1240,620);
    p=malloc(size);
    getimage(0,0,1240,620,p);
    cleardevice();
   
	state::start();
    state::shuffle();
    state::display();
	cleardevice();
	//delay(200);getch();
	
	while(!menu())
	{readimagefile("img/howto.gif",0,0,SW,SH);
	 while(!ismouseclick(WM_LBUTTONDOWN));
	}



	game:
    state::display();
	delay(200);getch();
	while(state::total(AI)!=0 && state::total(0)!=0)
    {outtext("Your move...");
     state::reinforce(0,int(ceil(state::tps(0)/3)));
     state::attack(0);
	 if(state::total(AI)==0)break;
     cleardevice();
     state::display();state::display();
     outtext("Computer's move...");
	 delay(600);getch();
     state::AIreinf(AI);
     state::AIattack(AI);
	 getch();
    }
    end:
    cleardevice();
    if(state::total(0)==0)   {readimagefile("img/lose.jpg",0,0,1240,620);outtextxy(504,370,"\"In war there is no prize for the runner-up\"");outtextxy(804,400,"- Omar Bradley");}
    else                     {readimagefile("img/win.jpg" ,0,0,1240,620);}
    
	while(!kbhit()){delay(100);}
	return 0;
}

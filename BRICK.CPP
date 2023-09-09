/*-------------------------------------------------------------------------

		        Brick Game 

Programmer           : Aabha Jahagirdar
Date                 : 02/06/23

-------------------------------------------------------------------------------------*/

#include <conio.h>
#include <graphics.h>
#include <dos.h>
#include <stdlib.h>
#include <iostream.h>

#define MAXX 15
#define MAXY 7

#define TRUE 1
#define FALSE 0

#define BORDERX 509
#define BORDERY 440

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define EXIT 27
#define PAUSE 32

#define disX 2
#define disY 2

#define HIDE 0
#define DISPLAY 1
#define hideIt 0

#define maxStar 200

#define MOVELEFT 0
#define MOVERIGHT 1

void *buff;

struct ball{
	int xBall;
	int yBall;
	int radius;
	int color;
	int dx;
	int dy;
}pBall;
struct block{
	int xBlock;
	int yBlock;
	int height;
	int width;
	int color;
}pBlock;
struct brick{
	int layer;
	int xnum; /* Number of bricks horizontally */
	int ynum; /* Number of bricks vertically */
	int aBrick[MAXY][MAXX];
	int xBrick;
	int yBrick;
	int color;
	int height;
	int width;
}pBrick;

struct box{
	int xBox;
	int yBox;
	int height;
	int width;
	int color;
}pBox[3];


int starsX[maxStar];
int starsY[maxStar];
int sX=0,sY=0;
int level=0,chapter=0,life=4;
long int score=0;
char str[4][8]={"LEVEL","CHAPTER","SCORE","STATUS"};
int aData[3],gamepause=0,whichDirection[3];
int SPEED=0,enableSound=TRUE;

void mainMenu();

void drawBall()
{
	int x=600,y=50;

	setcolor(pBall.color);
	setfillstyle(SOLID_FILL,pBall.color);
	circle(x,y,pBall.radius);
	floodfill(x,y,pBall.color);

	buff=malloc(sizeof(imagesize(x-pBall.radius,y-pBall.radius,x+pBall.radius,y+pBall.radius)));
	getimage(x-pBall.radius,y-pBall.radius,x+pBall.radius,y+pBall.radius,buff);

	setfillstyle(SOLID_FILL,hideIt);
	bar(x-pBall.radius,y-pBall.radius,x+pBall.radius,y+pBall.radius);


}
void drawBlock(int status)
{
	if(status==DISPLAY)
	{

		setfillstyle(SOLID_FILL,pBlock.color);
		bar(pBlock.xBlock,pBlock.yBlock,pBlock.xBlock+pBlock.width,pBlock.yBlock+pBlock.height);
		setcolor(15);
		rectangle(pBlock.xBlock,pBlock.yBlock,pBlock.xBlock+pBlock.width,pBlock.yBlock+pBlock.height);

	}

	else
	{
		setfillstyle(SOLID_FILL,hideIt);
		bar(pBlock.xBlock,pBlock.yBlock,pBlock.xBlock+pBlock.width,pBlock.yBlock+pBlock.height);
	}
}
void drawBox(int status, int index)
{

	if(status==DISPLAY)
	{

		setfillstyle(SOLID_FILL,pBox[index].color);
		bar(pBox[index].xBox,pBox[index].yBox,pBox[index].xBox+pBox[index].width,pBox[index].yBox+pBox[index].height);
		setcolor(15);
		rectangle(pBox[index].xBox,pBox[index].yBox,pBox[index].xBox+pBox[index].width,pBox[index].yBox+pBox[index].height);

	}

	else
	{
		setfillstyle(SOLID_FILL,hideIt);
		bar(pBox[index].xBox,pBox[index].yBox,pBox[index].xBox+pBox[index].width,pBox[index].yBox+pBox[index].height);
	}

}
void drawBrick()
{
	int disx=0,disy=0;

	for(int row=0;row<pBrick.ynum;row++)
	{
		for(int col=0;col<pBrick.xnum;col++)
		{
			if(pBrick.aBrick[row][col]==1)
			{
				disx=col*disX;
				disy=row*disY;

				setfillstyle(SOLID_FILL,pBrick.color);
				bar(pBrick.xBrick+(pBrick.width*col)+disx,pBrick.yBrick+(pBrick.height*row)+disy,pBrick.xBrick+(pBrick.width*col)+pBrick.width+disx,pBrick.yBrick+(pBrick.height*row)+pBrick.height+disy);
				setcolor(15);
				rectangle(pBrick.xBrick+(pBrick.width*col)+disx,pBrick.yBrick+(pBrick.height*row)+disy,pBrick.xBrick+(pBrick.width*col)+pBrick.width+disx,pBrick.yBrick+(pBrick.height*row)+pBrick.height+disy);

			}
		}
	}
}

void Text3D(int x, int y, int depth, int bgcolor, int fgcolor, char *caption)
{
	int i;
	setcolor(bgcolor);
	for(i=0;i<depth;i++)
		outtextxy(x+i,y+i,caption);
	setcolor(fgcolor);
	outtextxy(x+i,y+i,caption);

}

void updateData()
{
	int x=BORDERX+5,y=10;
	int w=100,h=100;
	int d=10;
	char sData[20];


	/* Clear */

	for(int i=0;i<3;i++)
	{
		setcolor(0);
		outtextxy(x+w/2-textwidth(itoa(aData[i],sData,10))/2,y+60,itoa(aData[i],sData,10));
		y+=(h+d);
	}


	aData[0]=level;
	aData[1]=chapter;
	aData[2]=score;

	y=10;

	/* Update*/

	for(int _i=0;_i<3;_i++)
	{
		setcolor(15);
		outtextxy(x+w/2-textwidth(itoa(aData[_i],sData,10))/2,y+60,itoa(aData[_i],sData,10));
		y+=(h+d);
	}


}


void checkCollisionBrick(int x, int y)
{
	int isCollidee=0;
	int disx=0,disy=0;
	int x1Ball,y1Ball,x2Ball,y2Ball;
	int x1Brick,y1Brick,x2Brick,y2Brick;

	x1Ball=x;
	y1Ball=y;

	x2Ball=x+2*pBall.radius;
	y2Ball=y+2*pBall.radius;

	for(int row=0;row<pBrick.ynum;row++)
	{
		for(int col=0;col<pBrick.xnum;col++)
		{
			disx=col*disX;
			disy=row*disY;

			if(pBrick.aBrick[row][col]==1)
			{
				x1Brick=pBrick.xBrick+(pBrick.width*col)+disx;
				x2Brick=pBrick.xBrick+(pBrick.width*col)+pBrick.width+disx;

				y1Brick=pBrick.yBrick+(pBrick.height*row)+disy;
				y2Brick=pBrick.yBrick+(pBrick.height*row)+pBrick.height+disy;

				if(((x1Ball>=x1Brick&&x1Ball<=x2Brick)||(x2Ball>=x1Brick&&x2Ball<=x2Brick))&&y1Ball==y2Brick)
				{
					isCollidee=1;
					pBall.dy=1;
				}
				else if(((x1Ball>=x1Brick&&x1Ball<=x2Brick)||(x2Ball>=x1Brick&&x2Ball<=x2Brick))&&y2Ball==y1Brick)
				{
					isCollidee=1;
					pBall.dy=-1;
				}
				else if(((y1Ball>=y1Brick&&y1Ball<=y2Brick)||(y2Ball>=y1Brick&&y2Ball<=y2Brick))&&x1Ball==x2Brick)
				{
					isCollidee=1;
					pBall.dx=1;
				}
				else if(((y1Ball>=y1Brick&&y1Ball<=y2Brick)||(y2Ball>=y1Brick&&y2Ball<=y2Brick))&&x2Ball==x1Brick)
				{
					isCollidee=1;
					pBall.dx=-1;
				}



				if(isCollidee==1)
				{
					setfillstyle(SOLID_FILL,hideIt);
					bar(pBrick.xBrick+(pBrick.width*col)+disx,pBrick.yBrick+(pBrick.height*row)+disy,pBrick.xBrick+(pBrick.width*col)+pBrick.width+disx,pBrick.yBrick+(pBrick.height*row)+pBrick.height+disy);

					pBrick.aBrick[row][col]=0;
					if(enableSound==TRUE)
					{
						sound(1000);
						delay(20);
						nosound();
					}
					score+=level+chapter;
					updateData();
					return;
				}
			}
		}
	}
}


void checkCollisionBox(int x, int y, int index)
{

	int x1Ball,y1Ball,x2Ball,y2Ball;
	int x1Box,y1Box,x2Box,y2Box;
	int isCollide=0;

	x1Ball=x;
	y1Ball=y;

	x2Ball=x+2*pBall.radius;
	y2Ball=y+2*pBall.radius;


				x1Box=pBox[index].xBox;
				y1Box=pBox[index].yBox;

				x2Box=pBox[index].xBox+pBox[index].width;
				y2Box=pBox[index].yBox+pBox[index].height;

				if(((x1Ball>=x1Box&&x1Ball<=x2Box)||(x2Ball>=x1Box&&x2Ball<=x2Box))&&y1Ball==y2Box)
				{

					pBall.dy=1;
					 isCollide=1;
				}
				else if(((x1Ball>=x1Box&&x1Ball<=x2Box)||(x2Ball>=x1Box&&x2Ball<=x2Box))&&y2Ball==y1Box)
				{

					pBall.dy=-1;
					 isCollide=1;
				}
				else if(((y1Ball>=y1Box&&y1Ball<=y2Box)||(y2Ball>=y1Box&&y2Ball<=y2Box))&&x1Ball==x2Box)
				{

					pBall.dx=2;
					 isCollide=1;
				}
				else if(((y1Ball>=y1Box&&y1Ball<=y2Box)||(y2Ball>=y1Box&&y2Ball<=y2Box))&&x2Ball==x1Box)
				{

					pBall.dx=-2;
					 isCollide=1;
				}

		if(isCollide==1)
		{
			if(enableSound==TRUE)
			{
				sound(1000);
				delay(20);
				nosound();
			}
		}



}

void checkCollisionBlock(int x, int y)
{

	int x1Ball,y1Ball,x2Ball,y2Ball;
	int isCollide=0;

	x1Ball=x;
	y1Ball=y;

	x2Ball=x+2*pBall.radius;
	y2Ball=y+2*pBall.radius;


	if(((x1Ball>=pBlock.xBlock&&x1Ball<=pBlock.xBlock+pBlock.width)||(x2Ball>=pBlock.xBlock&&x2Ball<=pBlock.xBlock+pBlock.width)) && y2Ball==pBlock.yBlock)
	{
			pBall.dy=-1;
			isCollide=1;
	}
	else if(((y1Ball>=pBlock.yBlock&&y1Ball<=pBlock.yBlock+pBlock.height)||(y2Ball>=pBlock.yBlock&&y2Ball<=pBlock.yBlock+pBlock.height)) && x1Ball==pBlock.xBlock+pBlock.width)
	{
			pBall.dx=+1;
			isCollide=1;
	}
	else if(((y1Ball>=pBlock.yBlock&&y1Ball<=pBlock.yBlock+pBlock.height)||(y2Ball>=pBlock.yBlock&&y2Ball<=pBlock.yBlock+pBlock.height)) && x2Ball==pBlock.xBlock)
	{
			pBall.dx=+1;
			isCollide=1;
	}

	 if(isCollide==1)
	 {
		if(enableSound==TRUE)
		{
			sound(1000);
			delay(20);
			nosound();
		}
	 }

}

void InitBox(int x, int y, int height, int width, int direction, int index)
{
	pBox[index].height=height;
	pBox[index].width=width;
	pBox[index].xBox=x;
	pBox[index].yBox=y;
	pBox[index].color=12;
	whichDirection[index]=direction;
}

void Init()
{

	pBrick.xBrick=13;
	pBrick.yBrick=12;
	pBrick.color=3;
	pBrick.height=20;
	pBrick.width=31;

	pBlock.height=20;
	pBlock.width=64;
	pBlock.xBlock=220;
	pBlock.yBlock=BORDERY-pBlock.height-2;
	pBlock.color=10;

	InitBox(100,MAXY*pBrick.height+pBrick.yBrick+20,20,100,MOVELEFT,0);
	InitBox(100,MAXY*pBrick.height+pBrick.yBrick+60,20,100,MOVERIGHT,1);
	InitBox(300,MAXY*pBrick.height+pBrick.yBrick+100,20,100,MOVELEFT,2);



	pBall.xBall=pBlock.xBlock+30;
	pBall.yBall=pBlock.yBlock-40;
	pBall.radius=10;
	pBall.color=4;
	pBall.dx=1;
	pBall.dy=1;

	setcolor(14);
	rectangle(9,9,BORDERX+1,BORDERY+1);
	setcolor(12);
	rectangle(10,10,BORDERX,BORDERY);

	drawBall();

	/*---------------------------------------------------*/

	settextstyle(0,0,0);

	{
		int x=BORDERX+5,y=10;
		int w=100,h=100;
		int d=10;

		for(int i=0;i<4;i++)
		{
			setcolor(i+11);
			rectangle(x,y,x+w,y+h);
			setcolor(15);
			outtextxy(x+w/2-textwidth(str[i])/2,y+30,str[i]);
			y+=(h+d);
		}

		setcolor(15);
		outtextxy(BORDERX+5+50-textwidth("PLAYING...")/2,400,"PLAYING...");

	}

	/*---------------------------------------------------*/

	for(int g=0;g<maxStar;g++)
	{
		sX=random(getmaxx());
		sY=random(getmaxy());
		starsX[g]=sX;
		starsY[g]=sY;
		putpixel(sX,sY,random(16));
	}


}

void moveBox(int status, int index)
{
	if(status==DISPLAY)
	{
		if(whichDirection[index]==MOVELEFT)
		{
			pBox[index].xBox-=1;
			if(pBox[index].xBox<=12)
				whichDirection[index]=MOVERIGHT;
		}
		else if(whichDirection[index]==MOVERIGHT)
		{
			pBox[index].xBox+=1;
			if(pBox[index].xBox>=BORDERX-pBox[index].width-1)
				whichDirection[index]=MOVELEFT;
		}
	}

	drawBox(status,index);
}


/*-----------------------------------------------------------------*/

void JClogo()
{


  /*********** Logo ************/

    setcolor(3);
    setfillstyle(SOLID_FILL,3);
    circle(280,180,30);
    floodfill(280,180,3);

    setcolor(15);
    setfillstyle(SOLID_FILL,15);
    circle(280,180,27);
    circle(280,180,24);
    floodfill(254,180,15);

    settextstyle(1,0,4);
    outtextxy(265,160,"AJ");
  /******************************/

    setcolor(15);
    settextstyle(0,0,2);
    Text3D(200,240,2,3,11,"Aabha Jahagirdar");
    Text3D(220,290,2,3,11,"Presents");
    Text3D(70,390,2,3,11,"AabhaJahagirdar@gmail.com");

    settextstyle(0,0,0);

}

//***************************************************************
//   Starting Screen
//****************************************************************

void Start()
{


   settextstyle(0,0,2);
   Text3D(230,200,2,3,11,"Loading...");
   settextstyle(0,0,0);
   for(int i=0;i<1000;i++)
	putpixel(random(640),random(480),random(16));
   delay(4000);
   cleardevice();

   JClogo();
   for(int l=0;l<1000;l++)
	putpixel(random(640),random(480),random(16));

   delay(5000);
   cleardevice();

   settextstyle(0,0,2);
   Text3D(230,200,2,3,11,"a game by");
   Text3D(250,240,2,3,11,"Aabha Jahagirdar");
   settextstyle(0,0,0);
   for(int k=0;k<1000;k++)
	putpixel(random(640),random(480),random(16));

   delay(4000);
   cleardevice();

   settextstyle(0,0,7);
   Text3D(50,150,4,3,11,"BRICK GAME");
   settextstyle(0,0,2);
   settextstyle(0,0,0);
   for(int j=0;j<2000;j++)
	putpixel(random(640),random(480),random(16));
   delay(6000);
   cleardevice();


}

//***************************************************************
//   Introduction Function
//****************************************************************

void Intro()
{

   cleardevice();
   setcolor(15);
   settextstyle(0,0,4);
   for(int i=0;i<4;i++)
   outtextxy(260+i,30+i,"HELP");
   settextstyle(0,0,2);
   setcolor(4);
   for(int k=0;k<=3;k++)
   {
	outtextxy(100+k,100+30-k,"Left  Key  : Move Left");
	outtextxy(100+k,130+30-k,"Right Key  : Move Right");
	outtextxy(100+k,160+30-k,"Space      : Pause");
	outtextxy(100+k,190+30-k,"Esc        : Return");
	outtextxy(100+k,220+30-k,"S          : Pause Sound");


	outtextxy(100+k,380+30-k,"Press any key to continue...");
	if(k==2)
	setcolor(12);
   }
   for(int _i=0;_i<1000;_i++)
	putpixel(random(640),random(480),random(16));
   getch();
}

/*------------------------------------------------------------------*/




void InitBrick()
{
	for(int row=0;row<pBrick.ynum;row++)
	{
		for(int col=0;col<pBrick.xnum;col++)
		{
			pBrick.aBrick[row][col]=0;


		}
	}
}


void InitChapter1()
{

	chapter=1;
	InitBrick();

	for(int row=0;row<pBrick.ynum;row++)
	{
		for(int col=0;col<pBrick.xnum;col++)
		{
			pBrick.aBrick[row][col]=1;


		}
	}
}

void InitChapter2()
{


	int s=0;

	chapter=2;
	InitBrick();

	for(int row=0;row<pBrick.ynum;row++)
	{
		for(int col=s;col<pBrick.xnum-s;col++)
		{
			pBrick.aBrick[row][col]=1;


		}
		s++;
	}
}

void InitChapter3()
{


	int s=0;

	chapter=3;
	InitBrick();

	for(int row=pBrick.ynum-1;row>=0;row--)
	{
		for(int col=s;col<pBrick.xnum-s;col++)
		{
			pBrick.aBrick[row][col]=1;


		}
		s++;
	}
}

void InitChapter4()
{


	int s=1,half=(pBrick.ynum-1)/2;

	chapter=4;
	InitBrick();

	for(int row=half;row>=0;row--)
	{
		for(int col=s;col<pBrick.xnum-s;col++)
		{
			pBrick.aBrick[row][col]=1;


		}
		s++;
	}

	s=2;

	for(int _row=half+1;_row<pBrick.ynum;_row++)
	{
		for(int _col=s;_col<pBrick.xnum-s;_col++)
		{
			pBrick.aBrick[_row][_col]=1;


		}
		s++;
	}



}

void InitChapter5()
{


	int s=2,half=(pBrick.ynum-1)/2;

	chapter=5;
	InitBrick();

	for(int row=half;row>=0;row--)
	{
		for(int col=s;col<pBrick.xnum-s;col++)
		{
			pBrick.aBrick[row][col]=1;


		}
		s++;
	}

	s=3;

	for(int _row=half+1;_row<pBrick.ynum;_row++)
	{
		for(int _col=s;_col<pBrick.xnum-s;_col++)
		{
			pBrick.aBrick[_row][_col]=1;


		}
		s++;
	}



}

void InitChapter6()
{


	int s=2,half=(pBrick.ynum-1)/2;

	chapter=6;
	InitBrick();

	for(int row=half-1;row>=0;row--)
	{
		for(int col=s;col<pBrick.xnum-s;col++)
		{
			pBrick.aBrick[row][col]=1;


		}
		s++;
	}

	s=2;

	for(int _row=half+1;_row<pBrick.ynum;_row++)
	{
		for(int _col=s;_col<pBrick.xnum-s;_col++)
		{
			pBrick.aBrick[_row][_col]=1;


		}
		s++;
	}



}



void InitChapter7()
{

	chapter=7;
	int value=1;
	InitBrick();

	for(int row=0;row<pBrick.ynum;row++)
	{
		for(int col=0;col<pBrick.xnum;col++)
		{
			pBrick.aBrick[row][col]=value;


		}
		value=!value;
	}
}


void InitChapter8()
{

    chapter=8;
	int s=6;
	InitBrick();

	for(int row=0;row<pBrick.ynum;row++)
	{
		for(int col=0;col<(pBrick.xnum-1)/2-s;col++)
		{
			pBrick.aBrick[row][col]=1;


		}
		for(int _col=pBrick.xnum-1;_col>(pBrick.xnum-1)/2+s;_col--)
		{
			pBrick.aBrick[row][_col]=1;


		}

		s--;
	}
}

void InitChapter9()
{

	chapter=9;
	int s=0;
	InitBrick();

	for(int row=0;row<pBrick.ynum;row++)
	{
		for(int col=0;col<(pBrick.xnum-1)/2-s;col++)
		{
			pBrick.aBrick[row][col]=1;


		}
		for(int _col=pBrick.xnum-1;_col>(pBrick.xnum-1)/2+s;_col--)
		{
			pBrick.aBrick[row][_col]=1;


		}

		s++;
	}
}



void InitChapter10()
{


	int col=2,row=0,maxDown=4,i,j;
	int s=0;

	chapter=10;
	InitBrick();

	for(row=0;row<pBrick.ynum;row++)
		pBrick.aBrick[row][col]=1;


	for(i=1;i<=maxDown;i++)
		for(j=1;j<=i;j++)
			pBrick.aBrick[i-1][col+j]=1;

	for(i=maxDown;i>=1;i--)
	{
		for(j=1;j<=i;j++)
			pBrick.aBrick[i-1][col+maxDown+j+s]=1;
			s++;
	}

	for(row=0;row<pBrick.ynum;row++)
		pBrick.aBrick[row][col+maxDown+5]=1;








}



/*--------------------------------------------------------------*/


void chapterMenu()
{
	int _x=220,_y=50,_h=30;
	int cY=_y+2*_h;
	int choice=1;
	int key;
	cleardevice();
	do{

		settextstyle(0,0,4);
		Text3D(_x-70,_y,2,3,11,"CHAPTER MENU");
		settextstyle(0,0,2);
		Text3D(_x,_y+2*_h,1,3,11,"1.  Chapter 1");
		Text3D(_x,_y+3*_h,1,3,11,"2.  Chapter 2");
		Text3D(_x,_y+4*_h,1,3,11,"3.  Chapter 3");
		Text3D(_x,_y+5*_h,1,3,11,"4.  Chapter 4");
		Text3D(_x,_y+6*_h,1,3,11,"5.  Chapter 5");
		Text3D(_x,_y+7*_h,1,3,11,"6.  Chapter 6");
		Text3D(_x,_y+8*_h,1,3,11,"7.  Chapter 7");
		Text3D(_x,_y+9*_h,1,3,11,"8.  Chapter 8");
		Text3D(_x,_y+10*_h,1,3,11,"9.  Chapter 9");
		Text3D(_x,_y+11*_h,1,3,11,"10. Chapter 10");
		Text3D(_x,_y+12*_h,1,3,11,"11. Main Menu");
		for(int j=0;j<3000;j++)
			putpixel(random(640),random(480),random(16));


	       setcolor(12);
	       outtextxy(_x-50,cY,">>");

	    do{
				key=getch();


			  if(key==UP)
			  {
				setcolor(0);
				outtextxy(_x-50,cY,">>");

				cY-=_h;
				choice--;

				if(choice<1)
				{
					choice=11;
					cY =_y+12*_h;
				}

				setcolor(12);
				outtextxy(_x-50,cY,">>");
				if(enableSound==TRUE)
				{
					sound(1000);
					delay(20);
					nosound();
				}

			  }
			  else if(key==DOWN)
			  {
				setcolor(0);
				outtextxy(_x-50,cY,">>");
				cY+=_h;
				choice++;
				if(choice>11)
				{
					choice=1;
					cY=_y+2*_h;
				}

				setcolor(12);
				outtextxy(_x-50,cY,">>");
				if(enableSound==TRUE)
				{
					sound(1000);
					delay(20);
					nosound();
				}
			 }

		}while(key!=13);

			switch(choice)
			{
				case 1:
				chapter=1;
				choice=0;
				break;
				case 2:
				chapter=2;
				choice=0;
				break;
				case 3:
				chapter=3;
				choice=0;
				break;
				case 4:
				chapter=4;
				choice=0;
				break;
				case 5:
				chapter=5;
				choice=0;
				break;
				case 6:
				chapter=6;
				choice=0;
				break;
				case 7:
				chapter=7;
				choice=0;
				break;
				case 8:
				chapter=8;
				choice=0;
				break;
				case 9:
				chapter=9;
				choice=0;
				break;
				case 10:
				chapter=10;
				choice=0;
				break;
				case 11:
				mainMenu();
				break;

			}


	}while(choice!=0);
	cleardevice();
	
	score=0;
	life=4;
	enableSound=TRUE;
}	
void mainMenu()
{
	int _x=220,_y=50,_h=30;
	int cY=_y+2*_h;
	int choice=1;
	int key;
	cleardevice();
	do{

		settextstyle(0,0,4);
		Text3D(_x-70,_y,2,3,11,"MAIN MENU");
		settextstyle(0,0,2);
		Text3D(_x,_y+2*_h,1,3,11,"1.  Level 1");
		Text3D(_x,_y+3*_h,1,3,11,"2.  Level 2");
		Text3D(_x,_y+4*_h,1,3,11,"3.  Level 3");
		Text3D(_x,_y+5*_h,1,3,11,"4.  Level 4");
		Text3D(_x,_y+6*_h,1,3,11,"5.  Level 5");
		Text3D(_x,_y+7*_h,1,3,11,"6.  Level 6");
		Text3D(_x,_y+8*_h,1,3,11,"7.  Level 7");
		Text3D(_x,_y+9*_h,1,3,11,"8.  Level 8");
		Text3D(_x,_y+10*_h,1,3,11,"9.  Level 9");
		Text3D(_x,_y+11*_h,1,3,11,"10. Level 10");
		Text3D(_x,_y+12*_h,1,3,11,"11. Exit");
		for(int j=0;j<3000;j++)
			putpixel(random(640),random(480),random(16));


	       setcolor(12);
	       outtextxy(_x-50,cY,">>");

	    do{
				key=getch();


			  if(key==UP)
			  {
				setcolor(0);
				outtextxy(_x-50,cY,">>");

				cY-=_h;
				choice--;

				if(choice<1)
				{
					choice=11;
					cY =_y+12*_h;
				}

				setcolor(12);
				outtextxy(_x-50,cY,">>");
				if(enableSound==TRUE)
				{
					sound(1000);
					delay(20);
					nosound();
				}

			  }
			  else if(key==DOWN)
			  {
				setcolor(0);
				outtextxy(_x-50,cY,">>");
				cY+=_h;
				choice++;
				if(choice>11)
				{
					choice=1;
					cY=_y+2*_h;
				}

				setcolor(12);
				outtextxy(_x-50,cY,">>");
				if(enableSound==TRUE)
				{
					sound(1000);
					delay(20);
					nosound();
				}
			 }

		}while(key!=13);

			switch(choice)
			{
				case 1:
				SPEED=10;
				level=1;
				choice=0;
				break;
				case 2:
				SPEED=9;
				level=2;
				choice=0;
				break;
				case 3:
				SPEED=8;
				level=3;
				choice=0;
				break;
				case 4:
				SPEED=7;
				level=4;
				choice=0;
				break;
				case 5:
				SPEED=6;
				level=5;
				choice=0;
				break;
				case 6:
				SPEED=5;
				level=6;
				choice=0;
				break;
				case 7:
				SPEED=4;
				level=7;
				choice=0;
				break;
				case 8:
				SPEED=3;
				level=8;
				choice=0;
				break;
				case 9:
				SPEED=2;
				level=9;
				choice=0;
				break;
				case 10:
				SPEED=1;
				level=10;
				choice=0;
				break;
				case 11:
				closegraph();
				restorecrtmode();
				clrscr();
				cout<<"\n\n\t\t Thank You for playing Brick Game.";
				cout<<"\n\n\t- Aabha J.\n\n\n";
				exit(0);
				break;

			}


	}while(choice!=0);
	chapterMenu();
}

/*---------------------------------------------------------------*/

void SelectChapter()
{

	switch(chapter)
	{
		case 1:
		InitChapter1();
		break;
		case 2:
		InitChapter2();
		break;
		case 3:
		InitChapter3();
		break;
		case 4:
		InitChapter4();
		break;
		case 5:
		InitChapter5();
		break;
		case 6:
		InitChapter6();
		break;
		case 7:
		InitChapter7();
		break;
		case 8:
		InitChapter8();
		break;
		case 9:
		InitChapter9();
		break;
		case 10:
		InitChapter10();
		break;
		default:
		InitChapter1();
		break;
	}


}

void showLife()
{
	int x=100,y=BORDERY+10;
	outtextxy(x-60,y+5,"Life : ");
	for(int i=0;i<life;i++)
	{
		putimage(x,y,buff,XOR_PUT);
		x+=2*pBall.radius+10;
	}
}


void InitGame()
{
	Init();
	pBrick.xnum=MAXX;
	pBrick.ynum=MAXY;
	pBrick.layer=MAXY;

	SelectChapter();

	settextstyle(0,0,0);
	drawBrick();
	drawBlock(DISPLAY);
	if(chapter==5||chapter==6||chapter==7||chapter==10)
	drawBox(DISPLAY,0);
	if(chapter==6||chapter==7||chapter==8||chapter==9||chapter==10)
	drawBox(DISPLAY,1);
	if(chapter==4||chapter==8||chapter==9||chapter==10)
	drawBox(DISPLAY,2);
	updateData();
	showLife();
}

void GameOver()
{

	char buffer[20];

		cleardevice();





		settextstyle(0,0,7);
		Text3D(100,150,4,3,11,"Game Over");
		settextstyle(0,0,4);
		Text3D(180,300,2,3,11,"Score : ");
		Text3D(180+textwidth("Score : "),300,2,3,11,itoa(score,buffer,10));
		settextstyle(0,0,2);
		Text3D(100,400,2,3,11,"Press any key to continue...");
		settextstyle(0,0,0);
		for(int j=0;j<2000;j++)
			putpixel(random(640),random(480),random(16));

		getch();
		mainMenu();
		InitGame();





	}


void GameCompleted()
{

	char buffer[20];

		cleardevice();





		settextstyle(0,0,7);
		Text3D(100,150,4,3,11,"The End");
		settextstyle(0,0,4);
		Text3D(180,300,2,3,11,"Score : ");
		Text3D(180+textwidth("Score : "),300,2,3,11,itoa(score,buffer,10));
		settextstyle(0,0,2);
		Text3D(100,400,2,3,11,"Press any key to continue...");
		settextstyle(0,0,0);
		for(int j=0;j<2000;j++)
			putpixel(random(640),random(480),random(16));

		getch();
		mainMenu();
		InitGame();





	}

int chapterCompleted()
{

	for(int row=0;row<pBrick.ynum;row++)
	{
		for(int col=0;col<pBrick.xnum;col++)
		{
			if(pBrick.aBrick[row][col]==1)
			return FALSE;


		}
	}

	return TRUE;

}





void main()
{

	int gd=DETECT,gm,key;
	initgraph(&gd,&gm,"c://turboc3//bgi");
	int bcolor=0;

	Start();
	Intro();
	mainMenu();
	InitGame();
	life=4;

	while(1)
	{
		bcolor=random(16);
		for(int g=0;g<maxStar;g++)
			putpixel(starsX[g],starsY[g],bcolor);

		if(life==0)
		{
			GameOver();
		}

		if(chapterCompleted())
		{
			if(chapter==10)
				GameCompleted();
			delay(100);
			cleardevice();
			chapter++;
			InitGame();
		}

		if(gamepause)
		{
			setcolor(0);
			outtextxy(BORDERX+5+50-textwidth("PAUSED...")/2,400,"PAUSED...");
			setcolor(15);
			outtextxy(BORDERX+5+50-textwidth("PLAYING...")/2,400,"PLAYING...");
			gamepause=0;
			putimage(pBall.xBall,pBall.yBall,buff,XOR_PUT);
		}

		putimage(pBall.xBall,pBall.yBall,buff,XOR_PUT);
		delay(SPEED);
		putimage(pBall.xBall,pBall.yBall,buff,XOR_PUT);

		pBall.xBall += pBall.dx;
		pBall.yBall += pBall.dy;


		if(chapter==5||chapter==6||chapter==7||chapter==10)
		{
			moveBox(HIDE,0);
			moveBox(DISPLAY,0);
		}
		if(chapter==6||chapter==7||chapter==8||chapter==9||chapter==10)
		{
			moveBox(HIDE,1);
			moveBox(DISPLAY,1);
		}
		if(chapter==4||chapter==8||chapter==9||chapter==10)
		{
			moveBox(HIDE,2);
			moveBox(DISPLAY,2);
		}
			checkCollisionBrick(pBall.xBall,pBall.yBall);
			checkCollisionBlock(pBall.xBall,pBall.yBall);
		if(chapter==5||chapter==6||chapter==7||chapter==10)
			checkCollisionBox(pBall.xBall,pBall.yBall,0);
		if(chapter==6||chapter==7||chapter==8||chapter==9||chapter==10)
			checkCollisionBox(pBall.xBall,pBall.yBall,1);
		if(chapter==4||chapter==8||chapter==9||chapter==10)
			checkCollisionBox(pBall.xBall,pBall.yBall,2);

		if(pBall.xBall <=10)
		{
			pBall.xBall = 10;
			pBall.dx = 1;
			if(enableSound==TRUE)
			{
				sound(1000);
				delay(20);
				nosound();
			}
		}
		else if(pBall.xBall >= BORDERX-2*pBall.radius)
		{
			pBall.xBall = BORDERX-2*pBall.radius;
			pBall.dx = -1;
			if(enableSound==TRUE)
			{
				sound(1000);
				delay(20);
				nosound();
			}
		}

		if(pBall.yBall <=10)
		{
			pBall.yBall = 10;
			pBall.dy = 1;
			if(enableSound==TRUE)
			{
				sound(1000);
				delay(20);
				nosound();
			}
		}
		else if(pBall.yBall >= BORDERY-2*pBall.radius)
		{

			showLife();
			life--;
			showLife();
			pBall.xBall=pBlock.xBlock+20;
			pBall.yBall=pBlock.yBlock-50;
			pBall.dx=1;
			pBall.dy=1;
			sound(1000);
			delay(10);
			sound(700);
			delay(10);
			sound(2000);
			delay(10);
			sound(1000);
			delay(10);
			nosound();
			delay(100);

		}




		while(kbhit())
		{
			key=getch();

			if(key==LEFT&&pBlock.xBlock>=18)
			{
				drawBlock(HIDE); //Erase
				pBlock.xBlock-=16;
				drawBlock(DISPLAY);// Draw
			}
			else if(key==RIGHT&&pBlock.xBlock+pBlock.width<BORDERX-2)
			{

				drawBlock(HIDE);// Erase
				pBlock.xBlock+=16;
				drawBlock(DISPLAY);// Draw
			}
			else if(key==PAUSE)
			{

				if(gamepause==0)
				{

					setcolor(0);
					outtextxy(BORDERX+5+50-textwidth("PLAYING...")/2,400,"PLAYING...");
					setcolor(15);
					outtextxy(BORDERX+5+50-textwidth("PAUSED...")/2,400,"PAUSED...");
					gamepause=1;
					putimage(pBall.xBall,pBall.yBall,buff,XOR_PUT);
					getch();
				}



			}
			else if(key==EXIT)
			{
				GameOver();
			}
			else if(key=='S'||key=='s')
			{
				enableSound=!enableSound;
			}
		}

	}




}



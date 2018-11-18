#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include <math.h> 
#define deltax 0.03
#define deltay 0.03
#define maxline 100
#define maxbox 100
typedef struct{
	double x,y;
}pointT; 
typedef struct {
	pointT a;/*ֱ�ߵĶ˵�*/ 
	pointT b;
	double x1,x2,y1,y2; 
} *LineT;
static double k[maxline];
typedef struct{
	LineT line[maxline];
	int nlines;
}*LineDB;/*ֱ�����ݿ�*/ 

typedef struct {
	pointT a;
	pointT b;
	pointT c;
	pointT d;
	double x1,x2,y1,y2; 
} *BoxT;
typedef struct{
	BoxT box[maxbox];
	int nboxs;
}*BoxDB;/*�������ݿ�*/ 

typedef struct {
	pointT a;
	double rx;
	double ry;
	
} *CirT;
typedef struct{
	CirT cir[maxbox];
	int ncir;
}*CirDB;//Բ�����ݿ� 
typedef struct{
	string str;
	double textx;
	double texty;
}*Text;
typedef struct{
	Text textt[maxbox];
	int ntext;
    }*TextDB;  
pointT CreatePoint(double x,double y);
void maxormin(int i);
void maxorminbox(int i);
static LineDB db;
static BoxDB dbbox;
static CirDB dbcir; 
static TextDB dbtext; 
static int rec;
static int size;
static double ccx = 1.0, ccy = 1.0;/*Բ������*/
static double radius = 1.0; /*Բ�뾶*/
static int v=0.2;
static char text[100]; /*������ַ���������*/
static int textlen = 0;/*������ַ�������*/
static double textx, texty; /*�ַ�������ʼλ��*/
static bool isDisplayText = FALSE; /*�ַ�����ʾ��־*/
static bool isDisplayCircle = TRUE; /*Բ��ʾ��־*/
static bool isSelect=FALSE; 
static bool isDelete=FALSE; 
static bool isChange=FALSE;
//static bool isSelect;/*�ж��Ƿ�ѡ��*/ 
static bool ridselect=FALSE;
void DrawCenteredCircle(double x, double y, double r);/*������Բ*/
/*�жϵ�(x0,y0)�Ƿ��ھ��ΰ�Χ��(x1, y1) --> (x2, y2)��Χ��*/
bool inBox(double x0, double y0, double x1, double x2, double y1, double y2);

void KeyboardEventProcess(int key,int event);/*������Ϣ�ص�����*/
void CharEventProcess(char c);/*�ַ���Ϣ�ص�����*/
void MouseEventProcess(int x, int y, int button, int event);/*�����Ϣ�ص�����*/
void DrawGrid(double x, double y, double width, double height,
              int columns, int rows);
void DrawBox(double x, double y, double width, double height);
void DrawSelectPattern(double x1,double x2,double y1,double y2);/*��ѡ��ʱ��ͼ��*/
void Main() /*����ʼ��ִ��һ��*/
{    
     string a="choose pattern";
     string b="Text";
     string c="Move";
     string d="Change size";
     double m=0.2,n=0.5;
	 int i;
     InitGraphics(); 
     db=New(LineDB); 
     db->nlines=0;
	 dbbox=New(BoxDB); 
     dbbox->nboxs=0;
      dbcir=New(CirDB);
     dbcir->ncir=0;
     dbtext=New(TextDB);
	 dbtext->ntext=0;
	registerKeyboardEvent(KeyboardEventProcess);/*ע�������Ϣ�ص�����*/
	registerCharEvent(CharEventProcess);/*ע���ַ���Ϣ�ص�����*/
	registerMouseEvent(MouseEventProcess);/*ע�������Ϣ�ص�����*/

	SetPenColor("Black"); 
    SetPenSize(3);
    
    ccx = GetWindowWidth()/2;
    ccy = GetWindowHeight()/2;
    DrawCenteredCircle(ccx, ccy, radius);
    MovePen(0,0);
    DrawBox(0,GetWindowHeight()-0.5,1.2,0.5);
    MovePen(0.08,GetWindowHeight()-0.25);
    DrawTextString(a);
    DrawGrid(0,GetWindowHeight()-1.5,0.6,0.5,2,2);
    MovePen(0.05,GetWindowHeight()-0.55);
    DrawLine(0.4,-0.4);
    DrawBox(0.67,GetWindowHeight()-0.92,0.45,0.35);
    MovePen(0.05,GetWindowHeight()-1.25);
    DrawEllipticalArc(-0.25, -0.2, 0, 360);
    MovePen(0.69,GetWindowHeight()-1.3);
    DrawTextString(b);
    DrawGrid(0,0,0.3,0.5,4,1);
    DrawBox(0,GetWindowHeight()-2.0,1.2,0.5);
    MovePen(0.35,GetWindowHeight()-1.85);
    DrawTextString(c);
    DrawBox(0,GetWindowHeight()-2.5,1.2,0.5);
    MovePen(0.1,GetWindowHeight()-2.35);
    DrawTextString(d);
    for(i=1;i<=4;i++){
    	MovePen(m,0.05);
    	SetPenSize(n);
    	DrawLine(0,0.35);
    	m+=0.3;
    	n+=0.5;
		}
		SetPenColor("Red");                    
}
void DrawCenteredCircle(double x, double y, double r)
{
    MovePen(x + r, y);
    DrawArc(r, 0.0, 360.0);    

}

void CharEventProcess(char c)
{     int flag=0;
 	 static char str[2] = {0, 0};

     switch (c) {
       case '\r': 
	        dbtext->ntext++;/* ע�⣺�س������ﷵ�ص��ַ���'\r'������'\n'*/
		   isDisplayText = TRUE;/*�����ַ�����ʾ��־*/
           isSelect=TRUE;
		   textlen=0;
   	 	   break;
 	   case 27: 
	   case 8:/*ESC*/
 	       break;
      default:
	  	   str[0] = c;/*�γɵ�ǰ�ַ����ַ���*/
	 	   text[textlen++] = c;/*����ǰ�ַ����뵽�����ַ���������*/
	 	   text[textlen] = '\0';
	 	   MovePen(dbtext->textt[dbtext->ntext]->textx,dbtext->textt[dbtext->ntext]->texty);
	 	   dbtext->textt[dbtext->ntext]->str=text;
	 	  
	       DrawTextString(text);
	 	   break;
    }
}

bool inBox(double x0, double y0, double x1, double x2, double y1, double y2)
{
	return (x0 >= x1 && x0 <= x2 && y0 >= y1 && y0 <= y2);
}

void MouseEventProcess(int x, int y, int button, int event)
{
 	 static bool isMoveCircle = FALSE;/*�ƶ�Բ��־*/
 	 static bool isMoveText = FALSE; /*�ƶ��ı���־*/ 
 	 static bool isMoveLine=FALSE;
 	 static bool isMoveBox=FALSE;
	 static bool isChangeRadius = FALSE;/*�ı�Բ�뾶��־*/
	 static bool isDrawLine=FALSE;/*��ֱ��*/ 
	 static bool isDrawBox=FALSE;/*������*/ 
	 static bool isDrawEllipse=FALSE; /*����Բ*/  
	 static bool isDrawText=FALSE;    
	 static bool isChangeCircle = FALSE;/*�ƶ�Բ��־*/
 	 static bool isChangeText = FALSE; /*�ƶ��ı���־*/ 
 	 static bool isChangeLine=FALSE;
 	 static bool isChangeBox=FALSE;
     static double r = 0.2;
 	 static double omx = 0.0, omy = 0.0;
 	 static double comx=0.0,comy=0.0; /*��ͼ���*/ 
	 static double premx=0,premy=0;/*cad����Ӧ����*/ 
	 static double judmx=0,judmy=0;/*�ж��뻭�ĸ�ͼ��*/ 
	 static double jcmx=0.0,jcmy=0.0;
     static int flag=1;
     static int judflag=0;
     static bool isMove=FALSE;
    // static int judflagLine=0,judflagBox=0,judflagEllipse=0,judflagText=0;
	 static int nlines=0;
	 static int nboxs=0;
	 static int ncir=0; 
	 static int record=0;
	 static int mode=1;
     double mx, my;
     int i;
     static int nstr=0;
 	 mx = ScaleXInches(x);/*pixels --> inches*/
 	 my = ScaleYInches(y);/*pixels --> inches*/
     
 	 
  switch (event) {
         case BUTTON_DOWN:
            if (button == LEFT_BUTTON){
            	
               if(inBox(mx,my,0.0,1.2,0,0.5)){ /*ѡ�ʻ���ϸ*/ 
         	       SetPenColor("Black"); 
                   SetPenSize(3);
                   DrawGrid(0,0,0.3,0.5,4,1);
         	   if(inBox(mx,my,0,0.3,0,0.5)){
         		   SetPenColor("Red");
				   DrawBox(0,0,0.3,0.5) ;
				   SetPenColor("Black");
				   size=1;
			      } 
			   else if(inBox(mx,my,0.3,0.6,0,0.5)){
				       SetPenSize(3);
				       SetPenColor("Red");
				       DrawBox(0.3,0,0.3,0.5) ;
				       SetPenColor("Black");
				       size=2;
			      }
			  else if(inBox(mx,my,0.6,0.9,0,0.5)){
				  	  SetPenSize(3);
				  	  SetPenColor("Red");
					  DrawBox(0.6,0,0.3,0.5) ;
					  SetPenColor("Black");
					 size=3;
			      }
			  else if(inBox(mx,my,0.9,1.2,0,0.5)){
				  	SetPenSize(3);
				  	SetPenColor("Red");
					DrawBox(0.9,0,0.3,0.5) ;
					SetPenColor("Black");
					size=4;
			      }
		     }  /*ѡ�ʻ���ϸ*/ 
		       /*ѡ��ͼ�α�־*/ 

			  
         	if(inBox(mx,my,0,1.2,GetWindowHeight()-2.5,GetWindowHeight()-0.5)){
			          judflag=1;}
         	if(judflag){
			         judmx=mx;
			         judmy=my;
			         judflag=0;
		         }
		      if(inBox(judmx,judmy,0,1.2,GetWindowHeight()-2.5,GetWindowHeight()-0.5)){
		      	  SetPenSize(3);
				  SetPenColor("Black");
			      DrawGrid(0,GetWindowHeight()-1.5,0.6,0.5,2,2);
	          if(inBox(judmx,judmy,0,0.6,GetWindowHeight()-1.0,GetWindowHeight()-0.5)){
				  	isDrawLine=TRUE;
				  	LineT lin;
         	        lin=New(LineT);	
         	        lin->a=CreatePoint(mx,my);
         		    db->line[nlines]=lin;
				  	SetPenColor("Red");
					DrawBox(0,GetWindowHeight()-1,0.6,0.5) ;
				
				 } 
			  else if(inBox(judmx,judmy,0.601,1.2,GetWindowHeight()-1.0,GetWindowHeight()-0.5)){
			  	   
			  	   	isDrawBox=TRUE;
			  	    BoxT box;
         	        box=New(BoxT);	
         	        box->a=CreatePoint(mx,my);
         	        box->c.x=mx;box->d.y=my;
         		    dbbox->box[nboxs]=box;
				  	SetPenColor("Red");
					DrawBox(0.6,GetWindowHeight()-1,0.6,0.5) ;
					
			  }
			  else if(inBox(judmx,judmy,0,0.6,GetWindowHeight()-1.5,GetWindowHeight()-1.0)){
				  	 isDrawEllipse=TRUE;
				  	 SetPenColor("Red");
					 DrawBox(0,GetWindowHeight()-1.5,0.6,0.5) ;
					
					 CirT cir;
         	         cir=New(CirT);	
         	         cir->a=CreatePoint(mx,my);
         		     dbcir->cir[ncir]=cir;
					
			      } 
			else if(inBox(judmx,judmy,0.601,1.2,GetWindowHeight()-1.5,GetWindowHeight()-1.0)){
				 
				  isDrawText=TRUE;
				  
				  SetPenColor("Red");
		          DrawBox(0.6,GetWindowHeight()-1.5,0.6,0.5) ;
				  
					 Text P;
					 P=New(Text);
					 P->textx=mx;
					 P->texty=my;
					 dbtext->textt[dbtext->ntext]=P;
				
					 
					  
			} 
                       	SetPenColor("Black");
					SetPenSize(size);
			  if(inBox(judmx,judmy,0,1.2,GetWindowHeight()-2.0,GetWindowHeight()-1.5)) {
			  	 isDrawBox=FALSE;
			  	 isDrawEllipse=FALSE; 
			  	 isDrawLine=FALSE;
			  	 isDrawText=FALSE;
			  	 isMove=TRUE;
			  	 isChange=FALSE;
			  }
			  if(inBox(judmx,judmy,0,1.2,GetWindowHeight()-2.5,GetWindowHeight()-2.0)){
			  	isMove=FALSE;
			  	isChange=TRUE;
			  }
			  
			   }
			   
			   //ѡ�в��� +�ƶ�����+ɾ������ 
			   
			   
			   for(i=0;i<nlines;i++){
			   	  if(inBox(mx,my,db->line[i]->x1,db->line[i]->x2,db->line[i]->y1,db->line[i]->y2)){
			   	  	   jcmx=mx,jcmy=my;
			   	  	   if(isMove){
			   	  	   	isMoveLine=TRUE;
			   	  	    record=i;
			   	  	   }
			   	  	   
			   	  	   if(isChange){
			   	  	   	 if(inBox(jcmx,jcmy,db->line[i]->x1,db->line[i]->x2,
							db->line[i]->y1+0.5*(db->line[i]->y2-db->line[i]->y1),
							db->line[i]->y2)){
			   	  	   	    mode=1;
			   	  	   } else mode=2;
			   	  	   	 isChangeLine=TRUE;
			   	  	   	 record=i;
			   	  	   } 
					   
			   	  	   if(isDelete) {
			   	  	   	    SetEraseMode(TRUE);
			   	  	   	    MovePen(db->line[i]->a.x,db->line[i]->a.y);
			   	  	   	    DrawLine(db->line[i]->b.x-db->line[i]->a.x,db->line[i]->b.y-db->line[i]->a.y);
			   	  	   	    DrawSelectPattern(db->line[i]->x1,db->line[i]->x2,db->line[i]->y1,db->line[i]->y2);
			   	  	        db->line[i]->x1=0.0,db->line[i]->x2=0.0,db->line[i]->y1=0.0,db->line[i]->y2=0.0;
			   	  	        isDelete=!isDelete;
			   	  	        SetEraseMode(FALSE);
								 }
			   	  	  
			   	  	   else {
							DrawSelectPattern(db->line[i]->x1,db->line[i]->x2,db->line[i]->y1,db->line[i]->y2); 
			   	  	   }
							
					 }
		      }  
		       for(i=0;i<nboxs;i++){
			   	  if(inBox(mx,my,dbbox->box[i]->x1,dbbox->box[i]->x2,dbbox->box[i]->y1,dbbox->box[i]->y2)){
			   	  	       jcmx=mx,jcmy=my;
			   	  	      if(isMove){
			   	  	   	    isMoveBox=TRUE;
			   	  	        record=i;
			   	  	      }
			   	  	      
			   	  	      if(isChange){
							   if(inBox(jcmx,jcmy,dbbox->box[i]->x1,dbbox->box[i]->x1+0.5*(dbbox->box[i]->x2-dbbox->box[i]->x1),dbbox->box[i]->y1+0.5*(dbbox->box[i]->y2-dbbox->box[i]->y1),dbbox->box[i]->y2))
			   	  	   	        mode=1;//���� 
			   	  	           else 
			   	  	           if(inBox(jcmx,jcmy,dbbox->box[i]->x1+0.5*(dbbox->box[i]->x2-dbbox->box[i]->x1),dbbox->box[i]->x2,dbbox->box[i]->y1+0.5*(dbbox->box[i]->y2-dbbox->box[i]->y1),dbbox->box[i]->y2))
			                    mode=2;//����
								else  
								if(inBox(jcmx,jcmy,dbbox->box[i]->x1,dbbox->box[i]->x1+0.5*(dbbox->box[i]->x2-dbbox->box[i]->x1),dbbox->box[i]->y1,dbbox->box[i]->y1+0.5*(dbbox->box[i]->y2-dbbox->box[i]->y1)))
								mode=3; //���� 
								else mode=4;//���� 
								
			   	  	   	        isChangeBox=TRUE;
			   	  	   	        record=i;
			   	  	        }
			   	  	        
			   	  	        if(isDelete) {
			   	  	        	
			   	  	   	    SetEraseMode(TRUE);
			   	  	   	    DrawBox(dbbox->box[i]->x1,dbbox->box[i]->y1,dbbox->box[i]->x2-dbbox->box[i]->x1,dbbox->box[i]->y2-dbbox->box[i]->y1); 
			   	  	   	    DrawSelectPattern(dbbox->box[i]->x1,dbbox->box[i]->x2,dbbox->box[i]->y1,dbbox->box[i]->y2);
			   	  	        dbbox->box[i]->x1=0.0,dbbox->box[i]->x2=0.0,dbbox->box[i]->y1=0.0,dbbox->box[i]->y2=0.0;
			   	  	        isDelete=!isDelete;
			   	  	        SetEraseMode(FALSE);
								 }
							else 
							DrawSelectPattern(dbbox->box[i]->x1,dbbox->box[i]->x2,dbbox->box[i]->y1,dbbox->box[i]->y2);
							
			   	  	   }
			   	  	    
		      }
               for(i=0;i<ncir;i++)
			   if(inBox(mx,my,dbcir->cir[i]->a.x-dbcir->cir[i]->rx,dbcir->cir[i]->a.x+dbcir->cir[i]->rx,
					dbcir->cir[i]->a.y-dbcir->cir[i]->ry,dbcir->cir[i]->a.y+dbcir->cir[i]->ry)){ 
					   jcmx=mx,jcmy=my;
					   if(isMove){
					   	isMoveCircle=TRUE; 
					    record=i;
					   }
					   
					   if(isDelete){
					   
			   	    	 SetEraseMode(TRUE);
			   	    	 MovePen(dbcir->cir[i]->a.x+dbcir->cir[i]->rx,dbcir->cir[i]->a.y);
			   	    	 DrawEllipticalArc(dbcir->cir[i]->rx,dbcir->cir[i]->ry,0,360);
			   	    	 DrawSelectPattern(dbcir->cir[i]->a.x-dbcir->cir[i]->rx,dbcir->cir[i]->a.x+dbcir->cir[i]->rx,
					     dbcir->cir[i]->a.y-dbcir->cir[i]->ry,dbcir->cir[i]->a.y+dbcir->cir[i]->ry); 
					     dbcir->cir[i]->a.x=0.0, dbcir->cir[i]->a.y=0.0,dbcir->cir[i]->rx=0.0;
						 isDelete=!isDelete;
			   	  	     SetEraseMode(FALSE);
			   	    }
			   	    	
			   	    	else DrawSelectPattern(dbcir->cir[i]->a.x-dbcir->cir[i]->rx,dbcir->cir[i]->a.x+dbcir->cir[i]->rx,
					     dbcir->cir[i]->a.y-dbcir->cir[i]->ry,dbcir->cir[i]->a.y+dbcir->cir[i]->ry); 
					     
					     if(isChange){
							   if(inBox(jcmx,jcmy,dbcir->cir[i]->a.x-dbcir->cir[i]->rx,dbcir->cir[i]->a.x,dbcir->cir[i]->a.y,dbcir->cir[i]->a.y+dbcir->cir[i]->ry))
			   	  	   	       mode=1;//���� 
										
			   	  	           else if(inBox(jcmx,jcmy,dbcir->cir[i]->a.x,dbcir->cir[i]->a.x+dbcir->cir[i]->rx,dbcir->cir[i]->a.y,dbcir->cir[i]->a.y+dbcir->cir[i]->ry))
			                    mode=2;//����
								else  
								if(inBox(jcmx,jcmy,dbcir->cir[i]->a.x-dbcir->cir[i]->rx,dbcir->cir[i]->a.x,dbcir->cir[i]->a.y-dbcir->cir[i]->ry,dbcir->cir[i]->a.y))
								mode=3; //���� 
								else mode=4;//���� 
								
			   	  	   	        isChangeCircle=TRUE;
			   	  	   	        record=i;
			   	  	        }
			   	    }
			   	    
			   	  for(i=0;i<dbtext->ntext;i++)  
	                   if(inBox(mx,my,dbtext->textt[i]->textx,dbtext->textt[i]->textx+TextStringWidth(dbtext->textt[i]->str),
				               dbtext->textt[i]->texty,dbtext->textt[i]->texty+GetFontHeight())){	
				              rec=i;
							  if(isDelete){
			   	    	             SetEraseMode(TRUE);
							      MovePen(dbtext->textt[record]->textx,dbtext->textt[record]->texty);/*��ʼλ��*/
				                    DrawTextString(dbtext->textt[record]->str);
						        	SetEraseMode(FALSE);
            }
							
							if(isMove){
				                 	isMoveText=TRUE;
				                 	 record=i;
				                 }	   
				        }
					   
					   
					   	
        } 
			  else if (button == RIGHT_BUTTON) {
					  isChangeRadius = TRUE;
			      } 
			  
		      omx = mx; omy = my;
		      if(flag){
		        comx=mx; 
			    comy=my;
			    flag=0;
			} 
		      break;
              
    	 case BUTTON_DOUBLECLICK:
			  break;
         case BUTTON_UP:
  		 	  if (button == LEFT_BUTTON) {
  		 	  	 if(isDrawLine ==TRUE){
  		 	  	 	db->line[nlines]->b=CreatePoint(mx,my);
				  	maxormin(nlines);	
					nlines++;
  		 	  	 	db->nlines++;
  		 	  		isDrawLine = FALSE;
  		 	  	 }
  		 	  	 if(isDrawBox ==TRUE){
  		 	  	 	dbbox->box[nboxs]->b=CreatePoint(mx,my);
  		 	  	 	dbbox->box[nboxs]->c.y=my;
					dbbox->box[nboxs]->d.x=mx;
					maxorminbox(nboxs);
					nboxs++;
  		 	  	 	dbbox->nboxs++;
  		 	  		isDrawBox = FALSE;
  		 	  	 }
  		 	  	 
  		 	  	  if(isDrawEllipse ==TRUE){
  		 	  	 	dbcir->cir[ncir]->rx=fabs(mx - dbcir->cir[ncir]->a.x);
					dbcir->cir[ncir]->ry=fabs(my - dbcir->cir[ncir]->a.y);
					dbcir->cir[ncir]->a.x=mx;
					ncir++;
  		 	  	 	dbcir->ncir++;
					isDrawEllipse =FALSE;}
  		 	  	  if(isDrawBox ==TRUE){
  		 	  	  	isDrawBox=FALSE;
  		 	  	  }
					 isMoveCircle = FALSE;
 	                 isMoveText = FALSE;  
 	                 isMoveLine=FALSE;
 	                 isMoveBox=FALSE;
 	                 isMove=FALSE;
 	                 isChangeLine=FALSE;
 	                 isChangeCircle = FALSE;
 	                 isChangeText = FALSE; 
 	                 isChangeBox=FALSE;
						}
						
			  else if (button == RIGHT_BUTTON) {
  		 	  	isChangeRadius = FALSE;
  		 	  } 
              break;
         case MOUSEMOVE:
			 if(mx>1.3) {
			 	
			 //�Ծ��εĲ��� 
			  if (isMoveBox){
			  	SetEraseMode(TRUE);
			    DrawBox(dbbox->box[record]->x1,dbbox->box[record]->y1,dbbox->box[record]->x2-dbbox->box[record]->x1,dbbox->box[record]->y2-dbbox->box[record]->y1); 
			   	DrawSelectPattern(dbbox->box[record]->x1,dbbox->box[record]->x2,dbbox->box[record]->y1,dbbox->box[record]->y2);
			  	dbbox->box[record]->x1 += mx - omx;
			  	dbbox->box[record]->x2 += mx - omx;
			    
				dbbox->box[record]->y1+= my - omy;
				dbbox->box[record]->y2+= my - omy;

				omx = mx;
				omy = my;
				SetEraseMode(FALSE);
				DrawBox(dbbox->box[record]->x1,dbbox->box[record]->y1,dbbox->box[record]->x2-dbbox->box[record]->x1,dbbox->box[record]->y2-dbbox->box[record]->y1); 
			   	DrawSelectPattern(dbbox->box[record]->x1,dbbox->box[record]->x2,dbbox->box[record]->y1,dbbox->box[record]->y2);
				}
				if(isChangeBox) {
					if(mode==1){
						
						SetEraseMode(TRUE);
			            DrawBox(dbbox->box[record]->x1,dbbox->box[record]->y1,dbbox->box[record]->x2-dbbox->box[record]->x1,dbbox->box[record]->y2-dbbox->box[record]->y1); 
			   	        DrawSelectPattern(dbbox->box[record]->x1,dbbox->box[record]->x2,dbbox->box[record]->y1,dbbox->box[record]->y2);
			  	        dbbox->box[record]->x1+= mx - omx;
				        dbbox->box[record]->y2+= my - omy;
				        omx = mx;
				        omy = my;
				        SetEraseMode(FALSE);
				        DrawBox(dbbox->box[record]->x1,dbbox->box[record]->y1,dbbox->box[record]->x2-dbbox->box[record]->x1,dbbox->box[record]->y2-dbbox->box[record]->y1); 
			   	        //DrawSelectPattern(dbbox->box[record]->x1,dbbox->box[record]->x2,dbbox->box[record]->y1,dbbox->box[record]->y2);
					}
					else if(mode==2){
						SetEraseMode(TRUE);
			            DrawBox(dbbox->box[record]->x1,dbbox->box[record]->y1,dbbox->box[record]->x2-dbbox->box[record]->x1,dbbox->box[record]->y2-dbbox->box[record]->y1); 
			   	        DrawSelectPattern(dbbox->box[record]->x1,dbbox->box[record]->x2,dbbox->box[record]->y1,dbbox->box[record]->y2);
			  	        dbbox->box[record]->x2+= mx - omx;
				        dbbox->box[record]->y2+= my - omy;
				        omx = mx;
				        omy = my;
				        SetEraseMode(FALSE);
				        DrawBox(dbbox->box[record]->x1,dbbox->box[record]->y1,dbbox->box[record]->x2-dbbox->box[record]->x1,dbbox->box[record]->y2-dbbox->box[record]->y1); 
						
					}
					else if(mode==3){
						SetEraseMode(TRUE);
			            DrawBox(dbbox->box[record]->x1,dbbox->box[record]->y1,dbbox->box[record]->x2-dbbox->box[record]->x1,dbbox->box[record]->y2-dbbox->box[record]->y1); 
			   	        DrawSelectPattern(dbbox->box[record]->x1,dbbox->box[record]->x2,dbbox->box[record]->y1,dbbox->box[record]->y2);
			  	        dbbox->box[record]->x1+= mx - omx;
				        dbbox->box[record]->y1+= my - omy;
				        omx = mx;
				        omy = my;
				        SetEraseMode(FALSE);
				        DrawBox(dbbox->box[record]->x1,dbbox->box[record]->y1,dbbox->box[record]->x2-dbbox->box[record]->x1,dbbox->box[record]->y2-dbbox->box[record]->y1); 
					}
					else {
						SetEraseMode(TRUE);
			            DrawBox(dbbox->box[record]->x1,dbbox->box[record]->y1,dbbox->box[record]->x2-dbbox->box[record]->x1,dbbox->box[record]->y2-dbbox->box[record]->y1); 
			   	        DrawSelectPattern(dbbox->box[record]->x1,dbbox->box[record]->x2,dbbox->box[record]->y1,dbbox->box[record]->y2);
			  	        dbbox->box[record]->x2+= mx - omx;
				        dbbox->box[record]->y1+= my - omy;
				        omx = mx;
				        omy = my;
				        SetEraseMode(FALSE);
				        DrawBox(dbbox->box[record]->x1,dbbox->box[record]->y1,dbbox->box[record]->x2-dbbox->box[record]->x1,dbbox->box[record]->y2-dbbox->box[record]->y1); 
						
					}
				}
				
				//��ֱ�ߵĲ��� 
				if(isMoveLine){
					SetEraseMode(TRUE);
					MovePen(db->line[record]->a.x,db->line[record]->a.y);
			   	  	DrawLine(db->line[record]->b.x-db->line[record]->a.x,db->line[record]->b.y-db->line[record]->a.y);
			   	  	DrawSelectPattern(db->line[record]->x1,db->line[record]->x2,db->line[record]->y1,db->line[record]->y2);
			   	  	db->line[record]->x1 += mx - omx;
				    db->line[record]->x2+= mx - omx;
				    db->line[record]->a.x+= mx - omx;
				    db->line[record]->b.x+= mx - omx;
					db->line[record]->y1+= my - omy;
					db->line[record]->y2+= my - omy;
					db->line[record]->a.y+= my - omy;
					db->line[record]->b.y+= my - omy;
					omx = mx;
				    omy = my;
				    SetEraseMode(FALSE);
				    MovePen(db->line[record]->a.x,db->line[record]->a.y);
			   	  	DrawLine(db->line[record]->b.x-db->line[record]->a.x,db->line[record]->b.y-db->line[record]->a.y);
			   	  	DrawSelectPattern(db->line[record]->x1,db->line[record]->x2,db->line[record]->y1,db->line[record]->y2);
					
				}
				if(isChangeLine){
			  	  if(mode==1){
			  	  	SetEraseMode(TRUE);
					MovePen(db->line[record]->a.x,db->line[record]->a.y);
			   	  	DrawLine(db->line[record]->b.x-db->line[record]->a.x,db->line[record]->b.y-db->line[record]->a.y);
			   	  	DrawSelectPattern(db->line[record]->x1,db->line[record]->x2,db->line[record]->y1,db->line[record]->y2);
			   	  	db->line[record]->x1+= mx - omx;
				    db->line[record]->a.x+= mx - omx;
					db->line[record]->y1+= my - omy;
					db->line[record]->a.y+= my - omy;
					maxormin(record);
					omx = mx;
				    omy = my;
				    SetEraseMode(FALSE);
				    MovePen(db->line[record]->a.x,db->line[record]->a.y);
			   	  	DrawLine(db->line[record]->b.x-db->line[record]->a.x,db->line[record]->b.y-db->line[record]->a.y);
			   	  	DrawSelectPattern(db->line[record]->x1,db->line[record]->x2,db->line[record]->y1,db->line[record]->y2);
			  	  }
			  	  else {
			  	  	SetEraseMode(TRUE);
					MovePen(db->line[record]->a.x,db->line[record]->a.y);
			   	  	DrawLine(db->line[record]->b.x-db->line[record]->a.x,db->line[record]->b.y-db->line[record]->a.y);
			   	  	DrawSelectPattern(db->line[record]->x1,db->line[record]->x2,db->line[record]->y1,db->line[record]->y2);
			   	  	db->line[record]->x2+= mx - omx;
			        db->line[record]->b.x+= mx - omx;
				    db->line[record]->y2+= my - omy;
				    db->line[record]->b.y+= my - omy;
				    maxormin(record);
					omx = mx;
				    omy = my;
				    SetEraseMode(FALSE);
				    MovePen(db->line[record]->a.x,db->line[record]->a.y);
			   	  	DrawLine(db->line[record]->b.x-db->line[record]->a.x,db->line[record]->b.y-db->line[record]->a.y);
			   	  	DrawSelectPattern(db->line[record]->x1,db->line[record]->x2,db->line[record]->y1,db->line[record]->y2);
			  	  }
			  }
			  //��Բ�Ĳ��� 
				if(isMoveCircle){
					SetEraseMode(TRUE);
			   	    MovePen(dbcir->cir[record]->a.x+dbcir->cir[record]->rx,dbcir->cir[record]->a.y);
			   	    DrawEllipticalArc(dbcir->cir[record]->rx,dbcir->cir[record]->ry,0,360);
			   	    DrawSelectPattern(dbcir->cir[record]->a.x-dbcir->cir[record]->rx,dbcir->cir[record]->a.x+dbcir->cir[record]->rx,
					dbcir->cir[record]->a.y-dbcir->cir[record]->ry,dbcir->cir[record]->a.y+dbcir->cir[record]->ry); 
					dbcir->cir[record]->a.x+= mx - omx;
				
					dbcir->cir[record]->a.y+= my - omy;
				    omx = mx;
				    omy = my;
				    SetEraseMode(FALSE);
				    MovePen(dbcir->cir[record]->a.x+dbcir->cir[record]->rx,dbcir->cir[record]->a.y);
			   	    DrawEllipticalArc(dbcir->cir[record]->rx,dbcir->cir[record]->ry,0,360);
			   	    DrawSelectPattern(dbcir->cir[record]->a.x-dbcir->cir[record]->rx,dbcir->cir[record]->a.x+dbcir->cir[record]->rx,
					dbcir->cir[record]->a.y-dbcir->cir[record]->ry,dbcir->cir[record]->a.y+dbcir->cir[record]->ry);
				}
				   if(isChangeCircle) {
						SetEraseMode(TRUE);
						MovePen(dbcir->cir[record]->a.x+dbcir->cir[record]->rx,dbcir->cir[record]->a.y);
						DrawEllipticalArc(dbcir->cir[record]->rx,dbcir->cir[record]->ry,0,360);
						DrawSelectPattern(dbcir->cir[record]->a.x-dbcir->cir[record]->rx,dbcir->cir[record]->a.x+dbcir->cir[record]->rx,
					   dbcir->cir[record]->a.y-dbcir->cir[record]->ry,dbcir->cir[record]->a.y+dbcir->cir[record]->ry);
				         SetEraseMode(FALSE);
				         if(mode==1){
				         dbcir->cir[record]->a.x+= (mx - omx)*0.5;
					     dbcir->cir[record]->a.y+= (my - omy)*0.5;
					     dbcir->cir[record]->rx -=  (mx - omx)*0.5;
					     dbcir->cir[record]->ry+= (my - omy)*0.5;
			        	MovePen(dbcir->cir[record]->a.x+dbcir->cir[record]->rx,dbcir->cir[record]->a.y);
				           DrawEllipticalArc(dbcir->cir[record]->rx,dbcir->cir[record]->ry,0,360);
				           }
				           
				           
				           if(mode==2){
					
				         dbcir->cir[record]->a.x+= (mx - omx)*0.5;
					     dbcir->cir[record]->a.y+= (my - omy)*0.5;
					     dbcir->cir[record]->rx +=  (mx - omx)*0.5;
					     dbcir->cir[record]->ry+= (my - omy)*0.5;
		                 MovePen(dbcir->cir[record]->a.x+dbcir->cir[record]->rx,dbcir->cir[record]->a.y);
				           DrawEllipticalArc(dbcir->cir[record]->rx,dbcir->cir[record]->ry,0,360);
				        }
				           
				            if(mode==3){
					
				         dbcir->cir[record]->a.x+= (mx - omx)*0.5;
					     dbcir->cir[record]->a.y+= (my - omy)*0.5;
					     dbcir->cir[record]->rx -=  (mx - omx)*0.5;
					     dbcir->cir[record]->ry -= (my - omy)*0.5;
		                 MovePen(dbcir->cir[record]->a.x+dbcir->cir[record]->rx,dbcir->cir[record]->a.y);
				           DrawEllipticalArc(dbcir->cir[record]->rx,dbcir->cir[record]->ry,0,360);
				        }
				         if(mode==4){
					
				         dbcir->cir[record]->a.x+= (mx - omx)*0.5;
					     dbcir->cir[record]->a.y+= (my - omy)*0.5;
					     dbcir->cir[record]->rx +=  (mx - omx)*0.5;
					     dbcir->cir[record]->ry -= (my - omy)*0.5;
		                 MovePen(dbcir->cir[record]->a.x+dbcir->cir[record]->rx,dbcir->cir[record]->a.y);
				           DrawEllipticalArc(dbcir->cir[record]->rx,dbcir->cir[record]->ry,0,360);
				        }
				           
				           
				
						omx = mx;
				           omy = my;	}
			
			
			
			}
				
				
				
				//�ı��ƶ�
				 if(isMoveText) {
				               
								
					        SetEraseMode(TRUE);
	          		         MovePen(dbtext->textt[record]->textx,dbtext->textt[record]->texty);/*��ʼλ��*/
				             DrawTextString(dbtext->textt[record]->str);
	 				         dbtext->textt[record]->textx += mx - omx;
					         dbtext->textt[record]->texty += my - omy;
					         omx = mx;
					         omy = my;				
					         SetEraseMode(FALSE);
					         MovePen(dbtext->textt[record]->textx, dbtext->textt[record]->texty);/*��ʼλ��*/
					         DrawTextString(dbtext->textt[record]->str);
				              break;	   
									   }
				                  
			 //����ͼ�� 
			 if(isDrawLine) {
			  	MovePen(comx,comy);
			    SetEraseMode(TRUE);
				DrawLine(premx-comx,premy-comy);
				MovePen(comx,comy);
			    SetEraseMode(FALSE);
			  	DrawLine(mx-comx,my-comy);
			    premx=mx;premy=my;
			  }flag=1;
			  
			  
			  if(isDrawBox){
		        SetEraseMode(TRUE);
				DrawBox(comx,comy,premx-comx,premy-comy);
			    SetEraseMode(FALSE);
			  	DrawBox(comx,comy,mx-comx,my-comy);
			    premx=mx;premy=my;
			  } flag=1;
			  if(isDrawEllipse&&(2*mx-comx)>1.3){
			  	MovePen(comx,comy);
			    SetEraseMode(TRUE);
				DrawEllipticalArc(-premx+comx,-premy+comy, 0, 360);
				MovePen(comx,comy);
			    SetEraseMode(FALSE);
			  	DrawEllipticalArc(-mx+comx,-my+comy, 0, 360);
			    premx=mx;premy=my;
			  }flag=1;
		      
			    
			  
			  
             
     }	
}
void KeyboardEventProcess(int key,int event)/*ÿ������������Ϣ����Ҫִ��*/
{
 	 double oldradius;
 	 
     switch (event) {
	 	case KEY_DOWN:
			 switch (key) {
			   case VK_UP:
			
		
			v=GetPointSize();
			SetEraseMode(TRUE);
			MovePen(dbtext->textt[rec]->textx,dbtext->textt[rec]->texty);/*��ʼλ��*/
		    DrawTextString(dbtext->textt[rec]->str);
		    SetEraseMode(FALSE);
			v+=10;
			SetPointSize(v);
			
			MovePen(dbtext->textt[rec]->textx,dbtext->textt[rec]->texty);/*��ʼλ��*/
		    DrawTextString(dbtext->textt[rec]->str);
			 break;
	case VK_DOWN:
			
			v=GetPointSize();
			SetEraseMode(TRUE);
				MovePen(dbtext->textt[rec]->textx,dbtext->textt[rec]->texty);/*��ʼλ��*/
		    DrawTextString(dbtext->textt[rec]->str);
		    SetEraseMode(FALSE);
			v-=10;
			SetPointSize(v);
			
			MovePen(dbtext->textt[rec]->textx,dbtext->textt[rec]->texty);/*��ʼλ��*/
		    DrawTextString(dbtext->textt[rec]->str);
			 break;
			     case VK_LEFT:
			         SetEraseMode(TRUE);
                     DrawCenteredCircle(ccx, ccy, radius);
					 ccx -= deltax;
					 SetEraseMode(FALSE);
                     DrawCenteredCircle(ccx, ccy, radius);
                     break;
			     case VK_RIGHT:
			         SetEraseMode(TRUE);
                     DrawCenteredCircle(ccx, ccy, radius);
					 ccx += deltax;
					 SetEraseMode(FALSE);
                     DrawCenteredCircle(ccx, ccy, radius);
                     break;
			     case VK_F1:
  					 SetEraseMode(TRUE);
                     DrawCenteredCircle(ccx, ccy, radius);
		 	         SetPenSize(GetPenSize()+100000);
					 SetEraseMode(FALSE);
                     DrawCenteredCircle(ccx, ccy, radius);
					 break;
			     case VK_F2:
  					 SetEraseMode(TRUE);
                     DrawCenteredCircle(ccx, ccy, radius);
		 	         SetPenSize(GetPenSize()-1);
					 SetEraseMode(FALSE);
                     DrawCenteredCircle(ccx, ccy, radius);
                     break;
			     case VK_F3:
			     case VK_PRIOR:
	 	     		 SetEraseMode(TRUE);
                     DrawCenteredCircle(ccx, ccy, radius);
                     radius += 0.1;
					 SetEraseMode(FALSE);
                     DrawCenteredCircle(ccx, ccy, radius);
                     break;
			     case VK_F4:
			     case VK_NEXT:
		 	         SetEraseMode(TRUE);
                     DrawCenteredCircle(ccx, ccy, radius);
                     radius -= 0.1;
					 SetEraseMode(FALSE);
                     DrawCenteredCircle(ccx, ccy, radius);
                     break;
			     case VK_F5:
                     break;
			     case VK_F6:
                     break;
			     case VK_F9:
			         InitConsole();
			         oldradius = radius;
			         printf("Input radius: ");
			         radius = GetReal();
			         FreeConsole();
  					 SetEraseMode(TRUE);
                     DrawCenteredCircle(ccx, ccy, oldradius);
					 SetEraseMode(FALSE);
                     DrawCenteredCircle(ccx, ccy, radius);
                     break;
                case VK_BACK :
                case VK_DELETE:
                	  isDelete=TRUE;
                	  break;
			 }
			 break;
		case KEY_UP:
			 break;
	 }	 
}
void DrawSelectPattern(double x1,double x2,double y1,double y2){
	SetPenColor("Black"); 
    SetPenSize(0.1);
    
    	DrawCenteredCircle(x1, y1, 0.03);
	    DrawCenteredCircle(x2, y2, 0.03);
	    DrawCenteredCircle(x1, y2, 0.03);
	    DrawCenteredCircle(x2, y1, 0.03);
    
	
} 
void DrawGrid(double x, double y, double width, double height,
              int columns, int rows)
{
    int i, j;

    for (i = 0; i < columns; i++) {
        for (j = 0; j < rows; j++) {
            DrawBox(x + i * width, y + j * height,
                    width, height);
        }
    }
}
void DrawBox(double x, double y, double width, double height)
{ 
    MovePen(x, y);
    DrawLine(0, height);
    DrawLine(width, 0);
    DrawLine(0, -height);
    DrawLine(-width, 0);
}
pointT CreatePoint(double x,double y){
	pointT p;
	p.x=x;
	p.y=y;
	return (p);
}
void maxormin(int i){
	  int nlines=i;
	  if(db->line[nlines]->a.x<db->line[nlines]->b.x){
		  db->line[nlines]->x1=db->line[nlines]->a.x;
		  db->line[nlines]->x2=db->line[nlines]->b.x;
		} 
	 else {
	     db->line[nlines]->x1=db->line[nlines]->b.x;
		 db->line[nlines]->x2=db->line[nlines]->a.x;
	}
	if(db->line[nlines]->a.y<db->line[nlines]->b.y){
		 db->line[nlines]->y1=db->line[nlines]->a.y;
		 db->line[nlines]->y2=db->line[nlines]->b.y;
		} 
	else {
	     db->line[nlines]->y1=db->line[nlines]->b.y;
		 db->line[nlines]->y2=db->line[nlines]->a.y;
	}
}
void maxorminbox(int i){
	  if(dbbox->box[i]->a.x<dbbox->box[i]->b.x){
		  dbbox->box[i]->x1=dbbox->box[i]->a.x;
		  dbbox->box[i]->x2=dbbox->box[i]->b.x;
		} 
	 else {
	     dbbox->box[i]->x1=dbbox->box[i]->b.x;
		 dbbox->box[i]->x2=dbbox->box[i]->a.x;
	}
	if(dbbox->box[i]->a.y<dbbox->box[i]->b.y){
		 dbbox->box[i]->y1=dbbox->box[i]->a.y;
		 dbbox->box[i]->y2=dbbox->box[i]->b.y;
		} 
	else {
	     dbbox->box[i]->y1=dbbox->box[i]->b.y;
		 dbbox->box[i]->y2=dbbox->box[i]->a.y;
	}
}


#include "sgl.h"
#include "sddrvs.dat"
#include "sgl_cd.h"

#define        PRESSED_PAD_UP                0xEFFF
#define        PRESSED_PAD_DOWN              0xDFFF
#define        PRESSED_PAD_LEFT              0xBFFF
#define        PRESSED_PAD_RIGHT             0x7FFF
#define        PRESSED_PAD_A                 0xFBFF
#define        PRESSED_PAD_B                 0xFEFF
#define        PRESSED_PAD_C                 0xFDFF
#define        PRESSED_PAD_X                 0xFFBF
#define        PRESSED_PAD_Z                 0xFFEF
#define        PRESSED_PAD_L                 0xFFF7
#define        PRESSED_PAD_R                 0xFF7F
#define        PRESSED_PAD_START             0xF7FF
#define        PRESSED_PAD_NOTHING           0xFFFF



#define        FPS_DIV        1
#define        FPS            (60 / FPS_DIV)

#define GOUR_REAL_MAX   737 
#define    GRaddr    0x1c38 //0xe000
 
/****************************************************************************/
#define	MAX_FILE  256
/****************************************************************************/

Uint8	readbuf[ (CDBUF_FORM1 * 2) / sizeof(Uint8) ];
Sint32	dirwork[SLCD_WORK_SIZE(MAX_FILE)];
//Uint8	readbuf[900];
Uint8 fartcnt = 0;
Uint8 audiobuf[4096];
Uint8 audiobuf2[2048];
Uint16 overlay[3264];
//Uint16	cov[384];
    
/****************************************************************************/

GOURAUDTBL       gourRealMax[GOUR_REAL_MAX];
Uint8            vwork[GOUR_REAL_MAX];
Uint8 panel = 0;

extern void display_string(char* str,int x);

extern Uint16 texture_textures001[];

extern XPDATA	xpdata_0[];
extern OBJECT object_0[];
extern Uint8 events[];
extern Uint8 welcome[];
extern Uint32 welcome_size;
extern PCM welcome_dat ;

extern PDATA PD_PLANE1;
extern PDATA PD_PLANE2;
extern PDATA PD_PLANE3;
extern PDATA PD_PLANE4;
extern TEXTURE  tex_textures[];
extern PICTURE pic_textures[];

extern pos0_drawn;
extern pos1_drawn;
extern pos2_drawn;

int *test;

 Uint8 ru;
 Uint8 gu;
 Uint8 bu;
 Uint16 final[1];
 Uint8 ra;
 Uint8 ga;
 Uint8 ba;

 Uint32 text_frames = 0;  
 Uint8 stepper = 0;
 

//sounds

char *clap = NULL;
Uint32 clap_size;
PCM clap_dat = {(_Mono | _PCM8Bit) , 0 , 127 , 0 , 0x7800 , 0 , 0 , 0 , 0} ;

char *jumpd = NULL;
Uint32 jump_size;
PCM jump_dat = {(_Mono | _PCM8Bit) , 0 , 127 , 0 , 0x7800 , 0 , 0 , 0 , 0} ;

char *fart = NULL;
Uint32 fart_size;
PCM fart_dat = {(_Mono | _PCM8Bit) , 0 , 127 , 0 , 0x7800 , 0 , 0 , 0 , 0} ;

    typedef struct cam{
        FIXED pos [XYZ];
        FIXED target[XYZ];
        ANGLE ang[XYZ];
    } CAMERA;
    
static CAMERA cmbuf;
static ANGLE tmp = DEGtoANG(0.0);

FIXED perupos[XYZS];
FIXED mpos[XYZS];
FIXED spos[XYZS];
FIXED info[XYZS];
FIXED dialog[XYZS];
FIXED tanu[XYZS];
FIXED fun[XYZS];
static ANGLE ang[XYZ];
static FIXED pos[XYZ];
static FIXED light[XYZ];

Uint8 madame = 0;
extern SPR_ATTR attr[];

void init_sound(){
    
    char sound_map[] = {0xff , 0xff} ;
    slInitSound(sddrvstsk , sizeof(sddrvstsk) , (Uint8 *)sound_map , sizeof(sound_map)) ;
}

void clearScreen()
{
     int i, j;

     for(i = 0; i < 30; i++)
     {
          for(j = 0; j < 40; j++)
          {
               slPrint(" ", slLocate(j, i));
          }
     }
}



void checkExitHere(Uint16 data)
{
    // check exit
    if(((data & PER_DGT_ST) == 0) && ((data & PER_DGT_TA) == 0) && ((data & PER_DGT_TB) == 0) && ((data & PER_DGT_TC) == 0)) {
        // return to demo demo loader
        // or activate multiplayer
        SYS_Exit(0);
    }
}


Uint16 frame;
FIXED jump;
FIXED speed = 0.0;
FIXED time;
Uint8 jumped = 0;
Uint8 stopped = 0;
Uint16 angle = 90;
Uint16 rota = 0;
Uint8 readyToPop = 0;
//slSin

    CDHN	cdhn;
    CDKEY	key[2];
    CDBUF	buf[2];
    Sint32	stat;
    Sint32	len[2];
    Uint32 convpos = 0;
    Uint16 i;
    
void loadfile(char* file,Uint32 pt){

    Uint16 rgb[0];
    
    convpos = 0;
    key[0].cn = key[0].sm = key[0].ci = CDKEY_NONE;
    key[1].cn = CDKEY_TERM;	
    //file name here
    cdhn = slCdOpen(file, key);
    buf[0].type = CDBUF_COPY;	
    buf[0].trans.copy.addr = readbuf;	
    buf[0].trans.copy.unit = CDBUF_FORM1;
    buf[0].trans.copy.size = 2;	
    buf[1].type = CDBUF_TERM;
    
    slCdLoadFile(cdhn, buf);  
     
    Uint16 cnt = 0;
    Uint32 pos = 0;
    Uint8 r,g,b;
    
    while (1) {
        
       
   
    
    // slPrint(file,slLocate(12,14));
     
	slSynch();     			
	stat = slCdGetStatus(cdhn, len);
	
	
	
	if (stat == CDSTAT_COMPLETED)
   	break;
	
	//slPrintFX(toFIXED(len[0]),slLocate(12,17));
        	if (len[0] == CDBUF_FORM1*2){

                for (i = 0; i< 4096;i++){
                    
                      if (pos % 3 == 0)
                      r = readbuf[i] >> 3;
                      
                      if (pos %3 == 1)
                      g = readbuf[i] >> 3;
                      
                      if (pos % 3 == 2){
                            
                                  b = readbuf[i] >> 3;
                                  
                                  rgb[0] = (1 << 15) | (b << 10) | (g << 5) | r;
                                  if (rgb[0] == 0x8000)
                                  rgb[0] = 0x0000;
                                  //bufconv[convpos++] = rgb;
                                
                                slDMACopy((Uint16*)rgb,
                              	(void *)( pt + convpos*2),
                            	2);  
                                                  
                                convpos++;
                       }
                    pos++;
                    
                }
                                         
                slCdResetBuf(cdhn, &(key[0]));   

            }
            
          //slPrintFX(toFIXED(convpos),slLocate(12,14));  
 
   } 
    
    
     for (i = 0; i< len[0];i++){
        
         if (pos % 3 == 0)
                      r = readbuf[i] >> 3;
                      
                      if (pos %3 == 1)
                      g = readbuf[i] >> 3;
                      
                      if (pos % 3 == 2){
                                  b = readbuf[i] >> 3;
                                  
                                  rgb[0] = (1 << 15) | (b << 10) | (g << 5) | r;
                                   if (rgb[0] == 0x8000)
                                  rgb[0] = 0x0000;
                                
                                slDMACopy((Uint16*)rgb,
                              	(void *)( pt + convpos*2),
                            	2);  
                                                  
                                convpos++;
                       }
                    pos++;
    }
                   
    //clearScreen();
    
}

void move(){

// 1/2 mv2  
// y = v0.t.sin(a) - 1/2gt2
        Uint16 data;
        Uint16 push;
        Uint16 pull;
       
        
        pull = Smpc_Peripheral[0].pull;
        data = Smpc_Peripheral[0].data;
        push = Smpc_Peripheral[0].push;

        if (((push & PER_DGT_TC) == 0)){
            clearScreen();
            
            if (madame == 0)
            madame = 1;
            else
            madame = 0;
        }
        
    if (((push & PER_DGT_TB) == 0)){
        slPCMOn(&clap_dat, clap, clap_size);
        
    }
        
    if (((push & PER_DGT_TX) == 0)){
        slPCMOn(&fart_dat, fart, fart_size);
        fartcnt++;
        
    }
            
        if (((push & PER_DGT_TA) == 0) && jumped == 0 && madame == 0){
            slPCMOn(&jump_dat, jumpd, jump_size);
            jump = time;
            speed = 10.0;
            jumped = 1;
        }
        
        if (((pull & PER_DGT_TA) == 0) && jumped == 1){
            //break the jump
            //speed = 0.0;
            stopped = 1;
        }
        
        
        if (((data & PER_DGT_KL) == 0) && madame == 1 ){
            mpos[X] = mpos[X] - toFIXED(3.0);
        }

        if (((data & PER_DGT_KR) == 0) && madame == 1 ){
            mpos[X] = mpos[X] + toFIXED(3.0);
        }
 
         if (((data & PER_DGT_KD) == 0) && madame == 1 ){
            mpos[Y] = mpos[Y] + toFIXED(3.0);
        }
        
         if (((data & PER_DGT_KU) == 0) && madame == 1 ){
            mpos[Y] = mpos[Y] - toFIXED(3.0);
        }
                      
         if (((data & PER_DGT_KL) == 0) && madame == 0 ){
              
             if ((spos[X] < toFIXED(-80.0) | (rota % 90 != 0)) && rota > 0) {  
             //cmbuf.target[X] += toFIXED(-1.0);
            rota--;
            cmbuf.pos[X] = slMulFX(toFIXED(240.0) , slSin(DEGtoANG(rota)));
            cmbuf.pos[Z] = toFIXED(120.0) - slMulFX(toFIXED(240.0) , slCos(DEGtoANG(rota)));
            spos[X] += toFIXED(2.0);
            if (rota % 90 == 0)
            panel = (panel - 1) % 4;
            } else {
             spos[X] -= toFIXED(2.0);
            }
             angle=120;
         }
         else
        if (((data & PER_DGT_KR) == 0) && madame == 0){
        
         if ((spos[X] > toFIXED(80.0) || (rota % 90 != 0)) && rota < 270) {
            rota++;
            cmbuf.pos[X] = slMulFX(toFIXED(240.0) , slSin(DEGtoANG(rota)));
            cmbuf.pos[Z] = toFIXED(120.0) - slMulFX(toFIXED(240.0) , slCos(DEGtoANG(rota)));
            spos[X] -= toFIXED(2.0);
            if (rota % 90 == 0)
            panel = (panel + 1) % 4;
            //cmbuf.target[X] += toFIXED(1.0);
         } else {
         spos[X] += toFIXED(2.0);
         }
         angle=60;
         }
         else
         angle=90;  
         
        if (jumped == 1){
            
            FIXED diff = speed*slMulFX((time - jump), slSin(DEGtoANG(angle))) - 0.5*18.0*slMulFX((time - jump),(time - jump));
            
            spos[Y] -=  diff;
            
            //slPrintFX(spos[Y], slLocate(18, 16));
            if (stopped == 1 && spos[Y] < toFIXED(30.0) && diff > toFIXED(0.0)){
                speed = 0.0;
                jump = time;
                stopped = 0;
            }
        } 
         
        if (spos[Y] > toFIXED(60.1))  {
            //slPrintFX(spos[Y], slLocate(18, 17));
                spos[Y] = toFIXED(60.0);
                jumped = 0;
                stopped = 0;
        }    
         
      
  
    
}

void title(){
    
  
    Uint8 blink = 0;
    while(1){
  
  
  
        light[X] = slCos(DEGtoANG(frame));
        light[Y] = toFIXED(-1.0);
        light[Z] = slSin(DEGtoANG(frame));
    
        slUnitMatrix(CURRENT);
        
        slLookAt(cmbuf.pos,cmbuf.target,DEGtoANG(0.0));
        slLight(light);
        
        slPushMatrix();
        {
           
            slRotY(DEGtoANG(-50));
            
            
            slTranslate(toFIXED(-70.0),pos[Y],toFIXED(0.0));
            slScale(toFIXED(5.0),toFIXED(5.0),toFIXED(5.0));
            //slPutObject((OBJECT *)object_0);
            slPutPolygonX((XPDATA *)xpdata_0,light);
        }
        slPopMatrix();
 
  
         slPushMatrix();
        {
           
            slRotY(DEGtoANG(50));
            
            
            slTranslate(toFIXED(0.0),toFIXED(30.0),toFIXED(0.0));
            slScale(toFIXED(2.0),toFIXED(2.0),toFIXED(2.0));
            //slPutObject((OBJECT *)object_0);
            slPutPolygonX((XPDATA *)xpdata_0,light);
        }
        slPopMatrix();
            
        
        Uint16 push;
        push = Smpc_Peripheral[0].push;

        if (frame % 30 == 0 && blink == 1)
        blink = 0;
        else
        if (frame % 60 == 0 && blink == 0)
        blink = 1;

        clearScreen();
        if (blink == 1)
        slPrint("PRESS [A] BUTTON", slLocate(12,26));
        
        slPrint("A BORING FRIDAY NIGHT II", slLocate(8,23));
        slSynch();
 
        frame++;
        time = slDivFX(FPS , frame);

        if (((push & PER_DGT_TA) == 0)){
            clearScreen();
            break;       
        }
    }
}

Uint8 popup(){
    
    
    
    if (dialog[S] > toFIXED(0.99))
    dialog[S] = toFIXED(0.00);
    if (dialog[S] < toFIXED(0.99))
    dialog[S] += toFIXED(0.05);
    
    slDispSprite((FIXED *)dialog,attr + 1,DEGtoANG(-90));
    
    if (dialog[S] > toFIXED(0.99))
    return 1;
    else
    return 0;
   
    
}

Uint8 infwin(){
    
    info[X] = toFIXED(0.0);
    info[Y] = toFIXED(0.0);
    info[Z] = toFIXED(160.0);
    
    if (info[S] > toFIXED(19.99))
    info[S] = toFIXED(0.00);
    if (info[S] < toFIXED(19.99))
    info[S] += toFIXED(0.50);
    
    slDispSprite((FIXED *)info,attr + 7,DEGtoANG(0.0));
    
    if (info[S] > toFIXED(19.99))
    return 1;
    else
    return 0;
   
    
}

Uint8 infwin_close(){
    
    info[X] = toFIXED(0.0);
    info[Y] = toFIXED(0.0);
    info[Z] = toFIXED(160.0);
    
    if (info[S] > toFIXED(0.49))
    info[S] -= toFIXED(0.50);
    
    slDispSprite((FIXED *)info,attr + 7,DEGtoANG(0.0));
    
    if (info[S] < toFIXED(0.49))
    return 1;
    else
    return 0;
   
    
}

Uint8 popclose(){

    pos0_drawn = 0;
    pos1_drawn = 0;
    pos2_drawn = 0;

    if (dialog[S] > toFIXED(0.04))
    dialog[S] -= toFIXED(0.05);
    
    slDispSprite((FIXED *)dialog,attr + 1,DEGtoANG(-90));
    
    if (dialog[S] < toFIXED(0.04))
    return 1;
    else
    return 0;
    
}



void event(){



if (events[3] == 1 && events[13] == 0){
    //slPrint("YO",slLocate(1,1));
     if (infwin() == 1){
        events[13] = 1;
       stepper = 0; 
    }
    
} else 
 
 if (events[13] == 1 && events[14] == 0){
   
    slDispSprite((FIXED *)info,attr + 7,DEGtoANG(0.0));
    
    if (stepper  == 0)
    slPrint("2ND YEAR E-MAIL STAT",slLocate(10,6));
    
    if (stepper == 1)
    slPrint("1ST YEAR : ",slLocate(11,9));
    
    if (stepper == 2)
    slPrint("6693",slLocate(22,9));
 
     if (stepper == 3)
    slPrint("2ND YEAR : ",slLocate(11,11));
    
    if (stepper == 4)
    slPrint("3679",slLocate(22,11));
    
    if (stepper == 5){
    slPrint("TOTAL : 10372 !!!",slLocate(11,14));
    slPCMOn(&clap_dat, clap, clap_size);
    stepper = 6;
     }
    //clap clap noise
    
    if (stepper == 7)
    slPrint("PER MONTH : ~300",slLocate(11,16));
     
     if (stepper == 8){
     loadfile("SKYP.dat",SpriteVRAM + CGADDRESS+387264+6272*3+32*32*2+40*24*2+8*8*2+64*48*2);
     stepper =9;
     }
     
     if (stepper >= 9)
     slDispSprite((FIXED *)fun,attr + 9,DEGtoANG(0.0));
    
    
    
        Uint16 push;
        push = Smpc_Peripheral[0].push;
   
        if (((push & PER_DGT_TA) == 0))       
        stepper++;
        
        if (stepper == 10){
            clearScreen();
            stepper = 0;
            events[14] = 1;
    }
        
    
} else 
if (events[14] == 1 && events[15] == 0){
    //slPrint("YO",slLocate(1,1));
     if (infwin_close() == 1){
        events[15] = 1;
       
    }
    
} else 

if (events[10] == 0 && events[9] == 1){
    
    
         if (popup() == 1)
        events[10] = 1;
                
    } else
    
    if (events[11]== 0 && events[9] == 1){
        
        
        slDispSprite((FIXED *)dialog,attr + 1,DEGtoANG(-90));
        slDispSprite((FIXED *)tanu,attr + 5,DEGtoANG(0.0));
        display_string("u8cb7u7269u3057u3066u306duff01",0);
        //display_string("u3042uff01u305fu306cu304bu3089u30e1u30fcu30ebu304du305fuff01",1);
        //display_string("u4ecau65e5u305fu306cu306f",2); 
 
        Uint16 push;
        push = Smpc_Peripheral[0].push;
   
        if (((push & PER_DGT_TA) == 0))       
        events[11] = 1;
                     
    }
    else if (events[12] == 0 && events[9] == 1){
        
       clearScreen();
       if (popclose() == 1) 
       events[12] = 1;
    } else
        
    if (events[7] == 0){
         if (popup() == 1)
        events[7] = 1;
                
    } else
    
    if (events[8]== 0){
        
              
        slDispSprite((FIXED *)dialog,attr + 1,DEGtoANG(-90));
        display_string("u4ecau65e5u3082u3001u304au3064u304bu308c",0);
        display_string("u3042uff01u305fu306cu304bu3089u30e1u30fcu30ebu304du305fuff01",1);
        //display_string("u4ecau65e5u305fu306cu306f",2); 
 
        Uint16 push;
        push = Smpc_Peripheral[0].push;
   
        if (((push & PER_DGT_TA) == 0))       
        events[8] = 1;
                     
    }
    else if (events[9] == 0){
        
       clearScreen();
       if (popclose() == 1) 
       events[9] = 1;
    }
     else
    
    if (spos[X] > toFIXED(60.0) && events[0] == 0 && panel == 0){
        
        if (popup() == 1)
        events[0] = 1;
    } else 
    
    if (spos[X] > toFIXED(60.0) && events[1] == 0 && panel ==  0){
        
        slDispSprite((FIXED *)dialog,attr + 1,DEGtoANG(-90));
        display_string("u3061u306au307fu306bu3001u4ecau307eu3067u30e1u30fcu30ebu306f",0);
        display_string("u306au3093u30b1u30f3u3067u3057u3087u3046u304bu3002",1);
               
        Uint16 push;
        push = Smpc_Peripheral[0].push;
   
        if (((push & PER_DGT_TA) == 0))       
        events[1] = 1;
        
        
    } else if (spos[X] > toFIXED(60.0) && events[3] == 0 && panel == 0){
       clearScreen();
       if (popclose() == 1) 
       events[3] = 1;
    }
    else if (spos[X] > toFIXED(60.0) && events[4] == 0 && panel == 1){
 
         if (popup() == 1)
        events[4] = 1;       
        
    } else 

if (spos[X] > toFIXED(-60.0) && events[16] == 0 && panel == 1){
    
     if (popup() == 1)
        events[16] = 1;
    
} else

if (events[16] == 1 && events[17] == 0){
    
    slDispSprite((FIXED *)dialog,attr + 1,DEGtoANG(-90));
    slDispSprite((FIXED *)perupos,attr + 8,DEGtoANG(0.0));
    display_string("u3042uff01u30e9u30c6u30f3u30acu30a4u304cu3044u305fuff01",0);
    display_string("u30a8u30adu30beu30c1u30c3u30afu304bu3093u305fu3063u3077u308a",1);
    display_string("u305fu3073u3060u3061u305fu304fu306au308bu306du3047",2);
 
        Uint16 push;
        push = Smpc_Peripheral[0].push;
   
        if (((push & PER_DGT_TA) == 0)) 
        events[17] = 1;
        
} else  

if (events[17] == 1 && events[18] == 0){
       clearScreen();
       if (popclose() == 1) 
       events[18] = 1;
}
else   

if ( spos[X] > toFIXED(60.0) && events[18] == 1 && events[19] == 0 && panel == 1){
    
     if (popup() == 1)
        events[19] = 1;
    
} else

if (events[19] == 1 && events[20] == 0){
    
    slDispSprite((FIXED *)dialog,attr + 1,DEGtoANG(-90));
    display_string("u3061u306au307fu306bu3001u3053u3068u3057u3082u30c1u30e5u30a2",0);
    display_string("u304cu304au304au304bu3063u305fu306duff01",1);
    display_string("u3069u3053u306bu3044u3063u305fu3063u3051u3002u3002u3002",2);

        Uint16 push;
        push = Smpc_Peripheral[0].push;
   
        if (((push & PER_DGT_TA) == 0)) 
        events[20] = 1;
            
}
else

if (events[20] == 1 && events[21] == 0){
    
    //slDispSprite((FIXED *)dialog,attr + 1,DEGtoANG(-90));

        if (infwin() == 1){
           events[21] = 1;
           stepper = 0; 
        }
        
    
} 
else 


if (events[21] == 1 && events[22] == 0){
 
 
        
        Uint16 push;
        push = Smpc_Peripheral[0].push;
   
        if (((push & PER_DGT_TA) == 0))       
        stepper++;
           
    slCdChgDir("TRIP/");
    
    //slDispSprite((FIXED *)dialog,attr + 1,DEGtoANG(-90));
    slDispSprite((FIXED *)info,attr + 7,DEGtoANG(0.0));
 
  if (stepper  == 0)
   slPrint("2ND YEAR TOURS STAT",slLocate(10,6));
   
   if (stepper == 1){
        
    }
    
    
    if (stepper == 2){
        
        slPrint("1) IZU : SEP 2015",slLocate(11,9));
        stepper = 3;
    }
    
    if (stepper == 3){
        
     }
    //load pic & disp - use same size and same sprite ref
    
    if (stepper == 4){
        //loadfile("SKYP.dat",SpriteVRAM + CGADDRESS+387264+6272*3+32*32*2+40*24*2+8*8*2+64*48*2);
        loadfile("SERP.data",SpriteVRAM + CGADDRESS+387264+6272*3+32*32*2+40*24*2+8*8*2+64*48*2);
        stepper = 5;
    }
     
     if (stepper == 5){
        slDispSprite((FIXED *)fun,attr + 9,DEGtoANG(0.0)); 
    }
        
 
    if (stepper == 6){
        
    clearScreen();
    slPrint("2ND YEAR TOURS STAT",slLocate(10,6));
    slPrint("2) KYUSHU : NOV 2015",slLocate(11,9));
    stepper = 7;
    }
    
    if (stepper == 7){
        
}
    
    if (stepper == 8){
        loadfile("KYUS.data",SpriteVRAM + CGADDRESS+387264+6272*3+32*32*2+40*24*2+8*8*2+64*48*2);
        stepper = 9;       
        
    }

     if (stepper == 9){
        slDispSprite((FIXED *)fun,attr + 9,DEGtoANG(0.0)); 
    }    

    if (stepper == 10){
        clearScreen();
    slPrint("2ND YEAR TOURS STAT",slLocate(10,6));
    slPrint("3) OSAKA : DEC 2015",slLocate(11,9));
    stepper = 11;
    }
    
    if (stepper == 11){
        
    }


    if (stepper == 12){
        loadfile("BALL.data",SpriteVRAM + CGADDRESS+387264+6272*3+32*32*2+40*24*2+8*8*2+64*48*2);
        stepper = 13;       
        
    }

     if (stepper == 13){
        slDispSprite((FIXED *)fun,attr + 9,DEGtoANG(0.0)); 
    }  
            
    if (stepper == 14){
        clearScreen();
         slPrint("2ND YEAR TOURS STAT",slLocate(10,6));
    slPrint("4) HAWAII : JAN 2016",slLocate(11,9));
    stepper = 15;
    }
    
    if (stepper == 15){
        
    }

    if (stepper == 16){
        loadfile("HAWA.data",SpriteVRAM + CGADDRESS+387264+6272*3+32*32*2+40*24*2+8*8*2+64*48*2);
        stepper = 17;       
        
    }

     if (stepper == 17){
        slDispSprite((FIXED *)fun,attr + 9,DEGtoANG(0.0)); 
    }  
     
    if (stepper == 18){
        clearScreen();
         slPrint("2ND YEAR TOURS STAT",slLocate(10,6));
        slPrint("5) SHIKOKU : MAY 2016",slLocate(11,9));
    stepper = 19;
    }
    
    if (stepper == 19){
        
}
 
     if (stepper == 20){
        
        loadfile("SHIK.data",SpriteVRAM + CGADDRESS+387264+6272*3+32*32*2+40*24*2+8*8*2+64*48*2);
        stepper = 21;       
        
    }

     if (stepper == 21){
        slDispSprite((FIXED *)fun,attr + 9,DEGtoANG(0.0)); 
    }  
    
    if (stepper == 22){
        clearScreen();
         slPrint("2ND YEAR TOURS STAT",slLocate(10,6));
         slPrint("6) NAGANO : JUL 2016",slLocate(11,9));
        stepper = 23;
    }
    
    if (stepper == 23){
        
    }
           
    if (stepper == 24){
        slPrint("HARD DAMITANU TIME",slLocate(11,12));
        loadfile("NAGA.data",SpriteVRAM + CGADDRESS+387264+6272*3+32*32*2+40*24*2+8*8*2+64*48*2);
        stepper = 25;       
        
    }

     if (stepper == 25){
        slDispSprite((FIXED *)fun,attr + 9,DEGtoANG(0.0)); 
    }  
    

    if (stepper == 26){
        clearScreen();
        slPrint("2ND YEAR TOURS STAT",slLocate(10,6));
        slPrint("7) HOKKAIDO : SEP 2016",slLocate(11,9));
        stepper = 27;
    }
    
    if (stepper == 27){
        
    }
           
    if (stepper == 28){
        loadfile("HOKK.data",SpriteVRAM + CGADDRESS+387264+6272*3+32*32*2+40*24*2+8*8*2+64*48*2);
        stepper = 29;       
        
    }

     if (stepper == 29){
        slDispSprite((FIXED *)fun,attr + 9,DEGtoANG(0.0)); 
        slPCMOn(&fart_dat, fart, fart_size);
        //slPCMOn(&fart_dat, fart, fart_size);
    }         
 
 
 
         if (stepper == 30){
            clearScreen();
            events[22] = 1;
        }      
    //clap clap noise
    
   // if (stepper == 8)
   // slPrint("PER MONTH : ~300",slLocate(11,16));

        

    
    
}   else    
 
 if (events[22] == 1 && events[23] == 0){
    //slPrint("YO",slLocate(1,1));
     if (infwin_close() == 1){
        events[23] = 1;
       
    }
    
} 
else
if (spos[X] > toFIXED(-60.0) && events[24] == 0 && panel == 2){
    
    pos0_drawn = 0;
    pos1_drawn = 0;
    pos2_drawn = 0;
    
     if (popup() == 1)
        events[24] = 1;
    
} 
else
if (events[24] == 1 && events[25] == 0){
    
    slDispSprite((FIXED *)dialog,attr + 1,DEGtoANG(-90));
    //slDispSprite((FIXED *)perupos,attr + 8,DEGtoANG(0.0));
    display_string("u305du3046u3060uff01u8cb7u7269u305bu306au3042u304bu3093",0);
    display_string("u30b8u30e5u30fcu30b9u3001u3068u3046u306bu3085u3046",1);
    //display_string("u305fu3073u3060u3061u305fu304fu306au308bu306du3047",2);
 
        Uint16 push;
        push = Smpc_Peripheral[0].push;
   
        if (((push & PER_DGT_TA) == 0)) 
        events[25] = 1;
        
} else  

if (events[25] == 1 && events[26] == 0){
       clearScreen();
       if (popclose() == 1) 
       events[26] = 1;
}

else

if (spos[X] > toFIXED(60.0) && events[27] == 0 && panel == 2){
    
    pos0_drawn = 0;
    pos1_drawn = 0;
    pos2_drawn = 0;
    
     if (popup() == 1)
        events[27] = 1;
    
} 
else
if (events[27] == 1 && events[28] == 0){
    
    slDispSprite((FIXED *)dialog,attr + 1,DEGtoANG(-90));
    //slDispSprite((FIXED *)perupos,attr + 8,DEGtoANG(0.0));
    display_string("u3044u3063u3057u3087u306bu3059u3093u3067u308bu306e",0);
    display_string("u306fuff19u30f6u3052u3064u3067u3059u306du3002",1);
    display_string("u307eu3068u3081u30bfu30a4u30e0uff01uff01uff01",2);
 
        Uint16 push;
        push = Smpc_Peripheral[0].push;
   
        if (((push & PER_DGT_TA) == 0)) 
        events[28] = 1;
        
} else  

if (events[28] == 1 && events[29] == 0){
       clearScreen();
       if (popclose() == 1) 
       events[29] = 1;
}
else
if (events[29] == 1 && events[30] == 0){
    //slPrint("YO",slLocate(1,1));
     if (infwin() == 1){
        events[30] = 1;
       stepper = 0; 
    }
    
} else 
 
 if (events[30] == 1 && events[31] == 0){
   
    slDispSprite((FIXED *)info,attr + 7,DEGtoANG(0.0));
    
    if (stepper  == 0)
    slPrint("COUPLE LIFE",slLocate(10,6));
    
    
  if (stepper  == 1)
    slPrint("2ND YEAR",slLocate(10,8)); 
    
  if (stepper  == 2)
    slPrint("1 - LIVE TOGETHER",slLocate(10,10));
  if (stepper  == 3)
    slPrint("2 - OVERSEA TRIP",slLocate(10,11));
  if (stepper  == 4)
    slPrint("3 - KAWAII BENTO",slLocate(10,12));

   if (stepper  == 5)
    slPrint("3RD YEAR",slLocate(10,14)); 
 
   if (stepper  == 6)
    slPrint("1 - OVERSEA TRIP II ?",slLocate(10,16));
  if (stepper  == 7)
    slPrint("2 - BYE BYE ZEN HOUSE ?",slLocate(10,17));
  if (stepper  == 8)
    slPrint("3 - TENSHOKU ?",slLocate(10,18));
   if (stepper  == 9)
    slPrint("4 - BYE BYE EMAIL ? (LOL)",slLocate(10,19));

                         
        Uint16 push;
        push = Smpc_Peripheral[0].push;
   
        if (((push & PER_DGT_TA) == 0))       
        stepper++;
        
        if (stepper == 10){
            clearScreen();
            stepper = 0;
            events[31] = 1;
    }
        
    
} else 
if (events[31] == 1 && events[32] == 0){
    //slPrint("YO",slLocate(1,1));
     if (infwin_close() == 1){
        events[32] = 1;
       
    }
    
}
else
if (spos[X] > toFIXED(60.0) && events[33] == 0 && panel == 3){
    
    pos0_drawn = 0;
    pos1_drawn = 0;
    pos2_drawn = 0;
    
     if (popup() == 1)
        events[33] = 1;
    
} 
else
if (events[33] == 1 && events[34] == 0){
    
    slDispSprite((FIXED *)dialog,attr + 1,DEGtoANG(-90));
    //slDispSprite((FIXED *)perupos,attr + 8,DEGtoANG(0.0));
    display_string("u3042uff01u3084u3063u3068u5bb6u306bu3064u3044u305fu3002",0);
    display_string("u305fu306cu306fu3069u3053u003f",1);
    display_string("u304au3002u3002u3002u306du3066u308bu306duff01",2);
 
        Uint16 push;
        push = Smpc_Peripheral[0].push;
   
        if (((push & PER_DGT_TA) == 0)) 
        events[34] = 1;
        
} else  

if (events[34] == 1 && events[35] == 0){
       clearScreen();
       if (popclose() == 1) 
       events[35] = 1;
}
else
if (events[35] == 1 && events[36]== 0){
 
        slCdChgDir("/TANU");   
         loadfile("NERU.data",SpriteVRAM + CGADDRESS+387264+6272*3+32*32*2+40*24*2+8*8*2+64*48*2);
        events[36] = 1; 
        fartcnt = 0;
           
}
else
if (events[36] == 1 && events[37] == 0){
    fun[X] =  toFIXED(-40.0);
    slDispSprite((FIXED *)fun,attr + 9,DEGtoANG(0.0)); 
    move();
    
    if (fartcnt >= 3){
        events[37] = 1;    
    }
}    
else
if (events[37] == 1 && events[38] == 0){
    
     loadfile("WAKE.data",SpriteVRAM + CGADDRESS+387264+6272*3+32*32*2+40*24*2+8*8*2+64*48*2);
     events[38] = 1;
     
}
else
if (events[38] == 1 && events[39] == 0){
    
     slDispSprite((FIXED *)fun,attr + 9,DEGtoANG(0.0)); 
   
        Uint16 push;
        push = Smpc_Peripheral[0].push;
   
        if (((push & PER_DGT_TA) == 0)) 
        events[39] = 1;
   
    
} else if (events[39] == 1 && events[40] == 0){
    
    slDispSprite((FIXED *)fun,attr + 9,DEGtoANG(0.0)); 
    spos[X] -= toFIXED(0.5);
    if (spos[X] <= 0.0){
    events[40] = 1;
    
    loadfile("KOKO.data",SpriteVRAM + CGADDRESS+387264+6272*3);
    
    
    }
    
} else if (events[40] == 1 && events[41] == 0) {
    
    slPrint("TANU....",slLocate(10,2));
    slPrint("3 NEN ME MO YOROSHIKUNE",slLocate(10,4));
    slPrint("JE T AIME TANU",slLocate(10,6));
    
    tanu[X] =  toFIXED(0.0);
    tanu[Y] =  toFIXED(-40.0);
    tanu[Z] = toFIXED(160.0);
    tanu[S] = toFIXED(0.0);     
    
    while (1){
        
        if (tanu[S] < toFIXED(2.0))
        tanu[S] += toFIXED(0.05);
        
        slDispSprite((FIXED *)tanu,attr + 5,DEGtoANG(0.0)); 
        
        slDispSprite((FIXED *)fun,attr + 9,DEGtoANG(0.0)); 
        slDispSprite((FIXED *)spos,attr + 0,DEGtoANG(0.0));
        
        slSynch();
        
    }
}
else
move();

}


    void loadsound(){
 
    key[0].cn = key[0].sm = key[0].ci = CDKEY_NONE;
    key[1].cn = CDKEY_TERM;	
    cdhn = slCdOpen("JUMP.WAV", key);
    buf[0].type = CDBUF_COPY;	
    buf[0].trans.copy.addr = audiobuf;	
    buf[0].trans.copy.unit = CDBUF_FORM1;
    buf[0].trans.copy.size = 2;	
    buf[1].type = CDBUF_TERM;
    
    slCdLoadFile(cdhn, buf);  
    
   
    i = 0;
    int j =0;
    Uint32 datalen = 0;
    while (1) {
    slPrint("NOW LOADING SOUND...",slLocate(9,15));
	slSynch();     			
	stat = slCdGetStatus(cdhn, len);
	
	if (stat == CDSTAT_COMPLETED)
   	break;
	
        	if (len[0] == 2048*2){
                
                if (i == 0)
                datalen = audiobuf[43] << 24 | audiobuf[42] << 16 | audiobuf[41] << 8 | audiobuf[40];
                
                
                for (j =0; j < 2048*2 ;j++)
                 if (j%2 == 1)
                  audiobuf2[j/2] = audiobuf[j];

                if (i == 0)
                slDMACopy(audiobuf2 + 23, (void*) 0x200000 , 2048-23);
                else
                slDMACopy(audiobuf2, (void*) 0x200000 + 2048 - 23 + (2048 * (i-1)), 2048);                
                i++;
               
                         
                slCdResetBuf(cdhn, &(key[0]));   
                //slPrintFX(toFIXED(i),slLocate(8,8));
            }
 
   } 
   
                
                
                for (j = 0; j < len[0] ;j++)
                 if (j%2 == 1)
                  audiobuf2[j/2] = audiobuf[j];
                  
                for (j = len[0] / 2; j <2048;j++ )
                  audiobuf2[j] = 0xff;
                
                if (i > 0)
                slDMACopy(audiobuf2, (void*) 0x200000 + 2048 - 23 + (2048 * (i-1)), len[0]);
                else          
                slDMACopy(audiobuf2 + 23, (void*) 0x200000 , len[0]);
   
    
    jump_size = datalen/2 - datalen%2; 
    jumpd = (char*) 0x200000;
   
    

   clearScreen();
}

  void loadsound3(){
 
    key[0].cn = key[0].sm = key[0].ci = CDKEY_NONE;
    key[1].cn = CDKEY_TERM;	
    cdhn = slCdOpen("FART.WAV", key);
    buf[0].type = CDBUF_COPY;	
    buf[0].trans.copy.addr = audiobuf;	
    buf[0].trans.copy.unit = CDBUF_FORM1;
    buf[0].trans.copy.size = 2;	
    buf[1].type = CDBUF_TERM;
    
    slCdLoadFile(cdhn, buf);  
    
   
    i = 0;
    int j =0;
    Uint32 datalen = 0;
    while (1) {
    slPrint("NOW LOADING SOUND...",slLocate(9,15));
	slSynch();     			
	stat = slCdGetStatus(cdhn, len);
	
	if (stat == CDSTAT_COMPLETED)
   	break;
	
        	if (len[0] == 2048*2){
                
                if (i == 0)
                datalen = audiobuf[43] << 24 | audiobuf[42] << 16 | audiobuf[41] << 8 | audiobuf[40];
                
                
                for (j =0; j < 2048*2 ;j++)
                 if (j%2 == 1)
                  audiobuf2[j/2] = audiobuf[j];

                if (i == 0)
                slDMACopy(audiobuf2 + 23, (void*) 0x250000 , 2048-23);
                else
                slDMACopy(audiobuf2, (void*) 0x250000 + 2048 - 23 + (2048 * (i-1)), 2048);                
                i++;
               
                         
                slCdResetBuf(cdhn, &(key[0]));   
                //slPrintFX(toFIXED(i),slLocate(8,8));
            }
 
   } 
   
                
                
                for (j = 0; j < len[0] ;j++)
                 if (j%2 == 1)
                  audiobuf2[j/2] = audiobuf[j];
                  
                for (j = len[0] / 2; j <2048;j++ )
                  audiobuf2[j] = 0xff;
                
                if (i > 0)
                slDMACopy(audiobuf2, (void*) 0x250000 + 2048 - 23 + (2048 * (i-1)), len[0]);
                else          
                slDMACopy(audiobuf2 + 23, (void*) 0x250000 , len[0]);
   
    
    fart_size = datalen/2 - datalen%2; 
    fart = (char*) 0x250000;
   
    

   clearScreen();
}

void loadsound2(){


    
    key[0].cn = key[0].sm = key[0].ci = CDKEY_NONE;
    key[1].cn = CDKEY_TERM;	
  
    cdhn = slCdOpen("YEAH.wav", key);
    buf[0].type = CDBUF_COPY;	
    buf[0].trans.copy.addr = audiobuf;	
    buf[0].trans.copy.unit = CDBUF_FORM1;
    buf[0].trans.copy.size = 2;	
    buf[1].type = CDBUF_TERM;
    
    slCdLoadFile(cdhn, buf);  
     
   
    i = 0;
    int j =0;
    Uint32 datalen = 0;
    while (1) {
    slPrint("NOW LOADING SND 2...",slLocate(9,15));
	slSynch();     			
	stat = slCdGetStatus(cdhn, len);
	
	//slPrintFX(toFIXED(stat),slLocate(8,8));
	
	if (stat == CDSTAT_COMPLETED)
   	break;
	
	
        	if (len[0] == 4096){
                
                if (i == 0)
                datalen = audiobuf[43] << 24 | audiobuf[42] << 16 | audiobuf[41] << 8 | audiobuf[40];
                
                
                for (j =0; j < 4096 ;j++)
                 if (j%2 == 1)
                  audiobuf2[j/2] = audiobuf[j];

                if (i == 0)
                slDMACopy(audiobuf2 + 23, (void*) 0x230000 , 2048-23);
                else
                slDMACopy(audiobuf2, (void*) 0x230000 + 2048 - 23 + (2048 * (i-1)), 2048);                
                i++;
               
                         
                slCdResetBuf(cdhn, &(key[0]));   
                
                
            }
 
   } 
   
                
                
                for (j = 0; j < len[0] ;j++)
                 if (j%2 == 1)
                  audiobuf2[j/2] = audiobuf[j];
                  
                for (j = len[0] / 2; j <2048;j++ )
                  audiobuf2[j] = 0xff;
                
                if (i > 0)
                slDMACopy(audiobuf2, (void*) 0x230000 + 2048 - 23 + (2048 * (i-1)), len[0]);
                else          
                slDMACopy(audiobuf2 + 23, (void*) 0x230000 , len[0]);
   
    
    clap_size = datalen/2 - datalen%2; 
    clap = (char*) 0x230000;
   
    

   clearScreen();
}

void ss_main(void)
{

    Sint32 ndir;
        
    Uint16 push;
    slInitSystem(TV_320x240, tex_textures, FPS_DIV);
    ndir = slCdInit(MAX_FILE, dirwork);
   
    init_sound();
   
    ang[X] = ang[Y] = ang[Z] = DEGtoANG(0.0);
    pos[X] = toFIXED(0.0);
    pos[Y] = toFIXED(0.0);
    pos[Z] = toFIXED(0.0);
    
    cmbuf.ang[X] = cmbuf.ang[Y] = cmbuf.ang[Z] = DEGtoANG(0.0);
    cmbuf.target[X] = cmbuf.target[Y] = cmbuf.target[Z] = toFIXED(0.0);
    cmbuf.target[Z] = toFIXED(120.0);
    cmbuf.pos[X] = cmbuf.pos[Y] = toFIXED(0.0); 
    cmbuf.pos[Z] = toFIXED(-120.0); 
    
   
 
    info[S] = toFIXED(1.0);
   //__asm__("MOV #01, R0");
   
    perupos[X] =  toFIXED(0.0);
    perupos[Y] =  toFIXED(30.0);
    perupos[Z] =  toFIXED(160.0);
    perupos[S] =  toFIXED(1.0);
  
    mpos[X] =  toFIXED(0.0);
    mpos[Y] =  toFIXED(0.0);
    mpos[Z] = toFIXED(160.0);
    mpos[S] = toFIXED(1.0);        
    
    spos[X] =  toFIXED(-40.0);
    spos[Y] =  toFIXED(60.0);
    spos[Z] = toFIXED(160.0);
    spos[S] = toFIXED(1.0);
 
    dialog[X] =  toFIXED(100.0);
    dialog[Y] =  toFIXED(-20.0);
    dialog[Z] = toFIXED(160.0);
    dialog[S] = toFIXED(1.0);
    
    tanu[X] =  toFIXED(80.0);
    tanu[Y] =  toFIXED(-95.0);
    tanu[Z] = toFIXED(160.0);
    tanu[S] = toFIXED(1.0);      

    fun[X] =  toFIXED(0.0);
    fun[Y] =  toFIXED(45.0);
    fun[Z] = toFIXED(160.0);
    fun[S] = toFIXED(1.0);  
    
    frame = 0;
    
   slInitGouraud(gourRealMax, GOUR_REAL_MAX, GRaddr, vwork);
    
   slIntFunction(slGouraudTblCopy);
  
    
    //have title screen here ?
    title();

    slPrint("NOW LOADING...",slLocate(12,15));
    loadfile("ROOM.dat",SpriteVRAM + CGADDRESS + 300864);
    loadfile("SUGA.dat",SpriteVRAM + CGADDRESS + 0);
    loadfile("ALFA.dat",0x06080000);
    
    loadfile("CARA.dat",SpriteVRAM + CGADDRESS + 86400);
    loadfile("DIAL.dat",SpriteVRAM + CGADDRESS + 89664);
    loadfile("TANU.dat",SpriteVRAM + CGADDRESS+387264+6272*3);
    
    loadfile("GREY.dat",SpriteVRAM + CGADDRESS+387264+6272*3+32*32*2+40*24*2);
    loadfile("MAAM.dat",SpriteVRAM +  CGADDRESS+387264+6272*3+32*32*2);
    loadfile("PERU.dat",SpriteVRAM + CGADDRESS+387264+6272*3+32*32*2+40*24*2+8*8*2);
    
    loadfile("COMBI.dat",SpriteVRAM + CGADDRESS+214464);
    loadfile("PATCHI.dat",SpriteVRAM + CGADDRESS+128064);

    loadsound();
    loadsound2();
    loadsound3();   
    
    Uint8 disp = 0;
    int k = 0;
    int l = 0;
    float alpha = 0.0f;
    
    while(1)
    {
        
        //slDispHex(&test,slLocate(1,24));
        slUnitMatrix(CURRENT);
        
        
       if (alpha < 1.01f){
            
        for (k=0;k < 1632 ;k++){
            
          if (texture_textures001[k] != 0){
 
                 ru = texture_textures001[k] << 3;
                 ru = ru >> 3;
                 gu = texture_textures001[k] >> 2;
                 gu = gu >> 3;
                 bu = texture_textures001[k] >> 7;
                 bu = bu >> 3;
 
 
                ra = (int)( ru *  alpha);
                ga = (int)( gu *  alpha);
                ba = (int)( bu *  alpha);

                overlay[k]= (1 << 15) | (ba << 10) | (ga << 5) | ra;
                
        } else {
            overlay[k] = texture_textures001[k];
        }
 
        }

        slDMACopy((void *)overlay,
    	(void *)(SpriteVRAM + CGADDRESS + 86400),
    	3264 );
  	
        alpha = alpha + 0.1f;
        } 

        slLookAt(cmbuf.pos,cmbuf.target,DEGtoANG(0.0));
         
        //sprite Damien ?
        slDispSprite((FIXED *)spos,attr + 0,DEGtoANG(0.0));
               
        event();

        
        
        
        //disp madame sprite
        if (madame == 1){
            slDispSprite((FIXED *)mpos,attr + 6,DEGtoANG(0.0));
            slPrint("MADAME MODE",slLocate(2,2));
        }
       
              
        slPushMatrix();
        {
         slTranslate(pos[X],pos[Y],pos[Z]);
         slPutPolygon(&PD_PLANE1);
         slPutPolygon(&PD_PLANE2);
         slPutPolygon(&PD_PLANE3);
         slPutPolygon(&PD_PLANE4);
        }
        slPopMatrix();
        
     
        frame++;
        
        
        time = slDivFX(FPS , frame);
        
        slDMAWait();  

        slSynch();
    
        
    };
}


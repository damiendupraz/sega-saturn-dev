#include	"sgl.h"

#define		SystemWork		0x060ffc00		/* System Variable Address */
#define		SystemSize		(0x06100000-0x060ffc00)		/* System Variable Size */
/* sl.lnk で指定した.bssセクションの開始、終了シンボル */
extern Uint32 _bstart, _bend;
/* */
extern void ss_main( void );

//extern Uint16 texture_textures006[];

extern SPR_ATTR attr[];

int pos0_drawn = 0;
int pos1_drawn = 0;
int pos2_drawn = 0;

FIXED tpos[XYZS];

// GNUSH: void to int
Uint8 events[] = {
  0, //0
  0, //1
  0, //2
  0 , //3
  0,//4 event on panel pachinko (popup)
  0, //5  text disp
  0, //6 close pop
  0, //7 popup gare
  0, //8 text gare 1
  0, //9 close gare 1
  0, //10 tanu1
  0, //11 tanu1 text
  0, //12 tanu1 close
  0, //13 first info
  0, //14first data disp
  0, //15
  0, //16 popup latin guy
  0, //17
  0, //18
  0, //19
  0, //20
  0, //21
  0, //22
  0, //23
  0,//24
  0, //25
  0, //26
  0, //37
  0, //28
  0, //29
  0,//30
  0,//31
  0, //32
  0,//33
  0, //34
  0, //35
  0, //36
  0, //37
  0, //38
  0, //39
  0, //40
  0, //41
};

int	main( void )
{
	Uint8	*dst;
	Uint32	i;

	/* 1.Zero Set .bss Section */
	for( dst = (Uint8 *)&_bstart; dst < (Uint8 *)&_bend; dst++ ) {
		*dst = 0;
	}
	/* 2.ROM has data at end of text; copy it. */

	/* 3.SGL System Variable Clear */
	for( dst = (Uint8 *)SystemWork, i = 0;i < SystemSize; i++) {
		*dst = 0;
	}

	/* Application Call */
	ss_main();
}


POINT point_PLANE1[] = {
    POStoFIXED(-120.0,-90.0,0.0),//0
    POStoFIXED(120.0,-90.0,0.0),//1
    POStoFIXED(120.0,90.0,0.0),//2
    POStoFIXED(-120.0,90.0,0.0),//3
    POStoFIXED(-120.0,-90.0,240.0),//4
    POStoFIXED(120.0,-90.0,240.0),//5
    POStoFIXED(120.0,90.0,240.0),//6
    POStoFIXED(-120.0,90.0,240.0),//7
 
};

POLYGON polygon_PLANE1[] = {
    NORMAL(0.0,1.0,0.0),VERTICES(0,1,2,3),
    
};
  

 POLYGON polygon_PLANE2[] = {
    NORMAL(0.0,1.0,0.0),VERTICES(1,5,6,2),
 }; 

 POLYGON polygon_PLANE3[] = {
    NORMAL(0.0,1.0,0.0),VERTICES(5,4,7,6),
 }; 
 
  POLYGON polygon_PLANE4[] = {
    NORMAL(0.0,1.0,0.0),VERTICES(4,0,3,7),
 }; 
 
ATTR attribute_PLANE1[] = {
    ATTRIBUTE(
    Single_Plane,
    SORT_CEN,
    0,
    No_Palet,
    No_Gouraud,
    CL32KRGB|MESHoff,
    sprNoflip,
    No_Option
    )
};


ATTR attribute_PLANE2[] = {
    ATTRIBUTE(
    Single_Plane,
    SORT_CEN,
    3,
    No_Palet,
    No_Gouraud,
    CL32KRGB|MESHoff,
    sprNoflip,
    No_Option
    )
};


ATTR attribute_PLANE3[] = {
    ATTRIBUTE(
    Single_Plane,
    SORT_CEN,
    4,
    No_Palet,
    No_Gouraud,
    CL32KRGB|MESHoff,
    sprNoflip,
    No_Option
    )
};

ATTR attribute_PLANE4[] = {
    ATTRIBUTE(
    Single_Plane,
    SORT_CEN,
    5,
    No_Palet,
    No_Gouraud,
    CL32KRGB|MESHoff,
    sprNoflip,
    No_Option
    )
};

PDATA PD_PLANE1 = {
    point_PLANE1,
    sizeof(point_PLANE1)/sizeof(POINT),
    polygon_PLANE1,
    sizeof(polygon_PLANE1)/sizeof(POLYGON),
    attribute_PLANE1
};

PDATA PD_PLANE2 = {
    point_PLANE1,
    sizeof(point_PLANE1)/sizeof(POINT),
    polygon_PLANE2,
    sizeof(polygon_PLANE2)/sizeof(POLYGON),
    attribute_PLANE2
};


PDATA PD_PLANE3 = {
    point_PLANE1,
    sizeof(point_PLANE1)/sizeof(POINT),
    polygon_PLANE3,
    sizeof(polygon_PLANE3)/sizeof(POLYGON),
    attribute_PLANE3
};

PDATA PD_PLANE4 = {
    point_PLANE1,
    sizeof(point_PLANE1)/sizeof(POINT),
    polygon_PLANE4,
    sizeof(polygon_PLANE4)/sizeof(POLYGON),
    attribute_PLANE4
};

//font arrays

char kana[] = "u3041u3042u3043u3044u3045u3046u3047u3048u3049u304au304bu304cu304du304eu304fu3050u3051u3052u3053u3054u3055u3056u3057u3058u3059u305au305bu305cu305du305eu305fu3060u3061u3062u3063u3064u3065u3066u3067u3068u3069u306au306bu306cu306du306eu306fu3070u3071u3072u3073u3074u3075u3076u3077u3078u3079u307au307bu307cu307du307eu307fu3080u3081u3082u3083u3084u3085u3086u3087u3088u3089u308au308bu308cu308du308eu308fu3090u3091u3092u3093u30a1u30a2u30a3u30a4u30a5u30a6u30a7u30a8u30a9u30aau30abu30acu30adu30aeu30afu30b0u30b1u30b2u30b3u30b4u30b5u30b6u30b7u30b8u30b9u30bau30bbu30bcu30bdu30beu30bfu30c0u30c1u30c2u30c3u30c4u30c5u30c6u30c7u30c8u30c9u30cau30cbu30ccu30cdu30ceu30cfu30d0u30d1u30d2u30d3u30d4u30d5u30d6u30d7u30d8u30d9u30dau30dbu30dcu30ddu30deu30dfu30e0u30e1u30e2u30e3u30e4u30e5u30e6u30e7u30e8u30e9u30eau30ebu30ecu30edu30eeu30efu30f0u30f1u30f2u30f3u30f4u30f5u30f6uff21uff22uff23uff24uff25uff26uff27uff28uff29uff2auff2buff2cuff2duff2euff2fuff30uff31uff32uff33uff34uff35uff36uff37uff38uff39uff3auff10uff11uff12uff13uff14uff15uff16uff17uff18uff19u3000uff0du3001u3002uff1fuff01u4ecau65e5u8cb7u7269u75b2u5bb6u5927u597duff5eu5e97u5e30u524du5f8cu6383u9664u30fc";
char sub[6];
char substr[6];

Uint16 get_index_char(char* str){
    
    int i;
    
    for (i = 0;i<= strlen(kana) - 1;i=i+5){
        
           sub[0] = kana[i];
           sub[1] = kana[i+1];
           sub[2] = kana[i+2];
           sub[3] = kana[i+3];
           sub[4] = kana[i+4];
            sub[5] = '\0';
           //slPrint(sub,slLocate(18,i/5));
           
           if (strcmp(sub,str) == 0){
            return i/5;
            break;       
            }
    }


};
    


void display_string(char* str,int pos){
    
    int drawn = 0;
    
    if (pos == 0)
    drawn = pos0_drawn;
    else if (pos == 1)
    drawn = pos1_drawn;
    else
    drawn = pos2_drawn;
    
    
    Uint16 max = strlen(str);
    Uint16 r = 0;
    Uint16 index;
 
    Uint16 subtexture[16*196];
     
     int a,b;
     for (a = 0; a<= 16*196 - 1;a++)
      subtexture[a] = 0x0000;
         
    while(r < max && drawn == 0){
        
     substr[0] = str[r];
     substr[1] = str[r+1];
     substr[2] = str[r+2];
     substr[3] = str[r+3];
     substr[4] = str[r+4];
     substr[5] = '\0';
     
     //slPrint((char*) substr,slLocate(12,0+r));
     index = get_index_char(substr);
     //slPrintFX( toFIXED(index),slLocate(12,0+r));
     
     r = r + 5;
      
     Uint16 hor = (index % 11) * 11;
     Uint16 ver = (index / 11) * 12;
     Uint16 hormax = 11;
     Uint16 vermax = 12;
     Uint16 k = 0;
     
     Uint8 i,j;
     //int bytectn = 0;
    for (i = ver + 1; i <= ver  + vermax; i++){
        
        Uint16 offset = 128 * i - 128;
        
        for (j = hor + 1; j <= hor + hormax; j++){
            
            Uint8 *left = (Uint8*) (0x06080000  +(offset + j - 1)*2);
            Uint8 *right = (Uint8*) (0x06080000  + (offset + j - 1)*2 + 1);
            
            subtexture[256 * (r/5 - 1) + 16*(i - ver) - 16 + j - hor - 1] = *left << 8 | *right << 0;
            //bytectn++;
        }
        
    }
 
            slDMACopy((void *)subtexture,
		  	(void *)(SpriteVRAM + ((CGADDRESS+387264+6272*pos) )),
			(Uint32)((16*196*2) ));    
            
            if (pos == 0)
            pos0_drawn = 1;
            else if (pos == 1)
            pos1_drawn = 1;
            else
            pos2_drawn = 1;
                  
     
    }

        tpos[X] = toFIXED(130 - (20*pos));
        tpos[Y] = toFIXED(-0 + (5*pos));
        tpos[Z] = toFIXED(160.0);
        tpos[S] = toFIXED(1.0);    
    
        
        slDispSprite((FIXED *)tpos,attr + 2 + pos,DEGtoANG(0.0));
      
    
};

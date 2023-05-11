#pragma config FOSC = HS)
#pragma config WDTE = OFF 
#pragma config PWRTE = ON 
#pragma config BOREN = ON 
#pragma config LVP = OFF 
#pragma config CPD = OFF 
#pragma config WRT = OFF 
#pragma config CP = OFF
#include <xc.h>
#include "LCD.h"
#include<string.h>
#define _XTAL_FREQ 20000000
// I/0 name
//button gate
#define bt1 RB0
#define bt2 RB1
#define bt3 RB2
#define bt4 RB3
#define bt5 RB4 // alarm enable 
// output

// state for month
int count = 0;
int ti = 0;
int ST[] = {0,0};
int *pST = &ST;
//function define
int get_state();
void set_realtime();
void Lcd_basic_mode();
void set_HM(int a[],int sn);
int Get_State_Month(int m);
void moisture_display();
//Declare global Date time variable
int DT[] = {0,0,0,1,1,22};// 6 variable for HH:mm:ss, DD:MM:YY

int ST_2[] = {0,0};//set time 1 and 2
int *pST_2 = &ST_2;
// Reading ADC value for moisture
unsigned int Adc_V1 = 0;
unsigned int Adc_V2 = 0;
float mois = 0;
float temp = 0;
void __interrupt() timer_adc_isr()
{ 
    if(TMR0IF==1) // Timer flag has been triggered due to timer overflow
    {
       TMR0 = 100; //Load the timer Value
       TMR0IF=0; //Clear timer interrupt flag
       count++;
    } 
    if (count == 200)
    { 
       ti = ti +1;
       if(ti%2 == 0) DT[2] = DT[2]+ 1; // count second for every double ti
       //g
       if(DT[2]>59)
        {
            DT[1]++;
            DT[2] = 0;
            if(DT[1]>59)
            {
                DT[0]++;
                DT[1] = 0;
                if(DT[0]>23)
                {
                    DT[3]++;
                    DT[0] = 0;
                    if(((DT[3] > 30)&&(Get_State_Month(DT[4])==1))||
                       ((DT[3] > 31)&&(Get_State_Month(DT[4])==2))||
                       ((DT[3] > 28)&&(Get_State_Month(DT[4])==3)))
                    {
                        DT[4]++;
                        DT[3] = 1;
                        if(DT[4]>12)
                        {
                            DT[4] = 1;
                            DT[5]++;
                        }
                    }
                }
            }
        }
       count=0;
    }
    GIE = 0;   ADIF = 0; 	
	GIE = 1;

}

//function define
int get_state();
void set_realtime();
void Lcd_basic_mode();
void Set_Time();
//
char state = 0;
void button1();
void button2();
void button3();
void button4();
int main()
{   
    
    TRISD = 0x00;
    TRISB = 0x1F;
    TRISC = 0x00;
    PORTC = 0x00;
    TRISA = 0x03;
    
    start:
    RD0 = 0;
    //adc config
    PCFG3=0; PCFG2=1; PCFG1=0; PCFG0=0;
	ADFM=1;  //  Right Justified
	ADON=1;  //  Enable  ADC module
	// Enable ADC interrupt
	ADIF=0;  ADIE=1;  PEIE=1; GIE=1;

    // timer config
    OPTION_REG = 0b00000101;
    TMR0=100;
    TMR0IE=1;
    GIE=1;
    PEIE=1;
    Lcd_Clear();
    Lcd_Start();
    count = 0;
    RD1 = 0;// water pump
  // buzzor
    while(1)
    {    
        do
        {
           
            Lcd_basic_mode();
            state = get_state();
            RD1 = 0;
            if((((DT[0] == *(pST)) && (DT[1] == *(pST + 1)))&& bt5 == 1)||
               (((DT[0] == *(pST_2)) && (DT[1] == *(pST_2 + 1)))&& bt5 == 1))
            {
                RD1 = 1;
                __delay_ms(10);
            }
            CHS2=0; CHS1=0; CHS0=0;
            __delay_us(25);
            GO_nDONE = 1; 
            while(GO_nDONE); 
            Adc_V1=ADRESH*256+ADRESL;
            temp = Adc_V1*0.488;
        // LCD display
        // Select AN1
            CHS2=0;  CHS1=0; CHS0=1;
            __delay_us(25);
            GO_nDONE = 1; 
            while(GO_nDONE);
            Adc_V2 = ADRESH*256+ADRESL;
            mois = Adc_V2*0.488;
            RD0 = 0;
            if(temp > 50 || mois > 200)
            {
                RD0 = 1;
                __delay_ms(10);
            }
        }while(state == 0 || state == 4);
        
        switch(state)
        {
            case 1:
                set_realtime();
                state = 0;
                Lcd_Clear();
                goto start;
                break;
            case 2:
                moisture_display();
                state = 0;
                goto start;
                do
                {
                    state = get_state();
                }while(state == 0);
                break;
            case 3:
                Set_Time();
                Lcd_Clear();
                break;
            case 4: // reset
                Lcd_Set_Cursor(1,1);
                Lcd_Print_String("Exit!!!");
                __delay_ms(500);
                state = 0;
                goto start;
                break;                
        }
    }
}

int get_state()
{   
   char state = 0;
   if(bt1 == 1)
    {
        state = 1;
        bt1 = 0;
    }
    else if(bt2 == 1)
    {
        state = 2;
        bt2 = 0;
    }
    else if(bt3 == 1)
    {
        state = 3;
        bt3 = 0;
    }
    else if(bt4 == 1)
    {
        state = 4;
        bt4 = 0;
    }
    else
    {
        state = 0;
    }
   __delay_ms(100);
    return state;
}
void Lcd_basic_mode()
{
    Lcd_Set_Cursor(1,1);
    Lcd_Print_String("TIME:");
    Print_Num(DT[0],1,6);
    Lcd_Set_Cursor(1,8);
    Lcd_Print_String(":");
    Print_Num(DT[1],1,9);
    Lcd_Set_Cursor(1,11);
    Lcd_Print_String(":");
    Print_Num(DT[2],1,12);
    // next line
    Lcd_Set_Cursor(2,1);
    Lcd_Print_String("DATE:");
    Print_Num(DT[3],2,6);
    Lcd_Set_Cursor(2,8);
    Lcd_Print_String("/");
    Print_Num(DT[4],2,9);
    Lcd_Set_Cursor(2,11);
    Lcd_Print_String("/");
    Print_Num(DT[5],2,12);
}
// Function settime 
//Function prototype
void set_realtime()
{
    TMR0IE=0; // disable timer interupt
    GIE=0;//    ||||
    PEIE=0;// disable timer interupt
    Lcd_Clear();
    char cur_i[] = {6,9,12};
    char j = 1;
    int pos = 0;
    char sv = 0; // state value
    Lcd_Set_Cursor(1,2);
    Lcd_Print_String("DATE AND TIME");
    Lcd_Set_Cursor(2,2);
    Lcd_Print_String("CONFIGURATION");
    __delay_ms(2000);
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Print_String("TIME:  :  :  ");
    Print_Num(DT[0],1,6);
    Print_Num(DT[1],1,9);
    Print_Num(DT[2],1,12);
    Lcd_Set_Cursor(2,1);
    Lcd_Print_String("DATE:  :  :  ");
    Print_Num(DT[3],2,6);
    Print_Num(DT[4],2,9);
    Print_Num(DT[5],2,12);
    do
    {
        NEXT:
        Print_Num(DT[sv],j,cur_i[pos]);
        __delay_ms(100);
        Lcd_Set_Cursor(j,cur_i[pos]);
        Lcd_Print_String("__");
        state = get_state();
        switch(state)
        {
            case 1:
                DT[sv]++;
                if(DT[0] > 23) DT[0] = 0;
                if(DT[1] > 59) DT[1] = 0;
                if(DT[2] > 59) DT[2] = 0;
                if(((DT[3] > 30)&&(Get_State_Month(DT[4])==1))||
                       ((DT[3] > 31)&&(Get_State_Month(DT[4])==2))||
                       ((DT[3] > 28)&&(Get_State_Month(DT[4])==3))) DT[3] = 0;
                if(DT[4] > 12) DT[4] = 0;
                if(DT[5] > 25) DT[5] = 0;
                state = 0;
                break;
            case 2: // move to previous position
                Print_Num(DT[sv],j,cur_i[pos]);
                if(sv <= 0) sv = 0;
                else sv --;
                
                if(pos <= 0 && j == 1) pos = 0;
                else if(pos == 0 && j > 1)
                {
                    pos = 2;
                    j = 1;
                }
                else pos--;
                
                break;
            case 3: // move to next position 
                Print_Num(DT[sv],j,cur_i[pos]);
                sv++;
                pos++;
                if(pos > 2)
                {
                    pos = 0;
                    j++;
                }
                goto NEXT;
                state = 0;
                break;
            
            case 4:
                break;
        }
    }while(state != 4); 
}
void Set_Time()
{
    Lcd_Clear();
    Lcd_Set_Cursor(1,2);
    Lcd_Print_String("SETTING ALARM");
    __delay_ms(2000);
    do
    {
        Lcd_Set_Cursor(1,1);
        Lcd_Print_String("B1 FOR ALARM 1");
        Lcd_Set_Cursor(2,1);
        Lcd_Print_String("B2 FOR ALARM 2");
        state = get_state();
        switch(state)
        {
            case 1:
                Lcd_Clear();
                Lcd_Set_Cursor(1,2);
                Lcd_Print_String("SETTING ALR 1");
                __delay_ms(2000); 
                set_HM(pST,1);
                break;
            case 2:
                Lcd_Clear();
                Lcd_Set_Cursor(1,2);
                Lcd_Print_String("SETTING ALR 2");
                __delay_ms(2000); 
                set_HM(pST_2,2);
                break;
            case 3:
                Lcd_Clear();
                while(state != 4)
                {   
                    state = get_state();
                    Lcd_Set_Cursor(1,2);
                    Lcd_Print_String("ALARM 1   :");
                    Print_Num(ST[0],1,10);
                    Print_Num(ST[1],1,13);
                    Lcd_Set_Cursor(2,2);
                    Lcd_Print_String("ALARM 2   :");
                    Print_Num(ST_2[0],2,10);
                    Print_Num(ST_2[1],2,13);
                }
                break;      
        }
    }while(state != 4); 
}

void set_HM(int *a,int sn)
{
    state = 0;
    int pos = 0;
    int cur_i[] = {6,9};
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Print_String("ALARM");
    Print_Num(sn,1,7);
    Lcd_Set_Cursor(2,6);
    Lcd_Print_String("00:00   ");
    do
    {
        state = get_state();
        Print_Num(*(a+pos),2,cur_i[pos]);
        __delay_ms(100);
        Lcd_Set_Cursor(2,cur_i[pos]);
        Lcd_Print_String("__");
        switch(state)
        {
            case 1:
                *(a+pos) =*(a+pos) + 1;
                if(*a == 24) *a = 0;
                if(*(a+1)== 60) *(a+1) = 0;
                break;
            case 2:
                
                break;
            case 3:
                Print_Num(*(a+pos),2,cur_i[pos]);
                pos++;
                if(pos == 2) pos = 1;
                break;
            case 4://save and exit
                Lcd_Clear();
                Lcd_Set_Cursor(1,1);
                Lcd_Print_String("SET ALARM   :");
                Print_Num(sn,1,11);
                Print_Num(*a,2,4);
                Print_Num(*(a+1),2,7);
                Lcd_Set_Cursor(2,6);
                Lcd_Print_String(":");
                state = 0;
                state = get_state();
                while(state!=4)
                {
                    state = 0;
                    state = get_state();  
                }
                
                break;
                
        }
    }while(state != 4);// 4 exit to main, 3 exit to mode
}
// get state date of month
int Get_State_Month(int m)
{
    if(m == 4|| 
            m == 6||
            m == 9||
            m == 11) return 1;
    else if(m == 1||
            m == 3||
            m == 5||
            m == 7||
            m == 8||
            m == 10||
            m == 12) return 2;
    else return 3;
}

void moisture_display()
{
 
    Lcd_Clear();
    do{
        CHS2=0; CHS1=0; CHS0=0;
        __delay_us(25);
        GO_nDONE = 1; 
        while(GO_nDONE); 
        Adc_V1=ADRESH*256+ADRESL;
        temp = Adc_V1*0.488;
	// LCD display
	// Select AN1
        CHS2=0;  CHS1=0; CHS0=1;
        __delay_us(25);
        GO_nDONE = 1; 
        while(GO_nDONE);
        Adc_V2 = ADRESH*256+ADRESL;
        mois = Adc_V2*0.488;
        int m,t;
        RD0 = 0;
        if(temp > 50 || mois > 200)
            {
                RD0 = 1;
                __delay_ms(10);
            }
        m = (int)mois;
        t = (int)temp;
        
        Lcd_Set_Cursor(1,1);
        Lcd_Print_String("MOISTURE:   %");
        Print_Num(m,1,10);
        Lcd_Set_Cursor(2,1);
        Lcd_Print_String("TEMPERATURE:    C");
        Lcd_Set_Cursor(2,15);
        Lcd_Print_Char(0b11011111);
        Print_Num(t,2,13);
        state = get_state();
    }while(state != 4);
}








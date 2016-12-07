#ifndef _MY_TYPES_H__
#define _MY_TYPES_H__
#include <stm32f10x.h>



/*--------------------------------------------------------------------------*/
extern uint8_t DEBUG;                  //是否输出调试信息
/*--------------------------------------------------------------------------*/



/*
 * 定义带位域的联合体类型，用作系统所有的标志位处理
 */
typedef union
{
    uint32_t  DW;
    uint16_t  W[2];
    uint8_t   B[4];
    struct
    {
        uint32_t canStartDecodeFlag: 1;    
        uint32_t keyTimerStartFlag: 1; 

        uint32_t keyState: 1;  
        uint32_t keyTouchStateFlag: 1;              
		
        uint32_t b4: 1;  
        uint32_t b5: 1;   
        uint32_t b6: 1;     
        uint32_t b7: 1;    
        uint32_t b8: 1;      
		    uint32_t b9: 1;  
        uint32_t b10: 1;   
        uint32_t b11: 1;        
		   
		    uint32_t b12: 1;        
        uint32_t b13: 1;   

        uint32_t b14: 1;    
        uint32_t b15: 1;      
        uint32_t b16: 1;

        uint32_t b17: 1;      
				
				
        uint32_t b18: 1;    
        uint32_t b19: 1;     

        uint32_t b20: 1;     

        uint32_t b21: 1;  
        uint32_t b22: 1;     

        uint32_t b23: 1;      
        uint32_t b24: 1;
        uint32_t b25: 1;  
        uint32_t b26: 1;    
        uint32_t b27: 1;


        uint32_t b28: 1;
        uint32_t b29: 1;
        uint32_t b30: 1;
        uint32_t b31: 1;
    }flag;
} myFlag;    //sizeof(myFlag) 为 4


typedef struct
{
  uint16_t b0:1;
  uint16_t b1:1;
  uint16_t b2:1;
  uint16_t b3:1;
  uint16_t b4:1;
  uint16_t b5:1;
  uint16_t b6:1;
  uint16_t b7:1;
  uint16_t b8:1;
  uint16_t b9:1;
  uint16_t b10:1;
  uint16_t b11:1;
  uint16_t b12:1;
  uint16_t b13:1;
  uint16_t b14:1;
  uint16_t b15:1;
}myFlag2;


/**
 *  @brief 变量的位清0和置1
 */
#define BIT_CLEAN(var,n)        (var) &= ~(1<<(n))   //变量var 的n位（即第n+1位）清0
#define BIT_SET(var,n)          (var) |=  (1<<(n))   //变量var 的n位（即第n+1位）置1
#define BIT_GET(var,n)          (((var)>>(n))&0x01)  //读取变量var 的n位（即第n+1位）

/*
 * 确保x的范围为 min~max
 */
#define RANGE(x,min,max)        ((uint8_t)((x)<(min) ? (min) : ( (x)>(max) ? (max):(x) )))


/*
*取32位的高8位 第3个8位，低2个8位，低8位
*/

#define HI_UINT32_T(a) (((a) >> 24) & 0xFF)
#define L3_UINT32_T(a) (((a) >> 16) & 0xFF)
#define L2_UINT32_T(a) (((a) >> 8) & 0xFF)
#define LO_UINT32_T(a) ((a) & 0xFF)

/*
*取16位的高8位和低8位
*/
#define HI_UINT16_T(a) (((a) >> 8) & 0xFF)
#define LO_UINT16_T(a) ((a) & 0xFF)


/*
*取8字节的高4位和低4位
*/
#define HI_UINT8_T(a) (((a) >> 4) & 0x0F)
#define LO_UINT8_T(a) ((a) & 0x0F)


/*
*将四个字节转换为32位
*/
#define BUILD_UINT32_T(Byte0, Byte1, Byte2, Byte3) \
          ((uint32_t)((uint32_t)((Byte0) & 0x00FF) \
          + ((uint32_t)((Byte1) & 0x00FF) << 8) \
          + ((uint32_t)((Byte2) & 0x00FF) << 16) \
          + ((uint32_t)((Byte3) & 0x00FF) << 24)))

/*
*将两个字节转换位16位
*/
#define BUILD_UINT16_T(loByte, hiByte) \
          ((uint16_t)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))


/*
 * 宏定义实现返回绝对值（x里不能有自加自减的语句，否则变量出错）
 */
#define MY_ABS(x) (((x) > 0) ? (x) : (-(x)))


extern myFlag FlagBit;
extern myFlag2 FlagBit2;


#endif


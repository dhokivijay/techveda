
#define  SECOND         0x00  //Second       (00..59) 
#define  MINUTE         0x02  //Minute       (00..59)
#define  HOUR           0x04  //Hour         (00..23) 
#define  DAY_IN_WEEK    0x06  //Day of week  (01..07)
#define  DAY            0x07  //Day          (01..31) 
#define  MONTH          0x08  //Month        (01..12) 
#define  YEAR           0x09 // Year         (00..99) 

#define  ADDRESS_REG       0x70
#define  DATA_REG          0x71
#define  ADDRESS_REG_MASK  0xe0

struct rtc_time {
        unsigned int sec;
        unsigned int min;
        unsigned int hour;
        unsigned int day;
        unsigned int mon;
        unsigned int year;

};

# define SUCCESS 0
# define DEV_NAME "myrtc"








/* ioctl implementation */

/* 
   step 1: indentify config ops possible on target device
   step 2: create "request code" for each config operation
   step 3: implement ioctl call back function and register it with 
	   "unlocked_ioctl" interface of file_operations structure.

 #define CONFIG0  0
 #define CONFIG1  10



   To encode request commnands the following params are needed
    1. MAGIC NO(8 bit)(common for all commands)
    2. Seq no(8 bit) (incremental)
    3. Arguement type(can vary for each command: may or may not be applicable)
    4. direction of data transfer( in | out param: may or may not be applicable)

 encoding macros:
    _IO(MAGIC,seqno)
	-- use this when arg is not involved 
    _IOW(MAGIC, seqno, type)
	-- use this when arg is in-param for application
    _IOR(MAGIC, seqno, type)
	-- use this when arg is out-param for application
    _IOWR(MAGIC, seqno, type)
	-- use this if arg is used both as in/out-param.

*/













#define VEDA_MAGIC       'V'

#define  SET_SECOND  _IOW(VEDA_MAGIC, 1, char)
#define  SET_MINUTE  _IOW(VEDA_MAGIC, 2, char)
#define  SET_HOUR    _IOW(VEDA_MAGIC, 3, char)
#define  SET_DAY     _IOW(VEDA_MAGIC, 4, char)
#define  SET_MONTH   _IOW(VEDA_MAGIC, 5, char)
#define  SET_YEAR    _IOW(VEDA_MAGIC, 6, char)

























/* Example -- Vga card (vramdev) ioctls

1. VGA_FILLZERO
#define VGA_FILLZERO 	_IO(MAGIC,seqno)

2. VGA_FILLCHAR
#define VGA_FILLCHAR	_IOW(MAGIC,seqno,char)

3. VGA_GETSIZE
#define VGA_GETSIZE	_IOR(MAGIC,seqno,uint)

4. VGA_SETSIZE
#define VGA_SETSIZE 	_IOW(MAGIC,seqno,uint)
*/

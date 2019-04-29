/* This program shows how to access the io details from a user
space program ( not inside kernel )
Author : Team - C
Version : 1.0
Remarks : we should run this program as root user
*/
# include <stdio.h>
# include <unistd.h>
# include <sys/io.h>

/* Address offsets into rtc's internal eeprom (chip maintains Date/Time vals here)*/
#define  SECOND         0x00  //Second       (00..59) 
#define  MINUTE         0x02  //Minute       (00..59)
#define  HOUR           0x04  //Hour         (00..23) 
#define  DAY_IN_WEEK    0x06  //Day of week  (01..0DAY)
#define  DAY            0x07  //Day          (01..31) 
#define  MONTH          0x08  //Month        (01..12) 
#define  YEAR           0x09 // Year         (00..99) 

/* cmos-rtc PIO address space on IBM PC arch  */
#define  ADDRESS_REG       0x70
#define  DATA_REG          0x71
#define  ADDRESS_REG_MASK  0xe0

int main()
{
	
	/* get permission to access rtc port address region */
	int res,a,b;

	res= ioperm(ADDRESS_REG, 0x04, 1);/* we can access ports 70,71,72,73*/
	printf("res =%d \n",res);

	/*Access reg using inb() and outb() functions */

	/* to access year */
	outb(YEAR ,ADDRESS_REG);
	printf(" year= %x \n",inb(DATA_REG));
 
	/* to access month */
	outb(MONTH,ADDRESS_REG);
	printf(" month = %x \n",inb(DATA_REG));

	/* to access day */
	outb(DAY,ADDRESS_REG);
	printf(" day = %x \n",inb(DATA_REG));
	return 0;
}

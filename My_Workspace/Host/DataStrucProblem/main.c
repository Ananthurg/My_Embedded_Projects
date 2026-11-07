
	#include<stdio.h>
	#include<stdint.h>


	struct Packet
	{
		short crc;
		char status;
		long payload;
		int bat;
		int sensor;
		long longAddr;
		short shortAddr;
		char addrMode;

	};

	struct Packet Data;

	int main (void)
	{
		uint8_t packetValue;
		printf("Enter the 32 bit Packets:\n");
		scanf("%X", &packetValue);


		Data.crc 		= (packetValue & 0x02);
		Data.status	    = ((packetValue >> 2) & 0x01);
		Data.payload 	= ((packetValue >> 3) & 0xFFF);


		printf("CRC value 		: %#x \n", Data.crc);
		printf("status value 	: %#x \n", Data.status);
		printf("payload value 	: %#lx \n", Data.payload);


		getchar();

		while(getchar() != '\n')
			    {

			    }
			    getchar();
			    return 0;
	}





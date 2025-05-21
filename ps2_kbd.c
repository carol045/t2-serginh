#include <hf-risc.h>

#define KBD_AXI_BASE			0xe4a90000
#define KBD_AXI_STATUS			(*(volatile uint32_t *)(KBD_AXI_BASE + 0x010))
#define KBD_AXI_DATA			(*(volatile uint32_t *)(KBD_AXI_BASE + 0x020))

#define KBD_AXI_STREADY			(1 << 0)
#define KBD_AXI_STVALID			(1 << 1)

uint8_t read_keyboard(){
	while (!(KBD_AXI_STATUS & KBD_AXI_STVALID));
	return KBD_AXI_DATA;
}

char return_key(){
	uint8_t tecla;
	uint8_t c;

	while(1){
		tecla = read_keyboard();

		if((tecla & 0xF0) == 0xF0){
			tecla = read_keyboard();
			break;
		}
	}

	switch (tecla){
		case 0x1C:
			c = 'A';
		break;
		case 0x21:
			c = 'C';
			break;
		case 0x44:
			c = 'O';
			break;
		case 0x4B:
			c = 'L';
			break;
		case 0x2D:
			c = 'R';
			break;
	default:
		c = 0;
		break;
	}

	return c;
}

int main(void){
	printf("=====================\n");
	printf("==     T2 - CSD    ==\n");
	printf("=====================\n\n");
	
	while (1) {
		uint8_t letra = return_key();
		printf("%c", letra);
	}

	return 0;
}

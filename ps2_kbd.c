#include <hf-risc.h>

#define KBD_AXI_BASE			0xe4a90000
#define KBD_AXI_STATUS			(*(volatile uint32_t *)(KBD_AXI_BASE + 0x010))
#define KBD_AXI_DATA			(*(volatile uint32_t *)(KBD_AXI_BASE + 0x020))

#define KBD_AXI_STREADY			(1 << 0)
#define KBD_AXI_STVALID			(1 << 1)

uint8_t get_key(){
	uint8_t data;
	while (!(KBD_AXI_STATUS & KBD_AXI_STVALID));
	data = KBD_AXI_DATA;
	
	return data;
}

char return_key()[
	char c;
	uint8_t valor; //Endereco na LUT?
	valor = get_key();

	switch (valor){
		case 0x1C:
			c = 'A';
		break;
	
	default:
		c = ' ';
		break;
	}
	

	return c;
}

int main(void){
	printf("=====================\n");
	printf("==     T2 - CSD    ==\n");
	printf("=====================\n\n");
	
	while (1) {
		printf("%c", return_key());
		delay_ms(1000);		
	}

	return 0;
}
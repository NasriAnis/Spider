#include "../drivers/screen.h"
#include "../arch/init.h"

void kmain(void){
	printnl("Welcome to the Spider Os kernel !");
	init_arch();
	printnl("Init architecture finished !");
	while (1) {
	}
}
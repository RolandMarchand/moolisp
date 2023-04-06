#include "common.h"
#include "env.h"

tgc_t gc;

void _main(void);

int main(void)
{
	volatile void *dummy;
	tgc_start(&gc, &dummy);
	_main();
	tgc_stop(&gc);

}

void _main(void)
{
}

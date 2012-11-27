/*******************************************/
/* Jason Birch   2012-11-26   V1.00        */
/* Joystick control for Raspberry Pi GPIO. */
/*******************************************/

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <fcntl.h>
#include <sys/mman.h>

#include "portable.h"

#include "jRPi.h"
#include "log.h"
#include "error.h"


static adv_device DEVICE[] = {
{ "auto", -1, "RPi joystick" },
{ 0, 0, 0 }
};


// Raspberry Pi V1 GPIO
// int GPIO_Pin[] = { 0, 1, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25 };
// Raspberry Pi V2 GPIO
int GPIO_Pin[] = { 2, 3, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27 };
char GPIO_Filename[RPI_BUTTONS][BUFF_SIZE];
int RPiButtons[RPI_BUTTONS];

int GpioFile;
char* GpioMemory;
char* GpioMemoryMap;
volatile unsigned* GPIO;
int GPIO_Mask[] = {
                     0x00000004,
                     0x00000008,
                     0x00000010,
                     0x00000080,
                     0x00000100,
                     0x00000200,
                     0x00000400,
                     0x00000800,
                     0x00004000,
                     0x00008000,
                     0x00020000,
                     0x00040000,
                     0x00400000,
                     0x08000000,
                     0x00000000,
                     0x00000000,
                     0x00000000
                  };


adv_error joystickb_RPi_init(int joystickb_id)
{
   FILE* File;
   int Index;
   char Buffer[BUFF_SIZE];

	log_std(("joystickb:RPi: joystickb_RPi_init(id:%d)\n", joystickb_id));

   memset(RPiButtons, '\0', sizeof(RPiButtons));

   for (Index = 0; Index < sizeof(GPIO_Pin) / sizeof(int); ++Index)
   {
      sprintf(Buffer, "/sys/class/gpio/export");
      if (!(File = fopen(Buffer, "w")))
         printf("Failed to open file: %s\n", Buffer);
      {
         fprintf(File, "%u", GPIO_Pin[Index]);
         fclose(File);

         sprintf(Buffer, "/sys/class/gpio/gpio%u/direction", GPIO_Pin[Index]);
         if (!(File = fopen(Buffer, "w")))
            printf("Failed to open file: %s\n", Buffer);
         {
            fprintf(File, "in");
            fclose(File);

            sprintf(GPIO_Filename[Index], "/sys/class/gpio/gpio%u/value", GPIO_Pin[Index]);
         }
      }
   }

   GPIO = NULL;
   GpioMemory = NULL;
   if ((GpioFile = open("/dev/mem", O_RDWR | O_SYNC)) < 0)
      printf("Failed to open memory\n");
   else
   {
      if (!(GpioMemory = malloc(BLOCK_SIZE + PAGE_SIZE - 1)))
         printf("Failed to allocate memory map\n");
      else
      {
         if ((unsigned long)GpioMemory % PAGE_SIZE)
            GpioMemory += PAGE_SIZE - ((unsigned long)GpioMemory % PAGE_SIZE);

         if ((long)(GpioMemoryMap = (unsigned char*)mmap((caddr_t)GpioMemory, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, GpioFile, GPIO_BASE)) < 0)
            printf("Failed to map memory\n");
         else
            GPIO = (volatile unsigned*)GpioMemoryMap;
      }
   }

	return 0;
}


void joystickb_RPi_done(void)
{
	log_std(("joystickb:RPi: joystickb_RPi_done()\n"));

   close(GpioFile);
}


unsigned joystickb_RPi_count_get(void)
{
	log_debug(("joystickb:RPi: joystickb_RPi_count_get()\n"));

	return 1;
}


unsigned joystickb_RPi_stick_count_get(unsigned joystick)
{
	log_debug(("joystickb:RPi: joystickb_RPi_stick_count_get()\n"));

	return 1;
}


unsigned joystickb_RPi_stick_axe_count_get(unsigned joystick, unsigned stick)
{
	log_debug(("joystickb:RPi: joystickb_RPi_stick_axe_count_get()\n"));

	return 0;
}


unsigned joystickb_RPi_button_count_get(unsigned joystick)
{
	log_debug(("joystickb:sdl: joystickb_sdl_button_count_get()\n"));

	return sizeof(GPIO_Pin) / sizeof(int);
}


unsigned joystickb_RPi_button_get(unsigned joystick, unsigned button)
{
	log_debug(("joystick:RPi: joystick_RPi_button_get()\n"));

	return RPiButtons[button];
}


int joystickb_RPi_stick_axe_analog_get(unsigned joystick, unsigned stick, unsigned axe)
{
	log_debug(("joystickb:RPi: joystickb_RPi_stick_axe_analog_get()\n"));

	return 0;
}

unsigned joystickb_RPi_stick_axe_digital_get(unsigned joystick, unsigned stick, unsigned axe, unsigned d)
{
	log_debug(("joystickb:RPi: joystickb_RPi_stick_axe_digital_get()\n"));

	return 0;
}


void joystickb_RPi_poll(void)
{
   FILE* File;
   int Index;
   int Char;

	log_debug(("joystickb:RPi: joystickb_RPi_poll()\n"));

   for (Index = 0; Index < RPI_BUTTONS; ++Index)
   {
      if (!GPIO)
      {
         File = fopen(GPIO_Filename[Index], "r");
         Char = fgetc(File);
         fclose(File);

         if (Char == '0')
            RPiButtons[Index] = 1;
         else
            RPiButtons[Index] = 0;
      }
      else
      {
         if (((int*)GPIO)[GPIO_ADDR_OFFSET] & GPIO_Mask[Index])
            RPiButtons[Index] = 0;
         else
            RPiButtons[Index] = 1;
      }
   }
}


unsigned joystickb_RPi_flags(void)
{
	return 0;
}


adv_error joystickb_RPi_load(adv_conf* context)
{
	return 0;
}


void joystickb_RPi_reg(adv_conf* context)
{
}


/**********/
/* Driver */
/**********/
joystickb_driver joystickb_rpi_driver = 
{
	"rpi",
	DEVICE,
	joystickb_RPi_load,
	joystickb_RPi_reg,
	joystickb_RPi_init,
	joystickb_RPi_done,
	0,
	0,
	joystickb_RPi_flags,
	joystickb_RPi_count_get,
	joystickb_RPi_stick_count_get,
	joystickb_RPi_stick_axe_count_get,
	0,
	0,
	joystickb_RPi_stick_axe_digital_get,
	joystickb_RPi_stick_axe_analog_get,
	joystickb_RPi_button_count_get,
	0,
	joystickb_RPi_button_get,
	0,
	0,
	0,
	0,
	0,
	joystickb_RPi_poll
};


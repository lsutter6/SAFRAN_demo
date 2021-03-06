/*
        demo-udp-03: udp-recv: a simple udp server
	receive udp messages

        usage:  udp-recv

        Paul Krzyzanowski
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <bcm2835.h>

#define MAXCHAR 5
#define PIN RPI_GPIO_P1_12
#define PWM_CHANNEL 0
#define RANGE 2048
#define MIN_DATA 900
#define MAX_DATA 1800
int direction = 1;
int data = 1300;

#define DELAY_TIME 3000
#define OPERATING_TIME 30000

int PWM_setup(){
	if (!bcm2835_init())
	return 1;
	
	bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_ALT5);
    bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_16);
    bcm2835_pwm_set_mode(PWM_CHANNEL, 1, 1);
    bcm2835_pwm_set_range(PWM_CHANNEL, RANGE);
    bcm2835_pwm_set_data(PWM_CHANNEL, MIN_DATA);
    bcm2835_delay(10000); //wait for a motor to be initialized
    
    return 0;
}

void PWM_send(int PWM){
	/* For Testing PWM only */
	extern int direction;
	extern int data;
	bcm2835_pwm_set_data(PWM_CHANNEL, PWM); //motor starts at 1378
	/*if (data == 1350)
	    direction = 1;   // Switch to increasing
	else if (data == 1400)
	    direction = -1;  // Switch to decreasing
	data += direction;
	bcm2835_pwm_set_data(PWM_CHANNEL, data);
	printf("%d\n", data);*/
	//bcm2835_delay(1);
	/* End for Testing PWM */
}

int main(int argc, char **argv)
{	
	FILE *f_log = fopen("start_logging.txt","w");
	if(f_log == NULL){
		printf("Error opening file start_logging.txt");
	}
	
	FILE *f_sensor = fopen("pwm.txt","r");
	if(f_sensor == NULL){
		printf("Error opening file pwm.txt");
	}
	char str_pwm[MAXCHAR];
	PWM_setup();
	int PWM = 1000;
	unsigned long i = 0;	
	
	while(1){
		f_sensor = fopen("pwm.txt","r");
		if(f_sensor == NULL){
			printf("Error opening file pwm.txt");
		}else{
			fgets(str_pwm, MAXCHAR, f_sensor);
		}
		fclose(f_sensor);
		PWM = atoi(str_pwm);		
		
		if(i < DELAY_TIME){			
			rewind(f_log);
			fprintf(f_log, "2");
			PWM = 1300;
			i++;
		}else if(i >= DELAY_TIME && i < DELAY_TIME + OPERATING_TIME){
			//PWM = 1481; //Open Loop
			rewind(f_log);
			fprintf(f_log, "1");
			i++;
		}else if(i >= DELAY_TIME + OPERATING_TIME && i < DELAY_TIME + OPERATING_TIME + DELAY_TIME){
			rewind(f_log);
			fprintf(f_log, "2");
			PWM = 1300;
			i++;
		}else{
			rewind(f_log);
			fprintf(f_log, "0");
			PWM = MIN_DATA;
			fclose(f_log);	
		}		
		
		//printf("%lu,%d\n",i,PWM);
		PWM_send(PWM);
		
		//delay(1);	
		bcm2835_delay(1);		
	}
	return 0;
}

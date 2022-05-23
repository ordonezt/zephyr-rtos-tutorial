#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

const struct device *dev;

/* Devices */
#define LED0_NODO   DT_ALIAS(led0)
#define LED0        DT_GPIO_LABEL(LED0_NODO, gpios)
#define LED0_PIN	DT_GPIO_PIN(LED0_NODO, gpios)
#define LED0_FLAGS	DT_GPIO_FLAGS(LED0_NODO, gpios)
/* Devices end */

/* Timers */
#define FAST_TOGGLE  50
#define SLOW_TOGGLE  1000

void fast_timer_expiry(struct k_timer *timer_id);
void slow_timer_expiry(struct k_timer *timer_id);

K_TIMER_DEFINE(fast_timer, fast_timer_expiry, NULL);
K_TIMER_DEFINE(slow_timer, slow_timer_expiry, NULL);

void fast_timer_expiry(struct k_timer *timer_id){
    gpio_pin_toggle(dev, LED0_PIN);
}
void slow_timer_expiry(struct k_timer *timer_id){
    static char estado = 0;

    if(estado){
        //Le doy arranque al timer rapido
        k_timer_start(&fast_timer, K_MSEC(FAST_TOGGLE), K_MSEC(FAST_TOGGLE));
    } else {
        //Pauso el timer rapido
        k_timer_stop(&fast_timer);
        //Apago el led por las dudas
        gpio_pin_set(dev, LED0_PIN, 0);
    }

    estado = !estado;
}
/* Timers end */

/* Threads */
#define BLINKY_STACK_SIZE   500
#define BLINKY_PRIORITY     5
void blinky_thread(void *, void *, void *);

K_THREAD_DEFINE(blinky_tid, BLINKY_STACK_SIZE,
                blinky_thread, NULL, NULL, NULL,
                BLINKY_PRIORITY, 0, 0);

void blinky_thread(void *arg1, void *arg2, void *arg3){
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    dev = device_get_binding(LED0);

    if(dev == NULL){
        printk("LED0 Binding error\n");
        return;
    }
    
    if(gpio_pin_configure(dev, LED0_PIN, GPIO_OUTPUT | LED0_FLAGS)){
        printk("LED0 Pin configure error");
        return;
    }

    k_timer_start(&slow_timer, K_MSEC(SLOW_TOGGLE), K_MSEC(SLOW_TOGGLE));

    while(1){
        printk("Blinky thread: Keep alive!");
        k_msleep(5000);
    }

}
/* Threads end */


/* Al igual que con los threads, los devices se pueden crear o instanciar tanto en
tiempo de compilacion como de ejecucion, lo recomendable es hacerlo en la compilacion
*/

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

/* Obtengo el nodo del device tree que voy a usar 
    led 0 es el alias que tiene el led, para ver eso
    te metes en /zephyrproject/zephyr/boards/arm/stm32f103_mini
    si la placa es distinta la buscas de forma similar
*/
#define LED0_NODO   DT_ALIAS(led0)

/* Obtenemos el label del led0 */
#define LED0        DT_GPIO_LABEL(LED0_NODO, gpios)

/* Obtenemos el pin del led0 */
#define LED0_PIN	DT_GPIO_PIN(LED0_NODO, gpios)

/* Obtenemos los flags que pueda tener el led0 */
#define LED0_FLAGS	DT_GPIO_FLAGS(LED0_NODO, gpios)

#define TOGGLE_DELAY    200

void main(void){
    const struct device *dev;
    int ret;

    dev = device_get_binding(LED0);

    if(dev == NULL){
        printk("LED0 Binding error\n");
        return;
    }

    ret = gpio_pin_configure(dev, LED0_PIN, GPIO_OUTPUT | LED0_FLAGS);
    
    if(ret){
        printk("LED0 Pin configure: ");
        switch(ret){
            case -ENOTSUP:
                /* if any of the configuration options is not supported (unless otherwise directed by flag documentation). */
                printk("Operacion no soportada\n");
                break;
            case -EINVAL:
                /* Invalid argument. */
                printk("Argumento invalido\n");
                break;
            case -EIO:
                /* I/O error when accessing an external GPIO chip. */
                printk("Error de I/O\n");
                break;
            case -EWOULDBLOCK:
                /* if operation would block. */
                printk("La operacion sera bloqueante\n");
                break;
            default:
                break;
        }
        return;
    }

    while(1){
        ret  = gpio_pin_toggle(dev, LED0_PIN);
        if(ret){
            printk("LED0 Toggle error: ");
            switch(ret){
                case -EIO:
                /* I/O error when accessing an external GPIO chip. */
                printk("Error de acceso a I/O\n");
                break;
                case -EWOULDBLOCK:
                /* if operation would block. */
                printk("La operacion sera bloqueante\n");
                break;
            }
        }
        k_msleep(TOGGLE_DELAY);
    }

}
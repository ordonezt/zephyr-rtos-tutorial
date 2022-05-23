El autor del tutorial te recomienda clonar el repo bajo la 
carpeta zephyr-project, esto es porque (o supongo, porque 
no se una mierd@) en esa carpeta ya tenes exportada la 
variable ZEPHYR_BASE, que es de donde va a buscar todo el
codigo del kernel y demas cosas del proyecto...
Si elegis clonarlo en cualquier otro lado (como hice yo) es
necesario que le indiques a west donde ir a buscar estas 
cosas, para hacer eso tenes que exportar la variable 
ZEPHYR_BASE de la siguiente manera:

export ZEPHYR_BASE=/home/TU_USUARIO/zephyr-project/zephyr

Ojo, esta manera de exportar la variable es temporal y solo 
sirve en la terminal que lo hiciste, es decir que si cerras
la terminal o apagas la compu lo tenes que hacer de nuevo,
hay otras maneras de hacerlo y que quede permanente pero yo 
tengo cagazo de dejar algo mal configurado y que despues no 
ande nada y por eso lo hice asi, eso lo podes ver en:

https://docs.zephyrproject.org/latest/develop/env_vars.html#env-vars

Bueno... una vez que exportaste esa variable west ya sabe de
donde agarrarse, lo unico que queda hacer es buildear el proyecto

west build -b TU_PLACA

donde TU_PLACA es el hardware que vos quieras, como yo al momento
de hacer esto no tengo ninguna use un emulador asi que si vas a 
usar una placa real puede que falte algun paso mas, como elegir
el toolchain, no lo se realmente. Bueno, para usar el emulador
reemplazas TU_PLACA por "qemu_x86", si tu compu tiene otra
arquitectura creo que hay otros emuladores, buscalo.

Si salio todo bien el proyecto deberia haber buildeado, te tira
cuanto espacio usaste y eso. Ahora para correrlo tenes que hacer

west build -t run

Y listo papa!

Aparentemente el emulador qemu_x86 tiene un bardo con el timing 
(no se si es un error exactamente), se soluciona tocando el 
kconfig pero todavia no entiendo como.
Ver: https://github.com/zephyrproject-rtos/zephyr/issues/26242
https://docs.zephyrproject.org/1.14.0/reference/kconfig/CONFIG_QEMU_TICKLESS_WORKAROUND.html

La solucion que encontre es compilar con otro emulador -> qemu_x86_64
Para poder "re-buildear" el proyecto con otra placa tenes que usar el mismo comando pero 
agregarle una -p, o sea:

west build -b qemu_x86_64

Para ver los mensajes de consola del micro podes usar un adaptador USB-UART, lo conectas
y te fijas que puerto le asigno tu pc asi

dmesg | grep tty

Cuando encuentres el puerto asignado usa esto (puede que tengas que instalar minicom
sudo apt-get install minicom)

sudo minicom -D /dev/ttyUSB0

En vez de ttyUSB0 pone el que corresponda
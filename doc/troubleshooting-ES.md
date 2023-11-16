# Resolución de problemas

- ¡Cometí un error y ahora no puedo usar el ratón!

  - La app puede manejarse con el teclado, usa TAB para moverte ó ALT + Letra subrayada seleccionar un control o clickar un botón. ALT + M Cambia el modelo de ratón en la lista.

- No entiendo BTN_BACK, BTN_SIDE, BTN_EXTRA, etc...

  - Hay muchísmos tipos de botón definidos en Linux. Lo siento, pero para mí es una tarea enorme ponerme a traducirlos. Muchos nombres son autoexplicativos, sino siempre puedes buscar en internet.

- Uno o varios botones del layout por defecto me salen en la parte izquierda como BTN_NONE.

  - No todos los tipos de botones en Linux aparecen visibles, son 118!. Edita el fichero de configuración en tu HOME **./config/qmice-remap/qmice-remap.ini** y descomenta (quitando el "#") los que quieras usar para tu modelo.

- Error **qt.qpa.plugin: Could not find the Qt platform plugin "wayland" in ""**

  - Es posible (no siempre) que la app no arranque y salga este error. Una solución es instalar en tu distro el paquete qt6-wayland. Si te aparece el error pero la app arranca, no tienes que preocuparte.

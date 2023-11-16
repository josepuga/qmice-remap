# Troubleshooting

- I made a mistake and now I cannot use the mouse!

  - You can use this app without mouse. Move with TAB or press ALT + underline word to selec a control. ALT + M to change Mouse Model.

- I don't understand BTN_BLACK, BTN_SIDE, BTN_EXTRA, etc...

  - There are a lot of defined buttons on Linux. I'm sorry, but for me is a bigger task to translate. Many names are self explainatory, in any case you can always look up on internet.

- One or more buttons of default layout are BTN_NONE in the left side.

  - No all buttons types availables on Linux are visibles, they are 118!. Edit the config file at your HOME **./config/qmice-remap/qmice-remap.ini** and uncomment (removing "#") the ones you want to use for your model.

- Error **qt.qpa.plugin: Could not find the Qt platform plugin "wayland" in ""**

  - It is possible (not always) that the app doesnt launch and this error appear. One solution is install with your distro package manager the package qt6-wayland. If that error appears but starts, don't worry about that.

# ToDo list

## Now
### Tasks
- apply GuiView en MainPanel
- revisar Load/Save Settings and Project. Hace falta diferenciarlos? open Project dropdown??
- revisar data saving render
- Save Analysis to Modal Menu
- Add: Render algorithm analysis to track (!)


### Bugs / Code Improvements
- Definir: va o no el "_" en las variables
- Sacar "//------" de a poco...
- Revisar Configuration Class


**********************************************************************************************
**********************************************************************************************
## Notes
- En algun momento hay que cortar las new features para la 1.0 porque esto no termina mas -> La version 1.0 = lo que necesito para laburar con los Estudios Audiovisuales. 
- Desactivar por ahora el channelSplit. v1.0 sin esa opcion. Desarrollar UI app pensando para un archivo mono o un solo analisis por archivo. Los distintos canales son gralmente una cuestion de espacializacion. De ultima, se renderizan varios analisis y despues se superponen. El SonoscopioRT si tiene sentido que sea multicanal.

## Backlog

- Testear TLNotes get values.


## VERSION 1.0
- Revisar thread mutex locks en draw y update, con el data saver: http://openframeworks.cc/ofBook/chapters/threads.html
- Check time measurement of drawing methods.
- Hard degub testing.

- Compilar con Essentia as OSX standalone app, sin necesidad de compilar?
- Add ICON
- Add Copyright


## VERSIONES FUTURAS
- Mute Channels option.
- Test Linux compilation.

    





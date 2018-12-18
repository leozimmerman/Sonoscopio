# ToDo list

## Now
### Tasks 

- Fix keyboard shortcuts.
    - Cmd+S: Save 
    - Cmd+T: Time measurements

### Bugs / Code Improvements
- Definir: va o no el "_" en las variables ->  Va si hace falta diferenciar....
- Sacar "//------" de a poco...


**********************************************************************************************

## Backlog
- Check memory management
- Tristimulus anda?
- Organize Xcode project and directory

## Notes
- En algun momento hay que cortar las new features para la 1.0 porque esto no termina mas -> La version 1.0 = lo que necesito para laburar con los Estudios Audiovisuales. 

- Desactivar por ahora el channelSplit. v1.0 sin esa opcion. Desarrollar UI app pensando para un archivo mono o un solo analisis por archivo. Los distintos canales son gralmente una cuestion de espacializacion. De ultima, se renderizan varios analisis y despues se superponen. El SonoscopioRT si tiene sentido que sea multicanal.



## VERSION 1.0
- Revisar thread mutex locks en draw y update, con el data saver: http://openframeworks.cc/ofBook/chapters/threads.html : No hace falta mutex porque el audio no se maneja en thread separada. Esto va para cuando se agregue el audio en RT.

- Check time measurement of drawing methods.
- Compilar con Essentia as OSX standalone app, sin necesidad de compilar?
- Add ICON
- Test Linux compilation.
- Add Copyright


## Ideas
- Add algorithm descriptions to meter popup.



    





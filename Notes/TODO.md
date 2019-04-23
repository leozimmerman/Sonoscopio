# ToDo list

## Now
### Bugs: 

### Tasks:
- Add GUI for toggling which OSC data to send: SendVectorValues, FrameNumber, ....

### Enhancements:
- Onsets Armed -> popup for selecting the track to trigger.
- Improve Markers add/remove/edit.

**********************************************************************************************

## Backlog
- Check memory management
- Organize Xcode project and directory


## VERSION 1.0
- Revisar thread mutex locks en draw y update, con el data saver: http://openframeworks.cc/ofBook/chapters/threads.html : No hace falta mutex porque el audio no se maneja en thread separada. Esto va para cuando se agregue el audio en RT.

- Check time measurement of drawing methods.
- Compilar con Essentia as OSX standalone app, sin necesidad de compilar?
- Add ICON
- Test Linux compilation.
- Add Copyright


## Ideas
- Add algorithm descriptions to meter popup.



    





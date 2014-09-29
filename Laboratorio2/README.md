EmTrack
=======

EmTrack es un plug-in para el Cliente de Chat Adium de la plataforma OSX que permite generar emoticones a partir de capturas del rostro del usuario. Para ello se utliza un atajo del teclado y comunicación UDP con la aplicación OpenFrameworks emotion-tracker.

EmTrack está desarrollado en Objective-C basado en tutoriales para la creación de plug-ins de Adium

https://trac.adium.im/wiki/CreatingPlugins

Por otra parte emoticon-tracker está desarrollado en C y se basa en ejemplo example-expression del Addon ofxFaceTracker de OpenFrameworks

https://github.com/kylemcdonald/ofxFaceTracker

La comunicación entre EmTrack y emoticon-tracker se realiza a través del protocolo UDP utilizando la librería AsyncCocoaSocket para Objective-C (https://github.com/robbiehanson/CocoaAsyncSocket) y el Addon ofxNetwork de OpenFrameworks.

Este prototipo por motivos de alcance no implementa el ingreso automático del emoticón en la ventana del chat, sino que utiliza como puente entre las dos aplicaciones el portapapeles del Sistema Operativo.




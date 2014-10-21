Laboratorio 3 - Quiénes te rodean en la sala?
=======

Quiénes te rodean en la sala? es una aplicación que presenta de forma interactiva los primeros datos sobre imaginarios y consumos culturales 2014 presentados por el ministerio de educación y cultura de Uruguay.

Esta aplicación ofrece una visualización contextualizada en una sala de cine 3D que pretende brindar un marco de referencia para la presentación de estadísticas y el cruzamiento de datos.

Se ofrece la posibilidad de configurar la aplicación mediante una serie de parámetros, permitiendo graficar y combinar distintos datos. Estos parámetros son:

- question:string, descripción del dato en representación.
- yesAnswer:integer, valor entero entre 0 y 100 que representa el porcentaje positivo.
- noAnswer:integer, valor complementario a noAnswer.
- IconPath:string, ruta al icono que representa al dato.
- IconName:string, utilizado para distinguir al icono internamente.
- modelPath:string, ruta al modelo 3D utilizado. 
- modelColorR, modelColorG, modelColorB:integer, color utilizado para el modelo 3D.
- modelScale:real, escala del modelo 3D.

Esta aplicación esta construida utilizando [openframeworks](http://openframeworks.cc) y los addons 3DModelLoaderExample, ofxGui y ofxXmlSettings. 



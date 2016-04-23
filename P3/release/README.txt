ev3c-addons is a C library with addons for the ev3c wrapper 
library of the Mindstorms EV3 brick using ev3dev.

Please, 
* find the doc in ./ev3c_addons_documentation
* find the headers in ./include
* find the main library in ./lib
* find the extra libraries in ./lib (libespeak.a libsonic.so libasound.so). 

C applications using the text-to-speech engine should include the following libraries when compiling:
libespeak.a libsonic.so libasound.so -lm -lpthread 

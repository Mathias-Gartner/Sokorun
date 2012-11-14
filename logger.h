//Version 0.1
//Jakob Maier


#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

/** EINSTELLUNGEN **/
    #define logPRINTF_BUFFER 1024												                    //max. Stringlänge für die Funktionen logger() und error()


/** PROTOTYPEN **/

void logger(bool mode,const char *format,...);                                                                //Logger (Gibt Daten in der Konsole aus, um bestimmte Programzustände zu melden)
void error(const char *source,const char *format,...);                                              //Error-Logger (Gibt eine Fehlermeldung aus)

#endif // LOGGER_H_INCLUDED

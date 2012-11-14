//Version 0.2
//Jakob Maier
//Allg. Grundfunktionen fürs Debugging



//Übergabeparameter:
//  1.) mode                            Ob am Einde eine neue Zeile ausgegeben werden soll wenn keine vorhanden ist und die Farbe gewechselt werden soll (1); Wenn 0: Wie ein normales printf()
//  1.) Formatierter String             Auszugebende Daten
//  2 - n)  Zusätzliche Daten
void logger(bool mode,const char *format,...)                                       //Logger (Gibt Daten in der Konsole aus, um bestimmte Programzustände zu melden)
{   if(!DEBUG) return;

	static char buffer[logPRINTF_BUFFER+1];											//Buffer
	va_list  argptr;																//Argument-Liste
	va_start( argptr, format );
	vsprintf( buffer, format, argptr );												//Mit sprintf-Funktion in Buffer übertragen
	va_end  ( argptr );
	buffer[logPRINTF_BUFFER]=0;														//Zur Sicherheit
	if(mode==1) textcolor(ccWHITE);
        printf("%s",buffer);
        if(mode==1 && buffer[strlen(buffer)-1]!='\n') putc('\n',stdout);
	if(mode==1) textcolor(ccLIGHTGRAY);
}


//Übergabeparameter:
//  1.) source        [string]          Wo das Fehler aufgetreten ist (zB. Unterprogramm-Name, Datei,...)
//  2.) Formatierter String             Was pasiert ist
//  3 - n)  Zusätzliche Daten
void error(const char *source,const char *format,...)                               //Error-Logger (Gibt eine Fehlermeldung aus)
{
    static char buffer[logPRINTF_BUFFER+1];											//Buffer
	time_t tt_date=time(NULL);

	tm *date=localtime(&tt_date);
	//Abspeichern in Datei:
	FILE *errorLog=fopen("errorlog.txt","a");
	if(errorLog!=NULL)
	{   fprintf(errorLog,"[%2d.%02d.%4d / %2d:%02d:%02d] Source: \"%s\" Message: \"\%s\"\n",date->tm_mday,date->tm_mon+1,1900+date->tm_year,date->tm_hour,date->tm_min,date->tm_sec,source,buffer);
        fclose(errorLog);
	}
	else printf("\nError opening Errorlog");

	if(!DEBUG) return;
    //Ausgabe auf Konsole:
	va_list  argptr;																//Argument-Liste
	va_start( argptr, format );
	vsprintf( buffer, format, argptr );												//Mit sprintf-Funktion in Buffer übertragen
	va_end  ( argptr );
	buffer[logPRINTF_BUFFER]=0;														//Zur Sicherheit
	textcolor(ccLIGHTRED);
	printf("\nEs kam zu einem Programfehler!\n    Aufgetreten in: \"");
	textcolor(ccRED); printf("%s",source); textcolor(ccLIGHTRED);
	printf("\"\n    Fehlermeldung: \"");
	textcolor(ccRED); printf("%s",buffer); textcolor(ccLIGHTRED);
	printf("\"");
	textcolor(ccLIGHTGRAY);
}

//Version 0.1
//Jakob Maier
//Allg. Grundfunktionen fürs Debugging



//Übergabeparameter:
//  1.) Formatierter String             Auszugebende Daten
//  2 - n)  Zusätzliche Daten
void logger(const char *format,...)                                                 //Logger (Gibt Daten in der Konsole aus, um bestimmte Programzustände zu melden)
{   if(!DEBUG) return;

	static char buffer[logPRINTF_BUFFER+1];											//Buffer
	va_list  argptr;																//Argument-Liste
	va_start( argptr, format );
	vsprintf( buffer, format, argptr );												//Mit sprintf-Funktion in Buffer übertragen
	va_end  ( argptr );
	buffer[logPRINTF_BUFFER]=0;														//Zur Sicherheit
	textcolor(WHITE);
	putc('\n',stdout);
	puts(buffer);
	textcolor(LIGHTGRAY);
}

//Übergabeparameter:
//  1.) source        [string]          Wo das Fehler aufgetreten ist (zB. Unterprogramm-Name, Datei,...)
//  2.) Formatierter String             Was pasiert ist
//  3 - n)  Zusätzliche Daten
void error(const char *source,const char *format,...)                               //Error-Logger (Gibt eine Fehlermeldung aus)
{   if(!DEBUG) return;

	static char buffer[logPRINTF_BUFFER+1];											//Buffer
	va_list  argptr;																//Argument-Liste
	va_start( argptr, format );
	vsprintf( buffer, format, argptr );												//Mit sprintf-Funktion in Buffer übertragen
	va_end  ( argptr );
	buffer[logPRINTF_BUFFER]=0;														//Zur Sicherheit
	textcolor(LIGHTRED);
	printf("\nEs kam zu einem Programfehler!\n    Aufgetreten in: \"");
	textcolor(RED); printf("%s",source); textcolor(LIGHTRED);
	printf("\"\n    Fehlermeldung: \"");
	textcolor(RED); printf("%s",buffer); textcolor(LIGHTRED);
	printf("\"");

    time_t tt_date=time(NULL);
	tm *date=localtime(&tt_date);
	//Abspeichern in Datei:
	FILE *errorLog=fopen("errorlog.txt","a");
	if(errorLog!=NULL)
	{   fprintf(errorLog,"[%2d.%02d.%4d / %2d:%02d:%02d] Source: \"%s\" Message: \"\%s\"\n",date->tm_mday,date->tm_mon+1,1900+date->tm_year,date->tm_hour,date->tm_min,date->tm_sec,source,buffer);
        fclose(errorLog);
	}
	else printf("\nError opening Errorlog");
	textcolor(LIGHTGRAY);
}

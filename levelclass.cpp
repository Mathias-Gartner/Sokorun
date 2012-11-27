//Version 0.0
//Jakob Maier
//Level-Klasse

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>
#include <windows.h>
#include "display.h"
#include "graphics.h"
#include "kugel.h"
#include "levelclass.h"
#include "lock.h"
#include "logger.h"
#include "transporter.h"

extern TEXTURE leveltiles;


bool freadToChar(FILE *datei,const char symbol)
{   char zeichen;
    do
    {   zeichen=fgetc(datei);
    }while(zeichen!=symbol && !feof(datei));
    if(feof(datei))
    {   error("freadToChar()","Wasn't able to read until char '%c' due to end of File",symbol);
        return 1;
    }
    return 0;
}

bool freadStrToChar(char *buf,FILE *datei,const char symbol)
{   do
    {   *buf=fgetc(datei);
        buf++;
    }while(*(buf-1)!=symbol && !feof(datei));
    *(buf-1)=0;
    if(feof(datei))
    {   error("freadStrToChar()","Wasn't able to read String until char '%c' due to end of File",symbol);
        return 1;
    }
    return 0;
}

bool freadPos(POS *pos,FILE *datei)
{   if(fscanf(datei,"%dx%d",&(pos->x),&(pos->y))!=2)
        return 1;
    return 0;
}


LEVEL::LEVEL(POS _origin,int _elsize,const char *LVLpath="",const bool skipMinorErrors=0)//skipMinorErrors: Wenn Levelfehler nicht zum abbruch führen sollen (für den Editor)
{   status=0;                           //Keine gültigen Leveldaten vorhanden
    origin=_origin;                     //Position, wo das LEvel ausgegeben wird (links unten)
    if(_elsize>=1)
        elsize=_elsize;                 //Größe eines Elementes/Feldes
    else
    {   error("LEVEL::LEVEL()","Fehler - Die Elementgroesse ist zu niedrig und wurde deshalb nicht uebernommen - elsize=%d. Es wird stattdessen der Wert 40 verwendet",_elsize);
        elsize=40;
    }

    animationType=0;                    //Animationstyp noch nicht definiert
    buildupAnimationProgress=0;         //Animation noch nicht gestartet

    kugelOriginStart=NULL;              //Keine Kugeln in der verketteten Liste
    lockOriginStart=NULL;               //Keine Schlösser in der verketteten Liste
    transporterOriginStart=NULL;        //Keine Transporter in der Verketteten Liste

    if(strcmp(LVLpath,"")!=0)           //Levelpfad zum laden übergeben
    {   loadLevel(LVLpath,skipMinorErrors);   //Level aus der Datei laden (Es wird die sichere Methode verwendet)
    }
}

void LEVEL::loadLevel(const char *LVLpath,const bool skipMinorErrors=0)            //Setzt die Daten im Speicher zurück und lädt ein Level
{   cleanup();                          //Daten zurücksetzen und freigeben

    if(levelloader(LVLpath,skipMinorErrors)!=0)   //Level aus der Datei laden
        cleanup();                      //Leveldatei war fehlerhaft -> ungueltige Daten wieder freigeben
    else if(status==0)
        cleanup();                      //Leveldatei war fehlerhaft -> ungueltige Daten wieder freigeben
}


void LEVEL::cleanup()   //Setzt alle Leveldaten zurück und gibt den Speicher wieder frei der reserviert wurde
{   //Muss zB. nach dem Ladeversuch eines fehlerhaften Levels ausgeführt werden
    status=0;

    //Verkettete Liste mit Kugeln freigeben:
    {   KUGELorigin *p=kugelOriginStart,*q;
        while(p!=NULL)
        {   q=(p->next);
            free(p);
            p=q;
        }
        kugelOriginStart=NULL;
    }

    //Verkettete Liste mit Schlössern freigeben:
    {   LOCKorigin *p=lockOriginStart,*q;
        while(p!=NULL)
        {   q=(p->next);
            free(p);
            p=q;
        }
        lockOriginStart=NULL;
    }

    //Verkettete Liste mit Transportern freigeben:
    {   TRANSPORTERorigin *p=transporterOriginStart,*q;
        while(p!=NULL)
        {   //Rail freigeben:
            RAIL *r=p->start,*s;
            while(r!=NULL && r!=p->start)
            {   s=(r->next);
                free(r);
                r=s;
            }
            p->start=NULL;
            //---
            q=(p->next);
            free(p);
            p=q;
        }
        transporterOriginStart=NULL;
    }
}




bool LEVEL::checkPos(const POS position)//Überprüft, ob sich die übergebenen Koordinaten im Spielfeld befinden (Wenn das Spielfeld 5x7 Felder gorß ist, ist (0x0) und (4x6) innerhalb, (3x7) aber außerhalb
{   if(position.x>=0&&position.x<size.x && position.y>=0&&position.y<size.y)
        return 0;
    return 1;
}


int LEVEL::convertLevel()              //Konvertiert die Daten eines veralteten Levelformates
{
    for(int y=0;y<size.y;y++)
    {   for(int x=0;x<size.x;x++)
        {   switch(spielfeld[y][x])
            {   case  0:
                case  1: break;
                case  2: spielfeld[y][x]= 5; break;
                case  3: spielfeld[y][x]= 7; break;
                case  4: spielfeld[y][x]=15; break;
                case  5: spielfeld[y][x]=14; break;
                case  6: spielfeld[y][x]=12; break;
                case  7: spielfeld[y][x]=13; break;
                case  8: spielfeld[y][x]=23; break;
                case  9: spielfeld[y][x]=22; break;
                case 10: spielfeld[y][x]=20; break;
                case 11: spielfeld[y][x]=21; break;
                case 12: spielfeld[y][x]=11; break;
                case 13: spielfeld[y][x]=10; break;
                case 14: spielfeld[y][x]= 8; break;
                case 15: spielfeld[y][x]= 9; break;
                case 16: spielfeld[y][x]=19; break;
                case 17: spielfeld[y][x]=18; break;
                case 18: spielfeld[y][x]=16; break;
                case 19: spielfeld[y][x]=17; break;
                case 20: spielfeld[y][x]=27; break;
                case 21: spielfeld[y][x]=26; break;
                case 22: spielfeld[y][x]=24; break;
                case 23: spielfeld[y][x]=25; break;
                case 24: spielfeld[y][x]=48; break;
                case 25: spielfeld[y][x]= 2; break;
                case 26: spielfeld[y][x]= 7; break;
                case 27: spielfeld[y][x]= 3; break;
                //case 28: spielfeld[y][x]=57; break;
                //case 29: spielfeld[y][x]=56; break;
                //case 30: spielfeld[y][x]=61; break;
                //case 31: spielfeld[y][x]=63; break;
                //case 32: spielfeld[y][x]=60; break;
                //case 33: spielfeld[y][x]=59; break;
                case 34: spielfeld[y][x]=46; break;
                case 35: spielfeld[y][x]=49; break;
                //case 36: spielfeld[y][x]=62; break;
                case 37: spielfeld[y][x]= 4; break;
                //case 38: spielfeld[y][x]= 0; break;
                //case 39: spielfeld[y][x]= 0; break;
                case 40: spielfeld[y][x]=31; break;
                case 41: spielfeld[y][x]=30; break;
                case 42: spielfeld[y][x]=28; break;
                case 43: spielfeld[y][x]=29; break;
                case 44: spielfeld[y][x]=34; break;
                case 45: spielfeld[y][x]=35; break;
                case 46: spielfeld[y][x]=32; break;
                case 47: spielfeld[y][x]=33; break;
                case 48: spielfeld[y][x]=36; break;
                case 49: spielfeld[y][x]=37; break;
                case 50: spielfeld[y][x]=38; break;
                case 51: spielfeld[y][x]=39; break;
                case 52: spielfeld[y][x]=40; break;
                //case 53: spielfeld[y][x]=58; break;
                //case 54: spielfeld[y][x]=47; break;
                case 55: spielfeld[y][x]=41; break;
                case 80: spielfeld[y][x]=42; break;
                case 81: spielfeld[y][x]=43; break;
                case 82: spielfeld[y][x]=44; break;
                case 83: spielfeld[y][x]=45; break;
                //case 84: spielfeld[y][x]=58; break;
                //case 85: spielfeld[y][x]=59; break;
                //case 86: spielfeld[y][x]=60; break;
                //case 87: spielfeld[y][x]=61; break;
                //case 88: spielfeld[y][x]=62; break;
                //case 89: spielfeld[y][x]=63; break;
                default:    error("LEVEL::convertLevel()","Unbekannter Feldtyp gefunden. Position: (%dx%d), Typ: %d",x,y,spielfeld[y][x]);
                            return -1;
            }
        }
    }
    return 0;
}



int LEVEL::levelloader(const char *LVLpath,const bool skipMinorErrors=0)//skipMinorErrors: Wenn Levelfehler nicht zum abbruch führen sollen (für den Editor)
{   /**ACHTUNG: Diese Methode ist unsicher und kann ungültige Daten erzeugen wenn die Leveldatei fehlerhaft ist. Stattdessen "loadLevel()" verwenden!**/

    if(status!=0)
    {   error("LEVEL::loadLevel()","Schwerwiegender Fehler. Es darf kein Level geladen werden, solange sich noch gueltige Leveldaten im Speicher befinden. status: %d",status);
        return 1;
    }
    status=0;                           //Keine gültigen Leveldaten

    FILE *level=fopen(LVLpath,"r");     //Leveldatei öffnen

    int kugelAnz,kanonenAnz,NtransAnz,DtransAnz,length,lockAnz; //Zwischenwerte
    int help;                           //Hilfsvariable
    float r,g,b;                        //Hilfsvariablen

    if(level==NULL)
    {   error("LEVEL::loadLevel()","Die Leveldatei konnte nicht geoeffnet werden. Pfad: \"%s\"",LVLpath);
        return 1;
    }

    ///ALLGEMEINE DATEN:
            if(freadToChar(level,'['))  return 1;    if(freadStrToChar(CreateDate,level,']'))  return 1;        //Erstelldatum
            if(freadToChar(level,'['))  return 1;    if(freadStrToChar(CreateTime,level,']'))  return 1;        //Erstellzeitpunkt

            if(freadToChar(level,'['))  return 1;    if(freadPos(&size,level))  return 1;                       //Spielfeldgröße
                if(size.x>MaxXsize || size.y>MaxYsize)                                                          //Spielfeld zu groß für das Programm
                {   error("LEVEL::loadLevel()","Die Spielfeldgroesze des geladenen Levels ist zu grosz fuer das Programm. Levelgroesze: (%dx%d) Max. Groesze: (%dx%d)",size.x,size.y,MaxXsize,MaxYsize);
                    return 2;//Spielfeld zu groß für das Programm
                }
            if(freadToChar(level,'['))  return 1;    if(freadPos(&avatarOrigin,level))  return 1;               //Spieler-Position
                if(checkPos(avatarOrigin)&&!skipMinorErrors)                                                    //Verbotene Spielerpsition
                {   error("LEVEL::loadLevel()","Die Spielfigur befindet sich auszerhalb des Spielfeldes. Levelgroesze: (%dx%d) Avatarposition: (%dx%d)",size.x,size.y,avatarOrigin.x,avatarOrigin.y);
                    return 3;//Verbotene Spielerpsition
                }
            ///Kugel-Anzahl, Kanonen-Anzahl, Transporter-Anzahl, Tödl.Transporter-Anzahl und Schlösseranzahl laden:
            if(freadToChar(level,'['))  return 1;   if(fscanf(level,"%d]",&kugelAnz) != 1)  { error("LEVEL::loadLevel","Kugelanzahl konnte nicht geladen werden"); return 1; }
                if(kugelAnz<=0 && !skipMinorErrors)                                                             //Zuwenig Kugeln
                {   error("LEVEL::loadLevel()","Es befinden sich keine Kugeln im Level, das Spiel kann daher nicht gespielt werden");
                    return 4;//Zuwenig Kugeln
                }
            if(freadToChar(level,'['))  return 1;   if(fscanf(level,"%d]",&kanonenAnz) != 1){ error("LEVEL::loadLevel()","Kanonenanzahl konnte nicht geladen werden"); return 1; }
            if(freadToChar(level,'['))  return 1;   if(fscanf(level,"%d]",&NtransAnz) != 1) { error("LEVEL::loadLevel()","Anzahl der Transporter konnte nicht geladen werden"); return 1; }
            if(freadToChar(level,'['))  return 1;   if(fscanf(level,"%d]",&DtransAnz) != 1) { error("LEVEL::loadLevel()","Anzahl der toedlichen Transporter konnte nicht geladen werden"); return 1; }
            if(freadToChar(level,'['))  return 1;   if(fscanf(level,"%d]",&lockAnz) != 1)   { error("LEVEL::loadLevel()","Anzahl der Schluessel und Schloesser konnte nicht geladen werden"); return 1; }

    ///KUGELN:
            if(kugelOriginStart!=NULL)
            {   error("LEVEL::loadLevel()","Es befinden sich Kugeln in der Liste, obwohl das Level erst geladen wird. Fehlerabfrage \"status\" fehlgeschlagen. Das Programm wird abgebrochen um Speicherverletzungen zu verhindern");
                MessageBox(NULL,"Es kam zu einem Fehler. Um Speicherverletzungen zu verhindern wird das Spiel jetzt angehalten. Für mehr Details sehen Sie im Erroglog nach.","Schwerwiegender Fehler",MB_OK|MB_ICONERROR);
                for(;;);
            }
            for(int i=0;i<kugelAnz;i++)
            {   KUGELorigin *p=(KUGELorigin*)malloc(sizeof(KUGELorigin));
                p->next=kugelOriginStart;

                if(freadToChar(level,'[')) {free(p);return 1;} if(freadPos(&(p->origin),level)) {free(p);return 1;}     //Kugelposition
                if(checkPos(p->origin)&&!skipMinorErrors)                                                               //Verbotene Kugelposition
                {   error("LEVEL::loadLevel()","Eine Kugel befindet sich auszerhalb des Spielfeldes. Levelgroesze: (%dx%d) Kugelposition: (%dx%d) Kugelnummer: %d",size.x,size.y,p->origin.x,p->origin.y,i+1);
                    free(p);    //Freigeben
                    return 5;//Verbotene Kugelposition
                }

                if(fscanf(level,"]*%d*",&help)==1)//Es wurde ein Kugeltyp angegeben
                {
                    if(help<0||help>1)
                    {   error("LEVEL::loadLevel()","Ein Kugelobjekt hat einen ungueltigen typ-Wert. Kugeltyp: %d Kugelnummer: %d",help,i+1);
                        free(p);    //Freigeben
                        return 6;//Ungültiger Kugeltyp
                    }
                    p->type=help;
                }else
                    p->type=0;//Normale Kugel
                kugelOriginStart=p;     //Einfügen in die Liste (am Anfang)
            }

    ///Levelboden laden:
            for(int y=0;y<size.y;y++)
            {   if(freadToChar(level,'#'))  return 1;
                for(int x=0;x<size.x;x++)
                {   if(fscanf(level,"%d",&help) != 1)  { error("LEVEL::loadLevel()","Spielfeld konnte nicht geladen werden. Position: (%dx%d)",x,y); return 1; }
                    spielfeld[y][x]=help;
                    if(x!=size.x-1 && fgetc(level)!='.'){ error("LEVEL::loadLevel()","Spielfeld konnte nicht geladen werden. Position: (%dx%d); (beginnend bei 0)",x,y); return 1; }
                }
            }


            if(DEBUG)   {   logger(1,"Das Spielfeld wurde aus der Leveldatei geladen:");
                    for(int y=0;y<size.y;y++)
                    {   logger(0,"     ");
                        for(int x=0;x<size.x;x++)
                        {   if(spielfeld[y][x]==25)//Wand
                            {   textbackground(ccLIGHTGRAY);
                                textcolor(ccBLACK);
                            }
                            if(spielfeld[y][x]==27)//Zielfeld
                                textcolor(ccYELLOW);
                            if(spielfeld[y][x]==0)//Leeres Feld
                                textcolor(ccBLACK);

                            //Prüfen, ob sich eine Kugel an dieser Position befindet:
                            KUGELorigin *p=kugelOriginStart;
                            while(p!=NULL)
                            {   if(p->origin.x==x && p->origin.y==y)//Kugel an dieser Position
                                {   textcolor(ccLIGHTBLUE);
                                    break;
                                }
                                p=p->next;
                            }

                            if(avatarOrigin.x==x && avatarOrigin.y==y)//Spielfigur an dieser Position
                                textcolor(ccLIGHTRED);

                            logger(0,"%02d ",spielfeld[y][x]);
                            textbackground(ccBLACK);
                            textcolor(ccLIGHTGRAY);
                        }logger(0,"\n");
                    }
                }


///JETZT HIER Kanonen, Transporter, Tödl.Transporter


//            for(int i=0;i<anz[1];i++)//Kanonen
//            {   do{zeichen=fgetc(datei);}while(zeichen!='[');fscanf(datei,"%02dx%02d",&hilf1,&hilf2);knnPos[i].x=hilf1;knnPos[i].y=hilf2;
//                    if(knnPos[i].x<0||knnPos[i].x>MAXSIZE_X)  {char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: x-kanon of No. %d in %s is %d  (class::editor)\n",time,i+1,pfad,knnPos[i].x);MessBoxError(mess);return;}
//                    if(knnPos[i].y<0||knnPos[i].y>MAXSIZE_Y)  {char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: y-kanon of No. %d in %s is %d  (class::editor)\n",time,i+1,pfad,knnPos[i].y);MessBoxError(mess);return;}
//                do{zeichen=fgetc(datei);}while(zeichen!='[');fscanf(datei,"%1d",&hilf);
//                    if(hilf<0||hilf>3)  {char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: kanon-ausrichtung of %d in %s (%d)  (class::editor)\n",time,i+1,pfad,hilf);MessBoxError(mess);return;}
//                    knnSde[i]=hilf;
//                do{zeichen=fgetc(datei);}while(zeichen!='[');fscanf(datei,"%1d",&hilf);
//                    if(hilf<1||hilf>5)  {char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: kanon-energy of %d in %s (%d)  (class::editor)\n",time,i+1,pfad,hilf);MessBoxError(mess);return;}
//                    knnSpd[i]=hilf;
//                do{zeichen=fgetc(datei);}while(zeichen!='[');fscanf(datei,"%05d",&hilf);
//                    if(hilf<50||hilf>15000)  {char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: kanon-pause of %d in %s (%d)  (class::editor)\n",time,i+1,pfad,hilf);MessBoxError(mess);return;}
//                    knnPause[i]=hilf;
//                if(info)
//                {   sprintf(mess,"KANONE geldaden:\n\nNummer:\t\t%d/%d\nPosition:\t\t(%02.0fx%02.0f)\nAusrichtung:\t%d\nEnergie: \t\t%d\nPause: \t\t%dms",i+1,anz[1],knnPos[i].x,knnPos[i].y,knnSde[i],knnSpd[i],knnPause[i]);
//                    MessBoxInfo(mess);
//                }
//            }

            for(int i=0;i<NtransAnz+DtransAnz;i++)//Alle Transporter
            {   TRANSPORTERorigin *p=(TRANSPORTERorigin*)malloc(sizeof(TRANSPORTERorigin));

                p->next=transporterOriginStart;

                p->start=NULL;  //Noch unbekannt

                if(freadToChar(level,'[')) {free(p);return 1;} if(fscanf(level,"%d]",&length) != 1){ error("LEVEL::loadLevel()","Transporter: Anzahl der Positionsdaten konnte nicht geladen werden. Nummer: %d",i+1);free(p);return 1; }   //Anzahl der Positionsdaten
                        if(length<2) {error("LEVEL::loadLevel()","Transporter: Das Schienennetz (Laenge) ist zu kurz. Nummer: %d; Laenge: %d",i+1,length); free(p); return 11;}         //zu Kurzes Schienennetz

                bool bidirectional;
                if(freadToChar(level,'[')) {free(p);return 1;} if(fscanf(level,"%d]",&help)  != 1){ error("LEVEL::loadLevel()","Transporter: Bewegungsart (Typ) konnte nicht geladen werden. Nummer: %d",i+1); free(p);return 1; }           //1: hin+zurück;	2: im Kreis
                        if(help==1) bidirectional=1; else if(help==2) bidirectional=0;
                        else {error("LEVEL::loadLevel()","Transporter: Bewegungsart (Typ) ist ungueltig.. Nummer: %d; Typ: %d; (Erlaubt: 1-2)",i+1,help); free(p);return 12; }//Falscher Bewegungstyp-Wert

                if(freadToChar(level,'[')) {free(p);return 1;} if(fscanf(level,"%d]",&help) != 1){ error("LEVEL::loadLevel()","Transporter: Index der Startposition konnte nicht geladen werden. Nummer: %d",i+1); free(p);return 1; }      //Startpositions-Index
                        if(help<0 || help>=length) {error("LEVEL::loadLevel()","Transporter: Index der Startposition ungueltig. Nummer: %d; Geladener Startpositionsindex: %d; Laenge: %d",i+1,help,length); free(p); return 13;}             //Falscher Startpositionsindex


                ///Positionen/Schienenstrecke laden:
                //DIE SCHIENENELEMENTE MÜSSEN IN EINE DOPPELT VERKETETTE LISTE GEPACKT WERDEN. In Abhängigkeit von unidirektionalen und birdirektionalen Transportern sollen das letzte und erste Element wieder aufeinander verweisen (Ring)
                    RAIL *start=NULL;               //Adresse des ersten Elements in der Liste
                    RAIL *last=NULL;                //Adresse des zuletzt geladenen Schienenstücks (das letzte Element in der Liste)
                    for(int j=0;j<length;j++)
                    {   RAIL *r=(RAIL*)malloc(sizeof(RAIL));
                        r->next=NULL;
                        r->prev=NULL;

                        if(start==NULL)             //Erstes Element (auch der last-Pointer ist NULL)
                        {   start=r;
                            p->start=r;
                            //next und prev-Pointer sind bereits NULL
                        }else
                        {   ///Pointer, der nach vorne zeigt muss befüllt werden:
                            last->next=r;           //Hinten anhängen
                            r->prev=last;
                        }

                        ///Pointer der aufs nächste Element zeigt muss befüllt werden
                        if(!bidirectional)          //Es muss ein Ring gebildet werden (letztes Element zeigt aufs erste, erstes aufs letzte
                        {   r->next=start;          //aufs erste Element zeigen (falls noch weitere Elemente angehägt werden, wird dieser Pointer wieder überschrieben)
                            start->prev=r;          //Das erste Element zeigt aufs letzte Element zurück
                        }else
                        {   r->next=NULL;               //(noch) kein weiteres Element
                            //start-prev beinhaltet bereits NULL
                        }

                        if(j==help)//Das ist das Feld der Startposition
                        {   p->origin=r;    //Startposition setzen
                        }

                        if(freadToChar(level,'[')) {free(p);return 1;} if(freadPos(&(r->position),level))  {free(p);return 1;}  //Position
                        if(checkPos(r->position)&&!skipMinorErrors)                                                             //Verbotene Kugelposition
                        {   error("LEVEL::loadLevel()","Ein Transporterfeld befindet sich auszerhalb des Spielfeldes. Levelgroesze: (%dx%d); Position: (%dx%d); Transporternummernummer: %d; Positionsindex: %d",size.x,size.y,r->position.x,r->position.y,i+1,r+1);
                            //Rail wieder löschen (befindet sich nicht in der Transporter-liste weil der aktuelle Transporter fehlerhaft ist und entfernt werden muss)
                                RAIL *R=p->start,*S;
                                while(R!=NULL && R!=p->start)//solange die Liste nicht zu Ende ist oder von vorne Beginnt
                                {   S=(R->next);
                                    free(R);
                                    R=S;
                                }
                                p->start=NULL;
                                free(r);
                            //---
                            free(p);
                            return 14;//Verbotene Transporterposition
                        }
                        r->outputType=-1;
                        last=r;
                    }

                if(p->start==NULL)//Kann eigentlich nicht vorkommen, weil oben bereits auf einen gültigen Wert überprüft wird. Wenn er ungültig ist, sollte mit 12 abgebrochen worden sein
                {   error("LEVEL::loadLevel()","Die Transporter-Startposition wurde nickt erkannt. Fehlerabfrage Nr. 13 ist fehlgeschlagen. Transporternummernummer: %d;",i+1);
                    //Rail wieder löschen (befindet sich nicht in der Transporter-liste weil der aktuelle Transporter fehlerhaft ist und entfernt werden muss)
                        RAIL *R=p->start,*S;
                        while(R!=NULL && R!=p->start)
                        {   S=(R->next);
                            free(R);
                            R=S;
                        }
                        p->start=NULL;
                    //---
                    free(p);
                    return 16;//Startposition wurde nicht erkannt
                }
                if(i<NtransAnz) p->type=0;  //Normaler Transporter
                else            p->type=1;  //Tödlicher Transporter
                p->reverse=bidirectional;   //Der Transporter darf nur umkehren, wenn er Bidirektional ist (in diesem Levelformat)
                p->speed=1+rand()%20;

                transporterOriginStart=p;   //Transporter am Anfang anhängen
                //outputType aller Elemente berechnen:
                if(calculateRailOutputType(p->start))
                {   return 17;  //Fehler im Schienennetz
                }
            }



    ///SCHLÖSSER:
            if(lockOriginStart!=NULL)
            {   error("LEVEL::loadLevel()","Es befinden sich Schloesser in der Liste, obwohl das Level erste geladen wird. womöglich muss hier jetzt die Liste geleert werden, wurde noch nicht ausprogrammiert");
                for(;;);
            }
            for(int i=0;i<lockAnz;i++)
            {   LOCKorigin *p=(LOCKorigin*)malloc(sizeof(LOCKorigin));
                p->next=lockOriginStart;

                if(freadToChar(level,'[')) {free(p);return 1;} if(freadPos(&(p->lock),level)) {free(p);return 1;}           //Schlossposition
                if(freadToChar(level,'[')) {free(p);return 1;} if(freadPos(&(p->key),level))  {free(p);return 1;}           //Schlüsselposition


                if(checkPos(p->lock)&&!skipMinorErrors)                                                       //Verbotene Kugelposition
                {   error("LEVEL::loadLevel()","Ein Schloss befindet sich auszerhalb des Levels. Levelgroesze: (%dx%d) Schlossposition: (%dx%d) Schlossnummer: %d",size.x,size.y,p->lock.x,p->lock.y,i);
                    free(p);
                    return 7;//Verbotene Schlossposition
                }

                if(checkPos(p->key)&&!skipMinorErrors)                                                       //Verbotene Kugelposition
                {   error("LEVEL::loadLevel()","Ein Schluessel befindet sich auszerhalb des Levels. Levelgroesze: (%dx%d) Schluesselposition: (%dx%d) Schlossnummer: %d",size.x,size.y,p->key.x,p->key.y,i);
                    free(p);
                    return 8;//Verbotene Schluesselposition
                }



                if(fscanf(level,"]*RGB(%f,%f,%f)*",&r,&g,&b)==3)//Es wurde eine Farbe angegeben
                {   if(r<0||r>1 || g<0||g>1 || b<0||b>1)
                    {   error("LEVEL::loadLevel()","Ein Schloss hat eine ungueltigen Farbwert. Farbe: (%f, %f, %f) Schlossnummer: %d.",r,g,b,i);
                        free(p);
                        return 9;//Ungültiger Farbwert für Schloss und Schlüssel
                    }
                    p->color=COLOR{r,g,b};
                }else
                {   switch(i%16)   //Farbwert wählen
                    {   case  0: p->color={0.00 , 0.75 , 0.02};  break;  //dunkelgrün
                        case  1: p->color={0.75 , 1.00 , 0.00};  break;  //hellgrün
                        case  2: p->color={0.00 , 0.60 , 0.30};  break;  //dunkelcyan
                        case  3: p->color={0.45 , 0.55 , 1.00};  break;  //hellblau
                        case  4: p->color={0.70 , 0.40 , 1.00};  break;  //hellLila
                        case  5: p->color={1.00 , 0.00 , 1.00};  break;  //magenta
                        case  6: p->color={1.00 , 0.15 , 0.50};  break;  //pink
                        case  7: p->color={1.00 , 0.45 , 0.30};  break;  //fleischfarbe
                        case  8: p->color={1.00 , 0.87 , 0.45};  break;  //hellgelb-orange
                        case  9: p->color={0.90 , 0.58 , 0.15};  break;  //dunkelgelb-orange
                        case 11: p->color={0.72 , 0.00 , 1.00};  break;  //dunkelLila
                        case 12: p->color={0.50 , 0.00 , 0.00};  break;  //dunkelrot
                        case 13: p->color={0.80 , 0.30 , 0.00};  break;  //dunkelorange
                        case 14: p->color={0.00 , 0.00 , 0.50};  break;  //dunkelblau
                        case 15: p->color={0.00 , 0.45 , 0.40};  break;  //dunkelcyan-grün
                    }
                }
                lockOriginStart=p;     //Einfügen in die Liste (am Anfang)
            }


    /// ///////////////////////////////


    if(convertLevel() != 0)
    {   return 10;  //Konvertieren fehlgeschlagen
    }
    strcpy(path,LVLpath);
    status=1;
    fclose(level);
    return 0;
}

bool LEVEL::calculateRailOutputType(RAIL *start)         //Setzt die Outputtypes für jedes Schienenelement (Ecke/Linie/Endstück/...)
{
    RAIL *p=start;
    //Zum berechnen sind das Element und dessen Nachbarelemente notwendig. Beide mit next- und prev-Pointer erreichbar
    bool startAllowed=1;
    ///Werte setzen
        while(p!=NULL && (p!=start || startAllowed))
        {   startAllowed=0;

            //Schienentyp feststellen:
                DIRECTION in,out;
                if(p->prev!=NULL)//Es gibt ein vorheriges Element
                    in=getRailDirection((p->prev)->position,p->position);
                else in=BEAM;
                if(p->next!=NULL)//Es gibt ein nächstes Element
                    out=getRailDirection(p->position,(p->next)->position);
                else out=BEAM;


                if(in==NONE || out==NONE)
                {   error("LEVEL::calculateRailOutputType()","Im Schienennetz befindet sich ein Fehler. 2 aufeinanderfolgende Schienenstuecke sind an der selben Position. in: %d, out: %d; position: (%dx%d)",in,out,p->position.x,p->position.y);
                    return -1;
                }
                else
                {       switch(in)
                        {   case UP:    switch(out)
                                        {   case UP:    p->outputType=4; break; //gerade
                                            case DOWN:  p->outputType=1; break; //umkehren
                                            case LEFT:  p->outputType=7; break;
                                            case RIGHT: p->outputType=6; break;
                                            case BEAM:  p->outputType=1; break;
                                            default:    break;
                                        }break;
                            case DOWN:  switch(out)
                                        {   case UP:    p->outputType=0; break; //umkehren
                                            case DOWN:  p->outputType=4; break; //gerade
                                            case LEFT:  p->outputType=8; break;
                                            case RIGHT: p->outputType=9; break;
                                            case BEAM:  p->outputType=0; break;
                                            default:    break;
                                        }break;
                            case LEFT:  switch(out)
                                        {   case UP:    p->outputType=9; break;
                                            case DOWN:  p->outputType=6; break;
                                            case LEFT:  p->outputType=5; break; //gerade
                                            case RIGHT: p->outputType=2; break; //umkehren
                                            case BEAM:  p->outputType=2; break;
                                            default:    break;
                                        }break;
                            case RIGHT: switch(out)
                                        {   case UP:    p->outputType=8; break;
                                            case DOWN:  p->outputType=7; break;
                                            case LEFT:  p->outputType=3; break; //umkehren
                                            case RIGHT: p->outputType=5; break; //gerade
                                            case BEAM:  p->outputType=3; break;
                                            default:    break;
                                        }break;
                            case BEAM:  switch(out)
                                        {   case UP:    p->outputType=0; break;
                                            case DOWN:  p->outputType=1; break;
                                            case LEFT:  p->outputType=3; break;
                                            case RIGHT: p->outputType=2; break;
                                            case BEAM:  error("LEVEL::calculateRailOutputType()","Im Schienennetz befindet sich ein Fehler. Ein Schienenstueck hat gar keinen Nachbarn");return -1;
                                            default:    break;
                                        }break;
                            default:    break;
                        }

                }
            p=p->next;
        }
    return 0;
}

DIRECTION LEVEL::getRailDirection(POS a,POS b)  //Gibt zurück in welche Richtung man sich von a aus bewegen muss, damit man b erreicht
{   if(a.x==b.x)
    {   if(a.y+1==b.y)  return DOWN;
        if(a.y-1==b.y)  return UP;
        if(a.y==b.y)    return NONE;    //a und b haben die gleichen Werte
        return BEAM;
    }
    if(a.y==b.y)
    {   if(a.x+1==b.x)  return RIGHT;
        if(a.x-1==b.x)  return LEFT;
        return BEAM;
    }
    return BEAM;
}


void LEVEL::printFloorElement(int element,POS coord,COLOR color=WHITE)
{   if(element==0)  return;

    POS pos;
    pos.x=origin.x+coord.x*elsize;
    pos.y=origin.y+(size.y-coord.y-1)*elsize;
    leveltiles.print({pos,{pos.x+elsize,pos.y+elsize}},{element%8,element/8},color);
}

bool LEVEL::runBuildupAnimation()
{   if(status!=1)
    {   error("LEVEL::runBuildupAnimation()","Das Level darf nicht ausgegeben werden, da es keine gueltigen Daten enthaelt. status=%d",status);
        return 1;   //Animation abbrechen
    }
    if(animationType==0)//Noch unbekannt
    {   srand(clock());
        animationType=(rand()%3)+1;
    }
    if(animationType==1)
    {   if(buildupAnimationProgress==0)
        {   srand(clock());                                         //Zufallszahlen generieren
            buildupAnimationOrigin={rand()%size.x,rand()%size.y};   //Ursprung auf einen Zufallswert setzen
        }
        buildupAnimationProgress++;

        buildupAnimationCompleted=1;                    //Zur überprüfung, ob die Animation abgeschlossen wurde
        for(int y=0;y<size.y;y++)
        {   for(int x=0;x<size.x;x++)
            {   float dist = sqrt((buildupAnimationOrigin.x-x)*(buildupAnimationOrigin.x-x) + (buildupAnimationOrigin.y-y)*(buildupAnimationOrigin.y-y));
                if(dist<=buildupAnimationProgress/2.5)
                {   printFloorElement(spielfeld[y][x],{x,y});
                }else
                {   buildupAnimationCompleted=0;
                    if(spielfeld[y][x]!=0 && dist-1<=buildupAnimationProgress/2.5)    //Rotes Feld ausgeben
                    {   printFloorElement(TILE_ANIMATIONFIELD,{x,y});
                    }
                }
            }
        }
        if(buildupAnimationCompleted==1)
        {   animationType=0;                            //Nächstes mal wieder eine zufällige Animationsart wählen
            buildupAnimationProgress=0;                 //Damit die Animation beim nächsten mal von vorne wiederholt wird
            return 1;
        }
        return 0;
    }else if(animationType==2)
    {   if(buildupAnimationProgress==0)//erster durchgang
        {   srand(clock());                                         //Zufallszahlen generieren
            variation=rand()%8;
        }
        buildupAnimationProgress+=5;
        for(int i=0;i<buildupAnimationProgress;i++)
        {   int x,y;
            if((variation&0x01) != 0)   //Erst waagrecht, dann senkrecht
            {   x=i%size.x;
                y=i/size.x;
            }else                       //Erst senkrecht, dann waagrecht
            {   x=i/size.x;
                y=i%size.x;
            }

            if((variation&0x02) != 0)   x=size.x-x-1;   //Von rechts nach links
            if((variation&0x04) != 0)   y=size.y-y-1;   //Von unten nach oben
            if(i>=buildupAnimationProgress-4 && buildupAnimationProgress<size.x*size.y)
                printFloorElement(TILE_ANIMATIONFIELD,{x,y});      //Rotes Feld
            else
                printFloorElement(spielfeld[y][x],{x,y});
        }
        if(buildupAnimationProgress>=size.x*size.y)
        {   animationType=0;                            //Nächstes mal wieder eine zufällige Animationsart wählen
            buildupAnimationProgress=0;                 //Damit die Animation beim nächsten mal von vorne wiederholt wird
            return 1;
        }
        return 0;
    }else if(animationType==3)
    {   if(buildupAnimationProgress==0)//erster Durchgang -> Speicher reservieren
        {   visible=(char*)malloc(size.x*size.y);
            for(int y=0;y<size.y;y++)
            {   for(int x=0;x<size.x;x++)
                    *(visible+(y*size.x)+x)=0;
            }
            srand(clock());                             //Zufallszahlen generieren
        }
        buildupAnimationProgress+=2;
        //weitere Felder, die angezeigt werde sollen, suchen:
        for(int i=0;i<3;i++)
        {   variation=0;                                //Verwendung als Timeout (um eine endlosschleife zu verhindern)
            do
            {   pos.x=rand()%size.x;
                pos.y=rand()%size.y;
                variation++;                            //Timeout++
            }while(*(visible+(pos.y*size.x)+pos.x)==1 && variation<=250);
            if(variation>=250)                          //Timeout - neue Felder brauchen zu lange um gefundne zu werden -> Animation abbrechen
            {   for(int y=0;y<size.y;y++)               //Alle Felder aktivieren
                {   for(int x=0;x<size.x;x++)
                        *(visible+(y*size.x)+x)=1;
                }
            }else
                *(visible+(pos.y*size.x)+pos.x)=1;      //gewähltes Feld aktivieren
        }

        //Alle aktivierten Felder anzeigen:
        for(int y=0;y<size.y;y++)
        {   for(int x=0;x<size.x;x++)
            {   if(*(visible+(y*size.x)+x)==1)
                    printFloorElement(spielfeld[y][x],{x,y});
            }
        }
        if(variation>=250)//Bei einem Timeout: abbrechen
        {   free(visible);                              //Speicher freigeben
            animationType=0;                            //Nächstes mal wieder eine zufällige Animationsart wählen
            buildupAnimationProgress=0;                 //Damit die Animation beim nächsten mal von vorne wiederholt wird
            return 1;
        }
        return 0;
    }
    error("LEVEL::runBuildupAnimation()","Eine Buildup-Animation hat entweder kein return durchgefuehrt, oder der animationType ist ungueltig. animationType=%d",animationType);
    buildupAnimationProgress=0;
    animationType=0;
    return 1;   //Animation abbrechen
}

void LEVEL::printPreview()
{   if(status!=1)
    {   error("LEVEL::printPreview()","Das Level darf nicht ausgegeben werden, da es keine gueltigen Daten enthaelt. status=%d",status);
        return;
    }
    printFloor();
    //Danach:
    //*******
    //
    printTransporter();     //Transporter
    printKugelnAtOrigins(); //Kugeln
    printAvatarAtOrigin();  //Avatar
    printLocksAtOrigins();  //Schlösser
    //Kanonen
}

void LEVEL::printRail(TRANSPORTERorigin *transp)            //Gibt den Schienenweg eines Transporters aus
{
    char num;
    bool mirrorX,mirrorY;
    POS pos;

    bool startAllowed=1;
    RAIL *r=transp->start;
    while(r!=NULL && (r!=transp->start || startAllowed))
    {   startAllowed=0;
        num=0;
        mirrorX=0;
        mirrorY=0;
        switch(r->outputType)
        {   case  0:    num=TILE_RAIL_VEND;       mirrorX=0; mirrorY=1; break;
            case  1:    num=TILE_RAIL_VEND;       mirrorX=0; mirrorY=0; break;
            case  2:    num=TILE_RAIL_HEND;       mirrorX=1; mirrorY=0; break;
            case  3:    num=TILE_RAIL_HEND;       mirrorX=0; mirrorY=0; break;
            case  4:    num=TILE_RAIL_VERT;       mirrorX=0; mirrorY=0; break;
            case  5:    num=TILE_RAIL_HORI;       mirrorX=0; mirrorY=0; break;
            case  6:    num=TILE_RAIL_EDGE;       mirrorX=0; mirrorY=0; break;
            case  7:    num=TILE_RAIL_EDGE;       mirrorX=1; mirrorY=0; break;
            case  8:    num=TILE_RAIL_EDGE;       mirrorX=1; mirrorY=1; break;
            case  9:    num=TILE_RAIL_EDGE;       mirrorX=0; mirrorY=1; break;
            default:    error("LEVEL::printRail()","Unbekannter Schienentyp entdeckt. Der Wert wird auf 0 (Beginn nach oben) zurueckgesetzt. outputType: %d",r->outputType);
                        r->outputType=0;
        }

        pos.x=origin.x+r->position.x*elsize;
        pos.y=origin.y+(size.y-r->position.y-1)*elsize;

             if(!mirrorX && !mirrorY)/*Normal*/  leveltiles.print({{pos.x,pos.y},{pos.x+elsize,pos.y+elsize}},{num%8,num/8},((transp->type==0)?TRANSPORTER_COLOR:DEATHTRANSPORTER_COLOR));
        else if(!mirrorX &&  mirrorY)/* >Y < */  leveltiles.print({{pos.x,pos.y+elsize},{pos.x+elsize,pos.y}},{num%8,num/8},((transp->type==0)?TRANSPORTER_COLOR:DEATHTRANSPORTER_COLOR));
        else if( mirrorX && !mirrorY)/* >X < */  leveltiles.print({{pos.x+elsize,pos.y},{pos.x,pos.y+elsize}},{num%8,num/8},((transp->type==0)?TRANSPORTER_COLOR:DEATHTRANSPORTER_COLOR));
        else                         /* >XY< */  leveltiles.print({{pos.x+elsize,pos.y+elsize},{pos.x,pos.y}},{num%8,num/8},((transp->type==0)?TRANSPORTER_COLOR:DEATHTRANSPORTER_COLOR));



        r=r->next;
    }
}

void LEVEL::printTransporter()                              //Gibt alle Schienenwege aus
{
    TRANSPORTERorigin *p=transporterOriginStart;
    while(p!=NULL)
    {   printRail(p);
        p=p->next;
    }
    //Jetzt die Transporter auf die Schienen setzen:
    p=transporterOriginStart;
    while(p!=NULL)
    {   printFloorElement((p->type==0)?TILE_TRANSPORTER:TILE_DEATHTRANSPORTER,(p->origin)->position);
        p=p->next;
    }
}



void LEVEL::printFloor()
{
    if(status!=1)
    {   error("LEVEL::printFloor()","Das Level darf nicht ausgegeben werden, da es keine gueltigen Daten enthaelt. status=%d",status);
        return;
    }
    /* * /
    if(!gamefloorPrepared)
    {   gamefloor = glGenLists(1);

        glNewList(gamefloor,GL_COMPILE_AND_EXECUTE);
            for(int y=0;y<size.y;y++)
            {   for(int x=0;x<size.x;x++)
                {   printFloorElement(spielfeld[y][x],{x,y});
                }
            }
        glEndList();
        gamefloorPrepared=1;
    }else
    {   glCallList(gamefloor);
    }


    /*/
    for(int y=0;y<size.y;y++)
    {   for(int x=0;x<size.x;x++)
        {   printFloorElement(spielfeld[y][x],{x,y});
        }
    }/* */
}

void LEVEL::printKugelnAtOrigins()
{   KUGELorigin *p=kugelOriginStart;
    while(p!=NULL)
    {   if(p->type==0)  printFloorElement(TILE_KUGEL,{p->origin.x,p->origin.y});
        else            printFloorElement(TILE_KUGELBLOCK,{p->origin.x,p->origin.y});
        p=p->next;
    }
}

void LEVEL::printLocksAtOrigins()
{   LOCKorigin *p=lockOriginStart;
    while(p!=NULL)
    {   printFloorElement(TILE_LOCK,{p->lock.x,p->lock.y},p->color);
        printFloorElement(TILE_KEY,{p->key.x, p->key.y },p->color);
        p=p->next;
    }
}

void LEVEL::printAvatarAtOrigin()
{   printFloorElement(TILE_AVATAR,{avatarOrigin.x,avatarOrigin.y});
}


char* LEVEL::getLevelPath()
{   return path;
}

void LEVEL::getMetaData(char **dateP,char **timeP)
{   *dateP=CreateDate;
    *timeP=CreateTime;
}

void LEVEL::setDisplayOptions(POS _origin,int _elsize)
{
   origin=_origin;
   if(_elsize>=1)
       elsize=_elsize;
   else
       error("LEVEL::setDisplayOptions()","Fehler - Die Elementgroesse ist zu niedrig und wurde deshalb nicht uebernommen - elsize=%d",_elsize);
}

int LEVEL::getStatus()
{  return status;
}


LEVEL::~LEVEL()
{   //Wird auch beim Abbau von abgeerbten Klassen (GAME) auch aufgerufen
    cleanup();
}

int LEVEL::getField(POS position)                           //Gibt den Wert des Spielfeldes zurück
{   return spielfeld[position.y][position.x];
}

void LEVEL::marker(POS position,COLOR color)                //Umrahmt ein bestimmtes Feld (Für debugging-Zwecke)
{   glLineWidth(4);
    POS pos;
    pos.x=origin.x+position.x* elsize;
    pos.y=origin.y+(size.y-position.y-1)* elsize;
    markArea({pos,{pos.x+elsize,pos.y+elsize}},color);
}

FIELDPROPERTY LEVEL::getFieldProperty(OBJEKT object,POS position)         //Gibt die Eigenschaften eines Feldes zurück
{   return fieldproperty[object][(int)spielfeld[position.y][position.x]];
}

POS LEVEL::getTargetBeamer()                                //Gibt die Position des Zielbeamers zurück
{   for(int y=0;y<size.y;y++)
        for(int x=0;x<size.x;x++)
            if(spielfeld[y][x]==TILE_TARGETBEAMER)                         //Zielbeamer gefunden
                return POS{x,y};
    return POS{-1,-1};                                      //existiert nicht
}

POS LEVEL::getSize()                                        //Gibt die Spielfeldgröße zurück
{   return size;
}


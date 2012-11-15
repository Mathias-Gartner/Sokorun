//Version 0.0
//Jakob Maier
//Level-Klasse

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


LEVEL::LEVEL(POS _origin,int _elsize,const char *LVLpath,const bool skipMinorErrors=0)//skipMinorErrors: Wenn Levelfehler nicht zum abbruch führen sollen (für den Editor)
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

    loadLevel(LVLpath,skipMinorErrors); //Level aus der Datei laden

    //gamefloorPrepared=0;
};

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



bool LEVEL::loadLevel(const char *LVLpath,const bool skipMinorErrors=0)//skipMinorErrors: Wenn Levelfehler nicht zum abbruch führen sollen (für den Editor)
{   status=0;                           //Keine gültigen Leveldaten

    FILE *level=fopen(LVLpath,"r");     //Leveldatei öffnen

    int kugelAnz,kanonenAnz,NtransAnz,DtransAnz,lockAnz; //Zwischenwerte
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
                {   error("LEVEL::loadLevel","Die Spielfeldgroesze des geladenen Levels ist zu grosz fuer das Programm. Levelgroesze: (%dx%d) Max. Groesze: (%dx%d)",size.x,size.y,MaxXsize,MaxYsize);
                    return 2;//Spielfeld zu groß für das Programm
                }
            if(freadToChar(level,'['))  return 1;    if(freadPos(&avatarOrigin,level))  return 1;               //Spieler-Position
                if(checkPos(avatarOrigin)&&!skipMinorErrors)                                                    //Verbotene Spielerpsition
                {   error("LEVEL::loadLevel","Die Spielfigur befindet sich auszerhalb des Spielfeldes. Levelgroesze: (%dx%d) Avatarposition: (%dx%d)",size.x,size.y,avatarOrigin.x,avatarOrigin.y);
                    return 3;//Verbotene Spielerpsition
                }
            ///Kugel-Anzahl, Kanonen-Anzahl, Transporter-Anzahl, Tödl.Transporter-Anzahl und Schlösseranzahl laden:
            if(freadToChar(level,'['))  return 1;   if(fscanf(level,"%d]",&kugelAnz) != 1)  { error("LEVEL::loadLevel","Kugelanzahl konnte nicht geladen werden"); return 1; }
                if(kugelAnz<=0 && !skipMinorErrors)                                                             //Zuwenig Kugeln
                {   error("LEVEL::loadLevel","Es befinden sich keine Kugeln im Level, das Spiel kann daher nicht gespielt werden");
                    return 4;//Zuwenig Kugeln
                }
            if(freadToChar(level,'['))  return 1;   if(fscanf(level,"%d]",&kanonenAnz) != 1){ error("LEVEL::loadLevel","Kanonenanzahl konnte nicht geladen werden"); return 1; }
            if(freadToChar(level,'['))  return 1;   if(fscanf(level,"%d]",&NtransAnz) != 1) { error("LEVEL::loadLevel","Anzahl der Transporter konnte nicht geladen werden"); return 1; }
            if(freadToChar(level,'['))  return 1;   if(fscanf(level,"%d]",&DtransAnz) != 1) { error("LEVEL::loadLevel","Anzahl der toedlichen Transporter konnte nicht geladen werden"); return 1; }
            if(freadToChar(level,'['))  return 1;   if(fscanf(level,"%d]",&lockAnz) != 1)   { error("LEVEL::loadLevel","Anzahl der Schluessel und Schloesser konnte nicht geladen werden"); return 1; }

    ///KUGELN:
            if(kugelOriginStart!=NULL)
            {   error("LEVEL::loadLevel","Es befinden sich Kugeln in der Liste, obwohl das Level erste geladen wird. womöglich muss hier jetzt die Liste geleert werden, wurde noch nicht ausprogrammiert");
                for(;;);
            }
            for(int i=0;i<kugelAnz;i++)
            {   KUGELorigin *p=(KUGELorigin*)malloc(sizeof(KUGELorigin));
                p->next=kugelOriginStart;

                if(freadToChar(level,'['))  return 1;    if(freadPos(&(p->origin),level))  return 1;            //Kugelposition
                if(checkPos(p->origin)&&!skipMinorErrors)                                                       //Verbotene Kugelposition
                {   error("LEVEL::loadLevel","Eine Kugel befindet sich auszerhalb des Spielfeldes. Levelgroesze: (%dx%d) Kugelposition: (%dx%d) Kugelnummer: %d",size.x,size.y,p->origin.x,p->origin.y,i);
                    return 5;//Verbotene Kugelposition
                }

                if(fscanf(level,"]*%02d*",&help)==1)//Es wurde ein Kugeltyp angegeben
                {
                    if(help<0||help>1)
                    {   error("LEVEL::loadLevel","Ein Kugelobjekt hat einen ungueltigen typ-Wert. Kugeltyp: %d Kugelnummer: %d",help,i);
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
                {   if(fscanf(level,"%02d",&spielfeld[y][x]) != 1)  { error("LEVEL::loadLevel","Spielfeld konnte nicht geladen werden. Position: (%dx%d)",x,y); return 1; }
                    if(x!=size.x-1 && fgetc(level)!='.'){ error("LEVEL::loadLevel","Spielfeld konnte nicht geladen werden. Position: (%dx%d)",x,y); return 1; }
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


///JETZT HIER Kanonen, Transporter, Tädl.Transporter


    ///SCHLÖSSER:
            if(lockOriginStart!=NULL)
            {   error("LEVEL::loadLevel","Es befinden sich Schloesser in der Liste, obwohl das Level erste geladen wird. womöglich muss hier jetzt die Liste geleert werden, wurde noch nicht ausprogrammiert");
                for(;;);
            }
            for(int i=0;i<lockAnz;i++)
            {   LOCKorigin *p=(LOCKorigin*)malloc(sizeof(LOCKorigin));
                p->next=lockOriginStart;

                if(freadToChar(level,'['))  return 1;    if(freadPos(&(p->lock),level))  return 1;          //Schlossposition
                if(freadToChar(level,'['))  return 1;    if(freadPos(&(p->key),level))  return 1;           //Schlüsselposition


                if(checkPos(p->lock)&&!skipMinorErrors)                                                       //Verbotene Kugelposition
                {   error("LEVEL::loadLevel","Ein Schloss befindet sich auszerhalb des Levels. Levelgroesze: (%dx%d) Schlossposition: (%dx%d) Schlossnummer: %d",size.x,size.y,p->lock.x,p->lock.y,i);
                    return 7;//Verbotene Schlossposition
                }

                if(checkPos(p->key)&&!skipMinorErrors)                                                       //Verbotene Kugelposition
                {   error("LEVEL::loadLevel","Ein Schluessel befindet sich auszerhalb des Levels. Levelgroesze: (%dx%d) Schluesselposition: (%dx%d) Schlossnummer: %d",size.x,size.y,p->key.x,p->key.y,i);
                    return 8;//Verbotene Schluesselposition
                }



                if(fscanf(level,"]*RGB(%1.3f,%1.3f,%1.3f)*",&r,&g,&b)==3)//Es wurde eine Farbe angegeben
                {   if(r<0||r>1 || g<0||g>1 || b<0||b>1)
                    {   error("LEVEL::loadLevel","Ein Schloss hat eine ungueltigen Farbwert. Farbe: (%f, %f, %f) Schlossnummer: %d.",r,g,b,i);
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
    status=1;


//            do{zeichen=fgetc(datei);}while(zeichen!='[');//Erstelldatum
//            do{zeichen=fgetc(datei);}while(zeichen!='[');//Erstellzeit
//            //do{zeichen=fgetc(datei);}while(zeichen!='§');//Das Recht zum
//            do{zeichen=fgetc(datei);}while(zeichen!='[');fscanf(datei,"%2dx%2d",&hilf1,&hilf2);size.x=hilf1;size.y=hilf2;//Levelgröße
//                if(size.x<1||size.x>MAXSIZE_X)  {char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: x-size in %s is %d\n ->cannot open file  (class::editor)\n",time,pfad,size.x);MessBoxError(mess);return;}
//                if(size.y<1||size.y>MAXSIZE_Y)  {char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: y-size in %s is %d\n  ->cannot open file  (class::editor)\n",time,pfad,size.y);MessBoxError(mess);return;}
//
//            if(info)
//            {   sprintf(mess,"SPIELFELDGROESSE geldaden:    (%02.0fx%02.0f)",size.x,size.y);  MessBoxInfo(mess);    }
//
//            do{zeichen=fgetc(datei);}while(zeichen!='[');fscanf(datei,"%2dx%2d",&hilf1,&hilf2);goo.x=hilf1;goo.y=hilf2;
//                if(goo.x<0||goo.x>MAXSIZE_X)  {char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: x-goo in %s is %d  (class::editor)\n",time,pfad,goo.x);MessBoxError(mess);return;}
//                if(goo.y<0||goo.y>MAXSIZE_Y)  {char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: y-goo in %s  (class::editor)\n",time,pfad);MessBoxError(mess);return;}
//
//            if(info)
//            {   sprintf(mess,"GOO geldaden:    (%02.0fx%02.0f)",goo.x,goo.y);  MessBoxInfo(mess);    }
//
//            do{zeichen=fgetc(datei);}while(zeichen!='[');fscanf(datei,"%02d",&hilf);
//                if(hilf<0||hilf>MAX_KUGEL)  {char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: kugelanzahl in %s (%d)  (class::editor)\n",time,pfad,hilf);MessBoxError(mess);return;}
//                for(int i=0;i<MAX_KUGEL;i++){kglakt[i]=0;}for(int i=0;i<hilf;i++){kglakt[i]=1;}anz[0]=hilf;
//
//            if(info)
//            {   sprintf(mess,"KUGELANZAHL geldaden:    %d",anz[0]);  MessBoxInfo(mess);    }
//
//            do{zeichen=fgetc(datei);}while(zeichen!='[');fscanf(datei,"%02d",&hilf);
//                if(hilf<0||hilf>MAX_KANON)  {char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: kanonenanzahl in %s (%d)  (class::editor)\n",time,pfad,hilf);MessBoxError(mess);return;}
//                for(int i=0;i<MAX_KANON;i++){knnakt[i]=0;}for(int i=0;i<hilf;i++){knnakt[i]=1;}anz[1]=hilf;
//            do{zeichen=fgetc(datei);}while(zeichen!='[');fscanf(datei,"%02d",&hilf);
//                if(hilf<0||hilf>MAX_TRANS)  {char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: transporteranzahl in %s (%d)  (class::editor)\n",time,pfad,hilf);MessBoxError(mess);return;}
//                for(int i=0;i<MAX_TRANS;i++){trpakt[i]=0;}for(int i=0;i<hilf;i++){trpakt[i]=1;}anz[2]=hilf;
//            do{zeichen=fgetc(datei);}while(zeichen!='[');fscanf(datei,"%02d",&hilf);
//                if(hilf<0||hilf>MAX_BLOCK)  {char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: blockanzahl in %s (%d)  (class::editor)\n",time,pfad,hilf);MessBoxError(mess);return;}
//                for(int i=0;i<MAX_BLOCK;i++){blkakt[i]=0;}for(int i=0;i<hilf;i++){blkakt[i]=1;}anz[3]=hilf;
//            do{zeichen=fgetc(datei);}while(zeichen!='[');fscanf(datei,"%02d",&hilf);
//                if(hilf<0||hilf>MAX_SCHLS)  {char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: schlossanzahl in %s (%d)  (class::editor)\n",time,pfad,hilf);MessBoxError(mess);return;}
//                for(int i=0;i<MAX_SCHLS;i++){slsakt[i]=0;}for(int i=0;i<hilf;i++){slsakt[i]=1;}anz[4]=hilf;
//            if(info)
//            {   sprintf(mess,"KANONENANZAHL geldaden:    \t%d\nTRANSPORTERANZAHL geldaden:\t%d\nBLOCKANZAHL geldaden:    \t\t%d\nSCHLOSSANZAHL geldaden:    \t\t%d  (class::editor)",anz[1],anz[2],anz[3],anz[4]);  MessBoxInfo(mess);    }
//
//            for(int i=0;i<anz[0];i++)/*KUGELN*/
//            {   do{zeichen=fgetc(datei);}while(zeichen!='[');fscanf(datei,"%02dx%02d",&hilf1,&hilf2);kgl[i].x=hilf1;kgl[i].y=hilf2;
//                if(kgl[i].x<0||kgl[i].x>MAXSIZE_X||kgl[i].y<0||kgl[i].y>MAXSIZE_Y||kglakt[i]<1)
//                {char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: kugeldetails in %s (nummer %d)\nx:  %d   y:  %d   akt:  %d  (class::editor)\n",time,pfad,i+1,kgl[i].x,kgl[i].y,kglakt[i]);MessBoxError(mess);return;}
//                if(info)
//                {   sprintf(mess,"KUGEL geldaden:\n\nNummer:\t\t%d/%d\nPosition:\t\t(%02.0fx%02.0f)",i+1,anz[0],kgl[i].x,kgl[i].y);
//                    MessBoxInfo(mess);
//                }
//            }
//            sprintf(mess,"SPIELFELD geladen:\n\n");
//            for(int y=0;y<size.y;y++)/*Daten*/
//            {   do{zeichen=fgetc(datei);}while(zeichen!='#');
//                for(int x=0;x<size.x;x++)
//                {   fscanf(datei,"%02d%c",&(feld[x][y]),&zeichen);
//                    if(feld[x][x]<0||feld[x][x]>40){char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: element in %s on position (%02.0fx%02.0f) is %d  (class::editor)\n",time,pfad,x,y,feld[x][x]);MessBoxError(mess);return;}
//                    sprintf(mess,"%s%02d",mess,feld[x][y]);
//                    if(x<size.x-1){sprintf(mess,"%s-",mess);}
//                    /**!!!**/                           /*^^*/
//                }
//                sprintf(mess,"%s\n",mess);
//            }if(info){   MessBoxInfo(mess); }
//
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
//
//            for(int i=0;i<anz[2];i++)//Transporter
//            {   do{zeichen=fgetc(datei);}while(zeichen!='[');fscanf(datei,"%02d",&hilf);/*länge*/
//                    if(hilf<2||hilf>=MAX_FIELD){char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: transporter-laenge of %d in %s (%d)  (class::editor)\n",time,i,pfad,hilf);MessBoxError(mess);return;}
//                    trplen[i]=hilf;
//                do{zeichen=fgetc(datei);}while(zeichen!='[');fscanf(datei,"%1d",&hilf1);/*typ*/
//                    if(hilf1<1||hilf1>2){char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: transporter-typ of %d in %s (%d)  (class::editor)\n",time,i,pfad,hilf1);MessBoxError(mess);return;}
//                    trptyp[i]=hilf1;
//                do{zeichen=fgetc(datei);}while(zeichen!='[');fscanf(datei,"%02d",&hilf1);/*Startposition*/
//                    if(hilf1<0||hilf1>=trplen[i]){char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: transporter-startposition of %d in %s (%d)  (class::editor)\n",time,i+1,pfad,hilf1);MessBoxError(mess);return;}
//                    trpSps[i]=hilf1;
//                for(int j=0;j<trplen[i];j++)
//                {   do{zeichen=fgetc(datei);}while(zeichen!='[');fscanf(datei,"%02dx%02d",&hilf1,&hilf2);trpPos[i][j].x=hilf1;trpPos[i][j].y=hilf2;
//                    if(trpPos[i][j].x<0||trpPos[i][j].x>MAXSIZE_X)  {char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: invalid argument: x-transporter of No. %d at field %d in %s is %d  (class::editor)\n",time,i+1,j,pfad,trpPos[i][j].x);MessBoxError(mess);return;}
//                    if(trpPos[i][j].y<0||trpPos[i][j].y>MAXSIZE_Y)  {char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: invalid argument: y-transporter of No. %d at field %d in %s is %d  (class::editor)\n",time,i+1,j,pfad,trpPos[i][j].y);MessBoxError(mess);return;}
//                }
//                if(info)
//                {   sprintf(mess,"TRANSPORTER geldaden:\n\nNummer:\t\t%d/%d\nLänge:\t\t%d\nTyp:\t\t%d\nStartposition:\t%d",i+1,anz[2],trplen[i],trptyp[i],trpSps[i]);
//                    MessBoxInfo(mess);
//                }
//            }
//
//            for(int i=0;i<anz[3];i++)//Blöcke (Gegner)
//            {   do{zeichen=fgetc(datei);}while(zeichen!='[');fscanf(datei,"%02d",&hilf);/*länge*/
//                    if(hilf<2||hilf>=MAX_FIELD){char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: block-laenge of %d in %s (%d)  (class::editor)\n",time,i+1,pfad,hilf);MessBoxError(mess);return;}
//                    blklen[i]=hilf;
//                do{zeichen=fgetc(datei);}while(zeichen!='[');fscanf(datei,"%1d",&hilf1);/*typ*/
//                    if(hilf1<1||hilf1>2){char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: block-typ of %d in %s (%d)  (class::editor)\n",time,i+1,pfad,hilf1);MessBoxError(mess);return;}
//                    blktyp[i]=hilf1;
//                do{zeichen=fgetc(datei);}while(zeichen!='[');fscanf(datei,"%02d",&hilf1);/*Startposition*/
//                    if(hilf1<0||hilf1>=blklen[i]){char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: wrong filedata: block-startposition of %d in %s (%d)  (class::editor)\n",time,i+1,pfad,hilf1);MessBoxError(mess);return;}
//                    blkSps[i]=hilf1;
//                for(int j=0;j<blklen[i];j++)
//                {   do{zeichen=fgetc(datei);}while(zeichen!='[');fscanf(datei,"%02dx%02d",&hilf1,&hilf2);blkPos[i][j].x=hilf1;blkPos[i][j].y=hilf2;
//                if(trpPos[i][j].x<0||trpPos[i][j].x>MAXSIZE_X)  {char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: invalid argument: x-block of No. %d at field %d in %s is %d\n",time,i+1,j,pfad,trpPos[i][j].x);MessBoxError(mess);return;}
//                        if(trpPos[i][j].y<0||trpPos[i][j].y>MAXSIZE_Y)  {char time[20];_strtime(time);sprintf(mess,"[%s] ERROR: invalid argument: y-block of No. %d at field %d in %s is %d\n",time,i+1,j,pfad,trpPos[i][j].y);MessBoxError(mess);return;}
//                }
//                if(info)
//                {   sprintf(mess,"BLOCK geldaden:\n\nNummer:\t\t%d/%d\nLänge:\t\t%d\nTyp:\t\t%d\nStartposition:\t%d",i+1,anz[3],blklen[i],blktyp[i],blkSps[i]);
//                    MessBoxInfo(mess);
//                }
//            }
//
//
//                do{zeichen=fgetc(datei);}while(zeichen!='>');fgets(levelname,30,datei);
//                if(strlen(levelname)==0||strcmp(levelname,"---")==0) {strcpy(levelname,"unbenannt");}
//
//            do{zeichen=fgetc(datei);}while(zeichen!='>');//Geschrieben mit
//
//            do{zeichen=fgetc(datei);}while(zeichen!='>');fgets(programmer,30,datei);
//                if(strlen(programmer)==0||strcmp(programmer,"---")==0) {strcpy(programmer,"unbekannt");}
//
//            do{zeichen=fgetc(datei);}while(zeichen!='>');fgets(sonstiges[0],30,datei);
//                if(strlen(sonstiges[0])==0) {strcpy(sonstiges[0],"---");}
//            do{zeichen=fgetc(datei);}while(zeichen!='>');fgets(sonstiges[1],30,datei);
//                if(strlen(sonstiges[1])==0) {strcpy(sonstiges[1],"---");}
//            do{zeichen=fgetc(datei);}while(zeichen!='>');fgets(sonstiges[2],30,datei);
//                if(strlen(sonstiges[2])==0) {strcpy(sonstiges[2],"---");}
//
//            if(levelname[strlen(levelname)-1]=='\n'){levelname[strlen(levelname)-1]=0;}
//            if(programmer[strlen(programmer)-1]=='\n'){programmer[strlen(programmer)-1]=0;}
//                    for(int i=0;i<3;i++)
//            {   if(sonstiges[i][strlen(sonstiges[i])-1]=='\n'){sonstiges[i][strlen(sonstiges[i])-1]=0;}
//            }
//
//            if(info)
//            {   sprintf(mess,"DATEIINFORMATIONEN geldaden:\n\nLevelname:\t%s\nErsteller:\t\t%s\nSonstiges:\n\t%s\n\t%s\n\t%s\n\nLADEVORGANG ABGESCHLOSSEN",levelname,programmer,sonstiges[0],sonstiges[1],sonstiges[2]);
//                MessBoxInfo(mess);
//            }
//
//
//            fclose(datei);
//            size=pos(15,15);






    fclose(level);

    strcpy(path,LVLpath);
    return 0;
};


void LEVEL::printFloorElement(int element,POS coord,COLOR color=WHITE)
{   POS pos;
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
        buildupAnimationProgress+=2;
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
        buildupAnimationProgress++;
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
    //Transporter
    printKugelnAtOrigins();
    printAvatarAtOrigin();
    printLocksAtOrigins();//Schlösser
    //Kanonen
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
    status=0;

    //Verkettete Liste mit Kugeln freigeben:
    KUGELorigin *p=kugelOriginStart,*q;
    while(p!=NULL)
    {   q=(p->next);
        free(p);
        p=q;
    }
    kugelOriginStart=NULL;
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
{   return fieldproperty[object][spielfeld[position.y][position.x]];
}

POS LEVEL::getTargetBeamer()                                //Gibt die Position des Zielbeamers zurück
{   for(int y=0;y<size.y;y++)
        for(int x=0;x<size.x;x++)
            if(spielfeld[y][x]==35)                         //Zielbeamer gefunden
                return POS{x,y};
    return POS{-1,-1};                                      //existiert nicht
}

POS LEVEL::getSize()                                        //Gibt die Spielfeldgröße zurück
{   return size;
}

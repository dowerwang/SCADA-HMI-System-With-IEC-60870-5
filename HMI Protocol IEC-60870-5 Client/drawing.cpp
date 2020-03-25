#include "drawing.h"
#include <QDebug>

drawing::drawing(QWidget *parent) : QWidget(parent)
{
}

void drawing::setSmallDonut()
{
    // set warna donutchart
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(139, 139, 139));
    painter.drawPie(QRectF(0,0,lebar,tinggi),0,360*16);
    if(warna==1){
        painter.setBrush(QColor(255, 143, 145)); // set warna pink
    }
    else if(warna==2){
        painter.setBrush(QColor(222, 192, 86)); // set warna kuning
    }
    else if(warna==3){
        painter.setBrush(QColor(105, 187, 235)); // set warna biru
    }
    painter.setPen(Qt::NoPen);
    painter.drawPie(QRectF(0,0,lebar,tinggi),0,slide*16);

    // set warna hole donutchart
    painter.setBrush(QColor(230, 230, 230));
    painter.setPen(QColor(230, 230, 230));
    painter.drawPie(QRectF(lebar/8.5,tinggi/8.5,lebar/1.3,tinggi/1.3),0,360*16);
}

void drawing::setOverview()
{
    int y1=80,h1=370, param1= 115;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QColor purple(255,85,255);

    QPen pen3(QColor(Qt::red));
    pen3.setWidth(4);
    pen3.setCapStyle(Qt::RoundCap);

    QPen pen2(QColor(Qt::white));
    pen2.setWidth(4);
//xx
    QPen pen1(QColor(Qt::black));
    pen1.setWidth(4);
    pen1.setCapStyle(Qt::RoundCap);
    painter.setPen(pen1);
    painter.drawRoundedRect(170,100,200,340,10,10);

    for(char i=0;i<3;i++){
        painter.drawRoundedRect(205,param1,133,74,5,5);
        painter.drawLine(225,param1+15,237,param1+15);
        painter.drawLine(231,param1+10,231,param1+20);
        painter.drawRoundedRect(225,param1-9,12,9,2,2);
        painter.drawRoundedRect(310,param1-9,12,9,2,2);
        painter.drawLine(309,param1+15,321,param1+15);
        param1 = param1+100;
    }

    painter.drawLine(370,225,370+50,225);
    //battery
    if(valueBTRStat == true){//RTU off
        painter.setBrush(purple);
        painter.drawRect(420,210,30,30);
    }
    else{//RTU on
        painter.setPen(pen1);
        if(valueBTR == 0){
            //line to battery
            painter.setBrush(QColor(Qt::black));
            painter.drawRect(420,210,30,30);

            //indicator battery
            painter.setPen(pen3);
            painter.drawLine(415,195,455,195);
            if(valueBTRindi == false){//arah batery
                painter.drawLine(425,188,415,195);
                painter.drawLine(425,203,415,195);
            }
            else{//arah BDI
                painter.drawLine(445,188,455,195);
                painter.drawLine(445,203,455,195);
            }
        }
        else if(valueBTR == 1){
            //line to battery
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(420,210,30,30);
        }
        else{
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(420,210,30,30);
            painter.setBrush(QColor(Qt::black));
            for(char i=0;i<30;i++)
                painter.drawRect(420+i,210+i,1,29-i);
        }
    }

    //busbar
    painter.setBrush(QColor(Qt::gray));
    painter.setPen(Qt::NoPen);
    painter.drawRect(370+111,y1+20,12,h1-60);//busbar1
    painter.drawRect(370+512,y1+20,12,h1-60);//busbar2

    painter.setPen(pen1);
    painter.drawLine(370+81,225,370+110,225);
    painter.drawLine(370+125,(450+60)/2,370+170,(450+60)/2);

    if(valueSWT == false){
        //switch
        painter.drawLine(370+170,(450+30)/2,370+205,(450+60)/2);
    }
    else{
        //switch
        painter.drawLine(370+170,(450+60)/2,370+205,(450+60)/2);
    }
    painter.drawLine(370+205,(450+60)/2,370+260,(450+60)/2);

    //BDI
    painter.setPen(pen2);
    painter.setBrush(QColor(Qt::black));
    painter.drawRoundedRect(370+250,(450-150)/2,140,250,10,10);
    painter.drawLine(370+388,(450-138)/2,370+253,(450+340)/2);

    //after BDI
    painter.setPen(pen1);
    painter.drawLine(370+394,(450+60)/2,370+440,(450+60)/2);

    if(valueBDIStat == true){//RTU off
        painter.setBrush(purple);
        painter.drawRect(810,240,30,30);
    }
    else{//RTU on
        if(valueBDI == 0){
            //line out BDI
            painter.setBrush(QColor(Qt::black));
            painter.drawRect(810,240,30,30);
            //indicator BDI
            painter.setPen(pen3);
            painter.drawLine(805,225,845,225);
            if(valueBDIindi == 0){
                painter.drawLine(815,218,805,225);
                painter.drawLine(815,233,805,225);
            }
            else{
                painter.drawLine(835,218,845,225);
                painter.drawLine(835,233,845,225);
            }
        }
        else if(valueBDI == 1){
            //line out BDI
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(810,240,30,30);
        }
        else{
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(810,240,30,30);
            painter.setBrush(QColor(Qt::black));
            for(char i=0;i<30;i++)
                painter.drawRect(810+i,240+i,1,29-i);
        }
    }

    painter.setPen(pen1);
    painter.drawLine(370+470,(450+60)/2,370+510,(450+60)/2);
    painter.drawLine(370+525,126,370+561,126);

    //plts
    if(valuePltsStat == true){//RTU off
        painter.setBrush(purple);
        painter.drawRect(370+565,110,30,30);
    }
    else{//RTU on
        if(valuePlts == 0){
            //line to plts
            painter.setBrush(QColor(Qt::black));
            painter.drawRect(370+565,110,30,30);

            //indicator RTU
            painter.drawLine(370+570,86,370+560,94);
            painter.drawLine(370+570,102,370+560,94);
            painter.drawLine(370+560,94,370+600,94);
        }
        else if(valuePlts == 1){
            //line to plts
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(370+565,110,30,30);
        }
        else{
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(370+565,110,30,30);
            painter.setBrush(QColor(Qt::black));
            for(char i=0;i<30;i++)
                painter.drawRect(370+565+i,110+i,1,29-i);
        }
    }
    painter.setPen(pen1);
    painter.drawLine(370+599,126,370+635,126);
    painter.setBrush(QColor(Qt::black));
    painter.drawRoundedRect(370+635,y1,150,90,10,10);
    painter.drawLine(370+525,255,370+561,255);

    //pltmh
    if(valuePltmhStat == true){//RTU off
        painter.setBrush(purple);
        painter.drawRect(370+565,240,30,30);
    }
    else{//RTU on
        if(valuePltmh == 0){
            //line to pltmh
            painter.setBrush(QColor(Qt::black));
            painter.drawRect(370+565,240,30,30);

            //indicator RTU
            painter.drawLine(370+570,216,370+560,224);
            painter.drawLine(370+570,232,370+560,224);
            painter.drawLine(370+560,224,370+600,224);
        }
        else if(valuePltmh == 1){
            //line to pltmh
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(370+565,240,30,30);
        }
        else{
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(370+565,240,30,30);
            painter.setBrush(QColor(Qt::black));
            for(char i=0;i<30;i++)
                painter.drawRect(370+565+i,240+i,1,29-i);
        }
    }
    painter.setPen(pen1);
    painter.drawLine(370+599,255,370+635,255);
    painter.setBrush(QColor(Qt::black));
    painter.drawRoundedRect(370+635,y1+130,150,90,10,10);
    painter.drawLine(370+525,387,370+561,387);

    //load
    if(valueLoadStat == true){//RTU off
        painter.setBrush(purple);
        painter.drawRect(370+565,372,30,30);
    }
    else{//RTU on
        if(valueLoad == 0){
            //line to load
            painter.setBrush(QColor(Qt::black));
            painter.drawRect(370+565,372,30,30);

            //indicator RTU
            painter.drawLine(370+590,348,370+600,356);
            painter.drawLine(370+590,364,370+600,356);
            painter.drawLine(370+560,356,370+600,356);
        }
        else if(valueLoad == 1){
            //line to load
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(370+565,372,30,30);
        }
        else{
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(370+565,372,30,30);
            painter.setBrush(QColor(Qt::black));
            for(char i=0;i<30;i++)
                painter.drawRect(370+565+i,372+i,1,29-i);
        }
    }
    painter.setPen(pen1);
    painter.drawLine(370+599,387,370+635,387);
    painter.setBrush(QColor(Qt::black));
    painter.drawRoundedRect(370+635,y1+260,150,90,10,10);
}

void drawing::setPlts()
{
    int param1 = 40, param2 = 18, param3 = 0, param_line =0;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QColor purple(255,85,255);
    QPen black(QColor(Qt::black));
    black.setWidth(3);
    QPen red(QColor(Qt::red));
    red.setWidth(3);
    QPen yellow(QColor(255,218,68));
    yellow.setWidth(3);
    QPen blue(QColor(Qt::blue));
    blue.setWidth(3);

    painter.setPen(black);
    //PV
    for(char i=0;i<3;i++){
        painter.drawEllipse(QRectF(945,param1-9,95,95));
        painter.drawPixmap(950,param1,75,75,QPixmap(":/img/solarPower.png"));
        param1 = param1+150;
    }
    painter.setPen(red);
    painter.drawLine(943,80+param_line,775,80+param_line);
    painter.drawLine(605,80,500,80);

    painter.setPen(yellow);
    painter.drawLine(943,230+param_line,775,230+param_line);
    painter.drawLine(605,80+150,500,80+150);

    painter.setPen(blue);
    painter.drawLine(943,380+param_line,775,380+param_line);
    painter.drawLine(605,80+300,500,80+300);

    for(char i=0;i<3;i++){
        painter.setPen(black);
        painter.drawLine(800,72+param3,790,64+param3);
        painter.drawLine(800,56+param3,790,64+param3);
        painter.drawLine(790,64+param3,830,64+param3);
        param3 = param3+150;
        painter.setPen(QColor(Qt::white));
        painter.setBrush(QColor(Qt::black));
        painter.drawRoundedRect(600,param2,180,125,10,10);
        param2= param2 + 150;
    }

    painter.setPen(black);
    //inverter R
    if(valuePltsInv1stat == true){//RTU off
        painter.setBrush(purple);
        painter.setPen(black);
        painter.drawRect(467,64,30,30);
    }
    else{//RTU on
        painter.setPen(black);
        if(valuePltsInv1 == 0){
            painter.setPen(black);
            //line to RTU
            painter.setBrush(QColor(Qt::black));
            painter.drawRect(467,64,30,30);

            //indicator RTU
            painter.drawLine(472,40,462,48);
            painter.drawLine(472,56,462,48);
            painter.drawLine(462,48,502,48);
        }
        else if(valuePltsInv1 == 1){
            //line to battery
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(467,64,30,30);
        }
        else{
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(467,64,30,30);
            painter.setBrush(QColor(Qt::black));
            for(char i=0;i<30;i++)
                painter.drawRect(467+i,64+i,1,29-i);
        }
    }
    painter.setPen(red);
    painter.drawLine(464,80,420,80);

    //inverter S
    if(valuePltsInv2stat == true){//RTU off
        painter.setBrush(purple);
        painter.setPen(black);
        painter.drawRect(467,64+150,30,30);
    }
    else{//RTU on
        painter.setPen(black);
        if(valuePltsInv2 == 0){
            painter.setPen(black);
            //line to RTU
            painter.setBrush(QColor(Qt::black));
            painter.drawRect(467,64+150,30,30);

            //indicator RTU
            painter.drawLine(472,40+150,462,48+150);
            painter.drawLine(472,56+150,462,48+150);
            painter.drawLine(462,48+150,502,48+150);
        }
        else if(valuePltsInv2 == 1){
            //line to battery
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(467,64+150,30,30);
        }
        else{
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(467,64+150,30,30);
            painter.setBrush(QColor(Qt::black));
            for(char i=0;i<30;i++)
                painter.drawRect(467+i,64+150+i,1,29-i);
        }
    }
    painter.setPen(yellow);
    painter.drawLine(464,80+150,420,80+150);

    //inverter T
    if(valuePltsInv3stat == true){//RTU off
        painter.setBrush(purple);
        painter.setPen(black);
        painter.drawRect(467,64+300,30,30);
    }
    else{//RTU on
        painter.setPen(black);
        if(valuePltsInv3 == 0){
            painter.setPen(black);
            //line to RTU
            painter.setBrush(QColor(Qt::black));
            painter.drawRect(467,64+300,30,30);

            //indicator RTU
            painter.drawLine(472,40+300,462,48+300);
            painter.drawLine(472,56+300,462,48+300);
            painter.drawLine(462,48+300,502,48+300);
        }
        else if(valuePltsInv3 == 1){
            //line to battery
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(467,64+300,30,30);
        }
        else{
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(467,64+300,30,30);
            painter.setBrush(QColor(Qt::black));
            for(char i=0;i<30;i++)
                painter.drawRect(467+i,64+300+i,1,29-i);
        }
    }
    painter.setPen(blue);
    painter.drawLine(464,80+300,420,80+300);

    //busbar
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::gray);
    painter.drawRect(402,30,20,400);

    painter.setPen(black);
    painter.drawLine(400,80+150,350,80+150);
    painter.drawLine(317,80+150,273,80+150);

    //Inverter Out
    if(valuePltsOutstat == true){//RTU off
        painter.setBrush(purple);
        painter.setPen(black);
        painter.drawRect(320,214,30,30);
    }
    else{//RTU on
        painter.setPen(black);
        if(valuePltsOut == 0){
            painter.setPen(black);
            //indicator RTU
            painter.drawLine(325,190,315,198);
            painter.drawLine(325,206,315,198);
            painter.drawLine(315,198,355,198);

            //line RTU
            painter.setBrush(QColor(Qt::black));
            painter.drawRect(320,214,30,30);
        }
        else if(valuePltsOut == 1){
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(320,214,30,30);
        }
        else{
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(320,214,30,30);
            painter.setBrush(QColor(Qt::black));
            for(char i=0;i<30;i++)
                painter.drawRect(320+i,214+i,1,29-i);
        }
    }
}

void drawing::setPltmh()
{
    int param1 = 600;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QColor purple(255,85,255);
    QPen black(QColor(Qt::black));
    black.setWidth(3);

    //line
    painter.setPen(black);
    painter.drawLine(param1+170,80+150,param1+30,80+150);
    painter.drawLine(param1-3,80+150,param1-47,80+150);

    //pltmh
   // painter.setBrush(QColor("Pink"));
   // painter.drawRoundedRect(param1+70,168,300,200,10,10);

    //pltmh RTU
    if(valuePltmhOutstat == true){//RTU off
        painter.setBrush(purple);
        painter.setPen(black);
        painter.drawRect(param1,214,30,30);
    }
    else{//RTU on
        painter.setPen(black);
        if(valuePltmhOut == 0){
            painter.setPen(black);
            //indicator RTU
            painter.drawLine(param1+5,190,param1-5,198);
            painter.drawLine(param1+5,206,param1-5,198);
            painter.drawLine(param1-5,198,param1+35,198);

            //line RTU
            painter.setBrush(QColor(Qt::black));
            painter.drawRect(param1,214,30,30);
        }
//        else if(valuePltmhOut == 1){
//            painter.setBrush(QColor("Pink"));
//            painter.drawRect(param1,214,30,30);
//        }
        else if(valuePltmhOut == 1){
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(param1,214,30,30);
            painter.setBrush(QColor(Qt::black));
            for(char i=0;i<30;i++)
                painter.drawRect(param1+i,214+i,1,29-i);
        }
    }

    //busbar
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::gray);
    painter.drawRect(param1-55,150,13,400);
}

void drawing::setLoad()
{
    int param1 = 600, param2=75, param3 = 60;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QColor purple(255,85,255);
    QPen black(QColor(Qt::black));
    black.setWidth(3);
    QPen red(QColor(Qt::red));
    red.setWidth(3);
    QPen yellow(QColor(255,218,68));
    yellow.setWidth(3);
    QPen blue(QColor(Qt::blue));
    blue.setWidth(3);

    //line SMALL TO LOAD
    param2 = param2+220;
    painter.setPen(black);
    //int x1, int y1, int x2, int y2
    painter.drawLine(param1+70,param2,param1+30,param2);
    // -70 line dari status ke ac bus
    painter.drawLine(param1-3,param2,param1-70,param2);

    //Load KOTAK
    painter.setPen(black);
    painter.setBrush(Qt::black);
    //int x, int y, int w, int h, qreal xRadius ( lonjong x ), qreal yRadius (lonjong y),Qt::SizeMode mode
    painter.drawRoundedRect(param1+70,200,250,200,10,10 );

    //Load RTU
    if(valueLoadTstat == true){//RTU off
        param3 = param3+220;
        painter.setBrush(purple);
        painter.setPen(black);
        param3 = param3+220;
        painter.drawRect(param1,param3,30,30);
    }
    else{//RTU on
        painter.setPen(black);
        param3 = param3+220;
        if(valueLoadT == 0){
            painter.setPen(black);
            //indicator RTU
            painter.drawLine(param1+20,param3-24,param1+35,param3-16);
            painter.drawLine(param1+20,param3-8,param1+35,param3-16);
            painter.drawLine(param1-5,param3-16,param1+35,param3-16);

            //line RTU
            painter.setBrush(QColor(Qt::black));
            painter.drawRect(param1,param3,30,30);
        }
        else if(valueLoadT == 1){
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(param1,param3,30,30);
        }
        else{// RTU INVALID
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(param1,param3,30,30);
            painter.setBrush(QColor(Qt::black));
            for(char i=0;i<30;i++)
                painter.drawRect(param1+i,param3+i,1,29-i);
        }
    }

    //busbar POSITION
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::gray);
    //int x, int y, int w, int h
    painter.drawRect(param1-80,200,13,250);
}

void drawing::setInverter()
{
    int param1 = 300, param2;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QColor purple(255,85,255);
    QPen black(QColor("#FF69FF"));
    black.setWidth(3);
    QPen red(QColor(Qt::red));
    red.setWidth(3);
    QPen yellow(QColor(255,218,68));
    yellow.setWidth(3);
    QPen blue(QColor(Qt::blue));
    blue.setWidth(3);
    QPen white(QColor(Qt::white));
    white.setWidth(4);

    painter.setBrush(QColor(Qt::gray));
    painter.setPen(Qt::NoPen);
    //int x, int y, int w, int h
    painter.drawRect(370,200,12,400);//busbar1
    painter.drawRect(1050,200,12,400);//busbar2

    painter.setPen(black);
    for(char i=0;i<3;i++){
        painter.setPen(black);
        painter.drawLine(384,param1,530,param1);
        painter.drawLine(570,param1,610,param1);
        painter.drawLine(720,param1,1048,param1);
        painter.setPen(white);
        painter.setBrush(QColor("#FF69FF"));
        painter.drawRoundedRect(612,param1-60,110,120,10,10);
        painter.drawLine(718,param1-55,615,param1+53);
        param1 = param1 +125;
    }
    painter.setPen(black);
    //Switch R
    if(valueInvRSw == false){
        param1 = 300;
        painter.drawLine(532,param1-15,568,param1);
    }
    else{
        param1 = 300;
        painter.drawLine(532,param1,568,param1);
    }

    //Switch S
    if(valueInvSSw == false){
        param1 = 100+325;
        painter.drawLine(532,param1-15,568,param1);
    }
    else{
        param1 = 100+325;
        painter.drawLine(532,param1,568,param1);
    }

    //Switch T
    if(valueInvTSw == false){
        param1 = 100+450;
        painter.drawLine(532,param1-15,568,param1);
    }
    else{
        param1 = 100+450;
        painter.drawLine(532,param1,568,param1);
    }

    //CB inverter R
    if(valueInvRstat == true){//RTU off
        param1 = 270;
        param2 = 765;
        painter.setPen(black);
        painter.setBrush(purple);
        painter.drawRect(param2+5,param1+15,30,30);
    }
    else{//RTU on
        painter.setPen(black);
        param1 = 270;
        param2 = 765;
        if(valueInvR == 0){
            //line to battery
            painter.setBrush(QColor(Qt::black));
            painter.drawRect(param2+5,param1+15,30,30);

            //indicator battery
            painter.setPen(red);
            painter.drawLine(param2,param1,param2+40,param1);
            if(valueInvRdir == false){//arah batery
                painter.drawLine(param2+30,param1-8,param2+40,param1);
                painter.drawLine(param2+30,param1+8,param2+40,param1);
            }
            else{//arah BDI
                painter.drawLine(param2+10,param1-8,param2,param1);
                painter.drawLine(param2+10,param1+8,param2,param1);
            }
        }
        else if(valueInvR == 1){
            //line to battery
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(param2+5,param1+15,30,30);
        }
        else{
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(param2+5,param1+15,30,30);
            painter.setBrush(QColor(Qt::black));
            for(char i=0;i<30;i++)
                painter.drawRect(param2+5+i,param1+15+i,1,29-i);
        }
    }

    //CB inverter S
    if(valueInvSstat == true){//RTU off
        param1 = 70+325;
        param2 = 765;
        painter.setPen(black);
        painter.setBrush(purple);
        painter.drawRect(param2+5,param1+15,30,30);
    }
    else{//RTU on
        painter.setPen(black);
        param1 = 70+325;
        param2 = 765;
        if(valueInvS == 0){
            //line to battery
            painter.setBrush(QColor(Qt::black));
            painter.drawRect(param2+5,param1+15,30,30);

            //indicator battery
            painter.setPen(red);
            painter.drawLine(param2,param1,param2+40,param1);
            if(valueInvSdir == false){//arah batery
                painter.drawLine(param2+30,param1-8,param2+40,param1);
                painter.drawLine(param2+30,param1+8,param2+40,param1);
            }
            else{//arah BDI
                painter.drawLine(param2+10,param1-8,param2,param1);
                painter.drawLine(param2+10,param1+8,param2,param1);
            }
        }
        else if(valueInvS == 1){
            //line to battery
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(param2+5,param1+15,30,30);
        }
        else{
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(param2+5,param1+15,30,30);
            painter.setBrush(QColor(Qt::black));
            for(char i=0;i<30;i++)
                painter.drawRect(param2+5+i,param1+15+i,1,29-i);
        }
    }

    //CB inverter R
    if(valueInvTstat == true){//RTU off
        param1 = 70+450;
        param2 = 765;
        painter.setPen(black);
        painter.setBrush(purple);
        painter.drawRect(param2+5,param1+15,30,30);
    }
    else{//RTU on
        painter.setPen(black);
        param1 = 70+450;
        param2 = 765;
        if(valueInvT == 0){
            //line to battery
            painter.setBrush(QColor(Qt::black));
            painter.drawRect(param2+5,param1+15,30,30);

            //indicator battery
            painter.setPen(red);
            painter.drawLine(param2,param1,param2+40,param1);
            if(valueInvTdir == false){//arah batery
                painter.drawLine(param2+30,param1-8,param2+40,param1);
                painter.drawLine(param2+30,param1+8,param2+40,param1);
            }
            else{//arah BDI
                painter.drawLine(param2+10,param1-8,param2,param1);
                painter.drawLine(param2+10,param1+8,param2,param1);
            }
        }
        else if(valueInvT == 1){
            //line to battery
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(param2+5,param1+15,30,30);
        }
        else{
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(param2+5,param1+15,30,30);
            painter.setBrush(QColor(Qt::black));
            for(char i=0;i<30;i++)
                painter.drawRect(param2+5+i,param1+15+i,1,29-i);
        }
    }
}

void drawing::setBattery()
{
    int param1 = 194, param2 = 1060;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QColor purple(255,85,255);
    QPen black(QColor(Qt::black));
    black.setWidth(3);
    QPen red(QColor(Qt::red));
    red.setWidth(3);
    QPen yellow(QColor(255,218,68));
    yellow.setWidth(3);
    QPen blue(QColor(Qt::blue));
    blue.setWidth(3);
    QPen white(QColor(Qt::white));
    white.setWidth(4);

    painter.setPen(black);
    //int x, int y, int w, int h
   // painter.drawRect(2,2,1000,400);
    //int x1, int y1, int x2, int y2
    painter.drawLine(1002,224,1152, 224);

    //CB Battery
    if(valueBtrOutstat == true){//RTU off
        painter.setPen(black);
        painter.setBrush(purple);
        painter.drawRect(param2+5,param1+15,30,30);
    }
    else{//RTU on
        painter.setPen(black);
        if(valueBtrOut == 0){
            //line to battery
            painter.setBrush(QColor(Qt::black));
            painter.drawRect(param2+5,param1+15,30,30);

            //indicator battery
            painter.setPen(red);
            painter.drawLine(param2,param1,param2+40,param1);
            if(valueBtrOutindi == true){//arah batery
                painter.drawLine(param2+30,param1-8,param2+40,param1);
                painter.drawLine(param2+30,param1+8,param2+40,param1);
            }
            else{//arah BDI
                painter.drawLine(param2+10,param1-8,param2,param1);
                painter.drawLine(param2+10,param1+8,param2,param1);
            }
        }
        else if(valueBtrOut == 1){
            //line to battery
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(param2+5,param1+15,30,30);
        }
        else{
            painter.setBrush(QColor(Qt::white));
            painter.drawRect(param2+5,param1+15,30,30);
            painter.setBrush(QColor(Qt::black));
            for(char i=0;i<30;i++)
                painter.drawRect(param2+5+i,param1+15+i,1,29-i);
        }
    }
}

void drawing::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    // set active vector
    switch(set){
    case 1:
        //donutchart pltmh
        setSmallDonut();
        break;
    case 2:
       // setOverview();
        break;
    case 3:
      //  setPlts();
        break;
    case 4:
     //   setPltmh();
        break;
    case 5:
      //  setLoad();
        break;
    case 6:
      //  setInverter();
        break;
    case 7:
      //  setBattery();
        break;
    }
}


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <math.h>
#include <QDebug>

struct conte
{
    double a=0;
    int b;
};

conte cont[ 100 ];
QString show_sc;
int bu_count = 0, sc_count = 0;  //数组，字符串计数
int statu=0;                     //运算模式
double outc;                     //输出
int modout = 10;                 //输出进制

void MainWindow::start_count()
{
    bool i = MainWindow::judge_line1();
    if ( i == 0 ) {  ui->lineEdit->setText(QString("ERROR"));  return; }

    trans();

    i = judge_line2();
    if ( i == 0 ) {  ui->lineEdit->setText(QString("ERROR"));  return; }

    outc = decisi(0 , bu_count - 1);

}

double MainWindow::decisi(int a , int b)
{
    int x = 0, y = 0;
    bool jud = 0;

    for( int i = 0 ; i <= b ; i++ )           //有无最外圈（）
        if(cont[i].b == 4)
        {
            x=i;
            jud = 1;
        }
    for( int i = b ; i >= 0 ; i-- )
        if( cont[i].b == 5) y = i;

    if( jud == 1 ) decisi( x , y );

    for(int i = a ; i <= b ; i++ )
    {
        if( cont[i].b == 0) continue;

        if ( cont[i].b == 2 && cont[i].a == 6)  //.处理
        {
            int te = cont[i+1].a;
            //qDebug() << cont[i+1].a <<endl;
            int l;
            for(l = 0; te > 0 ; l++)
                te/=10;
            //qDebug() << l <<endl;
            for( ; l>0 ; l--)
                cont[i+1].a /= 10;
            cont[i-1].a += cont[i+1].a;
            cont[i+1].b = 0;
            cont[i].b = 0;
            qDebug() << cont[i-1].a <<endl;
        }


    }
}

bool MainWindow::judge_line2()
{
    return 1;
}

void MainWindow::trans()                           //字符串转换
{
    int t=0,mat=10;

    for( int i=0 ; i< sc_count ;i++ ,mat =10 ,t=0 )
    {

        if( show_sc[i] == '[' )                                   //非十进制
        {
            for(i++,mat=0 ; show_sc[i] != ']' ; i++)
            {
                mat*=10;
                QChar ch = show_sc.QString::at(i);
                mat+=ch.unicode() - '0';
            }
            //qDebug() << mat << endl;
            i++ ;
        }
        if ( i<= sc_count && ((show_sc[i]>='0' && show_sc[i] <= '9')||(show_sc[i]>= 'A' && show_sc[i] <= 'F' )) )  //数值转换
        {
        for( ; i<= sc_count && ((show_sc[i]>='0' && show_sc[i] <= '9')||(show_sc[i] >= 'A' && show_sc[i] <= 'F' ));i++)
        {

            if(show_sc[i]>='0' && show_sc[i] <= '9')
            {
                t*=mat;
                QChar ch = show_sc.QString::at(i);
                t+=ch.unicode() - '0';
            }
            if( show_sc[i]>= 'A' && show_sc[i] <= 'F')
            {
                t*=mat;
                QChar ch = show_sc.QString::at(i);
                t+=ch.unicode() - 'A'+10;
            }
        }
        i--;
        cont[bu_count].a = t;
        cont[bu_count++].b = 1;
        //qDebug() << t << endl;
        }
        else                                                 //符号对应数值
        {
            if( show_sc[i] == '+'){
                    cont[bu_count].a = 1;
                    cont[bu_count++].b = 2; }
            if( show_sc[i] == '-'){
                    cont[bu_count].a = 2;
                    cont[bu_count++].b = 2; }
            if( show_sc[i] == '*'){
                    cont[bu_count].a = 3;
                    cont[bu_count++].b = 2; }
            if( show_sc[i] == '/'){
                    cont[bu_count].a = 4;
                    cont[bu_count++].b = 2; }
            if( show_sc[i] == '^'){
                    cont[bu_count].a = 5;
                    cont[bu_count++].b = 2; }
            if( show_sc[i] == '.'){
                    cont[bu_count].a = 6;
                    cont[bu_count++].b = 2; }
            if( show_sc[i] == 's' && show_sc[i+1] == 'q'){   //sqrt
                    cont[bu_count].a = 1;
                    cont[bu_count++].b = 3;
                    i+=3; }
            if( show_sc[i] == 's' && show_sc[i+1] == 'i'){   //sin
                    cont[bu_count].a = 2;
                    cont[bu_count++].b = 3;
                    i+=2; }
            if( show_sc[i] == 'c' && show_sc[i+1] == 'o'){   //cos
                    cont[bu_count].a = 3;
                    cont[bu_count++].b = 3;
                    i+=2; }
            if( show_sc[i] == 't' && show_sc[i+1] == 'a'){   //tan
                    cont[bu_count].a = 4;
                    cont[bu_count++].b = 3;
                    i+=2; }
            if( show_sc[i] == 'l' && show_sc[i+1] == 'g'){   //lg
                    cont[bu_count].a = 5;
                    cont[bu_count++].b = 3;
                    i++;  }
            if( show_sc[i] == 'l' && show_sc[i+1] == 'n'){   //ln
                    cont[bu_count].a = 6;
                    cont[bu_count++].b = 3;
                    i++;  }
            if( show_sc[i] == '('){
                    cont[bu_count++].b = 4;  }
            if( show_sc[i] == ')'){
                    cont[bu_count++].b = 5;  }

        }

    }
    for( int ii=0 ; ii < bu_count ; ii++) qDebug() << cont[ii].a<< " "<<cont[ii].b <<endl;
}

bool MainWindow::judge_line1()                       //第一次判断
{
    int l1=0,l2=0,r1=0,r2=0;

    for(int i=0 ; i<sc_count ;i++)                  //括号对称
    {
        if(show_sc[i] == '(') l1++;
        if(show_sc[i] == '[') l2++;
        if(show_sc[i] == ')') r1++;
        if(show_sc[i] == ']') r2++;
    }

    if(l1 != r1 || l2 != r2) return 0;

    for(int i=0 ; i<sc_count ;i++)
    {
        if ( show_sc[i] == '[')                      // [进制] 问题
        {
            if ( show_sc[i+1] == ']') return 0;      //[]
            if ( i > 0 && show_sc[i-1] == '.') return 0;      //[x]3.[x]3
            for(i++ ; show_sc[i] != ']'; i++)        //[+-^]
            {
                if(show_sc[i] < '0' || show_sc[i] > '9') return 0;
            }
        }
    }
    return 1;
}

void MainWindow::count()
{

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //button settings
    connect(ui->pushButton_55, &QPushButton::clicked, [=] () mutable {     //AC
        show_sc = "0";
        sc_count = 0;
        statu = 0;
        bu_count = 0;
        ui->lineEdit->setText(QString(show_sc));
        MainWindow::start_count ();
    }  );

    if ( statu == 0 )
    {
    if ( sc_count <= 90 ) {
    connect(ui->pushButton_0, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '0';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_1, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '1';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_2, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '2';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_3, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '3';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_4, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '4';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_5, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '5';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_6, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '6';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_7, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '7';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_8, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '8';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_9, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '9';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_10, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'A';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_11, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'B';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_12, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'C';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_13, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'D';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_14, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'E';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_15, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'F';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_16, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '.';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_51, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'P';
        show_sc [ sc_count++ ] = 'i';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_17, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '+';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_18, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '-';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_19, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '*';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_20, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '/';
        ui->lineEdit->setText(QString(show_sc));
    }  );
    connect(ui->pushButton_21, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '^';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_22, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 's';
        show_sc [ sc_count++ ] = 'q';
        show_sc [ sc_count++ ] = 'r';
        show_sc [ sc_count++ ] = 't';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_23, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 's';
        show_sc [ sc_count++ ] = 'i';
        show_sc [ sc_count++ ] = 'n';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_24, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'c';
        show_sc [ sc_count++ ] = 'o';
        show_sc [ sc_count++ ] = 's';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_25, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 't';
        show_sc [ sc_count++ ] = 'a';
        show_sc [ sc_count++ ] = 'n';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_26, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '(';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_27, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = ')';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_28, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'l';
        show_sc [ sc_count++ ] = 'n';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_29, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'l';
        show_sc [ sc_count++ ] = 'g';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_34, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = 'e';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_41, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '[';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_42, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = ']';
        ui->lineEdit->setText(QString(show_sc));
    } );
    connect(ui->pushButton_52, &QPushButton::clicked, [=] () mutable {
        show_sc [ sc_count++ ] = '-';
        ui->lineEdit->setText(QString(show_sc));
    } );

    }
    connect(ui->pushButton_50, &QPushButton::clicked, [=] () mutable {     //一般=
        ui->label->setText(QString(show_sc));
        MainWindow::start_count ();
    }  );
    }

    //screen settings
}


MainWindow::~MainWindow ()
    {
        delete ui;
    }











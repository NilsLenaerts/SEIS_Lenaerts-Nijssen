#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextEdit>
#include <iostream>
#include "ByteStream.h"
#include "Lexer.h"
#include "Parser.h"
#include "Compiler.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->txtInput->setText("test");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    ui->txtOutput->setText("Compiling ... Please wait.");
    QString inputString = ui->txtInput->toPlainText();
    //here we need the application.
    std::string input = inputString.toStdString();
    Lexer lexer{ input };
     std::cout << "Lexing done \n\n\n\n";
     Parser parser{ lexer };
     std::cout << "Parsing done \n\n\n\n";


     std::vector<Instruction> parsed = parser.parse();
     if (parsed.size() ==0){
          ui->txtOutput->setText("You either have to many or to few brackets in your code.");
     }else {
     Compiler compiler{parsed  };
     std::cout << "Compiling done \n\n\n\n";
     std::string result= compiler.getWasmString();
     ui->txtOutput->setText(QString::fromStdString(result));

     }






}


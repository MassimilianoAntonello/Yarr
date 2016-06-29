#include "editcfgdialog.h"
#include "ui_editcfgdialog.h"

EditCfgDialog::EditCfgDialog(Fei4 * f, QString cfgFNJ_param, QWidget * parent) :
    QDialog(parent),
    ui(new Ui::EditCfgDialog),
    fE(f),
    cfgFNJ(cfgFNJ_param)
{
    ui->setupUi(this);

    nlohmann::json j;
    this->fE->toFileJson(j);
    std::string s;
    s = j.dump(4);
    ui->cfgEdit->setPlainText(QString::fromStdString(s));
}

EditCfgDialog::~EditCfgDialog(){
    delete ui;
}

void EditCfgDialog::on_saveButton_clicked(){
    nlohmann::json j;
    try{
        j = nlohmann::json::parse(ui->cfgEdit->toPlainText().toStdString());
    }
    catch(std::invalid_argument){
        std::cerr << "ERROR - Malformed argument - aborting" << std::endl;
        return;
    }
    try{
        this->fE->fromFileJson(j);
    }
    catch(std::domain_error){
        std::cerr << "ERROR - Malformed argument - aborting" << std::endl;
        return;
    }
    QFile oF(this->cfgFNJ);
    oF.open(QIODevice::WriteOnly);
    QString qS;
    qS = QString::fromStdString(j.dump(4));
    QTextStream oS(&oF);
    oS << qS;
    oF.close();

    return;
}

void EditCfgDialog::on_applyButton_clicked(){
    nlohmann::json j;
    try{
        j = nlohmann::json::parse(ui->cfgEdit->toPlainText().toStdString());
    }
    catch(std::invalid_argument){
        std::cerr << "ERROR - Malformed config - aborting" << std::endl;
        return;
    }

    try{
        this->fE->fromFileJson(j);
    }
    catch(std::domain_error){
        std::cout << "ERROR - Malformed config - aborting" << std::endl;
        return;
    }

    return;
}

void EditCfgDialog::on_saveAsButton_clicked(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Select JSON config file"), "./util/", tr("JSON Config File(*.js *.json)"));
    nlohmann::json j;
    try{
        j = nlohmann::json::parse(ui->cfgEdit->toPlainText().toStdString());
    }
    catch(std::invalid_argument){
        std::cerr << "ERROR - Malformed config - aborting" << std::endl;
        return;
    }
    QFile oF(filename);
    oF.open(QIODevice::WriteOnly);
    QString qS;
    qS = QString::fromStdString(j.dump(4));
    if(qS.at(qS.size()-1) != '\n'){
        qS.append("\n");
    }
    QTextStream oS(&oF);
    oS << qS;
    oF.close();

    return;
}

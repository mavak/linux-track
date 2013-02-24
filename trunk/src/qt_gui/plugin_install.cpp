#include <plugin_install.h>
#include <ltr_gui_prefs.h>
#include <iostream>
#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>
#include <zlib.h>
#include "extractor.h"
#include "utils.h"


PluginInstall::PluginInstall(const Ui::LinuxtrackMainForm &ui):
  gui(ui), inst(NULL), dlfw(NULL),
  poem1(PREF.getRsrcDirPath() + "/tir_firmware/poem1.txt"),
  poem2(PREF.getRsrcDirPath() + "/tir_firmware/poem2.txt"), 
  gameData(PREF.getRsrcDirPath() + "/tir_firmware/gamedata.txt")
{
  if(QFile::exists(PREF.getDataPath("linuxtrack-wine.exe"))){
    inst = new WineLauncher();
    gui.pushButton_2->setEnabled(true);
    Connect();
  }
}

PluginInstall::~PluginInstall()
{
  if(dlfw != NULL){
    delete dlfw;
    dlfw = NULL;
  }
  delete inst;
}

void PluginInstall::Connect()
{
  QObject::connect(gui.pushButton_2, SIGNAL(pressed()),
    this, SLOT(installWinePlugin()));
  QObject::connect(inst, SIGNAL(finished(bool)),
    this, SLOT(instFinished(bool)));
}

void PluginInstall::instFinished(bool result)
{
  if(!result){
    QMessageBox::warning(NULL, "Wine bridge installation problem", 
      "Wine bridge installation failed!", QMessageBox::Ok);
    return;
  }
  if(!isTirFirmwareInstalled()){
    printf("Key source not existing!\n");
    tirFirmwareInstall();
  }else{
    printf("We have key source, continue with extraction...\n");
    tirFirmwareInstalled(true);
  }
}

void PluginInstall::tirFirmwareInstall()
{
  if(dlfw == NULL){
    dlfw = new Extractor();
    QObject::connect(dlfw, SIGNAL(finished(bool)),
      this, SLOT(tirFirmwareInstalled(bool)));
  }
  dlfw->show();
}


bool PluginInstall::isTirFirmwareInstalled()
{
  return QFile::exists(poem1) && QFile::exists(poem2) && QFile::exists(gameData);
}

void PluginInstall::tirFirmwareInstalled(bool ok)
{
  (void) ok;
  if(isTirFirmwareInstalled()){
    //All is fine...
  }else{
    QMessageBox::warning(NULL, "TrackIR firmware install problem", 
"TrackIR firmware package was not installed, without it\n\
the linuxtrack-wine bridge will not be fully functional!", QMessageBox::Ok);
  }
  gui.pushButton_2->setEnabled(true);
}

void PluginInstall::installWinePlugin()
{
  gui.pushButton_2->setEnabled(false);
  QString prefix = QFileDialog::getExistingDirectory(NULL, QString("Select Wine Prefix..."), 
                     QDir::homePath()+"/.wine", QFileDialog::ShowDirsOnly);
  QString installerPath = PREF.getDataPath("linuxtrack-wine.exe");
  
  inst->setEnv("WINEPREFIX", prefix);
  inst->run(installerPath);
}



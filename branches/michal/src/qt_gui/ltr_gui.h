#ifndef LTR_GUI__H
#define LTR_GUI__H

#include "pref_int.h"
#include "ui_ltr.h"
#include "webcam_prefs.h"
#include "wiimote_prefs.h"

class LinuxtrackGui : public QWidget
{
  Q_OBJECT
 public:
  LinuxtrackGui(QWidget *parent = 0);
 private slots:
  void on_QuitButton_pressed();
//  void on_WebcamIDs_currentIndexChanged(const QString &text);
//  void on_WebcamFormats_currentIndexChanged(const QString &text);
//  void on_WebcamResolutions_currentIndexChanged(const QString &text);
  void on_DeviceSelector_currentIndexChanged(int index);
  void on_RefreshDevices_pressed();
 private:
  Ui::LinuxtrackMainForm ui;
  WebcamPrefs *wcp;
  WiimotePrefs *wiip;
  pref_id dev_selector;
};


#endif
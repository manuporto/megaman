#ifndef CLIENT_ABOUTSCREEN_H_
#define CLIENT_ABOUTSCREEN_H_

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>

#include "MainWindow.h"

class MainWindow;

class AboutScreen {
private:
	MainWindow* main_w;
	Gtk::Box* screen;
public:
	explicit AboutScreen(MainWindow* main_w);
	void on_exit_btn_clicked();
	Gtk::Box* get_screen();
	virtual ~AboutScreen();
};

#endif /* CLIENT_ABOUTSCREEN_H_ */

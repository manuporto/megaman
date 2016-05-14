#ifndef GTKMM_MAINWINDOW_H
#define GTKMM_MAINWINDOW_H

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <string>

#include "common/Socket.h"

class MainWindow: public Gtk::Window {
public:
	MainWindow(const char* hostname, const char* port);
	virtual ~MainWindow();

protected:
	Gtk::Box main_frame;
	Gtk::Box* welcome_screen;
	//Gtk::Button nw_game_btn;
	//Gtk::Button exit_btn;
	//Gtk::Label game_name_lbl;

	//Signal Handlers
	void on_new_game_btn_clicked();
	void on_exit_game_btn_clicked();

private:
	typedef Glib::RefPtr<Gtk::Builder> GtkBuilder;
	const char* hostname;
	const char* port;
	Socket skt;
	void init_welcome_screen();
	GtkBuilder load_glade_file(std::string filename, Gtk::Box** container);
};

#endif  // GTKMM_MAINWINDOW_H

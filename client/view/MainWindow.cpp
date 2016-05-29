#include <glibmm/fileutils.h>
#include <glibmm/markup.h>
#include <iostream>
#include <string>
#include <gtkmm/entry.h>
#include <gtkmm/entrybuffer.h>

#include "MainWindow.h"
#include "GladeLoader.h"
#include "SignalProtocol.h"
#include "common/communication/Packet.h"
#include "StageRenderer.h"

#include <gtkmm/main.h>
#include <gtkmm/alignment.h>
#include <gtkmm/box.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/frame.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/table.h>
#include <gtkmm/window.h>


#define CONTAINER_NAME "container"

MainWindow::MainWindow(const char* hostname, const char* port) :
        bg_image("resources/background.png"), client(hostname, port) {
	set_title("Mega Man");
	set_size_request(640, 480);
    set_position(Gtk::WIN_POS_CENTER);
	set_border_width(0);
	layout.put(bg_image, 0, 0);
	init_welcome_screen();
    init_insert_name();
    init_stage_pick_screen();
    main_frame.pack_start(*welcome_screen);
    main_frame.pack_start(*insert_name);
    main_frame.pack_start(*stage_pick);
	//main_frame.pack_start(stage);
    add(main_frame);
	//layout.put(*welcome_screen, 0, 0);
	//add(layout);
	show_all();
    insert_name->hide();
    stage_pick->hide();
	//stage.hide();
}


// WELCOME SCREEN
void MainWindow::init_welcome_screen() {
	welcome_screen = NULL;
	GladeLoader::ScreenBuilder builder = GladeLoader::load_glade_file(
			"resources/welcome_screen.glade", &welcome_screen);
	Gtk::Button* btn = NULL;
	builder->get_widget("start_game_btn", btn);
	if (btn) {
		btn->signal_clicked().connect(
				sigc::mem_fun(*this, &MainWindow::on_new_game_btn_clicked));
	}
	builder->get_widget("about_btn", btn);
	if (btn) {
		btn->signal_clicked().connect(
				sigc::mem_fun(*this, &MainWindow::on_about_btn_clicked));
	}
	builder->get_widget("exit_btn", btn);
	if (btn) {
		btn->signal_clicked().connect(
				sigc::mem_fun(*this, &MainWindow::on_exit_game_btn_clicked));
	}
}

void MainWindow::on_new_game_btn_clicked() {
	std::cout << std::endl;
    welcome_screen->hide();
    insert_name->show();
	//layout.remove(*welcome_screen);
	//init_insert_name();
	//layout.put(*insert_name, 0, 0);
}

void MainWindow::on_about_btn_clicked() {}

void MainWindow::on_exit_game_btn_clicked() {
	std::cout << "Exit game" << std::endl;
	unset_application();
}

// INSERT NAME SCREEN
void MainWindow::init_insert_name() {
	insert_name = NULL;
	GladeLoader::ScreenBuilder builder = GladeLoader::load_glade_file(
			"resources/insert_name.glade", &insert_name);
	Gtk::Button* btn = NULL;
	Gtk::Entry* entry = NULL;
	builder->get_widget("name_entry", entry);
	builder->get_widget("confirm_name_btn", btn);
	if (btn) {
		btn->signal_clicked().connect(
				sigc::bind<Gtk::Entry*>(
						sigc::mem_fun(*this,
									  &MainWindow::on_confirm_name_btn_clicked),
						entry));
	}
	builder->get_widget("cancel_btn", btn);
	if (btn) {
		btn->signal_clicked().connect(
				sigc::mem_fun(*this, &MainWindow::on_cancel_btn_clicked));
	}
}

void MainWindow::on_confirm_name_btn_clicked(Gtk::Entry* text_entry) {
	std::cout << "Client started" << std::endl;
	this->client.connect_to_server();
	Glib::RefPtr<const Gtk::EntryBuffer> buffer = text_entry->get_buffer();
	Glib::ustring name = buffer->get_text();
	std::cout << name.c_str() << std::endl;
	std::string sname = name.raw();
	this->client.send_name(sname);
	//init_stage_pick_screen();
	//layout.remove(*insert_name);
	//layout.put(*stage_pick, 0, 0);
    insert_name->hide();
	stage_pick->show();
}

void MainWindow::on_cancel_btn_clicked() {
	std::cout << "Going back to welcome screen" << std::endl;
	//layout.remove(*insert_name);
	//layout.put(*welcome_screen, 0, 0);
    insert_name->hide();
	welcome_screen->show();
}

// STAGE PICK SCREEN
void MainWindow::init_stage_pick_screen() {
	stage_pick = NULL;
	GladeLoader::ScreenBuilder builder = GladeLoader::load_glade_file(
			"resources/stage_pick_box.glade", &stage_pick);
	Gtk::Button* btn = NULL;
	builder->get_widget("bombman_btn", btn);
	if (btn) {
		btn->signal_clicked().connect(
				sigc::bind<char>(
						sigc::mem_fun(*this,
								&MainWindow::on_boss_pick_btn_clicked), BOMBMAN));
	}
}

void MainWindow::on_boss_pick_btn_clicked(char stage_id) {
	std::cout << "Boss selected" << std::endl;
	//client.pick_stage(stage_id);
    stage_pick->hide();
    //stage.show();
    surface = new StageSurface();
    surface->run();
}

MainWindow::~MainWindow() { }

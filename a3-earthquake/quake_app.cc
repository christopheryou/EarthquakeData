/** CSci-4611 Assignment 3:  Earthquake
 */

#include "quake_app.h"
#include "config.h"
#include "earth.h"
#include "earthquake_database.h"
#include "date.h"
#include "earthquake.h"
#include <iostream>
#include <sstream>
#include <algorithm>

// Number of seconds in 1 year (approx.)
const int PLAYBACK_WINDOW = 12 * 28 * 24 * 60 * 60;

using namespace std;
Date prevYear;
QuakeApp::QuakeApp() : GraphicsApp(1280,720, "Earthquake"),
    playback_scale_(15000000.0), debug_mode_(false)
{
    // Define a search path for finding data files (images and earthquake db)
    search_path_.push_back(".");
    search_path_.push_back("./data");
    search_path_.push_back(DATA_DIR_INSTALL);
    search_path_.push_back(DATA_DIR_BUILD);

    
    quake_db_ = EarthquakeDatabase(Platform::FindFile("earthquakes.txt", search_path_));
    current_time_ = quake_db_.earthquake(quake_db_.min_index()).date().ToSeconds();

 }


QuakeApp::~QuakeApp() {
}


void QuakeApp::InitNanoGUI() {
    // Setup the GUI window
    nanogui::Window *window = new nanogui::Window(screen(), "Earthquake Controls");
    window->setPosition(Eigen::Vector2i(10, 10));
    window->setSize(Eigen::Vector2i(400,200));
    window->setLayout(new nanogui::GroupLayout());
    
    date_label_ = new nanogui::Label(window, "Current Date: MM/DD/YYYY", "sans-bold");
    
    globe_btn_ = new nanogui::Button(window, "Globe");
    globe_btn_->setCallback(std::bind(&QuakeApp::OnGlobeBtnPressed, this));
    globe_btn_->setTooltip("Toggle between map and globe.");
    
    new nanogui::Label(window, "Playback Speed", "sans-bold");
    
    nanogui::Widget *panel = new nanogui::Widget(window);
    panel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
                                            nanogui::Alignment::Middle, 0, 20));
    
    nanogui::Slider *slider = new nanogui::Slider(panel);
    slider->setValue(0.5f);
    slider->setFixedWidth(120);
    
    speed_box_ = new nanogui::TextBox(panel);
    speed_box_->setFixedSize(Eigen::Vector2i(60, 25));
    speed_box_->setValue("50");
    speed_box_->setUnits("%");
    slider->setCallback(std::bind(&QuakeApp::OnSliderUpdate, this, std::placeholders::_1));
    speed_box_->setFixedSize(Eigen::Vector2i(60,25));
    speed_box_->setFontSize(20);
    speed_box_->setAlignment(nanogui::TextBox::Alignment::Right);
    
    nanogui::Button* debug_btn = new nanogui::Button(window, "Toggle Debug Mode");
    debug_btn->setCallback(std::bind(&QuakeApp::OnDebugBtnPressed, this));
    debug_btn->setTooltip("Toggle displaying mesh triangles and normals (can be slow)");
    
    screen()->performLayout();

}

void QuakeApp::OnLeftMouseDrag(const Point2 &pos, const Vector2 &delta) {
    // Optional: In our demo, we adjust the tilt of the globe here when the
    // mouse is dragged up/down on the screen.
}


void QuakeApp::OnGlobeBtnPressed() {
    // TODO: This is where you can switch between flat earth mode and globe mode
}

void QuakeApp::OnDebugBtnPressed() {
    debug_mode_ = !debug_mode_;
}

void QuakeApp::OnSliderUpdate(float value) {
    speed_box_->setValue(std::to_string((int) (value * 100)));
    playback_scale_ = 30000000.0*value;
}


void QuakeApp::UpdateSimulation(double dt)  {
    // Advance the current time and loop back to the start if time is past the last earthquake
    current_time_ += playback_scale_ * dt;
    if (current_time_ > quake_db_.earthquake(quake_db_.max_index()).date().ToSeconds()) {
        current_time_ = quake_db_.earthquake(quake_db_.min_index()).date().ToSeconds();
    }
    if (current_time_ < quake_db_.earthquake(quake_db_.min_index()).date().ToSeconds()) {
        current_time_ = quake_db_.earthquake(quake_db_.max_index()).date().ToSeconds();
    }
    
    Date d(current_time_);
    stringstream s;
    s << "Current date: " << d.month()
        << "/" << d.day()
        << "/" << d.year();
    date_label_->setCaption(s.str());
    
    // TODO: Any animation, morphing, rotation of the earth, or other things that should
    // be updated once each frame would go here.
}


void QuakeApp::InitOpenGL() {
    // Set up the camera in a good position to see the entire earth in either mode
    proj_matrix_ = Matrix4::Perspective(60, aspect_ratio(), 0.1, 50);
    view_matrix_ = Matrix4::LookAt(Point3(0,0,3.5), Point3(0,0,0), Vector3(0,1,0));
    glClearColor(0.0, 0.0, 0.0, 1);
    
    // Initialize the earth object
    earth_.Init(search_path_);
    // Initialize the texture used for the background image
    stars_tex_.InitFromFile(Platform::FindFile("iss006e40544.png", search_path_));
}


void QuakeApp::DrawUsingOpenGL() {
    quick_shapes_.DrawFullscreenTexture(Color(1,1,1), stars_tex_);
    
    // You can leave this as the identity matrix and we will have a fine view of
    // the earth.  If you want to add any rotation or other animation of the
    // earth, the model_matrix is where you would apply that.
    Matrix4 model_matrix;
    // Draw the earth
    earth_.Draw(model_matrix, view_matrix_, proj_matrix_);
    if (debug_mode_) {
        earth_.DrawDebugInfo(model_matrix, view_matrix_, proj_matrix_);
    }
	//Find most recent quake and its stats
	int recentQuake = quake_db_.FindMostRecentQuake(current_time_);
	Earthquake earthq = quake_db_.earthquake(recentQuake);
	Point3 location = earth_.LatLongToPlane(earthq.latitude(), earthq.longitude());
	//Set color based on earth's magnitude
	//Set matrix 4 info as needed
	Color col(earthq.magnitude() - 6.5, 0.27, 0); //Color should vary based on magnitude
	Matrix4 spot =
		Matrix4::Translation(location - Point3(0, 0, 0)) *
		Matrix4::Scale(Vector3(0.075f, 0.075f, 0.075f) * 1.0f *(earthq.magnitude() / 7.5f));

	//Check if we've already stored that data.
	bool inQuakes = false;
	for (int i = 0; i < quakes.size(); i++) {
		if (quakes[i].date().year() == earthq.date().year()) {
			if (quakes[i].date().month() == earthq.date().month()) {
				if (quakes[i].date().day() == earthq.date().day()) {
					if (quakes[i].longitude() == earthq.longitude()) {
						if (quakes[i].latitude() == earthq.latitude()) {
							if (quakes[i].magnitude() == earthq.magnitude()) {
								inQuakes = true;
							}
						}
					}
				}
			}
		}
	}
	//If we haven't then we add it to our list of quakes.
	if (!inQuakes) {
		quakes.push_back(earthq);
		oneyear.push_back(spot);
		colors.push_back(col);
	}

	//Check if the map has reset to 1900.
	Date d(current_time_);
	if (d.year() < prevYear.year()) {
		quakes.erase(quakes.begin(), quakes.end());
		oneyear.erase(oneyear.begin(), oneyear.end());
		colors.erase(colors.begin(), colors.end());

	}

	int curDate = d.day() + (d.year() * 365) + (d.month() * 30);
	for (int i = 0; i < oneyear.size(); i++) {
		//Check if it has been a year
		//If so, delete the quake.
		int date = quakes[i].date().day() + (quakes[i].date().year() * 365) + (quakes[i].date().month() * 30);
		int dateDif = curDate - date;
		if (dateDif >= 365) {
			quakes.erase(quakes.begin() + i);
			oneyear.erase(oneyear.begin() + i);
			colors.erase(colors.begin() + i);
		}
		else 
		{
			//Otherwise draw it
			//Decrease its size
			quick_shapes_.DrawSphere(model_matrix * oneyear[i], view_matrix_, proj_matrix_, colors[i]);
			location = earth_.LatLongToPlane(quakes[i].latitude(), quakes[i].longitude());
			spot = Matrix4::Translation(location - Point3(0, 0, 0)) *
				Matrix4::Scale(Vector3(0.075f, 0.075f, 0.075f) * 1.0f *(quakes[i].magnitude() / 7.5f)* ((365.0f - dateDif) / 365.0f));
			oneyear[i] = spot;
		}
	}	
	// KEeps track of previous year
	prevYear = d;

    // TODO: You'll also need to draw the earthquakes.  It's up to you exactly
    // how you wish to do that.

	//We will want to vary the size of the spheres
	//We will want to shrink them gradually until one year from their spawn point
	//We will want to to vary the color
	//Color can remain the same -- does not need to gradually change

	//Earthquake fields:
	/*
    Date date();

    double longitude();

    double latitude();

    double magnitude();*/
}





#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(39);
	ofSetColor(255);

	this->word = "JunKiyoshi.com";

	this->font_size = 30;
	this->font.loadFont("fonts/Kazesawa-Bold.ttf", this->font_size, true, true, true);
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = this->log_list.size() - 1; i >= 0; i--) {

		if (this->log_list[i].back().y < -this->font_size || this->log_list[i].back().y > ofGetWindowHeight() + this->font_size) {

			this->log_list.erase(this->log_list.begin() + i);
			this->seed_list.erase(this->seed_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->index_list.erase(this->index_list.begin() + i);

			continue;
		}

		auto feature = this->velocity_list[i] * 35;
		auto deg = ofMap(ofNoise(this->seed_list[i], ofGetFrameNum() * 0.005), 0, 1, 0, 360);
		feature += glm::vec2(30 * cos(deg * DEG_TO_RAD), 30 * sin(deg * DEG_TO_RAD));
		auto next = this->log_list[i].back() + glm::normalize(feature) * 15;

		this->log_list[i].push_back(next);

		while (this->log_list[i].size() > 20) {

			this->log_list[i].erase(this->log_list[i].begin());
		}
	}

	if(ofGetFrameNum() % 1 == 0) {

		auto y = ofMap(sin(ofGetFrameNum() * 0.05), -1, 1, this->font_size, ofGetWindowHeight() - this->font_size);
		auto next_y = ofMap(sin((ofGetFrameNum() + 1) * 0.05), -1, 1, this->font_size, ofGetWindowHeight() - this->font_size);

		auto location = glm::vec2(360, y);
		auto next = glm::vec2(360, next_y);

		vector<glm::vec2> log;
		log.push_back(location);
		this->log_list.push_back(log);

		this->seed_list.push_back(ofMap(ofNoise(location * 0.001), 0, 1, 0, 1000));

		this->velocity_list.push_back(glm::normalize(location - next) * 2);
		this->index_list.push_back(int(ofGetFrameNum() * 0.5) % this->word.size());

	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	int sample_count = 180;
	ofBeginShape();
	for (int i = 0; i < this->log_list.size(); i++) {

		char c = this->word[this->index_list[i]];
		ofPath path = this->font.getCharacterAsPoints(c, true, false);

		vector<ofPolyline> outline = path.getOutline();

		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			outline_index != 0 ? ofNextContour(true) : ofNextContour(false);

			auto vertices = outline[outline_index].getResampledByCount(sample_count).getVertices();

			for (auto& vertex : vertices) {

				ofVertex(this->log_list[i].back() + vertex);
			}
		}
	}
	ofEndShape(true);

	/*
	int start = 150;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
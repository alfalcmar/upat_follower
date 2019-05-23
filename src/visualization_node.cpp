//----------------------------------------------------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018 Hector Perez Leon
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
// Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, sEXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
// OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//----------------------------------------------------------------------------------------------------------------------

#include <upat_follower/visualization.h>

int main(int _argc, char **_argv) {
    ros::init(_argc, _argv, "visualization_node");
    int pub_rate_;
    ros::param::param<int>("~pub_rate", pub_rate_, 30);

    Visualization visual;
    visual.save_data = false;
    if (visual.save_data) {
        std::string pkg_name_path = ros::package::getPath("upat_follower");
        std::string folder_data_name = pkg_name_path + "/tests/data/plot/";
        visual.csv_normal_distances_.open(folder_data_name + "normal_dist.csv");
        visual.csv_current_path_.open(folder_data_name + "current_path.csv");
    }
    double start_time;
    bool do_once = true;
    ros::Rate rate(pub_rate_);
    while (ros::ok()) {
        visual.pubMsgs();
        if (visual.current_path_.poses.size() > 0) {
            if (visual.ual_state_.state == 4) {
                if (visual.save_data == true) {
                    visual.saveMissionData();
                }
                if (do_once) {
                    start_time = ros::Time::now().toSec();
                    do_once = false;
                }
            } else {
                if(!do_once){
                    ROS_INFO("Time: %f", ros::Time::now().toSec() - start_time);
                    do_once = true;
                }
            }
        }
        ros::spinOnce();
        rate.sleep();
    }

    if (visual.save_data) {
        for (int i = 0; visual.current_path_.poses.size(); i++) {
            visual.csv_current_path_
                << visual.current_path_.poses.at(i).pose.position.x << ", "
                << visual.current_path_.poses.at(i).pose.position.y << ", "
                << visual.current_path_.poses.at(i).pose.position.z << std::endl;
        }
        visual.csv_normal_distances_.close();
        visual.csv_current_path_.close();
    }

    return 0;
}